/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __cursor__
#define __cursor__

#include <newdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

void DrawCursor(void);
void ClearCursor(void);

#ifdef __cplusplus
};
#endif

extern int CursorX;
extern int CursorY;
extern int CursorVisible;

#endif