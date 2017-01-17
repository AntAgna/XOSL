/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <textwnd.h>
#include <memory.h>

CTextWindow::CTextWindow(int Left, int Top, int Width, int Height, int Attr, CTextScreen &TextScreenToUse):
	TextScreen(TextScreenToUse)
{
	this->Left = Left;
	this->Top = Top;
	this->Width = Width;
	this->Height = Height;
	this->Attr = Attr;
	CursorX = 0;
	CursorY = 0;
	Window = new unsigned short [Width * Height];
	Clear();
}

CTextWindow::~CTextWindow()
{
	delete Window;
}

void CTextWindow::Clear()
{
	int X, Y, Index;

	for (Index = Y = 0; Y < Height; ++Y)
		for (X = 0; X < Width; ++X)
			Window[Index++] = Attr;
	Refresh();
}

void CTextWindow::PutStr(const char *Str)
{
	for (; *Str; AddChar(*Str++));
	Refresh();
}

void CTextWindow::PutChar(int Ch)
{
	AddChar(Ch);
	Refresh();
}

void CTextWindow::AddChar(int Ch)
{
	switch (Ch) {
		default:
			Window[CursorX + CursorY * Width] = Attr | Ch;
			if (++CursorX < Width)
				break;
		case '\n':
			++CursorY;
			if (CursorY == Height) {
				Scroll();
				--CursorY;
			}
		case '\r':
			CursorX = 0;
			break;
	}
}

void CTextWindow::Refresh()
{
	TextScreen.PutImage(Left,Top,Width,Height,Window);
}

void CTextWindow::Scroll()
{
	int Index;
	unsigned short *LastLine;

	MemCopy(Window,&Window[Width],2 * Width * (Height - 1));
	LastLine = &Window[Width * (Height - 1)];
	for (Index = 0; Index < Width; ++Index)
		*LastLine++ = Attr;
}
