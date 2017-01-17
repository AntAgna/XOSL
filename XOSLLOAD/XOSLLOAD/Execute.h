/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __execute__
#define __execute__

#include <defs.h>

_extern void Execute(unsigned short StartSeg, unsigned short ReloSS,
				 unsigned short ExeSP, unsigned short ReloCS,
				 unsigned short ExeIP);

#endif