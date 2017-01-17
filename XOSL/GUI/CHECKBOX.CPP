/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <checkbox.h>
#include <graph.h>
#include <key.h>

CCheckBox::CCheckBox(const char *Caption, int Checked, int Left, int Top, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,Graph->GetTextWidth(Caption,STYLE_REGULAR) + 21, Graph->GetTextHeight() + 2,Visible,false,HandlerClass)
{
	this->Caption = Caption;
	this->Checked = Checked;
	AutoSize = true;
	CheckBoxChange = NULL;
}

CCheckBox::CCheckBox(const char *Caption, int Checked, int Left, int Top, int Width, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,Width,Graph->GetTextHeight() + 2,Visible,false,HandlerClass)
{
	this->Caption = Caption;
	this->Checked = Checked;
	AutoSize = false;
}

CCheckBox::~CCheckBox()
{
}

void CCheckBox::SetCaption(const char *Caption)
{
	if (AutoSize) {
		this->Caption = Caption;
		SetMetrics(Graph->GetTextWidth(Caption,STYLE_REGULAR) + 21,Height);
	}
	else
		CControl::SetCaption(Caption);
}

void CCheckBox::SetAutoSize(int AutoSize)
{
	this->AutoSize = AutoSize;
	if (AutoSize)
			SetMetrics(Graph->GetTextWidth(Caption,STYLE_REGULAR) + 21,Height);
}

void CCheckBox::SetChecked(int Checked)
{
	if (Checked != this->Checked) {
		this->Checked = Checked;
		Refresh();
		if (CheckBoxChange && HandlerClass)
			CheckBoxChange(HandlerClass,Checked);
	}
}

int CCheckBox::IsChecked()
{
	return Checked;
}

int CCheckBox::MouseDown(int MouseX, int MouseY)
{
	int Status;

	Status = CControl::MouseDown(MouseX,MouseY);
	if (Status != -1 && Enabled)
		SetChecked(!Checked);
	return Status;
}

void CCheckBox::KeyPress(unsigned short Key)
{
	if (Enabled)
		switch (Key) {
			case KEY_ENTER:
			case KEY_K_ENTER:
			case KEY_SPACE:
				SetChecked(!Checked);
				break;
		}
}

void CCheckBox::Draw(long, long, long, long)
{
	int BoxTop;
	int CheckedColor;

	BoxTop = Height - 13 >> 1;

	Graph->Bar(0,BoxTop,13,13,21);
	Graph->HLine(0,BoxTop,12,18);
	Graph->VLine(0,BoxTop + 1,11,18);


	if (Enabled) {
		if (MouseIsOver) {
			Graph->HLine(1,BoxTop + 1,10,17);
			Graph->VLine(1,BoxTop + 2,9,17);
			Graph->HLine(1,BoxTop + 11,11,20);
			Graph->VLine(11,BoxTop + 1,10,20);
		}
		Graph->TextOut(19,1,Caption,STYLE_REGULAR,0);
		CheckedColor = 0;
	}
	else {
		Graph->TextOut(20,2,Caption,STYLE_REGULAR,21);
		Graph->TextOut(19,1,Caption,STYLE_REGULAR,18);
		CheckedColor = 18;
	}

	if (Checked) {
		Graph->VLine(3,BoxTop + 5,3,CheckedColor);
		Graph->VLine(4,BoxTop + 6,3,CheckedColor);
		Graph->VLine(5,BoxTop + 7,3,CheckedColor);
		Graph->VLine(6,BoxTop + 6,3,CheckedColor);
		Graph->VLine(7,BoxTop + 5,3,CheckedColor);
		Graph->VLine(8,BoxTop + 4,3,CheckedColor);
		Graph->VLine(9,BoxTop + 3,3,CheckedColor);
	}
	if (GotFocus)
		Graph->Rectangle(17,0,Width - 17,Height,18);

}

void CCheckBox::OnChange(TCheckBoxChange CheckBoxChange)
{
	this->CheckBoxChange = CheckBoxChange;
}
