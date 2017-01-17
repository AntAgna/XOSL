/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __dos__
#define __dos__

#include <defs.h>

typedef struct {
	unsigned short Reserved; /* 0000 */
	unsigned long SerialNumber;
	char VolumeLabel[11];
	char FSType[8];
} TDriveInfo;

_extern int GetDriveInfo(int Drive, TDriveInfo &DriveInfo);
_extern int GetDriveCount(void);

_extern const int DOSErrCode;

#endif