/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __dosdrv__
#define __dosdrv__

#include <defs.h>
#include <ptab.h>

#define FATTYPE_FAT16 16
#define FATTYPE_FAT32 32

class CDosDriveList {
	public:
		class CDosDrive {
		public:
			int DriveChar; // 'C', 'D', ...
			int FATType;
			int Drive;
			unsigned long StartSector;
		};


		CDosDriveList(CPartList &PartListToUse);
		~CDosDriveList();
		int LocateDrive(int DriveNum, CDosDrive &DosDrive);
	private:
		CPartList &PartList;

		unsigned long GetDosSerialNo(int DriveNum);
		unsigned long GetBRecSerialNo(int Index);

};

#endif