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
#include <string.h>
#include <mem.h>

#include "cstring.h"

#include <text.h>
extern void printf(const char *fmt, ...);

//---------------------------------------------------------------------------
CString::CString()
{
   StrLen = 0;
   String = new char[1];
   *String = '\0';
   CaseSense = 1;
}

CString::CString(CString &Str)
{
   StrLen = Str.StrLen;
	String = new char [StrLen + 1];
   strcpy(String,Str.String);
   CaseSense = 1;
}

CString::CString(const char *Str)
{
	StrLen = strlen(Str);
	String = new char [StrLen + 1];
	strcpy(String,Str);
	CaseSense = 1;
}

CString::CString(long Value, int Radix)
{
	char TempStr[16];

	itoa(Value,TempStr,Radix);
	StrLen = strlen(TempStr);
	String = new char [StrLen + 1];
	strcpy(String,TempStr);
	CaseSense = 1;
}

CString::~CString()
{
	delete String;
}


void CString::operator = (CString Str)
{
	char *NewStr;

	StrLen = Str.StrLen;
	NewStr = new char [StrLen + 1];
	strcpy(NewStr,Str.String);
	delete String;
	String = NewStr;
}

void CString::operator = (long Value)
{
	char TempStr[16];

	itoa(Value,TempStr,RADIX_DEC);
	StrLen = strlen(TempStr);
	delete String;
	String = new char [StrLen + 1];
	strcpy(String,TempStr);
}

void CString::operator = (const char *Str)
{
	char *NewStr;

	StrLen = strlen(Str);
	NewStr = new char [StrLen + 1];
	strcpy(NewStr,Str);
	delete String;
	String = NewStr;
}

void CString::operator += (CString Str)
{
   char *NewStr;

   StrLen += Str.StrLen;
   NewStr = new char [StrLen + 1];
   strcpy(NewStr,String);
   strcat(NewStr,Str.String);
   delete String;
   String = NewStr;
}

void CString::operator += (long Value)
{
   char TempStr[16];
   char *NewStr;

	itoa(Value,TempStr,RADIX_DEC);
	StrLen += strlen(TempStr);
	NewStr = new char [StrLen + 1];
	strcpy(NewStr,String);
	strcat(NewStr,TempStr);
	delete String;
	String = NewStr;
}

void CString::operator += (const char *Str)
{
	char *NewStr;

   StrLen += strlen(Str);
	NewStr = new char [StrLen + 1];
	strcpy(NewStr,String);
	strcat(NewStr,Str);
	delete String;
	String = NewStr;

}

int CString::operator < (CString Str)
{
   return StringCompare(Str.String) < 0;
}

int CString::operator < (long Value)
{
   char TempStr[16];

	itoa(Value,TempStr,RADIX_DEC);
	return StringCompare(TempStr) < 0;
}

int CString::operator < (const char *Str)
{
	return StringCompare(Str) < 0;
}

int CString::operator > (CString Str)
{
   return StringCompare(Str.String) > 0;
}

int CString::operator > (long Value)
{
   char TempStr[16];

	itoa(Value,TempStr,RADIX_DEC);
	return StringCompare(TempStr) > 0;
}

int CString::operator > (const char *Str)
{
	return StringCompare(Str) > 0;
}

int CString::operator <= (CString Str)
{
	return StringCompare(Str.String) <= 0;
}

int CString::operator <= (long Value)
{
	char TempStr[16];

	itoa(Value,TempStr,RADIX_DEC);
	return StringCompare(TempStr) <= 0;
}

int CString::operator <= (const char *Str)
{
	return StringCompare(Str) <= 0;
}

int CString::operator >= (CString Str)
{
	return StringCompare(Str.String) >= 0;
}

int CString::operator >= (long Value)
{
	char TempStr[16];

	itoa(Value,TempStr,RADIX_DEC);
	return StringCompare(TempStr) >= 0;
}

int CString::operator >= (const char *Str)
{
	return StringCompare(Str) >= 0;
}

int CString::operator == (CString Str)
{
	return StringCompare(Str.String) == 0;
}

int CString::operator == (long Value)
{
	char TempStr[16];

	itoa(Value,TempStr,RADIX_DEC);
	return StringCompare(TempStr) == 0;
}

int CString::operator == (const char *Str)
{
	return StringCompare(Str) == 0;
}

int CString::operator != (CString Str)
{
	return StringCompare(Str.String) != 0;
}

int CString::operator != (long Value)
{
	char TempStr[16];

	itoa(Value,TempStr,RADIX_DEC);
	return StringCompare(TempStr) != 0;
}

int CString::operator != (const char *Str)
{
   return StringCompare(Str) != 0;
}

CString::operator const char *()
{
   return String;
}

CString::operator long()
{
   return atol(String);
}

long CString::Length()
{
   return StrLen;
}

void CString::SetCaseSense(int CaseSense)
{
   this->CaseSense = CaseSense;
}

void CString::CharFill(int Ch, int Count)
{
   delete String;
   StrLen = Count;
	String = new char [Count + 1];
   memset(String,Ch,Count);
   String[Count] = '\0';
}

void CString::Split(int Index, CString &Str1, CString &Str2)
{
	char *StrPart1;
	char *StrPart2;

	StrPart1 = new char [Index + 1];
	StrPart1[Index] = '\0';
	memcpy(StrPart1,String,Index);
	StrPart2 = &String[Index];
	Str1 = StrPart1;
	Str2 = StrPart2;
	delete StrPart1;
}

void CString::DeleteChar(int Index)
{
	char *NewStr;

	NewStr = new char [StrLen];
	NewStr[--StrLen] = '\0';
	memcpy(NewStr,String,Index);
	memcpy(&NewStr[Index],&String[Index + 1],StrLen - Index);
	delete String;
	String = NewStr;

}

void CString::InsertChar(int Index, int Char)
{
	char *NewStr;

	NewStr = new char [StrLen + 2];
	memcpy(NewStr,String,Index);
	NewStr[Index] = Char;
	memcpy(&NewStr[Index + 1],&String[Index],StrLen - Index);
	NewStr[++StrLen] = '\0';
	delete String;
	String = NewStr;
}

int CString::StringCompare(const char *Str)
{
	if (CaseSense)
		return stricmp(String,Str);
	return strcmp(String,Str);
}

void CString::Clear()
{
	delete String;
	String = new char [1];
	*String = '\0';
	StrLen = 0;
}
