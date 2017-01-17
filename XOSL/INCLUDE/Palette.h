/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __palette__
#define __palette__

#include <newdefs.h>

typedef struct {
	int Red;
	int Green;
	int Blue;
} TRGBPalette;

typedef struct {
	int Hue;
	int Saturation;
	int Luminance;
} THSLPalette;

typedef union {
	TRGBPalette RGBPalette[256];
	THSLPalette HSLPalette[256];
} TPalette;

class CPalette {
	public:
		CPalette();
		~CPalette();
		void Install();
		void SetScheme(int Index);
		void SetHue(int Hue);
		void SetSaturation(int Saturation);
		void SetColorLayer(int Red, int Green, int Blue, int Alpha);
		void SetPaletteEntry(int Index, int Red, int Green, int Blue);

		void LockFading();
		void FadeIn();
		void FadeOut();
		void UltraFadeIn();
		void UltraFadeOut();
		void FadeToBlack();
		void SetFadeOutColor(int Intensity);

	private:
		void CopyDefaultPalette();
		void UpdatePalette();
		void InstallPalette();

		void HSL2RGB(int &Red, int &Green, int &Blue,
						 int Hue, int Saturation, int Luminance);
		int GetColor(long Temp1, long Temp2, long Temp3);

      void MergeColors(TRGBPalette &Back, const TRGBPalette &Front, int Alpha);

		int Scheme;
		int Hue;
		int Saturation;
		TRGBPalette ColorLayer;
		int Alpha;
		TPalette *Palette;

		TRGBPalette *PreFade;
		int Fading;

		int FadeColorInt;
};

#endif