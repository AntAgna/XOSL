/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <prefer.h>

#include <loader.h>

#include <text.h>
extern void printf(const char *fmt,...);

#define TAB_GRAPHICS 0
#define TAB_COLOR    1
#define TAB_MOUSE    2
#define TAB_PASSWORD 3
#define TAB_MISC     4





CPreference::CPreference(CLoader &LoaderToUse, CSetup &SetupToUse, CAbout &AboutToUse, CScreen &ScreenToUse, 
						 CXOSLData &XoslDataToUse, CMouse &MouseToUse):
	Loader(LoaderToUse), Screen(ScreenToUse), XoslData(XoslDataToUse), 
	Dialogs(ScreenToUse),
	GraphSettings(XoslDataToUse,*this,SetupToUse,LoaderToUse,AboutToUse,ScreenToUse,MouseToUse),
	ColorSettings(XoslDataToUse),
	MouseSettings(XoslDataToUse,Dialogs,MouseToUse),
	XoslPassword(XoslDataToUse,Dialogs),
	Miscellaneous(XoslDataToUse,*this)
{

	IgnoreKey = false;
	Initialized = false;
	Form = NULL;
}

CPreference::~CPreference()
{
}

void CPreference::Show()
{
	if (!Initialized) {
		Initialize();
		CenterWindow();
		if (XoslData.GetGraphics()->Font9) {
			RealignText();
		}
	}

	if (!Form->Visible) {
		Form->Show();
		Form->FocusControl(CloseBtn);
	}
	else
		Screen.SetFocusWnd(Form);
}

void CPreference::Hide()
{
	if (Form && Form->Visible) {
		if (XoslData.GetMiscPref()->AutoSave) {
			XoslData.SaveData();
		}
		Form->Hide();
	}
}

int CPreference::IsVisible()
{
	if (!Form) {
		return false;
	}
	return Form->Visible && Form->IsEnabled();
}

bool CPreference::GotFocus()
{
	if (!Form) {
		return false;
	}
	return Screen.GetFocusWnd() == Form;
}

int CPreference::GetIgnoreNextKey()
{
	if (IgnoreKey) {
		IgnoreKey = 0;
		return 1;
	}
	return 0;
}

void CPreference::SetIgnoreNextKey()
{
	IgnoreKey = 1;
}

void CPreference::Initialize()
{
	CreateControls();
	InitializeControls();
	ConnectEventHandlers();
	InitializeData();
	InstallControls();
	Initialized = true;
}


void CPreference::CreateControls()
{
	// main controls
	Form = new CForm("XOSL Preference",FORM_NORMAL,true,200,200,478,385,false);
	TabBevel = new CBevel(BEVEL_BOX,false,8,8,457,290,true);
	TabControl = new CTabControl(5,8,297,370,true,this);
	ResetBtn = new CButton("Reset",205,328,75,25,true,this);
	SaveBtn = new CButton("Save",292,328,75,25,true,this);
	CloseBtn = new CButton("Close",391,328,75,25,true,this);

	GraphSettings.CreateControls();
	ColorSettings.CreateControls();
	MouseSettings.CreateControls();
	XoslPassword.CreateControls();
	Miscellaneous.CreateControls();
}

void CPreference::InitializeControls()
{
	int Index;

	TabControl->SetTabCaption(0,"Graphics");
	TabControl->SetTabCaption(1,"Color");
	TabControl->SetTabCaption(2,"Mouse/Keyboard");
	TabControl->SetTabCaption(3,"Password");
	TabControl->SetTabCaption(4,"Misc");
	TabControl->SetTabPage(TabBevel);

	GraphSettings.InitializeControls(TabControl);
	ColorSettings.InitializeControls(TabControl);
	MouseSettings.InitializeControls(TabControl);
	XoslPassword.InitializeControls(TabControl);
	Miscellaneous.InitializeControls(TabControl);
}

void CPreference::ConnectEventHandlers()
{
	CloseBtn->OnClick((TWndOnClick)CloseBtnClick);
	ResetBtn->OnClick((TWndOnClick)ResetBtnClick);
	SaveBtn->OnClick((TWndOnClick)SaveBtnClick);
	TabControl->OnChange((TTabChange)TabChange);

	GraphSettings.ConnectEventHandlers();
	ColorSettings.ConnectEventHandlers();
	MouseSettings.ConnectEventHandlers();
	XoslPassword.ConnectEventHandlers();
	Miscellaneous.ConnectEventHandlers();
}

void CPreference::InitializeData()
{
	GraphSettings.InitializeData();
	ColorSettings.InitializeData();
	MouseSettings.InitializeData();
	Miscellaneous.InitializeData();
}


void CPreference::InstallControls()
{
	Form->AddControl(TabBevel);
	Form->AddControl(TabControl);
	Form->AddControl(ResetBtn);
	Form->AddControl(SaveBtn);
	Form->AddControl(CloseBtn);
	Screen.AddWindow(Form);

	GraphSettings.InstallControls(Form);
	ColorSettings.InstallControls(Form);
	MouseSettings.InstallControls(Form);
	XoslPassword.InstallControls(Form);
	Miscellaneous.InstallControls(Form);
}

void CPreference::CenterWindow()
{
	if (Form) {
		Screen.CenterWindow(Form);
	}
}

void CPreference::RealignText()
{
	if (Form) {
		GraphSettings.RealignText();
		ColorSettings.RealingText();
		MouseSettings.RealignText();
		XoslPassword.RealignText();
		Miscellaneous.RealignText();
	}
}



void CPreference::CloseBtnClick(CPreference &Preference)
{
	Preference.Hide();
	Preference.Loader.Focus();
}

void CPreference::ResetBtnClick(CPreference &Preference)
{
	switch (Preference.TabControl->GetActiveTab()) {
		case TAB_GRAPHICS:
			Preference.XoslData.ResetGraphics();
			Preference.GraphSettings.InitializeData();
			break;
		case TAB_COLOR:
			Preference.XoslData.ResetColor();
			Preference.ColorSettings.InitializeData();
			break;
		case TAB_MOUSE:
			Preference.XoslData.ResetMouse();
			Preference.MouseSettings.InitializeData();
			break;
		case TAB_MISC:
			Preference.XoslData.ResetMiscPref();
			Preference.Miscellaneous.InitializeData();
			break;
		default:
			break;
	}
}

void CPreference::SaveBtnClick(CPreference &Preference)
{
	Preference.XoslData.SaveData();
	Preference.Dialogs.ShowMessageDialog(Preference.Form,"Save","Data saved.");
}

void CPreference::TabChange(CPreference &Preference, int ActiveTab)
{
	if (ActiveTab == 3)
		Preference.ResetBtn->Disable();
	else
		Preference.ResetBtn->Enable();
}



