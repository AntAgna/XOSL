/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __screen__
#define __screen__

#include <cursor.h>
#include <wndlist.h>
#include <backgnd.h>
#include <graph.h>

class CControl;

class CScreen {
	public:
		CScreen();
		~CScreen();
		int ShowSplashScreen(TGraphMode &Mode, bool &LinearFB);
		void ShowSubTitle();

		void AddWindow(CControl *Wnd);
		void RemoveWindow(CControl *Wnd);
		int SetGraphicsMode(TGraphMode Mode, bool LinearFB);
		int TestGraphicsMode(TGraphMode Mode, bool LinearFB);
		void GetScreenSize(int &Width, int &Height);
		void SetFocusWnd(CControl *Wnd);
		CControl *GetFocusWnd();
		void FocusNext();
		void FocusPrev();

		void BeforeFix(int Left, int Top, int Width, int Height);
		void AfterFix(int Left, int Top, int Width, int Height);
		void Refresh();

		void MouseStatus(int Left, int Top, int ButtonDown);
		void KeyPress(int Key);

		void SetUseWallpaper(int UseWallpaper, int ScrnRefresh = 1);
		void SetWallpaper(int Width, int Height, const unsigned char *Wallpaper);
		void SetSplashLogo(int Width, int Height, const unsigned char *LogoBitmap);
		void SetIgnoreSU(int IgnoreSU);
		void InitScreen();

		void CenterWindow(CControl *Wnd, int dX = 0, int dY = 0);
	private:
		int AdjustArea(int &Left, int &Top, int &Width, int &Height);
		CWindowList *WindowList;
		int ScrnWidth, ScrnHeight;
		int ScrnBottom, ScrnRight;

		int MouseX, MouseY, MouseDown;

		int UseWallpaper;
		CBackground *Background;

		// Ignore Screen Update
		int IgnoreSU;
};


#endif