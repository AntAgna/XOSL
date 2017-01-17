/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __listbox__
#define __listbox__

#include <AniCntrl.h>
#include <strlist.h>

typedef struct SStrListNode{
	CStringList *StringList;
	int Color;
	int FontStyle;
	struct SStrListNode *Next;
	struct SStrListNode *Prev;
} TStrListNode;

typedef void (*TListBoxSelect)(void *HandlerClass, int ItemIndex);

class CScrollBar;

class CListBox: public CAnimatedControl {
	public:
		CListBox(int Columns, int ShowHeader, int Left, int Top, int Width, int Height, int Visible, void *HandlerClass);
		~CListBox();

		void SetColumn(int Index, int Width, const char *HeaderName);
		void SetShowHeader(int ShowHeader);

		int AddRow();
		void InsertRow(int Index);
		void DeleteRow(int Index);
		void AddItem(int Row, int Column, const char *String);
		CString GetItem(int Row, int Column);
		void Clear();

		void SetItemIndex(int Index);
		int GetItemIndex();
		int GetCount();

		void SetDefault(int Index);
		void SetRowStyle(int Row, int Color, int FontStyle);
		void SetBackgroundColor(int Color);

		void SetDrawStart(int Index);
		int GetDrawCount();

		void KeyPress(unsigned short Key);
		int MouseDown(int MouseX, int MouseY);

		void SetScrollBar(CScrollBar *ScrollBar);
		void UpdateScrollBar();

		void OnSelect(TListBoxSelect ListBoxSelect);
		void OnDoubleClick(TListBoxSelect ListBoxDoubleClick);

      void FontChanged();
	private:
		TStrListNode *GetEntry(int Index);

		void Draw(long Left, long Top, long Width, long Height);
		void DrawBody();
		void DrawHeader();
		void DrawItems();

		CStringList *ColumnHeaders;
		TStrListNode *Items;
		int *ColumnWidth;
		int Count;
		int ItemIndex;
		int Columns;
		int ShowHeader;

		int DrawStart;
		int DrawCount;

		CScrollBar *ScrollBar;
		static void ScrollBarChange(CListBox *ListBox, int Value);

		TListBoxSelect ListBoxSelect;
		TListBoxSelect ListBoxDoubleClick;

		int BackgroundColor;
		int DefaultIndex;

		unsigned long TickLastClick;
};

#endif