/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __vesa__
#define __vesa__

#include <defs.h>


//_extern int GetSVGAInfo(TSVGAInfo &SVGAInfo);
//_extern int ModeSupported(unsigned short Mode);


class CVesa {
public:
	enum TGraphicsMode {
		modeText = 0, mode640x480 = 1, mode800x600 = 2, 
		mode1024x768 = 3, mode1280x1024 = 4, mode1600x1200 = 5
	};


	CVesa();
	~CVesa();

	int Initialize();

	void GetVbeVersion(int &Major, int &Minor);
	const char *GetOemName();
	long GetVideoMemorySize();

	int GetGraphicsModeCount();
	int GetGraphicsModeSupported(TGraphicsMode GraphicsMode);

	const char *GetGraphicsModeName(TGraphicsMode GraphicsMode);

	
private:

	class CSvgaInfo {
	public:
		char VESA[4];
		unsigned char VbeMinor;
		unsigned char VbeMajor;
		char *OemName;
		unsigned long Capabilities;
		unsigned short *GraphicsModes;
		unsigned short VideoMemory;
		char Reserved[236];
		char OemScratchpad[256];
	};

	class CModeInfo {
	public:

		int ModeAttr;				// mode attribute (bit 0: mode supported)
		char AttrWinA;				// attribute window A
		char AttrWinB;				// attribute window B
		int Granularity;
		int WinSize;				// window size
		unsigned short SegWinA;     // segment window A
		unsigned short SegWinB;     // segment window B
		unsigned long SwitchAddr;		// -> FAR call bankswitch function
		int BytesScanline;			// bytes per scanline
		int Width;					// screen width
		int Height;					// screen height
		char CharCellWidth;			// charcell with in pixels
		char CharCellHeight;		// charcell height in pixels
		char MemPlanes;				// number of memory planes
		char BitsPixel;				// number of bits per pixel
		char Banks;					// number of memory banks
		char MemoryModel;
		char BankSize;
		char ImagePages;
		char Reserver0;
		char RedSize;				// red mask size
		char RedPos;				// red field position
		char GreenSize;				// green mask size
		char GreenPos;				// green field position
		char BlueSize;				// blue mask size
		char BluePos;				// blue field position
		char ResSize;				// reserved mask size
		char ResPos;				// reserved field position
		char DirectModeInfo;
		unsigned long LinearBuffer;	// physical address of linear frame buffer
		unsigned long OffScrnMem;	// address of offscreen memory
		int OffScrnSize;			// offscreen memory size (kb)
		char Reserved1[206];
	};


	CSvgaInfo SvgaInfo;
	int SupportedModes[6];
	int ModeCount;

	void DetectGraphicsModes();
	static int GetSvgaInfo(CSvgaInfo &SvgaInfo);
	static int GetModeInfo(int VesaMode, CModeInfo &ModeInfo);
	static const char *GraphicsModeNames[6];


};

#endif