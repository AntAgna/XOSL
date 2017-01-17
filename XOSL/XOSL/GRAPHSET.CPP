
#include <GraphSet.h>
#include <Prefer.h>
#include <loader.h>
#include <graph.h>

extern void *AdditionalFont;
extern void *DefaultFont;
	
	
static const char *StrGrScrnArea = "Screen area";
static const char *StrGrPersonal = "Personal";
static const char *StrGrVisual = "Visual effects";


CGraphSettings::CGraphSettings(CXOSLData &XoslDataToUse, CPreference &PreferenceToUse, CSetup &SetupToUse, CLoader &LoaderToUse, 
							   CAbout &AboutToUse, CScreen &ScreenToUse, CMouse &MouseToUse):
	XoslData(XoslDataToUse), Preference(PreferenceToUse), Setup(SetupToUse), 
	Loader(LoaderToUse), About(AboutToUse), Screen(ScreenToUse),
	Mouse(MouseToUse)
{
	Initialized = false;
}

CGraphSettings::~CGraphSettings()
{

}

void CGraphSettings::CreateControls()
{
	ModeBevel = new CBevel(BEVEL_FRAME,true,20,32,161,145,true);
	DisplayMode = new CLabel(StrGrScrnArea,STYLE_REGULAR,false,17,28,25,true);
	ModeSlider = new CTrackBar(0,Graph->GetModeCount() - 1,0,36,54,129,true,this);
	ModeValue = new CLabel("640x480",STYLE_REGULAR,true,17,70,70,true);

	FrameBuffer = new CCheckBox("Linear frame buffer",false,36,102,true,this);

	TestBtn = new CButton("Test",31,141,67,25,true,this);
	ApplyBtn = new CButton("Apply",103,141,67,25,true,this);

	VisualBevel = new CBevel(BEVEL_FRAME,true,188,32,265,145,true);
	VisualLabel = new CLabel(StrGrVisual,STYLE_REGULAR,false,17,196,25,true);
	ShowContent = new CCheckBox("Show window contents while dragging",false,208,54,true,this);
	Font = new CCheckBox("Use additional font",false,208,78,true,this);
	InvCursor = new CCheckBox("Invert mouse cursor color",false,208,102,true,this);
	Wallpaper = new CCheckBox("Use wallpaper",false,208,126,true,this);
	DisplayIndex = new CCheckBox("Display boot item index",false,208,150,true,this);

	PersonalBevel = new CBevel(BEVEL_FRAME,true,20,191,433,74,true);
	PersonalLabel = new CLabel(StrGrPersonal,STYLE_REGULAR,false,17,28,184,true);
	FadeIn = new CCheckBox("Disable fade-in",false,44,207,true,this);
	FadeOut = new CCheckBox("Disable fade-out",false,44,231,true,this);
	ClearScreen = new CCheckBox("Clear screen before booting",false,208,207,true,this);
	Animate = new CCheckBox("Animate controls",false,208,231,true,this);

}

void CGraphSettings::InitializeControls(CTabControl *TabControl)
{
//	if (!Graph->LinearSupport())
//		FrameBuffer->Disable();
	TabControl->AddControl(0,ModeBevel);
	TabControl->AddControl(0,DisplayMode);
	TabControl->AddControl(0,ModeSlider);
	TabControl->AddControl(0,ModeValue);
	TabControl->AddControl(0,FrameBuffer);
	TabControl->AddControl(0,TestBtn);
	TabControl->AddControl(0,ApplyBtn);

	TabControl->AddControl(0,PersonalBevel);
	TabControl->AddControl(0,PersonalLabel);
	TabControl->AddControl(0,FadeIn);
	TabControl->AddControl(0,FadeOut);
	TabControl->AddControl(0,ClearScreen);
	TabControl->AddControl(0,Animate);

	TabControl->AddControl(0,VisualBevel);
	TabControl->AddControl(0,VisualLabel);
	TabControl->AddControl(0,ShowContent);
	TabControl->AddControl(0,Font);
	TabControl->AddControl(0,InvCursor);
	TabControl->AddControl(0,Wallpaper);
	TabControl->AddControl(0,DisplayIndex);

}

void CGraphSettings::ConnectEventHandlers()
{
	ModeSlider->OnChange((TTrackBarChange)ModeSliderChange);
	ShowContent->OnChange((TCheckBoxChange)ShowContentChange);
	InvCursor->OnChange((TCheckBoxChange)InvCursorChange);
	Font->OnChange((TCheckBoxChange)FontChange);
	Wallpaper->OnChange((TCheckBoxChange)WallpaperChange);
	DisplayIndex->OnChange((TCheckBoxChange)DisplayIndexChange);

	TestBtn->OnClick((TWndOnClick)ModeTest);
	ApplyBtn->OnClick((TWndOnClick)ChangeMode);
	FadeOut->OnChange((TCheckBoxChange)FadeOutChange);
	FadeIn->OnChange((TCheckBoxChange)FadeInChange);
	ClearScreen->OnChange((TCheckBoxChange)ClearScreenChange);
	Animate->OnChange((TCheckBoxChange)AnimateChange);
}

void CGraphSettings::InitializeData()
{
	TGraphData *GraphData;

	GraphData = XoslData.GetGraphics();

	ModeSlider->SetValue(Graph->GetModeIndex((TGraphMode)GraphData->VideoMode));
//	if (!Graph->LinearSupport())
//		FrameBuffer->Disable();
//	else
//		FrameBuffer->SetChecked(GraphData->LinearFB);
	ShowContent->SetChecked(!GraphData->FrameMove);
	Font->SetChecked(GraphData->Font9);
	InvCursor->SetChecked(GraphData->CursorInverted);
	Wallpaper->SetChecked(GraphData->UseWallpaper);
	DisplayIndex->SetChecked(GraphData->DisplayItemIndex);
	FadeOut->SetChecked(GraphData->DisableFadeOut);
	FadeIn->SetChecked(GraphData->DisableFadeIn);
	ClearScreen->SetChecked(GraphData->ClearScreen);
	Animate->SetChecked(!GraphData->NoAnimation);
	Initialized = true;
}

void CGraphSettings::InstallControls(CForm *Form)
{
	PrefForm = Form;
	Form->AddControl(ModeBevel);
	Form->AddControl(DisplayMode);
	Form->AddControl(ModeSlider);
	Form->AddControl(ModeValue);
	Form->AddControl(FrameBuffer);
	Form->AddControl(TestBtn);
	Form->AddControl(ApplyBtn);

	Form->AddControl(PersonalBevel);
	Form->AddControl(PersonalLabel);
	Form->AddControl(FadeIn);
	Form->AddControl(FadeOut);
	Form->AddControl(ClearScreen);
	Form->AddControl(Animate);

	Form->AddControl(VisualBevel);
	Form->AddControl(VisualLabel);
	Form->AddControl(ShowContent);
	Form->AddControl(Font);
	Form->AddControl(InvCursor);
	Form->AddControl(Wallpaper);
	Form->AddControl(DisplayIndex);

}

void CGraphSettings::RealignText()
{
	bool Visible;

	Visible = DisplayMode->IsVisible();
	
	DisplayMode->SetVisible(false);
	PersonalLabel->SetVisible(false);
	VisualLabel->SetVisible(false);

	DisplayMode->SetCaption(StrGrScrnArea);
	PersonalLabel->SetCaption(StrGrPersonal);
	VisualLabel->SetCaption(StrGrVisual);

	DisplayMode->SetVisible(Visible);
	PersonalLabel->SetVisible(Visible);
	VisualLabel->SetVisible(Visible);

	ModeSliderChange(*this,ModeSlider->GetValue());
}

TGraphMode CGraphSettings::GetSelectedDisplayMode()
{
	TGraphMode DisplayMode;

	DisplayMode = Graph->GetModeFromIndex(ModeSlider->GetValue());
	switch (DisplayMode) {
		case 1:
			return mode640x480;
		case 2:
			return mode800x600;
		case 3:
			return mode1024x768;
		case 4:
			return mode1280x1024;
		case 5:
			return mode1600x1200;
		default:
			return mode640x480;
	}
}

// --------------------- event handlers --------------------- 

void CGraphSettings::ModeSliderChange(CGraphSettings &GraphSettings, int Value)
{
	const char *ModeName;
	TGraphMode Mode;

	Mode = Graph->GetModeFromIndex(Value);
	ModeName = Graph->GetModeName(Mode);
	GraphSettings.ModeValue->SetCaption(ModeName);
	if (Graph->LFBSupported(Mode)) {
		GraphSettings.FrameBuffer->Enable();
		GraphSettings.FrameBuffer->SetChecked(GraphSettings.XoslData.GetGraphics()->LinearFB);
	}
	else {
		GraphSettings.FrameBuffer->SetChecked(false);
		GraphSettings.FrameBuffer->Disable();
	}
}

void CGraphSettings::ShowContentChange(CGraphSettings &GraphSettings, int Checked)
{
	if (GraphSettings.Initialized) {
		GraphSettings.PrefForm->FrameMove = !Checked;
		GraphSettings.XoslData.GetGraphics()->FrameMove = GraphSettings.PrefForm->FrameMove;
	}
}

void CGraphSettings::FontChange(CGraphSettings &GraphSettings, int Checked)
{
	if (GraphSettings.Initialized) {

		Graph->SetFont(Checked ? AdditionalFont : DefaultFont);

		GraphSettings.Preference.RealignText();
		GraphSettings.Setup.RealignText();
		GraphSettings.Loader.RealignText();
		GraphSettings.About.RealignText();
		GraphSettings.Screen.Refresh();

		GraphSettings.XoslData.GetGraphics()->Font9 = Checked;
	}
}



void CGraphSettings::InvCursorChange(CGraphSettings &GraphSettings, int Checked)
{
	int X, Y;

	if (GraphSettings.Initialized) {
		InvertCursor(Checked);
		Graph->GetCursorXY(X,Y);
		Graph->SetCursorXY(X,Y);
		GraphSettings.XoslData.GetGraphics()->CursorInverted = Checked;
	}
}

void CGraphSettings::ModeTest(CGraphSettings &GraphSettings)
{
	int Status;
	TGraphMode LastMode;
	TGraphMode NewMode;
	bool LastLFB, NewLFB;

	Graph->GetMode(LastMode,LastLFB);
	NewMode = GraphSettings.GetSelectedDisplayMode();
	NewLFB = GraphSettings.FrameBuffer->IsChecked();
	Graph->HideCursor();
	Status = GraphSettings.Screen.TestGraphicsMode(NewMode,NewLFB);

	if (Status != -1) {
		while (!CKeyboard::KeyStrokeAvail() &&  !GraphSettings.Mouse.MouseDown());
		if (CKeyboard::KeyStrokeAvail())
			CKeyboard::WaitKeyStroke();
	}


	Graph->Palette->SetSaturation(GraphSettings.XoslData.GetColor()->Saturation);
	GraphSettings.Screen.SetGraphicsMode(LastMode,LastLFB);
	Graph->ShowCursor();
}

void CGraphSettings::ChangeMode(CGraphSettings &GraphSettings)
{
	TGraphMode LastMode;
	TGraphMode NewMode;
	bool LastLFB, NewLFB;
	int Width, Height;

	Graph->GetMode(LastMode,LastLFB);

	NewMode = GraphSettings.GetSelectedDisplayMode();
	NewLFB = GraphSettings.FrameBuffer->IsChecked();
	if (GraphSettings.Screen.SetGraphicsMode(NewMode,NewLFB) == -1)
		GraphSettings.Screen.SetGraphicsMode(LastMode,LastLFB);
	else {
		Graph->GetModeMetrics(Width,Height);
		GraphSettings.Mouse.SetWindow(0,0,Width,Height);
		GraphSettings.XoslData.GetGraphics()->VideoMode = NewMode;
		GraphSettings.XoslData.GetGraphics()->LinearFB = NewLFB;
		/* Center all windows */
		GraphSettings.Loader.CenterWindow();
		GraphSettings.Setup.CenterWindow();
		GraphSettings.Preference.CenterWindow();
		GraphSettings.About.CenterWindow();
	}
}


void CGraphSettings::WallpaperChange(CGraphSettings &GraphSettings, int Checked)
{
	if (GraphSettings.Initialized) {
		GraphSettings.Screen.SetUseWallpaper(Checked);
//		GraphSettings.Screen.Refresh();
		GraphSettings.XoslData.GetGraphics()->UseWallpaper = Checked;
	}
}

void CGraphSettings::FadeOutChange(CGraphSettings &GraphSettings, int Checked)
{
	if (GraphSettings.Initialized) {
		GraphSettings.XoslData.GetGraphics()->DisableFadeOut = Checked;
	}
}

void CGraphSettings::FadeInChange(CGraphSettings &GraphSettings, int Checked)
{
	if (GraphSettings.Initialized) {
		GraphSettings.XoslData.GetGraphics()->DisableFadeIn = Checked;
	}
}


void CGraphSettings::DisplayIndexChange(CGraphSettings &GraphSettings, int Checked)
{
	if (GraphSettings.Initialized) {
		GraphSettings.XoslData.GetGraphics()->DisplayItemIndex = Checked;
		GraphSettings.Loader.CreateBootList();
	}
}

void CGraphSettings::ClearScreenChange(CGraphSettings &GraphSettings, int Checked)
{
	if (GraphSettings.Initialized) {
		GraphSettings.XoslData.GetGraphics()->ClearScreen = Checked;
	}
}

void CGraphSettings::AnimateChange(CGraphSettings &GraphSettings, int Checked)
{
	if (GraphSettings.Initialized) {
		GraphSettings.XoslData.GetGraphics()->NoAnimation = !Checked;
		CAnimatedControl::Animate = Checked;
	}
}


