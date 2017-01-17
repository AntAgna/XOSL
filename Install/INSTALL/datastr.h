/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#ifndef DataStructH
#define DataStructH

// namepaces not supported by BC++ 3.1


/*
 * XOSL 1.1.0.dv1 uses CItemFile of CXosl100DataStructs
 * and CLastConfig of CXosl110DataStructs
 */


class CXosl100DataStructs {
public:
// XoslData.xdf isn't changed. Only additions are made.

	class CItemFile {
	public:
		class CBootItem {
		public:
			char ItemName[48];
			unsigned char Keys[16];
			short Activate;
			unsigned long Password;
			short PartIndex;
			short Valid;
			char HidingItems[92];
		};

		short BootItemCount;
		short PartCount;
		short InstantBoot;
		short Timeout;
		short Reserved[52];
		CBootItem BootItems[24];
	};


	class CLastConfig {
	public:
		class CConfPart {
		public:
			class CPTabEntry {
			public:
				unsigned char Activated;
				unsigned char StartHead;
				unsigned short StartSectCyl;
				unsigned char FSType;
				unsigned char EndHead;
				unsigned short EndSectCyl;
				unsigned long ABSSectorStart;
				unsigned long SectorCount;
			};

			CPTabEntry PTabEntry;
			unsigned short SectCyl;
			unsigned short DrvHead;
			char Index;
			char Type;
		};

		int LastBooted;
		int PartitionCount;
		CConfPart Partitions[92];
		int Reserved[10];
	};

};


class CXosl110DataStructs {
public:
	class CItemFile {
	public:
		class CBootItem {
		public:
			char ItemName[48];
			unsigned char Keys[16];
			short Activate;
			unsigned long Password;
			short PartIndex;
			short Valid;
			unsigned short Hotkey;
			char FixDriveNum;
			char Reserved;
			char HidingItems[92];
		};

		short BootItemCount;
		short PartCount;
		short InstantBoot;
		short Timeout;
		short Reserved[4];
		CBootItem BootItems[24];
	};


	class CLastConfig {
	public:
		class CPartKey {
		public:
			unsigned char Drive;
			unsigned long StartSector;
		};

		int PartitionCount;
		CPartKey LastPartList[92];
		char Reserved[50];
	};
};

#endif