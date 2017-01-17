/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999/2000 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <wndlist.h>
#include <graph.h>
#include <screen.h>
#include <control.h>
#include <form.h>

#include <algorith.hpp>


/*
 * NOTES:
 *
 * never delete a focused window
 * ThisNode undefined after delete
 */

/*
 * WidowList == dummy. Index == -1
 * WindowList---Item0---Item1---...
 */

CWindowList::CWindowList()
{
	FocusEntry = TabOrderList.end();
	hScreen = NULL;
	hForm = NULL;
	Visible = true;
	LastFocus = NULL;
}

CWindowList::~CWindowList()
{
}

void CWindowList::SetVisible(int Visible)
{
	this->Visible = Visible;
}

int CWindowList::GetVisible()
{
	return Visible;
}

void CWindowList::Add(CControl *Wnd)
{
	DrawList.insert(DrawList.end(),Wnd);
	TabOrderList.insert(TabOrderList.end(),Wnd);

	Wnd->SetParent(this);
}

void CWindowList::Remove(CControl *Wnd)
{
	list<CControl *>::iterator RemovePos;

	RemovePos = find(DrawList.begin(),DrawList.end(),Wnd);
	if (RemovePos != DrawList.end()) {
		DrawList.erase(RemovePos);
		TabOrderList.erase(find(TabOrderList.begin(),TabOrderList.end(),Wnd));
	}
}

int CWindowList::Count()
{
	return DrawList.size();
}


void CWindowList::SetPosition(int Left, int Top)
{
	this->Left = Left;
	this->Top = Top;
}

void CWindowList::GetPosition(int &Left, int &Top)
{
	Left = this->Left;
	Top = this->Top;
}

void CWindowList::SetMetrics(int Width, int Height)
{
	this->Width = Width;
	this->Height = Height;
}

void CWindowList::GetMetrics(int &Width, int &Height)
{
	Width = this->Width;
	Height = this->Height;
}

void CWindowList::SetFocusWnd(CControl *Wnd, int MoveFocused)
{
	CControl *OldFocus;

	if (!Wnd) {
		FocusEntry = TabOrderList.end();
		return;
	}

	if (FocusEntry != TabOrderList.end())
		OldFocus = *FocusEntry;
	else
		OldFocus = NULL;


	if (MoveFocused) {
		DrawList.erase(find(DrawList.begin(),DrawList.end(),Wnd));
		DrawList.insert(DrawList.end(),Wnd);
	}
	FocusEntry = find(TabOrderList.begin(),TabOrderList.end(),Wnd);

	SwitchFocus(OldFocus,Wnd);
}

CControl *CWindowList::GetFocusWnd()
{
	return FocusEntry != TabOrderList.end() ? *FocusEntry : NULL;
}

CControl *CWindowList::FocusNext()
{
	CControl *OldFocus;

	if (FocusEntry == TabOrderList.end())
		return NULL;

	OldFocus = *FocusEntry;

	do {
		++FocusEntry;
		if (FocusEntry == TabOrderList.end())
			FocusEntry = TabOrderList.begin();
	} while (!(*FocusEntry)->Enabled || !(*FocusEntry)->Visible || !(*FocusEntry)->FocusWnd);

	if ((*FocusEntry)->OnTop) {
		DrawList.erase(find(DrawList.begin(),DrawList.end(),*FocusEntry));
		DrawList.insert(DrawList.end(),*FocusEntry);
	}

	SwitchFocus(OldFocus,*FocusEntry);
	return *FocusEntry;
}

CControl *CWindowList::FocusPrev()
{
	CControl *OldFocus;

	if (FocusEntry == TabOrderList.end())
		return NULL;

	OldFocus = *FocusEntry;

	do {
		if (FocusEntry == TabOrderList.begin()) {
			FocusEntry = TabOrderList.end();
		}
		--FocusEntry;
	} while (!(*FocusEntry)->Enabled || !(*FocusEntry)->Visible || !(*FocusEntry)->FocusWnd);

	if ((*FocusEntry)->OnTop) {
		DrawList.erase(find(DrawList.begin(),DrawList.end(),*FocusEntry));
		DrawList.insert(DrawList.end(),*FocusEntry);
	}

	SwitchFocus(OldFocus,*FocusEntry);
	return *FocusEntry;
}

void CWindowList::FixDamage(long Left, long Top, long Width, long Height)
{
	list<CControl *>::iterator DrawEntry;
	long VPLeft, VPTop;
	long ClipLeft, ClipTop, ClipWidth, ClipHeight;
	long lLeft, lTop;

	if (!Visible || AdjustArea(Left,Top,Width,Height) == -1)
		return;
	lLeft = Left + this->Left;
	lTop = Top + this->Top;
	if (hScreen)
		hScreen->BeforeFix(lLeft, lTop, Width,Height);
	if (hForm)
		hForm->BeforeFix(lLeft, lTop, Width,Height);

	for (DrawEntry = DrawList.begin(); DrawEntry != DrawList.end(); ++DrawEntry) {
		(*DrawEntry)->FixDamage(Left,Top,Width,Height);
	}

	if (hScreen)
		hScreen->AfterFix(lLeft, lTop, Width,Height);
	if (hForm)
		hForm->AfterFix(lLeft, lTop, Width,Height);

}

void CWindowList::SetHandler(CScreen *hScreen)
{
	this->hScreen = hScreen;
}


void CWindowList::SetHandler(CForm *hForm)
{
	this->hForm = hForm;
}

void CWindowList::SwitchFocus(CControl *From, CControl *To)
{
	LastFocus = From;
	if (From)
		From->Blur();
	To->Focus();

	// ???????????????? It the following code necessary?
	if (From)
		Graph->FlushArea(Left + From->Left,Top + From->Top, From->Width,From->Height);
	Graph->FlushArea(Left + To->Left,Top + To->Top,To->Width,To->Height);
}

CControl *CWindowList::GetLastFocus()
{
	return LastFocus;
}



void CWindowList::MouseDown(int Left, int Top)
{
	list<CControl *>::iterator DrawEntry(DrawList.end());

	if (!DrawList.size()) {
		return;
	}

	do {
		--DrawEntry;
	} while ((*DrawEntry)->MouseDown(Left - this->Left,Top - this->Top) == -1 && DrawEntry != DrawList.begin());
}

void CWindowList::MouseUp()
{
	if (FocusEntry != TabOrderList.end())
		(*FocusEntry)->MouseUp();
}

void CWindowList::MouseMove(int Left, int Top)
{
	list<CControl *>::iterator DrawEntry(DrawList.end());

	if (!DrawList.size()) {
		return;
	}

	do {
		--DrawEntry;
	} while ((*DrawEntry)->MouseMove(Left - this->Left,Top - this->Top) == -1 && DrawEntry != DrawList.begin());

}

int CWindowList::AdjustArea(long &iLeft, long &iTop, long &iWidth, long &iHeight)
{
	long iRight, iBottom;
	long Right, Bottom;

	iRight = iLeft + iWidth - 1;
	iBottom = iTop + iHeight - 1;
	if (iLeft >= Width || iRight < 0 || iTop >= Height || iBottom < 0)
		return -1;
	if (iLeft < 0) {
		iWidth += iLeft;
		iLeft = 0;
	}
	if (iTop < 0) {
		iHeight += iTop;
		iTop = 0;
	}
	if (iRight >= Width)
		iWidth -= iRight - Width + 1;
	if (iBottom >= Height)
		iHeight -= iBottom - Height + 1;

	return 0;
}
