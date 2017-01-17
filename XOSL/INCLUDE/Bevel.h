/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __bevel__
#define __bevel__

#include <Control.h>

#define BEVEL_VLINE 0
#define BEVEL_HLINE 1
#define BEVEL_BOX   2
#define BEVEL_FRAME 3

class CBevel: public CControl {
	public:
		CBevel(int Type, int Lowered, int Left, int Top,
				 int Width, int Height, int Visible);
		CBevel(int Type, int Lowered, int Left, int Top,
				 int Size, int Visible);
		~CBevel();
	private:
		void Draw(long Left, long Top, long Width, long Height);
		void SetLowered(int Lowered);
		int Type;
		int ColorLeftTop, ColorRightBottom;
};

#endif