/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __fat16__
#define __fat16__

#include <fs.h>

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
	unsigned long StartSector;
	unsigned long BigSectorCount;
	unsigned short Drive;
	unsigned char Signature;
	unsigned long SerialNumber;
	unsigned char Label[11];
	unsigned char FSID[8];
	unsigned char Loader[448];
	unsigned short MagicNumber;  // 0xaa55
} TBootFAT16;

typedef struct {
	unsigned char FileName[8];
	unsigned char Extension[3];
	unsigned char Attribute;
	unsigned char Reserved[10];
	unsigned short Time;
	unsigned short Date;
	unsigned short StartCluster;
	unsigned long FileSize;
} TFAT16DirEntry;


class CFAT16: public CFileSystem {
	public:
		CFAT16();
		~CFAT16();
		int Mount(int Drive, unsigned long StartSector);
		unsigned short ReadFile(const char *FileName, void *Buffer);
	private:
		int Locate(const char *FileName, TFAT16DirEntry &Entry);
		void ReadFAT(unsigned short Cluster);
		void ReadDirectory(unsigned short Index, TFAT16DirEntry *Root);

		void GetNextCluster(unsigned short &Cluster);
		void ReadCluster(unsigned short Cluster, void *Buffer);


		TBootFAT16 BootSector;

		unsigned short *FAT;
		unsigned short FirstCluster;
		unsigned short LastCluster;

		unsigned short ClusterSize;
		unsigned long FATStart;
		unsigned long DirStart;
		unsigned long DataStart;
};

#endif