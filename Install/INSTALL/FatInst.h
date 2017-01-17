/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#ifndef FatInstH
#define FatInstH

#include <ipl.h>
#include <dosdrv.h>
#include <textui.h>
#include <files.h>
#include <dosio.h>

class CFatInstall {
public:
	CFatInstall(CTextUI &TextUIToUse, CXoslFiles &XoslFilesToUse, CDosFile &DosFileToUse);
	~CFatInstall();

	int CreateIpl(const CDosDriveList::CDosDrive &DosDrive, TIPL &Ipl);
	int InstallFiles(const CDosDriveList::CDosDrive &DosDrive);
	int InstallIpl(void *Ipl);

	void RemoveXoslFiles(char DosDriveChar);


private:
	CTextUI &TextUI;
	CXoslFiles &XoslFiles;
	CDosFile &DosFile;

	int CreateIplFat16(const CDosDriveList::CDosDrive &DosDrive, int UseLba, TIPL &Ipl);
	int CreateIplFat32(const CDosDriveList::CDosDrive &DosDrive, int UseLba, TIPL &Ipl);
};

#endif
