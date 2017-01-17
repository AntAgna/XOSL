/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __window__
#define __window__

#include <newdefs.h>
#include <wndlist.h>
#include <cstring.h>

/*
 * HandlerClass: pointer to class of the
 * event handler function
 */
typedef void (*TWndOnShow)(void *HandlerClass);
typedef void (*TWndOnClick)(void *HandlerClass);
typedef void (*TWndOnHide)(void *HandlerClass);
typedef void (*TWndOnKeyPress)(void *HandlerClass, unsigned short &Key);

class CControl {
	friend class CWindowList;
	friend class CScreen;
	public:
		CControl(int Left, int Top, int Width, int Height, int Visible, int OnTop, int FocusWnd, void *HandlerClass);
		virtual ~CControl();
		virtual void Show();
		virtual void Hide();
		void SetVisible(int Visible);
		virtual void Focus();
		virtual void Blur();
		int IsVisible();
		virtual void Enable();
		virtual void Disable();
		void SetEnabled(int Enabled);
		int IsEnabled();
		virtual void SetCaption(const char *Caption);
		const char *GetCaption();
		virtual void SetMetrics(int Width, int Height);
		void GetMetrics(int &Width, int &Height);
		virtual void SetPosition(int Left, int Top);
		void GetPosition(int &Left, int &Top);
		void GetAbsPosition(int &Left, int &Top);
		void Refresh();

		void SetParent(CWindowList *Parent);
		// Incoming events
		virtual void FixDamage(int Left, int Top, int Width, int Height);
		virtual void Draw(long Left, long Top, long Width, long Height) = 0;
		virtual void KeyPress(unsigned short Key);
		virtual int MouseDown(int MouseX, int MouseY);
		virtual int MouseMove(int X, int Y);
		virtual void MouseUp();
		// Outgoing events
		void OnClick(TWndOnClick WndOnClick);
		void OnShow(TWndOnShow WndOnShow);
		void OnHide(TWndOnHide WndOnHide);
		void OnKeyPress(TWndOnKeyPress WndOnKeyPress);

		int Visible;
		CWindowList *Parent;
	private:
		TWndOnClick WndOnClick;
		TWndOnShow WndOnShow;
		TWndOnHide WndOnHide;
		int FocusWnd;
	protected:
		TWndOnKeyPress WndOnKeyPress;
		int AdjustArea(int &iLeft, int &iTop, int &iWidth, int &iHeight);
		CString Caption;
		int Left, Top;
		int Width, Height;
		int Right, Bottom;
		int MouseX, MouseY;
		int GotFocus;
		int Enabled;
		int PreFocus;
		int OnTop;

		void *HandlerClass;
};

#endif