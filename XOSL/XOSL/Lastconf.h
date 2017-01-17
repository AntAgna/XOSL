/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __lastconf__
#define __lastconf__

#include <newdefs.h>
#include <ptab.h>
#include <fs.h>

typedef struct {
	unsigned char Drive;
	unsigned long StartSector;
} TPartKey;

typedef struct {
	int PartitionCount;
	TPartKey LastPartList[92];
	char Reserved[50]; // fill 512 byte
} TLastConfig;

class CLastConfig {
	public:
		CLastConfig(CFileSystem *FileSystemToUse, CPartList *PartListToUse);
		~CLastConfig();

		const short *GetRelocTable();
		void SetNewConfig();
	private:
		TLastConfig *FileData;
		short RelocTable[92];
		CFileSystem *FileSystem;
		CPartList *PartList;
};

#endif