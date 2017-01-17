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


void GetDriveInfo(int Drive);
void ConvRead(unsigned short SectCyl,unsigned short DrvHead, void *Buffer, int Count);
void Sector2CHS(long RSector, unsigned short &SectCyl, unsigned short &DrvHead);


extern long DrvHeadCount;
extern long DrvSectorCount;
extern int Drive;
extern long StartSector;


#endif
