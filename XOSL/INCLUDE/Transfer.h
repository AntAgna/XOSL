/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __transfer__
#define __transfer__

#include <newdefs.h>

//#define Scratchpad ( (void *)0x90008000 )
#define Scratchpad ( (void *)0x00008000 )

#define DISK_READ   0x0200
#define DISK_WRITE  0x0300
#define DISK_VERIFY 0x0400

typedef struct {
	int PacketSize;
	int SectorCount;
	void *TransferBuffer;
	unsigned long SectorLow;
	unsigned long SectorHigh;
} TLBAPacket;

class CDiskAccess {
	public:
		CDiskAccess();
		~CDiskAccess();

		// conventional stuff
		int DriveCount(int Fixed);
		int Transfer(int Action, unsigned short SectCyl,
						 unsigned short DrvHead, void *Buffer, int Count);
		int GetDriveInfo(int Drive, int &Heads, int &Sectors);
		int CopyFromScratchpad(void *Buffer, int Sectors);
		int CopyToScratchpad(const void *Buffer, int Sectors);

		// lba stuff
		int LBAAccessAvail(int Drive);
		int LBATransfer(int Action, int Drive, const TLBAPacket &LBAPacket);
};

#endif
