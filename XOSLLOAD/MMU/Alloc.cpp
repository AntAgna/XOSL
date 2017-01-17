/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

/*
 * Basic 'First Fit' allocation unit
 */

#include <alloc.h>
#include <mem.h>

typedef struct SMemDesc {
	struct SMemDesc *Next;
	struct SMemDesc *Prev;
	long PageCount;
	char Reserved[4];
} TMemDesc, *PMemDesc;

static PMemDesc FreeList;

_extern void AllocInit(unsigned long MemStart)
{
	PMemDesc NextItem;

	FreeList = (PMemDesc)MemStart;
	NextItem = (PMemDesc)(0x00010000 + (long)MemStart);
	FreeList->Prev = NULL;
	FreeList->Next = NextItem;
	FreeList->PageCount = 0;
	NextItem->Prev = FreeList;
	NextItem->Next = NULL;
	NextItem->PageCount = (0x000a0000 - PhysAddr(MemStart) >> 4) - 1;
}

void *operator new (unsigned int Size)
{
	long PageCount;
	PMemDesc MemDesc, OldDesc;

	PageCount = 1 + (Size >> 4);
	if (Size & 0x0f)
		++PageCount;

	for (MemDesc = FreeList->Next; MemDesc->PageCount < PageCount; MemDesc = MemDesc->Next);

	OldDesc = MemDesc;
	if (MemDesc->PageCount == PageCount) {
		// remove entire free memory block
		MemDesc->Prev->Next = MemDesc->Next;
		if (MemDesc->Next)
			MemDesc->Next->Prev = MemDesc->Prev;
	}
	else {
		// shrink free memory block
		MemDesc = (PMemDesc)((PageCount << 16) + (long)MemDesc);
		MemDesc->Next = OldDesc->Next;
		MemDesc->Prev = OldDesc->Prev;
		OldDesc->Prev->Next = MemDesc;
		OldDesc->Next->Prev = MemDesc;
		MemDesc->PageCount = OldDesc->PageCount - PageCount;
		OldDesc->PageCount = PageCount;
	}
	return (void *)(0x00010000 + (long)OldDesc);
}

void operator delete (void *ptr)
{
	PMemDesc Prev, Next, New;
	long MergeNext;

	if (!ptr)
		return;
	for (Next = FreeList->Next; (long)Next < (long)ptr; Next = Next->Next);

	Prev = Next->Prev;
	New = (PMemDesc)( ((long)ptr) - 0x00010000 );

	MergeNext = (Prev->PageCount << 16) + (long)Prev;
	if (MergeNext == (long)New) {
		Prev->PageCount += New->PageCount;
		New = Prev;
	}
	else {
		Prev->Next = New;
		New->Prev = Prev;
	}
	MergeNext = (New->PageCount << 16) + (long)New;
	if (MergeNext == (long)Next) {
		New->PageCount += Next->PageCount;
		New->Next = Next->Next;
		if (Next->Next)
			Next->Next->Prev = New;
	}
	else {
		New->Next = Next;
		Next->Prev = New;
	}
}

long CoreLeft()
{
	PMemDesc Entry;
	long Core = 0;

	for (Entry = FreeList->Next; Entry; Entry = Entry->Next)
		Core += Entry->PageCount;
	return Core << 4;
}