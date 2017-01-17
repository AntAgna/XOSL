

#include <ColorSettings.h>
#include <timer.h>

static const char *ColorSchemes[16] = {
	"Brick","Desert","Eggplant","Lilac","Maple","Marine","Plum",
	"Pumpkin","Rainy day","Red, white & blue","Rose","Slate",
	"Spruce","Strom","Teal","XOSL standard"
};
	   
	   
static const char *StrClColorScheme = "Color scheme";
static const char *StrClAdjustment = "Color adjustment";
static const char *StrClFadeColor = "Fade-in color";
	 
	 
CColorSettings::CColorSettings(CXOSLData &XoslDataToUse):
	XoslData(XoslDataToUse)
{
	Initialized = false;
}

CColorSettings::~CColorSettings()
{

}


void CColorSettings::CreateControls()
{
	SchemeGroup = new CBevel(BEVEL_FRAME,true,24,32,177,217,false);
	ColorScheme = new CLabel(StrClColorScheme,STYLE_REGULAR,false,17,32,25,false);
	SchemeListBox = new CListBox(1,false,48,56,129,169,false,this);
	ScrollBar = new CScrollBar(0,0,0,false,158,57,167,false,SchemeListBox);
	AdjustGroup = new CBevel(BEVEL_FRAME,true,216,32,233,137,false);
	Adjustment = new CLabel(StrClAdjustment,STYLE_REGULAR,false,17,224,25,false);
	HueLabel = new CLabel("Hue:",STYLE_REGULAR,true,17,232,57,false);
	SatLabel = new CLabel("Saturation:",STYLE_REGULAR,true,17,232,89,false);
	LumLabel = new CLabel("Luminance:",STYLE_REGULAR,true,17,232,121,false);
	HueValue = new CLabel("0°",STYLE_REGULAR,true,17,408,72,39,false);
	SatValue = new CLabel("0%",STYLE_REGULAR,true,17,408,104,39,false);
	LumValue = new CLabel("0%",STYLE_REGULAR,true,17,408,136,39,false);
	HueSlider = new CTrackBar(-63,63,0,232,72,169,false,this);
	SatSlider = new CTrackBar(-63,63,0,232,104,169,false,this);
	LumSlider = new CTrackBar(-51,50,0,232,136,169,false,this);

	FadeGroup = new CBevel(BEVEL_FRAME,true,216,184,233,97,false);
	FadeColor = new CLabel(StrClFadeColor,STYLE_REGULAR,false,17,224,177,false);
	FadeIntLabel = new CLabel("Intensity:",STYLE_REGULAR,true,17,232,205,false);
	FadeIntSlider = new CTrackBar(0,100,100,232,220,169,false,this);
	FadeIntValue = new CLabel("100%",STYLE_REGULAR,true,17,408,220,39,false);
	FadeTestBtn = new CButton("Test",295,244,75,25,false,this);

}

void CColorSettings::InitializeControls(CTabControl *TabControl)
{
	int Index; 

	TabControl->AddControl(1,SchemeGroup);
	TabControl->AddControl(1,ColorScheme);
	TabControl->AddControl(1,SchemeListBox);
	TabControl->AddControl(1,ScrollBar);
	TabControl->AddControl(1,AdjustGroup);
	TabControl->AddControl(1,Adjustment);
	TabControl->AddControl(1,HueLabel);
	TabControl->AddControl(1,SatLabel);
	TabControl->AddControl(1,LumLabel);
	TabControl->AddControl(1,HueValue);
	TabControl->AddControl(1,SatValue);
	TabControl->AddControl(1,LumValue);
	TabControl->AddControl(1,HueSlider);
	TabControl->AddControl(1,SatSlider);
	TabControl->AddControl(1,LumSlider);
	TabControl->AddControl(1,FadeGroup);
	TabControl->AddControl(1,FadeColor);
	TabControl->AddControl(1,FadeIntSlider);
	TabControl->AddControl(1,FadeIntLabel);
	TabControl->AddControl(1,FadeIntValue);
	TabControl->AddControl(1,FadeTestBtn);

	SchemeListBox->SetColumn(0,108,"");
	for (Index = 0; Index < 16; ++Index)
		SchemeListBox->AddItem(SchemeListBox->AddRow(),0,ColorSchemes[Index]);
	SchemeListBox->SetScrollBar(ScrollBar);
}

void CColorSettings::ConnectEventHandlers()
{
	SchemeListBox->OnSelect((TListBoxSelect)ColorSchemeSelect);
	HueSlider->OnChange((TTrackBarChange)ChangeHue);
	SatSlider->OnChange((TTrackBarChange)ChangeSat);
	LumSlider->OnChange((TTrackBarChange)ChangeLum);
	
	FadeIntSlider->OnChange((TTrackBarChange)FadeIntChange);
	FadeTestBtn->OnClick((TWndOnClick)FadeTestClick);
}

void CColorSettings::InitializeData()
{
	TColorData *ColorData;

	ColorData = XoslData.GetColor();

	SchemeListBox->SetItemIndex(ColorData->ColorScheme);
	HueSlider->SetValue(ColorData->Hue);
	SatSlider->SetValue(ColorData->Saturation);
	LumSlider->SetValue(ColorData->Luminance);
	FadeIntSlider->SetValue(ColorData->FadeColorInt);
	Initialized = true;
}

void CColorSettings::InstallControls(CForm *Form)
{
	Form->AddControl(SchemeGroup);
	Form->AddControl(ColorScheme);
	Form->AddControl(SchemeListBox);
	Form->AddControl(ScrollBar);
	Form->AddControl(AdjustGroup);
	Form->AddControl(Adjustment);
	Form->AddControl(HueLabel);
	Form->AddControl(SatLabel);
	Form->AddControl(LumLabel);
	Form->AddControl(HueValue);
	Form->AddControl(SatValue);
	Form->AddControl(LumValue);
	Form->AddControl(HueSlider);
	Form->AddControl(SatSlider);
	Form->AddControl(LumSlider);
	Form->AddControl(FadeGroup);
	Form->AddControl(FadeColor);
	Form->AddControl(FadeIntSlider);
	Form->AddControl(FadeIntLabel);
	Form->AddControl(FadeIntValue);
	Form->AddControl(FadeTestBtn);
}

void CColorSettings::RealingText()
{
	bool Visible;

	Visible = ColorScheme->IsVisible();
	
    ColorScheme->SetVisible(false);
	Adjustment->SetVisible(false);
	FadeColor->SetVisible(false);
	
	ColorScheme->SetCaption(StrClColorScheme);
	Adjustment->SetCaption(StrClAdjustment);
	FadeColor->SetCaption(StrClFadeColor);
	
	ColorScheme->SetVisible(Visible);
	Adjustment->SetVisible(Visible);
	FadeColor->SetVisible(Visible);
}

// --------------------- event handlers --------------------- 


void CColorSettings::ColorSchemeSelect(CColorSettings &ColorSettings, int ItemIndex)
{
	if (ColorSettings.Initialized) {
		Graph->Palette->SetScheme(ItemIndex);
		Graph->Palette->Install();
		ColorSettings.XoslData.GetColor()->ColorScheme = ItemIndex;
	}
}

void CColorSettings::ChangeHue(CColorSettings &ColorSettings, int Hue)
{
	CString HueMsg;

	
	HueMsg = (long) ( (360 * Hue) / 63 );
	HueMsg += "\xb0";
	ColorSettings.HueValue->SetCaption(HueMsg);
	if (ColorSettings.Initialized) {
		Graph->Palette->SetHue(Hue);
		Graph->Palette->Install();
		ColorSettings.XoslData.GetColor()->Hue = Hue;
	}
}

void CColorSettings::ChangeSat(CColorSettings &ColorSettings, int Sat)
{
	CString SatMsg;

	SatMsg = (long) ( (100 * Sat) / 63 );
	SatMsg += "%";
	ColorSettings.SatValue->SetCaption(SatMsg);
	if (ColorSettings.Initialized) {
		Graph->Palette->SetSaturation(Sat);
		Graph->Palette->Install();
		ColorSettings.XoslData.GetColor()->Saturation = Sat;
	}
}

void CColorSettings::ChangeLum(CColorSettings &ColorSettings, int Lum)
{
	CString LumMsg;

	LumMsg = (long) ( (100 * Lum) / 64 );
	LumMsg += "%";
	ColorSettings.LumValue->SetCaption(LumMsg);
	
	if (ColorSettings.Initialized) {
		if (Lum < 0)
			Graph->Palette->SetColorLayer(0,0,0,-Lum);
		else
			Graph->Palette->SetColorLayer(63,63,63,Lum);
		Graph->Palette->Install();	
		ColorSettings.XoslData.GetColor()->Luminance = Lum;
	}
}


void CColorSettings::FadeIntChange(CColorSettings &ColorSettings, int Intensity)
{
	CString IntValue;

	IntValue = Intensity;
	IntValue += "%";
	ColorSettings.FadeIntValue->SetCaption(IntValue);
	if (ColorSettings.Initialized) {
		ColorSettings.XoslData.GetColor()->FadeColorInt = Intensity;
	}
}

void CColorSettings::FadeTestClick(CColorSettings &ColorSettings)
{
	unsigned long TimerTicks;

	Graph->Palette->LockFading();
	Graph->Palette->SetFadeOutColor(ColorSettings.XoslData.GetColor()->FadeColorInt);
	Graph->Palette->FadeOut();
	for (TimerTicks = GetTimerTicks() + 18; TimerTicks != GetTimerTicks(); );
	Graph->Palette->FadeIn();
}


