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
	unsigned short ClusterByteSize;
	unsigned long FATStart;
	unsigned long DirStart;
	unsigned long DataStart;
	unsigned char FSType;
	unsigned char DriveNumber;
	unsigned long ABSSectorStart;
} TIPLData16Conv;

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
} TIPLData32Conv;

typedef struct {
	unsigned short RootEntries;
	unsigned short ClusterSectSize;
	unsigned short ClusterByteSize;
	unsigned long FATStart;
	unsigned long DirStart;
	unsigned long DataStart;
	unsigned char FSType;
	unsigned char DriveNumber;
	unsigned long ABSSectorStart;
} TIPLData16LBA;

typedef struct {
	unsigned short ClusterSectSize;
	unsigned short ClusterByteSize;
	unsigned long FATStart;
	unsigned long DataStart;
	unsigned long RootCluster;
	unsigned char FSType;
	unsigned char DriveNumber;
	unsigned long ABSSectorStart;
} TIPLData32LBA;


typedef struct {
	char IPL[436 - sizeof (TIPLData16Conv)];
	TIPLData16Conv IPLData;
	char Reserved[10];
} TIPLFAT16Conv;

typedef struct {
	char IPL[436 - sizeof (TIPLData32Conv)];
	TIPLData32Conv IPLData;
	char Reserved[10];
} TIPLFAT32Conv;

typedef struct {
	char IPL[436 - sizeof (TIPLData16LBA)];
	TIPLData16LBA IPLData;
	char Reserved[10];
} TIPLFAT16LBA;

typedef struct {
	char IPL[436 - sizeof (TIPLData32LBA)];
	TIPLData32LBA IPLData;
	char Reserved[10];
} TIPLFAT32LBA;


typedef union {
	TIPLFAT16Conv IPLFAT16Conv;
	TIPLFAT32Conv IPLFAT32Conv;
	TIPLFAT16LBA IPLFAT16LBA;
	TIPLFAT32LBA IPLFAT32LBA;
} TIPL;

#endif