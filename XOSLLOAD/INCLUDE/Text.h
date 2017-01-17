/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __text__
#define __text__

#include <defs.h>

_extern void PutS(const char far *Str);
_extern void PutCh(int Ch);
_extern int GetCh(void);

#endif