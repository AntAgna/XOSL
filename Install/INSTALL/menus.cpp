/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#include "menus.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>

static const char *YesNoList[2] = {"Yes", "No"};


CInstallMenus::CInstallMenus(CTextUI &TextUIToUse, CData &DataToUse, CPartList &PartListToUse, int FatAvailToUse):
	TextUI(TextUIToUse),
	Data(DataToUse),
	PartList(PartListToUse)
{
	FatAvail = FatAvailToUse;
	DosDriveList = NULL;
	PartNameList = NULL;
}

CInstallMenus::~CInstallMenus()
{
	if (DosDriveList) {
		delete DosDriveList[0];
		delete DosDriveList;
	}
}

void CInstallMenus::InitMainMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	
	TextUI.ClearMenu();
	TextUI.SetMenuTitle(" Main \xcd\xcd\xcd\xcd\xcd");
	TextUI.AddMenuItem(0,"Install XOSL","Install Extended Operating System Loader 1.1.5",1);
	TextUI.AddMenuItem(2,"Uninstall XOSL","Uninstall Extended Operating System Loader 1.1.5",1);
	TextUI.AddMenuItem(4,"Restore XOSL","Make your system start XOSL again (typically after installing an OS)",1);

	TextUI.AddMenuItem(6,"Exit install","Quit without installing Extended Operating System Loader 1.1.5",1);
	TextUI.SetItemIndex(0);
	TextUI.RefreshMenu();
}

void CInstallMenus::InitInstallMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	
	TextUI.ClearMenu();
	TextUI.SetMenuTitle(" Install \xcd\xcd");
	TextUI.AddMenuItem(0,"Install on a DOS drive","Install XOSL 1.1.5 on a DOS drive (FAT16/FAT32)",FatAvail);
	TextUI.AddMenuItem(2,"Install on a dedicated partition","Install XOSL 1.1.5 on an existing dedicated partition",1);
	TextUI.AddMenuItem(6,"Return to main menu","Return to main menu",1);
	TextUI.SetItemIndex(FatAvail ? 0 : 2);
	TextUI.RefreshMenu();
}

void CInstallMenus::InitFixMbrMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	
	TextUI.ClearMenu();
	TextUI.SetMenuTitle(" Restore \xcd\xcd");
	TextUI.AddMenuItem(0,"Restore on a DOS drive","Restore XOSL 1.1.5, installed on a DOS drive",FatAvail);
	TextUI.AddMenuItem(2,"Restore on a dedicated partition","Restore XOSL 1.1.5, installed on a dedicated partition",1);
	TextUI.AddMenuItem(6,"Return to main menu","Return to main menu",1);
	TextUI.SetItemIndex(FatAvail ? 0 : 2);
	TextUI.RefreshMenu();
}

void CInstallMenus::InitUninstallMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	
	TextUI.ClearMenu();
	TextUI.SetMenuTitle(" Uninstall ");
	TextUI.AddMenuItem(0,"Uninstall off a DOS drive","Uninstall XOSL 1.1.5 off a DOS drive",FatAvail);
	TextUI.AddMenuItem(2,"Uninstall off a dedicated partition","Uninstall XOSL 1.1.5 off a dedicated partition",1);
	TextUI.AddMenuItem(6,"Return to main menu","Return to main menu",1);
	TextUI.SetItemIndex(FatAvail ? 0 : 2);
	TextUI.RefreshMenu();
}

void CInstallMenus::InitInstFatMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	const char **ModeNameList;
	int ModeCount;
	const char **MouseTypeNames;
	int MouseTypeCount;

	Data.GetGraphicsModeNames(ModeNameList,ModeCount);
	Data.GetMouseTypeNames(MouseTypeNames,MouseTypeCount);
	if (!DosDriveList)
		CreateDosDriveList();

	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	TextUI.ClearMenu();
	TextUI.AddMenuItem(0,"Video mode","Initial video mode",1,ModeCount,ModeNameList,1);
	TextUI.AddMenuItem(1,"Mouse type","Initial mouse type",1,MouseTypeCount,MouseTypeNames,1);
	TextUI.AddMenuItem(2,"Install on drive","Drive to install XOSL on",1,DosDriveCount,(const char **)DosDriveList,1);
	TextUI.AddMenuItem(3,"Ranish Partition Manager","Install Ranish Partition Manager 2.38 beta 1.9 together with XOSL 1.1.5",1,2,YesNoList,1);
	TextUI.AddMenuItem(4,"Smart Boot Manager","Install Smart Boot Manager 3.6.4 for CD-ROM booting support.",1,2,YesNoList,1);

	TextUI.AddMenuItem(6,"Start installation","Install Extended Operating System Loader 1.1.5",1);
	TextUI.AddMenuItem(8,"Return to install menu","Return to install menu",1);
	TextUI.SetItemIndex(0);
	TextUI.RefreshMenu();
}

void CInstallMenus::InitInstSepMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	const char **ModeNameList;
	int ModeCount;
	const char **MouseTypeNames;
	int MouseTypeCount;

	Data.GetGraphicsModeNames(ModeNameList,ModeCount);
	Data.GetMouseTypeNames(MouseTypeNames,MouseTypeCount);
	if (!PartNameList)
		CreatePartList();

	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	TextUI.ClearMenu();
	TextUI.AddMenuItem(0,"Video mode","Initial video mode",1,ModeCount,ModeNameList,1);
	TextUI.AddMenuItem(1,"Mouse type","Initial mouse type",1,MouseTypeCount,MouseTypeNames,1);
	TextUI.AddMenuItem(2,"Ranish Partition Manager","Install Ranish Partition Manager 2.38 beta 1.9 together with XOSL 1.1.5",1,2,YesNoList,1);
	TextUI.AddMenuItem(3,"Smart Boot Manager","Install Smart Boot Manager 3.6.4 for CD-ROM booting support.",1,2,YesNoList,1);
	TextUI.AddMenuItem(4,"Drv Type System              Size ","Partition to install XOSL 1.1.5 on",1,PartNameCount,(const char **)PartNameList,0);
	TextUI.AddMenuItem(10,"Start installation","Install Extended Operating System Loader 1.1.5",1);
	TextUI.AddMenuItem(11,"Return to install menu","Return to install menu",1);
	TextUI.SetItemIndex(0);
	TextUI.RefreshMenu();
}


void CInstallMenus::InitErrorMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	TextUI.ClearMenu();
	TextUI.SetMenuTitle(" Error \xcd\xcd\xcd\xcd");
	TextUI.AddMenuItem(0,"An error has occured!","",0);
	TextUI.AddMenuItem(2,"Exit install","Quit without installing Extended Operating System Loader 1.1.5",1);
	TextUI.AddMenuItem(4,"Return to main menu","Return to main menu",1);
	TextUI.SetItemIndex(2);
	TextUI.RefreshMenu();
}

void CInstallMenus::InitDoneMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	TextUI.ClearMenu();
	TextUI.AddMenuItem(0,"Extended Operating System Loader","",0);
	TextUI.AddMenuItem(1,"has been successfully installed","",0);

	TextUI.AddMenuItem(3,"Reboot system","Quit install and reboot to run XOSL for the first time",1);
	TextUI.AddMenuItem(5,"Exit install","Quit install without rebooting",1);
	TextUI.AddMenuItem(7,"Return to main menu","Return to main menu",1);
	TextUI.SetItemIndex(3);
	TextUI.RefreshMenu();
}

void CInstallMenus::InitUnDoneMenu(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);
	TextUI.ClearMenu();
	TextUI.AddMenuItem(0,"Extended Operating System Loader","",0);
	TextUI.AddMenuItem(1,"has been successfully uninstalled","",0);

	TextUI.AddMenuItem(3,"Reboot system","Quit install and reboot",1);
	TextUI.AddMenuItem(5,"Exit install","Quit install",1);
	TextUI.AddMenuItem(7,"Return to main menu","Return to main menu",1);
	TextUI.SetItemIndex(3);
	TextUI.RefreshMenu();
}

void CInstallMenus::InitUninstallFat(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	static const char *MbrIplList[2] = { "Original","Default" };

	if (!DosDriveList)
		CreateDosDriveList();

	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);

	TextUI.ClearMenu();
	TextUI.AddMenuItem(0,"Uninstall off drive","Drive XOSL is currently installed on",1,DosDriveCount,(const char **)DosDriveList,1);
	TextUI.AddMenuItem(1,"Restore MBR","Restore the Master Boot Record with the original or default loader",1,2,MbrIplList,1);
	TextUI.AddMenuItem(4,"Start uninstall","Uninstall Extended Operating System Loader",1);
	TextUI.AddMenuItem(6,"Return to uninstall menu","Return to uninstall menu",1);
	TextUI.SetItemIndex(0);
	TextUI.RefreshMenu();

}

void CInstallMenus::InitUninstallSep(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	static const char *MbrIplList[2] = { "Original","Default" };

	if (!PartNameList)
		CreatePartList();

	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);

	TextUI.ClearMenu();
	TextUI.AddMenuItem(0,"Restore MBR","Restore the Master Boot Record with the original or default loader",1,2,MbrIplList,1);
	TextUI.AddMenuItem(2,"Drv Type System              Size ","Partition XOSL is currently installed on",1,PartNameCount,(const char **)PartNameList,0);
	TextUI.AddMenuItem(8,"Start uninstall","Uninstall Extended Operating System Loader",1);
	TextUI.AddMenuItem(10,"Return to uninstall menu","Return to uninstall menu",1);
	TextUI.SetItemIndex(0);
	TextUI.RefreshMenu();

}

void CInstallMenus::InitFixMbrFat(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	if (!DosDriveList)
		CreateDosDriveList();

	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);

	TextUI.ClearMenu();
	TextUI.AddMenuItem(0,"XOSL on drive","DOS drive XOSL is currently installed on",1,DosDriveCount,(const char **)DosDriveList,1);
	TextUI.AddMenuItem(4,"Start restore","Restore XOSL 1.1.5",1);
	TextUI.AddMenuItem(6,"Return to restore menu","Return to restore menu",1);
	TextUI.SetItemIndex(0);
	TextUI.RefreshMenu();

}

void CInstallMenus::InitFixMbrSep(CTextList::TListItemExecute MenuHandler, void *HandlerClass)
{
	if (!PartNameList)
		CreatePartList();

	TextUI.ConnectEventHandler(MenuHandler,HandlerClass);

	TextUI.ClearMenu();
	TextUI.AddMenuItem(0,"Drv Type System              Size ","Partition XOSL is currently installed on",1,PartNameCount,(const char **)PartNameList,0);
	TextUI.AddMenuItem(6,"Start restore","Restore XOSL 1.1.5",1);
	TextUI.AddMenuItem(8,"Return to upgrade menu","Return to upgrade menu",1);
	TextUI.SetItemIndex(0);
	TextUI.RefreshMenu();

}


void CInstallMenus::CreateDosDriveList()
{
	int Index;
	int *Buffer;

	DosDriveCount = Data.GetLastDrive() - ('C' - 'A');
	DosDriveList = new char *[DosDriveCount];
	Buffer = new int[DosDriveCount];
	for (Index = 0; Index < DosDriveCount; ++Index) {
		Buffer[Index] = Index + 'C';
		DosDriveList[Index] = (char *)&Buffer[Index];
	}
}

void CInstallMenus::CreatePartList()
// TODO: Implement the sprintf() function
{
	char *PartStr;
	const TPartition *Partition;
	int Index, Count;

	Count = PartList.GetCount();
	PartNameList = new char *[Count];
	PartResolveList = new int[Count];

	for (PartNameCount = Index = 0; Index < Count; ++Index) {
		Partition = PartList.GetPartition(Index);
		switch (Partition->Type) {
			case PART_PRIMARY:
			case PART_LOGICAL:
				switch (Partition->FSType) {
					case 0x05: // Extended
					case 0x0f: // Extended LBA
					case 0x85: // Linux Extended
						PartResolveList[Index] = -1;
						break;
					default:
						PartResolveList[Index] = PartNameCount;

						PartStr = new char [48];
						MemSet(PartStr,' ',32);
						MemCopy(PartStr,"HD",2);
						PartStr[2] = '0' + (Partition->Drive & ~0x80);
						MemCopy(&PartStr[4],Partition->Type == PART_PRIMARY ? "pri" : "log",3);
						MemCopy(&PartStr[9],Partition->FSName,strlen(Partition->FSName));
						itoa(Partition->SectorCount >> 11,&PartStr[29],10);
						PartNameList[PartNameCount++] = PartStr;
						break;
				}
				break;
			default:
				break;
		}
	}

}

int CInstallMenus::ResolvePartIndex(int ListIndex)
{
	int Count, Index;

	Count = PartList.GetCount();
	for (Index = 0; Index < Count; ++Index)
		if (PartResolveList[Index] == ListIndex)
			return Index;
	return -1;
}
