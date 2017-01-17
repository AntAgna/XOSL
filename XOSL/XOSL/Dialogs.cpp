/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <graph.h>
#include <gui.h>
#include <screen.h>
#include <text.h>
#include <dialogs.h>
#include <key.h>
#include <quit.h>

#define ClientHeight 105


CDialogs::CDialogs(CScreen &ScreenToUse):
	Screen(ScreenToUse)
{
	PwdObject = NULL;
	AlertObject = NULL;

	Form = new CForm("",FORM_NORMAL,true,0,0,204,ClientHeight + 24,false);
	BodyText = new CLabel("",STYLE_REGULAR,true,17,16,24,true);
	BodyEdit = new CEdit("",32,true,false,16,26,168,false,this);
	BodyEdit->OnKeyPress((TWndOnKeyPress)PwdKeyPress);
	CloseBtn = new CButton("Ok",62,64,75,25,true,this);
	CloseBtn->OnClick((TWndOnClick)CloseBtnClick);
	Form->AddControl(BodyText);
	Form->AddControl(BodyEdit);
	Form->AddControl(CloseBtn);
	Screen.AddWindow(Form);
}

CDialogs::~CDialogs()
{
}

void CDialogs::ShowMessageDialog(CForm *Parent, const char *Title, const char *Text)
{
	this->Parent = Parent;
	DialogType = dtMessage;
	ShowMessageBox(Title,Text,"Ok");
}

void CDialogs::ShowPasswordDialog(CForm *Parent, const char *Title)
{
	int ScrnWidth, ScrnHeight;
	int BoxLeft, BoxTop;

	this->Parent = Parent;
	DialogType = dtPassword;

	Graph->GetModeMetrics(ScrnWidth,ScrnHeight);
	BoxLeft = (ScrnWidth - 204) >> 1;
	BoxTop = (ScrnHeight - ClientHeight - 24) >> 1;
	if (Parent)
		Parent->Disable();
	Form->SetVisible(false);
	Form->SetCaption(Title);
	Form->SetPosition(BoxLeft,BoxTop);
	Form->SetMetrics(204,ClientHeight + 32);
	BodyText->Hide();
	BodyEdit->Show();
	BodyEdit->SetText("");
	CloseBtn->SetCaption("Ok");
	CloseBtn->SetPosition(62,72);
	Form->FocusControl(BodyEdit);
	Form->Show();
}

void CDialogs::ShowAlertDialog(CForm *Parent, const char *Title, const char *Text, const char *ButtonCaption)
{
	this->Parent = Parent;
	DialogType = dtAlert;
	ShowMessageBox(Title,Text,ButtonCaption);
}

void CDialogs::SetPasswordHandler(void *HandlerClass, TPwdProc Handler)
{
	PwdObject = HandlerClass;
	PwdProc = Handler;
}

void CDialogs::SetAlertHandler(void *HandlerClass, TAlertProc Handler)
{
	AlertObject = HandlerClass;
	AlertProc = Handler;
}


void CDialogs::CloseBtnClick(CDialogs &Dialogs)
{
	char Password[33];

	Dialogs.Form->Hide();
	if (Dialogs.Parent) {
		Dialogs.Parent->Enable();
		Dialogs.Screen.SetFocusWnd(Dialogs.Parent);
	}
	switch (Dialogs.DialogType) {
		case dtMessage:
			break;
		case dtPassword:
			if (Dialogs.PwdObject) {
				Dialogs.BodyEdit->GetText(Password);
				Dialogs.PwdProc(Dialogs.PwdObject,Password);
			}
			break;
		case dtAlert:
			if (Dialogs.AlertObject)
				Dialogs.AlertProc(Dialogs.AlertObject);
			break;
		default:
			break;
	}
}

void CDialogs::PwdKeyPress(CDialogs &Dialogs, unsigned short &Key)
{
	if (Key == KEY_ENTER || Key == KEY_K_ENTER) {
		Key = KEY_NONE;
		Dialogs.CloseBtnClick(Dialogs);
	}
}

void CDialogs::ShowMessageBox(const char *Title, const char *Text, const char *ButtonCaption)
{
	int ClientWidth;
	int ButtonLeft;
	int ScrnWidth, ScrnHeight;
	int BoxLeft, BoxTop;
	int TextLeft;

	ClientWidth = Graph->GetTextWidth(Text,STYLE_REGULAR) + 32;
	if (ClientWidth < 200) {
		TextLeft = (232 - ClientWidth) >> 1;
		ClientWidth = 200;
	}
	else
		TextLeft = 16;
	Graph->GetModeMetrics(ScrnWidth,ScrnHeight);
	BoxLeft = (ScrnWidth - ClientWidth - 4) >> 1;
	BoxTop = (ScrnHeight - ClientHeight - 24) >> 1;
	ButtonLeft = (ClientWidth - 75) >> 1;
	if (Parent)
		Parent->Disable();
	Form->SetVisible(false);
	Form->SetCaption(Title);
	Form->SetPosition(BoxLeft,BoxTop);
	Form->SetMetrics(ClientWidth + 4,ClientHeight + 32);
	BodyEdit->Hide();
	BodyText->Show();
	BodyText->SetCaption(Text);
	BodyText->SetPosition(TextLeft,24);
	CloseBtn->SetPosition(ButtonLeft,64);
	CloseBtn->SetCaption(ButtonCaption);
	Form->FocusControl(CloseBtn);
	if (Form->IsVisible()) {
		Screen.SetFocusWnd(Form);
	}
	else {
		Form->Show();
	}
}




