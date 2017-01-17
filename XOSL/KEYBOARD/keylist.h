/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef KeyListH
#define KeyListH

#include <newdefs.h>

class CKeyDesc {
public:
	unsigned short KeyCode;
	unsigned short NewKeyCode;
	const char *KeyName;
	const char *ShortKeyName;
	bool SpecialKey;
};

class CKeyCodeCompare {
public:
	inline CKeyCodeCompare(){}
	inline bool Equal(const CKeyDesc &Desc, unsigned short KeyCode)
	{
		return Desc.KeyCode == KeyCode;
	}

	inline bool Less(const CKeyDesc &Desc, unsigned short KeyCode)
	{
		return Desc.KeyCode < KeyCode;
	}
};

class CNewKeyCodeCompare {
public:
	CNewKeyCodeCompare(){}
	inline bool Equal(const CKeyDesc &Desc, unsigned short NewKeyCode)
	{
		return Desc.NewKeyCode == NewKeyCode;
	}	

	inline bool Less(const CKeyDesc &Desc, unsigned short NewKeyCode)
	{
		return Desc.NewKeyCode < NewKeyCode;
	}

};

#endif
