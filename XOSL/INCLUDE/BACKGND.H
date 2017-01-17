/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __background__
#define __background__

class CBackground {
	public:
		CBackground();
		~CBackground();
		void SplashScreen(int Width, int Height);
		void ShowSubTitle(int Width, int Height);

		void NewGraphicsMode(int Width, int Height, int UseWallpaper);
		void DrawTestScreen();
		void FixDamage(int Left, int Top, int Width, int Height);
		void SetWallpaper(int Width, int Height, const unsigned char *Wallpaper);
		void SetSplashLogo(int Width, int Height, const unsigned char *SplashLogo);
	private:
		void DrawTestBlock(int Width, int Height, int Color);
		const unsigned char *SplashLogo;
		int SplashLogoWidth;
		int SplashLogoHeight;
		const unsigned char *Wallpaper;
		int WallpaperWidth;
		int WallpaperHeight;

		void DrawBorderedText(int Left, int Top, const char *Text);


};

#endif