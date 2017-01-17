/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <items.h>
#include <mem.h>
#include <disk.h>
#include <key.h>

// TODO: maybe some more range checkings, etc.
// TODO: check return values

void printf(const char *,...);
extern void puts(const char *);

static const char *FileName = "BOOTITEMXDF";
static const char *OriginalMbrFile = "ORIG_MBRXCF";
static const char *SmartMbLoaderFile = "SBM_IPL0XCF";


CBootItems::CBootItems(CFileSystem &FileSystemToUse, CPartList &PartListToUse):
	FileSystem(FileSystemToUse),
	PartList(PartListToUse)
{
	Reset();

	if (!ItemFile.BootItemCount) {
		ItemFile.DefaultItem = -1;
		ItemFile.Timeout = 10;
	}

	// Check whether any changes have been made to the partition structure.
	// if any changes have been made -> compensate. Because partition structure
	// will not be changed that often, it doesn't really matter how long it
	// will take to compensate for it.
	if (PartListIsChanged()) {
		AdjustPartList();
		Save();
	}

}

CBootItems::~CBootItems()
{
}

int CBootItems::GetCount()
{
	return ItemFile.BootItemCount;
}

CBootItem *CBootItems::Get(int Index)
{
	if (Index < 0 || Index >= ItemFile.BootItemCount) {
		return NULL;
	}
	return &ItemFile.BootItems[Index];
}

int CBootItems::Add(CBootItem &NewItem)
{
	if (ItemFile.BootItemCount == 24) {
		return -1;
	}
	memcpy(&ItemFile.BootItems[ItemFile.BootItemCount],&NewItem,sizeof (CBootItem));
	++ItemFile.BootItemCount;
	return 0;
}

void CBootItems::Remove(int Index)
{
	if (Index >= ItemFile.BootItemCount)
		return;
	if (Index == ItemFile.DefaultItem)
		ItemFile.DefaultItem = -1;
	else
		if (Index < ItemFile.DefaultItem)
			--ItemFile.DefaultItem;

	MoveUp(Index + 1);
	--ItemFile.BootItemCount;	
}

void CBootItems::Clear()
{
	ItemFile.BootItemCount = 0;
	ItemFile.DefaultItem = -1;
}

void CBootItems::Swap(int Index1, int Index2)
{
	CBootItem Temp;

	memcpy(&Temp,&ItemFile.BootItems[Index1],sizeof (CBootItem));
	memcpy(&ItemFile.BootItems[Index1],&ItemFile.BootItems[Index2],sizeof (CBootItem));
	memcpy(&ItemFile.BootItems[Index2],&Temp,sizeof (CBootItem));
	if (ItemFile.DefaultItem == Index1)
		ItemFile.DefaultItem = Index2;
	else
		if (ItemFile.DefaultItem == Index2)
			ItemFile.DefaultItem = Index1;
}

void CBootItems::Save()
{
	FileSystem.WriteFile(FileName,&ItemFile);
}

void CBootItems::Reset()
{
	FileSystem.ReadFile(FileName,&ItemFile);
}

void CBootItems::SetDefault(int Index)
{
	ItemFile.DefaultItem = Index;
}

int CBootItems::GetDefault()
{
	return ItemFile.DefaultItem;
}

int CBootItems::GetTimeout()
{
	return ItemFile.Timeout;
}

void CBootItems::SetTimeout(int Timeout)
{
	ItemFile.Timeout = Timeout;
}

int CBootItems::InitBoot(int BootIndex)
{
	CDisk Disk;
	int Index, Count;
	const CBootItem *BootItem;
	const TPartition *Partition;
	int DriveNum;

	BootItem = &ItemFile.BootItems[BootIndex];
	Count = PartList.GetCount();
	Partition = PartList.GetPartition(BootItem->PartIndex);
	for (Index = 0; Index < Count; ++Index) {
		if (BootItem->HideList[Index]) {
			PartList.Hide(Index);
		}
	}

	if (Partition->Type == PART_MBR && Partition->Drive == 0x80) {
		if (FileSystem.ReadFile(OriginalMbrFile,IPL_ADDR) <= 0)
			return -1;
		PartList.InsertMbrPTab(IPL_ADDR);
	}
	else if (Partition->Type == PART_SBM && Partition->Drive == 0x80) {
		if (FileSystem.ReadFile(SmartMbLoaderFile,IPL_ADDR) <= 0)
			return -1;
		PartList.InsertMbrPTab(IPL_ADDR);
	}
	else {
		Disk.Map(Partition->Drive,Partition->StartSector);
		if (Disk.Read(0,IPL_ADDR,1) == -1)
			return -1;
	}

	if (BootItem->Activate)
		PartList.SetActive(BootItem->PartIndex);
	PartList.WriteStructure();


	if (BootItem->SwapDrives) {
		DriveFix.SwapDrive(Partition->Drive);
		DriveNum = 0x80;
	}
	else {
		DriveNum = Partition->Drive;
	}

	if (BootItem->FixDriveNum) {
		DriveFix.FixFAT(DriveNum,Partition->FSType,(TBootRecord *)IPL_ADDR);
	}


	StoreKeys(BootItem->Keys);
	return DriveNum;
}

bool CBootItems::IsDefault(int Index)
{
	return ItemFile.DefaultItem == Index;
}

bool CBootItems::CanFixFAT(int Index)
{
	int PartIndex;

	PartIndex = ItemFile.BootItems[Index].PartIndex;
	if (PartIndex >= ItemFile.PartCount) {
		return false;
	}
	return DriveFix.CanFixFAT(PartList.GetPartition(PartIndex));
}

bool CBootItems::CanSwap(int Index)
{
	int PartIndex;

	PartIndex = ItemFile.BootItems[Index].PartIndex;
	if (PartIndex >= ItemFile.PartCount) {
		return false;
	}
	return PartList.GetPartition(PartIndex)->Drive > 0x80;
}

void CBootItems::MoveUp(int Index)
{
	int MoveCount = ItemFile.BootItemCount - Index;

	memcpy(&ItemFile.BootItems[Index - 1],&ItemFile.BootItems[Index],MoveCount * sizeof(CBootItem));
}


void CBootItems::StoreKeys(const unsigned short *Keys)
{
	CKeyboard::Flush();
	for (; *Keys; ++Keys) {
		CKeyboard::StoreKeyStroke(*Keys);
	}
}


bool CBootItems::PartListIsChanged()
{
	int Count = PartList.GetCount();
	int Index;

	if (Count != ItemFile.PartCount) {
		return true;
	}
	for (Index = 0; Index < Count; ++Index) {
		if (PartList.Locate(ItemFile.PartList[Index].Drive,ItemFile.PartList[Index].StartSector) != Index) {
			return true;
		}
	}
	return false;
}

void CBootItems::AdjustPartList()
{
	int RelocTable[56];
	int Index;

	BuildRelocTable(RelocTable);
	UpdatePartList(RelocTable);
	for (Index = 0; Index < ItemFile.BootItemCount; ++Index) {
		UpdatePartIndex(ItemFile.BootItems[Index],RelocTable);
		UpdateHideList(ItemFile.BootItems[Index].HideList,RelocTable);
	}
}


void CBootItems::BuildRelocTable(int *RelocTable)
{
	int Index;
	int AddIndex;
	int RelocIndex;

	for (Index = 0; Index < 56; ++Index) {
		RelocTable[Index] = PartList.Locate(ItemFile.PartList[Index].Drive,ItemFile.PartList[Index].StartSector);
	}
	AddIndex = PartList.GetCount() - ItemFile.PartCount;
	for (Index = 0; Index < ItemFile.PartCount; ++Index) {
		if (RelocTable[Index] == -1) {
			++AddIndex;
		}
	}

	for (Index = ItemFile.PartCount; Index < 56; ++Index) {
		if (RelocTable[Index] == -1) {
			RelocTable[Index] = Index + AddIndex;
		}
	}
	
	RelocIndex = PartList.GetCount();
	for (Index = 0; Index < ItemFile.PartCount; ++Index) {
		if (RelocTable[Index] == -1) {
			RelocTable[Index] = RelocIndex++;
		}
	}
}


void CBootItems::UpdatePartList(int *RelocTable)
{
	bool ItemResolved[56];
	CPartDesc NewPartList[56];
	const TPartition *Partition;
	int Index;

	memset(ItemResolved,false,sizeof (bool[56]));
	for (Index = 0; Index < 56; ++Index) {
		if (RelocTable[Index] < 56) {
			NewPartList[RelocTable[Index]].Drive = ItemFile.PartList[Index].Drive;
			NewPartList[RelocTable[Index]].StartSector = ItemFile.PartList[Index].StartSector;
			ItemResolved[RelocTable[Index]] = true;
		}
	}
	for (Index = 0; Index < 56; ++Index) {
		if (!ItemResolved[Index]) {
			Partition = PartList.GetPartition(Index);
			ItemFile.PartList[Index].Drive = Partition->Drive;
			ItemFile.PartList[Index].StartSector = Partition->StartSector;
		}
		else {
			ItemFile.PartList[Index].Drive = NewPartList[Index].Drive;
			ItemFile.PartList[Index].StartSector = NewPartList[Index].StartSector;
		}
	}
	ItemFile.PartCount = PartList.GetCount();
}

void CBootItems::UpdatePartIndex(CBootItem &BootItem, int *RelocTable)
{
	BootItem.PartIndex = RelocTable[BootItem.PartIndex];
	if (BootItem.PartIndex >= 56) {
		BootItem.PartIndex = 55;
	}

	if (BootItem.PartIndex >= ItemFile.PartCount) {
		if (!BootItem.Disabled) {
			BootItem.Disabled = 2;
		}
	}
	else if (BootItem.Disabled == 2) {
		BootItem.Disabled = 0;
	}
}

void CBootItems::UpdateHideList(unsigned char *HideList, int *RelocTable)
{
	int HideIndex;
	unsigned char NewHideList[56];

	memset(NewHideList,false,sizeof (unsigned char[56]));
	for (HideIndex = 0; HideIndex < 56; ++HideIndex) {
		if (RelocTable[HideIndex] < 56) {
			NewHideList[RelocTable[HideIndex]] = HideList[HideIndex];
		}
	}
	memcpy(HideList,NewHideList,sizeof (unsigned char[56]));
}

