/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 *
 * This code is based on the CD-ROM booting support of Smart Boot Manager
 *
 */


#ifndef CDROM_H
#define CDROM_H

#include <newdefs.h>

class CFileSystem;
class CExtDriveParams;

class CCdRom {
public:
	CCdRom(CFileSystem &FileSystem);
	~CCdRom();

	int Install(short IdePort1, short IdePort2);
	void Uninstall();

	bool IsDriveCdRom(int Drive);

private:
	enum TBootExtFunc {
		funcInstInt13 = 0, funcUninstInt13, funcInitAtapi, 
		funcSwapDrives, funcSetIoPorts
	};

	CFileSystem &FileSystem;
	unsigned short &BaseMemSize;
	unsigned long BootExtensionAddr;

	unsigned long AllocConvMem(int KbCount);
	void FreeConvMem(int KbCount);

	int LoadBootExtension();
	void CallBootExtension(unsigned long Address, int Func, 
						   unsigned short BX, unsigned short CX);

	int GetExtDriveParams(int Drive, CExtDriveParams &Params);
};


#endif
