/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#include <FatInst.h>
#include <disk.h>
#include <bootrec.h>
#include <string.h>
#include <install.h>
#include <memory.h>

CFatInstall::CFatInstall(CTextUI &TextUIToUse, CXoslFiles &XoslFilesToUse, CDosFile &DosFileToUse):
	TextUI(TextUIToUse),
	XoslFiles(XoslFilesToUse),
	DosFile(DosFileToUse)
{

}

CFatInstall::~CFatInstall()
{

}

int CFatInstall::CreateIpl(const CDosDriveList::CDosDrive &DosDrive, TIPL &Ipl)
{
	CDiskAccess DiskAccess;
	int UseLba;

	TextUI.OutputStr("Initializing IPL...");
	// clear the 10 'reserved' bytes
	MemSet(Ipl.IPLFAT16Conv.Reserved,0,10);

	UseLba = DiskAccess.LBAAccessAvail(DosDrive.Drive) == 0;
	if (DosDrive.FATType == FATTYPE_FAT16)
		return CreateIplFat16(DosDrive,UseLba,Ipl);
	return CreateIplFat32(DosDrive,UseLba,Ipl);
}

int CFatInstall::CreateIplFat16(const CDosDriveList::CDosDrive &DosDrive, int UseLba, TIPL &Ipl)
{
	TIPLFAT16Conv &Fat16Ipl = Ipl.IPLFAT16Conv;
	TBootFAT16 Fat16;
	const char *IplFile;
	int fh;
	CDisk Disk;

	Disk.Map(DosDrive.Drive,DosDrive.StartSector);
	Disk.Read(0,&Fat16,1);


	if (!UseLba)
		IplFile = XoslFiles.GetIplFileName(CXoslFiles::enumIpl16Conv);
	else
		IplFile = XoslFiles.GetIplFileName(CXoslFiles::enumIpl16Lba);

	if ((fh = DosFile.Open(IplFile,CDosFile::accessReadOnly)) == -1) {
		TextUI.OutputStr("failed\nUnable to open %s",IplFile);
		return -1;
	}
	DosFile.Read(fh,&Fat16Ipl,sizeof (TIPLFAT16Conv) - 10);
	DosFile.Close(fh);

	if (!UseLba) {
		Fat16Ipl.IPLData.DiskSectors = Fat16.TrackSize;
		Fat16Ipl.IPLData.DiskHeads = Fat16.HeadCount;
	}
	Fat16Ipl.IPLData.RootEntries = Fat16.RootEntries;
	Fat16Ipl.IPLData.ClusterSectSize = Fat16.ClusterSize;
	Fat16Ipl.IPLData.ClusterByteSize = (unsigned short)Fat16.ClusterSize << 9;
	Fat16Ipl.IPLData.FATStart = Fat16.ReservedSectors;
	Fat16Ipl.IPLData.DirStart = (long)Fat16.ReservedSectors + (long)Fat16.FATCopies * (long)Fat16.FATSize;
	Fat16Ipl.IPLData.DataStart = (long)Fat16Ipl.IPLData.DirStart + (long)Fat16.RootEntries / 16;

	Fat16Ipl.IPLData.FSType = 0x06;
	Fat16Ipl.IPLData.DriveNumber = DosDrive.Drive;
	Fat16Ipl.IPLData.ABSSectorStart = DosDrive.StartSector;
	TextUI.OutputStr("done\n");
	return 0;
}

int CFatInstall::CreateIplFat32(const CDosDriveList::CDosDrive &DosDrive, int UseLba, TIPL &Ipl)
{
	TIPLFAT32Conv &Fat32Ipl = Ipl.IPLFAT32Conv;
	TBootFAT32 Fat32;
	const char *IplFile;
	int fh;
	long FATSize;
	CDisk Disk;

	Disk.Map(DosDrive.Drive,DosDrive.StartSector);
	Disk.Read(0,&Fat32,1);

	if (!UseLba)
		IplFile = XoslFiles.GetIplFileName(CXoslFiles::enumIpl32Conv); 
	else
		IplFile = XoslFiles.GetIplFileName(CXoslFiles::enumIpl32Lba);

	if ((fh = DosFile.Open(IplFile,CDosFile::accessReadOnly)) == -1) {
		TextUI.OutputStr("failed\nUnable to op %s",IplFile);
		return -1;
	}
	DosFile.Read(fh,&Fat32Ipl,sizeof (TIPLFAT32Conv) - 10);
	DosFile.Close(fh);

	if (!UseLba) {
		Fat32Ipl.IPLData.DiskSectors = Fat32.TrackSize;
		Fat32Ipl.IPLData.DiskHeads = Fat32.HeadCount;
	}
	Fat32Ipl.IPLData.ClusterSectSize = Fat32.ClusterSize;
	Fat32Ipl.IPLData.ClusterByteSize = (unsigned short)Fat32.ClusterSize << 9;
	Fat32Ipl.IPLData.FATStart = Fat32.ReservedSectors;
	FATSize = Fat32.FATSize ? Fat32.FATSize : Fat32.BigFATSize;
	Fat32Ipl.IPLData.DataStart = (long)Fat32.ReservedSectors + (long)Fat32.FATCopies * FATSize;
	Fat32Ipl.IPLData.RootCluster = Fat32.RootCluster;

	Fat32Ipl.IPLData.FSType = 0x0b;
	Fat32Ipl.IPLData.DriveNumber = DosDrive.Drive;
	Fat32Ipl.IPLData.ABSSectorStart = DosDrive.StartSector;
	TextUI.OutputStr("done\n");
	return 0;
}

int CFatInstall::InstallFiles(const CDosDriveList::CDosDrive &DosDrive)
{
	char DestFile[32];
	const char *SrcFile;
	int Index, Count;

	*DestFile = DosDrive.DriveChar;
	*(unsigned short *)&DestFile[1] = 0x5c3a; // ':\'


	Count = XoslFiles.GetCount();
	for (Index = 0; Index < Count; ++Index) {
		strcpy(&DestFile[3],XoslFiles.GetFileName(Index));
		SrcFile = XoslFiles.GetFileName(Index);
		TextUI.OutputStr("Copying %s...",SrcFile);
		if (DosFile.SetAttrib(DestFile,0) != -1)
			if (DosFile.Delete(DestFile) == -1) {
				TextUI.OutputStr("failed\nUnable to delete existing %s\n",SrcFile);
				return -1;
			}
		if (DosFile.Copy(SrcFile,DestFile) == -1) {
			TextUI.OutputStr("failed\nUnabled to copy %s\n",SrcFile);
			return -1;
		}
		DosFile.SetAttrib(DestFile,CDosFile::attrHidden);
		TextUI.OutputStr("done\n");
	}
	return 0;
}



int CFatInstall::InstallIpl(void *Ipl)
{
	CDisk Disk;
	unsigned char Mbr[512];
	unsigned short Checksum;
	int Index;

	TextUI.OutputStr("Installing IPL...");
	if (Disk.Map(0x80,0) == -1) {
		TextUI.OutputStr("failed\nUnable to access MBR");
		return -1;
	}
	Disk.Lock();
	if (Disk.Read(0,Mbr,1) == -1) {
		Disk.Unlock();
		TextUI.OutputStr("failed\nUnable to read MBR");
		return -1;
	}
	MemCopy(Mbr,Ipl,446);
	if (Disk.Write(0,Mbr,1) == -1) {
		Disk.Unlock();
		TextUI.OutputStr("failed\nUnable to write to MBR");
		return -1;
	}

	Disk.Unlock();
	TextUI.OutputStr("done\n");
	return 0;

}


void CFatInstall::RemoveXoslFiles(char DosDriveChar)
{
	char FileStr[32];
	const char *XoslFileName;
	int Index, Count;

	FileStr[0] = DosDriveChar;
	FileStr[1] = ':';
	FileStr[2] = '\\';
	Count = XoslFiles.GetCount();
	for (Index = 0; Index < Count; ++Index) {
		XoslFileName = XoslFiles.GetFileName(Index);
		TextUI.OutputStr("Removing %s...",XoslFileName);
		strcpy(&FileStr[3],XoslFileName);
		if (DosFile.Delete(FileStr) == -1)
			TextUI.OutputStr("failed\n");
		else
			TextUI.OutputStr("done\n");
	}		
}
