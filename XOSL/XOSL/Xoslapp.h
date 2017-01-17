/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __XOSL_Application__
#define __XOSL_Application__

#include <newdefs.h>
#include <capture.h>

#include <mouse.h>
#include <graph.h>
#include <screen.h>
#include <fs.h>

#include <loader.h>
#include <dialogs.h>
#include <default.h>

class CApplication {
	public:
		CApplication();
		~CApplication();
		int Execute();
	private:
		CMouse *Mouse;
		CFileSystem *FileSystem;
		CPartList *PartList;

		CScreen *Screen;

		CXOSLData *XOSLData;
		CBootItems *BootItems;

		CLoader * Loader;
		CDefaultBoot *DefaultBoot;
//		CAbout *About;
//		CSetup *Setup;
//		CPreference *Preference;

		CDialogs *Dialogs;

		int ClearScreen;

		void CriticalError(const char *ErrorMsg);
		void ExecuteBypass();
		//
		void CreateCoreClasses();
		void DestroyCoreClasses();
		void InitializeCore();
		void FinalizeCore();
		//
		void CreateApplication();
		void DestroyApplication();
		//
		void LoadDefaultFont();
		void LoadXOSLSplashLogo();
		void LoadXOSLLogo();
		void LoadSplashLogo();
		void LoadAdditionalFont();
		void LoadWallpaper();

		void InitializeMouse();
		void InitAppGraphics();

		void DisplayBootError();

		void Shutdown();
		void Restart();
		void Reboot();

		static void ShutDownReboot(CApplication *Application);


		void DoSplashFadeOut();
		void DoAppFadeIn();
		bool AutoBootAvail();

		void StartPart();
		static void PartPwdProc(CApplication &Application, const char *Password);
		void PerformAntiVirus();

#ifdef DOS_DEBUG
		void ScanForDebugKeys(int Key);
#endif

		CTextCapture *TextCapture;

};

#endif