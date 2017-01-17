/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#include <vesa.h>
#include <string.h>
#include <memory.h>


#include <textui.h>
extern CTextUI *GlobalTextUI;


const char *CVesa::GraphicsModeNames[6] = {
	"Text","640x480","800x600","1024x768","1280x1024","1600x1200"
};


CVesa::CVesa()
{
}

CVesa::~CVesa()
{
}

int CVesa::Initialize()
{
	if (GetSvgaInfo(SvgaInfo) == -1 || MemCompare(SvgaInfo.VESA,"VESA",4))
		return -1;
	DetectGraphicsModes();
	return 0;
}

void CVesa::GetVbeVersion(int &Major, int &Minor)
{
	Major = SvgaInfo.VbeMajor;
	Minor = SvgaInfo.VbeMinor;
}

const char *CVesa::GetOemName()
{
	return SvgaInfo.OemName;
}

long CVesa::GetVideoMemorySize()
{
	return (long)SvgaInfo.VideoMemory << 6;
}

void CVesa::DetectGraphicsModes()
{
	unsigned short *GraphicsModes;
	CModeInfo ModeInfo;
	int Index;


	for (Index = 0; Index <= mode1600x1200; ++Index)
		SupportedModes[Index] = -1;

	// search the graphics mode list for suitable graphics modes
	for (GraphicsModes = SvgaInfo.GraphicsModes; *GraphicsModes != 0xffff; ++GraphicsModes)
		if (GetModeInfo(*GraphicsModes,ModeInfo) != -1 && (ModeInfo.ModeAttr & 1) != 0 && ModeInfo.BitsPixel == 8)
			if (ModeInfo.Width == 640 && ModeInfo.Height == 480)
				SupportedModes[mode640x480] = *GraphicsModes;
			else if (ModeInfo.Width == 800 && ModeInfo.Height == 600)
				SupportedModes[mode800x600] = *GraphicsModes;
			else if (ModeInfo.Width == 1024 && ModeInfo.Height == 768)
				SupportedModes[mode1024x768] = *GraphicsModes;
			else if (ModeInfo.Width == 1280 && ModeInfo.Height == 1024)
				SupportedModes[mode1280x1024] = *GraphicsModes;
			else if (ModeInfo.Width == 1600 && ModeInfo.Height == 1200)
				SupportedModes[mode1600x1200] = *GraphicsModes;




	for (Index = 0, ModeCount = 0; Index <= mode1600x1200; ++Index)
		if (SupportedModes[Index] != -1)
			++ModeCount;
	if (!ModeCount) {
		// try the default VesaMode values (the mode list used above might be empty!)
		if (GetModeInfo(0x0101,ModeInfo) != -1 && (ModeInfo.ModeAttr & 1) != 0)
			SupportedModes[mode640x480] = 0x0101;
		if (GetModeInfo(0x0103,ModeInfo) != -1 && (ModeInfo.ModeAttr & 1) != 0)
			SupportedModes[mode800x600] = 0x0103;
		if (GetModeInfo(0x0105,ModeInfo) != -1 && (ModeInfo.ModeAttr & 1) != 0)
			SupportedModes[mode1024x768] = 0x0105;
		if (GetModeInfo(0x0107,ModeInfo) != -1 && (ModeInfo.ModeAttr & 1) != 0)
			SupportedModes[mode1280x1024] = 0x0107;
		for (Index = 0, ModeCount = 0; Index <= mode1600x1200; ++Index)
			if (SupportedModes[Index] != -1)
				++ModeCount;
	}

}

int CVesa::GetGraphicsModeCount()
{
	return ModeCount;
}

int CVesa::GetGraphicsModeSupported(TGraphicsMode GraphicsMode)
{
	return SupportedModes[GraphicsMode];
}

const char *CVesa::GetGraphicsModeName(TGraphicsMode GraphicsMode)
{
	return GraphicsModeNames[GraphicsMode];
}
