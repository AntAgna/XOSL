/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <rgb.h>
#include <graph.h>
#include <dosfile.h>
/*
static void SetMemoryBank(int Index)
{
	asm mov	ax,0x4f05
	asm mov	bx,1
	asm mov	dx,Index
	asm int 	0x10
}

void DumpScreen(const char *FileName)
{
	int Width, Height;
	unsigned long Size;
	int BankCount;
	unsigned short LastBankSize;
	int Index;
	int Handle;

	Graph->GetModeMetrics(Width,Height);
	Size = (long)Width * Height;
	BankCount = Size >> 16;
	LastBankSize = Size;

	Handle = DosCreate(FileName);
	for (Index = 0; Index < BankCount; ++Index) {
		SetMemoryBank(Index);
		DosWrite(Handle,(void *)0xa0000000,32768);
		DosWrite(Handle,(void *)0xa0008000,32768);
	}
	SetMemoryBank(Index);
	DosWrite(Handle,(void *)0xa0000000,LastBankSize);
	DosClose(Handle);
}      */

extern void puts(const char *Str);

void DumpScreen(const char *FileName)
{
	int Width, Height;
	int Handle;
	unsigned char Line[1280];
	int Top;

	Graph->GetModeMetrics(Width,Height);

	Handle = DosCreate(FileName);

	 for (Top = 0; Top < Height; ++Top) {
		  Graph->GetImage(0,Top,Width,1,Line);
		  DosWrite(Handle,Line,Width);
	 }
	DosClose(Handle);
	puts("Done");
}


void DumpPalette(const char *FileName)
{
	int RGB[256][3];
	int Index;
	int Handle;

	for (Index = 0; Index < 256; ++Index)
		GetRGB(Index,RGB[Index][0],RGB[Index][1],RGB[Index][2]);
	Handle = DosCreate(FileName);
	DosWrite(Handle,RGB,256 * 3 * 2);
	DosClose(Handle);
}
