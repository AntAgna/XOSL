
#include <Default.h>
#include <String.h>
#include <Timer.h>

#include <Graph.h>

CDefaultBoot::CDefaultBoot(CScreen &ScreenToUse, CLoader &LoaderToUse):
	Screen(ScreenToUse), Loader(LoaderToUse)
{
}

CDefaultBoot::~CDefaultBoot()
{
}

void CDefaultBoot::Show(const char *BootItemName, int TimeoutValue)
{
	int Status;

	 this->BootItemName = BootItemName;
	Timeout = ((unsigned long)TimeoutValue << 16) / 3600;

	CreateBootString(0);
	CreateControls(TimeoutValue << 4);
	InstallControls();
	Screen.CenterWindow(Form,0,-100);
	Form->Show();
}

void CDefaultBoot::CreateControls(int ProgressBarMax)
{
	int Width;

	BootLabel = new CLabel(BootString,STYLE_REGULAR,true,17,8,12,true);

	Width = Graph->GetTextWidth(BootString,STYLE_REGULAR);
	Width += 16;
	if (Width < 241) {
		Width = 241;
	}
	Form = new CForm("XOSL auto boot",FORM_NORMAL,true,100,100,Width + 24,104,false);
	ProgressBar = new CProgressBar(0,ProgressBarMax,0,8,28,Width,13,true);
	KeyLabel = new CLabel("Press Enter to boot or Escape to abort...",STYLE_REGULAR,true,17,8,56,true);
}

void CDefaultBoot::InstallControls()
{
	Form->AddControl(BootLabel);
	Form->AddControl(ProgressBar);
	Form->AddControl(KeyLabel);
	Screen.AddWindow(Form);
}


void CDefaultBoot::CreateBootString(unsigned long TicksPassed)
{
	int Sec, Min;
	
	TicksPassed *= 3600;
	Sec = (Timeout * 3600 - TicksPassed) >> 16;
	
	BootString = "Booting ";
	BootString += BootItemName;
	BootString += " in ";

	if (Sec >= 60) {
		Min = Sec / 60;
		Sec %= 60;
		BootString += Min;
		if (Sec >= 10) {
			BootString += ".";
		}
		else {
			BootString += ".0";
		}
		BootString += Sec;
		BootString += " minutes";
	}
	else {
		BootString += Sec;
		BootString += " seconds";
	}

}

int CDefaultBoot::WaitTimeout()
{
	unsigned long Time1, Time2, TimePassed, Progress;
	unsigned short Key = 0;

	Time1 = GetTimerTicks();
	for (;;) {
		Time2 = GetTimerTicks();
		TimePassed = Time2 - Time1;
		CreateBootString(TimePassed);
		BootLabel->SetCaption(BootString);
		Progress = (TimePassed * 3600) >> 12;
		ProgressBar->SetProgress(Progress);

		if (CKeyboard::KeyStrokeAvail()) {
			Key = CKeyboard::WaitKeyStroke();
		}


		if (Key && Key != KEY_ENTER && Key != KEY_K_ENTER) {
#ifndef DOS_DEBUG
			Form->Hide();
#endif
			Loader.Show();
			Loader.SelectDefault();
			Graph->ShowCursor();
			return Key;
		}
	
		if (TimePassed >= Timeout || Key == KEY_ENTER || Key == KEY_K_ENTER) {
			Loader.SelectDefault();
			return -1;
		}
	
	}
}
/*

	int X, Y;
	unsigned long Time1, Time2, Timeout, TimePassed;
	long Progress;
	int Sec, Min;
	CString BootStr;
	int ContinueIB = 1;

//	Setup->SetTimerSlider(BootItems->GetTimeout());
	// Handle Instant Boot here

	Timeout = ((unsigned long)BootItems->GetTimeout() << 16) / 3600;
	TimePassed = 0;
	Time1 = GetTimerTicks();
	while (ContinueIB) {
		Time2 = GetTimerTicks();
		TimePassed = Time2 - Time1;
		Progress = TimePassed * 3600;
		Sec = BootItems->GetTimeout() - (Progress >> 16);
		Progress >>= 12;
		BootStr = "Booting in ";
		if (Sec >= 60) {
			Min = Sec / 60;
			Sec %= 60;
			BootStr += Min;
			if (Sec >= 10)
				BootStr += ".";
			else
				BootStr += ".0";
			BootStr += Sec;
			BootStr += " minutes.";
		}
		else {
			BootStr += Sec;
			BootStr += " seconds.";
		}
		Loader->SetTimeoutLabelCaption(BootStr);
		Loader->SetTimeoutProgress(Progress);
		Mouse->GetXY(X,Y);
		Graph->SetCursorXY(X,Y);

		ContinueIB = !Keyboard->KbHit() && TimePassed <= Timeout;
		ContinueIB = ContinueIB && !Mouse->MouseDown();
	}
#ifndef DOS_DEBUG
	Loader->AbortInstantBoot();
#endif
	if (!Keyboard->KbHit() && !Mouse->MouseDown())
		return -1;*/

