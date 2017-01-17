/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <textbox.h>
#include <string.h>
#include <memory.h>

CTextBox::CTextBox(int Left, int Top, int Width, int Centered, int Attr, CTextScreen &TextScreenToUse):
	TextScreen(TextScreenToUse)
{
	this->Left = Left;
	this->Top = Top;
	this->Width = Width;
	this->Centered = Centered;
	this->Attr = Attr;
	Str = new char [Width + 1];
	Str[Width] = '\0';
	TextScreen.FillX(Left,Top,32,Attr,Width);
}

CTextBox::~CTextBox()
{
	delete Str;
}

void CTextBox::SetText(const char *Str)
{
	int StrLen;
	int StrLeft;

	if ((StrLen = strlen(Str)) > Width)
		MemCopy(this->Str,Str,Width);
	else
		strcpy(this->Str,Str);
	if (Centered)
		StrLeft = Width - StrLen >> 1;
	else
		StrLeft = 0;
	TextScreen.FillX(Left,Top,32,Attr,Width);
	TextScreen.PutStr(Left + StrLeft,Top,this->Str,Attr);
}
