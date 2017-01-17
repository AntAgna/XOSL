/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __ptab__
#define __ptab__

#include <defs.h>

#define PART_PRIMARY 0
#define PART_LOGICAL 1
#define PART_MBR     2
#define PART_FLOPPY  3

typedef struct {
	unsigned char Activated;
	unsigned char StartHead;
	unsigned short StartSectCyl;
	unsigned char FSType;
	unsigned char EndHead;
	unsigned short EndSectCyl;
	unsigned long RelativeSector;
	unsigned long SectorCount;
} TPartEntry;

typedef struct {
	char IPL[446];
	TPartEntry Entries[4];
	unsigned short MagicNumber; // 0xaa55
} TPartTable;

typedef struct S_MBRNode {
	unsigned long AbsoluteSector;
	short Drive;
	short Type; // primary or logical
	TPartTable *Table;
	struct S_MBRNode *Next;
} TMBRNode;

typedef struct {
	short Drive;
	unsigned long StartSector;
	unsigned long SectorCount;
	const char *FSName;
	short FSType;
	short Type; // primary, logical, mbr or floppy
} TPartition;

typedef struct S_PartNode {
	TPartEntry *Entry;
	TPartition *Partition;
	struct S_PartNode *Next;
} TPartNode;

class CPartList {
	public:
		CPartList();
		~CPartList();
		void ReadStructure();
		void WriteStructure();
		const TPartition *GetPartition(int Index);
		int Locate(int Drive, unsigned long StartSector);
		int GetCount();
		int CanHide(int Index);
		void Hide(int Index);
		void Reveal(int Index);
		int CanActivate(int Index);
		void SetAllowActiveHD(int Status);
		void SetActive(int Index);
		void SetFsType(int Index, int FsType);
	public:
		typedef struct {
			int FSID;
			const char *FSName;
		} TFSNameEntry;
	private:
		void ClearActive(int Drive);

		TMBRNode *AddDrive(int Drive, unsigned long StartSector, unsigned long ExtStart, int Type, TMBRNode *MBRList);
		void CreatePartList(int FloppyCount);
		TPartNode *CreatePartNode(const TMBRNode *MBRNode, int Index);
		TPartNode *CreateNonPartNode(int Drive);
		const char *GetFSName(int FSID);
		void CreatePLUP();

		TMBRNode MBRList;
		TPartNode PartList;
		int Count;
		TPartNode **PLUP;
		static TFSNameEntry FSNameList[];
		int AllowActiveHD;
};



#endif