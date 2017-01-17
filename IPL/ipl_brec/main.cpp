/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <disk.h>
#include <mem.h>


class CBootRecord {
public:
	unsigned char Jump[3];			// jmp short + nop
	unsigned char OEM_ID[8];		// XOSLINST
	unsigned short SectorSize;		// 512
	unsigned char ClusterSize;		// 16 (8192 byte)
	unsigned short ReservedSectors;	// 1
	unsigned char FATCopies;		// 1
	unsigned short RootEntries;		// 32
	unsigned short SectorCount;		// ?
	unsigned char MediaDescriptor;	// 0xF8 (?) 
	unsigned short FATSize;			// 1
	unsigned short TrackSize;		// ? (sectors per head)
	unsigned short HeadCount;		// ?
	unsigned long HiddenSectors;	// ? (partition offset)
	unsigned long BigSectorCount;	// 0 (total sectors < 65536)
	unsigned short Drive;			// ?
	unsigned char Signature;		// 0x29 (?)
	unsigned long SerialNo;			// 0x4c534f58 (don't really care)
	unsigned char Label[11];		// XOSL110
	unsigned char FSID[8];			// FAT16
	unsigned char Loader[448];		// IPL
	unsigned short MagicNumber;		// 0x534f (used by XOSLLOAD)
};

class CDirectoryEntry {
public:
	unsigned char FileName[8];
	unsigned char Extension[3];
	unsigned char Attribute;
	unsigned char Reserved[10];
	unsigned short Time;
	unsigned short Date;
	unsigned short StartCluster;
	unsigned long FileSize;
};



static void  LoadXoslLoad();
static int LocateXoslLoad();


void main()
{
	CBootRecord *BootRecord = (CBootRecord *)0x00007c00;
	DiskMap(BootRecord->Drive,BootRecord->HiddenSectors);
	LoadXoslLoad();
}

void LoadXoslLoad()
// Sector 0 = boot record, sector 1 = FAT, sector 2,3 = root dir, sector 4 = data start (cluster 2)
{
	long XoslLoadCluster;
	long XoslLoadSector;

	XoslLoadCluster = LocateXoslLoad();
	
	XoslLoadSector = (((long)XoslLoadCluster - 2) << 4) + 4;

	// assumption: xoslload <= 8192 byte
	DiskRead(XoslLoadSector,(void *)0x80000100,16);
}

int LocateXoslLoad()
{
	CDirectoryEntry Root[16];
	int Index;

	// assumption: xoslload.xcf is among the first 16 file entries
	DiskRead(2,Root,1);

	for (Index = 0; Index < 16; ++Index)
		if (MemCompare(Root[Index].FileName,"XOSLLOADXCF",11) == 0)
			return Root[Index].StartCluster;

	// halt
	for (;;); 
}

