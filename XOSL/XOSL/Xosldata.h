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

#include <fs.h>
#include <graph.h>

typedef struct {
	TGraphMode VideoMode;
	int FrameMove;
	int LinearFB;
	int Font9;
	int CursorInverted;
	int UseWallpaper;
	int DisableFadeOut;
	int DisableFadeIn;
	int DisplayItemIndex;
	int ClearScreen;
	int NoAnimation;
	int Reserved[5];
} TGraphData;

typedef struct {
	int Reserved1; // can be used again
	int ColorScheme;
	int Hue;
	int Saturation;
	int Luminance;
	int FadeColorInt;
	int Reserved2[10];
} TColorData;

typedef struct {
	int MouseType;
	int MouseSpeed;
	int PS2Sampling;
	int KbdLayout;
	int EnhancedSupport;
	int Reserved[11];
} TMouseData;

typedef struct {
	unsigned long Password;
	int Reserved[14];
} TPassword;

typedef struct {
	int AutoSave;
	int Reserved1;	// was 'BootOnSelect'. Not used anymore
	int KeyColdReboot;
	int KeyShutdown;
	int KeyRestart;
	int KeyPart;
	int SetAutoBoot;
	int SoftPowerOff;
	int KeyCycle;
	int ActiveAllow;
	int AntiVirus;
	int NoAutoBootDialog;
	int Reserved[4];
} TMiscPref;

typedef struct {
	int XOSLVersion;
	TGraphData GraphData;
	TColorData ColorData;
	TMouseData MouseData;
	TPassword PasswordData;
	TMiscPref MiscPref;
	int Reserved[175];
} TXOSLData;

class CXOSLData {
	public:
		CXOSLData(CFileSystem *FileSystemToUse);
		~CXOSLData();

		void SaveData();

		void ResetGraphics();
		TGraphData *GetGraphics();

		void ResetColor();
		TColorData *GetColor();

		void ResetMouse();
		TMouseData *GetMouse();

		unsigned long GetPassword();
		void SetPassword(unsigned long Password);

		void ResetMiscPref();
		TMiscPref *GetMiscPref();

	private:
		TXOSLData *Data;
		TXOSLData *Backup;
		CFileSystem *FileSystem;
};



#endif