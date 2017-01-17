
#include <ComboBox.h>
#include <Graph.h>
#include <key.h>

printf(const char *,...);

CComboBox::CComboBox(int DropDownCount, int Left, int Top, int Width, int Visible, void *HandlerClass):
	CAnimatedControl(Left,Top,Width,21 + 2 + (Graph->GetTextHeight() + 1) * DropDownCount,Visible,true,HandlerClass)
{
	ItemIndex = -1;
	DrawIndex = 0;
	Expanded = false;
	this->DropDownCount = DropDownCount;
	VisibleHeight = 21;
	ComboBoxChange = NULL;
}

CComboBox::~CComboBox()
{
}

void CComboBox::Blur()
{
	Expanded = false;
	CControl::Blur();
}

int CComboBox::MouseDown(int MouseX, int MouseY)
{
	int Status;

	if (!Expanded && MouseY - Top > 21)
		return -1;
	Status = CControl::MouseDown(MouseX,MouseY);

	if (Status != -1 && Enabled) {
		MouseY -= Top;
		if (MouseY < 22 && !Expanded) {
			Expanded = true;
			Refresh();
		}
		else if (Expanded) {
			if (MouseX >= Left && MouseX <= Right && MouseY >= 22 && MouseY < VisibleHeight - 1 ) {
				SetItemIndex(DrawIndex);
			}
			Expanded = false;
			VisibleHeight = 21;
			MouseIsOver = false;
			Refresh();
		}
	}
	else if (Expanded) {
		Expanded = false;
		DrawIndex = ItemIndex;
		VisibleHeight = 21;
		Refresh();
	}
	return Status;
}

int CComboBox::MouseMove(int X, int Y)
{
	int Status;

	Status = CAnimatedControl::MouseMove(X,Y);
	if (MouseIsOver && MouseY - Top > VisibleHeight) {
		MouseIsOver = false;
		MouseOut();
		Status = -1;
	}

	Y -= Top;
	if (Expanded && MouseX >= Left && MouseX <= Right && Y >= 22 && Y < VisibleHeight - 1 ) {
		DrawIndex = (Y - 22) / (Graph->GetTextHeight() + 1);
		Refresh();
	}
	return Status;
}

void CComboBox::MouseOver()
{
	if (MouseY - Top <= VisibleHeight) {
		CAnimatedControl::MouseOver();
	}
	else {
		MouseIsOver = false;
	}
}

void CComboBox::SetItemIndex(int ItemIndex)
{
	list<CString>::iterator Item;

	if (ItemIndex == this->ItemIndex || ItemIndex < -1 || ItemIndex >= ItemList.size()) {
		return;
	}

	this->ItemIndex = ItemIndex;
	DrawIndex = ItemIndex;
	if (ItemIndex != -1) {
		for (Item = ItemList.begin(); ItemIndex--; ++Item);
		Caption = *Item;
	}
	Refresh();
	if (ComboBoxChange && HandlerClass) {
		ComboBoxChange(HandlerClass,this->ItemIndex);
	}
}

int CComboBox::GetItemIndex()
{
	return ItemIndex;
}

void CComboBox::OnChange(TComboBoxChange ComboBoxChange)
{
	this->ComboBoxChange = ComboBoxChange;
}


void CComboBox::KeyPress(unsigned short Key)
{
	switch (Key) {
		case KEY_UP:
		case KEY_UP_EXT:
			SetDrawIndex(DrawIndex - 1);
			break;
		case KEY_DOWN:
		case KEY_DOWN_EXT:
			SetDrawIndex(DrawIndex + 1);
			break;
		case KEY_HOME:
		case KEY_HOME_EXT:
			SetDrawIndex(0);
			break;
		case KEY_END:
		case KEY_END_EXT:
			SetDrawIndex(ItemList.size() - 1);
			break;
		case KEY_ENTER:
		case KEY_K_ENTER:
			if (Expanded) {
				Expanded = false;
				VisibleHeight = 21;
				SetItemIndex(DrawIndex);
			}
			break;
		case KEY_ESCAPE:
			if (Expanded) {
				Expanded = false;
				VisibleHeight = 21;
				DrawIndex = ItemIndex;
				Refresh();
			}
			break;

/*		case KEY_PAGEUP:
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
			break;*/
		default:
			if (WndOnKeyPress && HandlerClass)
				WndOnKeyPress(HandlerClass,Key);
			break;
	}
}

void CComboBox::Draw(long, long DamageTop, long, long)
{
	int TextColor;

	if (!Expanded && DamageTop >= 21) {
		return;
	}

	Graph->Bar(1,1,Width - 2,19,21);

	if (GotFocus && !Expanded) {
		Graph->Bar(3,3,Width - 19,16,22);
		TextColor = 21;
	}
	else {
		TextColor = 17;
	}

	if (ItemIndex != -1) {
		Graph->TextOut(5,3,(const char *)Caption,STYLE_REGULAR,TextColor);
	}

	Graph->HLine(0,0,Width - 1,18);
	Graph->VLine(0,1,19,18);

	if (Enabled && MouseIsOver) {
		Graph->HLine(1,1,Width - 3,17);
		Graph->VLine(1,2,17,17);

		Graph->HLine(1,19,Width - 2,20);
		Graph->VLine(Width - 2,1,18,20);
	}
	else {
		Graph->VLine(Width - 2,1,18,21);
	}

	Graph->HLine(0,20,Width,21);
	Graph->VLine(Width - 1,0,20,21);

	DrawButton();
	if (Expanded) {
		DrawList();
	}

}

void CComboBox::DrawButton()
{
	int ButtonLeft = Width - 15;
	int ArrowColor;

	Graph->Bar(ButtonLeft,2,12,16,19);
	if (Enabled) {
		ArrowColor = 17;
	}
	else {
		ArrowColor = 18;
		Graph->HLine(ButtonLeft + 8,10,2,21);
		Graph->HLine(ButtonLeft + 7,11,2,21);
		Graph->PutPixel(ButtonLeft + 7,12,21);
	}
	Graph->HLine(ButtonLeft + 4,9,5,ArrowColor);
	Graph->HLine(ButtonLeft + 5,10,3,ArrowColor);
	Graph->PutPixel(ButtonLeft + 6,11,ArrowColor);

	if (MouseIsOver && Enabled) {
		Graph->HLine(ButtonLeft,18,13,17);
		Graph->VLine(ButtonLeft + 12,2,16,17);
		Graph->HLine(ButtonLeft + 1,17,11,18);
		Graph->VLine(ButtonLeft + 11,3,14,18);
		Graph->HLine(ButtonLeft + 1,3,10,20);
		Graph->VLine(ButtonLeft + 1,4,13,20);
	}
	else {
		Graph->HLine(ButtonLeft,18,13,18);
		Graph->VLine(ButtonLeft + 12,2,16,18);
	}
}

void CComboBox::DrawList()
{
	int DrawCount = ItemList.size() < DropDownCount ? ItemList.size() : DropDownCount;
	int RowHeight = Graph->GetTextHeight() + 1;
	int ListHeight = RowHeight * DrawCount;
	int TextTop = 22;
	int Index;
	list<CString>::iterator Item;

	VisibleHeight = ListHeight + 2 + 21;

	Graph->Bar(1,22,Width - 2,ListHeight,21);

	for (Index = 0, Item = ItemList.begin(); Index < DrawCount; ++Item, ++Index, TextTop += RowHeight) {
		if (Index != DrawIndex) {
			Graph->TextOut(4,TextTop,*Item,STYLE_REGULAR,17);
		}
		else {
			Graph->Bar(1,TextTop,Width - 2,RowHeight,22);
			Graph->TextOut(4,TextTop,*Item,STYLE_REGULAR,21);
		}
	}

	Graph->Rectangle(0,21,Width,ListHeight + 2,MouseIsOver ? 17 : 18);

}



void CComboBox::AddItem(const char *ItemName)
{
	ItemList.insert(ItemList.end(),ItemName);
}

int CComboBox::GetCount()
{
	return ItemList.size();
}


void CComboBox::SetDrawIndex(int Index)
{
	if (Index >= 0 && Index < ItemList.size() && DrawIndex != Index) {
		DrawIndex = Index;
		Expanded = true;
		Refresh();
	}
	else if (!Expanded) {
		Expanded = true;
		Refresh();
	}
}
