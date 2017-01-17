/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <edit.h>
#include <graph.h>
#include <string.h>
#include <key.h>

extern void printf(const char *fmt, ...);

CEdit::CEdit(const char *Text, int MaxLength, int Masked, int ReadOnly, int Left, int Top, int Width, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,Width,21,Visible,false,HandlerClass)
{
	Caption = Text;
	this->MaxLength = MaxLength;
	this->Masked = Masked;
	this->ReadOnly = ReadOnly;
	Cursor = 0;
}


CEdit::~CEdit()
{
}

void CEdit::SetMaxLength(int MaxLength)
{
	this->MaxLength = MaxLength;
}

void CEdit::SetMasked(int Masked)
{
	this->Masked = Masked;
}

void CEdit::SetReadOnly(int ReadOnly)
{
	this->ReadOnly = ReadOnly;
}

void CEdit::Focus()
{
	Cursor = Caption.Length();
	CControl::Focus();
}

void CEdit::Draw(long,long,long,long)
{
	int TextTop, TextLeft;
	int TextWidth;
	const char *DisplayStr;

	TextTop = 21 - Graph->GetTextHeight() >> 1;
	Graph->Bar(1,1,Width - 2,Height - 2,21);

	if (!Masked)
		DisplayStr = Caption;
	else {
		Mask.CharFill('*',Caption.Length());
		DisplayStr = Mask;
	}
	if (!GotFocus) {
		TextWidth = Graph->GetTextWidth(DisplayStr,STYLE_REGULAR);
		if (TextWidth > Width - 5)
			TextLeft = Width - TextWidth - 5;
		else
			TextLeft = 3;
	}
	else {
		CString Str, Str1, Str2;

		Str = DisplayStr;
		Str.Split(Cursor,Str1,Str2);
		TextWidth = Graph->GetTextWidth(Str1,STYLE_REGULAR);
		if (TextWidth > Width - 5)
			TextLeft = Width - TextWidth - 5;
		else
			TextLeft = 3;
	}
//	printf("(%d,%d)  ",TextLeft,TextWidth);
	if (GotFocus)
		Graph->VLine(TextLeft + TextWidth,3,15,17);

	Graph->TextOut(TextLeft,TextTop,(const char *)DisplayStr,STYLE_REGULAR,17);

	Graph->HLine(0,0,Width - 1,18);
	Graph->VLine(0,1,Height - 2,18);

	if (Enabled && MouseIsOver) {
		Graph->HLine(1,1,Width - 3,17);
		Graph->VLine(1,2,Height - 4,17);

		Graph->HLine(1,Height - 2,Width - 2,20);
		Graph->VLine(Width - 2,1,Height - 3,20);
	}

	Graph->HLine(0,Height - 1,Width,21);
	Graph->VLine(Width - 1,0,Height - 1,21);

}

void CEdit::KeyPress(unsigned short Key)
{
	char KeyName[16];

	if (ReadOnly)
		return;
	if (WndOnKeyPress && HandlerClass)
		WndOnKeyPress(HandlerClass,Key);
	switch (Key) {
		case KEY_NONE:
			return;
		case KEY_BACKSPACE:
			if (Cursor) {
				Caption.DeleteChar(Cursor - 1);
				--Cursor;
				Refresh();
			}
			break;
		case KEY_LEFT:
		case KEY_LEFT_EXT:
			if (Cursor) {
				--Cursor;
				Refresh();
			}
			break;
		case KEY_RIGHT:
		case KEY_RIGHT_EXT:
			if (Cursor < Caption.Length()) {
				++Cursor;
				Refresh();
			}
			break;
		case KEY_DELETE:
		case KEY_DELETE_EXT:
			if (Cursor != Caption.Length()) {
				Caption.DeleteChar(Cursor);
				Refresh();
			}
			break;
		case KEY_HOME:
		case KEY_HOME_EXT:
			Cursor = 0;
			Refresh();
			break;
		case KEY_END:
		case KEY_END_EXT:
			Cursor = Caption.Length();
			Refresh();
			break;
		case KEY_PAGEUP:
		case KEY_PU_EXT:
		case KEY_CTRLLEFT:
		case KEY_CTRLL_EXT:
			Cursor -= 4;
			if (Cursor < 0)
				Cursor = 0;
			Refresh();
			break;
		case KEY_PAGEDOWN:
		case KEY_PD_EXT:
		case KEY_CTRLRIGHT:
		case KEY_CTRLR_EXT:
			Cursor += 4;
			if (Cursor > Caption.Length())
				Cursor = Caption.Length();
			Refresh();
			break;
		case KEY_ESCAPE:
			Caption.Clear();
			Cursor = 0;
			Refresh();
			break;
		default:
			if (!MaxLength || Caption.Length() < MaxLength && Key != 0x0000 && !CKeyboard::IsSpecialKey(Key)) {
				CKeyboard::GetShortKeyName(Key,KeyName);
				Caption.InsertChar(Cursor,*KeyName);
				++Cursor;
				Refresh();
			}
			break;
	}
}

void CEdit::SetText(const char *Text)
{
	SetCaption(Text);
	Cursor = Caption.Length();
}

void CEdit::GetText(char *Text)
{
	strcpy(Text,Caption);
}

