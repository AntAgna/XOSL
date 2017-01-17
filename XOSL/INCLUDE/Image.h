/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __image__
#define __image__

#include <Control.h>

class CImage: public CControl {
	public:
		CImage(const void *Image, int ImageWidth, int ImageHeight, int Border,
				 int Left, int Top, int Width, int Height, int Visible);
		~CImage();
		void SetImage(const void *Image, int ImageWidth, int ImageHeight);
		void SetBorder(int Border);
		void SetColor(int Color);
	private:
		void Draw(long Left, long Top, long Width, long Height);
		const void *Image;
		int ImageWidth, ImageHeight;
		int Border;
		int Color;
};

#endif