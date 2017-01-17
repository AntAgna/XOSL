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

class CDosFile {
public:
	enum TFileAccess {
		accessReadOnly = 0, accessWriteOnly = 1, accessReadWrite = 2
	};
	enum TFileAttr {
		attrReadOnly = 0x0001, attrHidden = 0x0002, attrSystem = 0x0004, attrArchive = 0x0020
	};
	enum TWhence {
		seekStart = 0, seekCurrent = 1, seekEnd = 2
	};

	CDosFile();
	~CDosFile();

	static int Create(const char *FileName);
	static int Open(const char *FileName, TFileAccess Access);
	static void Close(int Handle);
	static unsigned short Read(int Handle, void *Buffer, unsigned short Length);
	static unsigned short Write(int Handle, const void *Buffer, unsigned short Length);

	static int SetAttrib(const char *FileName, int Attributes);
	int Delete(const char *FileName);

	int Copy(const char *Src, const char *Dest);
	int Append(int hOutFile, const char *FileName);
	long FileSize(const char *FileName);
	void GetNameExt(const char *FileName, char *Name, char *Ext);

	static char TransferBuffer[32768];

private:
	static int Unlink(const char *FileName);
	static long LSeek(int Handle, long Offset, TWhence Whence);
};


#endif
