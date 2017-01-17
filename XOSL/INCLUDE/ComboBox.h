#ifndef ComboBoxH
#define ComboBoxH

#include <AniCntrl.h>
#include <list.hpp>

typedef void (*TComboBoxChange)(void *HandlerClass, int ItemIndex);


class CComboBox : public CAnimatedControl {
public:
	CComboBox(int DropDownCount, int Left, int Top, int Width, int Visible, void *HandlerClass);
	~CComboBox();

	void Blur();

	int MouseDown(int MouseX, int MouseY);
	void KeyPress(unsigned short Key);

	void AddItem(const char *ItemName);
	int GetCount();

	int MouseMove(int X, int Y);
	void MouseOver();
	
	void SetItemIndex(int ItemIndex);
	int GetItemIndex();

	void OnChange(TComboBoxChange ComboBoxChange);


private:
	bool Expanded;
	list<CString> ItemList;
	int ItemIndex, DrawIndex;
	int DropDownCount;
	int VisibleHeight;
	TComboBoxChange ComboBoxChange;

	void Draw(long Left, long Top, long Width, long Height);
	void DrawButton();
	void DrawList();
	void SetDrawIndex(int Index);
};

#endif
