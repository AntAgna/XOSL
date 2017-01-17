/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

/*
 * CDisk
 * Map a hard disk on sector level
 * Uses LBA access when available (necessary for 8GB+ disks)
 */

#ifndef __disk__
#define __disk__

#include <defs.h>
#include <transfer.h>

class CDisk {
	public:
		CDisk();
		~CDisk();

		int DriveCount(int Fixed);

		int Map(int Drive, long StartSector);

		int Read(long Sector, void *Buffer, int Count);
		int Write(long Sector, const void *Buffer, int Count);
		int Verify(long Sector, int Count);

		void Lock();
		void Unlock();

	private:
		int Transfer(int Action, long Sector, void *Buffer, int Count);
		void Sector2CHS(long RSector, unsigned short &SectCyl, unsigned short &DrvHead);

		CDiskAccess DiskAccess;

		int DrvHeadCount;
		int DrvSectorCount;
		int Drive;
		long StartSector;
		int UseLBA;
		int DiskMapped;
};

#endif