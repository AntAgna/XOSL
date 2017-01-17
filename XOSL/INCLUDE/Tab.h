/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __tab__
#define __tab__

#include <AniCntrl.h>

typedef void (*TTabChange)(void *HandlerClass, int ActiveTab);

class CTabControl: public CAnimatedControl {
	public:
		typedef struct STabList {
			CControl *Window;
			struct STabList *Next;
		} TTabList;

		typedef struct {
			TTabList *TabList;
			CString *Caption;
			int Width;
		} TTabArray;

	public:
		CTabControl(int TabCount, int Left, int Top, int Width, int Visible, void *HandlerClass);
		~CTabControl();
		void SetTabCaption(int TabIndex, const char *Caption);
		void AddControl(int TabIndex, CControl *Window);
		int GetActiveTab();
		void SetActiveTab(int TabIndex);
		void SetTabPage(CControl *TabPage);

		void KeyPress(unsigned short Key);
		int MouseDown(int MouseX, int MouseY);

		void OnChange(TTabChange TabChange);
	private:
		void Draw(long Left, long Top, long Width, long Height);
		void SetWndVisible(int Visible);
		void DrawTab(int TabLeft, int TabTop, int TabWidth,
						 int TabHeight, int Active, const char *Caption);
		TTabArray *TabArray;
		int TabCount;
		int ActiveTab;
		CControl *TabPage;
		TTabChange TabChange;
};

#endif