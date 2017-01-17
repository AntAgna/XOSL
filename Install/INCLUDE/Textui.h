/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __textui__
#define __textui__

#include <defs.h>
#include <textwnd.h>
#include <textbox.h>
#include <textlist.h>
#include <textpop.h>


class CTextUI {
	public:
		void ConnectEventHandler(CTextList::TListItemExecute Handler, void *_this);
		CTextUI(CTextScreen &TextScreenToUse);
		~CTextUI();
		void OutputStr(const char *format, ...);
		void AddMenuItem(int Index, const char *Caption,
			const char *HelpText, int Enabled);
		void AddMenuItem(int Index, const char *Caption, const char *HelpText,
			int Enabled, int OptionCount, const char **OptionList, int IsCombo);
		void RefreshMenu();
		void ClearMenu();
		void SetItemIndex(int ItemIndex);
		void HandleKey(int Key);
		int GetOptionIndex(int Index);

		void SetMenuTitle(const char *Title);

		void ShowPopup(int Left, int Top, int Width, int Height, const char *Text);

	private:
		void DrawText();
		void DrawBorders();

		CTextScreen &TextScreen;
		CTextWindow Output;
		CTextBox HelpText;
		CTextList MenuItems;

		const char *MenuTitle;
		int MenuTitleLength;

		CTextPopup *Popup;
};


#endif