#ifndef TextPopupH
#define TextPopupH

#include <TextScr.h>
	  
class CTextPopup {
public:
	CTextPopup(int Left, int Top, int Width, int Height, const char *Text, CTextScreen &TextScreenToUse);
	~CTextPopup();

	void Show();
	void Hide();

private:
	CTextScreen &TextScreen;
	
	int Left, Top;
	int Width, Height;
	const char *Text;

	unsigned short *ScreenBackup;

	void DisplayString(const char *String);
};

#endif
