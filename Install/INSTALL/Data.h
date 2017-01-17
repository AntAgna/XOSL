/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __data__
#define __data__

#include <defs.h>
#include <dosdrv.h>
#include <textui.h>

#include <vesa.h>
#include <mouse.h>

class CData {
	public:
		CData(CTextUI &TextUIToUse, CPartList &PartListToUse);
		~CData();
		int CollectData(int DetectGraph, int DetectMouse, int DetectDrive);
		void GetGraphicsModeNames(const char **&ModeNameList, int &ModeCount);
		void GetMouseTypeNames(const char **&MouseTypeNameList, int &MouseTypeCount);
		CVesa::TGraphicsMode GetGraphicsMode(int Index);
		CMouse::TMouseType GetMouseType(int Index);

		int GetLastDrive();
		static unsigned short GetDosVersion();
	private:
		CTextUI &TextUI;
		CPartList &PartList;
		int GraphDetected;
		int MouseDetected;
		int DriveDetected;

		CVesa::TGraphicsMode GraphicsModes[6];
		const char *ModeNames[6];
		int ModeCount;
		int DetectGraphicsModes();

		int MouseTypes[5];
		int MouseTypeCount;
		const char *MouseTypeNames[5];
		void DetectMouseTypes();

		int LastDrive;
		void DetectDrives();
		static int GetDriveCount();
};

#endif