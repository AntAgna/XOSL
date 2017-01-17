/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <backgnd.h>
#include <graph.h>
#include <rgb.h>
#include <cstring.h>

static const char *Title = "Extended Operating System Loader 1.1.5";
static const char *SubTitle = "with Ranish Partition Manager 2.38 Beta 1.9";

CBackground::CBackground()
{
}

CBackground::~CBackground()
{
}

void CBackground::SplashScreen(int Width, int Height)
{
	int Left, Top;

	Graph->Bar(0,0,Width,Height,0xfe);
	Left = Width - SplashLogoWidth >> 1;
	Top = Height - SplashLogoHeight >> 1;
	Graph->PutImage(Left,Top,SplashLogoWidth,SplashLogoHeight,SplashLogo);
}

void CBackground::ShowSubTitle(int Width, int Height)
{
	int Left, Top;
	int TextWidth, TextHeight;

	TextWidth = Graph->GetTextWidth(SubTitle,STYLE_REGULAR);
	TextHeight = Graph->GetTextHeight();
	Left = Width - TextWidth - 2; //(Width - TextWidth) >> 1;
	Top = Height - TextHeight - 2;// (Height - SplashLogoHeight >> 1) + SplashLogoHeight + 20;
	DrawBorderedText(Left,Top,SubTitle);
	Graph->FlushArea(Left,Top,TextWidth,TextHeight);
}


void CBackground::NewGraphicsMode(int Width, int Height, int UseWallpaper)
{
	int X, Y;
	int TitleLeft, TitleTop;

	if (UseWallpaper)
		for (Y = 0; Y < Height; Y += WallpaperHeight)
			for (X = 0; X < Width; X += WallpaperWidth)
				Graph->PutImage(X,Y,WallpaperWidth,WallpaperHeight,Wallpaper);
	else
		Graph->Bar(0,0,Width,Height,16);

	TitleLeft = Width - Graph->GetTextWidth(Title,STYLE_REGULAR) - 2;
	TitleTop = Height - Graph->GetTextHeight() - 2;
	DrawBorderedText(TitleLeft,TitleTop,Title);

	Graph->StoreBuffer(0,0,Width,Height);

}

void CBackground::DrawBorderedText(int Left, int Top, const char *Text)
{
	Graph->TextOut(Left - 1,Top,Text,STYLE_REGULAR,0);
	Graph->TextOut(Left + 1,Top,Text,STYLE_REGULAR,0);
	Graph->TextOut(Left,Top - 1,Text,STYLE_REGULAR,0);
	Graph->TextOut(Left,Top + 1,Text,STYLE_REGULAR,0);
	Graph->TextOut(Left - 1,Top - 1,Text,STYLE_REGULAR,0);
	Graph->TextOut(Left - 1,Top + 1,Text,STYLE_REGULAR,0);
	Graph->TextOut(Left + 1,Top - 1,Text,STYLE_REGULAR,0);
	Graph->TextOut(Left + 1,Top + 1,Text,STYLE_REGULAR,0);
	Graph->TextOut(Left,Top,Text,STYLE_REGULAR,15);
}

void CBackground::DrawTestScreen()
{
	DrawTestBlock(1600,1200,2);
	DrawTestBlock(1280,1024,1);
	DrawTestBlock(1024,768,4);
	DrawTestBlock(800,600,2);
	DrawTestBlock(640,480,1);
	Graph->HLine(0,0,20,15);
	Graph->VLine(0,0,20,15);
}

void CBackground::DrawTestBlock(int Width, int Height, int Color)
{
	int Step, Index;
	int TextLeft, TextTop;
	TGraphMode GraphMode;
	const char *ModeName;

	Graph->Bar(0,0,Width,Height,Color);
	Step = Height / 10;
	for (Index = 0; Index + 10 < Height; Index += Step) {
		Graph->HLine(Width - 20,Index,20,15);
		Graph->VLine(Width - 1,Index - 10,20,15);
	}
	Step = Width / 10;
	for (Index = 0; Index + 10 < Width; Index += Step) {
		Graph->VLine(Index,Height - 20,20,15);
		Graph->HLine(Index - 10, Height - 1,20,15);
	}
	GraphMode = Graph->GetModeFromMetrics(Width,Height);
	ModeName = Graph->GetModeName(GraphMode);
	TextTop = Height - Graph->GetTextHeight() - 30;
	TextLeft = Width - Graph->GetTextWidth(ModeName,STYLE_BOLD) >> 1;
	Graph->TextOut(TextLeft,TextTop,ModeName,STYLE_BOLD,0);

}

void CBackground::FixDamage(int Left, int Top, int Width, int Height)
{
	Graph->RestoreBuffer(Left,Top,Width,Height);
}

void CBackground::SetSplashLogo(int Width, int Height, const unsigned char *SplashLogo)
{
	this->SplashLogo = SplashLogo;
	SplashLogoWidth = Width;
	SplashLogoHeight = Height;
}

void CBackground::SetWallpaper(int Width, int Height, const unsigned char *Wallpaper)
{
	this->Wallpaper = Wallpaper;
	WallpaperWidth = Width;
	WallpaperHeight = Height;
}
