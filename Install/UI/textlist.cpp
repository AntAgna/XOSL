/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <textlist.h>
#include <key.h>
#include <string.h>


#ifndef NULL
#define NULL ((void *)0)
#endif

CTextList::CTextList(int Left, int Top, int Width, int Height, CTextScreen &TextScreenToUse, CTextBox &TextBoxToUse):
	TextScreen(TextScreenToUse), TextBox(TextBoxToUse)
{
	int Index;

	ItemIndex = 0;
	FirstDraw = 1;
	this->Left = Left;
	this->Top = Top;
	this->Width = Width;
	this->Height = Height;
	ListItems = new CListItem[Height];
	for (Index = 0; Index < Height; ++Index)
		ListItems[Index].Type = tltNone;
	ItemIndex = 0;
	EventHandler = NULL;
}

CTextList::~CTextList()
{
	Clear();
	delete ListItems;
}

void CTextList::AddItem(int Index, const char *Caption, const char *HelpText, int Enabled)
{
	CListItem *Node;

	Node = &ListItems[Index];
	Node->Type = tltAction;
	Node->Caption = new char [strlen(Caption) + 1];
	strcpy(Node->Caption,Caption);
	if (HelpText) {
		Node->HelpText = new char [strlen(HelpText) + 1];
		strcpy(Node->HelpText,HelpText);
	}
	else
		Node->HelpText = NULL;
	Node->Enabled = Enabled;
}

void CTextList::AddItem(int Index, const char *Caption, const char *HelpText,
	int Enabled, int OptionCount, const char **OptionList, int IsCombo)
{
	CListItem *Node;
	int ItemWidth, MaxWidth;
	char **OptionNode;

	AddItem(Index,Caption,HelpText,Enabled);
	Node = &ListItems[Index];
	Node->Type = IsCombo ? tltCombo : tltList;
	Node->OptionCount = OptionCount;
	Node->OptionList = new char *[OptionCount];
	OptionNode = Node->OptionList;
	for (MaxWidth = Index = 0; Index < OptionCount; ++Index, ++OptionNode) {
		ItemWidth = strlen(OptionList[Index]);
		*OptionNode = new char [ItemWidth + 1];
		strcpy(*OptionNode,OptionList[Index]);
		if (ItemWidth > MaxWidth)
			MaxWidth = ItemWidth;
	}
	
	Node->OptionWidth = MaxWidth;
	Node->OptionIndex = 0;
}

void CTextList::Refresh()
{
	int Index;

	if (FirstDraw) {
		for (Index = 0; Index < Height; ++Index) 
			TextScreen.FillX(Left,Top + Index,' ',0x1f00,Width);
		FirstDraw = 0;
	}
   GetMaxOptionWidth();
	for (Index = 0; Index < Height; ++Index) {
		switch (ListItems[Index].Type) {
			case tltNone:
				break;
			case tltAction:
				DrawActionItem(Index);
				break;
			case tltCombo:
				DrawOptionItem(Index);
				break;
			case tltList:
				DrawListItem(Index);
				break;
			default:
				break;
		}
	}
}

void CTextList::Clear()
{
	int Index;
	int OptIndex;
	CListItem *Item;

	for (Index = 0; Index < Height; ++Index) {
		Item = &ListItems[Index];
		if (Item->Type != tltNone) {
			if (Item->Type == tltCombo || Item->Type == tltList) {
				for (OptIndex = 0; OptIndex < Item->OptionCount; ++OptIndex)
					delete Item->OptionList[OptIndex];
				delete Item->OptionList;
			}
			delete Item->Caption;
			delete Item->HelpText;
			Item->Type = tltNone;
		}
	}
	ItemIndex = 0;
	FirstDraw = 1;
}

void CTextList::SetItemIndex(int ItemIndex)
{
	this->ItemIndex = ItemIndex;
}


int CTextList::GetOptionIndex(int Index)
{
	return ListItems[Index].OptionIndex;
}


void CTextList::ConnectEventHandler(CTextList::TListItemExecute Handler, void *_this)
{
	EventHandler = Handler;
	Event_this = _this;
}


void CTextList::HandleKeyAction(int Key)
{
	CListItem *Item;

	switch (Key) {
		case KEY_UP:
			do {
				--ItemIndex;
				if (ItemIndex < 0)
					ItemIndex = Height - 1;
			} while (ListItems[ItemIndex].Type == tltNone ||
						!ListItems[ItemIndex].Enabled);
			Refresh();
			break;
		case KEY_DOWN:
			do {
				++ItemIndex;
				if (ItemIndex == Height)
					ItemIndex = 0;
			} while (ListItems[ItemIndex].Type == tltNone ||
						!ListItems[ItemIndex].Enabled);
			Refresh();
			break;
		case KEY_PAGEUP:
		case KEY_MIN:
			Item = &ListItems[ItemIndex];
			if (Item->Type == tltCombo) {
				if (--Item->OptionIndex < 0)
					Item->OptionIndex = Item->OptionCount - 1;
				Refresh();
			}
			else {
				if (Item->Type == tltList) {
					if (--Item->OptionIndex < 0)
						++Item->OptionIndex;
					Refresh();
				}
			}
			break;
		case KEY_PAGEDOWN:
		case KEY_PLUS:
			Item = &ListItems[ItemIndex];
			if (Item->Type == tltCombo) {
				if (++Item->OptionIndex == Item->OptionCount)
					Item->OptionIndex = 0;
				Refresh();
			}
			else {
				if (Item->Type == tltList) {
					if (++Item->OptionIndex == Item->OptionCount)
						--Item->OptionIndex;
					Refresh();
				}
			}

			break;
		case KEY_ENTER:
			if (ListItems[ItemIndex].Type == tltAction && EventHandler)
				EventHandler(Event_this,ItemIndex);
			break;
		default:
			break;
	}
}

void CTextList::GetMaxOptionWidth()
{
	int Index;
	CListItem *Item;

	MaxOptionWidth = 0;
	for (Index = 0; Index < Height; ++Index) {
		Item = &ListItems[Index];
		if (Item->Type == tltCombo && Item->OptionWidth > MaxOptionWidth)
			MaxOptionWidth = Item->OptionWidth;
	}
}

void CTextList::DrawActionItem(int Index)
{
	CListItem &Item = ListItems[Index];
	int Attr;

	if (!Item.Enabled)
		Attr = 0x1f00;
	else {
		if (ItemIndex == Index) {
			Attr = 0x4f00;
			TextBox.SetText(Item.HelpText);
		}
		else
			Attr = 0x1e00;
	}
	TextScreen.PutStr(Left,Top + Index,Item.Caption,Attr);
}

void CTextList::DrawOptionItem(int Index)
{
	CListItem &Item = ListItems[Index];
	int Attr;
	int OptionLeft;
	int OptionTop;

	if (!Item.Enabled)
		Attr = 0x1f00;
	else {
		if (ItemIndex == Index) {
			Attr = 0x4f00;
			TextBox.SetText(Item.HelpText);
		}
		else
			Attr = 0x1e00;
	}
	OptionLeft = Left + Width - MaxOptionWidth;
	OptionTop = Top + Index;
	TextScreen.PutStr(Left,OptionTop,Item.Caption,0x1f00);
	TextScreen.PutChar(OptionLeft - 2,OptionTop,':',0x1f00);
	TextScreen.FillX(OptionLeft,OptionTop,' ',Attr,Item.OptionWidth);
	TextScreen.PutStr(OptionLeft,OptionTop,Item.OptionList[Item.OptionIndex],Attr);
}


void CTextList::DrawListItem(int Index)
{
	CListItem &Item = ListItems[Index];
	int SelectAttr, Attr, TitleAttr;
	int OptionTop;
	int OptionIndex;
	
	if (!Item.Enabled) {
		SelectAttr = 0x1f00;
		Attr = 0x1700;
		TitleAttr = 0x1700;
	}
	else
		if (ItemIndex == Index) {
			SelectAttr = 0x0f00;
			Attr = 0x1f00;
			TitleAttr = 0x4f00;
			TextBox.SetText(Item.HelpText);
		}
		else {
			SelectAttr = 0x1e00;
			Attr = 0x1700;
			TitleAttr = 0x1f00;
		}
	OptionTop = Top + Index;
	OptionIndex = Item.OptionIndex;
	TextScreen.PutStr(Left,OptionTop,Item.Caption,TitleAttr);

	TextScreen.FillX(Left,OptionTop + 1,' ',Attr,Item.OptionWidth);
	if (OptionIndex)
		TextScreen.PutStr(Left,OptionTop + 1,Item.OptionList[OptionIndex - 1],Attr);
	
	TextScreen.FillX(Left,OptionTop + 2,' ',SelectAttr,Item.OptionWidth);
	TextScreen.PutStr(Left,OptionTop + 2,Item.OptionList[OptionIndex],SelectAttr);
	
	TextScreen.FillX(Left,OptionTop + 3,' ',Attr,Item.OptionWidth);
	if (OptionIndex < Item.OptionCount - 1)
		TextScreen.PutStr(Left,OptionTop + 3,Item.OptionList[OptionIndex + 1],Attr);
}
