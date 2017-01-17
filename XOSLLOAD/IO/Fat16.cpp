/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <fat16.h>
#include <disk.h>
#include <mem.h>

#define INMEMORY_CLUSTERS 4096
#define FAT_SECTOR_COUNT  (INMEMORY_CLUSTERS / 256) // 256 clusters/sector

#define INMEMORY_ENTRIES 32
#define DIR_SECTOR_COUNT (INMEMORY_ENTRIES / 16)

#define FILE_DELETED 0xe5

CFAT16::CFAT16(): CFileSystem()
{
	FAT = new unsigned short[INMEMORY_CLUSTERS];
}

CFAT16::~CFAT16()
{
	delete FAT;
}

int CFAT16::Mount(int Drive, long StartSector)
{
	int Status;

	Status = CFileSystem::Mount(Drive,StartSector);
	if (Status != -1) {
		Disk->Read(0,&BootSector,1);
		if (memcmp(BootSector.FSID,"FAT16   ",8) != 0)
			Status = -1;
		else {
			ClusterSize = (unsigned short)BootSector.ClusterSize << 9;
			FATStart = BootSector.ReservedSectors;
			DirStart = FATStart + BootSector.FATSize * (int)BootSector.FATCopies;
			DataStart = DirStart + (BootSector.RootEntries >> 4);
			LastCluster = 0;
		}
	}
	return Status;
}

unsigned short CFAT16::ReadFile(const char *FileName, void *Buffer)
{
	unsigned short Cluster;
	TFAT16DirEntry Entry;
	void *ClusterData;
	unsigned long SizeLeft;

	if (Locate(FileName,Entry) == -1)
		return 0;
	if (Entry.FileSize) {
		ClusterData = new char[ClusterSize];
		SizeLeft = Entry.FileSize;
		for (Cluster = Entry.StartCluster; Cluster != 0xffff; GetNextCluster(Cluster)) {
			ReadCluster(Cluster,ClusterData);
			memcpy(Buffer,ClusterData,SizeLeft > ClusterSize ? ClusterSize : SizeLeft);
			(char *)Buffer += ClusterSize;
			SizeLeft -= ClusterSize;
		}
		delete ClusterData;
	}
	return Entry.FileSize;
}

void CFAT16::ReadFAT(unsigned short Cluster)
{
	unsigned long Sector;

	Sector = (Cluster / INMEMORY_CLUSTERS) * FAT_SECTOR_COUNT + FATStart;
	Disk->Read(Sector,FAT,FAT_SECTOR_COUNT);

	FirstCluster = (Cluster / INMEMORY_CLUSTERS) * INMEMORY_CLUSTERS;
	LastCluster = FirstCluster + 4095;
}

void CFAT16::ReadDirectory(unsigned short Index, TFAT16DirEntry *Root)
{
	unsigned long Sector;

	Sector = DirStart + (Index / INMEMORY_ENTRIES) * DIR_SECTOR_COUNT;
	Disk->Read(Sector,Root,DIR_SECTOR_COUNT);
}

int CFAT16::Locate(const char *FileName, TFAT16DirEntry &Entry)
{
	TFAT16DirEntry Root[INMEMORY_ENTRIES];
	unsigned short ABSIndex;
	int Index;

	Index = INMEMORY_ENTRIES;
	for (ABSIndex = 0; ABSIndex < BootSector.RootEntries; ++Index, ++ABSIndex) {
		if (Index == INMEMORY_ENTRIES) {
			Index = 0;
			ReadDirectory(ABSIndex,Root);
		}
		if (!Root[Index].FileName[0])
			return -1;
		if (*Root[Index].FileName != 0xe5)
			if (memcmp(Root[Index].FileName,FileName,8) == 0 &&
				 memcmp(Root[Index].Extension,FileName + 8,3) == 0) {
				memcpy(&Entry,&Root[Index],sizeof (TFAT16DirEntry));
				return 0;
			 }
	}
	return -1;
}


void CFAT16::GetNextCluster(unsigned short &Cluster)
{
	if (Cluster < FirstCluster || Cluster > LastCluster)
		ReadFAT(Cluster);
	Cluster = FAT[Cluster - FirstCluster];
}

void CFAT16::ReadCluster(unsigned short Cluster, void *Buffer)
{
	unsigned long Sector;

	Sector = DataStart + (long)(Cluster - 2) * (long)BootSector.ClusterSize;
	Disk->Read(Sector,Buffer,BootSector.ClusterSize);
}