/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

/*
 * Changes: Linear frame buffer isn't 'detected' anymore when the
 * the supported display modes are detected.
 */


#include <graph.h>
#include <mem.h>
#include <int10.h>
#include <vpgraph.h>
#include <mouse.h>
#include <retrace.h>
#include <cursor.h>
#include <pmgraph.h>

#define CURSOR_WIDTH 11
#define CURSOR_HEIGHT 19


typedef struct SGraphQueue{
	long PhysParams;
	long Func;
} TGraphQueue, far *PGraphQueue;

extern "C" {
	void InitViewport(long Width, long Height);
	void InitVBE2(unsigned long Addr);
};

static struct {
	int VesaMode;
	int Width, Height;
	bool LFBSupport;
	unsigned long LinearBuffer;
	int Granularity;
	unsigned long SwitchAddr;
	const char *ModeName;
} VesaModeList[] = {
	{0x0003,0,0,false,0,0,0,"Text 80x25"},
	{-1,640,480,false,0,0,0,"640x480"},
	{-1,800,600,false,0,0,0,"800x600"},
	{-1,1024,768,false,0,0,0,"1024x768"},
	{-1,1280,1024,false,0,0,0,"1280x1024"},
	{-1,1600,1200,false,0,0,0,"1600x1200"}
};


long GRWidth, GRHeight;
int GRThisBank;
long TextHeight;

static long TempVPLeft, TempVPTop;

TGraphQueue GraphAction;


CGraph *Graph;

CGraph::CGraph(CMouse *MouseToUse)
{
	Mouse = MouseToUse;
	InitDesc();
	Palette = new CPalette;
	DetectVideoModes();
	Mode = modeText;
	UseLFB = false;
}

CGraph::~CGraph()
{
	delete Palette;
}

void CGraph::DetectVideoModes()
{
	TModeInfo ModeInfo;
	TSVGAInfo SVGAInfo;
	unsigned short *ModeList;
	int Index, Count;

	if (GetSVGAInfo(&SVGAInfo))
		return;
	GraphInfo.Name = SVGAInfo.name;
	GraphInfo.VersionMaj = SVGAInfo.vermaj;
	GraphInfo.VersionMin = SVGAInfo.vermin;
	GraphInfo.Memory = SVGAInfo.memory;

	Count = 0;
	for (ModeList = SVGAInfo.modes; *ModeList != 0xffff; ++ModeList) {
		if (!GetModeInfo(*ModeList,&ModeInfo) &&
			 (ModeInfo.mode_attr & 1) != 0 && ModeInfo.bits_pixel == 8)
			if (ModeInfo.width == 640 && ModeInfo.height == 480) {
				AddVideoMode(mode640x480,*ModeList,ModeInfo);
				++Count;
			}
			else if (ModeInfo.width == 800 && ModeInfo.height == 600) {
				AddVideoMode(mode800x600,*ModeList,ModeInfo);
				++Count;
			}
			else if (ModeInfo.width == 1024 && ModeInfo.height == 768) {
				AddVideoMode(mode1024x768,*ModeList,ModeInfo);
				++Count;
			}
			else if (ModeInfo.width == 1280 && ModeInfo.height == 1024) {
				AddVideoMode(mode1280x1024,*ModeList,ModeInfo);
				++Count;
			}
			else if (ModeInfo.width == 1600 && ModeInfo.height == 1200) {
				AddVideoMode(mode1600x1200,*ModeList,ModeInfo);
				++Count;
			}
	}


	if (!Count) {
		// either VESA modes our not supported, or ModeList is empty
		// try the default VesaModes
		if (!GetModeInfo(0x0101,&ModeInfo) && (ModeInfo.mode_attr & 1) != 0)
			AddVideoMode(mode640x480,0x0101,ModeInfo);
		if (!GetModeInfo(0x0103,&ModeInfo) && (ModeInfo.mode_attr & 1) != 0)
			AddVideoMode(mode800x600,0x0103,ModeInfo);
		if (!GetModeInfo(0x0105,&ModeInfo) && (ModeInfo.mode_attr & 1) != 0)
			AddVideoMode(mode1024x768,0x0105,ModeInfo);
		if (!GetModeInfo(0x0107,&ModeInfo) && (ModeInfo.mode_attr & 1) != 0)
			AddVideoMode(mode1280x1024,0x0107,ModeInfo);
	}
}

void CGraph::AddVideoMode(TGraphMode GraphMode, int VesaMode, TModeInfo &ModeInfo)
{
	VesaModeList[GraphMode].VesaMode = VesaMode;
	if ((ModeInfo.mode_attr & 0x80) != 0) {
		VesaModeList[GraphMode].LFBSupport = true;
	}
	VesaModeList[GraphMode].LinearBuffer = ModeInfo.linear_buffer;
	VesaModeList[GraphMode].Granularity = ModeInfo.granularity;
	VesaModeList[GraphMode].SwitchAddr = ModeInfo.switch_addr;
}

int CGraph::SetMode(TGraphMode GraphMode, bool UseLFB)
{
	unsigned short VesaMode;

	if (VesaModeList[GraphMode].VesaMode == -1)
		return -1;
	if (UseLFB && !VesaModeList[GraphMode].LFBSupport) {
		return -1;
	}

	VesaMode = VesaModeList[GraphMode].VesaMode;
	if (UseLFB) {
		// Set bit 14 to indicate linear/flat frame buffer model
		VesaMode |= 0x4000;
	}

	if (SwitchTo(VesaMode))
		return -1;
	Mode = GraphMode;
	this->UseLFB = UseLFB;

	if (VesaMode != 3) {
		GRWidth = VesaModeList[GraphMode].Width;
		GRHeight = VesaModeList[GraphMode].Height;
		if (UseLFB) {
			InitVBE2(VesaModeList[GraphMode].LinearBuffer);
		}
		else {
			InitVBE2(0);
			GRThisBank = 0;
			CreateBankswitch(VesaModeList[GraphMode].Granularity,VesaModeList[GraphMode].SwitchAddr);
		}

		GraphInfo.Width = GRWidth;
		GraphInfo.Height = GRHeight;

		ModeName = VesaModeList[GraphMode].ModeName;
		InitViewport(GRWidth,GRHeight);
		Palette->Install();
	}
	return 0;
}

void CGraph::GetMode(TGraphMode &GraphMode, bool &UseLFB)
{
	GraphMode = Mode;
	UseLFB = this->UseLFB;
}

TGraphMode CGraph::GetModeFromIndex(int ModeIndex)
{
	int Index;

	if (ModeIndex < GetModeCount()) {
		for (Index = 0; Index != ModeIndex; )
			if (VesaModeList[Index + 1].VesaMode != -1)
				++Index;
		return (TGraphMode)(Index + 1);
	}
	return mode640x480;
}

TGraphMode CGraph::GetModeFromMetrics(int Width, int Height)
{
	if (Width == 640 && Height == 480)
		return mode640x480;
	if (Width == 800 && Height == 600)
		return mode800x600;
	if (Width == 1024 && Height == 768)
		return mode1024x768;
	if (Width == 1280 && Height == 1024)
		return mode1280x1024;
	if (Width == 1600 && Height == 1200)
		return mode1600x1200;
	return mode640x480;
}

int CGraph::GetModeIndex(TGraphMode GraphMode)
{
	int Index, ModeIndex;

	for (Index = 1, ModeIndex = 0; Index < GraphMode; ++Index)
		if (VesaModeList[Index].VesaMode != -1)
			++ModeIndex;
	return ModeIndex;
}

int CGraph::GetModeCount()
{
	int Index, Count = 0;

	for (Index = mode640x480; Index <= mode1600x1200; ++Index)
		if (VesaModeList[Index].VesaMode != -1)
			 ++Count;
	return Count;
}

bool CGraph::LFBSupported(TGraphMode GraphMode)
{
	return VesaModeList[GraphMode].LFBSupport;
}

const char *CGraph::GetModeName()
{
	return ModeName;
}

const char *CGraph::GetModeName(int Mode)
{
	return Mode >= 0 && Mode <= mode1600x1200 ? VesaModeList[Mode].ModeName : "";
}


void CGraph::GetModeMetrics(int &Width, int &Height)
{
	Width = GraphInfo.Width;
	Height = GraphInfo.Height;
}

void CGraph::GetGraphInfo(TGraphInfo &GraphInfo)
{
	memcpy(&GraphInfo,&this->GraphInfo,sizeof (TGraphInfo));
}

int CGraph::ModeSupported(TGraphMode Mode)
{
	return VesaModeList[Mode].VesaMode != -1;
}

void CGraph::CreateBankswitch(int Granularity, unsigned long SwitchAddr)
// pure memory manipulation...
{
	int shl = 0;
	int bmul = 64 / Granularity;
	char far *switchproc = (char far *)BankSwitch;

	for (; bmul >>= 1; ++shl);
	if (shl) {
		*switchproc++ = 0x52; // push dx
		*((unsigned short far *)switchproc)++ = 0xe2c1; // shl dx,...
		*switchproc++ = shl; // shl dx,_shl
	}
	*switchproc++ = 0x9a; // call ....:....
	*((unsigned long far *)switchproc)++ = SwitchAddr;
	if (shl)
		*switchproc++ = 0x5a; // pop dx
	*switchproc = 0xc3; // ret
}

void CGraph::FlushArea(long Left, long Top, long Width, long Height)
{
	PerformPMAction(VPFlushArea,0,Height,Width,Top,Left);
}

void CGraph::UltraFlush()
{
	PerformPMAction(VPUltraFlush,0,0,0,0,0);
}

void CGraph::StoreBuffer(long Left, long Top, long Width, long Height)
{
	PerformPMAction(VPStoreBuffer,0,Height,Width,Top,Left);
}

void CGraph::RestoreBuffer(long Left, long Top, long Width, long Height)
{
	PerformPMAction(VPRestoreBuffer,0,Height,Width,Top,Left);
}

void CGraph::PerformPMAction(void *Func, long P5, long, long, long, long)
{
	GraphAction.PhysParams = PhysAddr(&P5);
	GraphAction.Func = FP_OFS(Func);
	DoPMGraphics();
}


void CGraph::SetViewportOrigin(long Left, long Top)
{
	PerformPMAction(::SetViewportOrigin,0,0,0,Top,Left);
}

void CGraph::GetViewportOrigin(long &Left, long &Top)
{
	PerformPMAction(::GetViewportOrigin,0,0,0,PhysAddr(&Top),PhysAddr(&Left));
}

void CGraph::SetClippingRegion(long Left, long Top, long Width, long Height)
{
	PerformPMAction(::SetClippingRegion,0,Height,Width,Top,Left);
}

void CGraph::GetClippingRegion(long &Left, long &Top, long &Width, long &Height)
{
	PerformPMAction(::GetClippingRegion,0,PhysAddr(&Height),PhysAddr(&Width),PhysAddr(&Top),PhysAddr(&Left));
}

void CGraph::PutPixel(long Left, long Top, long Color)
{
	PerformPMAction(VPPutPixel,0,0,Color,Top,Left);
}

void CGraph::HLine(long Left, long Top, long Width, long Color)
{
	PerformPMAction(VPHLine,0,Color,Width,Top,Left);
}

void CGraph::VLine(long Left, long Top, long Height, long Color)
{
	PerformPMAction(VPVLine,0,Color,Height,Top,Left);
}

void CGraph::Rectangle(long Left, long Top, long Width, long Height, long Color)
{
	PerformPMAction(VPRectangle,Color,Height,Width,Top,Left);
}

void CGraph::Bar(long Left, long Top, long Width, long Height, long Color)
{
	PerformPMAction(VPBar,Color,Height,Width,Top,Left);
}

void CGraph::PutImage(long Left, long Top, long Width, long Height, const void far *Image)
{
	PerformPMAction(VPPutImage,PhysAddr(Image),Height,Width,Top,Left);
}

void CGraph::GetImage(long Left, long Top, long Width, long Height, const void far *Image)
{
	PerformPMAction(VPGetImage,PhysAddr(Image),Height,Width,Top,Left);
}

void CGraph::Line(long X1, long Y1, long X2, long Y2, long Color)
{
	PerformPMAction(VPLine,Color,Y2,X2,Y1,X1);
}

void CGraph::WaitRetrace()
{
	::WaitRetrace();
}

void CGraph::SetCursorXY(int X, int Y)
{
	int OldX, OldY;

	OldX = CursorX;
	OldY = CursorY;
	CursorX = X;
	CursorY = Y;

	if (CursorVisible) {
		FlushArea(X,Y,CURSOR_WIDTH,CURSOR_HEIGHT);
		FlushArea(OldX,OldY,CURSOR_WIDTH,CURSOR_HEIGHT);
	}
}

void CGraph::GetCursorXY(int &X, int &Y)
{
	X = CursorX;
	Y = CursorY;
}

void CGraph::ShowCursor()
{
	if (Mouse->Available() != -1) {
		CursorVisible = true;
		FlushArea(CursorX,CursorY,CURSOR_WIDTH,CURSOR_HEIGHT);
	}
}

void CGraph::HideCursor()
{
	CursorVisible = false;
	FlushArea(CursorX,CursorY,CURSOR_WIDTH,CURSOR_HEIGHT);
}

void CGraph::SetFont(void *FontFile)
{
	int *pFontFile;

	pFontFile = (int *)FontFile;

	//FontHeader = new TFontEntry[256];

	DataSize = *pFontFile++;

	TextHeight = *pFontFile++;

//	memcpy(FontHeader,pFontFile,sizeof (TFontEntry) * 256);
	FontHeader = (TFontEntry *)pFontFile;
	pFontFile += sizeof (TFontEntry) * 128;
	PhysFontHeader = PhysAddr(FontHeader);
	PhysFontData = PhysAddr(pFontFile);
//	MapFontData();
}

int CGraph::GetTextWidth(const char *Str, int Style)
{
	int Length;
	const unsigned char *pStr;

	pStr = (const unsigned char *)Str;

	for (Length = 0; *pStr; Length += FontHeader[*pStr++].Width);
	if (Style == STYLE_BOLD)
		++Length;
	return Length;
}

int CGraph::GetTextHeight()
{
	return TextHeight;
}

void CGraph::TextOut(int Left, int Top, const char *Str, int Style, int Color)
{
	PerformPMAction(VPTextOut,0,Color,PhysAddr(Str),Top,Left);
	if (Style == STYLE_BOLD) {
		PerformPMAction(VPTextOut,0,Color,PhysAddr(Str),Top,Left + 1);
	}
}

