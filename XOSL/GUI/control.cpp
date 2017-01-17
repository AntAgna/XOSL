/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <Control.h>
#include <screen.h>
#include <graph.h>

CControl::CControl(int Left, int Top, int Width, int Height,
	int Visible, int OnTop, int FocusWnd, void *HandlerClass)
{
	this->Left = Left;
	this->Top = Top;
	this->Width = Width;
	this->Height = Height;
	Right = Left + Width - 1;
	Bottom = Top + Height - 1;

	this->Visible = Visible;
	Enabled = true;
	Parent = NULL;
	WndOnClick = NULL;
	WndOnShow = NULL;
	WndOnHide = NULL;
	WndOnKeyPress = NULL;
	this->OnTop = OnTop;
	this->FocusWnd = FocusWnd;
	this->HandlerClass = HandlerClass;
	GotFocus = false;
	PreFocus = true;
}

CControl::~CControl()
{
}

void CControl::Show()
{
	if (Visible || !Parent)
		return;
	Visible = true;
	Refresh();
	if (WndOnShow && HandlerClass)
		WndOnShow(HandlerClass);
}

void CControl::Hide()
{
	if (!Visible || !Parent)
		return;
	Visible = false;
	Refresh();
	if (WndOnHide && HandlerClass)
		WndOnHide(HandlerClass);
}

void CControl::SetVisible(int Visible)
{
	this->Visible = Visible;
}

void CControl::Focus()
{
	GotFocus = true;
	if (!PreFocus)
		Parent->FixDamage(Left,Top,Width,Height);
}

void CControl::Blur()
{
	GotFocus = false;
	if (!PreFocus)
		Parent->FixDamage(Left,Top,Width,Height);
}

int CControl::IsVisible()
{
	return Visible;
}

void CControl::Enable()
{
	if (!Enabled) {
		Enabled = true;
		Refresh();
	}
}

void CControl::Disable()
{
	if (Enabled) {
		Enabled = false;
		Refresh();
	}
}

void CControl::SetEnabled(int Enabled)
{
	if (Enabled)
		Enable();
	else
		Disable();
}

int CControl::IsEnabled()
{
	return Enabled;
}

void CControl::SetCaption(const char *Caption)
{
	this->Caption = Caption;
	Refresh();
}

const char *CControl::GetCaption()
{
	return Caption;
}

void CControl::SetMetrics(int NewWidth, int NewHeight)
{
	int DamageWidth, DamageHeight;
	int DamageLeft, DamageTop;

	if (Width == NewWidth && Height == NewHeight)
		return;
	if (NewWidth < Width)
		DamageWidth = Width;
	else
		DamageWidth = NewWidth;
	if (NewHeight < Height)
		DamageHeight = Height;
	else
		DamageHeight = NewHeight;
	Width = NewWidth;
	Height = NewHeight;
	Right = Left + Width - 1;
	Bottom = Top + Height - 1;
	if (Visible && Parent) {
		GetAbsPosition(DamageLeft,DamageTop);
		Parent->FixDamage(Left,Top,DamageWidth,DamageHeight);
		Graph->FlushArea(DamageLeft,DamageTop,DamageWidth,DamageHeight);
	}
}

void CControl::GetMetrics(int &Width, int &Height)
{
	Width = this->Width;
	Height = this->Height;
}

void CControl::SetPosition(int Left, int Top)
{
	int DamageLeft, DamageTop;
	int DamageWidth, DamageHeight;
	int OldLeft, OldTop;


	if (this->Left == Left && this->Top == Top)
		return;
	OldLeft = DamageLeft = this->Left;
	OldTop = DamageTop = this->Top;
	this->Left = Left;
	this->Top = Top;
	Right = Left + Width - 1;
	Bottom = Top + Height - 1;
	if (!Visible || !Parent)
		return;

	if (DamageLeft < Left)
		DamageWidth = Width + Left - DamageLeft;
	else {
		DamageWidth = Width + DamageLeft - Left;
		DamageLeft = Left;
	}
	if (DamageTop < Top)
		DamageHeight = Height + Top - DamageTop;
	else {
		DamageHeight = Height + DamageTop - Top;
		DamageTop = Top;
	}

	Parent->FixDamage(DamageLeft,DamageTop,DamageWidth,DamageHeight);
	Graph->FlushArea(Left,Top,Width,Height);
	Graph->FlushArea(OldLeft,OldTop,Width,Height);
}

void CControl::GetPosition(int &Left, int &Top)
{
	Left = this->Left;
	Top = this->Top;
}

void CControl::GetAbsPosition(int &Left, int &Top)
{
	if (!Parent)
		return;
	Parent->GetPosition(Left,Top);
	Left += this->Left;
	Top += this->Top;
}

void CControl::SetParent(CWindowList *Parent)
{
	this->Parent = Parent;
}

void CControl::Refresh()
{
	int DamageLeft, DamageTop;

	if (!Parent || !Parent->GetVisible())
		return;
	Parent->FixDamage(Left,Top,Width,Height);
	GetAbsPosition(DamageLeft,DamageTop);
	Graph->FlushArea(DamageLeft,DamageTop,Width,Height);
}

void CControl::FixDamage(int Left, int Top, int Width, int Height)
{
	int iAbsLeft, iAbsTop;
//	long AbsLeft, AbsTop;
	long VPLeft, VPTop;
	long ClipLeft,ClipTop,ClipWidth,ClipHeight;
//	long lLeft, lTop, lWidth, lHeight;

	if (Visible && AdjustArea(Left,Top,Width,Height) != -1 && Parent) {
		PreFocus = false;
		Graph->GetClippingRegion(ClipLeft,ClipTop,ClipWidth,ClipHeight);
		Graph->GetViewportOrigin(VPLeft,VPTop);

		GetAbsPosition(iAbsLeft,iAbsTop);
//		AbsLeft = iAbsLeft;
//		AbsTop = iAbsTop;

		Graph->SetViewportOrigin(iAbsLeft,iAbsTop);

		Left -= this->Left;
		Top -= this->Top;

//		lLeft = AbsLeft + Left;
//		lTop = AbsTop + Top;
//		lWidth = Width;
//		lHeight = Height;
		Graph->SetClippingRegion(iAbsLeft + Left,iAbsTop + Top,Width,Height);

		Draw(Left,Top,Width,Height);

		Graph->SetViewportOrigin(VPLeft,VPTop);
		Graph->SetClippingRegion(ClipLeft,ClipTop,ClipWidth,ClipHeight);

	}
}


void CControl::KeyPress(unsigned short Key)
{
	++Key;
}


int CControl::AdjustArea(int &iLeft, int &iTop, int &iWidth, int &iHeight)
{
	long iBottom, iRight;

	iBottom = iTop + iHeight - 1;
	iRight = iLeft + iWidth - 1;
	if (iRight < Left || iLeft > Right | iBottom < Top || iTop > Bottom)
		return -1;
	if (iLeft < Left) {
		iWidth -= Left - iLeft;
		iLeft = Left;
	}
	if (iTop < Top) {
		iHeight -= Top - iTop;
		iTop = Top;
	}
	if (iRight > Right)
		iWidth -= iRight - Right;
	if (iBottom > Bottom)
		iHeight -= iBottom - Bottom;
	return 0;
}


int CControl::MouseDown(int MouseX, int MouseY)
{
	this->MouseX = MouseX;
	this->MouseY = MouseY;
	if (!Visible || !FocusWnd || !Parent)
		return -1;
	if (Enabled && MouseX >= Left && MouseX <= Right && MouseY >= Top && MouseY <= Bottom) {
		if (!GotFocus/* && Enabled*/)
			Parent->SetFocusWnd(this,OnTop);
		return 0;
	}
	return -1;
}

int CControl::MouseMove(int X, int Y)
{
	MouseX = X;
	MouseY = Y;
	return -1;
}

void CControl::MouseUp()
{
	if (!Enabled)
		return;
	if (WndOnClick && HandlerClass)
		if (MouseX >= Left && MouseX < Left + Width &&
			 MouseY >= Top && MouseY < Top + Height)
			WndOnClick(HandlerClass);
}

void CControl::OnClick(TWndOnClick WndOnClick)
{
	this->WndOnClick = WndOnClick;
}

void CControl::OnShow(TWndOnShow WndOnShow)
{
	this->WndOnShow = WndOnShow;
}

void CControl::OnHide(TWndOnHide WndOnHide)
{
	this->WndOnHide = WndOnHide;
}

void CControl::OnKeyPress(TWndOnKeyPress WndOnKeyPress)
{
	this->WndOnKeyPress = WndOnKeyPress;
}
