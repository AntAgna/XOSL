/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __filesystem__
#define __filesystem__

class CDisk;

class CFileSystem {
	public:
		CFileSystem();
		virtual ~CFileSystem();
		virtual int Mount(int Drive, unsigned long StartSector);
		virtual unsigned short ReadFile(const char *FileName, void *Buffer) = 0;
	protected:
		CDisk *Disk;
};

extern CFileSystem *FileSystem;


#endif