/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __stringlist__
#define __stringlist__

#include <cstring.h>

class CStringList {


	public:
		CStringList(int Count);
		~CStringList();

		void Insert(int Index, const char *String);
		void Delete(int Index);
		const char *Get(int Index);
		void Clear();

	private:

		typedef struct {
			CString *String;
		} TPCString;

		TPCString *Items;
		int Count;
};

#endif