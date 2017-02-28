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

typedef struct {                        // Pos      Len after
	unsigned char Jump[3];              // 0-2      3
	unsigned char OEM_ID[8];            // 3-10     11
	unsigned short SectorSize;          // 11-12    13              x
	unsigned char ClusterSize;          // 13       14              x
	unsigned short ReservedSectors;     // 14-15    16              x
	unsigned char FATCopies;            // 16       17              x
	unsigned short RootEntries;         // 17-18    19              x
	unsigned short SectorCount;         // 19-20    21              x   x = BIOS Parameter Block (25 bytes) (11-35)
	unsigned char MediaDescriptor;      // 21       22              x
	unsigned short FATSize;             // 22-23    24              x
	unsigned short TrackSize;           // 24-25    26              x
	unsigned short HeadCount;           // 26-27    28              x
	unsigned long HiddenSectors;        // 28-31    32              x
	unsigned long BigSectorCount;       // 32-35    36              x
	unsigned short Drive;               // 36-37    38              y
	unsigned char Signature;            // 38       39              y
	unsigned long SerialNo;             // 39-42    43              y   y = Extended BIOS Parameter Block (26 bytes) (36-61)
	unsigned char Label[11];            // 43-53    54              y
	unsigned char FSID[8];              // 54-61    62              y
	unsigned char Loader[448];          // 62-509   510             z   z = Bootstrap code
	unsigned short MagicNumber; // aa55 // 510-511
} TBootFAT16;

typedef struct {
	// Sector 1
	unsigned char Jump[3];              // 0-2      3
	unsigned char OEM_ID[8];            // 3-10     11
	unsigned short SectorSize;          // 11-12    13
	unsigned char ClusterSize;          // 13       14
	unsigned short ReservedSectors;     // 14-15    16
	unsigned char FATCopies;            // 16       17
	unsigned char Reserved1[4];         // 17-20    21
	unsigned char MediaDescriptor;      // 21       22
	// unsigned short FATSize;
	unsigned char Reserved1b[2];        // 22-23    24
	unsigned short TrackSize;           // 24-25    26
	unsigned short HeadCount;           // 26-27    28
	unsigned long HiddenSectors;        // 28-31    32
	unsigned long BigSectorCount;       // 32-35    36
	unsigned long BigFATSize;           // 36-39    40
	// unsigned char ActiveFATs;
	unsigned short Flags;               // 40-41    42
	unsigned char FSVerMajor;           // 42       43
	unsigned char FSVerMinor;           // 43       44
	unsigned long RootCluster;          // 44-47    48
	unsigned short FSSectorNumber;      // 48-49    50
	unsigned short BootSectorBackup;    // 50-51    52
	unsigned char Reserved2[12];        // 52-63    64
	unsigned char Drive;                // 64       65
	unsigned char Reserved3;            // 65       66
	unsigned char Signature;            // 66       67
	unsigned long SerialNo;             // 67-70    71
	unsigned char Label[11];            // 71-81    82
	unsigned char FSID[8];              // 82-89    90
	// unsigned char Loader[418];
	// unsigned long MagicNumber; // 0xaa550000
	unsigned char Loader[420];          // 90-509   510
	unsigned short MagicNumber; // aa55 // 510-511

	// Sector 1 and 2 are ignored

} TBootFAT32;

typedef union {
	TBootFAT16 FAT16;
	TBootFAT32 FAT32;
} TBootRecord;

#endif