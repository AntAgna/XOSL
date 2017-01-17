#ifndef MiscH
#define MiscH

#include <newdefs.h>
#include <gui.h>

#include <xosldata.h>
#include <key.h>

class CPreference;
class CLoader;

class CMiscellaneous {
public:
	CMiscellaneous(CXOSLData &XoslDataToUse, CPreference &PreferenceToUse);
	~CMiscellaneous();

	void CreateControls();
	void InitializeControls(CTabControl *TabControl);
	void ConnectEventHandlers();
	void InitializeData();
	void InstallControls(CForm *Form);
	void RealignText();

private:
	CXOSLData &XoslData;
	CPreference &Preference;

	bool Initialized;

	void CreateKeyStr(CEdit *Edit, int Key);

private:
	// controls
	CBevel *KeysBevel;
	CLabel *KeysLabel;
	CLabel *CycleLabel;
	CEdit *CycleEdit;
	CLabel *RestartLabel;
	CEdit *RestartEdit;
	CLabel *RebootLabel;
	CEdit *RebootEdit;
	CLabel *ShutdownLabel;
	CEdit *ShutdownEdit;
	CCheckBox *PowerOff;

	CBevel *HandlingBevel;
	CLabel *HandlingLabel;
	
	CCheckBox *AutoSave;
	CCheckBox *AutoBootDialog;
	CCheckBox *SetInstBoot;
	CCheckBox *ActiveAllow;
	CCheckBox *AntiVirus;

	CBevel *PartManBevel;
	CLabel *PartManLabel;
	CLabel *PartKeyLabel;
	CEdit *PartKeyEdit;


private:
	// event handlers

	static void CycleEditKeyPress(CMiscellaneous &Miscellaneous, int &Key);
	static void RebootEditKeyPress(CMiscellaneous &Miscellaneous, int &Key);
	static void ShutdownEditKeyPress(CMiscellaneous &Miscellaneous, int &Key);
	static void RestartEditKeyPress(CMiscellaneous &Miscellaneous, int &Key);
	static void PowerOffChange(CMiscellaneous &Miscellaneous, int Checked);
	
	
    static void AutoSaveChange(CMiscellaneous &Miscellaneous, int Checked);
	static void AutoBootDialogChange(CMiscellaneous &Miscellaneous, int Checked);
	static void SetInstBootChange(CMiscellaneous &Miscellaneous, int Checked);
	static void ActiveAllowChange(CMiscellaneous &Miscellaneous, int Checked);
	static void AntiVirusChange(CMiscellaneous &Miscellaneous, int Checked);

	static void PartKeyEditKeyPress(CMiscellaneous &Miscellaneous, int &Key);


};

#endif
