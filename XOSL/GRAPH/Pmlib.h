/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __pmlib__
#define __pmlib__

#include <newdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

void PMMemCopy(unsigned long Dest, unsigned long Src, unsigned long Count);

#ifdef __cplusplus
};
#endif


#endif