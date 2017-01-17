#ifndef MouseSetH
#define MouseSetH

#include <newdefs.h>
#include <gui.h>
#include <dialogs.h>

#include <xosldata.h>

class CMouseSettings {
public:
	CMouseSettings(CXOSLData &XoslDataToUse, CDialogs &DialogsToUse, CMouse &MouseToUse);
	~CMouseSettings();

	void CreateControls();
	void InitializeControls(CTabControl *TabControl);
	void ConnectEventHandlers();
	void InitializeData();
	void InstallControls(CForm *Form);
	void RealignText();

private:
	CXOSLData &XoslData;
	CDialogs &Dialogs;
	CMouse &Mouse;

	CForm *PrefForm;

	bool Initialized;

	void InitKbdLayout(int Layout);

private:
	// controls
	CBevel *TypeGroup;
	CLabel *TypeLabel;
	CComboBox *MouseBox;

	CBevel *SpeedGroup;
	CLabel *SpeedLabel;
	CTrackBar *SpeedSlider;
	CBevel *SamplingGroup;
	CLabel *SamplingLabel;
	CTrackBar *SamplingSlider;
	CButton *ApplyBtn;

	CBevel *KeyboardGroup;
	CLabel *KeyboardLabel;
	CComboBox *LayoutList;
	CCheckBox *EnhKeyboard;


private:
	// event handlers
	static void SpeedChange(CMouseSettings &MouseSettings, int Value);
	static void SamplingChange(CMouseSettings &MouseSettings, int Value);
	static void ApplyClick(CMouseSettings &MouseSettings);
	static void KbdLayoutSelect(CMouseSettings &MouseSettings, int ItemIndex);
	static void KbdEnhChanges(CMouseSettings &MouseSettings, bool Checked);

};

#endif
