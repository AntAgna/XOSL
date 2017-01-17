/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __graph__
#define __graph__

#include <newdefs.h>
#include <palette.h>
#include <mouse.h>
#include <..\graph\int10.h>

#define STYLE_REGULAR 0
#define STYLE_BOLD    1

typedef struct {
	int Index;
	unsigned char Items;
	unsigned char Width;
} TFontEntry;

typedef enum { 
	modeText = 0, mode640x480, mode800x600, 
	mode1024x768, mode1280x1024, mode1600x1200
} TGraphMode;


typedef struct {
	const char far *Name; // name of videocard
	int VersionMaj;       // major VBE version number
	int VersionMin;       // minor VBE version number
	int Memory;           // amount of installed videomemory (kb)
	int Width;		       // screen width
	int Height;           // screen height
} TGraphInfo;

class CGraph {
	public:
		CGraph(CMouse *MouseToUse);
		~CGraph();
		void DetectVideoModes();
	private:
		void AddVideoMode(TGraphMode GraphMode, int VesaMode, TModeInfo &ModeInfo);
	public:
		int SetMode(TGraphMode GraphMode, bool UseLFB);
		void GetMode(TGraphMode &GraphMode, bool &UseLFB);

		TGraphMode GetModeFromIndex(int ModeIndex);
		TGraphMode GetModeFromMetrics(int Width, int Height);
		int GetModeIndex(TGraphMode GraphMode);
		int GetModeCount();

		bool LFBSupported(TGraphMode GraphMode);


		const char *GetModeName();
		const char *GetModeName(int Mode/*TGraphMode Mode*/);
		void GetModeMetrics(int &Width, int &Height);
		void GetGraphInfo(TGraphInfo &GraphInfo);
		int ModeSupported(TGraphMode Mode);

		void WaitRetrace();

		void FlushArea(long Left, long Top, long Width, long Height);
		void UltraFlush();
		void StoreBuffer(long Left, long Top, long Width, long Height);
		void RestoreBuffer(long Left, long Top, long Width, long Height);

		void SetViewportOrigin(long Left, long Top);
		void GetViewportOrigin(long &Left, long &Top);
		void SetClippingRegion(long Left, long Top, long Width, long Height);
		void GetClippingRegion(long &Left, long &Top, long &Width, long &Height);

		void PutPixel(long Left, long Top, long Color);
		void HLine(long Left, long Top, long Width, long Color);
		void VLine(long Left, long Top, long Height, long Color);
		void Rectangle(long Left, long Top, long Width, long Height, long Color);
		void Bar(long Left, long Top, long Width, long Height, long Color);
		void PutImage(long Left, long Top, long Width, long Height, const void far *Image);
		void GetImage(long Left, long Top, long Width, long Height, const void far *Image);
		void Line(long X1, long Y1, long X2, long Y2, long Color);



		// Cursor
		void SetCursorXY(int X, int Y);
		void GetCursorXY(int &X, int &Y);
		void ShowCursor();
		void HideCursor();

		// Text
		void SetFont(void *FontFile);
		int GetTextWidth(const char *Str, int Style);
		int GetTextHeight();
		void TextOut(int Left, int Top, const char *Str, int Style, int Color);


		CPalette *Palette;
	private:
		// Text
		TFontEntry *FontHeader;
		//
		void CreateBankswitch(int Granularity, unsigned long SwitchAddr);

		void PerformPMAction(void *Func,long P5, long P4, long P3, long P2, long P1);

		TGraphInfo GraphInfo;
		TGraphMode Mode;
		bool UseLFB;
		const char *ModeName;

		CMouse *Mouse;
};

extern CGraph *Graph;

#endif