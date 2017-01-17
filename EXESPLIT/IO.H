/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __io__
#define __io__

#include <defs.h>

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR   2

#define S_IRDONLY 0x0001
#define S_IHIDDEN 0x0002
#define S_ISYSTEM 0x0004
#define S_IATTRIB 0x0020

_extern int creat(const char *path);
_extern int open(const char *path, int access);
_extern void close(int handle);
_extern unsigned short read(int handle, void *buf, unsigned short len);
_extern unsigned short write(int handle, void *buf, unsigned short len);
_extern int chmod(const char *path, int amode);

#endif
