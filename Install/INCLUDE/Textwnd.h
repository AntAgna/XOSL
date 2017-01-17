/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __textwnd__
#define __textwnd__

#include <textscr.h>

class CTextWindow {
	public:
		CTextWindow(int Left, int Top, int Width, int Height, int Attr, CTextScreen &TextScreenToUse);
		~CTextWindow();
		void Clear();
		void PutStr(const char *Str);
		void PutChar(int Ch);

	private:
		void AddChar(int Ch);
		void Refresh();
		void Scroll();

		int Left, Top;
		int Width, Height;
		unsigned short Attr;
		unsigned short *Window;
		int CursorX, CursorY;

		CTextScreen &TextScreen;
};

#endif