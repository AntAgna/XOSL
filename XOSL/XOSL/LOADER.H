/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __loader__
#define __loader__

#include <gui.h>
#include <graph.h>
#include <items.h>
#include <xosldata.h>
#include <encpwd.h>
#include <dialogs.h>
#include <key.h>
#include <ctype.h>

#include <about.h>
#include <setup.h>
#include <prefer.h>

#include <cstring.h>

class CLoader {
	public:
		CLoader(CScreen &ScreenToUse, CBootItems &BootItemsToUse, CXOSLData &XoslDataToUse,
				CPartList &PartListToUse, CMouse &MouseToUse);
		~CLoader();

		void Show(bool AutoBoot = false);
		void Hide();

		unsigned short WaitTimeout();

		int IsVisible();
		void Focus();
		int GotFocus();
		int CanBoot();
		int GetBootItemIndex();
		void SelectDefault();

		void CenterWindow();

		void ShowBootError(const char *ErrMsg);
		void AbortInstantBoot();
		void SetTimeoutLabelCaption(const char *BootStr);
		void SetTimeoutProgress(int Progress);

		void CreateBootList();

		void RealignText();

		void CycleWindows();
		bool GetIgnoreNextKey();

		void HandleAutoSave();
	private:
		CSetup Setup;
		CAbout About;
		CPreference Preference;

		CScreen &Screen;
		CBootItems &BootItems;
		CXOSLData &XoslData;

		CDialogs Dialogs;

		bool Initialized;
		void Initialize(bool AutoBoot);
		bool EnterSetup;

		int InListBootIndex[24];

		void UpdateMaxItemWidth(int &MaxItemWidth, const char *ItemName);
		void ResizeLoader(int MaxItemWidth);


		void CreateControls(bool AutoBoot);
		void ConnectEventHandlers();

		void CreateBootString(unsigned long TicksPassed);
		unsigned long Timeout;
		CString BootString;
	private:

		

		static void BootBtnClick(CLoader &Loader);
		static void AboutBtnClick(CLoader &Loader);
		static void SetupBtnClick(CLoader &Loader);
		static void PreferBtnClick(CLoader &Loader);
		static void PwdProc(CLoader &Loader, const char *Password);
		static void BootPwdProc(CLoader &Loader, const char *Password);
		static void ButtonKeyPress(CLoader &Loader, unsigned short &Key);
		static void BootListPress(CLoader &Loader, int &Key);
		static void BootItemsSelect(CLoader &Loader, int ItemIndex);
		static void BootItemsKeyPress(CLoader &Loader, unsigned short &Key);


	private:

		CForm *Form;
		CImage *Image;
		CLabel *Label1;
		CLabel *Label2;
		CLabel *Label3;
		CLabel *Label4;
		CLabel *Label5;
		CLabel *BootLabel;
		CBevel *Bevel;
		CScrollBar *ScrollBar;
		CButton *BootBtn;
		CButton *SetupBtn;
		CButton *PreferBtn;
		CButton *AboutBtn;
		CProgressBar *ProgressBar;
		CRadioGroup *BootItemsList;

		int BootItemIndex;
		int StartIndex;
		int BootBtnPressed;



	private:
		void InitializeControls();
		void InstallControls();
		void InitInstantBoot();

};



#endif
