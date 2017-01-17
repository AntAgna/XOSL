/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */



#include <dosio.h>
#include <string.h>
#include <memory.h>

char CDosFile::TransferBuffer[32768];


CDosFile::CDosFile()
{
}

CDosFile::~CDosFile()
{
}

int CDosFile::Delete(const char *FileName)
{
	if (SetAttrib(FileName,0) == -1)
		return -1;
	return Unlink(FileName);
}

int CDosFile::Copy(const char *Src, const char *Dest)
{
	int hInFile;
	int hOutFile;
	unsigned short Size;

	if ((hInFile = Open(Src,accessReadOnly)) == -1)
		return -1;
	if ((hOutFile = Create(Dest)) == -1) {
		Close(hInFile);
		return -1;
	}

	while ((Size = Read(hInFile,TransferBuffer,32768)) != 0)
		Write(hOutFile,TransferBuffer,Size);

	Close(hInFile);
	Close(hOutFile);
	return 0;
}


int CDosFile::Append(int hOutFile, const char *FileName)
{
	int hInFile;
	unsigned short Size;

	if ((hInFile = Open(FileName,accessReadOnly)) == -1)
		return -1;

	while ((Size = Read(hInFile,TransferBuffer,32768)) != 0)
		Write(hOutFile,TransferBuffer,Size);
	Close(hInFile);
	return 0;
}

long CDosFile::FileSize(const char *FileName)
{
	int fh;
	long FileSize;

	if ((fh = Open(FileName,accessReadOnly)) == -1)
		return -1;
	FileSize = LSeek(fh,0,seekEnd);
	Close(fh);
	return FileSize;
}

void CDosFile::GetNameExt(const char *FileName, char *Name, char *Ext)
{
	char *pExt;
	int NameLen, ExtLen;

	pExt = strchr(FileName,'.');
	if (!pExt) {
		NameLen = strlen(FileName);
		ExtLen = 0;
	}
	else {
		NameLen = (unsigned short)pExt - (unsigned short)FileName;
		ExtLen = strlen(FileName) - NameLen - 1;
	}

	MemCopy(Name,FileName,NameLen);
	MemCopy(Ext,&FileName[NameLen + 1],ExtLen);
	Name[NameLen] = '\0';
	Ext[ExtLen] = '\0';
}

