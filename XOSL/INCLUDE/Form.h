/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __form__
#define __form__

#include <Control.h>

#define FORM_NORMAL 0
#define FORM_TRANSP 1

class CForm: public CControl {
	public:
		CForm(const char *Caption, int Style, int OnTop, int Left, int Top,
				int Width, int Height, int Visible);
		~CForm();
		void Show();
		void Hide();

		void SetMetrics(int Width, int Height);
		void SetPosition(int Left, int Top);
		void FixDamage(int Left, int Top, int Width, int Height);

		void BeforeFix(int Left, int Top, int Width, int Height);
		void AfterFix(int Left, int Top, int Width, int Height);


		int MouseDown(int MouseX, int MouseY);
		int MouseMove(int X, int Y);
		void MouseUp();

		void AddControl(CControl *Control);
		void RemoveControl(CControl *Control);
		void FocusControl(CControl *Control);

		void KeyPress(unsigned short Key);

		static int FrameMove;

	private:
		void Draw(long Left, long Top, long Width, long Height);
		int ButtonDown;
		CWindowList *Controls;
		int Style;
		int IgnoreAfter;
		int IsDrawn;
	private:
		void ShowFrame();
		void HideFrame();
		void FlushFrame();
		char *LeftFrame;
		char *RightFrame;
		char *TopFrame;
		char *BottomFrame;
		int DistX, DistY;
		int MoveLeft, MoveTop;
		int MoveRight, MoveBottom;

		int oMoveLeft, oMoveTop;
		int oMoveRight, oMoveBottom;
};



#endif