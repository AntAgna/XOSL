#ifndef PasswordH
#define  PasswordH

#include <gui.h>
#include <items.h>
#include <dialogs.h>

class CSetup;
	 
class CPassword {
public:
	CPassword(CSetup &SetupToUse, CBootItems &BootItemsToUse, CDialogs &DialogsToUse);
	~CPassword();

	void CreateControls();
	void InitializeControls(CTabControl *TabControl);
	void ConnectEventHandlers();
	void DisableControls();
	void EnableControls();
	void InstallControls(CForm *Form);
	void RealignText();
private:
	CSetup &Setup;
	CBootItems &BootItems;
	CDialogs &Dialogs;

private:
	// controls
	CBevel *PasswordGroupBevel;
	CLabel *PasswordGroupLabel;
	CLabel *NewPassLabel;
	CEdit *NewPassEdit;
	CLabel *ConfirmPassLabel;
	CEdit *ConfirmPassEdit;
	CButton *ApplyPassBtn;
	CButton *ClearPassBtn;


private:
	// event handlers

	static void ApplyPassBtnClick(CPassword &Password);
	static void ClearPassBtnClick(CPassword &Password);

};

#endif
