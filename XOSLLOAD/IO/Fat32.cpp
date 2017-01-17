/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <fat32.h>
#include <disk.h>
#include <mem.h>

#define INMEMORY_CLUSTERS 4096
#define FAT_SECTOR_COUNT  (INMEMORY_CLUSTERS / 128) // 128 clusters/sector

#define FILE_DELETED 0xe5

CFAT32::CFAT32(): CFileSystem()
{
	FAT = new long[INMEMORY_CLUSTERS];
}

CFAT32::~CFAT32()
{
	delete FAT;
}

int CFAT32::Mount(int Drive, long StartSector)
{
	int Status;
	long FATSize;

	Status = CFileSystem::Mount(Drive,StartSector);
	if (Status != -1) {
		Disk->Read(0,&BootSector,1);
		if (memcmp(BootSector.FSID,"FAT32   ",8) != 0)
			Status = -1;
		else {
			ClusterSize = (unsigned short)BootSector.ClusterSize << 9;

			FATSize = BootSector.BigFATSize ? BootSector.BigFATSize : BootSector.FATSize;

			FATStart = BootSector.ReservedSectors;
			DataStart = FATStart + FATSize * (long)BootSector.FATCopies;
			LastCluster = 0;
		}
	}
	return Status;
}

unsigned short CFAT32::ReadFile(const char *FileName, void *Buffer)
{
	long Cluster;
	TFAT32DirEntry Entry;
	void *ClusterData;
	unsigned long SizeLeft;

	if (Locate(FileName,Entry) == -1)
		return 0;
	if (Entry.FileSize) {
		ClusterData = new char[ClusterSize];
		SizeLeft = Entry.FileSize;
		Cluster = (long)Entry.StartClusterL + ((long)Entry.StartClusterH << 16);
		for (; Cluster != 0x0fffffff; GetNextCluster(Cluster)) {
			ReadCluster(Cluster,ClusterData);
			memcpy(Buffer,ClusterData,SizeLeft > ClusterSize ? ClusterSize : SizeLeft);
			(char *)Buffer += ClusterSize;
			SizeLeft -= ClusterSize;
		}
		delete ClusterData;
	}
	return Entry.FileSize;
}

void CFAT32::ReadFAT(long Cluster)
{
	unsigned long Sector;

	Sector = (Cluster / INMEMORY_CLUSTERS) * FAT_SECTOR_COUNT + FATStart;
	Disk->Read(Sector,FAT,FAT_SECTOR_COUNT);

	FirstCluster = (Cluster / INMEMORY_CLUSTERS) * INMEMORY_CLUSTERS;
	LastCluster = FirstCluster + 4095;
}

int CFAT32::Locate(const char *FileName, TFAT32DirEntry &Entry)
{
	TFAT32DirEntry *Entries;
	int EntryCount;
	int Index;
	long Cluster;

	EntryCount = ClusterSize / sizeof (TFAT32DirEntry);
	Entries = new TFAT32DirEntry[EntryCount];
	for (Cluster = BootSector.RootCluster; Cluster != 0x0fffffff; GetNextCluster(Cluster)) {
		ReadCluster(Cluster,Entries);
		for (Index = 0; Index < EntryCount; ++Index) {
			if (!Entries[Index].FileName[0]) {
				delete Entries;
				return -1;
			}
			if (*Entries[Index].FileName != 0xe5)
				if (memcmp(Entries[Index].FileName,FileName,8) == 0 &&
					 memcmp(Entries[Index].Extension,FileName + 8,3) == 0) {
					memcpy(&Entry,&Entries[Index],sizeof (TFAT32DirEntry));
					delete Entries;
					return 0;
				}
		}
	}
	delete Entries;
	return -1;
}


void CFAT32::GetNextCluster(long &Cluster)
{
	if (Cluster < FirstCluster || Cluster > LastCluster)
		ReadFAT(Cluster);
	Cluster = FAT[Cluster - FirstCluster];
}

void CFAT32::ReadCluster(long Cluster, void *Buffer)
{
	unsigned long Sector;

	Sector = DataStart + (long)(Cluster - 2) * (long)BootSector.ClusterSize;
	Disk->Read(Sector,Buffer,BootSector.ClusterSize);
}

