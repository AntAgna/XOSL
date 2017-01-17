/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __items__
#define __items__

#include <newdefs.h>
#include <lastconf.h>
#include <ptab.h>
#include <fs.h>
#include <fatfix.h>



class CBootItem {
public:
	char ItemName[48];
	unsigned char PartIndex;
	unsigned short Hotkey;
	char Activate;
	char FixDriveNum;
	char Disabled;
		unsigned long Password;
	unsigned short Keys[16];
	unsigned char HideList[56];
	char SwapDrives;
	char Reserved[11]; // reserved for future use
}; // sizeof(CBootItem) == 158

class CPartDesc {
public:
	unsigned char Drive;
	unsigned long StartSector;
}; // sizeof(CPartDesc) == 5


class CBootItemFile {
public:
	CBootItem BootItems[24];
	CPartDesc PartList[56];
	unsigned char BootItemCount;
	unsigned char DefaultItem;
	unsigned char Reserved0; // was timeout
	unsigned char PartCount;
	unsigned short Timeout;
	char Reserved[18]; // reserved for future use
}; //  sizeof(CBootItemFile) == 4096


class CBootItems {
	public:
		CBootItems(CFileSystem &FileSystemToUse, CPartList &PartListToUse);
		~CBootItems();
		int GetCount();
		CBootItem *Get(int Index);
		int Add(CBootItem &NewItem);
		void Remove(int Index);
		void Clear();
		void Swap(int Index1, int Index2);

		
		void Save();
		void Reset();

		void SetDefault(int Index);
		int GetDefault();
		int GetTimeout();
		void SetTimeout(int Timeout);

		int InitBoot(int BootIndex);

		bool IsDefault(int Index);
		bool CanFixFAT(int Index);
		bool CanSwap(int Index);

	private:
		CDriveFix DriveFix;
		CBootItemFile ItemFile;
		
		CFileSystem &FileSystem;
		CPartList &PartList;
            
		void MoveUp(int Index);
		void StoreKeys(const unsigned short *Keys);

		void Initialize();
	    bool PartListIsChanged();
		void AdjustPartList();

		void BuildRelocTable(int *RelocTable);
		void UpdatePartList(int *RelocTable);

		void UpdatePartIndex(CBootItem &BootItem, int *RelocTable);
		void UpdateHideList(unsigned char *HideList, int *RelocTable);

};



#endif