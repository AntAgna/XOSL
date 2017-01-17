
#include <Misc.h>
#include <prefer.h>
#include <loader.h>

static const char *StrMiscKeys = "Misc keys";
static const char *StrHandling = "Boot handling";
static const char *StrPartMan = "Ranish Partition Manager";
	
	

CMiscellaneous::CMiscellaneous(CXOSLData &XoslDataToUse, CPreference &PreferenceToUse):
	XoslData(XoslDataToUse), Preference(PreferenceToUse)
{
	Initialized = false;
}

CMiscellaneous::~CMiscellaneous()
{

}

void CMiscellaneous::CreateControls()
{
	KeysBevel = new CBevel(BEVEL_FRAME,true,44,34,161,248,false);
	KeysLabel = new CLabel(StrMiscKeys,STYLE_REGULAR,false,17,52,27,false);
	CycleLabel = new CLabel("Cycle windows",STYLE_REGULAR,true,17,68,50,false);
	CycleEdit = new CEdit("",10,false,false,68,66,113,false,this);
	RestartLabel = new CLabel("Restart XOSL",STYLE_REGULAR,true,17,68,106,false);
	RestartEdit = new CEdit("",10,false,false,68,122,113,false,this);
	RebootLabel = new CLabel("Cold reboot",STYLE_REGULAR,true,17,68,154,false);
	RebootEdit = new CEdit("",10,false,false,68,170,113,false,this);
	ShutdownLabel = new CLabel("Shutdown",STYLE_REGULAR,true,17,68,202,false);
	ShutdownEdit = new CEdit("",10,false,false,68,218,113,false,this);
	PowerOff = new CCheckBox("Soft power off ",false,76,250,false,this);

	HandlingBevel = new CBevel(BEVEL_FRAME,true,212,34,217,149,false);
	HandlingLabel = new CLabel(StrHandling,STYLE_REGULAR,false,17,220,27,false);

	AutoSave = new CCheckBox("Auto save",false,236,54,false,this);
	AutoBootDialog = new CCheckBox("No auto boot dialog",false,236,78,false,this);
	SetInstBoot = new CCheckBox("Auto boot last boot item",false,236,102,false,this);
	ActiveAllow = new CCheckBox("Allow active partition per HD",false,236,126,false,this);
	AntiVirus = new CCheckBox("MBR virus protection",false,236,150,false,this);

	PartManBevel = new CBevel(BEVEL_FRAME,true,212,202,217,80,false);
	PartManLabel = new CLabel(StrPartMan,STYLE_REGULAR,false,17,220,193,false);
	PartKeyLabel = new CLabel("Execute on",STYLE_REGULAR,true,17,236,220,false);
	PartKeyEdit = new CEdit("",10,false,false,236,236,161,false,this);
}

void CMiscellaneous::InitializeControls(CTabControl *TabControl)
{
	TabControl->AddControl(4,KeysBevel);
	TabControl->AddControl(4,KeysLabel);
	TabControl->AddControl(4,CycleLabel);
	TabControl->AddControl(4,CycleEdit);
	TabControl->AddControl(4,RestartLabel);
	TabControl->AddControl(4,RestartEdit);
	TabControl->AddControl(4,RebootLabel);
	TabControl->AddControl(4,RebootEdit);
	TabControl->AddControl(4,ShutdownLabel);
	TabControl->AddControl(4,ShutdownEdit);
	TabControl->AddControl(4,PowerOff);

	TabControl->AddControl(4,HandlingBevel);
	TabControl->AddControl(4,HandlingLabel);

	TabControl->AddControl(4,AutoSave);
	TabControl->AddControl(4,AutoBootDialog);
	TabControl->AddControl(4,SetInstBoot);
	TabControl->AddControl(4,ActiveAllow);
	TabControl->AddControl(4,AntiVirus);

	TabControl->AddControl(4,PartManBevel);
	TabControl->AddControl(4,PartManLabel);
	TabControl->AddControl(4,PartKeyLabel);
	TabControl->AddControl(4,PartKeyEdit);

}

void CMiscellaneous::ConnectEventHandlers()
{
	AutoSave->OnChange((TCheckBoxChange)AutoSaveChange);
	AutoBootDialog->OnChange((TCheckBoxChange)AutoBootDialogChange);
	SetInstBoot->OnChange((TCheckBoxChange)SetInstBootChange);
	ActiveAllow->OnChange((TCheckBoxChange)ActiveAllowChange);
	AntiVirus->OnChange((TCheckBoxChange)AntiVirusChange);

	CycleEdit->OnKeyPress((TWndOnKeyPress)CycleEditKeyPress);
	RebootEdit->OnKeyPress((TWndOnKeyPress)RebootEditKeyPress);
	ShutdownEdit->OnKeyPress((TWndOnKeyPress)ShutdownEditKeyPress);
	RestartEdit->OnKeyPress((TWndOnKeyPress)RestartEditKeyPress);
	PowerOff->OnChange((TCheckBoxChange)PowerOffChange);

	PartKeyEdit->OnKeyPress((TWndOnKeyPress)PartKeyEditKeyPress);
}

void CMiscellaneous::InitializeData()
{
	TMiscPref *MiscPref;

	MiscPref = XoslData.GetMiscPref();

	AutoSave->SetChecked(MiscPref->AutoSave);
	AutoBootDialog->SetChecked(MiscPref->NoAutoBootDialog);
	SetInstBoot->SetChecked(MiscPref->SetAutoBoot);
	ActiveAllow->SetChecked(MiscPref->ActiveAllow);
	AntiVirus->SetChecked(MiscPref->AntiVirus);

	CreateKeyStr(RebootEdit,MiscPref->KeyColdReboot);
	CreateKeyStr(ShutdownEdit,MiscPref->KeyShutdown);
	CreateKeyStr(RestartEdit,MiscPref->KeyRestart);
	CreateKeyStr(CycleEdit,MiscPref->KeyCycle);
	PowerOff->SetChecked(MiscPref->SoftPowerOff);

	CreateKeyStr(PartKeyEdit,MiscPref->KeyPart);

	Initialized = true;

}

void CMiscellaneous::InstallControls(CForm *Form)
{
	Form->AddControl(KeysBevel);
	Form->AddControl(KeysLabel);
	Form->AddControl(CycleLabel);
	Form->AddControl(CycleEdit);
	Form->AddControl(RestartLabel);
	Form->AddControl(RestartEdit);
	Form->AddControl(RebootLabel);
	Form->AddControl(RebootEdit);
	Form->AddControl(ShutdownLabel);
	Form->AddControl(ShutdownEdit);
	Form->AddControl(PowerOff);

	Form->AddControl(HandlingBevel);
	Form->AddControl(HandlingLabel);
	
	Form->AddControl(AutoSave);
	Form->AddControl(AutoBootDialog);
	Form->AddControl(SetInstBoot);
	Form->AddControl(ActiveAllow);
	Form->AddControl(AntiVirus);

	Form->AddControl(PartManBevel);
	Form->AddControl(PartManLabel);
	Form->AddControl(PartKeyLabel);
	Form->AddControl(PartKeyEdit);

}

void CMiscellaneous::RealignText()
{
	bool Visible;

	Visible = KeysLabel->IsVisible();
	
	KeysLabel->SetVisible(false);
	HandlingLabel->SetVisible(false);
	PartManLabel->SetVisible(false);
	
	KeysLabel->SetCaption(StrMiscKeys);
	HandlingLabel->SetCaption(StrHandling);
	PartManLabel->SetCaption(StrPartMan);
	
	KeysLabel->SetVisible(Visible);
	HandlingLabel->SetVisible(Visible);
	PartManLabel->SetVisible(Visible);
}

void CMiscellaneous::CreateKeyStr(CEdit *Edit, int Key)
{
	char KeyName[64];

	CKeyboard::GetKeyName(Key,KeyName);
	Edit->SetText(KeyName);

}




// --------------------- event handlers --------------------- 


void CMiscellaneous::ActiveAllowChange(CMiscellaneous &Miscellaneous, int Checked)
{
	if (Miscellaneous.Initialized) {
		Miscellaneous.XoslData.GetMiscPref()->ActiveAllow = Checked;
	}
}

void CMiscellaneous::SetInstBootChange(CMiscellaneous &Miscellaneous, int Checked)
{
	if (Miscellaneous.Initialized) {
		Miscellaneous.XoslData.GetMiscPref()->SetAutoBoot = Checked;
	}
}

void CMiscellaneous::CycleEditKeyPress(CMiscellaneous &Miscellaneous, int &Key)
{
	char KeyName[64];

	Miscellaneous.XoslData.GetMiscPref()->KeyCycle = Key;
	Miscellaneous.Preference.SetIgnoreNextKey();
	CKeyboard::GetKeyName(Key,KeyName);
	Miscellaneous.CycleEdit->SetText(KeyName);
	Key = KEY_END;
}

void CMiscellaneous::RebootEditKeyPress(CMiscellaneous &Miscellaneous, int &Key)
{
	char KeyName[64];

	Miscellaneous.XoslData.GetMiscPref()->KeyColdReboot = Key;
	Miscellaneous.Preference.SetIgnoreNextKey();
	CKeyboard::GetKeyName(Key,KeyName);
	Miscellaneous.RebootEdit->SetText(KeyName);
	Key = KEY_END;
}

void CMiscellaneous::ShutdownEditKeyPress(CMiscellaneous &Miscellaneous, int &Key)
{
	char KeyName[64];

	Miscellaneous.XoslData.GetMiscPref()->KeyShutdown = Key;
	Miscellaneous.Preference.SetIgnoreNextKey();
	CKeyboard::GetKeyName(Key,KeyName);
	Miscellaneous.ShutdownEdit->SetText(KeyName);
	Key = KEY_END;
}

void CMiscellaneous::RestartEditKeyPress(CMiscellaneous &Miscellaneous, int &Key)
{
	char KeyName[64];

	Miscellaneous.XoslData.GetMiscPref()->KeyRestart = Key;
	Miscellaneous.Preference.SetIgnoreNextKey();
	CKeyboard::GetKeyName(Key,KeyName);
	Miscellaneous.RestartEdit->SetText(KeyName);
	Key = KEY_END;
}

void CMiscellaneous::PowerOffChange(CMiscellaneous &Miscellaneous, int Checked)
{
	if (Miscellaneous.Initialized) {
		Miscellaneous.XoslData.GetMiscPref()->SoftPowerOff = Checked;
	}
}

void CMiscellaneous::AutoSaveChange(CMiscellaneous &Miscellaneous, int Checked)
{
	Miscellaneous.XoslData.GetMiscPref()->AutoSave = Checked;
}

void CMiscellaneous::AutoBootDialogChange(CMiscellaneous &Miscellaneous, int Checked)
{
	Miscellaneous.XoslData.GetMiscPref()->NoAutoBootDialog = Checked;
}

void CMiscellaneous::AntiVirusChange(CMiscellaneous &Miscellaneous, int Checked)
{
	Miscellaneous.XoslData.GetMiscPref()->AntiVirus = Checked;
}

void CMiscellaneous::PartKeyEditKeyPress(CMiscellaneous &Miscellaneous, int &Key)
{
	char KeyName[64];

	Miscellaneous.XoslData.GetMiscPref()->KeyPart = Key;
	Miscellaneous.Preference.SetIgnoreNextKey();
	CKeyboard::GetKeyName(Key,KeyName);
	Miscellaneous.PartKeyEdit->SetText(KeyName);
	Key = KEY_END;
}

