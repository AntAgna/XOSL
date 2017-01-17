/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __edit__
#define __edit__

#include <AniCntrl.h>
#include <cstring.h>

class CEdit: public CAnimatedControl {
	public:
		CEdit(const char *Text, int MaxLength, int Masked, int ReadOnly, int Left, int Top, int Width, int Visible, void *HandlerClass);
		~CEdit();
		void SetMaxLength(int MaxLength);
		void SetMasked(int Masked);
		void SetReadOnly(int ReadOnly);

		void KeyPress(unsigned short Key);

		void SetText(const char *Text);
		void GetText(char *Text);


	private:
		void Focus();
		void Draw(long Left, long Top, long Width, long Height);
		int MaxLength;
		int Masked;
		int ReadOnly;

		int Cursor;

		CString Mask;

};

#endif