/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */



#include <transfer.h>




//static void Sector2CHS(long RSector, unsigned short &SectCyl, unsigned short &DrvHead);


void DiskMap(int DriveToUse, long StartSectorToUse)
{
	Drive = DriveToUse;
	StartSector = StartSectorToUse;
	GetDriveInfo(DriveToUse);
}

void DiskRead(long Sector, void *Buffer, int Count)
{
	unsigned short SectCyl, DrvHead;

	Sector2CHS(Sector,SectCyl,DrvHead);
	ConvRead(SectCyl,DrvHead,Buffer,Count);
}

/*void Sector2CHS(long RSector, unsigned short &SectCyl, unsigned short &DrvHead)
{
	int Cylinder;
	int Sector;
	int Head;

	RSector += StartSector;

	Sector = RSector % DrvSectorCount + 1;
	RSector /= DrvSectorCount;
	Head = RSector % DrvHeadCount;
	Cylinder = RSector / DrvHeadCount;

	DrvHead = Drive | (Head << 8);
	SectCyl = Sector | ((Cylinder & 0xff) << 8) | ((Cylinder >> 8) << 6);
}
*/
