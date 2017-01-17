/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <radiogrp.h>
#include <graph.h>
#include <key.h>
#include <scroll.h>

void printf(const char *,...);

CRadioGroup::CRadioGroup(int Left, int Top, int Width, int Height, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,Width,Height,Visible,false,HandlerClass)
{
	Count = 0;
	ButtonList.Next = NULL;
	LastButton = &ButtonList;
	ItemIndex = 0;

	ScrollBar = NULL;
	RadioGroupSelect = NULL;

	DrawStart = 0;
	DrawCount = Height / 24;
	HighlightColor = 17;
	AniButtonIndex = -1;
}

CRadioGroup::~CRadioGroup()
{
	Clear();
}

void CRadioGroup::AddButton(const char *Caption)
{
	TButtonNode *NewNode;

	NewNode = LastButton->Next = new TButtonNode;
	NewNode->Caption = new CString(Caption);
	NewNode->Prev = LastButton;
	NewNode->Next = NULL;
	LastButton = NewNode;
	++Count;
	UpdateScrollBar();
}

void CRadioGroup::DeleteButton(int ButtonIndex)
{
	int Index;
	TButtonNode *Node;

	if (ButtonIndex >= Count)
		return;
	for (Node = ButtonList.Next, Index = 0; Index < ButtonIndex; ++Index)
		Node = Node->Next;
	Node->Prev->Next = Node->Next;
	if (Node->Next)
		Node->Next->Prev = Node->Prev;
	else
		LastButton = LastButton->Prev;
	delete Node->Caption;
	delete Node;
	--Count;
	DrawStart = 0;
	ItemIndex = 0;
	UpdateScrollBar();
}

void CRadioGroup::Clear()
{
	TButtonNode *Node;
	TButtonNode *DelNode;

	for (Node = ButtonList.Next; Node;) {
		DelNode = Node;
		Node = Node->Next;
		delete DelNode->Caption;
		delete DelNode;
	}
	ButtonList.Next = NULL;
	LastButton = &ButtonList;
	Count = 0;
	DrawStart = 0;
	ItemIndex = 0;
	UpdateScrollBar();
}

void CRadioGroup::SetChecked(int Index)
{
	if (!Count)
		return;
	if (Index >= Count)
		Index = Count - 1;
	if (ItemIndex != Index) {
		ItemIndex = Index;
		if (ItemIndex < DrawStart)
			DrawStart = ItemIndex;
		else
			while (ItemIndex >= DrawStart + DrawCount)
				++DrawStart;
		Refresh();
		if (ScrollBar)
			ScrollBar->SetValue(DrawStart);
		if (RadioGroupSelect && HandlerClass)
			RadioGroupSelect(HandlerClass,ItemIndex);
	}

}

int CRadioGroup::GetChecked()
{
	return ItemIndex;
}

int CRadioGroup::GetButtonCount()
{
	return Count;
}

void CRadioGroup::SetHighlight(int Status)
{
	HighlightColor = Status ? 22 : 17;
}


void CRadioGroup::SetDrawStart(int Index)
{
	if (DrawStart != Index && Index < Count) {
		DrawStart = Index;
		Refresh();
		if (ScrollBar)
			ScrollBar->SetValue(DrawStart);
	}
}

void CRadioGroup::Draw(long, long, long, long)
{
	int ButtonTop, ButtonHeight;
	TButtonNode *Node;
	int Index;
	int ButtonFocus;
	int Checked;

	if (!Count)
		return;
	ButtonHeight = Graph->GetTextHeight() + 2;
	for (Node = ButtonList.Next, Index = 0; Index < DrawStart; ++Index)
		Node = Node->Next;
	for (ButtonTop = 0, Index = 0;
		  Node && Index < DrawCount;
		  ++Index, ButtonTop += 24, Node = Node->Next) {
		Checked = (Index + DrawStart) == ItemIndex;
		ButtonFocus = GotFocus && Checked;
		DrawButton(ButtonTop,ButtonHeight,ButtonFocus,Checked,*Node->Caption);
	}
}

void CRadioGroup::DrawButton(int ButtonTop, int ButtonHeight, int GotFocus,
	int Checked, const char *Caption)
{
	int CheckedColor;
	bool DrawMouseOver;

	if (Enabled) {
		Graph->TextOut(19,ButtonTop + 1,Caption,STYLE_REGULAR,Checked ? HighlightColor : 17);
		CheckedColor = 17;
	}
	else {
		Graph->TextOut(20,ButtonTop + 2,Caption,STYLE_REGULAR,21);
		Graph->TextOut(19,ButtonTop + 1,Caption,STYLE_REGULAR,18);
		CheckedColor = 18;
	}
	if (GotFocus)
		Graph->Rectangle(17,ButtonTop,GetFocusWidth(Caption),ButtonHeight,18);

	DrawMouseOver = ButtonTop == AniButtonIndex && Enabled;
	ButtonTop += (ButtonHeight - 13) >> 1;

	if (DrawMouseOver) {
		Graph->Bar(3,ButtonTop + 4,8,9,21);
		Graph->HLine(5,ButtonTop + 13,4,21);
		Graph->VLine(11,ButtonTop + 4,8,21);
		Graph->VLine(12,ButtonTop + 6,4,21);

		Graph->HLine(5,ButtonTop + 2,4,18);
		Graph->HLine(3,ButtonTop + 3,8,18);
		Graph->VLine(2,ButtonTop + 4,8,18);
		Graph->VLine(1,ButtonTop + 6,4,18);

		Graph->HLine(5,ButtonTop + 3,4,17);
		Graph->HLine(3,ButtonTop + 4,2,17);
		Graph->HLine(9,ButtonTop + 4,2,17);
		Graph->VLine(2,ButtonTop + 6,4,17);
		Graph->PutPixel(3,ButtonTop + 5,17);
		Graph->PutPixel(3,ButtonTop + 10,17);

		Graph->HLine(3,ButtonTop + 11,2,20);
		Graph->HLine(5,ButtonTop + 12,4,20);
		Graph->HLine(9,ButtonTop + 11,2,20);
		Graph->VLine(11,ButtonTop + 6,4,20);
		Graph->PutPixel(10,ButtonTop + 5,20);
		Graph->PutPixel(10,ButtonTop + 10,20);
	}
	else {
		Graph->Bar(3,ButtonTop + 4,8,8,21);

		Graph->HLine(5,ButtonTop + 2,4,17);
		Graph->HLine(3,ButtonTop + 3,8,17);
		Graph->VLine(2,ButtonTop + 4,8,17);
		Graph->VLine(1,ButtonTop + 6,4,17);

		Graph->HLine(3,ButtonTop + 12,8,18);
		Graph->HLine(5,ButtonTop + 13,4,18);
		Graph->VLine(11,ButtonTop + 4,8,18);
		Graph->VLine(12,ButtonTop + 6,4,18);

		Graph->HLine(5,ButtonTop + 3,4,21);
		Graph->HLine(5,ButtonTop + 12,4,21);
		Graph->VLine(2,ButtonTop + 6,4,21);
		Graph->VLine(11,ButtonTop + 6,4,21);
	}


	if (Checked) {
		Graph->HLine(6,ButtonTop + 6,2,CheckedColor);
		Graph->HLine(5,ButtonTop + 7,4,CheckedColor);
		Graph->HLine(5,ButtonTop + 8,4,CheckedColor);
		Graph->HLine(6,ButtonTop + 9,2,CheckedColor);
	}
}

int CRadioGroup::GetFocusWidth(const char *Caption)
{
	int WndFocusWidth;
	int TextFocusWidth;

	WndFocusWidth = Width - 17;
	TextFocusWidth = Graph->GetTextWidth(Caption,STYLE_REGULAR) + 4;
	return WndFocusWidth < TextFocusWidth ? WndFocusWidth : TextFocusWidth;
}


void CRadioGroup::KeyPress(unsigned short Key)
{
	int Index;

	if (WndOnKeyPress && HandlerClass)
		WndOnKeyPress(HandlerClass,Key);	
	Index = ItemIndex;
	switch (Key) {
		case KEY_NONE:
			return;
		case KEY_UP:
		case KEY_UP_EXT:
			if (Index > 0) {
				--Index;
				SetChecked(Index);
			}
			break;
		case KEY_DOWN:
		case KEY_DOWN_EXT:
			if (Index < Count - 1) {
				++Index;
				SetChecked(Index);
			}
			break;
		case KEY_HOME:
		case KEY_HOME_EXT:
			SetChecked(0);
			break;
		case KEY_END:
		case KEY_END_EXT:
			SetChecked(Count - 1);
			break;
		case KEY_PAGEUP:
		case KEY_PU_EXT:
			Index -= DrawCount - 1;
			if (Index < 0)
				Index = 0;
			SetChecked(Index);
			break;
		case KEY_PAGEDOWN:
		case KEY_PD_EXT:
			Index += DrawCount - 1;
			if (Index >= Count)
				Index = Count - 1;
			SetChecked(Index);
			break;
		default:
			break;
	}
}

int CRadioGroup::MouseDown(int MouseX, int MouseY)
{
	int Status;
	int Index;

	Status = CControl::MouseDown(MouseX,MouseY);
	if (Status != -1 && Enabled) {
		MouseY -= Top;
		Index = MouseY / 24 + DrawStart;
		if (Index < Count && MouseY > 0)
			SetChecked(Index);
	}
	return Status;
}


int CRadioGroup::MouseMove(int X, int Y)
{
	int NewOffset;

	if (!Enabled) {
		return -1;
	}
	CControl::MouseMove(X,Y);
	if (MouseX >= Left && MouseX <= Right && MouseY >= Top && MouseY <= Bottom) {
		NewOffset = ((MouseY - Top) / 24) * 24;
		if (Animate && AniButtonIndex != NewOffset) {
			AniButtonIndex = NewOffset;
			MouseOver();
		}
//		return 0;
		return -1;
	}
	else {
		if (AniButtonIndex != -1) {
			AniButtonIndex = -1;
			MouseOut();
		}
	}
	return -1;
}


void CRadioGroup::SetScrollBar(CScrollBar *ScrollBar)
{
	this->ScrollBar = ScrollBar;
	ScrollBar->SetOwner(this);
	ScrollBar->OnChange((TScrollChange)ScrollBarChange);
	UpdateScrollBar();
}

void CRadioGroup::ScrollBarChange(CRadioGroup *RadioGroup, int Value)
{
	RadioGroup->SetDrawStart(Value);
}

void CRadioGroup::UpdateScrollBar()
{
	int ScrollCount;

	if (!ScrollBar)
		return;
	ScrollCount = Count - DrawCount;
	if (ScrollCount <= 0)
		ScrollBar->Hide();
	else {
//		ScrollBar->Parent->SetVisible(false);
		ScrollBar->SetMin(0);
		ScrollBar->SetMax(ScrollCount);
		ScrollBar->SetValue(DrawStart);
		ScrollBar->Show();

//		ScrollBar->Parent->SetVisible(true);
//		ScrollBar->Refresh();
	}
}

void CRadioGroup::OnSelect(TRadioGroupSelect RadioGroupSelect)
{
	this->RadioGroupSelect = RadioGroupSelect;
}
