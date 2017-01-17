#ifndef GeneralH
#define GeneralH

#include <gui.h>
#include <items.h>
#include <screen.h>
#include <ptab.h>
#include <hide.h>

class CSetup;
		 
class CGeneral {
public:
	CGeneral(CSetup &SetupToUse, CBootItems &BootItemsToUse, CScreen &ScreenToUse, CPartList &PartListToUse);
	~CGeneral();

	void CreateControls();
	void InitializeControls(CTabControl *TabControl);
	void ConnectEventHandlers();
	void DisableControls();
	void EnableControls();
	void InstallControls(CForm *Form);
	void RealignText();

	void SelectBootItem(int ItemIndex);

	void ToggleDefault();

private:
	CSetup &Setup;
	CBootItems &BootItems;
	CPartList &PartList;

	CHidingDialog HidingDialog;

	void SetTimerSlider(int Timeout);

private:
	// Controls
	CBevel *StatusGroupBevel;
	CLabel *StatusGroupLabel;
	CCheckBox *Activate;
	CCheckBox *FATFix;
	CCheckBox *SwapDrives;
	CCheckBox *Disabled;
	CBevel *TimeoutGroupBevel;
	CLabel *TimeoutGroupLabel;
	CTrackBar *TimeoutSlider;
	CLabel *TimeoutLabel;
	CCheckBox *DefaultItem;
	CButton *HidingBtn;

private:
	// Event handlers
	static void DefaultItemClick(CGeneral &General, bool Checked);
	static void ActivateClick(CGeneral &General, bool Checked);
	static void FATFixClick(CGeneral &General, bool Checked);
	static void SwapDrivesClick(CGeneral &General, bool Checked);
	static void DisabledClick(CGeneral &General, bool Checked);

	static void HidingBtnClick(CGeneral &General);
	static void TimeoutSliderChange(CGeneral &General, int Value);

};

#endif
