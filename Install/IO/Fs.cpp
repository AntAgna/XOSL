/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <fs.h>
#include <disk.h>

CFileSystem::CFileSystem()
{
}

CFileSystem::~CFileSystem()
{
}

int CFileSystem::Mount(int Drive, unsigned long StartSector)
{
	return Disk.Map(Drive,StartSector);
}

