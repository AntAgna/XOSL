
#include <Password.h>
#include <Setup.h>
#include <encpwd.h>

static const char *StrPassword = "Password";

CPassword::CPassword(CSetup &SetupToUse, CBootItems &BootItemsToUse, CDialogs &DialogsToUse):
	Setup(SetupToUse),
	BootItems(BootItemsToUse),
	Dialogs(DialogsToUse)
{
}

CPassword::~CPassword()
{
}

void CPassword::CreateControls()
{
	PasswordGroupBevel = new CBevel(BEVEL_FRAME,true,28,237,337,122,false);
	PasswordGroupLabel = new CLabel(StrPassword,STYLE_REGULAR,false,17,36,230,false);
	NewPassLabel = new CLabel("New password",STYLE_REGULAR,true,17,52,254,false);
	NewPassEdit = new CEdit("",32,true,false,52,270,129,false,this);
	ConfirmPassLabel = new CLabel("Confirm password",STYLE_REGULAR,true,17,212,254,false);
	ConfirmPassEdit = new CEdit("",32,true,false,212,270,129,false,this);
	ApplyPassBtn = new CButton("Apply",264,298,75,25,false,this);
	ClearPassBtn = new CButton("Clear",53,298,75,25,false,this);
}

void CPassword::InitializeControls(CTabControl *TabControl)
{
	TabControl->AddControl(1,PasswordGroupBevel);
	TabControl->AddControl(1,PasswordGroupLabel);
	TabControl->AddControl(1,NewPassLabel);
	TabControl->AddControl(1,NewPassEdit);
	TabControl->AddControl(1,ConfirmPassLabel);
	TabControl->AddControl(1,ConfirmPassEdit);
	TabControl->AddControl(1,ApplyPassBtn);
	TabControl->AddControl(1,ClearPassBtn);
}

void CPassword::ConnectEventHandlers()
{
	ApplyPassBtn->OnClick((TWndOnClick)ApplyPassBtnClick);
	ClearPassBtn->OnClick((TWndOnClick)ClearPassBtnClick);
}

void CPassword::DisableControls()
{
	NewPassEdit->Disable();
	ConfirmPassEdit->Disable();
	ApplyPassBtn->Disable();
	ClearPassBtn->Disable();
}

void CPassword::EnableControls()
{
	NewPassEdit->Enable();
	ConfirmPassEdit->Enable();
	ApplyPassBtn->Enable();
	ClearPassBtn->Enable();
}

void CPassword::InstallControls(CForm *Form)
{
	Form->AddControl(PasswordGroupBevel);
	Form->AddControl(PasswordGroupLabel);
	Form->AddControl(NewPassLabel);
	Form->AddControl(NewPassEdit);
	Form->AddControl(ConfirmPassLabel);
	Form->AddControl(ConfirmPassEdit);
	Form->AddControl(ApplyPassBtn);
	Form->AddControl(ClearPassBtn);
}

void CPassword::RealignText()
{
	bool Visible; 

	Visible = PasswordGroupLabel->IsVisible();
	PasswordGroupLabel->SetVisible(false);
	PasswordGroupLabel->SetCaption(StrPassword);
	PasswordGroupLabel->SetVisible(Visible);
}



void CPassword::ApplyPassBtnClick(CPassword &Password)
{
	unsigned long Code;
	char PasswordStr[48];

	Password.NewPassEdit->GetText(PasswordStr);
	Code = EncodePassword(PasswordStr);
	Password.ConfirmPassEdit->GetText(PasswordStr);
	if (Code == EncodePassword(PasswordStr)) {
		Password.BootItems.Get(Password.Setup.BootItemIndex)->Password = Code;
		Password.Setup.BootItemList->AddItem(Password.Setup.BootItemIndex,1,Code ? "yes" : "no");
		Password.Setup.BootItemList->Refresh();
	}
	else {
		Password.Dialogs.ShowMessageDialog(Password.Setup.Form,"New password","Passwords do not match.");
	}
}

void CPassword::ClearPassBtnClick(CPassword &Password)
{
	Password.NewPassEdit->SetText("");
	Password.ConfirmPassEdit->SetText("");
	Password.ApplyPassBtnClick(Password);
}

