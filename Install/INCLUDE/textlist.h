/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __textlist__
#define __textlist__

#include <textscr.h>
#include <textbox.h>

class CTextList {
	public:
		typedef void (*TListItemExecute)(void *_this, int ItemIndex);

		CTextList(int Left, int Top, int Width, int Height, CTextScreen &TextScreenToUse, CTextBox &TextBoxToUse);
		~CTextList();
		void AddItem(int Index, const char *Caption,
			const char *HelpText, int Enabled);
		void AddItem(int Index, const char *Caption, const char *HelpText,
			int Enabled, int OptionCount, const char **OptionList, int IsCombo);

		
		void Refresh();
		void Clear();
		void SetItemIndex(int ItemIndex);
		int GetOptionIndex(int Index);

		void ConnectEventHandler(TListItemExecute Handler, void *_this);

		void HandleKeyAction(int Key);
	private:
		enum TTextListType { 
			tltNone, 
			tltAction,
			tltCombo,
			tltList,
		};
		class CListItem {
		public:
			TTextListType Type;
			char *Caption;
			char *HelpText;
			int Enabled;
			int OptionCount;
			char **OptionList;
			int OptionWidth;
			int OptionIndex;
		};

		int FirstDraw;
		void DrawActionItem(int Index);
		void DrawOptionItem(int Index);
		void DrawListItem(int Index);
		void GetMaxOptionWidth();

		int Left, Top;
		int Width, Height;
		int ItemIndex;
		int MaxOptionWidth;
		CListItem *ListItems;

		TListItemExecute EventHandler;
		void *Event_this;

		CTextScreen &TextScreen;
		CTextBox &TextBox;

};

#endif