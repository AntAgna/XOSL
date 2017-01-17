/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <listbox.h>
#include <graph.h>
#include <key.h>
#include <scroll.h>
#include <text.h>
#include <timer.h>

CListBox::CListBox(int Columns, int ShowHeader, int Left, int Top, int Width, int Height, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,Width,Height,Visible,false,HandlerClass)
{
	int DrawHeight;
	int RowHeight;

	ScrollBar = NULL;
	ListBoxSelect = NULL;
	Count = 0;
	ItemIndex = -1;
	this->Columns = Columns;
	this->ShowHeader = ShowHeader;
	DrawStart = 0;
	DrawHeight = Height - 4;
	if (ShowHeader)
		DrawHeight -= 20;
	RowHeight = Graph->GetTextHeight() + 1;
	if (RowHeight < 16)
		RowHeight = 16;
	DrawCount = DrawHeight / RowHeight;
	ColumnHeaders = new CStringList(Columns);
	ColumnWidth = new int[Columns];
	while (Columns)
		ColumnWidth[--Columns] = 0;
	Items = new TStrListNode;
	Items->Next = NULL;
	Items->Prev = NULL;
	DefaultIndex = -1;
	TickLastClick = GetTimerTicks();
	ListBoxDoubleClick = NULL;
	BackgroundColor = 21;
}

CListBox::~CListBox()
{
	Clear();
	delete Items;
	delete ColumnWidth;
	delete ColumnHeaders;
}

void CListBox::SetColumn(int Index, int Width, const char *HeaderName)
{
	if (Index >= Columns)
		return;
	ColumnWidth[Index] = Width;
	ColumnHeaders->Insert(Index,HeaderName);
}

void CListBox::SetShowHeader(int ShowHeader)
{
	this->ShowHeader = ShowHeader;
	Refresh();
}

int CListBox::AddRow()
{
	InsertRow(Count);
	return Count - 1;
}

void CListBox::InsertRow(int Index)
{
	TStrListNode *Entry;
	TStrListNode *NewRow;

	if (Index > Count)
		Index = Count;
	Entry = GetEntry(Index - 1);
	NewRow = new TStrListNode;
	NewRow->StringList = new CStringList(Columns);
	NewRow->Color = 17;
	NewRow->FontStyle = STYLE_REGULAR;
	NewRow->Next = Entry->Next;
	NewRow->Prev = Entry;
	if (Entry->Next)
		Entry->Next->Prev = NewRow;
	Entry->Next = NewRow;
	++Count;
	UpdateScrollBar();
}

void CListBox::DeleteRow(int Index)
{
	TStrListNode *Entry;

	if (Index >= Count)
		return;
	Entry = GetEntry(Index);
	Entry->Prev->Next = Entry->Next;
	if (Entry->Next)
		Entry->Next->Prev = Entry->Prev;
	delete Entry->StringList;
	delete Entry;
	--Count;
	UpdateScrollBar();
}

void CListBox::AddItem(int Row, int Column, const char *String)
{
	TStrListNode *Entry;

	if (Row >= Count)
		return;
	Entry = GetEntry(Row);
	Entry->StringList->Insert(Column,String);
}

CString CListBox::GetItem(int Row, int Column)
{
	TStrListNode *Entry;

	if (Row >= Count)
		return "";
	Entry = GetEntry(Row);
	return Entry->StringList->Get(Column);
}

void CListBox::Clear()
{
	TStrListNode *Entry;
	TStrListNode *Temp;

	for (Entry = Items->Next; Entry;) {
		Temp = Entry;
		Entry = Entry->Next;
		delete Temp->StringList;
		delete Temp;
	}
	Items->Next = NULL;
	Count = 0;
	ItemIndex = -1;
	DrawStart = 0;
	UpdateScrollBar();
}

void CListBox::SetItemIndex(int Index)
{
	if (Index >= Count)
		Index = -1;
	if (ItemIndex != Index) {
		ItemIndex = Index;
		if (ItemIndex < 0)
			DrawStart = 0;
		else
			if (ItemIndex < DrawStart)
				DrawStart = ItemIndex;
			else
				while (ItemIndex >= DrawStart + DrawCount)
					++DrawStart;
		Refresh();
		if (ScrollBar)
			ScrollBar->SetValue(DrawStart);
		if (ListBoxSelect && HandlerClass)
			ListBoxSelect(HandlerClass,ItemIndex);
	}
}

int CListBox::GetItemIndex()
{
	return ItemIndex;
}

int CListBox::GetCount()
{
	return Count;
}

void CListBox::SetDefault(int Index)
{
	if (DefaultIndex != Index) {
		DefaultIndex = Index;
		Refresh();
	}
}

void CListBox::SetDrawStart(int Index)
{
	if (DrawStart != Index) {
		this->DrawStart = Index;
		Refresh();
		if (ScrollBar)
			ScrollBar->SetValue(DrawStart);
	}
}

void CListBox::SetRowStyle(int Row, int Color, int FontStyle)
{
	TStrListNode *Entry;

	Entry = GetEntry(Row);
	Entry->Color = Color;
	Entry->FontStyle = FontStyle;
	Refresh();
}

void CListBox::SetBackgroundColor(int Color)
{
	BackgroundColor = Color;
}


int CListBox::GetDrawCount()
{
	return DrawCount;
}

TStrListNode *CListBox::GetEntry(int Index)
{
	TStrListNode *Entry;

	for (Entry = Items; Index >= 0; --Index)
		Entry = Entry->Next;
	return Entry;
}

void CListBox::Draw(long, long, long, long)
{
	Graph->Bar(1,1,Width - 2,Height - 2,BackgroundColor);
	DrawItems();
	DrawBody();
	if (ShowHeader)
		DrawHeader();
}

void CListBox::DrawBody()
{
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

void CListBox::DrawHeader()
{
	int Index;
	int Left, Width;
	int TextTop;

	TextTop = 20 - Graph->GetTextHeight();
	Left = 2;
	for (Index = 0; Index < Columns; ++Index) {
		Width = ColumnWidth[Index];

		if (MouseIsOver && Enabled) {
			Graph->Bar(Left + 2,4,Width - 4,16,19);
			Graph->TextOut(Left + 3,TextTop,ColumnHeaders->Get(Index),STYLE_REGULAR,17);
			Graph->HLine(Left,2,Width - 1,21);
			Graph->VLine(Left,3,18,21);
			Graph->HLine(Left + 1,3,Width - 3,20);
			Graph->VLine(Left + 1,4,16,20);
			Graph->HLine(Left + 1,20,Width - 2,18);
			Graph->HLine(Left,21,Width,17);
			Left += Width;
			Graph->VLine(Left - 2,3,17,18);
			Graph->VLine(Left - 1,2,19,17);
		}
		else {
			Graph->Bar(Left,2,Width,20,19);
			Graph->TextOut(Left + 3,TextTop,ColumnHeaders->Get(Index),STYLE_REGULAR,17);
/*			Graph->HLine(Left,2,Width - 1,21);
			Graph->VLine(Left,3,18,21);
			Graph->HLine(Left,21,Width,18);
*/			Left += Width;
//			Graph->VLine(Left - 1,2,19,18);
		}

	}
}

void CListBox::DrawItems()
{
	int Col, Row;
	long OldLeft, OldTop, OldWidth, OldHeight;
	long ClipLeft, ClipTop, ClipWidth, ClipHeight;
	int AbsLeft, AbsTop;
	TStrListNode *Entry;
	int TextStyle;

	int TextLeft, TextTop, ColWidth;
	int TextStart;
	int DrawEnd;
	int RowHeight;

	if (!Count)
		return;
	RowHeight = Graph->GetTextHeight() + 1;
	if (RowHeight < 16)
		RowHeight = 16;
	TextStart = ShowHeader ? 22 : 2;
	GetAbsPosition(AbsLeft, AbsTop);
	ClipTop = AbsTop + TextStart;
	ClipHeight = Height - 2 - TextStart;

	DrawEnd = DrawStart + DrawCount;
	TextLeft = 5;
	for (Col = 0; Col < Columns; ++Col) {
		ColWidth = ColumnWidth[Col];

		Graph->GetClippingRegion(OldLeft, OldTop, OldWidth, OldHeight);
		ClipLeft = AbsLeft + TextLeft - 3;
		ClipWidth = ColWidth;
		Graph->SetClippingRegion(ClipLeft,ClipTop,ClipWidth,ClipHeight);
		/***/

		TextTop = TextStart;
		Entry = GetEntry(DrawStart);
		for (Row = DrawStart; Entry && Row <= DrawEnd; ++Row) {
			TextStyle = Row != DefaultIndex ? Entry->FontStyle : STYLE_BOLD;
			if (Row != ItemIndex)
				Graph->TextOut(TextLeft,TextTop,Entry->StringList->Get(Col),TextStyle,Entry->Color);
			else {
				if (GotFocus) {
					Graph->Bar(TextLeft - 3,TextTop,ColWidth,RowHeight,17);
					Graph->TextOut(TextLeft,TextTop,Entry->StringList->Get(Col),TextStyle,21);
				}
				else {
					Graph->HLine(TextLeft - 3,TextTop,ColWidth,18);
					Graph->HLine(TextLeft - 3,TextTop + RowHeight - 1,ColWidth,18);
					if (Col == 0)
						Graph->VLine(TextLeft - 3,TextTop,RowHeight,18);
					if (Col == Columns - 1)
						Graph->VLine(TextLeft + ColWidth - 4,TextTop,RowHeight,18);
					Graph->TextOut(TextLeft,TextTop,Entry->StringList->Get(Col),TextStyle,Entry->Color);
				}
			}

			Entry = Entry->Next;
			TextTop += RowHeight;
		}
		/***/
		TextLeft += ColWidth;
		Graph->SetClippingRegion(OldLeft, OldTop, OldWidth, OldHeight);
	}
}

void CListBox::KeyPress(unsigned short Key)
{
	int Index;

	Index = ItemIndex;
	switch (Key) {
		case KEY_UP:
		case KEY_UP_EXT:
			if (Index > 0) {
				--Index;
				SetItemIndex(Index);
			}
			break;
		case KEY_DOWN:
		case KEY_DOWN_EXT:
			if (Index < Count - 1) {
				++Index;
				SetItemIndex(Index);
			}
			break;
		case KEY_HOME:
		case KEY_HOME_EXT:
			SetItemIndex(0);
			break;
		case KEY_END:
		case KEY_END_EXT:
			SetItemIndex(Count - 1);
			break;
		case KEY_PAGEUP:
		case KEY_PU_EXT:
			Index -= DrawCount - 1;
			if (Index < 0)
				Index = 0;
			SetItemIndex(Index);
			break;
		case KEY_PAGEDOWN:
		case KEY_PD_EXT:
			Index += DrawCount - 1;
			if (Index >= Count)
				Index = Count - 1;
			SetItemIndex(Index);
			break;
		default:
			if (WndOnKeyPress && HandlerClass)
				WndOnKeyPress(HandlerClass,Key);
			break;
	}
}

int CListBox::MouseDown(int MouseX, int MouseY)
{
	int Status;
	int Index;
	unsigned long TimerTick;
	int LastIndex;
	int RowHeight;

	Status = CControl::MouseDown(MouseX,MouseY);
	if (Status != -1 && Enabled) {
		MouseY -= Top + 1;
		if (ShowHeader)
			MouseY -= 20;
		RowHeight = Graph->GetTextHeight() + 1;
		if (RowHeight < 16)
			RowHeight = 16;
		Index = MouseY / RowHeight + DrawStart;
		if (Index < Count && MouseY > 0) {
			TimerTick = GetTimerTicks();
			LastIndex = ItemIndex;
			SetItemIndex(Index);

			// Check if this is the second click within about 250ms
			if (ListBoxDoubleClick && TimerTick - TickLastClick <= 5 && LastIndex == ItemIndex) {
				ListBoxDoubleClick(HandlerClass,ItemIndex);
			}
			TickLastClick = TimerTick;
		}
	}
	return Status;
}

void CListBox::SetScrollBar(CScrollBar *ScrollBar)
{
	this->ScrollBar = ScrollBar;
	ScrollBar->SetOwner(this);
	ScrollBar->OnChange((TScrollChange)ScrollBarChange);
	UpdateScrollBar();
}

void CListBox::ScrollBarChange(CListBox *ListBox, int Value)
{
	ListBox->SetDrawStart(Value);
}

void CListBox::UpdateScrollBar()
{
	int ScrollCount;

	if (!ScrollBar)
		return;
	ScrollCount = Count - DrawCount;
	if (ScrollCount <= 0)
		ScrollBar->Disable();
	else {
//		ScrollBar->Parent->SetVisible(false);
		ScrollBar->SetMin(0);
		ScrollBar->SetMax(ScrollCount);
		ScrollBar->SetValue(DrawStart);
		ScrollBar->Enable();

//		ScrollBar->Parent->SetVisible(true);
//		ScrollBar->Refresh();
	}
}

void CListBox::OnSelect(TListBoxSelect ListBoxSelect)
{
	this->ListBoxSelect = ListBoxSelect;
}

void CListBox::OnDoubleClick(TListBoxSelect ListBoxDoubleClick)
{
	this->ListBoxDoubleClick = ListBoxDoubleClick;
}

void CListBox::FontChanged()
{
	int DrawHeight;
	int RowHeight;

	DrawHeight = Height - 4;
	if (ShowHeader)
		DrawHeight -= 20;
	RowHeight = Graph->GetTextHeight() + 1;
	if (RowHeight < 16)
		RowHeight = 16;
	DrawCount = DrawHeight / RowHeight;
	UpdateScrollBar();
}
