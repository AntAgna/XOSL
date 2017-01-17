#ifndef DefaultH
#define DefaultH

#include <newdefs.h>
#include <gui.h>
#include <screen.h>
#include <key.h>
#include <loader.h>

class CDefaultBoot {
public:
	CDefaultBoot(CScreen &ScreenToUse, CLoader &LoaderToUse);
	~CDefaultBoot();

	void Show(const char *BootItemName, int TimeoutValue);
	int WaitTimeout();

private:
	CScreen &Screen;
	CLoader &Loader;

	unsigned long Timeout;
	CString BootString;
	const char *BootItemName;


	void CreateControls(int ProgressBarMax);
	void InstallControls();

	void CreateBootString(unsigned long TicksPassed);

private:
	CForm *Form;
	CLabel *BootLabel;
	CProgressBar *ProgressBar;
	CLabel *KeyLabel;

};

#endif
