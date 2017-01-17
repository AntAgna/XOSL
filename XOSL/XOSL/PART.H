/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef PartH
#define PartH

#include <newdefs.h>
#include <gui.h>
#include <screen.h>
#include <ptab.h>

typedef void (*TPartDialogOnApply)(void *HandlerClass);

class CPartDialog {
public:
	CPartDialog(CScreen &ScreenToUse, CPartList &PartListToUse);
	~CPartDialog();

	void ShowModal(CForm *Owner, const char *Caption, const char *DefaultName, int DefaultIndex);

	void SetOnApply(void *HandlerClass, TPartDialogOnApply Handler);
	void GetSelection(int &ItemIndex, char *ItemName);
private:
	CPartList &PartList;
	CScreen &Screen;

	TPartDialogOnApply OnApply;
	void *HandlerClass;
	
	int Initialized;

	CForm *Owner;

	bool AutoName;

private:	// controls
	CForm *Form;

	CButton *ApplyBtn;
	CButton *CancelBtn;
	CListBox *PartListBox;
	CScrollBar *ScrollBar;
	CLabel *Label;
	CEdit *NameEdit;


private:

	void CreateControls();
	void InitializeControls();
	void InstallControls();

	void InitializeDialog();

	void PositionDialog();

	void AddDisk(int Row, int Drive);
	void AddType(int Row, int Type);
	void AddSystem(int Row, const char *FSName);
	void AddSize(int Row, long Sectors);
	void AddVolume(int Row, const char *VolumeLabel);

	void CloseDialog();

	static void ApplyBtnClick(CPartDialog &PartDialog);
	static void CancelBtnClick(CPartDialog &PartDialog);
	static void PartListBoxSelect(CPartDialog &PartDialog, int ItemIndex);
	static void NameEditKeyPress(CPartDialog &PartDialog, unsigned short &Key);
	static void PartListBoxKeyPress(CPartDialog &PartDialog, int &Key);
};



#endif