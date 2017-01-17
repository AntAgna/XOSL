/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __exestruc__
#define __exestruc__

typedef struct {
	unsigned short Signature;
	unsigned short PartPage;
	unsigned short PageCount;
	unsigned short ReloCount;
	unsigned short HeaderSize;
	unsigned short MinMem;
	unsigned short MaxMem;
	unsigned short ReloSS;
	unsigned short ExeSP;
	unsigned short ChkSum;
	unsigned short ExeIP;
	unsigned short ReloCS;
	unsigned short TableOff;
	unsigned short Overlay;
} TExeHeader;


#endif