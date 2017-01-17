
#include <General.h>
#include <Setup.h>
#include <Loader.h>


	
static const char *StrGeneralStatus = "Status";
static const char *StrGeneralTimeout = "Timeout";
	
	
CGeneral::CGeneral(CSetup &SetupToUse, CBootItems &BootItemsToUse, CScreen &ScreenToUse, CPartList &PartListToUse):
	Setup(SetupToUse),
	BootItems(BootItemsToUse),
	PartList(PartListToUse),
	HidingDialog(ScreenToUse,PartListToUse,BootItemsToUse)
{
}

CGeneral::~CGeneral()
{
}

void CGeneral::CreateControls()
{
	StatusGroupBevel = new CBevel(BEVEL_FRAME,true,28,237,169,122,true);
	StatusGroupLabel = new CLabel(StrGeneralStatus,STYLE_REGULAR,false,17,36,230,true);
	Activate = new CCheckBox("Activate partition",false,44,254,true,this);
	FATFix = new CCheckBox("Fix FAT drive number",false,44,278,true,this);
	SwapDrives = new CCheckBox("Swap drives",false,44,302,true,this);
	Disabled = new CCheckBox("Disabled",false,44,326,true,this);

	TimeoutGroupBevel = new CBevel(BEVEL_FRAME,true,212,237,153,82,true);
	TimeoutGroupLabel = new CLabel(StrGeneralTimeout,STYLE_REGULAR,false,17,220,230,true);
	TimeoutSlider = new CTrackBar(0,81,0,228,254,121,true,this);
	TimeoutLabel = new CLabel("0 sec",STYLE_REGULAR,true,17,292,270,true);
	DefaultItem = new CCheckBox("Default boot item",false,228,294,true,this);
	
	HidingBtn = new CButton("Hiding",251,334,75,25,true,this);
}

void CGeneral::InitializeControls(CTabControl *TabControl)
{
	TabControl->AddControl(0,StatusGroupBevel);
	TabControl->AddControl(0,StatusGroupLabel);
	TabControl->AddControl(0,Activate);
	TabControl->AddControl(0,FATFix);
	TabControl->AddControl(0,SwapDrives);
	TabControl->AddControl(0,Disabled);
	TabControl->AddControl(0,TimeoutGroupBevel);
	TabControl->AddControl(0,TimeoutGroupLabel);
	TabControl->AddControl(0,TimeoutSlider);
	TabControl->AddControl(0,TimeoutLabel);
	TabControl->AddControl(0,DefaultItem);
	TabControl->AddControl(0,HidingBtn);

	SetTimerSlider(BootItems.GetTimeout());
}

void CGeneral::ConnectEventHandlers()
{
	DefaultItem->OnChange((TCheckBoxChange)DefaultItemClick);
	Activate->OnChange((TCheckBoxChange)ActivateClick);
	FATFix->OnChange((TCheckBoxChange)FATFixClick);
	SwapDrives->OnChange((TCheckBoxChange)SwapDrivesClick);
	Disabled->OnChange((TCheckBoxChange)DisabledClick);
	HidingBtn->OnClick((TWndOnClick)HidingBtnClick);
	TimeoutSlider->OnChange((TTrackBarChange)TimeoutSliderChange);

}

void CGeneral::DisableControls()
{
	DefaultItem->Disable();
	Activate->Disable();
	FATFix->Disable();
	SwapDrives->Disable();
	Disabled->Disable();
	HidingBtn->Disable();
}

void CGeneral::EnableControls()
{
	DefaultItem->Enable();
	Activate->Enable();
	HidingBtn->Enable();
}

void CGeneral::InstallControls(CForm *Form)
{
	Form->AddControl(StatusGroupBevel);
	Form->AddControl(StatusGroupLabel);
	Form->AddControl(Activate);
	Form->AddControl(FATFix);
	Form->AddControl(SwapDrives);
	Form->AddControl(Disabled);
	Form->AddControl(TimeoutGroupBevel);
	Form->AddControl(TimeoutGroupLabel);
	Form->AddControl(TimeoutSlider);
	Form->AddControl(TimeoutLabel);
	Form->AddControl(DefaultItem);
	Form->AddControl(HidingBtn);
}

void CGeneral::RealignText()
{
	bool Visible;

	Visible = StatusGroupLabel->IsVisible();
	StatusGroupLabel->SetVisible(false);
	TimeoutGroupLabel->SetVisible(false);
	StatusGroupLabel->SetCaption(StrGeneralStatus);
	TimeoutGroupLabel->SetCaption(StrGeneralTimeout);
	StatusGroupLabel->SetVisible(Visible);
	TimeoutGroupLabel->SetVisible(Visible);
}

void CGeneral::SelectBootItem(int ItemIndex)
{
	CBootItem *Entry;

	Entry = BootItems.Get(ItemIndex);
	DefaultItem->SetChecked(BootItems.IsDefault(ItemIndex));
	Activate->SetChecked(Entry->Activate);
	FATFix->SetChecked(Entry->FixDriveNum);
	SwapDrives->SetChecked(Entry->SwapDrives);
	Disabled->SetChecked(Entry->Disabled);
	if (BootItems.CanFixFAT(ItemIndex)) {
		FATFix->Enable();
	}
	else {
		FATFix->Disable();
	}

	if (BootItems.CanSwap(ItemIndex)) {
		SwapDrives->Enable();
	}
	else {
		SwapDrives->Disable();
	}

	if (Entry->PartIndex >= PartList.GetCount()) {
		Disabled->Disable();
	}
	else {
		Disabled->Enable();
	}

	if (PartList.CanActivate(Entry->PartIndex)) {
		Activate->Enable();
	}
	else {
		Activate->Disable();
	}

}

void CGeneral::ToggleDefault()
{
	if (!BootItems.IsDefault(Setup.BootItemIndex)) {
		DefaultItem->SetChecked(true);
	}
	else {
		DefaultItem->SetChecked(false);
	}
}

void CGeneral::SetTimerSlider(int Timeout)
{
	int Value;

	if (Timeout >= 300)
		Value = (Timeout + 1530) / 30;
	else
		if (Timeout >= 60)
			Value = (Timeout + 615) / 15;
		else
			if (Timeout >= 30)
				Value = (Timeout + 30) / 2;
			else
				Value = Timeout;
	TimeoutSlider->SetValue(Value);
	TimeoutSliderChange(*this,Value);
}


// -----------------------------

void CGeneral::DefaultItemClick(CGeneral &General, bool Checked)
{
	int BootItemIndex = General.Setup.BootItemIndex;
	int DefaultIndex = Checked ? BootItemIndex : -1;

	if (Checked || BootItemIndex == General.BootItems.GetDefault()) {
		General.BootItems.SetDefault(DefaultIndex);
		General.Setup.BootItemList->SetDefault(DefaultIndex);
	}
}

void CGeneral::ActivateClick(CGeneral &General, bool Checked)
{
	General.BootItems.Get(General.Setup.BootItemIndex)->Activate = Checked;
}

void CGeneral::FATFixClick(CGeneral &General, bool Checked)
{
	General.BootItems.Get(General.Setup.BootItemIndex)->FixDriveNum = Checked;
}

void CGeneral::SwapDrivesClick(CGeneral &General, bool Checked)
{
	General.BootItems.Get(General.Setup.BootItemIndex)->SwapDrives = Checked;
}


void CGeneral::DisabledClick(CGeneral &General, bool Checked)
{
	General.BootItems.Get(General.Setup.BootItemIndex)->Disabled = Checked;
	General.Setup.BootItemList->SetRowStyle(General.Setup.BootItemIndex,Checked ? 19 : 17,STYLE_REGULAR);
	General.Setup.Loader.CreateBootList();
}

void CGeneral::HidingBtnClick(CGeneral &General)
{
	General.HidingDialog.ShowModal(General.Setup.Form,General.Setup.BootItemIndex);
}


void CGeneral::TimeoutSliderChange(CGeneral &General, int Value)
{
	CString Time;
	int Min, Sec;

	if (Value > 60)
		Value = 30 * Value - 1530;
	else
		if (Value > 44)
			Value = 15 * Value - 615;
		else
			if (Value > 30)
				Value = 2 * Value - 30;

	if (Value >= 60) {
		Min = Value / 60;
		Sec = Value % 60;
		Time = Min;
		if (Sec < 10)
			Time += ".0";
		else
			Time += ".";
		Time += Sec;
		Time += " min";
	}
	else {
		Time = Value;
		Time += " sec";
	}
	General.TimeoutLabel->SetCaption(Time);
	General.BootItems.SetTimeout(Value);
}
