
#include <TextPop.h>

CTextPopup::CTextPopup(int Left, int Top, int Width, int Height, const char *Text, CTextScreen &TextScreenToUse):
	TextScreen(TextScreenToUse)
{
	this->Left = Left;
	this->Top = Top;
	this->Width = Width;
	this->Height = Height;
	this->Text = Text;
	ScreenBackup = new unsigned short[Width * Height];
}

CTextPopup::~CTextPopup()
{
	delete ScreenBackup;
}

void CTextPopup::Show()
{
	int Bottom = Top + Height - 1;
	int Right = Left + Width - 1;
	int Index;

	TextScreen.GetImage(Left,Top,Width,Height,ScreenBackup);

	TextScreen.FillX(Left + 1,Top,0xc4,0x0f00,Width - 2);
	TextScreen.FillX(Left + 1,Bottom,0xc4,0x0f00,Width - 2);
	TextScreen.FillY(Left,Top + 1,0xb3,0x0f00,Height - 2);
	TextScreen.FillY(Right,Top + 1,0xb3,0x0f00,Height - 2);
	TextScreen.PutChar(Left,Top,0xda,0x0f00);
	TextScreen.PutChar(Right,Top,0xbf,0x0f00);
	TextScreen.PutChar(Left,Bottom,0xc0,0x0f00);
	TextScreen.PutChar(Right,Bottom,0xd9,0x0f00);

	for (Index = Top + 1; Index < Bottom; ++Index) {
		TextScreen.FillX(Left + 1,Index,' ',0x0f00,Width - 2);
	}
	DisplayString(Text);

}

void CTextPopup::Hide()
{
	TextScreen.PutImage(Left,Top,Width,Height,ScreenBackup);
}

void CTextPopup::DisplayString(const char *String)
{
	int CursorX = Left + 2;
	int CursorY = Top + 2;

	for (; *String; ++String, ++CursorX) {
		if (*String == '\n') {
			CursorX = Left + 1;
			++CursorY;
		}
		else {
			TextScreen.PutChar(CursorX,CursorY,*String,0x0f00);
		}
	}
}
