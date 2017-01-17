/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __about__
#define __about__

#include <gui.h>
#include <screen.h>
#include <graph.h>
#include <xosldata.h>

class CLoader;

class CAbout {
	public:
		CAbout(CLoader &LoaderToUse, CScreen &ScreenToUse, CXOSLData &XoslDataToUse);
		~CAbout();

		void Show();
		void Hide();
		int IsVisible();
		int GotFocus();

		void CenterWindow();
		void RealignText();

	private:
		CLoader &Loader;
		CScreen &Screen;
		CXOSLData &XoslData;

		bool Initialized;

		void Initialize();
		void CreateControls();
		void InitializeControls();
		void ConnectEventHandlers();
		void InstallControls();


		CForm *Form;
		CImage *Image;
		CLabel *XoslTitle;
		CLabel *Copyright;
		CLabel *HttpXosl;
		CLabel *Warranty1;
		CLabel *Warranty2;
		CListBox *InfoListBox;
		CScrollBar *ScrollBar;
		CButton *CloseBtn;

		static void CloseBtnClick(CAbout &About);

};


#endif

