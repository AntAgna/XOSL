/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __textscr__
#define __textscr__

class CTextScreen {
	public:
		CTextScreen(int Attr);
		~CTextScreen();
		void PutStr(int X, int Y, const char *Str, int Attr);
		void PutChar(int X, int Y, int Ch, int Attr);
		void FillX(int X, int Y, int Ch, int Attr, int Count);
		void FillY(int X, int Y, int Ch, int Attr, int Count);
		void PutImage(int X, int Y, int Width, int Height, unsigned short *Image);
		void GetImage(int X, int Y, int Width, int Height, unsigned short *Image);
	private:
};

#endif