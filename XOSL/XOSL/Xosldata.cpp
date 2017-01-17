/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <xosldata.h>
#include <mem.h>

static const char *XOSLDataFile = "XOSLDATAXDF";

CXOSLData::CXOSLData(CFileSystem *FileSystemToUse)
{
	FileSystem = FileSystemToUse;
	Data = new TXOSLData;
	Backup = new TXOSLData;
	FileSystem->ReadFile(XOSLDataFile,Data);
	memcpy(Backup,Data,sizeof (TXOSLData));
}

CXOSLData::~CXOSLData()
{
}

void CXOSLData::SaveData()
{
	FileSystem->WriteFile(XOSLDataFile,Data);
	memcpy(Backup,Data,sizeof (TXOSLData));
}

void CXOSLData::ResetGraphics()
{
	memcpy(&Data->GraphData,&Backup->GraphData,sizeof (TGraphData));
}

TGraphData *CXOSLData::GetGraphics()
{
	return &Data->GraphData;
}

void CXOSLData::ResetColor()
{
	memcpy(&Data->ColorData,&Backup->ColorData,sizeof (TColorData));
}

TColorData *CXOSLData::GetColor()
{
	return &Data->ColorData;
}

void CXOSLData::ResetMouse()
{
	memcpy(&Data->MouseData,&Backup->MouseData,sizeof (TMouseData));
}

TMouseData *CXOSLData::GetMouse()
{
	return &Data->MouseData;
}

unsigned long CXOSLData::GetPassword()
{
	return Data->PasswordData.Password;
}

void CXOSLData::SetPassword(unsigned long Password)
{
	Data->PasswordData.Password = Password;
}

void CXOSLData::ResetMiscPref()
{
	memcpy(&Data->MiscPref,&Backup->MiscPref,sizeof (TMiscPref));
}

TMiscPref *CXOSLData::GetMiscPref()
{
	return &Data->MiscPref;
}
