/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __radiogrp__
#define __radiogrp__

#include <AniCntrl.h>

typedef void (*TRadioGroupSelect)(void *HandlerClass, int ItemIndex);

class CScrollBar;

class CRadioGroup: public CAnimatedControl {
	typedef struct ButtonNode_S {
		CString *Caption;
		struct ButtonNode_S *Prev;
		struct ButtonNode_S *Next;
	} TButtonNode;
	public:
		CRadioGroup(int Left, int Top, int Width, int Height, int Visible, void *HandlerClass);
		~CRadioGroup();
		void AddButton(const char *Caption);
		void DeleteButton(int ButtonIndex);
		void Clear();
		void SetChecked(int Index);
		int GetChecked();
		int GetButtonCount();

		void SetHighlight(int Status);

		void SetDrawStart(int Index);

		void KeyPress(unsigned short Key);
		int MouseDown(int MouseX, int MouseY);
		int MouseMove(int X, int Y);

		void SetScrollBar(CScrollBar *ScrollBar);
		void OnSelect(TRadioGroupSelect RadioGroupSelect);

	private:
		void Draw(long, long, long, long);
		void DrawButton(int ButtonTop, int ButtonHeight, int GotFocus,
							 int Checked, const char *Caption);
		int GetFocusWidth(const char *Caption);
		void UpdateScrollBar();

      static void ScrollBarChange(CRadioGroup *RadioGroup, int Value);
		CScrollBar *ScrollBar;
		TRadioGroupSelect RadioGroupSelect;
		int ItemIndex;
		int DrawStart;
		int DrawCount;

		int Count;
		TButtonNode ButtonList;
		TButtonNode *LastButton;

		int HighlightColor;

		int AniButtonIndex;

};

#endif