/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <ptab.h>
#include <transfer.h>
#include <disk.h>
#include <mem.h>
#include <ctype.h>

#include <fat16.h>
#include <fat32.h>
//#include <ntfs.h>

#define FSTYPE_EXTENDED 0x05
#define FSTYPE_EXTENDEDLBA 0x0f
#define FSTYPE_HIDDENEXTENDED 0x1f
#define FSTYPE_LINUXEXT 0x85

extern void printf(const char *,...);

CPartList::CPartList()
{
	PartListChanged = false;
	VolumeLabelsRead = false;
	ReadStructure();
	AllowActiveHD = 0;
}

CPartList::~CPartList()
{
	// stuff should be deallocated here
}

void CPartList::ReadStructure()
{
	int DrvCount, Index;
	CDiskAccess DiskAccess;

	DrvCount = DiskAccess.DriveCount(0x80);
	for (Index = 0; Index < DrvCount; ++Index)
		AddDrive(Index | 0x80,0,0,PART_PRIMARY);
	CreatePartList(DiskAccess.DriveCount(0x00));
	// Create PartList Look-up Table
	CreatePLUP();
}

void CPartList::AddDrive(int Drive, unsigned long StartSector, unsigned long ExtStart, int Type)
{
	CMBRNode NewNode;
	CDisk Disk;
	TPartTable *PartTable;
	const TPartEntry *Entries;
	int Index;
	unsigned long NewStartSector;

	if (Disk.Map(Drive,StartSector) == -1)
		return;
	PartTable = new TPartTable;
	if (Disk.Read(0,PartTable,1) == -1 || PartTable->MagicNumber != 0xaa55) {
		delete PartTable;
		return;
	}

	NewNode.AbsoluteSector = StartSector;
	NewNode.Drive = Drive;
	NewNode.Type = Type;
	NewNode.Table = PartTable;
	MBRList.insert(MBRList.end(),NewNode);
                               
	Entries = PartTable->Entries;
	for (Index = 0; Index < 4; ++Index)
		switch (Entries[Index].FSType) {
			case FSTYPE_EXTENDED:
			case FSTYPE_EXTENDEDLBA:
			case FSTYPE_LINUXEXT:
			case FSTYPE_HIDDENEXTENDED:
				if (Type != PART_LOGICAL) {
					ExtStart = Entries[Index].RelativeSector;
					NewStartSector = ExtStart;
				}
				else
					NewStartSector = ExtStart + Entries[Index].RelativeSector;
				AddDrive(Drive,NewStartSector,ExtStart,PART_LOGICAL);
				break;
			default:
				break;
		}
}


void CPartList::CreatePartList(int FloppyCount)
{
	list<CMBRNode>::iterator MBRNode;
	int Index, Drive;
	const TPartEntry *Entries;

	if (MBRList.size()) {
		MBRNode = MBRList.begin();
		Drive = (*MBRNode).Drive;
		CreateNonPartNode(Drive);
		CreateNonPartNode(-1);

		for (; MBRNode != MBRList.end(); ++MBRNode) {
			if (Drive != (*MBRNode).Drive) {
				Drive = (*MBRNode).Drive;
				CreateNonPartNode(Drive);
			}
			for (Index = 0; Index < 4; ++Index) {
				Entries = &(*MBRNode).Table->Entries[Index];
				if (Entries->RelativeSector && ((Entries->FSType != FSTYPE_EXTENDED && Entries->FSType != FSTYPE_EXTENDEDLBA &&
					Entries->FSType != FSTYPE_HIDDENEXTENDED && Entries->FSType != FSTYPE_LINUXEXT) ||
					(*MBRNode).Type != PART_LOGICAL)) {
					CreatePartNode(MBRNode,Index);
				}
			}
		}
	}
	for (Index = 0; Index < FloppyCount; ++Index)
		CreateNonPartNode(Index);
}


void CPartList::CreateNonPartNode(int Drive)
{
	CPartNode PartNode;
	TPartition *Partition;

	Partition = PartNode.Partition = new TPartition;
	PartNode.Entry = NULL;

	Partition->Drive = Drive;
	Partition->StartSector = 0;
	Partition->SectorCount = 0;
	Partition->VolumeLabel = "";
	if (Drive >= 0x80) {
		if (Drive == 0x80)
			Partition->FSName = "Original MBR";
		else
			Partition->FSName = "Master Boot Record";
		Partition->FSType = -1;
		Partition->Type = PART_MBR;
	}
	else if (Drive == -1) {
		Partition->FSName = "Smart Boot Manager";
		Partition->FSType = -1;
		Partition->Type = PART_SBM;
		Partition->Drive = 0x80;
		Partition->StartSector = PART_SBM;
	}
	else {
		Partition->FSName = "Boot floppy";
		Partition->FSType = -1;
		Partition->Type = PART_FLOPPY;
	}
	PartList.insert(PartList.end(),PartNode);
}

void CPartList::CreatePartNode(list<CMBRNode>::iterator MBRNode, int Index)
{
	CPartNode PartNode;
	TPartition *Partition;
	const TPartEntry *PartEntry;

	Partition = PartNode.Partition = new TPartition;
	PartEntry = PartNode.Entry = &(*MBRNode).Table->Entries[Index];

	Partition->Drive = (*MBRNode).Drive;
	Partition->StartSector = (*MBRNode).AbsoluteSector + PartEntry->RelativeSector;
	Partition->SectorCount = PartEntry->SectorCount;
	Partition->FSName = GetFSName(PartEntry->FSType);
	Partition->FSType = PartEntry->FSType;
	Partition->Type = (*MBRNode).Type;
	Partition->VolumeLabel = "";
	PartList.insert(PartList.end(),PartNode);
}

const char *CPartList::GetFSName(int FSID)
{
	int Index;

	for (Index = 0; FSNameList[Index].FSID != FSID && FSNameList[Index].FSID != 0xff; ++Index);
	return FSNameList[Index].FSName;
}

void CPartList::CreatePLUP()
{
	int Index;
	list<CPartNode>::iterator PartListEntry;

	PartListEntry = PartList.begin();
	PLUP = new CPartNode *[PartList.size()];
	for (Index = 0; Index < PartList.size(); ++Index, ++PartListEntry)
		PLUP[Index] = &*PartListEntry; // --> assignment operator has to be implemented for list<>
}

void CPartList::WriteStructure()
{
	CDisk Disk;
	list<CMBRNode>::iterator MBRListEntry;

	if (PartListChanged) {
		// TODO: Detect that active partition is already active
		for (MBRListEntry = MBRList.begin(); MBRListEntry != MBRList.end(); ++MBRListEntry) {
			Disk.Map((*MBRListEntry).Drive,(*MBRListEntry).AbsoluteSector);
			Disk.Write(0,(*MBRListEntry).Table,1);
		}
	}
}

const TPartition *CPartList::GetPartition(int Index)
{
	if (Index >= PartList.size()) {
		return NULL;
	}
	return PLUP[Index]->Partition;
}

int CPartList::Locate(int Drive, unsigned long StartSector)
{
	int Index;
	const TPartition *Partition;

	for (Index = 0; Index < PartList.size(); ++Index) {
		Partition = PLUP[Index]->Partition;
		if (Partition->Drive == Drive && Partition->StartSector == StartSector)
			return Index;
	}
	return -1;
}

int CPartList::GetCount()
{
	return PartList.size();
}

int CPartList::CanHide(int Index)
{
	switch (PLUP[Index]->Entry->FSType & 0xef) {
		case 0x01:
		case 0x04:
		case 0x06:
		case 0x07:
		case 0x0b:
		case 0x0c:
		case 0x0e:
		case 0x0f:
			return 1;
		default:
			return 0;
	}
}

void CPartList::Hide(int Index)
{
	unsigned char &FSType = PLUP[Index]->Entry->FSType;

	if (CanHide(Index) && (FSType | 0x10) != FSType) {
		FSType |= 0x10;
		PartListChanged = true;
	}
}

void CPartList::Reveal(int Index)
{
	unsigned char &FSType = PLUP[Index]->Entry->FSType;

	if (CanHide(Index) && (FSType & 0xef) != FSType) {
		FSType &= 0xef;
		PartListChanged = true;
	}
}

void CPartList::RevealAll()
{
	int Index;

	for (Index = 0; Index < PartList.size(); ++Index)
		switch (PLUP[Index]->Entry->FSType) {
			case 0x11:
			case 0x14:
			case 0x16:
			case 0x17:
			case 0x1b:
			case 0x1c:
			case 0x1e:
			case 0x1f:
				PLUP[Index]->Entry->FSType &= 0x0ef;
				PLUP[Index]->Partition->FSName = GetFSName(PLUP[Index]->Entry->FSType);
				PartListChanged = true;
				break;
			default:
				break;
		}
}

int CPartList::CanActivate(int Index)
{
	int Type;

	Type = PLUP[Index]->Partition->Type;
	return Type != PART_MBR && Type != PART_FLOPPY;
}

void CPartList::SetAllowActiveHD(int Status)
{
	AllowActiveHD = Status;
}

void CPartList::SetActive(int PartIndex)
{
	int Index;
	int Drive;

	if (!AllowActiveHD) {
		// clear active flag for all partitions
		for (Index = 0; Index < PartList.size(); ++Index) {
			if (Index != PartIndex && PLUP[Index]->Entry->Activated == 0x80) {
				PLUP[Index]->Entry->Activated = 0x00;
				PartListChanged = true;            
			}
		}
	}
	else {
		// clear active flag for all partitions on same drive only
		Drive = PLUP[PartIndex]->Partition->Drive;
		for (Index = 0; Index < PartList.size(); ++Index) {
			if (Index != PartIndex && PLUP[Index]->Entry->Activated == 0x80 && PLUP[Index]->Partition->Drive == Drive) {
				PLUP[Index]->Entry->Activated = 0x00;
				PartListChanged = true;
			}
		}
	
	}
	if (PLUP[PartIndex]->Entry->Activated != 0x80) {
		PLUP[PartIndex]->Entry->Activated = 0x80;
		PartListChanged = true;
	}	
}

void CPartList::InsertMbrPTab(void *DestAddr)
{
	char *Dest = (char *)DestAddr;

	if (MBRList.size())
		memcpy(&Dest[446],(*MBRList.begin()).Table->Entries,sizeof(TPartEntry[4]));
}

void CPartList::ReadVolumeLabels()
// TODO: volume labels of NTFS partitions?
{
	TPartition *Partition;
	CDisk Disk;
	char BootRecord[512];
	TBootFAT16 *BootFAT16;
	TBootFAT32 *BootFAT32;
	int Index;

	if (VolumeLabelsRead) {
		return;
	}
	BootFAT16 = (TBootFAT16 *)BootRecord;
	BootFAT32 = (TBootFAT32 *)BootRecord;
	for (Index = 0; Index < PartList.size(); ++Index) {
		Partition = PLUP[Index]->Partition;

		if (Partition->Drive < 0x80) {
			VolumeLabelsRead = true;
			return;
		}
		Disk.Map(Partition->Drive,Partition->StartSector);
		Disk.Read(0,BootRecord,1);

		if (memcmp(BootFAT16->FSID,"FAT1",4) == 0) {
			Partition->VolumeLabel = new char [12];
			CreateVolumeLabel(BootFAT16->Label,Partition->VolumeLabel);
		}
		else {
			if (memcmp(BootFAT32->FSID,"FAT3",4) == 0) {
				Partition->VolumeLabel = new char [12];
				CreateVolumeLabel(BootFAT32->Label,Partition->VolumeLabel);
			}
		}
	}
	VolumeLabelsRead = true;
}

void CPartList::CreateVolumeLabel(const char *RawLabel, char *VolumeLabel)
{
	int Index;

	memcpy(VolumeLabel,RawLabel,11);
	for (Index = 10; Index >= 0 && VolumeLabel[Index] == ' '; --Index);
	VolumeLabel[Index + 1] = '\0';
	if (VolumeLabel[0])
		for (Index = 1; VolumeLabel[Index]; ++Index)
			VolumeLabel[Index] = tolower(VolumeLabel[Index]);
}


void CPartList::GetDiskPartitions(int Drive, list<CPartNode>::iterator &First, list<CPartNode>::iterator &Last)
{
	if (Drive >= 0x80) {
		for (First = PartList.begin(); First != PartList.end() && (*First).Partition->Drive != Drive; ++First);
		for (Last = First; Last != PartList.end() && (*Last).Partition->Drive == Drive; ++Last);
	}
	else {
		// Get all floppies, and not just the one specified.
		Last = PartList.end();
		First = PartList.end();
		// (*--First).Partition->Drive is too complex for BC++3.1
		for (--First; (*First).Partition->Drive == 0x00 && First != PartList.begin() ; --First);
		++First;
	}
}

int CPartList::GetPartIndex(int Drive, int PartIndex)
{
	int Index;
	list<CPartNode>::iterator Entry;

	if (Drive < 0x80) {
		Drive = 0x00;
	}
	for (Index = 0, Entry = PartList.begin(); Entry != PartList.end() && (*Entry).Partition->Drive != Drive; ++Index, ++Entry);
	Index += PartIndex;
	if (Index > PartList.size()) {
		return -1;
	}
	return Index;
}

void CPartList::GetDiskPartIndex(int Index, int &Drive, int &PartIndex)
{
	list<CPartNode>::iterator Entry;

	if (Index > PartList.size()) {
		return;
	}

	Drive = 0x80;
	PartIndex = -1;
	Entry = PartList.begin();
	do {
		if ((*Entry).Partition->Drive != Drive && Drive != 0x00) {
			Drive = (*Entry).Partition->Drive;
			PartIndex = 0;
		}
		else {
			++PartIndex;
		}
		++Entry;
	} while (Index--);
/*	for (Entry = PartList.begin(); Index; ++Entry, --Index) {
		if ((*Entry).Partition->Drive != Drive && Drive != 0x00) {
			Drive = (*Entry).Partition->Drive;
			PartIndex = 0;
		}
		else {
			++PartIndex;
		}
	}*/
}

const CPartList::TFSNameEntry CPartList::FSNameList[] = {
	{0x01,"Microsoft FAT12"},
	{0x04,"Microsoft FAT16"},
	{0x05,"Extended"},
	{0x06,"Microsoft FAT16"},
	{0x07,"HPFS or NTFS"},
	{0x0a,"OS/2 Boot Manager"},
	{0x0b,"Microsoft FAT32"},
	{0x0c,"Microsoft FAT32 LBA"},
	{0x0e,"Microsoft FAT16 LBA"},
	{0x0f,"Extended LBA"},
	{0x11,"Hidden FAT12"},
	{0x14,"Hidden FAT16"},
	{0x16,"Hidden FAT16"},
	{0x17,"Hidden NTFS"},
	{0x1b,"Hidden FAT32"},
	{0x1c,"Hidden FAT32 LBA"},
	{0x1e,"Hidden FAT16 LBA"},
	{0x1f,"Hidden Extended LBA"},
	{0x38,"TheOS"},
	{0x63,"Unix SysV/386"},
	{0x78,"XOSL FS"},
	{0x82,"Linux Swap"},
	{0x83,"Linux Native"},
	{0x85,"Linux Extended"},
	{0xa5,"FreeBSD, BSD/386"},
	{0xeb,"BeOS"},
	{0xff,"Unknown"}
};
