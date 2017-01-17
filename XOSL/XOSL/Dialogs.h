/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __messagebox__
#define __messagebox__

#include <gui.h>

typedef void (*TPwdProc)(void *HandlerClass, const char *Password);
typedef void (*TAlertProc)(void *HandlerClass);


class CDialogs {
public:
	CDialogs(CScreen &ScreenToUse);
	~CDialogs();

	void ShowMessageDialog(CForm *Parent, const char *Title, const char *Text);
	void ShowPasswordDialog(CForm *Parent, const char *Title);
	void ShowAlertDialog(CForm *Parent, const char *Title, const char *Text, const char *ButtonCaption);

	void SetPasswordHandler(void *HandlerClass, TPwdProc Handler);
	void SetAlertHandler(void *HandlerClass, TAlertProc Handler);


private:
	typedef enum { dtMessage, dtPassword, dtAlert } TDialogType;

	CScreen &Screen;

	CForm *Parent;

	void *PwdObject;
	TPwdProc PwdProc;

	void *AlertObject;
	TAlertProc AlertProc;

	TDialogType DialogType;

	// control on the dialogs
	CForm *Form;
	CLabel *BodyText;
	CEdit *BodyEdit;
	CButton *CloseBtn;

	static void CloseBtnClick(CDialogs &Dialogs);
	static void PwdKeyPress(CDialogs &Dialogs, unsigned short &Key);

	void ShowMessageBox(const char *Title, const char *Text, const char *ButtonCaption);

};

#endif