/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999-2000 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef KeyH
#define KeyH

#define KEY_NONE	    0x0000
#define KEY_ESCAPE    	0x011b
#define KEY_UP        	0x4800
#define KEY_UP_EXT		0x48e0
#define KEY_DOWN      	0x5000
#define KEY_DOWN_EXT	0x50e0
#define KEY_LEFT      	0x4b00
#define KEY_LEFT_EXT	0x4be0
#define KEY_RIGHT     	0x4d00
#define KEY_RIGHT_EXT	0x4de0
#define KEY_CTRLLEFT  	0x7300
#define KEY_CTRLL_EXT	0x73e0
#define KEY_CTRLRIGHT 	0x7400
#define KEY_CTRLR_EXT	0x74e0
#define KEY_HOME      	0x4700
#define KEY_HOME_EXT	0x47e0
#define KEY_END       	0x4f00
#define KEY_END_EXT		0x4fe0
#define KEY_INSERT    	0x5200
#define KEY_INSERT_EXT	0x52e0
#define KEY_DELETE    	0x5300
#define KEY_DELETE_EXT	0x53e0
#define KEY_PAGEUP    	0x4900
#define KEY_PU_EXT		0x49e0
#define KEY_PAGEDOWN  	0x5100
#define KEY_PD_EXT		0x51e0
#define KEY_TAB       	0x0f09
#define KEY_STAB      	0x0f00
#define KEY_ENTER     	0x1c0d
#define KEY_K_ENTER   	0xe00d
#define KEY_SPACE     	0x3920
#define KEY_BACKSPACE 	0x0e08
#define KEY_F1        	0x3b00
#define KEY_F2        	0x3c00
#define KEY_F3        	0x3d00
#define KEY_F4        	0x3e00
#define KEY_F5        	0x3f00
#define KEY_F6        	0x4000
#define KEY_F7        	0x4100
#define KEY_F8        	0x4200
#define KEY_F9        	0x4300
#define KEY_F10       	0x4400
#define KEY_1         	0x0231
#define KEY_7         	0x0837
#define KEY_SLEEP     	0x5f80 // Windows 98 Power Keyboard
#define KEY_SHIFT_F1  	0x5400
#define KEY_SHIFT_F10 	0x5d00
#define KEY_ALT_F1    	0x6900
#define KEY_ALT_F10   	0x7300
#define KEY_CTRL_F1   	0x5e00
#define KEY_CTRL_F10  	0x6800


#include <newdefs.h>
#include <../keyboard/keylist.h>
		 
#define KEY_COUNT 316
		 
class CKeyboard {
public:
	CKeyboard();
	~CKeyboard();

	enum TKeyLayout { enumLayoutUS = 0, enumLayoutFrench = 1};
	
	static unsigned short WaitKeyStroke();
	static void StoreKeyStroke(unsigned short KeyCode);
	static bool KeyStrokeAvail();
	static void Flush();
	static void SetEnhancedSupport(bool Status);
	
	static void SelectLayout(TKeyLayout KeyLayout);
	static void GetKeyName(unsigned short KeyCode, char *KeyName);
	static void GetShortKeyName(unsigned short KeyCode, char *KeyName);
	static unsigned short GetNewKeyCode(unsigned short KeyCode);
	static unsigned short GetKeyCode(unsigned short NewKeyCode);
	static bool IsSpecialKey(unsigned short KeyCode);
private:
	static const CKeyDesc USKeyList[KEY_COUNT];
	static const CKeyDesc FrenchKeyList[KEY_COUNT];

	static const CKeyDesc *SelectedLayout;

//	static const CKeyDesc *Current;

	static bool EnhancedSupport;

	static unsigned short WaitKeyStrokeEnh();
	static unsigned short WaitKeyStrokeDef();
	static bool KeyStrokeAvailEnh();
	static bool KeyStrokeAvailDef();
};

		 
		 


#endif