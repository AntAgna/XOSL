
#include <bootkeys.h>
#include <Setup.h>
#include <CString.h>
#include <string.h>
#include <mem.h>
	
static const char *StrKeys = "Boot keys";		
		
CBootKeys::CBootKeys(CSetup &SetupToUse, CBootItems &BootItemsToUse):
	Setup(SetupToUse),
	BootItems(BootItemsToUse)
{
}

CBootKeys::~CBootKeys()
{
}

void CBootKeys::CreateControls()
{
	KeysGroupBevel = new CBevel(BEVEL_FRAME,true,28,237,337,122,false);
	KeysGroupLabel = new CLabel(StrKeys,STYLE_REGULAR,false,17,36,230,false);
	KeysEdit = new CEdit("",48,false,false,52,270,289,false,this);
	KeyName = new CLabel("",STYLE_REGULAR,true,17,252,266,false);
	BackspaceBtn = new CButton("Backspc",53,298,67,25,false,this);
	TabBtn = new CButton("Tab",126,298,67,25,false,this);
	ShiftTabBtn = new CButton("Shift-Tab",199,298,67,25,false,this);
	EscapeBtn = new CButton("Escape",272,298,67,25,false,this);
}

void CBootKeys::InitializeControls(CTabControl *TabControl)
{
	TabControl->AddControl(2,KeysGroupBevel);
	TabControl->AddControl(2,KeysGroupLabel);
	TabControl->AddControl(2,KeysEdit);
	TabControl->AddControl(2,KeyName);
	TabControl->AddControl(2,BackspaceBtn);
	TabControl->AddControl(2,TabBtn);
	TabControl->AddControl(2,ShiftTabBtn);
	TabControl->AddControl(2,EscapeBtn);
}

void CBootKeys::ConnectEventHandlers()
{
	KeysEdit->OnKeyPress((TWndOnKeyPress)KeysEditKeyPress);
	BackspaceBtn->OnClick((TWndOnClick)BackspaceBtnClick);
	TabBtn->OnClick((TWndOnClick)TabBtnClick);
	ShiftTabBtn->OnClick((TWndOnClick)ShiftTabBtnClick);
	EscapeBtn->OnClick((TWndOnClick)EscapeBtnClick);
}

void CBootKeys::DisableControls()
{
	KeysEdit->Disable();
	BackspaceBtn->Disable();
	TabBtn->Disable();
	ShiftTabBtn->Disable();
	EscapeBtn->Disable();
}

void CBootKeys::EnableControls()
{
	KeysEdit->Enable();
	BackspaceBtn->Enable();
	TabBtn->Enable();
	ShiftTabBtn->Enable();
	EscapeBtn->Enable();
}

void CBootKeys::InstallControls(CForm *Form)
{
	Form->AddControl(KeysGroupBevel);
	Form->AddControl(KeysGroupLabel);
	Form->AddControl(KeysEdit);
	Form->AddControl(KeyName);
	Form->AddControl(BackspaceBtn);
	Form->AddControl(TabBtn);
	Form->AddControl(ShiftTabBtn);
	Form->AddControl(EscapeBtn);
}

void CBootKeys::RealignText()
{
	bool Visible;

	Visible = KeysGroupLabel->IsVisible();
	KeysGroupLabel->SetVisible(false);
	KeysGroupLabel->SetCaption(StrKeys);
	KeysGroupLabel->SetVisible(Visible);
}


//--------------------
void CBootKeys::KeysEditKeyPress(CBootKeys &BootKeys, int &Key)
{
	switch (Key) {
		case KEY_ESCAPE:
			memset(BootKeys.BootItems.Get(BootKeys.Setup.BootItemIndex)->Keys,0,32);
			BootKeys.UpdateKeyDisplay();
			break;
		case KEY_BACKSPACE:
			BootKeys.RemoveKey();
			Key = KEY_END;
			break;
		default:
			BootKeys.AddKey(Key);
			Key = KEY_END;
			break;
	}
	BootKeys.Setup.SetIgnoreNextKey();
}

void CBootKeys::BackspaceBtnClick(CBootKeys &BootKeys)
{
	BootKeys.AddKey(0x0e08);
}

void CBootKeys::TabBtnClick(CBootKeys &BootKeys)
{
	BootKeys.AddKey(0x0f09);
}

void CBootKeys::ShiftTabBtnClick(CBootKeys &BootKeys)
{
	BootKeys.AddKey(0x0f00);
}

void CBootKeys::EscapeBtnClick(CBootKeys &BootKeys)
{
	BootKeys.AddKey(0x011b);
}


void CBootKeys::AddKey(int Key)
{
	unsigned short *KeyList = BootItems.Get(Setup.BootItemIndex)->Keys;
	int Index;

	for (Index = 0; KeyList[Index]; ++Index);
	if (Index < 15) {
		KeyList[Index] = Key;
		UpdateKeyDisplay();
	}
}

void CBootKeys::RemoveKey()
{
	unsigned short *KeyList = BootItems.Get(Setup.BootItemIndex)->Keys;
	int Index;

	for (Index = 0; KeyList[Index]; ++Index);
	if (Index) {
		KeyList[Index - 1] = 0;
		UpdateKeyDisplay();
	}
}

void CBootKeys::GetKeysString(int ItemIndex, CString &KeysString)
{
	unsigned short *KeyList = BootItems.Get(ItemIndex)->Keys;
	char ShortKeyName[32];
	int Index;

	for (Index = 0; KeyList[Index]; ++Index) {
		if (Index) {
			KeysString += ".";
		}
		CKeyboard::GetShortKeyName(CKeyboard::GetKeyCode(KeyList[Index]),ShortKeyName);
		KeysString += ShortKeyName;
	}
}

void CBootKeys::UpdateKeyDisplay()
{
	CString KeysStr;

	GetKeysString(Setup.BootItemIndex,KeysStr);
	KeysEdit->SetText(KeysStr);
	Setup.DisplayBootItem(Setup.BootItemIndex,true);
}


