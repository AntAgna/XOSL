/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __checkbox__
#define __checkbox__

#include <AniCntrl.h>

typedef void (*TCheckBoxChange)(void *HandlerClass, int Checked);

class CCheckBox: public CAnimatedControl {
	public:
		CCheckBox(const char *Caption, int Checked, int Left, int Top, int Visible, void *HandlerClass);
		CCheckBox(const char *Caption, int Checked, int Left, int Top, int Width, int Visible, void *HandlerClass);
   	~CCheckBox();

      void SetCaption(const char *Caption);
		void SetAutoSize(int AutoSize);
		void SetChecked(int Checked);
		int IsChecked();

		int MouseDown(int MouseX, int MouseY);
		void KeyPress(unsigned short Key);

		void OnChange(TCheckBoxChange CheckBoxChange);
	private:
		void Draw(long Left, long Top, long Width, long Height);
		int AutoSize;
		int Checked;
		TCheckBoxChange CheckBoxChange;

};

#endif
