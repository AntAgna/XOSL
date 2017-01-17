/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __setup__
#define __setup__

#include <newdefs.h>
#include <screen.h>
#include <ptab.h>
#include <items.h>
#include <gui.h>
#include <part.h>
#include <dialogs.h>

#include <bootkeys.h>
#include <general.h>
#include <password.h>
#include <xosldata.h>

class CLoader;

class CSetup {
	friend CBootKeys;
	friend CGeneral;
	friend CPassword;
public:
	CSetup(CLoader &LoaderToUse, CScreen &ScreenToUse, CPartList &PartListToUse, 
		   CBootItems &BootItemsToUse, CXOSLData &XoslDataToUse);
	~CSetup();
	
	void Show();
	void Hide();
	bool IsVisible();
	bool GotFocus();

	void CenterWindow();
	void RealignText();

	void DisplayBootItem(int Row, bool Refresh);

	bool GetIgnoreNextKey();

private:

	CLoader &Loader;
	CScreen &Screen;
	CPartList &PartList;
	CBootItems &BootItems;
	CXOSLData &XoslData;

	CPartDialog PartDialog;
	CDialogs Dialogs;

	CGeneral General;
	CBootKeys BootKeys;
	CPassword Password;

	bool Initialized;

	int BootItemIndex;

	bool IgnoreKey;
	void SetIgnoreNextKey();


	void Initialize();
	void CreateControls();
	void InitializeControls();
	void ConnectEventHandlers();
	void InstallControls();

	void DisableControls();
	void EnableControls();
	void DisplayBootItems(bool Refresh);
private:
	// controls

	CForm *Form;
	CButton *CloseBtn;
	CListBox *BootItemList;
	CScrollBar *ScrollBar;
	CButton *AddBtn;
	CButton *EditBtn;
	CButton *CloneBtn;
	CButton *RemoveBtn;
	CButton *MoveUpBtn;
	CButton *MoveDownBtn;
	CLabel *HotkeyLabel;
	CEdit *HotkeyEdit;
	CBevel *TabBevel;
	CTabControl *TabControl;
	CButton *RestoreBtn;
	CButton *SaveBtn;

	// General

	// Password


private:

private:
	// event handlers

	static void AddBootItem(CSetup &Setup);
	static void EditBootItem(CSetup &Setup);

	static void CloseBtnClick(CSetup &Setup);
	static void AddBtnClick(CSetup &Setup);
	static void EditBtnClick(CSetup &Setup);
	static void BootItemSelect(CSetup &Setup, int ItemIndex);
	static void BootItemSelectDefault(CSetup &Setup, int ItemIndex);

	static void SaveBtnClick(CSetup &Setup);
	static void RestoreBtnClick(CSetup &Setup);

	static void MoveUpBtnClick(CSetup &Setup);
	static void MoveDownBtnClick(CSetup &Setup);
	static void CloneBtnClick(CSetup &Setup);
	static void RemoveBtnClick(CSetup &Setup);
	static void HotkeyEditKeyPress(CSetup &Setup, int &Key);
};



#endif