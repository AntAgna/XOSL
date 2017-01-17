/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <fatfix.h>

#define FAT12      0x01
#define FAT16SMALL 0x04
#define FAT16LARGE 0x06
#define FAT32      0x0b
#define FAT32LBA   0x0c
#define FAT16LBA   0x0e


CDriveFix::CDriveFix()
{
}

CDriveFix::~CDriveFix()
{
}

int CDriveFix::CanFixFAT(const TPartition *Partition)
{
	if (Partition->Drive <= 0x80)
		return false;
	switch (Partition->FSType & ~0x10) {
		case FAT12:
		case FAT16SMALL:
		case FAT16LARGE:
		case FAT32:
		case FAT32LBA:
		case FAT16LBA:
			return true;
		default:
			return false;
	}
}

void CDriveFix::FixFAT(int Drive, int FSType, TBootRecord *BootRecord)
{
	if (Drive < 0x80)
		return;
	switch (FSType & ~0x10) {
		case FAT12:
		case FAT16SMALL:
		case FAT16LARGE:
		case FAT16LBA:
			BootRecord->BootFAT16.Drive = Drive;
			break;
		case FAT32:
		case FAT32LBA:
			BootRecord->BootFAT32.Drive = Drive;
			break;
		default:
			break;
	}
}
