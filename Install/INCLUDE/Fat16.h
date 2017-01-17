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
#include <bootrec.h>


class CFAT16: public CFileSystem {
	public:
		CFAT16();
		~CFAT16();
		int Mount(int Drive, long StartSector);
		unsigned short ReadFile(const char *FileName, void *Buffer);
		int WriteFile(const char *FileName, const void *Buffer);

		void DosFileToRawFile(char *RawFile, const char *DosFile);

	private:
		class CFAT16DirEntry {
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

		int Locate(const char *FileName, CFAT16DirEntry &Entry);
		void ReadFAT(unsigned short Cluster);
		void ReadDirectory(unsigned short Index, CFAT16DirEntry *Root);

		void GetNextCluster(unsigned short &Cluster);
		void ReadCluster(unsigned short Cluster, void *Buffer);
		void WriteCluster(unsigned short Cluster, const void *Buffer);


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