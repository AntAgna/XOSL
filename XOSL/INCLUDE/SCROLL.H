/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __scroll__
#define __scroll__

#include <AniCntrl.h>

typedef void (*TScrollChange)(void *HandlerClass, int Value);


class CScrollBar: public CAnimatedControl {
	public:
		CScrollBar(int Min, int Max, int Value, int Ctl3D, int Left, int Top, int Height, int Visible, void *HandlerClass);
		~CScrollBar();
		void SetCtl3D(int Ctl3D);
		void SetMin(int Min);
		void SetMax(int Max);
		void SetValue(int Value);
		int GetValue();
		void Focus();
		void Blur();

		void OnChange(TScrollChange ScrollChange);

		int MouseDown(int MouseX, int MouseY);
		int MouseMove(int X, int Y);
		void MouseUp();

		void SetOwner(CControl *Owner);

	private:
		void Draw(long Left, long Top, long Width, long Height);
		void DrawButton(int Top, int ButtonUp, int UpArrow, int Enabled);
		void DrawArrow(int UpArrow, int ButtonUp, int Enabled);
		int Min;
		int Max;
		int Value;
		int Ctl3D;
		int TopButtonClick;
		int BottomButtonClick;
		int ScrollButtonClick;
		TScrollChange ScrollChange;
		bool MouseAction;
		CControl *Owner;

};

#endif