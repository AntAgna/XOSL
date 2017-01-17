#ifndef GraphSetH
#define GraphSetH

#include <newdefs.h>
#include <gui.h>

#include <xosldata.h>
#include <setup.h>
#include <about.h>

#include <key.h>
#include <mouse.h>

class CPreference;
class CLoader;

class CGraphSettings {
public:
	CGraphSettings(CXOSLData &XoslDataToUse, CPreference &PreferenceToUse, CSetup &SetupToUse, CLoader &LoaderToUse, 
				   CAbout &AboutToUse, CScreen &ScreenToUse, CMouse &MouseToUse);
	~CGraphSettings();

	void CreateControls();
	void InitializeControls(CTabControl *TabControl);
	void ConnectEventHandlers();
	void InitializeData();
	void InstallControls(CForm *Form);
	void RealignText();

private:
	CXOSLData &XoslData;
	CPreference &Preference;
	CSetup &Setup;
	CLoader &Loader;
	CAbout &About;
	CScreen &Screen;
	CMouse &Mouse;

	bool Initialized;
	CForm *PrefForm;

	TGraphMode GetSelectedDisplayMode();

private:
	// controls
	CBevel *ModeBevel;
	CLabel *DisplayMode;
	CTrackBar *ModeSlider;
	CLabel *ModeValue;
	CCheckBox *FrameBuffer;
	CButton *TestBtn;
	CButton *ApplyBtn;

	CBevel *PersonalBevel;
	CLabel *PersonalLabel;
	CCheckBox *FadeIn;
	CCheckBox *FadeOut;
	CCheckBox *ClearScreen;
	CCheckBox *Animate;

	CBevel *VisualBevel;
	CLabel *VisualLabel;
	CCheckBox *ShowContent;
	CCheckBox *Font;
	CCheckBox *InvCursor;
	CCheckBox *Wallpaper;
	CCheckBox *DisplayIndex;


private:
	// event handlers
	static void ModeSliderChange(CGraphSettings &GraphSettings, int Value);
	static void ShowContentChange(CGraphSettings &GraphSettings, int Checked);
	static void FontChange(CGraphSettings &GraphSettings, int Checked);
	static void InvCursorChange(CGraphSettings &GraphSettings, int Checked);
	static void ModeTest(CGraphSettings &GraphSettings);
	static void ChangeMode(CGraphSettings &GraphSettings);

	static void WallpaperChange(CGraphSettings &GraphSettings, int Checked);
	static void FadeOutChange(CGraphSettings &GraphSettings, int Checked);
	static void FadeInChange(CGraphSettings &GraphSettings, int Checked);

	static void DisplayIndexChange(CGraphSettings &GraphSettings, int Checked);
	static void ClearScreenChange(CGraphSettings &GraphSettings, int Checked);
	static void AnimateChange(CGraphSettings &GraphSettings, int Checked);

};

#endif
