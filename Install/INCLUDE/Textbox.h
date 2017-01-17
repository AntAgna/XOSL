/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __textbox__
#define __textbox__

#include <textscr.h>

class CTextBox {
	public:
		CTextBox(int Left, int Top, int Width, int Centered, int Attr, CTextScreen &TextScreenToUse);
		~CTextBox();
		void SetText(const char *Str);
	private:
		int Left;
		int Top;
		int Width;
		int Centered;
		int Attr;
		char *Str;

		CTextScreen &TextScreen;
};

#endif