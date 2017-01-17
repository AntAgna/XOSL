/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <files.h>

CXoslFiles::CXoslFiles()
{
	IgnoreRPM = false;
}

CXoslFiles::~CXoslFiles()
{
}

int CXoslFiles::GetCount()
{
	if (!IgnoreRPM) {
		return FileCount;
	}
	return FileCount - 2;
}

const char *CXoslFiles::GetFileName(int Index)
{
	return FileList[Index];
}

const char *CXoslFiles::GetXoslDataName()
{
	return XoslData;
}

const char *CXoslFiles::GetBootItemName()
{
	return BootItem;
}

const char *CXoslFiles::GetIplFileName(TFileIpl IplToUse)
{
	return IplFileList[IplToUse];
}

const char *CXoslFiles::GetCurrentMbrName()
{
	return CurrentMBR;
}

const char *CXoslFiles::GetOriginalMbrName()
{
	return OriginalMBR;
}

const char *CXoslFiles::GetDefaultMbrName()
{
	return "DEFAULT.MBR";
}

const char *CXoslFiles::GetSmartBmName()
{
	return "SBM_IPL0.XCF";
}


void CXoslFiles::IgnorePartManFiles()
{
	IgnoreRPM = true;
}


const char *CXoslFiles::XoslData = "XOSLDATA.XDF";
const char *CXoslFiles::BootItem = "BOOTITEM.XDF";
const char *CXoslFiles::CurrentMBR = "CURR_MBR.XCF";
const char *CXoslFiles::OriginalMBR = "ORIG_MBR.XCF";


const char *CXoslFiles::FileList[] = {
	"XOSLLOAD.XCF",
	"XOSLIMG0.XXF","XOSLIMG1.XXF","XOSLIMG2.XXF","XOSLIMG3.XXF","XOSLIMG4.XXF","XOSLIMG5.XXF",
	"XOSLLOGO.XBF","XOSLWALL.XBF","SPLASHLG.XBF",
	"DEFAULT.XFF","EXTRA.XFF",
	"ORIG_MBR.XCF","CURR_MBR.XCF","SBM_IPL0.XCF",
	"BOOTITEM.XDF","XOSLDATA.XDF",
	"XRPART00.XXF","XRPART01.XXF"
};

const int CXoslFiles::FileCount = sizeof (FileList) / sizeof (char *);


const char *CXoslFiles::IplFileList[] = {
	"IPL06.BIN","IPL0B.BIN","IPL06LBA.BIN","IPL0BLBA.BIN",
	"IPLS.BIN","IPLSLBA.BIN"
};
