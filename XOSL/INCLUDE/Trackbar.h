/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __trackbar__
#define __trackbar__

#include <AniCntrl.h>

typedef void (*TTrackBarChange)(void *HandlerClass, int Value);

class CTrackBar: public CAnimatedControl {
	public:
		CTrackBar(int Min, int Max, int Value, int Left, int Top, int Width, int Visible, void *HandlerClass);
		~CTrackBar();

		void SetMin(int Min);
		void SetMax(int Max);
		void SetValue(int Value);
		int GetValue();

		void SetMetrics(int Width, int Height);

		int MouseDown(int MouseX, int MouseY);
		int MouseMove(int X, int Y);
		void MouseUp();

		void OnChange(TTrackBarChange TrackBarChange);
		void KeyPress(unsigned short Key);

	private:
		void Draw(long Left, long Top, long Width, long Height);
		void SetSliderPos();
		void DrawSlider();
		int Min;
		int Max;
		int Value;
		int SliderPos;
		int ButtonDown;
		TTrackBarChange TrackBarChange;
};


#endif