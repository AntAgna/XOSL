/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <label.h>

#include <graph.h>


CLabel::CLabel(const char *Caption, int FontStyle, int Transparent, int Color,
	int Left, int Top, int Visible):
	CControl(Left,Top,Graph->GetTextWidth(Caption,FontStyle),
			  Graph->GetTextHeight(),Visible,false,false,NULL)
{
	this->Caption = Caption;
	this->FontStyle = FontStyle;
	this->Transparent = Transparent;
	this->Color = Color;
	AutoSize = true;
}

CLabel::CLabel(const char *Caption, int FontStyle, int Transparent, int Color,
	int Left, int Top, int Width, int Visible):
	CControl(Left,Top,Width,Graph->GetTextHeight(),Visible,false,false,NULL)
{
	this->Caption = Caption;
	this->FontStyle = FontStyle;
	this->Transparent = Transparent;
	this->Color = Color;
	AutoSize = false;
}

CLabel::~CLabel()
{
}

void CLabel::SetTransparent(int Transparent)
{
	this->Transparent = Transparent;
	Refresh();
}

void CLabel::SetAutoSize(int AutoSize)
{
	this->AutoSize = AutoSize;
	if (AutoSize)
		SetMetrics(Graph->GetTextWidth(Caption,FontStyle),Height);
}

void CLabel::SetFontStyle(int Style)
{
	FontStyle = Style;
	SetMetrics(Graph->GetTextWidth(Caption,FontStyle),Graph->GetTextHeight());
}

void CLabel::SetColor(int Color)
{
	this->Color = Color;
	Refresh();
}

void CLabel::SetCaption(const char *Caption)
{
	int TextWidth;

	if (AutoSize) {
		this->Caption = Caption;
		TextWidth = Graph->GetTextWidth(Caption,FontStyle);

		if (TextWidth != Width)
			SetMetrics(TextWidth,Height);
		else
			Refresh();
	}
	else
		CControl::SetCaption(Caption);
}

void CLabel::Draw(long, long, long, long)
{
	if (!Transparent)
		Graph->Bar(0,0,Width,Height,19);
	Graph->TextOut(0,0,Caption,FontStyle,Color);
}