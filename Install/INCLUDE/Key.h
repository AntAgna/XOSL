/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __key__
#define __key__

#define KEY_NONE	    0x0000
#define KEY_ESCAPE    0x011b
#define KEY_UP        0x4800
#define KEY_DOWN      0x5000
#define KEY_LEFT      0x4b00
#define KEY_RIGHT     0x4d00
#define KEY_HOME      0x4700
#define KEY_END       0x4f00
#define KEY_PAGEUP    0x4900
#define KEY_PAGEDOWN  0x5100
#define KEY_TAB       0x0f09
#define KEY_STAB      0x0f00
#define KEY_ENTER     0x1c0d
#define KEY_SPACE     0x3920
#define KEY_BACKSPACE 0x0e08
#define KEY_F1        0x3b00
#define KEY_F2        0x3c00
#define KEY_F3        0x3d00
#define KEY_F4        0x3e00
#define KEY_F5        0x3f00
#define KEY_F6        0x4000
#define KEY_F7        0x4100
#define KEY_F8        0x4200
#define KEY_F9        0x4300
#define KEY_F10       0x4400
#define KEY_1         0x0231
#define KEY_7         0x0837
#define KEY_MIN       0x4a2d
#define KEY_PLUS      0x4e2b

class CKeyboard {
	public:
		CKeyboard();
		~CKeyboard();
		int GetCh();
		int KbHit();
		void Flush();
		void StoreCh(int Key);
};

#endif