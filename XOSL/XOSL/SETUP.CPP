/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <setup.h>
#include <loader.h>
#include <part.h>
#include <key.h>

#include <mem.h>
#include <cstring.h>
#include <string.h>

// NOTE: new approach: Initialize when dialog is opened!
// this will shorten the XOSL start-up time

void printf(const char *format,...);

////

/*
static const char *StrYes = "yes";
static const char *StrNo = "no";

static const char *StrMiTimerLabel = "Instant Boot";
static const char *StrMiKeyLabel = "Additional keys";
static const char *StrMiHotkeyLabel = "Hotkey";
static const char *StrMiPwdLabel = "Password";
 */
 
 
 
//////////

/*
int CSetup::UpdateIB = 1;
int CSetup::HidingUpdate;
 */

CSetup::CSetup(CLoader &LoaderToUse, CScreen &ScreenToUse, CPartList &PartListToUse, CBootItems &BootItemsToUse, CXOSLData &XoslDataToUse):
	Loader(LoaderToUse), Screen(ScreenToUse), PartList(PartListToUse), BootItems(BootItemsToUse),
	XoslData(XoslDataToUse),
	PartDialog(ScreenToUse,PartListToUse), Dialogs(ScreenToUse), 
	BootKeys(*this,BootItemsToUse), General(*this,BootItemsToUse,ScreenToUse,PartListToUse), 
	Password(*this,BootItemsToUse,Dialogs)
{
	Initialized = false;
	Form = NULL;
	IgnoreKey = false;
}

CSetup::~CSetup()
{
}

void CSetup::Show()
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

void CSetup::Hide()
{
	if (Form && Form->Visible) {
		if (XoslData.GetMiscPref()->AutoSave) {
			BootItems.Save();
		}
		Form->Hide();
	}
}

bool CSetup::IsVisible()
{
	if (!Form) {
		return false;
	}
	return Form->Visible && Form->IsEnabled();
}

bool CSetup::GotFocus()
{
	if (!Form) {
		return false;
	}
	return Screen.GetFocusWnd() == Form;
}
void CSetup::CenterWindow()
{
	if (Form) {
		Screen.CenterWindow(Form);
	}
}

void CSetup::RealignText()
{
	if (Form) {
		BootItemList->FontChanged();

		General.RealignText();
		Password.RealignText();
		BootKeys.RealignText();
	}
}


void CSetup::DisplayBootItem(int Row, bool Refresh)
{
	const CBootItem *BootItem;
	CString KeysStr;

	BootItem = BootItems.Get(Row);
	BootKeys.GetKeysString(Row,KeysStr);
	BootItemList->AddItem(Row,0,BootItem->ItemName);
	BootItemList->AddItem(Row,1,BootItem->Password ? "yes" : "no");
	BootItemList->AddItem(Row,2,KeysStr);
	BootItemList->SetRowStyle(Row,BootItem->Disabled ? 19 : 17,STYLE_REGULAR);
	if (Refresh)
		BootItemList->Refresh();
}

void CSetup::Initialize()
{
	CreateControls();
	InitializeControls();
	ConnectEventHandlers();
	if (!BootItems.GetCount()) {
		DisableControls();
	}
	else {
		DisplayBootItems(false);
		BootItemList->SetItemIndex(0);
	}
	InstallControls();
	Initialized = true;
}

void CSetup::CreateControls()
{
	// main controls
	Form = new CForm("XOSL boot items configuration",FORM_NORMAL,true,100,100,487,436,false);
	CloseBtn = new CButton("Close",396,352,75,25,true,this);
	BootItemList = new CListBox(3,true,8,8,377,169,true,this);
	ScrollBar = new CScrollBar(0,0,0,false,366,9,167,true,BootItemList);
	AddBtn = new CButton("Add",396,16,75,25,true,this);
	EditBtn = new CButton("Edit",396,48,75,25,true,this);
	CloneBtn = new CButton("Clone",396,80,75,25,true,this);
	RemoveBtn = new CButton("Remove",396,144,75,25,true,this);
	MoveUpBtn = new CButton("Move up",12,184,75,25,true,this);
	MoveDownBtn = new CButton("Move down",92,184,75,25,true,this);
	HotkeyLabel = new CLabel("Hotkey:",STYLE_REGULAR,true,17,216,186,true);
	HotkeyEdit = new CEdit("",32,false,false,264,184,117,true,this);
	TabBevel = new CBevel(BEVEL_BOX,false,8,216,377,161,true);
	TabControl = new CTabControl(3,8,376,377,true,this);
	RestoreBtn = new CButton("Reset",396,280,75,25,true,this);
	SaveBtn = new CButton("Save",396,320,75,25,true,this);

	General.CreateControls();
	Password.CreateControls();
	BootKeys.CreateControls();
}

void CSetup::InitializeControls()
{
	BootItemList->SetColumn(0,160,"Boot item name");
	BootItemList->SetColumn(1,65,"Password");
	BootItemList->SetColumn(2,132,"Boot keys");
	BootItemList->SetScrollBar(ScrollBar);

	TabControl->SetTabCaption(0,"General");
	TabControl->SetTabCaption(1,"Password");
	TabControl->SetTabCaption(2,"Keys");
	TabControl->SetTabPage(TabBevel);
	
	General.InitializeControls(TabControl);
	Password.InitializeControls(TabControl);
	BootKeys.InitializeControls(TabControl);
}

void CSetup::ConnectEventHandlers()
{
	// 
	CloseBtn->OnClick((TWndOnClick)CloseBtnClick);
	AddBtn->OnClick((TWndOnClick)AddBtnClick);
	EditBtn->OnClick((TWndOnClick)EditBtnClick);
	CloneBtn->OnClick((TWndOnClick)CloneBtnClick);
	RemoveBtn->OnClick((TWndOnClick)RemoveBtnClick);
	BootItemList->OnSelect((TListBoxSelect)BootItemSelect);
	BootItemList->OnDoubleClick((TListBoxSelect)BootItemSelectDefault);
	HotkeyEdit->OnKeyPress((TWndOnKeyPress)HotkeyEditKeyPress);

	MoveUpBtn->OnClick((TWndOnClick)MoveUpBtnClick);
	MoveDownBtn->OnClick((TWndOnClick)MoveDownBtnClick);

	SaveBtn->OnClick((TWndOnClick)SaveBtnClick);
	RestoreBtn->OnClick((TWndOnClick)RestoreBtnClick);
    
	General.ConnectEventHandlers();
	Password.ConnectEventHandlers();
	BootKeys.ConnectEventHandlers();
}

void CSetup::InstallControls()
{
	Form->AddControl(CloseBtn);
	Form->AddControl(BootItemList);
	Form->AddControl(ScrollBar);
	Form->AddControl(AddBtn);
	Form->AddControl(EditBtn);
	Form->AddControl(CloneBtn);
	Form->AddControl(RemoveBtn);
	Form->AddControl(MoveUpBtn);
	Form->AddControl(MoveDownBtn);
	Form->AddControl(HotkeyLabel);
	Form->AddControl(HotkeyEdit);

	General.InstallControls(Form);
	Password.InstallControls(Form);
	BootKeys.InstallControls(Form);

	Form->AddControl(TabBevel);
	Form->AddControl(TabControl);

	Form->AddControl(RestoreBtn);
	Form->AddControl(SaveBtn);
	Screen.AddWindow(Form);
}

void CSetup::DisableControls()
{
	BootItemList->Disable();
	EditBtn->Disable();
	CloneBtn->Disable();
	RemoveBtn->Disable();
	MoveUpBtn->Disable();
	MoveDownBtn->Disable();
	HotkeyEdit->Disable();
	HotkeyEdit->SetText("");

	General.DisableControls();
	Password.DisableControls();
	BootKeys.DisableControls();
}

void CSetup::EnableControls()
{
	BootItemList->Enable();
	EditBtn->Enable();
	CloneBtn->Enable();
	RemoveBtn->Enable();
	HotkeyEdit->Enable();

	General.EnableControls();
	Password.EnableControls();
	BootKeys.EnableControls();
}

//-------------------------------------
void CSetup::AddBootItem(CSetup &Setup)
{
	CBootItem Entry;
	int NewIndex;
	CBootItem *NewBootItem;

	memset(&Entry,0,sizeof (CBootItem));
	Setup.PartDialog.GetSelection(NewIndex,Entry.ItemName);
	Entry.PartIndex = NewIndex;

	Entry.Activate = Setup.PartList.CanActivate(Entry.PartIndex);
	Setup.BootItems.Add(Entry);

	NewIndex = Setup.BootItems.GetCount() - 1;
	NewBootItem = Setup.BootItems.Get(NewIndex);
	if (Setup.BootItems.CanFixFAT(NewIndex)) {
		NewBootItem->FixDriveNum = 1;
	}
	NewBootItem->SwapDrives = 0;

	Setup.BootItemList->AddRow();
	Setup.DisplayBootItem(NewIndex,false);
	if (!NewIndex)
		Setup.EnableControls();
	Setup.BootItemList->SetItemIndex(NewIndex);
	Setup.Loader.CreateBootList();
}

void CSetup::EditBootItem(CSetup &Setup)
{
	CBootItem *Entry;
	int NewIndex;

	Entry = Setup.BootItems.Get(Setup.BootItemIndex);
	Setup.PartDialog.GetSelection(NewIndex,Entry->ItemName);
	Entry->PartIndex = NewIndex;
	Setup.BootItemList->AddItem(Setup.BootItemIndex,0,Entry->ItemName);
	Setup.BootItemList->Refresh();
	BootItemSelect(Setup,Setup.BootItemList->GetItemIndex());
	Setup.Loader.CreateBootList();
}


void CSetup::CloseBtnClick(CSetup &Setup)
{
	Setup.Hide();
	Setup.Loader.Focus();
}

void CSetup::AddBtnClick(CSetup &Setup)
{
	if (Setup.BootItems.GetCount() == 24) {
		Setup.Dialogs.ShowAlertDialog(Setup.Form,"Error: limit reached","XOSL cannot have more than 24 boot items","Ok");
	}
	else {
		Setup.PartDialog.SetOnApply(&Setup,(TPartDialogOnApply)AddBootItem);
		Setup.PartDialog.ShowModal(Setup.Form,"Add boot item",NULL,2);
	}
}

void CSetup::EditBtnClick(CSetup &Setup)
{
	CBootItem *Entry = Setup.BootItems.Get(Setup.BootItemList->GetItemIndex());
	int PartIndex;

	Setup.PartDialog.SetOnApply(&Setup,(TPartDialogOnApply)EditBootItem);
	PartIndex = Entry->PartIndex < Setup.PartList.GetCount() ? Entry->PartIndex : 2;
	Setup.PartDialog.ShowModal(Setup.Form,"Edit boot item",Entry->ItemName,PartIndex);
	if (Entry->Disabled == 2) {
		Entry->Disabled = 1;
	}
}

void CSetup::BootItemSelect(CSetup &Setup, int ItemIndex)
{
	int Hotkey;

	if (ItemIndex != -1) {
		Setup.BootItemIndex = ItemIndex;

		Hotkey = Setup.BootItems.Get(ItemIndex)->Hotkey;
		Setup.HotkeyEditKeyPress(Setup,Hotkey);

		Setup.General.SelectBootItem(ItemIndex);
		Setup.BootKeys.UpdateKeyDisplay();
		if (!ItemIndex)
			Setup.MoveUpBtn->Disable();
		else
			Setup.MoveUpBtn->Enable();

		if (ItemIndex == Setup.BootItems.GetCount() - 1)
			Setup.MoveDownBtn->Disable();
		else
			Setup.MoveDownBtn->Enable();
	}
}

void CSetup::BootItemSelectDefault(CSetup &Setup, int)
{
	Setup.General.ToggleDefault();
}

void CSetup::SaveBtnClick(CSetup &Setup)
{
	Setup.BootItems.Save();
	Setup.Dialogs.ShowMessageDialog(Setup.Form,"Boot items","Data saved");
}

void CSetup::RestoreBtnClick(CSetup &Setup)
{
	Setup.BootItems.Reset();
	if (Setup.BootItems.GetCount()) {
		Setup.EnableControls();
		Setup.DisplayBootItems(false);
		if (Setup.BootItemList->GetItemIndex()) {
			Setup.BootItemList->SetItemIndex(0);
		}
		else {
			Setup.BootItemSelect(Setup,0);
			Setup.BootItemList->Refresh();
		}
	}
	else {
		Setup.BootItemList->Clear();
		Setup.DisableControls();
	}
	Setup.Loader.CreateBootList();
}

void CSetup::DisplayBootItems(bool Refresh)
{
	int Index, Count;

	Count = BootItems.GetCount();
	for (Index = BootItemList->GetCount(); Index < Count; ++Index)
		BootItemList->AddRow();
	for (Index = 0; Index < Count; ++Index)
		DisplayBootItem(Index,false);
	while (BootItemList->GetCount() > Count)
		BootItemList->DeleteRow(BootItemList->GetCount() - 1);
	BootItemList->SetDefault(BootItems.GetDefault());
	if (Refresh)
		BootItemList->Refresh();
}

void CSetup::MoveUpBtnClick(CSetup &Setup)
{
	Setup.BootItems.Swap(Setup.BootItemIndex,Setup.BootItemIndex - 1);
	Setup.DisplayBootItems(false);
	Setup.BootItemList->SetItemIndex(Setup.BootItemIndex - 1);
	Setup.Loader.CreateBootList();
}

void CSetup::MoveDownBtnClick(CSetup &Setup)
{
	Setup.BootItems.Swap(Setup.BootItemIndex,Setup.BootItemIndex + 1);
	Setup.DisplayBootItems(false);
	Setup.BootItemList->SetItemIndex(Setup.BootItemIndex + 1);
	Setup.Loader.CreateBootList();
}

void CSetup::CloneBtnClick(CSetup &Setup)
{
	int NewIndex;

	NewIndex = Setup.BootItems.GetCount();
	if (NewIndex == 24) {
		Setup.Dialogs.ShowAlertDialog(Setup.Form,"Error: limit reached","XOSL cannot have more than 24 boot items","Ok");
	}
	else {
		Setup.BootItems.Add(*Setup.BootItems.Get(Setup.BootItemIndex));
		Setup.BootItemList->AddRow();
		Setup.DisplayBootItem(NewIndex,false);
		Setup.BootItemList->SetItemIndex(NewIndex);
	}
	Setup.Loader.CreateBootList();
}

void CSetup::RemoveBtnClick(CSetup &Setup)
{
	Setup.BootItems.Remove(Setup.BootItemIndex);
	Setup.DisplayBootItems(false);
	if (Setup.BootItems.GetCount()) {
		if (Setup.BootItemList->GetItemIndex()) {
			Setup.BootItemList->SetItemIndex(0);
		}
		else {
			Setup.BootItemSelect(Setup,0);
			Setup.BootItemList->Refresh();
		}
	}
	else {
		Setup.DisableControls();
	}
	Setup.Loader.CreateBootList();
}




bool CSetup::GetIgnoreNextKey()
{
	if (IgnoreKey) {
		IgnoreKey = false;
		return true;
	}
	return false;
}

void CSetup::SetIgnoreNextKey()
{
	IgnoreKey = true;
}

void CSetup::HotkeyEditKeyPress(CSetup &Setup, int &Key)
{
	char KeyName[64];

	if (Key != KEY_ESCAPE && Key != KEY_NONE) {
		Setup.SetIgnoreNextKey();
		CKeyboard::GetKeyName(Key,KeyName);
	}
	else {
		Key = 0;
		KeyName[0] = '\0';
	}

	Setup.BootItems.Get(Setup.BootItemIndex)->Hotkey = Key;
	Setup.HotkeyEdit->SetText(KeyName);
	Key = KEY_END;
}
