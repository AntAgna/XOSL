/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <part.h>
#include <graph.h>

#include <fat16.h>
#include <fat32.h>
#include <mem.h>
#include <disk.h>
#include <key.h>

CPartDialog::CPartDialog(CScreen &ScreenToUse, CPartList &PartListToUse):
	Screen(ScreenToUse), PartList(PartListToUse)
{
	Initialized = false;
	HandlerClass = NULL;
}

CPartDialog::~CPartDialog()
{
}


void CPartDialog::ShowModal(CForm *Owner, const char *Caption, const char *DefaultName, int DefaultIndex)
{
	if (!Initialized) {
		CreateControls();
		InitializeControls();
		InstallControls();

		PartList.ReadVolumeLabels();
		InitializeDialog();
		Initialized = true;
	}
	this->Owner = Owner;
	if (Owner) {
		Owner->Disable();
	}

	if (DefaultName) {
		AutoName = false;
		NameEdit->SetText(DefaultName);
	}
	else {
		AutoName = true;
		PartListBoxSelect(*this,DefaultIndex);
	}
	PartListBox->SetItemIndex(DefaultIndex);
	PositionDialog();
	Form->SetCaption(Caption);
	Form->Show();
	Form->FocusControl(PartListBox);
}

void CPartDialog::SetOnApply(void *HandlerClass, TPartDialogOnApply Handler)
{
	this->HandlerClass = HandlerClass;
	OnApply = Handler;
}

void CPartDialog::GetSelection(int &ItemIndex, char *ItemName)
{
	ItemIndex = PartListBox->GetItemIndex();
	NameEdit->GetText(ItemName);
}


void CPartDialog::CreateControls()
{
	Form = new CForm("",FORM_NORMAL,true,0,0,527,266,false);

	ApplyBtn = new CButton("Apply",344,204,75,25,true,this);
	CancelBtn = new CButton("Cancel",424,204,75,25,true,this);
	PartListBox = new CListBox(5,true,16,8,487,185,true,this);
	ScrollBar = new CScrollBar(0,0,0,false,484,9,183,true,PartListBox);
	Label = new CLabel("Boot item name:",STYLE_REGULAR,true,17,18,206,true);
	NameEdit = new CEdit("",47,false,false,120,204,209,true,this);
}

void CPartDialog::InitializeControls()
{
	ApplyBtn->OnClick((TWndOnClick)ApplyBtnClick);
    CancelBtn->OnClick((TWndOnClick)CancelBtnClick);
	PartListBox->OnSelect((TListBoxSelect)PartListBoxSelect);
	NameEdit->OnKeyPress((TWndOnKeyPress)NameEditKeyPress);
	PartListBox->OnKeyPress((TWndOnKeyPress)PartListBoxKeyPress);
//	ScrollBar->OnKeyPress((TWndOnKeyPress)PartListBoxKeyPress);

	PartListBox->SetColumn(0,50,"Disk");
	PartListBox->SetColumn(1,65,"Type");
	PartListBox->SetColumn(2,135,"System");
	PartListBox->SetColumn(3,70,"Size (Mb)");
	PartListBox->SetColumn(4,147,"Volume label");
	PartListBox->SetScrollBar(ScrollBar);
}

void CPartDialog::InstallControls()
{
	Form->AddControl(ApplyBtn);
	Form->AddControl(CancelBtn);
	Form->AddControl(PartListBox);
	Form->AddControl(ScrollBar);
	Form->AddControl(Label);
	Form->AddControl(NameEdit);
	Screen.AddWindow(Form);
}


void CPartDialog::InitializeDialog()
{
	int Count;
	const TPartition *Partition;
	int Row;

	Count = PartList.GetCount();
	for (Row = 0; Row < Count; ++Row) {
		PartListBox->AddRow();

		Partition = PartList.GetPartition(Row);

		AddDisk(Row,Partition->Drive);
		AddType(Row,Partition->Type);
		AddSystem(Row,Partition->FSName);
		AddSize(Row,Partition->SectorCount);
		AddVolume(Row,Partition->VolumeLabel);

	}
	PartListBox->SetItemIndex(1);
}

void CPartDialog::AddDisk(int Row, int Drive)
{
	CString Msg;

	if (Drive & 0x80)
		Msg = "HD";
	else
		Msg = "FD";
	Msg += (long)(Drive & 0x007f);
	PartListBox->AddItem(Row,0,Msg);
}

void CPartDialog::AddType(int Row, int Type)
{
	static const char *Types[] = {
		"primary","logical","mbr","floppy","loader"
	};

	PartListBox->AddItem(Row,1,Types[Type]);
}

void CPartDialog::AddSystem(int Row, const char *FSName)
{
	PartListBox->AddItem(Row,2,FSName);
}

void CPartDialog::AddSize(int Row, long Sectors)
{
	CString Msg;

	Msg = Sectors >> 11;
	PartListBox->AddItem(Row,3,Msg);
}

void CPartDialog::AddVolume(int Row, const char *VolumeLabel)
{
	PartListBox->AddItem(Row,4,VolumeLabel);
}

void CPartDialog::PositionDialog()
{
	int Left, Top;
	int ScreenWidth, ScreenHeight;
	int OwnerWidth, OwnerHeight;
	int FormWidth, FormHeight;

	Form->GetMetrics(FormWidth,FormHeight);
	if (!Owner) {
		Graph->GetModeMetrics(ScreenWidth,ScreenHeight);
		Left = (ScreenWidth - FormWidth) / 2;
		Top = (ScreenHeight - FormHeight) / 2 - 32;
	}
	else {
		Owner->GetMetrics(OwnerWidth,OwnerHeight);
		Owner->GetPosition(Left,Top);
		Left += (OwnerWidth - FormWidth) / 2;
		Top += 10;
	}
	Form->SetPosition(Left,Top);
}

void CPartDialog::CloseDialog()
{
	if (Owner) {
		Owner->Enable();
		Screen.SetFocusWnd(Owner);
	}
	Form->Hide();
}

void CPartDialog::ApplyBtnClick(CPartDialog &PartDialog)
{
	PartDialog.CloseDialog();
	if (PartDialog.HandlerClass) {
		PartDialog.OnApply(PartDialog.HandlerClass);
	}
}

void CPartDialog::CancelBtnClick(CPartDialog &PartDialog)
{
	PartDialog.CloseDialog();
}

void CPartDialog::PartListBoxSelect(CPartDialog &PartDialog, int ItemIndex)
{
	if (PartDialog.AutoName) {
		PartDialog.NameEdit->SetText(PartDialog.PartList.GetPartition(ItemIndex)->VolumeLabel);
	}
}

void CPartDialog::NameEditKeyPress(CPartDialog &PartDialog, unsigned short &Key)
{
	PartDialog.AutoName = false;
	if (Key == KEY_ENTER || Key == KEY_K_ENTER) {
		Key = KEY_NONE;
		PartDialog.ApplyBtnClick(PartDialog);
	}
}

void CPartDialog::PartListBoxKeyPress(CPartDialog &PartDialog, int &Key)
{
	if (Key == KEY_ESCAPE) {
		PartDialog.CloseDialog();
	}
	else {
		PartDialog.Form->FocusControl(PartDialog.NameEdit);
		PartDialog.NameEdit->KeyPress(Key);
	}
}
