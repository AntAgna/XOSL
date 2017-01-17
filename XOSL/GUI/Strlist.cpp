/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <strlist.h>

static const char *EmptyString = "";

CStringList::CStringList(int Count)
{
	int Item;

	this->Count = Count;
	Items = new TPCString[Count];
	for (Item = 0; Item < Count; ++Item)
		Items[Item].String = new CString;
}

CStringList::~CStringList()
{
	int Item;

	for (Item = 0; Item < Count; ++Item)
		delete Items[Item].String;
	delete Items;
}

void CStringList::Insert(int Index, const char *String)
{
	if (Index >= Count)
		return;
	*Items[Index].String = String;
}

void CStringList::Delete(int Index)
{
	if (Index >= Count)
		return;
	*Items[Index].String = EmptyString;
}

const char *CStringList::Get(int Index)
{
	if (Index >= Count)
		return EmptyString;
	return *Items[Index].String;
}

void CStringList::Clear()
{
	int Index;

	for (Index = 0; Index < Count; ++Index)
		*Items[Index].String = EmptyString;
}