/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <loader.h>
#include <about.h>
#include <setup.h>
#include <prefer.h>


#include <text.h>
#include <string.h>
#include <Timer.h>

#include <strtable.h>

void printf(const char *format, ...)
{
	unsigned short *argl;
	char buf[32];

	for (argl = &((unsigned short *)&format)[2]; *format; ++format)
		switch (*format) {
			case '\n':
				puts("\r\n");
				break;
			case '%':
				switch (*++format) {
					case 'c':
						putch(*argl++);
						break;
					case 'd':
						itoa(*argl++,buf,DEC);
						puts(buf);
						break;
					case 'x':
						itoa(*argl++,buf,HEX);
						puts(buf);
						break;
					case 's':
						puts((const char *)*(long *)argl);
						argl += 2;
						break;
					case 'l':
						if (format[1] == 'd') {
							++format;
							itoa(*(unsigned long *)argl++,buf,DEC);
							puts(buf);
							break;
						}
						if (format[1] == 'x') {
							++format;
							itoa(*(unsigned long *)argl++,buf,HEX);
							puts(buf);
							break;
						}
					default:
						putch('%');
						--format;
						break;
				}
				break;
			default:
				putch(*format);
				break;
		}
}



extern char *LogoBitmap;



static const char *IndexPrefix = "123456789ABCDEFGHIJKLMNO";


CLoader::CLoader(CScreen &ScreenToUse, CBootItems &BootItemsToUse, CXOSLData &XoslDataToUse,
				 CPartList &PartListToUse, CMouse &MouseToUse):

	Setup(*this,ScreenToUse,PartListToUse,BootItemsToUse,XoslDataToUse),
	About(*this,ScreenToUse,XoslDataToUse),
	Preference(*this,Setup,About,ScreenToUse,XoslDataToUse,MouseToUse),

	Screen(ScreenToUse), BootItems(BootItemsToUse), XoslData(XoslDataToUse),

	Dialogs(ScreenToUse)
{
	Initialized = false;
	Form = NULL;
	BootBtnPressed = false;
}

CLoader::~CLoader()
{
}


void CLoader::Show(bool AutoBoot)
{
	if (!Initialized) {
		Initialize(AutoBoot);
		CenterWindow();
		if (XoslData.GetGraphics()->Font9) {
			RealignText();
		}
	}

	if (!Form->Visible) {
		Form->Show();
		Form->FocusControl(BootBtn);
	}
	else
		Screen.SetFocusWnd(Form);
}

void CLoader::Hide()
{
	if (!Form) {
		return;
	}
	if (Form->Visible)
		Form->Hide();
	Setup.Hide();
	Preference.Hide();
	About.Hide();
}

int CLoader::IsVisible()
{
	if (!Form) {
		return false;
	}
	return Form->Visible && Form->IsEnabled();
}

void CLoader::Focus()
{
	if (Form) {
		Screen.SetFocusWnd(Form);
	}
}

int CLoader::GotFocus()
{
	if (!Form) {
		return false;
	}
	return Screen.GetFocusWnd() == Form;
}


void CLoader::Initialize(bool AutoBoot)
{
	CreateControls(AutoBoot);
	InitializeControls();
	ConnectEventHandlers();
	InstallControls();
	CreateBootList();
	Initialized = true;
}


void CLoader::CreateControls(bool AutoBoot)
{	 
	int Width;

	Form = new CForm(StringTable->Loader.FormTitle,FORM_NORMAL,true,100,100,461,328,false);
	Image = new CImage(LogoBitmap,218,146,true,8,16,220,148,true);
	Width = Graph->GetTextWidth(StringTable->Loader.Version,STYLE_REGULAR);
	Label1 = new CLabel(StringTable->Loader.Version,STYLE_REGULAR,true,17,8 + (218 - Width >> 1),168,Width,true);
	Width = Graph->GetTextWidth(StringTable->Loader.Copyright,STYLE_REGULAR);
	Label2 = new CLabel(StringTable->Loader.Copyright,STYLE_REGULAR,true,17,8 + (218 - Width >> 1),184,Width,true);
	Width = Graph->GetTextWidth(StringTable->Loader.URL,STYLE_REGULAR);
	Label3 = new CLabel(StringTable->Loader.URL,STYLE_REGULAR,true,22,8 + (218 - Width >> 1),200,Width,true);
	Label5 = new CLabel(StringTable->Loader.ChooseOS,STYLE_REGULAR,false,17,248,9,true);
	Bevel = new CBevel(BEVEL_FRAME,true,240,16,209,193,true);
	BootBtn = new CButton(StringTable->Loader.BootBtnText,307,224,75,25,true,this);
	SetupBtn = new CButton(StringTable->Loader.SetupBtnText,8,272,65,25,true,this);
	PreferBtn = new CButton(StringTable->Loader.PreferBtnText,80,272,75,25,true,this);
	AboutBtn = new CButton(StringTable->Loader.AboutBtnText,160,272,65,25,true,this);
	BootItemsList = new CRadioGroup(256,32,173,24 * 7,true,this);
	ScrollBar = new CScrollBar(0,0,0,true,432,16,193,false,BootItemsList);
	
	if (AutoBoot) {
		Timeout = ((unsigned long)BootItems.GetTimeout() << 16) / 3600;
		CreateBootString(0);
	}
	// auto boot controls
	Width = Graph->GetTextWidth(StringTable->Loader.EndTimer,STYLE_REGULAR);
	Label4 = new CLabel(StringTable->Loader.EndTimer,STYLE_REGULAR,true,17,8 + (218 - Width >> 1),240,Width,AutoBoot);
	BootLabel = new CLabel(BootString,STYLE_REGULAR,true,17,240,264,AutoBoot);
	ProgressBar = new CProgressBar(0,BootItems.GetTimeout() << 4,0,240,280,209,17,AutoBoot);
}

void CLoader::InitializeControls()
{
	BootBtn->SetFontStyle(STYLE_BOLD);
	BootItemsList->SetHighlight(true);
	BootItemsList->SetScrollBar(ScrollBar);
}

void CLoader::ConnectEventHandlers()
{
	BootBtn->OnClick((TWndOnClick)BootBtnClick);
	AboutBtn->OnClick((TWndOnClick)AboutBtnClick);
	SetupBtn->OnClick((TWndOnClick)SetupBtnClick);
	PreferBtn->OnClick((TWndOnClick)PreferBtnClick);

	BootBtn->OnKeyPress((TWndOnKeyPress)ButtonKeyPress);
	AboutBtn->OnKeyPress((TWndOnKeyPress)ButtonKeyPress);
	SetupBtn->OnKeyPress((TWndOnKeyPress)ButtonKeyPress);
	PreferBtn->OnKeyPress((TWndOnKeyPress)ButtonKeyPress);

	BootItemsList->OnSelect((TListBoxSelect)BootItemsSelect);
	BootItemsList->OnKeyPress((TWndOnKeyPress)BootItemsKeyPress);
}

void CLoader::InstallControls()
{
	int Index;

	Form->AddControl(Bevel);
	Form->AddControl(Image);
	Form->AddControl(Label1);
	Form->AddControl(Label2);
	Form->AddControl(Label3);
	Form->AddControl(Label4);
	Form->AddControl(Label5);
	Form->AddControl(BootLabel);
	Form->AddControl(BootBtn);
	Form->AddControl(SetupBtn);
	Form->AddControl(PreferBtn);
	Form->AddControl(AboutBtn);
	Form->AddControl(ProgressBar);
	Form->AddControl(BootItemsList);
	Form->AddControl(ScrollBar);

	Screen.AddWindow(Form);
}



void CLoader::CreateBootList()
{
	int Count;
	int ItemIndex, ListIndex;
	CBootItem *BootItem;
	char ItemName[64] = "x. ";
	bool DisplayIndex = XoslData.GetGraphics()->DisplayItemIndex;
	int MaxItemWidth = 154;

	BootItemsList->Clear();
	Count = BootItems.GetCount();

	for (ItemIndex = ListIndex = 0; ItemIndex < Count; ++ItemIndex) {
		BootItem = BootItems.Get(ItemIndex);
		

	if (!BootItem->Disabled) {
			if (!DisplayIndex) {
				UpdateMaxItemWidth(MaxItemWidth,BootItem->ItemName);
				BootItemsList->AddButton(BootItem->ItemName);
			}
			else {
				ItemName[0] = IndexPrefix[ListIndex];
				strcpy(&ItemName[3],BootItem->ItemName);
				UpdateMaxItemWidth(MaxItemWidth,ItemName);
				BootItemsList->AddButton(ItemName);
			}
			InListBootIndex[ListIndex] = ItemIndex;
			++ListIndex;
		}
	}

	ResizeLoader(MaxItemWidth);

	if (ListIndex) {
		BootBtn->Enable();
		BootItemsList->Enable();
		BootItemsSelect(*this,0);
	}
	else {
		BootBtn->Disable();
		BootItemsList->Disable();
	}
	if (Initialized) {
		BootItemsList->Refresh();
	}
}

void CLoader::UpdateMaxItemWidth(int &MaxItemWidth, const char *ItemName)
{
	int ItemWidth;

	ItemWidth = Graph->GetTextWidth(ItemName,STYLE_BOLD);
	if (ItemWidth > MaxItemWidth) {
		MaxItemWidth = ItemWidth;
	}
}

void CLoader::ResizeLoader(int MaxItemWidth)
{
	int DeltaWidth;

	DeltaWidth = MaxItemWidth - 154;
	ScrollBar->SetPosition(432 + DeltaWidth,16);
	Bevel->SetMetrics(209 + DeltaWidth,193);
	BootItemsList->SetMetrics(173 + DeltaWidth,24 * 7);
	BootBtn->SetPosition(307 + DeltaWidth / 2,224);
	Form->SetMetrics(461 + DeltaWidth,328);
}



void CLoader::CenterWindow()
{
	if (Form) {
		Screen.CenterWindow(Form);
	}
}

void CLoader::RealignText()
{
	int Left,Top;

	if (!Form) {
		return;
	}

	Label1->SetVisible(false);
	Label2->SetVisible(false);
	Label3->SetVisible(false);
	Label5->SetVisible(false);

	Label1->GetPosition(Left,Top);
	Left = 8 + (218 - Graph->GetTextWidth(StringTable->Loader.Version,STYLE_REGULAR) >> 1);
	Label1->SetPosition(Left,Top);

	Label2->GetPosition(Left,Top);
	Left = 8 + (218 - Graph->GetTextWidth(StringTable->Loader.Copyright,STYLE_REGULAR) >> 1);
	Label2->SetPosition(Left,Top);

	Label3->GetPosition(Left,Top);
	Left = 8 + (218 - Graph->GetTextWidth(StringTable->Loader.URL,STYLE_REGULAR) >> 1);
	Label3->SetPosition(Left,Top);

//	Label4->GetPosition(Left,Top);
//	Left = 8 + (218 - Graph->GetTextWidth(StringTable->Loader.EndTimer,STYLE_REGULAR) >> 1);
//	Label4->SetPosition(Left,Top);

	Label5->SetCaption(StringTable->Loader.ChooseOS);

	Label1->SetVisible(true);
	Label2->SetVisible(true);
	Label3->SetVisible(true);
	Label5->SetVisible(true);

}

int CLoader::CanBoot()
{
	return BootBtnPressed;
}

int CLoader::GetBootItemIndex()
{
	return BootItemIndex;
}

void CLoader::SelectDefault()
{
	int Index, Count;

	BootItemIndex = BootItems.GetDefault();
	if (Initialized) {
		Count = BootItemsList->GetButtonCount();
		for (Index = 0; Index < Count; ++Index) {
			if (InListBootIndex[Index] == BootItemIndex) {
				BootItemsList->SetChecked(Index);
				return;
			}
		}
	}
}

void CLoader::ShowBootError(const char *ErrMsg)
{
	Dialogs.ShowMessageDialog(Form,StringTable->Loader.BootError,ErrMsg);
	BootBtnPressed = false;
}

/*	  
void CLoader::CycleWindows()
// Loader - Setup - Preference - About
{
	int VisibleWin[4];
	int Index;

	if (!Form) {
		return;
	}
	if (GotFocus())
		Index = 1;
	else
		if (Setup.GotFocus())
			Index = 2;
		else
			if (Preference.GotFocus())
				Index = 3;
			else
				if (About.GotFocus())
					Index = 0;
				else
					return; // Shutdown is pressed, without Soft power-off

	VisibleWin[0] = IsVisible();
	VisibleWin[1] = Setup.IsVisible();
	VisibleWin[2] = Preference.IsVisible();
	VisibleWin[3] = About.IsVisible();
	while (!VisibleWin[Index])
		Index = (Index + 1) & 3;
	switch (Index) {
		case 0:
			Show();
			break;
		case 1:
			Setup.Show();
			break;
		case 2:
			Preference.Show();
			break;
		case 3:
			About.Show();
			break;
	} 
}  */


// event handlers

void CLoader::BootBtnClick(CLoader &Loader)
{
	const CBootItem *Item;

	Loader.HandleAutoSave();
	Item = Loader.BootItems.Get(Loader.BootItemIndex);
	if (Item->Password) {
		Loader.Dialogs.SetPasswordHandler(&Loader,(TPwdProc)BootPwdProc);
		Loader.Dialogs.ShowPasswordDialog(Loader.Form,StringTable->Loader.EnterPassword);
	}
	else
		Loader.BootBtnPressed = true;
}

void CLoader::BootItemsSelect(CLoader &Loader, int ItemIndex)
{
	Loader.BootItemIndex = Loader.InListBootIndex[ItemIndex];
}


void CLoader::AboutBtnClick(CLoader &Loader)
{
	Loader.About.Show();
}

void CLoader::SetupBtnClick(CLoader &Loader)
{
	if (!Loader.Setup.IsVisible() && Loader.XoslData.GetPassword()) {
		Loader.EnterSetup = true;
		Loader.Dialogs.SetPasswordHandler(&Loader,(TPwdProc)PwdProc);
		Loader.Dialogs.ShowPasswordDialog(Loader.Form,StringTable->Loader.SetupPassTitle);
	}
	else
		Loader.Setup.Show();
}

void CLoader::PreferBtnClick(CLoader &Loader)
{
	if (!Loader.Preference.IsVisible() && Loader.XoslData.GetPassword()) {
		Loader.EnterSetup = false;
		Loader.Dialogs.SetPasswordHandler(&Loader,(TPwdProc)PwdProc);
		Loader.Dialogs.ShowPasswordDialog(Loader.Form,StringTable->Loader.PreferPassTitle);
	}
	else {
		Loader.Preference.Show();
	}
}


void CLoader::PwdProc(CLoader &Loader, const char *Password)
{
	unsigned long Code;

	Code = EncodePassword(Password);
	if (Loader.XoslData.GetPassword() == Code)
		if (Loader.EnterSetup)
			Loader.Setup.Show();
		else
			Loader.Preference.Show();
	else
		if (Loader.EnterSetup)
			Loader.Dialogs.ShowMessageDialog(Loader.Form,StringTable->Loader.SetupPassTitle,StringTable->Loader.InvalidPassword);
		else
			Loader.Dialogs.ShowMessageDialog(Loader.Form,StringTable->Loader.PreferPassTitle,StringTable->Loader.InvalidPassword);
}

void CLoader::BootPwdProc(CLoader &Loader, const char *Password)
{
	unsigned long Code;

	Code = EncodePassword(Password);
	if (Loader.BootItems.Get(Loader.BootItemIndex)->Password == Code)
		Loader.BootBtnPressed = true;
	else
		Loader.Dialogs.ShowMessageDialog(Loader.Form,StringTable->Loader.BootPassTitle,StringTable->Loader.InvalidPassword);
}

void CLoader::BootItemsKeyPress(CLoader &Loader, unsigned short &Key)
{
	if (Key == KEY_ENTER || Key == KEY_K_ENTER) {
		BootBtnClick(Loader);
	}
	else {
		if (Key != KEY_UP && Key != KEY_UP_EXT && Key != KEY_DOWN && Key != KEY_DOWN_EXT)
			ButtonKeyPress(Loader,Key);
	}
}


void CLoader::ButtonKeyPress(CLoader &Loader, unsigned short &Key)
{
	int Index, Count;
	CBootItem *Entry;
	int lKey;

	Count = Loader.BootItemsList->GetButtonCount();
	if (!Count || !Key)
		return;

	// check for hotkeys
	for (Index = 0; Index < Count; ++Index) {
		Entry = Loader.BootItems.Get(Loader.InListBootIndex[Index]);
		if (Entry->Hotkey == Key) {
			Loader.BootItemsList->SetChecked(Index);
			// always instantly boot
			BootBtnClick(Loader);
			Key = 0;
			return;
		}
	}
	// check for 1-9,a-n
	lKey = tolower(Key & 0xff);
	if (Key == KEY_UP || Key == KEY_UP_EXT || Key == KEY_DOWN || Key == KEY_DOWN_EXT) {
		Loader.Form->FocusControl(Loader.BootItemsList);
		Loader.BootItemsList->KeyPress(Key);
	}
	else
		if ( (lKey >= '1' && lKey <= '9') ||
			  (lKey >= 'a' && lKey <= 'n') ) {

			if (lKey >= '1' && lKey <= '9')
				Index = lKey - '1';
			else
				Index = lKey - 'a' + 9;
			if (Index < Count) {
				Loader.BootItemsList->SetChecked(Index);
				Loader.BootBtnClick(Loader);
			}
		}
		else
			return;
	Key = 0;
}

bool CLoader::GetIgnoreNextKey()
{
	return Setup.GetIgnoreNextKey() || Preference.GetIgnoreNextKey();
}

void CLoader::HandleAutoSave()
{
	if (Form) {
		Setup.Hide();
		Preference.Hide();
	}
}



void CLoader::CreateBootString(unsigned long TicksPassed)
{
	int Sec, Min;
	
	TicksPassed *= 3600;
	Sec = (Timeout * 3600 - TicksPassed) >> 16;
	
	if (Sec >= 60) {
		BootString = StringTable->Loader.TimerMinPre;
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
		BootString += StringTable->Loader.TimerMinPost;
	}
	else {
		BootString = StringTable->Loader.TimerSecPre;
		BootString += Sec;
		BootString += StringTable->Loader.TimerSecPost;
	}

}

unsigned short CLoader::WaitTimeout()
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
			Label4->Hide();
			BootLabel->Hide();
			ProgressBar->Hide();
#endif
			Graph->ShowCursor();
			return Key;
		}
	
		if (TimePassed >= Timeout || Key == KEY_ENTER || Key == KEY_K_ENTER) {
			return -1;
		}
	
	}
}

