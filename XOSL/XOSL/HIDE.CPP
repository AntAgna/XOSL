/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <hide.h>
#include <graph.h>

#include <fat16.h>
#include <fat32.h>
#include <mem.h>
#include <disk.h>
#include <string.h>

extern void printf(const char *,...);

CHidingDialog::CHidingDialog(CScreen &ScreenToUse, CPartList &PartListToUse, CBootItems &BootItemsToUse):
	Screen(ScreenToUse), PartList(PartListToUse), BootItems(BootItemsToUse)
{
	Initialized = false;
}
	
CHidingDialog::~CHidingDialog()
{
}


void CHidingDialog::ShowModal(CForm *Owner, int ItemIndex)
{
	if (!Initialized) {
		CreateControls();
		InitializeControls();
		InstallControls();

		PartList.ReadVolumeLabels();
		InitPartListBox();
		Initialized = true;
	}
	this->Owner = Owner;
	if (Owner) {
		Owner->Disable();
	}

	BootItemIndex = ItemIndex;
	
	UpdateLocalHideList();

	SetCaption();
	UpdateBootItemList();
	UpdateHidingList();
	if (PartListBox->GetItemIndex() != 1)
		PartListBox->SetItemIndex(1);
	else
		SelectPartition(*this,1);

	PositionDialog();
	Form->Show();
	Form->FocusControl(ApplyBtn);
}

void CHidingDialog::CreateControls()
{
	Form = new CForm("",FORM_NORMAL,true,0,0,633,298,false);

	ApplyBtn = new CButton("Apply",448,240,75,25,true,this);
	CancelBtn = new CButton("Cancel",536,240,75,25,true,this);
	PartListBox = new CListBox(4,true,8,8,422,185,true,this);
	PartScrollBar = new CScrollBar(0,0,0,false,411,9,183,true,PartListBox);
	HideCheckBox = new CCheckBox("Hide",false,12,200,true,this);
	HideAllBtn = new CButton("Hide all",12,232,81,25,true,this);
	HideNoneBtn = new CButton("Hide none",100,232,81,25,true,this);
	ItemsListBox = new CListBox(1,false,440,8,177,225,true,this);
	ItemsScrollBar = new CScrollBar(0,0,0,false,598,9,223,true,ItemsListBox);
	CopyFromBtn = new CButton("Copy from",350,200,81,25,true,this);
}

void CHidingDialog::InitializeControls()
{
	ApplyBtn->OnClick((TWndOnClick)ApplyBtnClick);
	CancelBtn->OnClick((TWndOnClick)CancelBtnClick);
	PartListBox->OnSelect((TListBoxSelect)SelectPartition);
	HideCheckBox->OnChange((TCheckBoxChange)HideCheckBoxChange);
	HideAllBtn->OnClick((TWndOnClick)HideAllBtnClick);
	HideNoneBtn->OnClick((TWndOnClick)HideNoneBtnClick);
	CopyFromBtn->OnClick((TWndOnClick)CopyFromBtnClick);

	ItemsListBox->OnDoubleClick((TListBoxSelect)SelectBootItem);

	PartListBox->SetColumn(0,147,"Volume label");
	PartListBox->SetColumn(1,50,"Disk");
	PartListBox->SetColumn(2,135,"System");
	PartListBox->SetColumn(3,70,"Size (MB)");
	PartListBox->SetScrollBar(PartScrollBar);

	ItemsListBox->SetColumn(0,157,"");
	ItemsListBox->SetScrollBar(ItemsScrollBar);

}

void CHidingDialog::InstallControls()
{
	Form->AddControl(ApplyBtn);
	Form->AddControl(CancelBtn);
	Form->AddControl(PartListBox);
	Form->AddControl(PartScrollBar);
	Form->AddControl(HideCheckBox);
	Form->AddControl(HideAllBtn);
	Form->AddControl(HideNoneBtn);
	Form->AddControl(ItemsListBox);
	Form->AddControl(ItemsScrollBar);
	Form->AddControl(CopyFromBtn);
	Screen.AddWindow(Form);
}


void CHidingDialog::InitPartListBox()
{
	int Count;
	const TPartition *Partition;
	int Row;

	Count = PartList.GetCount();
	for (Row = 0; Row < Count; ++Row) {
		PartListBox->AddRow();

		Partition = PartList.GetPartition(Row);

		AddVolume(Row,Partition->VolumeLabel);
		AddDisk(Row,Partition->Drive);
		AddSystem(Row,Partition->FSName);
		AddSize(Row,Partition->SectorCount);

	}
}

void CHidingDialog::AddVolume(int Row, const char *VolumeLabel)
{
	PartListBox->AddItem(Row,0,VolumeLabel);
}

void CHidingDialog::AddDisk(int Row, int Drive)
{
	CString Msg;

	if (Drive & 0x80)
		Msg = "HD";
	else
		Msg = "FD";
	Msg += (long)(Drive & 0x007f);
	PartListBox->AddItem(Row,1,Msg);
}

void CHidingDialog::AddSystem(int Row, const char *FSName)
{
	PartListBox->AddItem(Row,2,FSName);
}

void CHidingDialog::AddSize(int Row, long Sectors)
{
	CString Msg;

	Msg = Sectors >> 11;
	PartListBox->AddItem(Row,3,Msg);
}

void CHidingDialog::PositionDialog()
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

void CHidingDialog::CloseDialog()
{
	if (Owner) {
		Owner->Enable();
		Screen.SetFocusWnd(Owner);
	}
	Form->Hide();
}


void CHidingDialog::ApplyBtnClick(CHidingDialog &HidingDialog)
{
	HidingDialog.UpdateBootItemHideList();
	HidingDialog.CloseDialog();
}

void CHidingDialog::CancelBtnClick(CHidingDialog &HidingDialog)
{
	HidingDialog.CloseDialog();
}

void CHidingDialog::SetCaption()
{
	char Caption[84];

	strcpy(Caption,"Hiding partitions - ");
	strcat(Caption,BootItems.Get(BootItemIndex)->ItemName);
	Form->SetCaption(Caption);
}

void CHidingDialog::UpdateBootItemList()
{
	int Index, Count;

	Count = BootItems.GetCount();
	for (Index = ItemsListBox->GetCount(); Index < Count; ++Index) {
		ItemsListBox->AddRow();
	}

	for (Index = 0; Index < Count; ++Index) {
		ItemsListBox->AddItem(Index,0,BootItems.Get(Index)->ItemName);
	}
	ItemsListBox->SetDefault(BootItemIndex);
	ItemsListBox->SetItemIndex(BootItemIndex);
}

void CHidingDialog::UpdateHidingList()
{
	int Index, Count;

	Count = PartList.GetCount();
	for (Index = 0; Index < Count; ++Index) {
		PartListBox->SetRowStyle(Index,LocalHideList[BootItemIndex][Index] ? 19 : 17,STYLE_REGULAR);
	}

}

void CHidingDialog::SelectPartition(CHidingDialog &HidingDialog, int ItemIndex)
{
	if (!HidingDialog.PartList.CanHide(ItemIndex)) {
		HidingDialog.HideCheckBox->SetChecked(false);
		HidingDialog.HideCheckBox->Disable();
	}
	else {
		HidingDialog.HideCheckBox->Enable();
		HidingDialog.HideCheckBox->SetChecked(HidingDialog.LocalHideList[HidingDialog.BootItemIndex][ItemIndex]);
	}
}

void CHidingDialog::SelectBootItem(CHidingDialog &HidingDialog, int ItemIndex)
{
	HidingDialog.ItemsListBox->SetDefault(ItemIndex);
	HidingDialog.BootItemIndex = ItemIndex;
	HidingDialog.SetCaption();
	HidingDialog.UpdateHidingList();
	SelectPartition(HidingDialog,HidingDialog.PartListBox->GetItemIndex());
}

void CHidingDialog::HideCheckBoxChange(CHidingDialog &HidingDialog, bool Checked)
{
	int Index;

	Index = HidingDialog.PartListBox->GetItemIndex();
	HidingDialog.LocalHideList[HidingDialog.BootItemIndex][Index] = Checked;
	HidingDialog.PartListBox->SetRowStyle(Index,Checked ? 19 : 17,STYLE_REGULAR);
}

void CHidingDialog::HideAllBtnClick(CHidingDialog &HidingDialog)
{
	int Index, Count;
	int PartIndex;

	PartIndex = HidingDialog.BootItems.Get(HidingDialog.BootItemIndex)->PartIndex;
	Count = HidingDialog.PartList.GetCount();
	for (Index = 0; Index < Count; ++Index)
		if (HidingDialog.PartList.CanHide(Index) && Index != PartIndex)
			HidingDialog.LocalHideList[HidingDialog.BootItemIndex][Index] = true;
	HidingDialog.UpdateHidingList();
	SelectPartition(HidingDialog,HidingDialog.PartListBox->GetItemIndex());
}

void CHidingDialog::HideNoneBtnClick(CHidingDialog &HidingDialog)
{
	int Index, Count;

	Count = HidingDialog.PartList.GetCount();
	for (Index = 0; Index < Count; ++Index)
		HidingDialog.LocalHideList[HidingDialog.BootItemIndex][Index] = false;
	HidingDialog.UpdateHidingList();
	SelectPartition(HidingDialog,HidingDialog.PartListBox->GetItemIndex());
}

void CHidingDialog::CopyFromBtnClick(CHidingDialog &HidingDialog)
{
	memcpy(HidingDialog.LocalHideList[HidingDialog.BootItemIndex],HidingDialog.LocalHideList[HidingDialog.ItemsListBox->GetItemIndex()],sizeof (char[56]));
	HidingDialog.UpdateHidingList();
	SelectPartition(HidingDialog,HidingDialog.PartListBox->GetItemIndex());
}

void CHidingDialog::UpdateLocalHideList()
{
	int Index, Count;

	Count = BootItems.GetCount();
	for (Index = 0; Index < Count; ++Index) {
		memcpy(LocalHideList[Index],BootItems.Get(Index)->HideList,sizeof (char[56]));
	}
}

void CHidingDialog::UpdateBootItemHideList()
{
	int Index, Count;

	Count = BootItems.GetCount();
	for (Index = 0; Index < Count; ++Index) {
		memcpy(BootItems.Get(Index)->HideList,LocalHideList[Index],sizeof (char[56]));
	}
}
