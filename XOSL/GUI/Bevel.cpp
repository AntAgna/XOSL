/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <bevel.h>
#include <graph.h>

#define BEVEL_WIDTH(Type,Width) (Type == BEVEL_VLINE ? 2 : Width)
#define BEVEL_HEIGHT(Type,Height) (Type == BEVEL_HLINE ? 2 : Height)

CBevel::CBevel(int Type, int Lowered, int Left, int Top,
	int Width, int Height, int Visible):
	CControl(Left,Top,BEVEL_WIDTH(Type,Width),BEVEL_HEIGHT(Type,Height),
			  Visible,false,false,NULL)
{
	this->Type = Type;
	SetLowered(Lowered);
}

CBevel::CBevel(int Type, int Lowered, int Left, int Top,
	int Size, int Visible):
	CControl(Left,Top,BEVEL_WIDTH(Type,Size),BEVEL_HEIGHT(Type,Size),
			  Visible,false,false,NULL)
{
	this->Type = Type;
	SetLowered(Lowered);
}

CBevel::~CBevel()
{
}

void CBevel::Draw(long, long, long, long)
{
	switch (Type) {
		case BEVEL_VLINE:
			Graph->VLine(0,0,Height,ColorLeftTop);
			Graph->VLine(1,0,Height,ColorRightBottom);
			break;
		case BEVEL_HLINE:
			Graph->HLine(0,0,Width,ColorLeftTop);
			Graph->HLine(0,1,Width,ColorRightBottom);
			break;
		case BEVEL_BOX:
			Graph->HLine(0,0,Width - 1,ColorLeftTop);
			Graph->VLine(0,1,Height - 2,ColorLeftTop);
			Graph->HLine(0,Height - 1,Width,ColorRightBottom);
			Graph->VLine(Width - 1,0,Height - 1,ColorRightBottom);
			break;
		case BEVEL_FRAME:
			Graph->Rectangle(1,1,Width - 1,Height - 1,ColorRightBottom);
			Graph->Rectangle(0,0,Width - 1,Height - 1,ColorLeftTop);
			break;
	}
}

void CBevel::SetLowered(int Lowered)
{
	if (Lowered) {
		ColorLeftTop = 18;
		ColorRightBottom = 21;
	}
	else {
		ColorLeftTop = 21;
		ColorRightBottom = 18;
	}
}