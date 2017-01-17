/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __prefer__
#define __prefer__

#include <newdefs.h>
#include <gui.h>
#include <setup.h>
#include <about.h>

#include <screen.h>
#include <graph.h>
#include <cstring.h>
#include <xosldata.h>
#include <dcx.h>
#include <key.h>
#include <mouse.h>
#include <dialogs.h>
#include <timer.h>

#include <graphset.h>
#include <colorset.h>
#include <mouseset.h>
#include <xoslpass.h>
#include <misc.h>
	 
class CLoader;
		
class CPreference {
public:
	CPreference(CLoader &LoaderToUse, CSetup &SetupToUse, CAbout &AboutToUse, CScreen &ScreenToUse, 
				CXOSLData &XoslDataToUse, CMouse &MouseToUse);
	~CPreference();

	void Show();
	void Hide();
	int IsVisible();
	bool GotFocus();

	void CenterWindow();
	void RealignText();

	bool GetIgnoreNextKey();
	void SetIgnoreNextKey();

private:
	CLoader &Loader;
	CScreen &Screen;
	CXOSLData &XoslData;

	CDialogs Dialogs;

	CGraphSettings GraphSettings;
	CColorSettings ColorSettings;
	CMouseSettings MouseSettings;
	CXoslPassword XoslPassword;
	CMiscellaneous Miscellaneous;

	bool Initialized;
	bool IgnoreKey;

	void Initialize();
	void CreateControls();
	void InitializeControls();
	void ConnectEventHandlers();
	void InstallControls();
	void InitializeData();

private:
	// Controls
	CForm *Form;
	CBevel *TabBevel;
	CTabControl *TabControl;
	CButton *ResetBtn;
	CButton *SaveBtn;
	CButton *CloseBtn;

private:
	// event handlers
	static void CloseBtnClick(CPreference &Preference);
	static void TabChange(CPreference &Preference, int ActiveTab);
	static void ResetBtnClick(CPreference &Preference);
	static void SaveBtnClick(CPreference &Preference);

};



#endif