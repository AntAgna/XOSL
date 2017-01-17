/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#include <stdio.h>
#include <main.h>
#include <text.h>


static const char *InstallWarning = 
"PLEASE READ THE FOLLOWING CAREFULLY\n"
"\n"
"You have selected to install XOSL on a dedicated\n"
"partition. Note that this is a destructive action. ALL\n"
"DATA ON THE SELECTED PARTITION WILL BE DESTROYED! So\n"
"make sure you double-check the partition you select,\n"
"that it is ABSOLUTELY the partition to install XOSL on.\n"
"\n"
"Also, XOSL does not create its own partition. Instead,\n"
"you have to create a dedicated partition for XOSL\n"
"yourself, BEFORE you run install.\n"
"\n"
"Press any key to continue...";

const char InstallNotes[] =
"Extended Operating System Loader 1.1.5\n"
"\n"
"Make sure you have read NOTES.TXT!\n"
"\n"
"Before installling XOSL 1.1.5, it is\n"
"highly recommended you first read the\n"
"installation walkthrough, found in the\n"
"manual/install directroy\n"
"\n"
"Press any key to continue...";



void main()
{
	puts("XOSL INSTALL UTILITY");
	puts("Copyright (c) 1999/2000, Geurt Vos");
	
	CApplication *Application = new CApplication;
	Application->ApplicationLoop();
	delete Application;
}


CApplication::CApplication():
	TextScreen(new CTextScreen(0x1f00)),
	TextUI(*TextScreen),
	Data(TextUI,PartList),
	Installer(TextUI,PartList),
	InstallMenus(TextUI,Data,PartList,(CData::GetDosVersion() & 0xff) > 4)
{
	DoExit = 0;
	DoReboot = 0;
	TextUI.OutputStr("Extended Operating System Loader\ncomes with ABSOLUTELY NO WARRANTY\n\n");

	InstallMenus.InitMainMenu((CTextList::TListItemExecute)MainMenuExecute,this);
	
	TextUI.ShowPopup(18,5,44,14,InstallNotes);
	

}

CApplication::~CApplication()
{
	delete TextScreen;
	if (DoReboot)
		Reboot();
}


void CApplication::ApplicationLoop()
{
	int Key;

	while (!DoExit) {
		while (!Keyboard.KbHit());
		Key = Keyboard.GetCh();
		if (Key == KEY_ESCAPE)
			DoExit = 1;
		TextUI.HandleKey(Key);
	}

}


int CApplication::StartInstallFat()
{
	CVesa::TGraphicsMode GraphicsMode;
	CMouse::TMouseType MouseType;

	CDosDriveList DosDriveList(PartList);
	CDosDriveList::CDosDrive DosDrive;
	int DosDriveIndex;
	bool PartMan;
	bool SmartBootManager;

	GraphicsMode = Data.GetGraphicsMode(TextUI.GetOptionIndex(0));
	MouseType = Data.GetMouseType(TextUI.GetOptionIndex(1));
	DosDriveIndex = TextUI.GetOptionIndex(2);
	PartMan = TextUI.GetOptionIndex(3) == 0;
	SmartBootManager = TextUI.GetOptionIndex(4) == 0;

	if (DosDriveList.LocateDrive(DosDriveIndex,DosDrive) == -1) {
		TextUI.OutputStr("Unable to locate drive %c:\n",'C' + DosDriveIndex);
		return -1;
	}

	if (Installer.Install(GraphicsMode,MouseType,DosDrive,PartMan,SmartBootManager) == -1) {
		TextUI.OutputStr("Install error\n");
		return -1;
	}

	return 0;
}

int CApplication::StartInstallSep()
{
	CVesa::TGraphicsMode GraphicsMode;
	CMouse::TMouseType MouseType;
	int PartIndex;
	bool PartMan;
	bool SmartBootManager;
	
	GraphicsMode = Data.GetGraphicsMode(TextUI.GetOptionIndex(0));
	MouseType = Data.GetMouseType(TextUI.GetOptionIndex(1));
	PartMan = TextUI.GetOptionIndex(2) == 0;
	SmartBootManager = TextUI.GetOptionIndex(3) == 0;

	PartIndex = InstallMenus.ResolvePartIndex(TextUI.GetOptionIndex(4));

	if (Installer.Install(GraphicsMode,MouseType,PartIndex,PartMan,SmartBootManager) == -1) {
		TextUI.OutputStr("Install error\n");
		return -1;
	}
	return 0;
}


int CApplication::StartRestoreFat()
{
	CDosDriveList DosDriveList(PartList);
	CDosDriveList::CDosDrive DosDrive;
	int DosDriveIndex;

	DosDriveIndex = TextUI.GetOptionIndex(0);

	if (DosDriveList.LocateDrive(DosDriveIndex,DosDrive) == -1) {
		TextUI.OutputStr("Unable to locate drive %c:\n",'C' + DosDriveIndex);
		return -1;
	}

	if (Installer.Restore(DosDrive) == -1) {
		TextUI.OutputStr("Install error\n");
		return -1;
	}
	return 0;

}

int CApplication::StartRestoreSep()
{
	int PartIndex;
	
	PartIndex = InstallMenus.ResolvePartIndex(TextUI.GetOptionIndex(0));

	if (Installer.Restore(PartIndex) == -1) {
		TextUI.OutputStr("Install error\n");
		return -1;
	}

	return 0;
}


int CApplication::StartUninstallFat()
{
	CDosDriveList DosDriveList(PartList);
	CDosDriveList::CDosDrive DosDrive;
	int DosDriveIndex;
	int OriginalMbr;

	DosDriveIndex = TextUI.GetOptionIndex(0);
	OriginalMbr = !TextUI.GetOptionIndex(1);

	if (DosDriveList.LocateDrive(DosDriveIndex,DosDrive) == -1) {
		TextUI.OutputStr("Unable to locate drive %c:\n",'C' + DosDriveIndex);
		return -1;
	}
	return Installer.Uninstall(DosDrive,OriginalMbr);
}

int CApplication::StartUninstallSep()
{
	int PartIndex;
	int OriginalMbr;
	
	OriginalMbr = !TextUI.GetOptionIndex(0);
	PartIndex = InstallMenus.ResolvePartIndex(TextUI.GetOptionIndex(2));
	return Installer.Uninstall(PartIndex,OriginalMbr);
}

void CApplication::MainMenuExecute(CApplication *Application, TEnumMainMenu Item)
{
	switch (Item) {
		case enumInstall:
			if (Application->Data.CollectData(1,1,1) == -1)
				Application->InstallMenus.InitErrorMenu((CTextList::TListItemExecute)Application->ErrorMenuExecute,Application);
			else
				Application->InstallMenus.InitInstallMenu((CTextList::TListItemExecute)Application->InstallMenuExecute,Application);
			break;
		case enumRestore:
			Application->Data.CollectData(0,0,1);
			Application->InstallMenus.InitFixMbrMenu((CTextList::TListItemExecute)Application->RestoreMenuExecute,Application);
			break;
		case enumUninstall:
			Application->Data.CollectData(0,0,1);
			Application->InstallMenus.InitUninstallMenu((CTextList::TListItemExecute)Application->UninstallMenuExecute,Application);
			break;
		case enumExit:
			Application->DoExit = 1;
			break;
		default:
			break;
	}
}


void CApplication::InstallMenuExecute(CApplication *Application, TEnumInstallMenu Item)
{
	switch (Item) {
		case enumInstFat:
			Application->InstallMenus.InitInstFatMenu((CTextList::TListItemExecute)Application->InstFatMenuExecute,Application);
			break;
		case enumInstSep:
			Application->InstallMenus.InitInstSepMenu((CTextList::TListItemExecute)Application->InstSepMenuExecute,Application);
			Application->TextUI.OutputStr("WARNING:\nAll data on the partition you installXOSL on will be destroyed!\n\n");
			Application->TextUI.ShowPopup(10,4,60,17,InstallWarning);
			break;
		case enumInstAbort:
			Application->InstallMenus.InitMainMenu((CTextList::TListItemExecute)Application->MainMenuExecute,Application);
			break;
		default:
			break;
	}
}



void CApplication::RestoreMenuExecute(CApplication *Application, TEnumInstallMenu Item)
{
	switch (Item) {
		case enumInstFat:
			Application->InstallMenus.InitFixMbrFat((CTextList::TListItemExecute)Application->RestoreFatExecute,Application);
			break;
		case enumInstSep:
			Application->InstallMenus.InitFixMbrSep((CTextList::TListItemExecute)Application->RestoreSepExecute,Application);
			break;
		case enumInstAbort:
			Application->InstallMenus.InitMainMenu((CTextList::TListItemExecute)Application->MainMenuExecute,Application);
			break;
		default:
			break;
	}
}


void CApplication::UninstallMenuExecute(CApplication *Application, TEnumInstallMenu Item)
{
	switch (Item) {
		case enumInstFat:
			Application->InstallMenus.InitUninstallFat((CTextList::TListItemExecute)Application->UninstallFatExecute,Application);
			break;
		case enumInstSep:
			Application->InstallMenus.InitUninstallSep((CTextList::TListItemExecute)Application->UninstallSepExecute,Application);
			break;
		case enumInstAbort:
			Application->InstallMenus.InitMainMenu((CTextList::TListItemExecute)Application->MainMenuExecute,Application);
			break;
		default:
			break;
	}
}




void CApplication::InstFatMenuExecute(CApplication *Application, TEnumInstFatMenu Item)
{
	switch (Item) {
		case enumInstFatStart:
			if (Application->StartInstallFat() == -1)
				Application->InstallMenus.InitErrorMenu((CTextList::TListItemExecute)Application->ErrorMenuExecute,Application);
			else
				Application->InstallMenus.InitDoneMenu((CTextList::TListItemExecute)Application->DoneMenuExecute,Application);
			break;
		case enumInstFatAbort:
			Application->InstallMenus.InitInstallMenu((CTextList::TListItemExecute)Application->InstallMenuExecute,Application);
			break;
		default:
			break;
	}
}


void CApplication::InstSepMenuExecute(CApplication *Application, TEnumInstSepMenu Item)
{
	switch (Item) {
		case enumInstSepStart:
			if (Application->StartInstallSep() == -1)
				Application->InstallMenus.InitErrorMenu((CTextList::TListItemExecute)Application->ErrorMenuExecute,Application);
			else
				Application->InstallMenus.InitDoneMenu((CTextList::TListItemExecute)Application->DoneMenuExecute,Application);
			break;
		case enumInstSepAbort:
			Application->InstallMenus.InitInstallMenu((CTextList::TListItemExecute)Application->InstallMenuExecute,Application);
			break;
		default:
			break;
	}
}


void CApplication::ErrorMenuExecute(CApplication *Application, TEnumErrorMenu Item)
{
	switch (Item) {
		case enumErrorMain:
			Application->InstallMenus.InitMainMenu((CTextList::TListItemExecute)Application->MainMenuExecute,Application);
			break;
		case enumErrorQuit:
			Application->DoExit = 1;
			break;
		default:
			break;
	}
}

void CApplication::DoneMenuExecute(CApplication *Application, TEnumDoneMenu Item)
{
	switch (Item) {
		case enumDoneReboot:
			Application->DoReboot = 1;
		case enumDoneExit:
			Application->DoExit = 1;
			break;
		case enumDoneMain:
			Application->InstallMenus.InitMainMenu((CTextList::TListItemExecute)Application->MainMenuExecute,Application);
			break;
		default:
			break;
	}
}

void CApplication::UninstallFatExecute(CApplication *Application, TEnumFatUninstallMenu Item)
{
	switch (Item) {
		case enumFatUnStart:
			if (Application->StartUninstallFat() == -1)
				Application->InstallMenus.InitErrorMenu((CTextList::TListItemExecute)Application->ErrorMenuExecute,Application);
			else
				Application->InstallMenus.InitUnDoneMenu((CTextList::TListItemExecute)Application->DoneMenuExecute,Application);			
			break;
		case enumFatUnAbort:
			Application->InstallMenus.InitUninstallMenu((CTextList::TListItemExecute)Application->UninstallMenuExecute,Application);
			break;
		default:
			break;
	}
}

void CApplication::UninstallSepExecute(CApplication *Application, TEnumSepUninstallMenu Item)
{
	switch (Item) {
		case enumSepUnStart:
			if (Application->StartUninstallSep() == -1)
				Application->InstallMenus.InitErrorMenu((CTextList::TListItemExecute)Application->ErrorMenuExecute,Application);
			else
				Application->InstallMenus.InitUnDoneMenu((CTextList::TListItemExecute)Application->DoneMenuExecute,Application);			
			break;
		case enumSepUnAbort:
			Application->InstallMenus.InitUninstallMenu((CTextList::TListItemExecute)Application->UninstallMenuExecute,Application);
			break;
		default:
			break;
	}
}


void CApplication::RestoreFatExecute(CApplication *Application, TEnumFatUninstallMenu Item)
{
	switch (Item) {
		case enumFatFixStart:
			if (Application->StartRestoreFat() == -1)
				Application->InstallMenus.InitErrorMenu((CTextList::TListItemExecute)Application->ErrorMenuExecute,Application);
			else
				Application->InstallMenus.InitDoneMenu((CTextList::TListItemExecute)Application->DoneMenuExecute,Application);
			break;
		case enumFatFixAbort:
			Application->InstallMenus.InitFixMbrMenu((CTextList::TListItemExecute)Application->RestoreMenuExecute,Application);
			break;
		default:
			break;
	}
}

void CApplication::RestoreSepExecute(CApplication *Application, TEnumSepUninstallMenu Item)
{
	switch (Item) {
		case enumSepFixStart:
			if (Application->StartRestoreSep() == -1)
				Application->InstallMenus.InitErrorMenu((CTextList::TListItemExecute)Application->ErrorMenuExecute,Application);
			else
				Application->InstallMenus.InitDoneMenu((CTextList::TListItemExecute)Application->DoneMenuExecute,Application);
			break;
		case enumSepFixAbort:
			Application->InstallMenus.InitFixMbrMenu((CTextList::TListItemExecute)Application->RestoreMenuExecute,Application);
			break;
		default:
			break;
	}
}
