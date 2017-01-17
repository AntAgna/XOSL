/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __wndlist__
#define __wndlist__

#include <newdefs.h>
#include <list.hpp>

class CControl;
class CScreen;
class CForm;


class CWindowList {
	public:
		CWindowList();
		~CWindowList();
		void SetVisible(int Visible);
		int GetVisible();

		void Add(CControl *Wnd);
		void Remove(CControl *Wnd);
		int Count();

		void SetPosition(int Left, int Top);
		void GetPosition(int &Left, int &Top);
		void SetMetrics(int Width, int Height);
		void GetMetrics(int &Width, int &Height);

		void SetFocusWnd(CControl *Wnd, int MoveFocused);
		CControl *GetFocusWnd();
		CControl *FocusNext();
		CControl *FocusPrev();
		CControl *GetLastFocus();

		void FixDamage(long Left, long Top, long Width, long Height);

		void SetHandler(CScreen *hScreen);
		void SetHandler(CForm *hForm);

		void MouseDown(int Left, int Top);
		void MouseUp();
		void MouseMove(int Left, int Top);

	private:
		void SwitchFocus(CControl *From, CControl *To);
		int AdjustArea(long &iLeft, long &iTop, long &iWidth, long &iHeight);

		list<CControl *> TabOrderList;
		list<CControl *> DrawList;

		list<CControl *>::iterator FocusEntry;
		CControl *LastFocus;

		long Left, Top;
		long Width, Height;
		int Visible;

		CScreen *hScreen;
		CForm *hForm;

};

#endif