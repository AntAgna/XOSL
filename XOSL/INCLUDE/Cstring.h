/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

//---------------------------------------------------------------------------
#ifndef __cstring__
#define __cstring__
//---------------------------------------------------------------------------

#define RADIX_BIN 2
#define RADIX_OCT 8
#define RADIX_DEC 10
#define RADIX_HEX 16


class CString {
public:
	CString();
    CString(CString &Str);
	CString(const char *Str);
	CString(long Value, int Radix = RADIX_DEC);
	~CString();
	void operator = (CString Str);
    void operator = (long Value);
    void operator = (const char *Str);

	void operator += (CString Str);
	void operator += (long Value);
	void operator += (const char *Str);

	int operator < (CString Str);
    int operator < (long Value);
    int operator < (const char *Str);

	int operator > (CString Str);
    int operator > (long Value);
    int operator > (const char *Str);

	int operator <= (CString Str);
    int operator <= (long Value);
    int operator <= (const char *Str);

    int operator >= (CString Str);
    int operator >= (long Value);
    int operator >= (const char *Str);

    int operator == (CString Str);
    int operator == (long Value);
    int operator == (const char *Str);

    int operator != (CString Str);
    int operator != (long Value);
    int operator != (const char *Str);

    operator const char *();
	operator long();
	long Length();
	void SetCaseSense(int CaseSense);

	void CharFill(int Ch, int Count);

	void Split(int Index, CString &Str1, CString &Str2);
	void DeleteChar(int Index);
    void InsertChar(int Index, int Char);

	void Clear();
private:
	int StringCompare(const char *Str);
	int CaseSense;
    char *String;
    int StrLen;
};


#endif
