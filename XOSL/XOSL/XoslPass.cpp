
#include <XoslPass.h>
#include <encpwd.h>
#include <disk.h>
#include <mbrpass.h>


static const char *StrPwLabel = "XOSL Main password";


CXoslPassword::CXoslPassword(CXOSLData &XoslDataToUse, CDialogs &DialogsToUse):
	XoslData(XoslDataToUse), Dialogs(DialogsToUse)
{

}

CXoslPassword::~CXoslPassword()
{

}

void CXoslPassword::CreateControls()
{
	Group = new CBevel(BEVEL_FRAME,true,132,32,201,217,false);
	Label = new CLabel(StrPwLabel,STYLE_REGULAR,false,17,140,25,false);
	OldLabel = new CLabel("Old password:",STYLE_REGULAR,true,17,172,56,false);
	NewLabel = new CLabel("New password:",STYLE_REGULAR,true,17,172,104,false);
	ReLabel = new CLabel("Re-enter password:",STYLE_REGULAR,true,17,172,152,false);
	OldEdit = new CEdit("",32,true,false,172,72,121,false,this);
	NewEdit = new CEdit("",32,true,false,172,120,121,false,this);
	ReEdit = new CEdit("",32,true,false,172,168,121,false,this);
	ApplyBtn = new CButton("Apply",195,208,75,25,false,this);
}

void CXoslPassword::InitializeControls(CTabControl *TabControl)
{
	TabControl->AddControl(3,Group);
	TabControl->AddControl(3,Label);
	TabControl->AddControl(3,OldLabel);
	TabControl->AddControl(3,NewLabel);
	TabControl->AddControl(3,ReLabel);
	TabControl->AddControl(3,OldEdit);
	TabControl->AddControl(3,NewEdit);
	TabControl->AddControl(3,ReEdit);
	TabControl->AddControl(3,ApplyBtn);
}

void CXoslPassword::ConnectEventHandlers()
{
	ApplyBtn->OnClick((TWndOnClick)ApplyBtnClick);
}

void CXoslPassword::InstallControls(CForm *Form)
{
	PrefForm = Form;
	Form->AddControl(Group);
	Form->AddControl(Label);
	Form->AddControl(OldLabel);
	Form->AddControl(NewLabel);
	Form->AddControl(ReLabel);
	Form->AddControl(OldEdit);
	Form->AddControl(NewEdit);
	Form->AddControl(ReEdit);
	Form->AddControl(ApplyBtn);
}

void CXoslPassword::RealignText()
{
	bool Visible;

	Visible = Label->IsVisible();
	Label->SetVisible(false);
	Label->SetCaption(StrPwLabel);
	Label->SetVisible(Visible);
}




void CXoslPassword::ApplyBtnClick(CXoslPassword &XoslPassword)
{
	unsigned long Code;
	unsigned long OldCode;
	char Password[48];

	OldCode = XoslPassword.XoslData.GetPassword();
	XoslPassword.OldEdit->GetText(Password);
	Code = EncodePassword(Password);
	if (Code != OldCode) {
		XoslPassword.Dialogs.ShowMessageDialog(XoslPassword.PrefForm,"Old password","Invalid password.");
		return;
	}
	XoslPassword.NewEdit->GetText(Password);
	Code = EncodePassword(Password);
	XoslPassword.ReEdit->GetText(Password);
	if (Code != EncodePassword(Password)) {
		XoslPassword.Dialogs.ShowMessageDialog(XoslPassword.PrefForm,"New password","Passwords do not match.");
		return;
	}

	XoslPassword.SetMBRPassword(Code);

	XoslPassword.XoslData.SetPassword(Code);
	XoslPassword.XoslData.SaveData();
	XoslPassword.Dialogs.ShowMessageDialog(XoslPassword.PrefForm,"Password","New password saved.");
}


void CXoslPassword::SetMBRPassword(unsigned long Code)
{
	CDisk Disk;
	CMBRPassword MBR;

	Disk.Map(0x80,0);
	Disk.Read(0,&MBR,1);

	MBR.Password = Code;

	Disk.Write(0,&MBR,1);
}

