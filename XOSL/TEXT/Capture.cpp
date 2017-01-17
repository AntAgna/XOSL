/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <capture.h>
#include <mem.h>
#include <text.h>

#define TextScreenPtr ((unsigned short *)0xb8000000)

CTextCapture::CTextCapture()
{
	TextScreen = new unsigned short [80 * 25];
	memcpy(TextScreen,TextScreenPtr,80 * 25 * 2);
	wherexy(&CursorX,&CursorY);
}

CTextCapture::~CTextCapture()
{
	memcpy(TextScreenPtr,TextScreen,80 * 25 * 2);
	delete TextScreen;
	gotoxy(CursorX,CursorY);
}
