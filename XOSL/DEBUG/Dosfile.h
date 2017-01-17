/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __dosfile__
#define __dosfile__

#include <newdefs.h>

int DosCreate(const char *FileName);
void DosClose(int FileHandle);
void DosWrite(int FileHandle, const void *Data, unsigned short Size);

#endif
