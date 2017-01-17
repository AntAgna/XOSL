/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __label__
#define __label__

#include <Control.h>

class CLabel: public CControl {
	public:
		CLabel(const char *Caption, int FontStyle, int Transparent, int Color,
				 int Left, int Top, int Visible);
		CLabel(const char *Caption, int FontStyle, int Transparent, int Color,
				 int Left, int Top, int Width, int Visible);
      ~CLabel();

		void SetTransparent(int Transparent);
		void SetAutoSize(int AutoSize);
		void SetFontStyle(int Style);
		void SetColor(int Color);
		void SetCaption(const char *Caption);
	private:
		void Draw(long Left, long Top, long Width, long Height);
		int Transparent;
		int AutoSize;
		int FontStyle;
		int Color;
};

#endif

