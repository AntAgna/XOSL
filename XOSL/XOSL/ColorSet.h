#ifndef ColorSetH
#define ColorSetH

#include <newdefs.h>
#include <gui.h>
 
#include <xosldata.h>
		
class CColorSettings {
public:
	CColorSettings(CXOSLData &XoslDataToUse);
	~CColorSettings();

	void CreateControls();
	void InitializeControls(CTabControl *TabControl);
	void ConnectEventHandlers();
	void InitializeData();
	void InstallControls(CForm *Form);
	void RealingText();

private:
	CXOSLData &XoslData;

	bool Initialized;

private:
	// controls
	CBevel *SchemeGroup;
	CLabel *ColorScheme;
	CListBox *SchemeListBox;
	CScrollBar *ScrollBar;
	CBevel *AdjustGroup;
	CLabel *Adjustment;
	CLabel *HueLabel;
	CLabel *SatLabel;
	CLabel *LumLabel;
	CLabel *HueValue;
	CLabel *SatValue;
	CLabel *LumValue;
	CTrackBar *HueSlider;
	CTrackBar *SatSlider;
	CTrackBar *LumSlider;

	CBevel *FadeGroup;
	CLabel *FadeColor;
	CLabel *FadeIntLabel;
	CTrackBar *FadeIntSlider;
	CLabel *FadeIntValue;
	CButton *FadeTestBtn;

private:
	// event handler
	static void ColorSchemeSelect(CColorSettings &ColorSettings, int ItemIndex);
	static void ChangeHue(CColorSettings &ColorSettings, int Hue);
	static void ChangeSat(CColorSettings &ColorSettings, int Sat);
	static void ChangeLum(CColorSettings &ColorSettings, int Lum);
	static void FadeIntChange(CColorSettings &ColorSettings, int Intensity);
	static void FadeTestClick(CColorSettings &ColorSettings);

};

#endif
