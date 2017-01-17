/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 2000 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */


#ifndef SearchHPP
#define SearchHPP

template <typename TList, typename TItem, class CCompare>
int LinearSearch(const TList *List, int Size, TItem Key, CCompare &Compare)
{
	int Index;

	for (Index = 0; Index < Size; ++Index) {
		if (Compare.Equal(List[Index],Key)) {
			return Index;
		}
	}
	return -1;
}

template <typename TList, typename TItem, class CCompare>
int BinarySearch(const TList *List, int Size, TItem Key, CCompare &Compare)
{
	int Top, Middle, Bottom;

	Top = Size - 1;
	Bottom = 0;
	while (Top > Bottom) {
		Middle = (Top + Bottom) >> 1;
		if (Compare.Less(List[Middle],Key)) {
			Bottom = Middle + 1;
		}
		else {
			Top = Middle;
		}
	}
	if (Top != -1 && Compare.Equal(List[Top],Key)) {
		return Top;
	}
	return -1;
}


#endif