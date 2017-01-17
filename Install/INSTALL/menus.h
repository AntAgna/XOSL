/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __MenuDraw__
#define __MenuDraw__

#include <textui.h>
#include <data.h>
#include <ptab.h>

class CInstallMenus {
public:
	CInstallMenus(CTextUI &TextUIToUse, CData &DataToUse, CPartList &PartListToUse, int FatAvailToUse);
	~CInstallMenus();

	void InitMainMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitInstallMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitFixMbrMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitUninstallMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitInstFatMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitInstSepMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitErrorMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitDoneMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitUnDoneMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass);

	void InitUninstallFat(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitUninstallSep(CTextList::TListItemExecute MenuHandler, void *HandlerClass);

	void InitFixMbrFat(CTextList::TListItemExecute MenuHandler, void *HandlerClass);
	void InitFixMbrSep(CTextList::TListItemExecute MenuHandler, void *HandlerClass);

	int ResolvePartIndex(int ListIndex);

private:
	CTextUI &TextUI;
	CData &Data;
	CPartList &PartList;

	char **DosDriveList;
	int DosDriveCount;
	void CreateDosDriveList();

	char **PartNameList;
	int PartNameCount;
	void CreatePartList();

	int *PartResolveList;

	int FatAvail;
};

#endif
