/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __main__
#define __main__

#include <defs.h>
#include <key.h>

#include <data.h>
#include <install.h>

#include <textui.h>
#include <textscr.h>

#include <menus.h>

class CApplication {
public:
	CApplication();
	~CApplication();

	void ApplicationLoop();

private:
	enum TEnumMainMenu { enumInstall = 0, enumRestore = 4, enumUninstall = 2, enumExit = 6 };
	enum TEnumInstallMenu { enumInstFat = 0, enumInstSep = 2, enumInstAbort = 6 };
	enum TEnumInstFatMenu { enumInstFatStart = 6, enumInstFatAbort = 8 };
	enum TEnumInstSepMenu{ enumInstSepStart = 10, enumInstSepAbort = 11 };
	enum TEnumErrorMenu { enumErrorQuit = 2, enumErrorMain = 4};
	enum TEnumDoneMenu { enumDoneReboot = 3, enumDoneExit = 5, enumDoneMain = 7};
	enum TEnumFatUninstallMenu { enumFatUnStart = 4, enumFatUnAbort = 6 };
	enum TEnumSepUninstallMenu { enumSepUnStart = 8, enumSepUnAbort = 10 };
	enum TEnumFatRestoreMenu { enumFatFixStart = 4, enumFatFixAbort = 6 };
	enum TEnumSepRestoreMenu { enumSepFixStart = 6, enumSepFixAbort = 8 };
	
	static void MainMenuExecute(CApplication *Application, TEnumMainMenu Item);
	static void InstallMenuExecute(CApplication *Application, TEnumInstallMenu Item);
	static void RestoreMenuExecute(CApplication *Application, TEnumInstallMenu Item);
	static void UninstallMenuExecute(CApplication *Application, TEnumInstallMenu Item);
	static void InstFatMenuExecute(CApplication *Application, TEnumInstFatMenu Item);
	static void InstSepMenuExecute(CApplication *Application, TEnumInstSepMenu Item);

	static void UninstallFatExecute(CApplication *Application, TEnumFatUninstallMenu Item);
	static void UninstallSepExecute(CApplication *Application, TEnumSepUninstallMenu Item);

	static void RestoreFatExecute(CApplication *Application, TEnumFatUninstallMenu Item);
	static void RestoreSepExecute(CApplication *Application, TEnumSepUninstallMenu Item);

	static void ErrorMenuExecute(CApplication *Application, TEnumErrorMenu Item);
	static void DoneMenuExecute(CApplication *Application, TEnumDoneMenu Item);

	int StartInstallFat();
	int StartInstallSep();

	int StartRestoreFat();
	int StartRestoreSep();

	int StartUninstallFat();
	int StartUninstallSep();

	static void Reboot();

	CKeyboard Keyboard;

	CPartList PartList;
	CTextScreen *TextScreen;
	CTextUI TextUI;

	CData Data;
	CInstaller Installer;

	CInstallMenus InstallMenus;
	
	int DoExit;
	int DoReboot;
	


};

#endif
