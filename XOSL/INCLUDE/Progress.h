/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __progress__
#define __progress__

#include <Control.h>

class CProgressBar: public CControl {
	public:
		CProgressBar(int Min, int Max, int Progress,
						 int Left, int Top, int Width, int Height, int Visible);
		~CProgressBar();
		void SetMin(int Min);
		void SetMax(int Max);
		void SetProgress(int Progress);
		int GetProgress();
		void SetShowText(int ShowText);
		void SetColor(int Color);
		void SetTextColor(int TextColor);
	private:
		void Draw(long Left, long Top, long Width, long Height);
		int Min;
		int Max;
		int Progress;
		int ShowText;
		int Color;
		int TextColor;
		CString Text;
		int TextWidth;

		long Percent;
};

#endif