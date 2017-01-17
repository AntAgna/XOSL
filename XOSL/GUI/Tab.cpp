/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <tab.h>
#include <graph.h>
#include <key.h>

CTabControl::CTabControl(int TabCount, int Left, int Top, int Width, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,Width,24,Visible,false,HandlerClass)
{
	int Index;

	TabArray = new TTabArray[TabCount];
	this->TabCount = TabCount;
	ActiveTab = 0;
	for (Index = 0; Index < TabCount; ++Index) {
		TabArray[Index].TabList = new TTabList;
		TabArray[Index].TabList->Next = NULL;
		TabArray[Index].Caption = new CString;
	}
	TabChange = NULL;
}

CTabControl::~CTabControl()
{
	int Index;
	TTabList *Entry;
	TTabList *Temp;

	for (Index = 0; Index < TabCount; ++Index) {
		Entry = TabArray[Index].TabList;
		while (Entry) {
			Temp = Entry;
			Entry = Entry->Next;
			delete Temp;
		}
	}
	delete TabArray;
}

void CTabControl::SetTabCaption(int TabIndex, const char *Caption)
{
	int TabWidth;

	if (TabIndex >= TabCount)
		return;
	*TabArray[TabIndex].Caption = Caption;
	TabWidth = Graph->GetTextWidth(Caption,STYLE_REGULAR) + 14;
	if (TabWidth < 54)
		TabWidth = 54;
	TabArray[TabIndex].Width = TabWidth;
}

void CTabControl::AddControl(int TabIndex, CControl *Window)
{
	TTabList *Entry;

	if (TabIndex >= TabCount)
		return;
	for (Entry = TabArray[TabIndex].TabList; Entry->Next; Entry = Entry->Next);
	Entry = Entry->Next = new TTabList;
	Entry->Window = Window;
	Entry->Next = NULL;
}

int CTabControl::GetActiveTab()
{
	return ActiveTab;
}

void CTabControl::SetActiveTab(int TabIndex)
{
	if (TabIndex < 0)
		TabIndex = 0;
	else
		if (TabIndex >= TabCount)
			TabIndex = TabCount - 1;
	if (TabIndex != ActiveTab) {
		SetWndVisible(false);
		ActiveTab = TabIndex;
		SetWndVisible(true);
		Refresh();
		if (TabPage)
			TabPage->Refresh();
		if (TabChange && HandlerClass)
			TabChange(HandlerClass,ActiveTab);
	}
}

void CTabControl::SetTabPage(CControl *TabPage)
{
	this->TabPage = TabPage;
}

void CTabControl::SetWndVisible(int Visible)
{
	TTabList *Entry;

	for (Entry = TabArray[ActiveTab].TabList->Next; Entry; Entry = Entry->Next)
		Entry->Window->SetVisible(Visible);
}

void CTabControl::Draw(long, long, long, long)
{
	int Index;
	int TabLeft, TabWidth;
	int ActiveLeft, ActiveWidth;
	const char *ActiveCaption;

	TabLeft = 2;
	Graph->HLine(0,0,Width,18);
	for (Index = 0; Index < TabCount; ++Index) {
		TabWidth = TabArray[Index].Width;
		if (Index != ActiveTab)
			DrawTab(TabLeft,1,TabWidth,21,false,*TabArray[Index].Caption);
		else {
			ActiveLeft = TabLeft - 2;
			ActiveWidth = TabWidth + 4;
			ActiveCaption = *TabArray[Index].Caption;
		}
		TabLeft += TabWidth;
	}
	DrawTab(ActiveLeft,0,ActiveWidth,24,true,ActiveCaption);
}

void CTabControl::DrawTab(int TabLeft, int TabTop, int TabWidth,
	int TabHeight, int Active, const char *Caption)
{
	int TabRight, TabBottom;

	TabRight = TabLeft + TabWidth - 1;
	TabBottom = TabTop + TabHeight - 1;

	Graph->VLine(TabLeft,TabTop,TabHeight - 2,21);
	Graph->PutPixel(TabLeft + 1,TabBottom - 1,21);
	if (MouseIsOver && Enabled) {
		Graph->VLine(TabLeft + 1,TabTop,TabHeight - 2,20);
		Graph->HLine(TabLeft + 2,TabBottom - 1,TabWidth - 4,18);
		Graph->HLine(TabLeft + 2,TabBottom,TabWidth - 4,17);
		Graph->PutPixel(TabRight - 1,TabBottom - 1,17);
		Graph->VLine(TabRight - 1,TabTop,TabHeight - 2,18);
		Graph->VLine(TabRight,TabTop,TabHeight - 2,17);
		Graph->Bar(TabLeft + 2,TabTop,TabWidth - 4,TabHeight - 2,19);
	}
	else {
		Graph->HLine(TabLeft + 2,TabBottom,TabWidth - 4,18);
		Graph->PutPixel(TabRight - 1,TabBottom - 1,18);
		Graph->VLine(TabRight,TabTop,TabHeight - 2,18);
		Graph->HLine(TabLeft + 2,TabBottom - 1,TabWidth - 4,19);
		Graph->Bar(TabLeft + 1,TabTop,TabWidth - 2,TabHeight - 2,19);
	}
	if (Active) {
		if (GotFocus)
			Graph->Rectangle(TabLeft + 3,TabTop + 2,TabWidth - 6,TabHeight - 5,18);
		Graph->TextOut(TabLeft + 9, TabTop + 5,Caption,STYLE_REGULAR,17);
	}
	else
		Graph->TextOut(TabLeft + 7, TabTop + 2,Caption,STYLE_REGULAR,17);
}

void CTabControl::KeyPress(unsigned short Key)
{
	switch (Key) {
		case KEY_LEFT:
		case KEY_LEFT_EXT:
			SetActiveTab(ActiveTab - 1);
			break;
		case KEY_RIGHT:
		case KEY_RIGHT_EXT:
			SetActiveTab(ActiveTab + 1);
			break;
		case KEY_HOME:
		case KEY_HOME_EXT:
			SetActiveTab(0);
			break;
		case KEY_END:
		case KEY_END_EXT:
			SetActiveTab(TabCount - 1);
			break;
		default:
			break;
	}
}

int CTabControl::MouseDown(int MouseX, int MouseY)
{
	int Status;
	int TabIndex;
	int TabLeft;

	Status = CControl::MouseDown(MouseX,MouseY);
	if (Status != -1) {
		MouseX -= Left;
		TabLeft = 2;
		for (TabIndex = 0; TabIndex < TabCount; ++TabIndex) {
			TabLeft += TabArray[TabIndex].Width;
			if (MouseX < TabLeft) {
				if (TabIndex != ActiveTab)
					SetActiveTab(TabIndex);
				return Status;
			}
		}
	}
	return Status;
}

void CTabControl::OnChange(TTabChange TabChange)
{
	this->TabChange = TabChange;
}
