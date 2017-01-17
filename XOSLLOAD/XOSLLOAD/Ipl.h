/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __ipl__
#define __ipl__

typedef struct {
	unsigned long DiskSectors;
	unsigned long DiskHeads;
	unsigned short RootEntries;
	unsigned short ClusterSectSize;
	unsigned short ClisterByteSize;
	unsigned long FATStart;
	unsigned long DirStart;
	unsigned long DataStart;
	unsigned char FSType;
	unsigned char DriveNumber;
	unsigned long ABSSectorStart;
} TFat16IplData;

typedef struct {
	unsigned long DiskSectors;
	unsigned long DiskHeads;
	unsigned short ClusterSectSize;
	unsigned short ClusterByteSize;
	unsigned long FATStart;
	unsigned long DataStart;
	unsigned long RootCluster;
	unsigned char FSType;
	unsigned char DriveNumber;
	unsigned long ABSSectorStart;
} TFat32IplData;

typedef struct {
	char Ipl[436 - sizeof (TFat16IplData)];
	TFat16IplData IplData;
	char Reserved[10];
	char PTable[16 * 4];
	unsigned short MagicNumber; // 0xaa55
} TFat16Ipl;

typedef struct {
	char Ipl[436 - sizeof (TFat32IplData)];
	TFat32IplData IplData;
	char Reserved[10];
	char PTable[16 * 4];
	unsigned short MagicNumber; // 0xaa55
} TFat32Ipl;

#endif