
#include <MouseSet.h>
#include <key.h>

static const char *StrMoTypeLabel = "Mouse type";
static const char *StrMoSpeedLabel = "Mouse speed";
static const char *StrMoSamplingLabel = "PS/2 sampling rate";
static const char *StrMoKbdLayout = "Keyboard layout";

void printf(const char *,...);
	 
CMouseSettings::CMouseSettings(CXOSLData &XoslDataToUse, CDialogs &DialogsToUse, CMouse &MouseToUse):
	XoslData(XoslDataToUse), Dialogs(DialogsToUse), Mouse(MouseToUse)
{
	Initialized = false;
}

CMouseSettings::~CMouseSettings()
{

}

void CMouseSettings::CreateControls()
{
	TypeGroup = new CBevel(BEVEL_FRAME,true,24,32,193,110,false);
	TypeLabel = new CLabel(StrMoTypeLabel,STYLE_REGULAR,false,17,32,25,false);
	MouseBox = new CComboBox(8,44,56,153,false,this);
	ApplyBtn = new CButton("Apply",83,99,75,25,false,this);

	SpeedGroup = new CBevel(BEVEL_FRAME,true,24,160,193,41,false);
	SpeedLabel = new CLabel(StrMoSpeedLabel,STYLE_REGULAR,false,17,32,153,false);
	SpeedSlider = new CTrackBar(0,8,7,44,175,153,false,this);

	SamplingGroup = new CBevel(BEVEL_FRAME,true,24,216,193,41,false);
	SamplingLabel = new CLabel(StrMoSamplingLabel,STYLE_REGULAR,false,17,32,209,false);
	SamplingSlider = new CTrackBar(0,6,0,44,231,153,false,this);

	KeyboardGroup = new CBevel(BEVEL_FRAME,true,232,32,209,86,false);
	KeyboardLabel = new CLabel(StrMoKbdLayout,STYLE_REGULAR,false,17,240,25,false);
	LayoutList = new CComboBox(8,256,56,161,false,this);
	EnhKeyboard = new CCheckBox("Enhanced keyboard",false,264,88,false,this);
}

void CMouseSettings::InitializeControls(CTabControl *TabControl)
{
	TabControl->AddControl(2,TypeGroup);
	TabControl->AddControl(2,TypeLabel);
	TabControl->AddControl(2,MouseBox);
	TabControl->AddControl(2,SpeedGroup);
	TabControl->AddControl(2,SpeedLabel);
	TabControl->AddControl(2,SpeedSlider);
	TabControl->AddControl(2,SamplingGroup);
	TabControl->AddControl(2,SamplingLabel);
	TabControl->AddControl(2,SamplingSlider);
	TabControl->AddControl(2,ApplyBtn);
	TabControl->AddControl(2,KeyboardGroup);
	TabControl->AddControl(2,KeyboardLabel);
	TabControl->AddControl(2,LayoutList);
	TabControl->AddControl(2,EnhKeyboard);


	MouseBox->AddItem("Serial COM1");
	MouseBox->AddItem("Serial COM2");
	MouseBox->AddItem("Serial COM3");
	MouseBox->AddItem("Serial COM4");
	MouseBox->AddItem("PS/2");
	MouseBox->AddItem("None");

	LayoutList->AddItem("French");
	LayoutList->AddItem("United States");

}

void CMouseSettings::ConnectEventHandlers()
{
	SpeedSlider->OnChange((TTrackBarChange)SpeedChange);
	SamplingSlider->OnChange((TTrackBarChange)SamplingChange);
	ApplyBtn->OnClick((TWndOnClick)ApplyClick);
	LayoutList->OnChange((TListBoxSelect)KbdLayoutSelect);
	EnhKeyboard->OnChange((TCheckBoxChange)KbdEnhChanges);
}

void CMouseSettings::InitializeData()
{
	TMouseData *MouseData;

	MouseData = XoslData.GetMouse();

	MouseBox->SetItemIndex(MouseData->MouseType);
	SpeedSlider->SetValue(MouseData->MouseSpeed);
	SamplingSlider->SetValue(MouseData->PS2Sampling);
	SpeedChange(*this,MouseData->MouseSpeed);
	SamplingChange(*this,MouseData->PS2Sampling);

	InitKbdLayout(MouseData->KbdLayout);

	EnhKeyboard->SetChecked(MouseData->EnhancedSupport);

	Initialized = true;
}

void CMouseSettings::InitKbdLayout(int Layout)
{
	int Index;

	switch (Layout) {
		case CKeyboard::enumLayoutFrench:
			Index = 0;
			break;
		case CKeyboard::enumLayoutUS:
			Index = 1;
			break;
		default:
			Index = 1;
			break;
	}

	if (LayoutList->GetItemIndex() != Index) {
		LayoutList->SetItemIndex(Index);
	}
	else {
		KbdLayoutSelect(*this,Index);
	}


}

void CMouseSettings::InstallControls(CForm *Form)
{
	PrefForm = Form;
	Form->AddControl(TypeGroup);
	Form->AddControl(TypeLabel);
	Form->AddControl(MouseBox);
	Form->AddControl(ApplyBtn);
	Form->AddControl(SpeedGroup);
	Form->AddControl(SpeedLabel);
	Form->AddControl(SpeedSlider);
	Form->AddControl(SamplingGroup);
	Form->AddControl(SamplingLabel);
	Form->AddControl(SamplingSlider);
	Form->AddControl(KeyboardGroup);
	Form->AddControl(KeyboardLabel);
	Form->AddControl(LayoutList);
	Form->AddControl(EnhKeyboard);
}

void CMouseSettings::RealignText()
{
	bool Visible;

	Visible = TypeLabel->IsVisible();
	
	TypeLabel->SetVisible(false);
	SpeedLabel->SetVisible(false);
	SamplingLabel->SetVisible(false);
	KeyboardLabel->SetVisible(false);
	
	TypeLabel->SetCaption(StrMoTypeLabel);
	SpeedLabel->SetCaption(StrMoSpeedLabel);
	SamplingLabel->SetCaption(StrMoSamplingLabel);
	KeyboardLabel->SetCaption(StrMoKbdLayout);
	
	TypeLabel->SetVisible(Visible);
	SpeedLabel->SetVisible(Visible);
	SamplingLabel->SetVisible(Visible);
}

// --------------------- event handlers --------------------- 

void CMouseSettings::SpeedChange(CMouseSettings &MouseSettings, int Value)
{
	if (MouseSettings.Initialized) {
		MouseSettings.Mouse.SetSpeed(Value);
		MouseSettings.XoslData.GetMouse()->MouseSpeed = Value;
	}
}

void CMouseSettings::SamplingChange(CMouseSettings &MouseSettings, int Value)
{
	if (MouseSettings.Initialized) {
		MouseSettings.Mouse.SetSampling(Value);
		MouseSettings.XoslData.GetMouse()->PS2Sampling = Value;
	}
}

void CMouseSettings::ApplyClick(CMouseSettings &MouseSettings)
{
	int LastPort;
	int MousePort;
	int Checked;
	CString Message;
	static const char *MsgBoxTitle = "Mouse";
	static const int PortList[] = {
		MOUSE_COM1,MOUSE_COM2,MOUSE_COM3,MOUSE_COM4,MOUSE_PS2
	};

	Graph->HideCursor();
	LastPort = MouseSettings.Mouse.GetMouse();
	Checked = MouseSettings.MouseBox->GetItemIndex();
	if (Checked == 5) {
		MouseSettings.Mouse.SetMouse(-1);
		MouseSettings.XoslData.GetMouse()->MouseType = -1;
	}
	else {
		MousePort = PortList[Checked];
		Message = "Initializing ";
		Message += MouseSettings.Mouse.MousePortName(MousePort);
		Message += "...";
		MouseSettings.Dialogs.ShowMessageDialog(MouseSettings.PrefForm,MsgBoxTitle,Message);
		if (MouseSettings.Mouse.SetMouse(MousePort) == -1) {
			MouseSettings.Mouse.SetMouse(LastPort);
			Message += "failed!";
			MouseSettings.Dialogs.ShowMessageDialog(MouseSettings.PrefForm,MsgBoxTitle,Message);
			if (LastPort != -1)
				Graph->ShowCursor();
		}
		else {
			MouseSettings.XoslData.GetMouse()->MouseType = MousePort;
			Message += "done";
			MouseSettings.Dialogs.ShowMessageDialog(MouseSettings.PrefForm,MsgBoxTitle,Message);
			Graph->ShowCursor();
		}
		SpeedChange(MouseSettings,MouseSettings.SpeedSlider->GetValue());
		SamplingChange(MouseSettings,MouseSettings.SamplingSlider->GetValue());
	}
}

void CMouseSettings::KbdLayoutSelect(CMouseSettings &MouseSettings, int ItemIndex)
{
	CKeyboard::TKeyLayout Layout;

	switch (ItemIndex) {
		case 0: // French
			Layout = CKeyboard::enumLayoutFrench;
			break;
		case 1: // US
			Layout = CKeyboard::enumLayoutUS;
			break;
		default:
			return;
	}

	if (MouseSettings.Initialized) {
		CKeyboard::SelectLayout(Layout);
		MouseSettings.XoslData.GetMouse()->KbdLayout = Layout;
	}
}

void CMouseSettings::KbdEnhChanges(CMouseSettings &MouseSettings, bool Checked)
{
	if (MouseSettings.Initialized) {
		MouseSettings.XoslData.GetMouse()->EnhancedSupport = Checked;
		CKeyboard::SetEnhancedSupport(Checked);
	}
}
