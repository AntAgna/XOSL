/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <progress.h>
#include <graph.h>

CProgressBar::CProgressBar(int Min, int Max, int Progress,
	int Left, int Top, int Width, int Height, int Visible):
	CControl(Left,Top,Width,Height,Visible,false,false,NULL)
{
	this->Min = Min;
	this->Max = Max - Min;
	this->Progress = Progress - Min;
	this->Color = 17;
	this->TextColor = 21;
	this->ShowText = false;
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::SetMin(int Min)
{
	Max += this->Min - Min;
	Progress = this->Min - Min;
	this->Min = Min;
	Refresh();
}

void CProgressBar::SetMax(int Max)
{
	this->Max = Max - Min;
	Refresh();
}

void CProgressBar::SetProgress(int Progress)
{
	this->Progress = Progress - Min;
	if (ShowText) {
		Text = ((long)100 * this->Progress) / Max;
		Text += "%";
		TextWidth = Graph->GetTextWidth(Text,STYLE_REGULAR);
	}
	Refresh();
}

int CProgressBar::GetProgress()
{
	return Progress + Min;
}

void CProgressBar::SetShowText(int ShowText)
{
	this->ShowText = ShowText;
	if (ShowText)
		SetProgress(Progress + Min);
	else
		Refresh();
}

void CProgressBar::SetColor(int Color)
{
	this->Color = Color;
	Refresh();
}

void CProgressBar::SetTextColor(int TextColor)
{
	this->TextColor = TextColor;
	Refresh();
}

void CProgressBar::Draw(long, long, long, long)
{
	int ProgressWidth;
	int TextLeft, TextTop;

	Graph->HLine(0,0,Width - 1,18);
	Graph->VLine(0,1,Height - 2,18);
	Graph->HLine(0,Height - 1,Width,21);
	Graph->VLine(Width - 1,0,Height - 1,21);

	if (Progress == Max)
		ProgressWidth = Width - 4;
	else
		ProgressWidth = ((long)(Width - 4) * Progress) / Max;
	Graph->Bar(2,2,ProgressWidth,Height - 4,Color);
	if (ShowText || GotFocus) {
		TextLeft = Width - TextWidth >> 1;
		TextTop = Height - Graph->GetTextHeight() >> 1;
		Graph->TextOut(TextLeft,TextTop,Text,STYLE_REGULAR,TextColor);
	}
}
