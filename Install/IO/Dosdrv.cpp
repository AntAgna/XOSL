/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <dosdrv.h>
#include <bootrec.h>
#include <disk.h>

#define GetFATType(FSType) \
	(FSType == 0x06 || FSType == 0x0e ? FATTYPE_FAT16 : FATTYPE_FAT32)

CDosDriveList::CDosDriveList(CPartList &PartListToUse):
	PartList(PartListToUse)
{
}

CDosDriveList::~CDosDriveList()
{
}

int CDosDriveList::LocateDrive(int Drive, CDosDrive &DosDrive)
{
	int Index;
	int PartCount;
	unsigned long DosSerialNo;
	unsigned long BRecSerialNo;
	const TPartition *Partition;

	DosDrive.DriveChar = Drive + 'C';
	if ((DosSerialNo = GetDosSerialNo(Drive)) == (unsigned long)-1)
		return -1;
	PartCount = PartList.GetCount();
	for (Index = 0; Index < PartCount; ++Index)
		if ((BRecSerialNo = GetBRecSerialNo(Index)) != (unsigned long)-1 &&
			 BRecSerialNo == DosSerialNo) {
			Partition = PartList.GetPartition(Index);
			DosDrive.FATType = GetFATType(Partition->FSType);
			DosDrive.Drive = Partition->Drive;
			DosDrive.StartSector = Partition->StartSector;
			return 0;
		}
	return -1;
}

unsigned long CDosDriveList::GetBRecSerialNo(int Index)
{
	TBootRecord BootRecord;
	const TPartition *Partition;
	CDisk Disk;

	Partition = PartList.GetPartition(Index);
	if (Disk.Map(Partition->Drive,Partition->StartSector) == -1)
		return (unsigned long)-1;
	if (Disk.Read(0,&BootRecord,1) == -1)
		return (unsigned long)-1;
	switch (Partition->FSType) {
		case 0x06: // FAT16
		case 0x0e: // FAT16 LBA
			return BootRecord.FAT16.SerialNo;
		case 0x0b: // FAT32
		case 0x0c: // FAT32 LBA
			return BootRecord.FAT32.SerialNo;
		default:
			return (unsigned long) -1;
	}
}
