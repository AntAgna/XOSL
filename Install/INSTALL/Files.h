/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef FilesH
#define FilesH

#include <defs.h>

class CXoslFiles {
public:
	CXoslFiles();
	~CXoslFiles();
	int GetCount();
	const char *GetFileName(int Index);
	const char *GetXoslDataName();
	const char *GetBootItemName();

	const char *GetCurrentMbrName();
	const char *GetOriginalMbrName();
	const char *GetDefaultMbrName();
	const char *GetSmartBmName();

	void IgnorePartManFiles();

	enum TFileIpl { enumIpl16Conv = 0, enumIpl32Conv, enumIpl16Lba, enumIpl32Lba, enumIplS, enumIplSLba};

	const char *GetIplFileName(TFileIpl IplToUse);


private:
	static const char *FileList[];
	static const char *IplFileList[];
	static const int FileCount;


	static const char *XoslData;
	static const char *BootItem;
	static const char *CurrentMBR;
	static const char *OriginalMBR;

	bool IgnoreRPM;

};


#endif