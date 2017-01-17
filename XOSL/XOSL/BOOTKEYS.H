#ifndef BootKeysH
#define BootKeysH

#include <gui.h>
#include <key.h>
#include <items.h>
#include <cstring.h>

class CSetup;

class CBootKeys {
public:
	CBootKeys(CSetup &SetupToUse, CBootItems &BootItemsToUse);
	~CBootKeys();

	void CreateControls();
	void InitializeControls(CTabControl *TabControl);
	void ConnectEventHandlers();
	void DisableControls();
	void EnableControls();
	void InstallControls(CForm *Form);
	void RealignText();

	void GetKeysString(int ItemIndex, CString &KeysString);
	void UpdateKeyDisplay();
private:
	CSetup &Setup;
	CBootItems &BootItems;

	void AddKey(int Key);
	void RemoveKey();


private:
	// Controls
	CBevel *KeysGroupBevel;
	CLabel *KeysGroupLabel;
	CEdit *KeysEdit;
	CLabel *KeyName;
	CButton *BackspaceBtn;
	CButton *TabBtn;
	CButton *ShiftTabBtn;
	CButton *EscapeBtn;

private:

	static void KeysEditKeyPress(CBootKeys &BootKeys, int &Key);
	static void BackspaceBtnClick(CBootKeys &BootKeys);
	static void TabBtnClick(CBootKeys &BootKeys);
	static void ShiftTabBtnClick(CBootKeys &BootKeys);
	static void EscapeBtnClick(CBootKeys &BootKeys);
};


#endif
