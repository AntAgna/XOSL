/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __button__
#define __button__

#include <AniCntrl.h>

class CButton: public CAnimatedControl {
	public:
		CButton(const char *Caption, int Left, int Top, int Width, int Height, int Visible, void *HandlerClass);
		~CButton();
		void SetFontStyle(int FontStyle);
		int MouseDown(int MouseX, int MouseY);
		int MouseMove(int X, int Y);
		void MouseUp();
		void KeyPress(unsigned short Key);
	private:
		void Draw(long Left, long Top, long Width, long Height);
		int ButtonDown, DrawDown;
		int FontStyle;

		void DrawNormal(int CapLeft, int CapTop);
		void DrawFocussed(int CapLeft, int CapTop);

};

#endif