/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __xosldata__
#define __xosldata__

#include <vesa.h>

class CXoslData {
public:
	class CGraphData {
	public:
		CVesa::TGraphicsMode VideoMode;
		int FrameMove;
		int LinearFB;
		int Font9;
		int CursorInverted;
		int UseWallpaper;
		int DisableFadeOut;
		int DisableFadeIn;
		int DisplayItemIndex;
		int ClearScreen;
		int Reserved[6];
	};
	
	class CColorData {
	public:
		int Reserved1; // can be used again
		int ColorScheme;
		int Hue;
		int Saturation;
		int Luminance;
		int FadeColorInt;
		int Reserved2[10];
	};

	class CMouseData {
	public:
		int MouseType;
		int MouseSpeed;
		int PS2Sampling;
		int Reserved[13];
	};

	class CPassword {
	public:
		unsigned long Password;
		int Reserved[14];
	};

	class CMiscPref {
	public:
		int AutoSave;
		int BootOnSelect;
		int KeyColdReboot;
		int KeyShutdown;
		int KeyRestart;
		int KeyPart;
		int SetAutoBoot;
		int SoftPowerOff;
		int KeyCycle;
		int ActiveAllow;
		int AntiVirus;
		int Reserved[5];
	};

public:
	unsigned short XoslVersion;
	CGraphData GraphData;
	CColorData ColorData;
	CMouseData MouseData;
	CPassword PasswordData;
	CMiscPref MiscPref;
	int Reserved[175];
};

#endif