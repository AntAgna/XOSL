/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */



#include <Install.h>

#include <xosldata.h>
#include <memory.h>
#include <disk.h>
#include <fat16.h>
#include <string.h>
#include <text.h>
#include <key.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>


CInstaller::CInstaller(CTextUI &TextUIToUse, CPartList &PartListToUse):
	TextUI(TextUIToUse),
	PartList(PartListToUse),
	FatInstall(TextUIToUse,XoslFiles,DosFile),
	FsCreator(TextUIToUse,XoslFiles,DosFile)
{
}

CInstaller::~CInstaller()
{
}

int CInstaller::Install(CVesa::TGraphicsMode GraphicsMode, CMouse::TMouseType MouseType, const CDosDriveList::CDosDrive &DosDrive, bool PartMan, bool SmartBm)
{
	TIPL Ipl;

	if (!PartMan) {
		XoslFiles.IgnorePartManFiles();
	}
	if (CreateXoslData(GraphicsMode,MouseType) == -1)
		return -1;
	if (CreateBootItem() == -1)
		return -1;
	if (BackupOriginalMbr(0,XoslFiles.GetOriginalMbrName()) == -1)
		return -1; 

	if (SmartBm) {
		InstallSmartBootManager(DosDrive.Drive);
	}
	if (BackupOriginalMbr(-1,XoslFiles.GetSmartBmName()) == -1) {
		return -1;
	}

	if (FatInstall.CreateIpl(DosDrive,Ipl) == -1)
		return -1;

	if (BackupCurrentMbr(&Ipl) == -1)
		return -1;

	if (FatInstall.InstallFiles(DosDrive) == -1)
		return -1;
	if (FatInstall.InstallIpl(&Ipl) == -1)
		return -1;

	TextUI.OutputStr("\nInstall complete\n");
	return 0;
}

int CInstaller::Install(CVesa::TGraphicsMode GraphicsMode, CMouse::TMouseType MouseType, int PartIndex, bool PartMan, bool SmartBm)
{
	const TPartition *Partition;
	TIPL Ipl;
	CDosDriveList::CDosDrive DosDrive;

	if (!PartMan) {
		XoslFiles.IgnorePartManFiles();
	}
	Partition = PartList.GetPartition(PartIndex);
	DosDrive.Drive = Partition->Drive;
	DosDrive.FATType = FATTYPE_FAT16; // dedicated partition always FAT16
	DosDrive.StartSector = Partition->StartSector;

	if (Partition->SectorCount < 800) {
		TextUI.OutputStr("XOSL 1.1.5 requires a partition of\nat least 400kb\n\n");
		return -1;
	}
		
	if (CreateXoslData(GraphicsMode,MouseType) == -1)
		return -1;
	if (CreateBootItem() == -1)
		return -1;
	if (BackupOriginalMbr(Partition->FSType,XoslFiles.GetOriginalMbrName()) == -1)
		return -1;
	
	if (SmartBm) {
		InstallSmartBootManager(DosDrive.Drive);
	}
	if (BackupOriginalMbr(-1,XoslFiles.GetSmartBmName()) == -1) {
		return -1;
	}

	if (FsCreator.InstallFs(Partition->Drive,Partition->StartSector) == -1)
		return -1;

	if (FatInstall.CreateIpl(DosDrive,Ipl) == -1)
		return -1;

	if (BackupCurrentMbr(&Ipl,Partition->Drive,Partition->StartSector) == -1)
		return -1;



	SetPartId(PartIndex,0x78);


	if (FatInstall.InstallIpl(&Ipl) == -1)
		return -1;

	TextUI.OutputStr("\nInstall complete\n");
	return 0;
}

int CInstaller::Uninstall(const CDosDriveList::CDosDrive &DosDrive, int OriginalMbr)
{
	char MbrBuffer[512];
	const char *MbrFileName;

	if (!OriginalMbr || LoadDosMbr(DosDrive.DriveChar,XoslFiles.GetOriginalMbrName(),MbrBuffer) == -1)
		if (LoadDefaultMbr(MbrBuffer) == -1)
			return -1;
	
	if (FatInstall.InstallIpl(MbrBuffer) == -1)
		return -1;

	FatInstall.RemoveXoslFiles(DosDrive.DriveChar);
	TextUI.OutputStr("\nUninstall complete\n");
	return 0;
}

int CInstaller::Uninstall(int PartIndex, int OriginalMbr)
{
	const TPartition *Partition;
	char OriginalMbrBuffer[512];
	char DefaultMbrBuffer[512];
	char *MbrBuffer;
	

	if (LoadRawMbr(PartIndex,XoslFiles.GetOriginalMbrName(),OriginalMbrBuffer) != -1)
		SetPartId(PartIndex,*(unsigned short *)&OriginalMbrBuffer[508]);

	if (!OriginalMbr) {
		if (LoadDefaultMbr(DefaultMbrBuffer) == -1)
			return -1;
		MbrBuffer = DefaultMbrBuffer;
	}
	else
		MbrBuffer = OriginalMbrBuffer;

		
	if (FatInstall.InstallIpl(MbrBuffer) == -1)
		return -1;
		
	Partition = PartList.GetPartition(PartIndex);
	FsCreator.RestorePartition(Partition->Drive,Partition->StartSector);



	TextUI.OutputStr("\nUninstall complete\n");
	return 0;
}



int CInstaller::Restore(const CDosDriveList::CDosDrive &DosDrive)
{
	char Ipl[512];

	if (LoadDosMbr(DosDrive.DriveChar,XoslFiles.GetCurrentMbrName(),Ipl) == -1)
		return -1;

	if (FatInstall.InstallIpl(Ipl) == -1)
		return -1;

	TextUI.OutputStr("\nRestore complete\n");
	return 0;
}

int CInstaller::Restore(int PartIndex)
{
	char CurrentMbr[512];


	if (LoadRawMbr(PartIndex,XoslFiles.GetCurrentMbrName(),CurrentMbr) == -1)
		return -1;

	if (FatInstall.InstallIpl(CurrentMbr) == -1)
		return -1;

	TextUI.OutputStr("\nRestore complete\n");
	return 0;
}




int CInstaller::CreateXoslData(CVesa::TGraphicsMode GraphicsMode, CMouse::TMouseType MouseType)
{
	CXoslData XoslData;
	CXoslData::CGraphData &GraphData = XoslData.GraphData;
	CXoslData::CColorData &ColorData = XoslData.ColorData;
	CXoslData::CMouseData &MouseData = XoslData.MouseData;
	CXoslData::CMiscPref &MiscPref = XoslData.MiscPref;
	int fh;

	TextUI.OutputStr("Creating data file...");

	MemSet(&XoslData,0,sizeof (CXoslData));

	GraphData.VideoMode = GraphicsMode;
	GraphData.FrameMove = 1;
	
	ColorData.ColorScheme = 15; // XOSL default
	ColorData.FadeColorInt = 100; // 100% -> White

	MouseData.MouseType = MouseType;
	MouseData.MouseSpeed = 5;
	MouseData.PS2Sampling = 5;

	MiscPref.KeyRestart = 0x4400;    // F10
	MiscPref.KeyColdReboot = 0x7100; // Alt-F10
	MiscPref.KeyShutdown = 0x6700;   // Ctrl-F10
	MiscPref.KeyCycle = 0x1100;      // Alt-W
	MiscPref.KeyPart = 0x1910;       // Ctrl-P
	MiscPref.ActiveAllow = 1;
	MiscPref.AutoSave = 1;

	XoslData.XoslVersion = 2;

	if ((fh = DosFile.Create(XoslFiles.GetXoslDataName())) == -1) {
		TextUI.OutputStr("failed\nUnable to create %s\n",XoslFiles.GetXoslDataName());
		return -1;
	}
	if (DosFile.Write(fh,&XoslData,sizeof (CXoslData)) != sizeof (CXoslData)) {
		TextUI.OutputStr("failed\nDisk full.\n");
		DosFile.Close(fh);
		return -1;
	}

	DosFile.Close(fh);
	TextUI.OutputStr("done\n");


	return 0;
}


int CInstaller::CreateBootItem()
{
	char *BootItemData = new char[BOOTITEM_FILESIZE];
	int hFile;
	
	TextUI.OutputStr("Creating boot items file...");
	MemSet(BootItemData,0,BOOTITEM_FILESIZE);
	if ((hFile = DosFile.Create(XoslFiles.GetBootItemName())) == -1) {
		TextUI.OutputStr("failed\nUnable to create %s\n",XoslFiles.GetBootItemName());
		delete[] BootItemData;
		return -1;
	}
	if (DosFile.Write(hFile,BootItemData,BOOTITEM_FILESIZE) != BOOTITEM_FILESIZE) {
		TextUI.OutputStr("failed\nDisk full.\n");
		DosFile.Close(hFile);
		delete[] BootItemData;
		return -1;
	}

	DosFile.Close(hFile);
	TextUI.OutputStr("done\n");
	delete[] BootItemData;
	return 0;
}

int CInstaller::BackupOriginalMbr(int PartId, const char *DestFileName)
{
	CDisk Disk;
	unsigned short Mbr[256];
	int hFile;

	if (PartId != -1) {
		TextUI.OutputStr("Creating backup of MBR...");
	}
	else {
		TextUI.OutputStr("Creating SBM loader...");
	}
	Disk.Map(0x80,0);
	Disk.Read(0,Mbr,1);

	if (PartId != -1) {
		Mbr[254] = PartId;
	}

	if ((hFile = DosFile.Create(DestFileName)) != -1) {
		if (DosFile.Write(hFile,Mbr,512) != 512) {
			TextUI.OutputStr("failed\nFloppy disk full.\n");
			DosFile.Close(hFile);
			return -1;
		}
		DosFile.Close(hFile);
	}

	TextUI.OutputStr("done\n");
	return 0;
}

int CInstaller::BackupCurrentMbr(void *Ipl)
{
	int hFile;

	TextUI.OutputStr("Creating backup of new MBR...");
	if ((hFile = DosFile.Create(XoslFiles.GetCurrentMbrName())) != -1) {
		if (DosFile.Write(hFile,Ipl,512) != 512) {
			TextUI.OutputStr("failed\nFloppy disk full.\n");
			DosFile.Close(hFile);
			return -1;
		}
		DosFile.Close(hFile);
	}

	TextUI.OutputStr("done\n");
	return 0;	
}


int CInstaller::BackupCurrentMbr(void *Ipl, int Drive, unsigned long StartSector)
{
	CFAT16 *FileSystem = new CFAT16;
	const char *DosFileName;
	char RawFileName[16];


	TextUI.OutputStr("Creating backup of new MBR...");
	
	DosFileName = XoslFiles.GetCurrentMbrName();
	FileSystem->DosFileToRawFile(RawFileName,DosFileName);

	FileSystem->Mount(Drive,StartSector);
	if (FileSystem->WriteFile(RawFileName,Ipl) == -1) {
		TextUI.OutputStr("failed\nFloppy disk full.\n");
		delete FileSystem;
		return -1;
	}

	TextUI.OutputStr("done\n");
	delete FileSystem;
	return 0;	
}



int CInstaller::LoadDosMbr(int DriveChar, const char *DosFileName, void *MbrBuffer)
{
	int hFile;
	char FileName[24];

	FileName[0] = DriveChar;
	FileName[1] = ':';
	FileName[2] = '\\';
	strcpy(&FileName[3],DosFileName);

	TextUI.OutputStr("Loading backup of MBR...");
	if ((hFile = DosFile.Open(FileName,CDosFile::accessReadOnly)) == -1) {
		TextUI.OutputStr("failed\nUnable to open %s\n",FileName);
		return -1;
	}
	DosFile.Read(hFile,MbrBuffer,512);
	DosFile.Close(hFile);
	TextUI.OutputStr("done\n");
	return 0;
}


int CInstaller::LoadRawMbr(int PartIndex, const char *DosFileName, void *MbrBuffer)
{
	const TPartition *Partition;
	CFAT16 *FileSystem = new CFAT16; // dedicated partition always FAT16

	char RawFileName[16];

	Partition = PartList.GetPartition(PartIndex);

	if (FileSystem->Mount(Partition->Drive,Partition->StartSector) == -1) {
		TextUI.OutputStr("Unable to mount the partition\n");
		delete FileSystem;
		return -1;
	}

	FileSystem->DosFileToRawFile(RawFileName,DosFileName);

	TextUI.OutputStr("Loading backup of MBR...");
	if (FileSystem->ReadFile(RawFileName,MbrBuffer) != 512) {
		TextUI.OutputStr("failed\nUnable to load %s\n",DosFileName);
		delete FileSystem;
		return -1;
	}
	TextUI.OutputStr("done\n");
	delete FileSystem;
	return 0;
}

int CInstaller::LoadDefaultMbr(void *MbrBuffer)
{
	int hFile;

	TextUI.OutputStr("Loading default MBR...");
	if ((hFile = DosFile.Open(XoslFiles.GetDefaultMbrName(),CDosFile::accessReadOnly)) == -1) {
		TextUI.OutputStr("failed\nUnable to open %s\n",XoslFiles.GetDefaultMbrName());
		return -1;
	}
	DosFile.Read(hFile,MbrBuffer,512);
	DosFile.Close(hFile);
	TextUI.OutputStr("done\n");
	return 0;
}


void CInstaller::SetPartId(int PartIndex, int PartId)
{
	TextUI.OutputStr("Updating partition table...");
	PartList.SetFsType(PartIndex,PartId);
	PartList.WriteStructure();
	TextUI.OutputStr("done\n");
}


const char *SmartBmStatement =
"XOSL INSTALL UTILITY will now install Smart Boot Manager 3.6.4.\n"
"\n"
"* XOSL uses Smart Boot Manager (SBM) to provide CD-ROM booting support.\n"
"* To install SBM, sbminst.exe will be used. Because of this, XOSL \n"
"  install cannot determine whether installation of SBM was successful.\n"
"  sbminst.exe will ask for confirmation if you wish to install SBM.\n"
"* After install, sbmbckup.bin will hold a backup of the data over-\n"
"  written and can be used to fully uninstall SBM. XOSL, however, will\n"
"  not used this file to uninstall SBM - it does not required it. It is\n"
"  still provided so you are free to use it to fully uninstall SBM (and\n"
"  XOSL, if still installed).\n"
"* To start SBM, select 'Smart Boot Manager' from the partition list in\n"
"  XOSL. Use the boot keys feature to control SBM from XOSL (see manual\n"
"  for details).\n"
"* If installing SBM was not successful, booting 'Smart Boot Manager'\n"
"  from XOSL will have the same effect as booting the original MBR.\n"
"\n"
"Smart Boot Manager, Copyright (C) 2000 Suzhe";



void CInstaller::InstallSmartBootManager(int /*Drive*/)
{
	TextUI.ShowPopup(3,2,74,20,SmartBmStatement);
	CKeyboard Keyboard;
	TextUI.HandleKey(Keyboard.GetCh());

	LockScreen();
	ClearScreen();

	puts("Loading SBMINST.EXE...");
	int Status = spawnl(P_WAIT,"SBMINST.EXE","SBMINST.EXE","-b sbmbckup.bin","-d 128",0);

	if (Status != -1) {
		puts("\nPress any key to continue XOSL install...");
		Keyboard.GetCh();
	}
	else {
		perror("");
		Keyboard.GetCh();
	}

	UnlockScreen();

	if (Status == -1) {
		TextUI.OutputStr("Unable to run sbminst.exe...ignored\n");
	}



}


