/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <scroll.h>
#include <graph.h>
#include <key.h>

#define SCROLLBAR_WIDTH 18

#define BUTTON_UP     0
#define BUTTON_DOWN   1
#define BUTTON_SCROLL 2

void printf(const char *,...);

CScrollBar::CScrollBar(int Min, int Max, int Value, int Ctl3D, int Left, int Top, int Height, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,SCROLLBAR_WIDTH,Height,Visible,false,HandlerClass)
{
	this->Min = Min;
	this->Max = Max;
	this->Value = Value;
	this->Ctl3D = Ctl3D;
	ScrollChange = NULL;
	TopButtonClick = false;
	BottomButtonClick = false;
	ScrollButtonClick = false;
	MouseAction = false;
	Owner = NULL;
}

CScrollBar::~CScrollBar()
{
}

void CScrollBar::SetOwner(CControl *Owner)
{
	this->Owner = Owner;
}

void CScrollBar::SetCtl3D(int Ctl3D)
{
	this->Ctl3D = Ctl3D;
	Refresh();
}

void CScrollBar::SetMin(int Min)
{
	this->Min = Min;
	Refresh();
}

void CScrollBar::SetMax(int Max)
{
	this->Max = Max;
	Refresh();
}

void CScrollBar::SetValue(int Value)
{
	if (Value < Min)
		Value = Min;
	else
		if (Value > Max)
			Value = Max;
	if (Value != this->Value) {
		this->Value = Value;
		Refresh();
		if (ScrollChange && HandlerClass)
			ScrollChange(HandlerClass,Value);
	}
}

void CScrollBar::Focus()
{
	if (!MouseAction && Parent && HandlerClass) {
		if (Parent->GetLastFocus() == HandlerClass) {
			Parent->FocusNext();
		}
		else {
			Parent->FocusPrev();
		}
	}
	else {
		if (Owner) {
			Owner->Focus();
		}
		GotFocus = true;
	}
}

void CScrollBar::Blur()
{
	GotFocus = false;
}


int CScrollBar::GetValue()
{
	return Value;
}

void CScrollBar::OnChange(TScrollChange ScrollChange)
{
	this->ScrollChange = ScrollChange;
}

int CScrollBar::MouseDown(int MouseX, int MouseY)
{
	int Status;

	MouseAction = true;
	Status = CControl::MouseDown(MouseX, MouseY);
	if (Status != -1) {
		if (MouseX > Left && MouseX < Right)
			if (MouseY > Top && MouseY < Top + 17) {
				TopButtonClick = true;
				SetValue(Value - 1);
			}
			else
				if (MouseY < Bottom && MouseY > Bottom - 17) {
					BottomButtonClick = true;
					SetValue(Value + 1);
				}
				else {
					ScrollButtonClick = true;
					MouseMove(MouseX,MouseY);
				}
	}
	else {
		MouseAction = false;
	}
	return Status;
}

int CScrollBar::MouseMove(int X, int Y)
{
	int ValueM2;
	int Value;
	int Status;

	Status = CAnimatedControl::MouseMove(X,Y);
//	if (Status != -1 && Parent) {
//		Parent->MouseMove(X,Y);
//	}

	if (ScrollButtonClick) {
		ValueM2 = ((long)(Y - Top - 17) * (Max - Min) * 2) / (Height - 50);
		Value = (ValueM2 >> 1) + Min;
		if (ValueM2 & 0x01)
			++Value;
		SetValue(Value);
	}
	return Status;
}

void CScrollBar::MouseUp()
{
	if (TopButtonClick || BottomButtonClick) {
		TopButtonClick = false;
		BottomButtonClick = false;
		Refresh();
	}
	else
		ScrollButtonClick = false;
	MouseAction = false;
	if (Parent && HandlerClass)
		Parent->SetFocusWnd((CControl *)HandlerClass,false);
}


void CScrollBar::Draw(long, long, long, long)
{
	register int ScrollPos;

	if (Ctl3D) {
		Graph->HLine(0,0,17,18);
		Graph->VLine(0,1,Height - 2,18);
		Graph->HLine(1,Height - 1,17,21);
		Graph->VLine(17,1,Height - 2,21);
	}
	Graph->Bar(1,1,16,Height - 2,20);
	DrawButton(1,TopButtonClick ? BUTTON_DOWN : BUTTON_UP,true,Enabled);
	DrawButton(Height - 17,BottomButtonClick ? BUTTON_DOWN : BUTTON_UP,false,Enabled);
	if (Enabled) {
		ScrollPos = 17 + ((long)(Value - Min) * (Height - 50)) / (Max - Min);
		DrawButton(ScrollPos,BUTTON_SCROLL,true,true);
	}
}

void CScrollBar::DrawButton(int Top, int Button, int UpArrow, int Enabled)
{
	if (Button == BUTTON_DOWN) {
		Graph->Rectangle(1,Top,16,16,18);
		DrawArrow(UpArrow,false,true);
	}
	else {
		if (MouseIsOver && Enabled) {
			Graph->HLine(1,Top + 15,16,17);
			Graph->VLine(16,Top,15,17);

			Graph->HLine(2,Top + 1,13,21);
			Graph->VLine(2,Top + 2,12,21);

			Graph->HLine(2,Top + 14,14,18);
			Graph->VLine(15,Top + 1,13,18);
		}
		else {
			Graph->HLine(1,Top + 15,16,18);
			Graph->VLine(16,Top,15,18);

			Graph->HLine(2,Top + 1,13,21);
			Graph->VLine(2,Top + 2,12,21);
		}
		if (Button == BUTTON_UP)
			DrawArrow(UpArrow,true,Enabled);
	}
}

void CScrollBar::DrawArrow(int UpArrow, int ButtonUp, int Enabled)
{
	int Left, Top, Color;

	if (ButtonUp) {
		Left = 0;
		Top = 0;
	}
	else {
		Left = 1;
		Top = 1;
	}
	if (UpArrow) {
		Left += 8;
		Top += 7;
		if (Enabled)
			Color = 17;
		else {
			Color = 18;
			Graph->HLine(Left - 2,Top + 4,7,21);
		}
		Graph->PutPixel(Left,Top,Color);
		Graph->HLine(Left - 1,Top + 1,3,Color);
		Graph->HLine(Left - 2,Top + 2,5,Color);
		Graph->HLine(Left - 3,Top + 3,7,Color);
	}
	else {
		Left += 5;
		Top += Height - 11;
		if (Enabled)
			Color = 17;
		else {
			Color = 18;
			Graph->Line(Left + 4,Top + 3,Left + 6,Top + 1,21);
			Graph->Line(Left + 4,Top + 4,Left + 7,Top + 1,21);
		}
		Graph->HLine(Left,Top,7,Color);
		Graph->HLine(Left + 1,Top + 1,5,Color);
		Graph->HLine(Left + 2,Top + 2,3,Color);
		Graph->PutPixel(Left + 3,Top + 3,Color);
	}
}


