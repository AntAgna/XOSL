/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#include <translba.h>


int Drive;
unsigned long StartSector;


void DiskMap(int DriveToUse, unsigned long StartSectorToUse)
{
	Drive = DriveToUse;
	StartSector = StartSectorToUse;
}

void DiskRead(unsigned long Sector, void *Buffer, int Count)
{
	TLBAPacket LBAPacket;

	LBAPacket.PacketSize = 0x0010;
	LBAPacket.SectorCount = Count;
	LBAPacket.TransferBuffer = Buffer;
	LBAPacket.SectorLow = Sector + StartSector;
	LBAPacket.SectorHigh = 0;
	LBARead(Drive,LBAPacket);
}

