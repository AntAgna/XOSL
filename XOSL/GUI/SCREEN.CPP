/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <screen.h>
#include <graph.h>
#include <backgnd.h>
#include <Control.h>
#include <cursor.h>
#include <mouse.h>
#include <text.h>

/*
 * The screen consists of four layers:
 * layer 1: background; drawn by BeforeFix()
 * layer 2: all controls (forms)
 * layer 3: 'foreground'; drawn by AfterFix() (empty at the moment)
 * layer 4: mouse cursor
 */

CScreen::CScreen()
{
	IgnoreSU = false;
	Background = new CBackground;
	WindowList = new CWindowList();
	WindowList->SetPosition(0,0);
	WindowList->SetHandler(this);
	UseWallpaper = false;
	MouseX = -1;
	MouseY = -1;
	MouseDown = 0;
}

CScreen::~CScreen()
{
	delete Background;
	delete WindowList;
}

int CScreen::ShowSplashScreen(TGraphMode &Mode, bool &LinearFB)
{
	int Width, Height;
	int Result = -1;

	// LinearFB is set to 'false' either when SetMode() fails or when it's not supported for the first mode
	while (Mode != modeText && Result == -1) {
		if (LinearFB && Graph->LFBSupported(Mode)) {
			Result = Graph->SetMode(Mode,true);
			if (Result == -1) {
				LinearFB = false;
			}
		}
		else {
			LinearFB = false;
			Result = Graph->SetMode(Mode,false);
		}
		if (Result == -1) {
			--(int)Mode;
		}
	}
	if (Mode == modeText)
		return -1;

	Graph->GetModeMetrics(Width,Height);
	Background->SplashScreen(Width,Height);
	Graph->UltraFlush();
	return 0;
}

void CScreen::ShowSubTitle()
{
	int Width, Height;
    
	Graph->GetModeMetrics(Width,Height);
	Background->ShowSubTitle(Width,Height);
}

void CScreen::BeforeFix(int Left, int Top, int Width, int Height)
{
	if (!AdjustArea(Left,Top,Width,Height))
		return;
	Background->FixDamage(Left,Top,Width,Height);
}

void CScreen::AfterFix(int, int, int, int)
{
}

void CScreen::AddWindow(CControl *Wnd)
{
	WindowList->Add(Wnd);
}

void CScreen::RemoveWindow(CControl *Wnd)
{
	WindowList->Remove(Wnd);
}

int CScreen::SetGraphicsMode(TGraphMode Mode, bool LinearFB)
{
	if (Graph->SetMode(Mode,LinearFB) == -1 || Mode == modeText)
		return -1;
	InitScreen();
	return 0;
}

int CScreen::TestGraphicsMode(TGraphMode Mode, bool LinearFB)
{
	int Width, Height;

	if (Graph->SetMode(Mode,LinearFB) == -1)
		return -1;
	Graph->GetModeMetrics(Width,Height);
	Background->DrawTestScreen();
	Graph->UltraFlush();
	return 0;
}

void CScreen::GetScreenSize(int &Width, int &Height)
{
	Width = ScrnWidth;
	Height = ScrnHeight;
}

int CScreen::AdjustArea(int &Left, int &Top, int &Width, int &Height)
{
	long Bottom, Right;

	if (Top >= ScrnHeight || Left >= ScrnWidth)
		return false;
	if (Top < 0) {
		Height += Top;
		Top = 0;
	}
	Bottom = Top + Height - 1;
	if (Bottom >= ScrnBottom)
		Height -= Bottom - ScrnBottom;
	if (Left < 0) {
		Width += Left;
		Left = 0;
	}
	Right = Left + Width - 1;
	if (Right > ScrnRight)
		Width -= Right - ScrnRight;
	if (Width > 0 && Height > 0)
		return true;
	return false;
}

void CScreen::SetFocusWnd(CControl *Wnd)
{
	WindowList->SetFocusWnd(Wnd,true);
}

CControl *CScreen::GetFocusWnd()
{
	return WindowList->GetFocusWnd();
}

void CScreen::FocusNext()
{
	CControl *Current;
	CControl *New;

	Current = GetFocusWnd();
	New = WindowList->FocusNext();
	if (New && New != Current)
		SetFocusWnd(New);
}

void CScreen::FocusPrev()
{
	CControl *Current;
	CControl *New;

	Current = GetFocusWnd();
	New = WindowList->FocusPrev();
	if (New && New != Current)
		SetFocusWnd(New);
}


void CScreen::Refresh()
{
	if (!IgnoreSU) {
		WindowList->FixDamage(0,0,ScrnWidth,ScrnHeight);
		Graph->UltraFlush();
	}
}

void CScreen::MouseStatus(int Left, int Top, int ButtonDown)
{
	if (ButtonDown != MouseDown) {
		MouseDown = ButtonDown;
		if (MouseDown)
			WindowList->MouseDown(Left,Top);
		else
			WindowList->MouseUp();
	}
	if (Left != MouseX || Top != MouseY) {
		MouseX = Left;
		MouseY = Top;
		WindowList->MouseMove(Left,Top);
	}
}

void CScreen::KeyPress(int Key)
{
	CControl *Wnd;

	Wnd = WindowList->GetFocusWnd();
	if (Wnd && Wnd->Visible)
		Wnd->KeyPress(Key);
}

void CScreen::SetUseWallpaper(int UseWallpaper, int ScrnRefresh)
{
	if (UseWallpaper != this->UseWallpaper) {
		this->UseWallpaper = UseWallpaper;
		if (!IgnoreSU && ScrnRefresh) {
			Background->NewGraphicsMode(ScrnWidth,ScrnHeight,UseWallpaper);
			Refresh();
		}
	}
}

void CScreen::SetWallpaper(int Width, int Height, const unsigned char *Wallpaper)
{
	Background->SetWallpaper(Width,Height,Wallpaper);
}

void CScreen::SetSplashLogo(int Width, int Height, const unsigned char *LogoBitmap)
{
	Background->SetSplashLogo(Width,Height,LogoBitmap);
}

void CScreen::SetIgnoreSU(int IgnoreSU)
{
	this->IgnoreSU = IgnoreSU;
}

void CScreen::InitScreen()
{
	Graph->GetModeMetrics(ScrnWidth,ScrnHeight);
	ScrnRight = ScrnWidth - 1;
	ScrnBottom = ScrnHeight - 1;
	Background->NewGraphicsMode(ScrnWidth,ScrnHeight,UseWallpaper);

	WindowList->SetMetrics(ScrnWidth,ScrnHeight);
	WindowList->FixDamage(0,0,ScrnWidth,ScrnHeight);
	Graph->UltraFlush();
}

void CScreen::CenterWindow(CControl *Wnd, int dX, int dY)
{
	int WndWidth, WndHeight;
	int ScreenWidth, ScreenHeight;

	Wnd->GetMetrics(WndWidth,WndHeight);
	Graph->GetModeMetrics(ScreenWidth,ScreenHeight);
	Wnd->SetPosition(dX + (ScreenWidth - WndWidth) / 2,dY + (ScreenHeight - WndHeight) / 2);
}
