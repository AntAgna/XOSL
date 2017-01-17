/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#include <dcx.h>
#include <vpgraph.h>
#include <mem.h>
#include <pmlib.h>
#include <grmem.h>

#define CURSOR_WIDTH 11
#define CURSOR_HEIGHT 19


int CursorX = 0;
int CursorY = 0;
int CursorVisible = 0;

void DrawCursor(void)
{
	int Pixel;

	if (!CursorVisible)
		return;
	VPGetImage(CursorX,CursorY,CURSOR_WIDTH,CURSOR_HEIGHT,CursorBGnd);
	PMMemCopy(CursorFGnd,CursorBGnd,CURSOR_WIDTH * CURSOR_HEIGHT);
	CreateCursor(CursorFGnd);
	VPPutImage(CursorX,CursorY,CURSOR_WIDTH,CURSOR_HEIGHT,CursorFGnd);

}

void ClearCursor(void)
{
	if (!CursorVisible)
		return;
	VPPutImage(CursorX,CursorY,CURSOR_WIDTH,CURSOR_HEIGHT,CursorBGnd);
}


