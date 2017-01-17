/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <button.h>
#include <graph.h>
#include <key.h>

CButton::CButton(const char *Caption, int Left, int Top, int Width, int Height, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,Width,Height,Visible,false,HandlerClass)
{
	this->Caption = Caption;
	ButtonDown = false;
	DrawDown = false;
	FontStyle = STYLE_REGULAR;
}

CButton::~CButton()
{
}

void CButton::SetFontStyle(int FontStyle)
{
	this->FontStyle = FontStyle;
}

int CButton::MouseDown(int MouseX, int MouseY)
{
	int Status;
	int HadFocus;

	HadFocus = GotFocus;
	DrawDown = true;
	Status = CControl::MouseDown(MouseX,MouseY);
	if (Status != -1 && Enabled) {
		ButtonDown = true;
		if (HadFocus)
			Refresh();
	}
	else
		DrawDown = false;
	return Status;
}

int CButton::MouseMove(int X, int Y)
{
	int Status;

	Status = CAnimatedControl::MouseMove(X,Y);

	if (ButtonDown) {
		if (MouseX < Left || MouseX > Right || MouseY < Top || MouseY > Bottom) {
			if (DrawDown) {
				DrawDown = false;
				Refresh();
			}
		}
		else if (!DrawDown) {
			DrawDown = true;
			Refresh();
		}
	}
	return Status;
}

void CButton::MouseUp()
{
	ButtonDown = false;
	if (DrawDown) {
		DrawDown = false;
		Refresh();
		CControl::MouseUp();
	}
}

void CButton::Draw(long, long, long, long)
{
	int XCap, YCap;

	XCap = (Width - Graph->GetTextWidth(Caption,FontStyle)) >> 1;
	YCap = (Height - Graph->GetTextHeight()) >> 1;

	Graph->Bar(1,1,Width - 2,Height - 2,19);
	if (GotFocus) {
		Graph->Rectangle(0,0,Width,Height,17);
		if (DrawDown) {
			Graph->Bar(2,2,Width - 4,Height - 4,19);
			Graph->Rectangle(1,1,Width - 2,Height - 2,18);
			Graph->TextOut(XCap + 1,YCap + 1,Caption,FontStyle,17);
			Graph->Rectangle(5,5,Width - 8,Height - 8,18);
		}
		else {
			Graph->Rectangle(4,4,Width - 8,Height - 8,18);
//			Graph->TextOut(XCap,YCap,Caption,FontStyle,17);
			if (Enabled) {
				Graph->TextOut(XCap,YCap,Caption,FontStyle,17);
			}
			else {
				Graph->TextOut(XCap + 1,YCap + 1,Caption,FontStyle,21);
				Graph->TextOut(XCap,YCap,Caption,FontStyle,18);
			}
			Graph->HLine(1,1,Width - 3,21);
			Graph->VLine(1,2,Height - 4,21);
			if (MouseIsOver) {
				Graph->HLine(1,Height - 2,Width - 2,17);
				Graph->VLine(Width - 2,1,Height - 3,17);
				Graph->HLine(2,Height - 3,Width - 4,18);
				Graph->VLine(Width - 3,2,Height - 5,18);
			}
			else {
				Graph->HLine(1,Height - 2,Width - 2,18);
				Graph->VLine(Width - 2,1,Height - 3,18);
			}
		}

	}
	else {
		Graph->HLine(0,0,Width - 1,21);
		Graph->VLine(0,1,Height - 2,21);
		if (Enabled && MouseIsOver) {
			Graph->HLine(0,Height - 1,Width,17);
			Graph->VLine(Width - 1,0,Height - 1,17);
			Graph->HLine(1,Height - 2,Width - 2,18);
			Graph->VLine(Width - 2,1,Height - 3,18);
			Graph->TextOut(XCap,YCap,Caption,FontStyle,17);
		}
		else {
			Graph->HLine(0,Height - 1,Width,18);
			Graph->VLine(Width - 1,0,Height - 1,18);
			if (Enabled) {
				Graph->TextOut(XCap,YCap,Caption,FontStyle,17);
			}
			else {
				Graph->TextOut(XCap + 1,YCap + 1,Caption,FontStyle,21);
				Graph->TextOut(XCap,YCap,Caption,FontStyle,18);
			}
		}
	}

/*
	if (GotFocus) {
		Graph->Rectangle(0,0,Width,Height,17);
		if (DrawDown) {
			Graph->Bar(2,2,Width - 4,Height - 4,19);
			Graph->Rectangle(1,1,Width - 2,Height - 2,18);
			Graph->TextOut(XCap + 1,YCap + 1,Caption,FontStyle,17);
			Graph->Rectangle(5,5,Width - 8,Height - 8,18);
		}
		else {
			Graph->Bar(3,3,Width - 6,Height - 6,19);
			if (Enabled)
				Graph->TextOut(XCap,YCap,Caption,FontStyle,17);
			else {
				Graph->TextOut(XCap + 1,YCap + 1,Caption,FontStyle,21);
				Graph->TextOut(XCap,YCap,Caption,FontStyle,18);
			}
			Graph->Rectangle(0,0,Width,Height,17);
			Graph->HLine(1,1,Width - 3,21);
			Graph->VLine(1,2,Height - 4,21);
			Graph->HLine(2,2,Width - 5,20);
			Graph->VLine(2,3,Height - 6,20);
			Graph->HLine(1,Height - 2,Width - 2,17);
			Graph->VLine(Width - 2,1,Height - 3,17);
			Graph->HLine(2,Height - 3,Width - 4,18);
			Graph->VLine(Width - 3,2,Height - 5,18);
//			Graph->TextOut(XCap,YCap,Caption,STYLE_REGULAR,17);
			Graph->Rectangle(4,4,Width - 8,Height - 8,18);
		}
	}
	else {
		Graph->Bar(2,2,Width - 4,Height - 4,19);
		Graph->HLine(0,0,Width - 1,21);
		Graph->VLine(0,1,Height - 2,21);
		Graph->HLine(1,1,Width - 3,20);
		Graph->VLine(1,2,Height - 4,20);
		Graph->HLine(0,Height - 1,Width,17);
		Graph->VLine(Width - 1,0,Height - 1,17);
		Graph->HLine(1,Height - 2,Width - 2,18);
		Graph->VLine(Width - 2,1,Height - 3,18);
		if (Enabled)
			Graph->TextOut(XCap,YCap,Caption,FontStyle,17);
		else {
			Graph->TextOut(XCap + 1,YCap + 1,Caption,FontStyle,21);
			Graph->TextOut(XCap,YCap,Caption,FontStyle,18);
		}
	}	*/
}

void CButton::KeyPress(unsigned short Key)
{
	if (ButtonDown)
		return;
	if (WndOnKeyPress && HandlerClass)
		WndOnKeyPress(HandlerClass,Key);
	switch (Key) {
		case KEY_ENTER:
		case KEY_K_ENTER:
		case KEY_SPACE:
			MouseDown(Left,Top);
			MouseUp();
			break;
	}
}


