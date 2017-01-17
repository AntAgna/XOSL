
#include <AntiVir.h>
#include <disk.h>
#include <graph.h>
#include <mem.h>
#include <quit.h>

CAntiVirus::CAntiVirus(CFileSystem &FileSystemToUse):
	FileSystem(FileSystemToUse)
{
}

CAntiVirus::~CAntiVirus()
{
}

void CAntiVirus::Sweep()
{
	char RealMBR[512];
	char DiskMBR[512];

	GetRealMBR(RealMBR);
	if (GetDiskMBR(DiskMBR) == -1) {
		DisplayError();
		CKeyboard::WaitKeyStroke();
	}
	else {
		if (memcmp(RealMBR,DiskMBR,440) != 0) {
			DisplayWarning();
			if (CKeyboard::WaitKeyStroke() == KEY_ENTER) {
				FixMBR(DiskMBR);
				DisplayResult();
				CKeyboard::WaitKeyStroke();
			}
			Reboot();
		}
	}

}

void CAntiVirus::ClearScreen()
{
	int Width, Height;

	Graph->GetModeMetrics(Width,Height);
	Graph->Bar(0,0,Width,Height,0);
	Graph->TextOut(64,64,"--- XOSL anti-virus ---",STYLE_BOLD,15);
}

void CAntiVirus::DisplayError()
{
	ClearScreen();
	Graph->TextOut(64,96,"XOSL was unable to read \"CURR_MBR.XCF\". Because of this,",STYLE_REGULAR,15);
	Graph->TextOut(64,112,"XOSL will be unable to perform \"MBR virus protection\".",STYLE_REGULAR,15);
	Graph->TextOut(64,144,"Press any key to continue with XOSL, without performing \"MBR virus protection\"...",STYLE_REGULAR,15);
	Graph->UltraFlush();
}

void CAntiVirus::DisplayWarning()
{
	ClearScreen();
	Graph->TextOut(64,96,"WARNING: XOSL has detected that your Master Boot Record does",STYLE_REGULAR,15);
	Graph->TextOut(64,112,"not match the one install by XOSL. This may indicate a virus!",STYLE_REGULAR,15);
	Graph->TextOut(64,144,"Press Enter to restore the XOSL MBR, or Escape to reboot...",STYLE_REGULAR,15);
	Graph->UltraFlush();
}

void CAntiVirus::DisplayResult()
{
	ClearScreen();
	Graph->TextOut(64,96,"The master boot record has been restored",STYLE_REGULAR,15);
	Graph->TextOut(64,128,"Press any key to reboot...",STYLE_REGULAR,15);
	Graph->UltraFlush();
}

void CAntiVirus::GetRealMBR(char *RealMBR)
{
	CDisk Disk;

	Disk.Map(0x80,0);
	Disk.Read(0,RealMBR,1);
}

int CAntiVirus::GetDiskMBR(char *DiskMBR)
{
	if (FileSystem.ReadFile("CURR_MBRXCF",DiskMBR) != 512) {
		return -1;
	}
	return 0;
}


void CAntiVirus::FixMBR(const char *DiskMBR)
{
	CDisk Disk;
	char MBR[512];

	Disk.Map(0x80,0);
	Disk.Read(0,MBR,1);
	memcpy(MBR,DiskMBR,440);
	Disk.Write(0,MBR,1);
}

void CAntiVirus::Reboot()
{
	CQuit Quit(NULL);

	Quit.Reboot();
}

