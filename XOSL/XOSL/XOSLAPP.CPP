/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <xoslapp.h>
#include <cstring.h>
#include <text.h>
#include <timer.h>
#include <mem.h>

#include <key.h>
#include <bypass.h>

#include <mouse.h>
#include <graph.h>
#include <key.h>
#include <screen.h>
#include <fs.h>
#include <fat16.h>
#include <fat32.h>
#include <quit.h>

#include <ptab.h>
#include <xosldata.h>
#include <items.h>

#include <loader.h>
#include <setup.h>
#include <prefer.h>
#include <about.h>
#include <dialogs.h>
#include <dcx.h>
#include <antivir.h>
#include <disk.h>
#include <cdrom.h>

#include <dump.h>

#include <strtable.h>

void printf(const char *format, ...);
/*void getch()
{
	asm xor ah,ah
	asm int 0x16
}	*/

long CoreLeft();

/**/

typedef struct {
	int Drive;
	int FSType;
	long StartSector;
} TMountPart;

#ifndef DOS_DEBUG

#define MOUNT_PART  ( *(const TMountPart *)0x00007c00 )

#else

//const TMountPart MOUNT_PART = {0x80,0x06,0x0031b600};

const TMountPart MOUNT_PART = {0x80,0x06,0x00246c00};
//const TMountPart MOUNT_PART = {0x80,0x06,0x005ffa40};
static char *FileName = "dump_a.dmp";

#endif


/**/

#define LOGO_WIDTH 218
#define LOGO_HEIGHT 146

#define WALLPAPER_WIDTH 150
#define WALLPAPER_HEIGHT 150

#define Int19Reboot() (__emit__(0xcd,0x19))

#define BypassRequest() ( (peekb(0x0000,0x0417) & 0x02) == 0x02 )

/* Global vars. */
unsigned char *SplashLogo;
unsigned char *LogoBitmap;
unsigned char *WallpaperBitmap;
void *DefaultFont;
void *AdditionalFont;



/**/
const char *DefaultFontFileName = "DEFAULT XFF";
const char *AddFontFileName     = "EXTRA   XFF";
const char *LogoFileName        = "XOSLLOGOXBF";
const char *SplashFileName      = "SPLASHLGXBF";
const char *WallpaperFileName   = "XOSLWALLXBF";

CApplication::CApplication()
{
	TextCapture = new CTextCapture;
}

CApplication::~CApplication()
{
	if (!ClearScreen)
		delete TextCapture;
}

int CApplication::Execute()
/*
 * XOSL main application function
 */
{
	int Key;
	int X, Y;
	int Status;
	TMiscPref *MiscPref;
	CBootItem *BootItem;

	if (BypassRequest())
		ExecuteBypass();


	CreateCoreClasses();
	InitializeCore();

	// reveal all partitions if requested
	MiscPref = XOSLData->GetMiscPref();
	PartList->RevealAll();

	CreateApplication();

	DoSplashFadeOut();

	InitAppGraphics();

	/* Display main window */

	if (!AutoBootAvail()) {
		Graph->ShowCursor();
		Loader->Show();
		Screen->InitScreen();
		DoAppFadeIn();
		Key = 0;
	}
	else {
		if (!MiscPref->NoAutoBootDialog) {
			DefaultBoot->Show(BootItems->Get(BootItems->GetDefault())->ItemName,BootItems->GetTimeout());
		}
		else {
			Loader->Show(true);
			Loader->SelectDefault();
		}
		
		Screen->InitScreen();
		DoAppFadeIn();

		if (!MiscPref->NoAutoBootDialog) {
			Key = DefaultBoot->WaitTimeout();
		}
		else {
			Key = Loader->WaitTimeout();
		}
		if (Key != -1) {
			CKeyboard::Flush();
			CKeyboard::StoreKeyStroke(Key);
		}
	}
	/* Main loop */
	do {
		while (Key != -1) {
			while (!CKeyboard::KeyStrokeAvail() && !Loader->CanBoot()) {
				Mouse->GetXY(X,Y);
				Graph->SetCursorXY(X,Y);
				Screen->MouseStatus(X,Y,Mouse->MouseDown());
			}
			if (Loader->CanBoot())
				Key = -1;
			else {
				Key = CKeyboard::WaitKeyStroke();

				Screen->KeyPress(Key);
				if (!Loader->GetIgnoreNextKey()) {
					if (Key == MiscPref->KeyCycle)
						Screen->FocusNext();
						//Loader->CycleWindows();
					else
#ifndef DOS_DEBUG
						if (Key == MiscPref->KeyShutdown) {
							Loader->HandleAutoSave();
							Shutdown();
						}
						else if (Key == MiscPref->KeyRestart) {
							Loader->HandleAutoSave();
							Restart();
						}
						else if (Key == MiscPref->KeyColdReboot) { 
							Loader->HandleAutoSave();
							Reboot();
						}
						else if (Key == MiscPref->KeyPart) {
							Loader->HandleAutoSave();
							if (XOSLData->GetPassword()) {
								Dialogs->SetPasswordHandler(this,(TPwdProc)PartPwdProc);
								Dialogs->ShowPasswordDialog(NULL,"Ranish Partition Manager");
							}
							else {
								StartPart();
							}
						}

#else
						ScanForDebugKeys(Key);
//						unsigned short NewKey = CKeyboard::GetNewKeyCode(Key);
//						printf("\r[%x - %c] [%x - %c]      ",Key,Key & 0xff,NewKey,NewKey & 0xff);

#endif
				}
			}
		}

#ifdef DOS_DEBUG
		if (MiscPref->SetAutoBoot) {
			BootItems->SetDefault(Loader->GetBootItemIndex());
			BootItems->Save();
		}
		/* terminate program instead of booting */
		delete Mouse;
		Graph->SetMode(modeText,false);
		puts(BootItems->Get(Loader->GetBootItemIndex())->ItemName);
		asm mov ah,0x4c
		asm int 0x21
#endif

		// save settings _before_ drive swapping is done!
		if (MiscPref->SetAutoBoot) {
			BootItems->SetDefault(Loader->GetBootItemIndex());
			BootItems->Save();
		}


		PartList->SetAllowActiveHD(MiscPref->ActiveAllow);
		Status = BootItems->InitBoot(Loader->GetBootItemIndex());
		if (Status == -1) {
			DisplayBootError();
			Key = 0;
		}
	} while (Status == -1);

	ClearScreen = XOSLData->GetGraphics()->ClearScreen;

	if (!XOSLData->GetGraphics()->DisableFadeOut)
		Graph->Palette->FadeToBlack();
	Graph->SetMode(modeText,false);

	DestroyApplication();
	FinalizeCore();
	DestroyCoreClasses();

	return Status;
}

void CApplication::CriticalError(const char *ErrorMsg)
{
	int Key;
	CBypass XoslBypass;

	Graph->SetMode(modeText,false);
	delete Mouse;
	XoslBypass.Execute(ErrorMsg);
}

void CApplication::ExecuteBypass(/*int Password*/)
{
	CBypass XoslBypass;

	XoslBypass.Execute();
}

void CApplication::CreateCoreClasses()
{
	StringTable = new CXoslStringTable;
	Mouse = new CMouse;
	Graph = new CGraph(Mouse);
	Screen = new CScreen;
	switch (MOUNT_PART.FSType) {
		case 0x06: /* FAT16 */
			FileSystem = new CFAT16;
			break;
		case 0x0b: /* FAT32 */
			FileSystem = new CFAT32;
			break;
		default:
			CriticalError("Unknown filesystem.");
			break;
	}
}

void CApplication::DestroyCoreClasses()
{
	delete FileSystem;
	delete Screen;
	delete Graph;
	delete Mouse;
}

void CApplication::InitializeCore()
{
	TGraphData *GraphData;

	/* Initialize splash screen */
	FileSystem->Mount(MOUNT_PART.Drive,MOUNT_PART.StartSector);
	LoadXOSLSplashLogo();
	Screen->SetSplashLogo(LOGO_WIDTH,LOGO_HEIGHT,SplashLogo);
	
/*
	CCdRom *CdRom = new CCdRom(*FileSystem);
	if (CdRom->Install(0,0) == -1) {
		CriticalError("EDD30.BIN not found.");
	}
	for (int Drive = 0x80; Drive < 0x84; ++Drive) {
		printf("Drive %d: %d\n",Drive,CdRom->IsDriveCdRom(Drive));
	}

	for ( Drive = 0x80; Drive < 0x84; ++Drive) {
		printf("Drive %d: %d\n",Drive,CdRom->IsDriveCdRom(Drive));
	}

	CdRom->Uninstall();
	delete CdRom;
	getch();
*/	
	
	
	XOSLData = new CXOSLData(FileSystem);
	GraphData = XOSLData->GetGraphics();
	if (Screen->ShowSplashScreen(GraphData->VideoMode,GraphData->LinearFB) == -1)
		CriticalError("Graphics mode not supported.");
	/* Load additional data */
	LoadDefaultFont();
	Graph->SetFont(DefaultFont);

	// Credits for Ranish Partition Manager
	Screen->ShowSubTitle();

	if (XOSLData->GetMiscPref()->AntiVirus) {
		PerformAntiVirus();
	}

	LoadAdditionalFont();
	LoadXOSLLogo();
	LoadWallpaper();
	Screen->SetWallpaper(WALLPAPER_WIDTH,WALLPAPER_HEIGHT,WallpaperBitmap);
	PartList = new CPartList;
	BootItems = new CBootItems(*FileSystem,*PartList);
	/* Additional initialization */
	InitializeMouse();
}

void CApplication::FinalizeCore()
{
	delete BootItems;
	delete XOSLData;
	delete PartList;
}

void CApplication::CreateApplication()
{
	// ignore screen updates
	Screen->SetIgnoreSU(true);

	Loader = new CLoader(*Screen,*BootItems,*XOSLData,*PartList,*Mouse);
	DefaultBoot = new CDefaultBoot(*Screen,*Loader);
//	About = new CAbout(Loader,Screen);
//	Setup = new CSetup(*Loader,*Screen,*PartList,*BootItems,*XOSLData,*Keyboard);
//	Preference = new CPreference(*Loader,*Setup,*About,*Screen,*XOSLData,*Keyboard,*Mouse);


	Dialogs = new CDialogs(*Screen);

//	Loader->Initialize(Preference,Setup,About,Screen,BootItems,XOSLData);
//	Preference->InitializeData();
//	Setup->InitializeData();
	
	Screen->SetIgnoreSU(false);
}

void CApplication::DestroyApplication()
/*
 * delete takes time, so if not really necessary, then don't.
 */
{
/*	delete About;
	delete Preference;
	delete Setup;
	delete Loader;*/
}

void CApplication::LoadDefaultFont()
{
	DefaultFont = new char[8192];
	if (!FileSystem->ReadFile(DefaultFontFileName,DefaultFont))
		CriticalError("Font file not found.");
}

void CApplication::LoadXOSLSplashLogo()
{
	SplashLogo = new unsigned char [(unsigned int)(LOGO_WIDTH * LOGO_HEIGHT)];
	if (!FileSystem->ReadFile(SplashFileName,SplashLogo))
		CriticalError("Logo bitmap not found.");
}

void CApplication::LoadXOSLLogo()
{
	LogoBitmap = new unsigned char [(unsigned int)(LOGO_WIDTH * LOGO_HEIGHT)];
	if (!FileSystem->ReadFile(LogoFileName,LogoBitmap))
		CriticalError("Logo bitmap not found.");
}

void CApplication::LoadAdditionalFont()
{
	AdditionalFont = new char [8192];
	if (!FileSystem->ReadFile(AddFontFileName,AdditionalFont))
		CriticalError("Font file not found.");
}

void CApplication::LoadWallpaper()
{
	WallpaperBitmap = new unsigned char [(unsigned int)(WALLPAPER_WIDTH * WALLPAPER_HEIGHT)];
	if (!FileSystem->ReadFile(WallpaperFileName,WallpaperBitmap))
		CriticalError("Wallpaper bitmap not found.");
}

void CApplication::InitializeMouse()
{
	TMouseData *MouseData;
	int MousePort;

	/* Initialize mouse */
	MouseData = XOSLData->GetMouse();
	if (MouseData->MouseType != -1) {
		if (Mouse->SetMouse(MouseData->MouseType) == -1) {
			for (MousePort = 0; MousePort < 5 && !Mouse->Detect(MousePort); ++MousePort);
			if (MousePort == 5) {
				// Mouse not found, so set to 'no mouse'
				MouseData->MouseType = -1;
			}
			else {
				Mouse->SetMouse(MousePort);
				MouseData->MouseType = MousePort;
			}
		}
	}
}

void CApplication::InitAppGraphics()
{
	int Width, Height;
	TGraphData *GraphData;
	TMouseData *MouseData;
	TColorData *ColorData;

	Graph->GetModeMetrics(Width,Height);
	/* Initialize mouse window, etc.
		Doesn't matter if no mouse installed */
	Mouse->SetWindow(0,0,Width,Height);
	Width >>= 1;
	Height >>= 1;
	Mouse->SetXY(Width,Height);
	Graph->SetCursorXY(Width,Height);

	GraphData = XOSLData->GetGraphics();
	MouseData = XOSLData->GetMouse();
	ColorData = XOSLData->GetColor();

	// keyboard layout
	CKeyboard::SelectLayout((CKeyboard::TKeyLayout)MouseData->KbdLayout);
	CKeyboard::SetEnhancedSupport(MouseData->EnhancedSupport);

	// FrameMove
	CForm::FrameMove = GraphData->FrameMove;
	CAnimatedControl::Animate = !GraphData->NoAnimation;

	// Cursor
	InvertCursor(GraphData->CursorInverted);
	// Font
	if (GraphData->Font9) {
		Graph->SetFont(AdditionalFont);
	}

	// Wallpaper
	Screen->SetUseWallpaper(GraphData->UseWallpaper,false);

	// Color
	Graph->Palette->SetScheme(ColorData->ColorScheme);
	Graph->Palette->SetHue(ColorData->Hue);
	Graph->Palette->SetSaturation(ColorData->Saturation);
	if (ColorData->Luminance < 0) {
		Graph->Palette->SetColorLayer(0,0,0,-ColorData->Luminance);
	}
	else {
		Graph->Palette->SetColorLayer(63,63,63,ColorData->Luminance);
	}
	Graph->Palette->Install();

	// Mouse speed & sampling
	Mouse->SetSpeed(MouseData->MouseSpeed);
	Mouse->SetSampling(MouseData->PS2Sampling);

}

void CApplication::DisplayBootError()
{
	CString ErrMsg;

	ErrMsg = "Unable to boot '";
	ErrMsg += BootItems->Get(Loader->GetBootItemIndex())->ItemName;
	ErrMsg += "'.";
	Loader->ShowBootError(ErrMsg);
}

void CApplication::Shutdown()
{
	CQuit Quit(Mouse);

	Loader->Hide();
	Dialogs->SetAlertHandler(this,(TAlertProc)ShutDownReboot);
	Dialogs->ShowAlertDialog(NULL,"Shutdown","It is now safe to turn off your computer","Reboot");
	if (XOSLData->GetMiscPref()->SoftPowerOff)
		Quit.Shutdown();
}

void CApplication::ShutDownReboot(CApplication *Application)
{
	Application->Reboot();
}

void CApplication::Restart()
{
	CQuit Quit(Mouse);

	Quit.Restart();
}

void CApplication::Reboot()
{
	CQuit Quit(Mouse);

	Quit.Reboot();
}

void CApplication::DoSplashFadeOut()
{
	Graph->Palette->LockFading();
	if (!XOSLData->GetGraphics()->DisableFadeIn) {
		Graph->Palette->SetFadeOutColor(XOSLData->GetColor()->FadeColorInt);
		Graph->Palette->FadeOut();
	}
	else {
		Graph->Palette->SetFadeOutColor(0);
		Graph->Palette->UltraFadeOut();
	}
}

void CApplication::DoAppFadeIn()
{
	if (!XOSLData->GetGraphics()->DisableFadeIn)
		Graph->Palette->FadeIn();
	else
		Graph->Palette->UltraFadeIn();
}


bool CApplication::AutoBootAvail()
{
	return BootItems->GetDefault() != -1 && !BootItems->Get(BootItems->GetDefault())->Disabled;
}


#ifdef DOS_DEBUG
void CApplication::ScanForDebugKeys(int Key)
/*
 * Keys for debug information
 * F9 - Terminate XOSL
 * F8 - Refresh screen
 * F7 - Set cursor position to 0,0
 * F6 - Screen shot
 * F5 - Dump RGB palette
 * F4 - Print CoreLeft()
 */
{
	if (Key == KEY_F9) {
		printf("delete Mouse\n");
		delete Mouse;
		printf("Graph->SetMode(modeText,false);\n");
		Graph->SetMode(modeText,false);
		printf(".exit\n");
		asm mov ah,0x4c
		asm int 0x21
	}
	if (Key == KEY_F8)
		Screen->Refresh();
	if (Key == KEY_F7)
		gotoxy(0,0);
	if (Key == KEY_F6) {
		DumpScreen(FileName);
		++FileName[5];
	}
	if (Key == KEY_F5)
		DumpPalette("pal.dmp");
	if (Key == KEY_F4) {
		printf("\nCoreLeft(): %ld\n",CoreLeft());
		gotoxy(0,0);
	}
	if (Key == KEY_F1) {
		printf("\nF4 - Print CoreLeft()\nF5 - Dump RGB palette\nF6 - Screen shot\n");
		printf("F7 - Set cursor position to (0,0)\nF8 - Refresh screen\nF9 - Terminate XOSL\n");
	}
}
#endif

void CApplication::StartPart()
// have to clean-up this mess...
{
	void *Ptr1 = (void *)0x10000000;
	void *Ptr2 = (void *)0x18000000;


	if (FileSystem->ReadFile("XRPART00XXF",Ptr1) != 32768) {
		Dialogs->ShowMessageDialog(NULL,"Ranish Partition Manager","Either Ranish Partition Manager is not installed, or its files are missing");
		return;
	}

	Graph->SetMode(modeText,false);
	puts("\nStarting Ranish Partition Manager...");

	if (  FileSystem->ReadFile("XRPART01XXF",Ptr2) != 32768) {
		CriticalError("\r\nError loading XRPART01XXF\r\n");
	}

	DestroyApplication();
	FinalizeCore();
	DestroyCoreClasses();

	asm {
		xor	ax,ax
		mov es,ax
		push 0x1000
	    push 0x0200
		retf
	}

}

void CApplication::PartPwdProc(CApplication &Application, const char *Password)
{
	unsigned long Code;

	Code = EncodePassword(Password);
	if (Application.XOSLData->GetPassword() == Code) {
		Application.StartPart();
	}
	else {
		Application.Dialogs->ShowMessageDialog(NULL,"Ranish Partition Manager","Invalid Password");
	}
}

void CApplication::PerformAntiVirus()
{
	CAntiVirus AntiVirus(*FileSystem);

	AntiVirus.Sweep();
}
