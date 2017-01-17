/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <form.h>
#include <graph.h>
#include <key.h>

int CForm::FrameMove = 1;


CForm::CForm(const char *Caption, int Style, int OnTop, int Left, int Top,
	int Width, int Height, int Visible):
	CControl(Left,Top,Width,Height,Visible,OnTop,true,NULL)
{

	this->Caption = Caption;
	this->Style = Style;
	ButtonDown = false;

	Controls = new CWindowList();
	Controls->SetHandler(this);
	Controls->SetVisible(false);
   IsDrawn = false;

	if (Style == FORM_NORMAL) {
		Controls->SetPosition(Left + 2,Top + 22);
		Controls->SetMetrics(Width - 4,Height - 24);
	}
	else {
		Controls->SetPosition(Left,Top);
		Controls->SetMetrics(Width,Height);
	}

	IgnoreAfter = false;
}

CForm::~CForm()
{
	delete Controls;
}

void CForm::Show()
{
	if (Visible)
		return;
	Controls->SetVisible(true);
	Parent->SetFocusWnd(this,true);
	CControl::Show();
}

void CForm::Hide()
{
	if (!Visible)
		return;
	Controls->SetVisible(false);
	CControl::Hide();
}

void CForm::SetMetrics(int Width, int Height)
{
	if (Style == FORM_NORMAL)
		Controls->SetMetrics(Width - 4,Height - 24);
	else
		Controls->SetMetrics(Width,Height);
	CControl::SetMetrics(Width,Height);
}

void CForm::SetPosition(int Left, int Top)
{
	if (Style == FORM_NORMAL)
		Controls->SetPosition(Left + 2,Top + 22);
	else
		Controls->SetPosition(Left,Top);
	CControl::SetPosition(Left,Top);
}

void CForm::FixDamage(int Left, int Top, int Width, int Height)
{
	int iAbsLeft, iAbsTop;
//	long AbsLeft, AbsTop;
	long VPLeft, VPTop;
	long ClipLeft,ClipTop,ClipWidth,ClipHeight;
//	long lLeft, lTop, lWidth, lHeight;

	if (!IsDrawn) {
		IsDrawn = true;
		Controls->SetVisible(true);
	}
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


		IgnoreAfter = true;
		if (Style == FORM_NORMAL)
			Controls->FixDamage(Left - 2,Top - 22,Width,Height);
		else
			Controls->FixDamage(Left,Top,Width,Height);
		IgnoreAfter = false;
	}
}

void CForm::BeforeFix(int Left, int Top, int Width, int Height)
{
	if (Style == FORM_NORMAL)
		Graph->Bar(Left,Top,Width,Height,19);
}

void CForm::AfterFix(int Left, int Top, int Width, int Height)
{
	if ((!OnTop || Parent->GetFocusWnd() != this) && !IgnoreAfter)
		Parent->FixDamage(Left,Top,Width,Height);
}

int CForm::MouseDown(int MouseX, int MouseY)
{
	int Status;

	if (!Enabled && MouseX >= Left && MouseX <= Right && MouseY >= Top && MouseY <= Bottom)
		return 0;

	Status = CControl::MouseDown(MouseX,MouseY);
	if (Status != -1 && Enabled)
		if (Style == FORM_NORMAL &&
			 MouseX >= Left && MouseX <= Right &&
			 MouseY >= Top && MouseY <= Top + 21) {
			/**/
			ButtonDown = true;

			if (FrameMove) {
				DistX = Left - MouseX;
				DistY = Top - MouseY;

				LeftFrame = new char[3 * Height + 6];
				RightFrame = new char[3 * Height+ 6];
				TopFrame = new char[3 * Width+ 6];
				BottomFrame = new char[3 * Width+ 6];

				oMoveLeft = MoveLeft = MouseX + DistX;
				oMoveTop = MoveTop = MouseY + DistY;
				oMoveRight = MoveRight = MoveLeft + Width - 1;
				oMoveBottom = MoveBottom = MoveTop + Height - 1;

            Graph->HideCursor();
				ShowFrame();
			}
			/**/
		}
		else
			Controls->MouseDown(MouseX,MouseY);
	return Status;
}

int CForm::MouseMove(int X, int Y)
{
	if (ButtonDown) {
		/**/

		if (!FrameMove) {
			SetPosition(Left + X - MouseX,Top + Y - MouseY);
		}
		else {
			HideFrame();
			MoveLeft = X + DistX;
			MoveTop = Y  + DistY;
			MoveRight = MoveLeft + Width - 1;
			MoveBottom = MoveTop + Height - 1;
			ShowFrame();
		}
		CControl::MouseMove(X,Y);
		return 0;
		/**/
	}

	CControl::MouseMove(X,Y);

	if (GotFocus) {
		Controls->MouseMove(X,Y);
		return 0;
	}
	return -1;
}

void CForm::MouseUp()
{
	if (ButtonDown) {
		/**/
		ButtonDown = false;
		if (FrameMove) {
			HideFrame();
			FlushFrame();
			SetPosition(MouseX + DistX,MouseY + DistY);
			Graph->ShowCursor();
			delete LeftFrame;
			delete RightFrame;
			delete TopFrame;
			delete BottomFrame;
		}
		/**/
	}
	else
		Controls->MouseUp();
}


//void CForm::Draw(long Left, long Top, long Width, long Height)
void CForm::Draw(long, long, long, long)
{
	int lTB;
	int TextTop;

	if (Style == FORM_TRANSP)
		return;
	Graph->HLine(0,0,Width - 1,20);
	Graph->VLine(0,1,Height - 2,20);

	Graph->HLine(1,1,Width - 2,21);
	Graph->VLine(1,2,Height - 3,21);

	Graph->HLine(0,Height - 1,Width,17);
	Graph->VLine(Width - 1,0,Height - 1,17);

	Graph->HLine(1,Height - 2,Width - 2,18);
	Graph->VLine(Width - 2,1,Height - 3,18);

	Graph->VLine(2,2,20,19);
	Graph->VLine(Width - 3,2,20,19);

	Graph->VLine(3,3,18,18);
	Graph->HLine(3,2,Width - 7,18);

	Graph->VLine(Width - 4,3,19,21);
	Graph->HLine(3,21,Width - 6,21);

	TextTop = 22 - Graph->GetTextHeight() >> 1;
	if (GotFocus) {
		for (lTB = 3; lTB < 21; ++lTB)
			Graph->HLine(4,lTB,Width - 8,lTB + 19);
		Graph->TextOut(7,TextTop,Caption,STYLE_BOLD,21);
	}
	else {
		Graph->Bar(4,3,Width - 8,18,18);
		Graph->TextOut(7,TextTop,Caption,STYLE_BOLD,19);
	}

}

void CForm::AddControl(CControl *Control)
{
	Controls->Add(Control);
}

void CForm::RemoveControl(CControl *Control)
{
	Controls->Remove(Control);
}

void CForm::FocusControl(CControl *Control)
{
	Controls->SetFocusWnd(Control,false);
}

void CForm::KeyPress(unsigned short Key)
{
	CControl *Wnd;

	switch (Key) {
		case KEY_TAB:
			Controls->FocusNext();
			break;
		case KEY_STAB:
			Controls->FocusPrev();
			break;
		default:
			Wnd = Controls->GetFocusWnd();
			if (Wnd && Wnd->Visible)
				Wnd->KeyPress(Key);
			break;
	}
}

void CForm::ShowFrame()
{
	Graph->GetImage(MoveLeft - 1,MoveTop - 1,3,Height + 2,LeftFrame);
	Graph->GetImage(MoveLeft - 1,MoveTop - 1,Width + 2,3,TopFrame);
	Graph->GetImage(MoveRight - 1,MoveTop - 1,3,Height + 2,RightFrame);
	Graph->GetImage(MoveLeft - 1,MoveBottom - 1,Width + 2,3,BottomFrame);

	Graph->Rectangle(MoveLeft,MoveTop,Width,Height,15);
	Graph->Rectangle(MoveLeft - 1,MoveTop - 1,Width + 2,Height + 2,0);
	Graph->Rectangle(MoveLeft  + 1,MoveTop + 1,Width - 2,Height - 2,0);
	FlushFrame();
}

void CForm::HideFrame()
{
	Graph->PutImage(MoveLeft - 1,MoveTop - 1,3,Height + 2,LeftFrame);
	Graph->PutImage(MoveLeft - 1,MoveTop - 1,Width + 2,3,TopFrame);
	Graph->PutImage(MoveRight - 1,MoveTop - 1,3,Height + 2,RightFrame);
	Graph->PutImage(MoveLeft - 1,MoveBottom - 1,Width + 2,3,BottomFrame);

	oMoveLeft = MoveLeft;
	oMoveTop = MoveTop;
	oMoveRight = MoveRight;
	oMoveBottom = MoveBottom;
}

void CForm::FlushFrame()
{

	Graph->FlushArea(MoveLeft - 1,MoveTop - 1,3,Height + 2);
	Graph->FlushArea(MoveLeft - 1,MoveTop - 1,Width + 2,3);
	Graph->FlushArea(MoveRight - 1,MoveTop - 1,3,Height + 2);
	Graph->FlushArea(MoveLeft - 1,MoveBottom - 1,Width + 2,3);

	Graph->FlushArea(oMoveLeft - 1,oMoveTop - 1,3,Height + 2);
	Graph->FlushArea(oMoveLeft - 1,oMoveTop - 1,Width + 2,3);
	Graph->FlushArea(oMoveRight - 1,oMoveTop - 1,3,Height + 2);
	Graph->FlushArea(oMoveLeft - 1,oMoveBottom - 1,Width + 2,3);

//	Graph->WaitRetrace();
}
