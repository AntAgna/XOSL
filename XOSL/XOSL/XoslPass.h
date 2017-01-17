#ifndef XoslPassH
#define XoslPassH

#include <newdefs.h>
#include <gui.h>

#include <xosldata.h>
#include <dialogs.h>

class CXoslPassword {
public:
	CXoslPassword(CXOSLData &XoslDataToUse, CDialogs &DialogsToUse);
	~CXoslPassword();

	void CreateControls();
	void InitializeControls(CTabControl *TabControl);
	void ConnectEventHandlers();

	void InstallControls(CForm *Form);
	void RealignText();


private:
	CXOSLData &XoslData;
	CDialogs &Dialogs;

	CForm *PrefForm;

	void SetMBRPassword(unsigned long Code);

private:
	// controls
	CBevel *Group;
	CLabel *Label;
	CLabel *OldLabel;
	CLabel *NewLabel;
	CLabel *ReLabel;
	CEdit *OldEdit;
	CEdit *NewEdit;
	CEdit *ReEdit;
	CButton *ApplyBtn;

private:
	// event handlers
	static void ApplyBtnClick(CXoslPassword &XoslPassword);

};

#endif
