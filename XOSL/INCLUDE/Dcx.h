/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __dcx__
#define __dcx__

#include <newdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

void CreateCursor(unsigned long CursorFGnd);
void far InvertCursor(int Invert);

#ifdef __cplusplus
};
#endif

#endif