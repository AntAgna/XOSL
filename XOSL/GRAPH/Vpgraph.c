/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <newdefs.h>
#include <pmgraph.h>
#include <mem.h>
#include <cursor.h>
#include <grmem.h>

//Clip is absolute
long ClipLeft, ClipTop;
long ClipWidth, ClipHeight;
long ClipRight, ClipBottom;

long VPLeft, VPTop;

long ScrnWidth, ScrnHeight;

extern long TextHeight;

unsigned long AddrLinearFB = 0;

static void VPBlockTransfer(long Left, long Top, long Width, long Height,
									 long PhysImage, long Put);

void far InitViewport(long Width, long Height)
{
	ClipLeft = ClipTop = 0;
	VPLeft = VPTop = 0;
	ClipWidth = ScrnWidth = Width;
	ClipHeight = ScrnHeight = Height;
	ClipRight = Width - 1;
	ClipBottom = Height - 1;
}

void far InitVBE2(unsigned long Addr)
{
	AddrLinearFB = Addr;
}

void VPUltraFlush(void)
{
	DrawCursor();
	if (AddrLinearFB == 0)
		UltraFlush();
	else
		UltraFlushVBE2();
	ClearCursor();
}

void VPFlushArea(long Left, long Top, long Width, long Height)
{
	long Right, Bottom;

	Left += VPLeft;
	Top += VPTop;
	if (Left < ClipLeft) {
		Width -= ClipLeft - Left;
		Left = ClipLeft;
	}
	Right = Left + Width - 1;
	if (Right > ClipRight)
		Width -= Right - ClipRight;
	if (Top < ClipTop) {
		Height -= ClipTop - Top;
		Top = ClipTop;
	}
	Bottom = Top + Height - 1;
	if (Bottom > ClipBottom)
		Height -= Bottom - ClipBottom;
	if (Width > 0 && Height > 0) {
		DrawCursor();
		if (AddrLinearFB == 0)
			FlushArea(Left,Top,Width,Height);
		else
			FlushAreaVBE2(Left,Top,Width,Height);
		ClearCursor();
	}
}

void VPStoreBuffer(long Left, long Top, long Width, long Height)
{
	long Right, Bottom;

	Left += VPLeft;
	Top += VPTop;
	if (Left < ClipLeft) {
		Width -= ClipLeft - Left;
		Left = ClipLeft;
	}
	Right = Left + Width - 1;
	if (Right > ClipRight)
		Width -= Right - ClipRight;
	if (Top < ClipTop) {
		Height -= ClipTop - Top;
		Top = ClipTop;
	}
	Bottom = Top + Height - 1;
	if (Bottom > ClipBottom)
		Height -= Bottom - ClipBottom;
	if (Width > 0 && Height > 0)
		StoreBuffer(Left,Top,Width,Height);
}

extern void VPRestoreBuffer(long Left, long Top, long Width, long Height)
{
	long Right, Bottom;

	Left += VPLeft;
	Top += VPTop;
	if (Left < ClipLeft) {
		Width -= ClipLeft - Left;
		Left = ClipLeft;
	}
	Right = Left + Width - 1;
	if (Right > ClipRight)
		Width -= Right - ClipRight;
	if (Top < ClipTop) {
		Height -= ClipTop - Top;
		Top = ClipTop;
	}
	Bottom = Top + Height - 1;
	if (Bottom > ClipBottom)
		Height -= Bottom - ClipBottom;
	if (Width > 0 && Height > 0)
		RestoreBuffer(Left,Top,Width,Height);
}

void VPPutPixel(long Left, long Top, long Color)
{
	Left += VPLeft;
	Top += VPTop;
	if (Left < ClipLeft || Top < ClipTop ||
		 Left > ClipRight || Top > ClipBottom)
		return;
	PutPixel(Left,Top,Color);
}

void VPHLine(long Left, long Top, long Width, long Color)
{
	long Right;

	Left += VPLeft;
	Top += VPTop;
	if (Top < ClipTop || Top > ClipBottom || Left > ClipRight)
		return;
	if (Left < ClipLeft) {
		Width -= ClipLeft - Left;
		Left = ClipLeft;
	}
	Right = Left + Width - 1;
	if (Right > ClipRight)
		Width -= Right - ClipRight;
	if (Width > 0)
		HLine(Left,Top,Width,Color);
}

void VPVLine(long Left, long Top, long Height, long Color)
{
	long Bottom;

	Left += VPLeft;
	Top += VPTop;
	if (Left < ClipLeft || Left > ClipRight || Top > ClipBottom)
		return;
	if (Top < ClipTop) {
		Height -= ClipTop - Top;
		Top = ClipTop;
	}
	Bottom = Top + Height - 1;
	if (Bottom > ClipBottom)
		Height -= Bottom - ClipBottom;
	if (Height > 0)
		VLine(Left,Top,Height,Color);
}

void VPRectangle(long Left, long Top, long Width, long Height, long Color)
{
	VPHLine(Left,Top,Width,Color);
	VPHLine(Left,Top + Height - 1,Width,Color);
	VPVLine(Left,Top,Height,Color);
	VPVLine(Left + Width - 1,Top,Height,Color);
}

void VPBar(long Left, long Top, long Width, long Height, long Color)
{
	long Right, Bottom;

	Left += VPLeft;
	Top += VPTop;
	if (Left < ClipLeft) {
		Width -= ClipLeft - Left;
		Left = ClipLeft;
	}
	Right = Left + Width - 1;
	if (Right > ClipRight)
		Width -= Right - ClipRight;
	if (Top < ClipTop) {
		Height -= ClipTop - Top;
		Top = ClipTop;
	}
	Bottom = Top + Height - 1;
	if (Bottom > ClipBottom)
		Height -= Bottom - ClipBottom;
	if (Width > 0 && Height > 0)
		Bar(Left,Top,Width,Height,Color);
}

void VPPutImage(long Left, long Top, long Width, long Height, long PhysImage)
{
	VPBlockTransfer(Left,Top,Width,Height,PhysImage,true);
}

void VPGetImage(long Left, long Top, long Width, long Height, long PhysImage)
{
	VPBlockTransfer(Left,Top,Width,Height,PhysImage,false);
}

static void VPBlockTransfer(long Left, long Top, long Width, long Height,
									 long PhysImage, long Put)
{
	long IAdd;
	long Bottom, Right;

	Left += VPLeft;
	Top += VPTop;
	if (Top > ClipBottom || Left > ClipRight)
		return;
	if (Top < ClipTop) {
		PhysImage += (ClipTop - Top) * Width;
		Height -= ClipTop - Top;
		Top = ClipTop;
	}
	Bottom = Top + Height - 1;
	if (Bottom > ClipBottom)
		Height -= Bottom - ClipBottom;

	if (Left < ClipLeft) {
		PhysImage += ClipLeft - Left;
		IAdd = ClipLeft - Left;
		Width -= IAdd;
		Left = ClipLeft;
	}
	else
		IAdd = 0;

	Right = Left + Width - 1;
	if (Right > ClipRight) {
		IAdd += Right - ClipRight;
		Width -= Right - ClipRight;
	}
	if (Width > 0 && Height > 0)
		if (Put)
			PutImage(Left,Top,Width,Height,IAdd,PhysImage);
		else
			GetImage(Left,Top,Width,Height,IAdd,PhysImage);
}

void VPLine(long X1, long Y1, long X2, long Y2, long Color)
{
	long LgDelta, ShDelta, LgStep, ShStep, Cycle;

	LgDelta = X2 - X1;
	ShDelta = Y2 - Y1;
	if (LgDelta < 0) {
		LgDelta = -LgDelta;
		LgStep = -1;
	}
	else
		LgStep = 1;
	if (ShDelta < 0) {
		ShDelta = -ShDelta;
		ShStep = -1;
	}
	else
		ShStep = 1;

	if (LgDelta > ShDelta) {
		Cycle = LgDelta >> 1;
		while (X1 != X2) {
			VPPutPixel(X1,Y1,Color);
			X1 += LgStep;
			Cycle += ShDelta;
			if (Cycle > LgDelta) {
				Y1 += ShStep;
				Cycle -= LgDelta;
			}
		}
	}
	else {
		Cycle = ShDelta >> 1;
		LgDelta ^= ShDelta ^= LgDelta;
		LgStep ^= ShStep ^= LgStep;
		while (Y1 != Y2) {
			VPPutPixel(X1,Y1,Color);
			Y1 += LgStep;
			Cycle += ShDelta;
			if (Cycle > LgDelta) {
				X1 += ShStep;
				Cycle -= LgDelta;
			}
		}
	}
}

void VPTextOut(long Left, long Top, long PhysStr, long Color)
{
	long Width;

	Width = PMTextWidth(PhysStr);


	VPGetImage(Left,Top,Width,TextHeight,GrTextBuffer);
	CreateText(Width,PhysStr,Color);
	VPPutImage(Left,Top,Width,TextHeight,GrTextBuffer);

}

