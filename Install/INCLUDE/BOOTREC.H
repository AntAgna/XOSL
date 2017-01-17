/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __bootrec__
#define __bootrec__

typedef struct {
	unsigned char Jump[3];
	unsigned char OEM_ID[8];
	unsigned short SectorSize;
	unsigned char ClusterSize;
	unsigned short ReservedSectors;
	unsigned char FATCopies;
	unsigned short RootEntries;
	unsigned short SectorCount;
	unsigned char MediaDescriptor;
	unsigned short FATSize;
	unsigned short TrackSize;
	unsigned short HeadCount;
	unsigned long HiddenSectors;
	unsigned long BigSectorCount;
	unsigned short Drive;
	unsigned char Signature;
	unsigned long SerialNo;
	unsigned char Label[11];
	unsigned char FSID[8];
	unsigned char Loader[448];
	unsigned short MagicNumber;  // 0xaa55
} TBootFAT16;

typedef struct {
	// Sector 1
	unsigned char Jump[3];
	unsigned char OEM_ID[8];
	unsigned short SectorSize;
	unsigned char ClusterSize;
	unsigned short ReservedSectors;
	unsigned char FATCopies;
	unsigned char Reserved1[4];
	unsigned char MediaDescriptor;
	unsigned short FATSize;
	unsigned short TrackSize;
	unsigned short HeadCount;
	unsigned long HiddenSectors;
	unsigned long BigSectorCount;
	unsigned long BigFATSize;
	unsigned char ActiveFATs;
	unsigned char FSVerMajor;
	unsigned short FSVerMinor;
	unsigned long RootCluster;
	unsigned short FSSectorNumber;
	unsigned short BootSectorBackup;
	unsigned char Reserved2[12];
	unsigned char Drive;
	unsigned char Reserved3;
	unsigned char Signature;
	unsigned long SerialNo;
	unsigned char Label[11];
	unsigned char FSID[8];
	unsigned char Loader[418];
	unsigned long MagicNumber; // 0xaa550000

	  // Sector 1 and 2 are ignored

} TBootFAT32;

typedef union {
	TBootFAT16 FAT16;
	TBootFAT32 FAT32;
} TBootRecord;

#endif