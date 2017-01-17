/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <defs.h>
#include <data.h>
#include <textui.h>
#include <transfer.h>

#include <vesa.h>
#include <mouse.h>


CData::CData(CTextUI &TextUIToUse, CPartList &PartListToUse): 
	TextUI(TextUIToUse),
	PartList(PartListToUse)
{
	GraphDetected = 0;
	MouseDetected = 0;
	DriveDetected = 0;
}

CData::~CData()
{
}

int CData::CollectData(int DetectGraph, int DetectMouse, int DetectDrive)
{
	if ((GraphDetected || !DetectGraph) && (MouseDetected || !DetectMouse) && (DriveDetected || !DetectDrive))
		return 0;

	TextUI.OutputStr("Analyzing system...\n");
	if (DetectGraph) {
		if (DetectGraphicsModes() == -1)
			return -1;
		GraphDetected = 1;
	}

	if (DetectMouse) {
		DetectMouseTypes();
		MouseDetected = 1;
	}

	if (DetectDrive) {
		DetectDrives();
		DriveDetected = 1;
	}

	TextUI.OutputStr("\n");
	return 0;
}


int CData::DetectGraphicsModes()
{
	int Index;
	int VbeMajor, VbeMinor;
	int GraphicsMode;
	const char *ModeName;
	CVesa Vesa;

	// Detect VBE 1.2 support and display some info.
	ModeCount = 0;
	TextUI.OutputStr("Graphics controller:\n");
	if (Vesa.Initialize() != -1) {
		Vesa.GetVbeVersion(VbeMajor,VbeMinor);
		TextUI.OutputStr("OEM name: %s\n",Vesa.GetOemName());
		TextUI.OutputStr("VBE version number: %d.%d\n",VbeMajor,VbeMinor);
		TextUI.OutputStr("Video memory: %ldkb\n",Vesa.GetVideoMemorySize());
	}
	else {
		TextUI.OutputStr("\nDisplay controller not supported!\n");
		return -1;
	}
	if (((VbeMajor << 8) | VbeMinor) < 0x0102) {
		TextUI.OutputStr("\nVBE version not supported!\n");
		return -1;
	}


	// find available graphics modes
	TextUI.OutputStr("Supported graphics modes:\n");
	ModeCount = 0;
	if (Vesa.GetGraphicsModeCount()) {
		for (Index = CVesa::mode640x480; Index <= CVesa::mode1600x1200; ++Index) {
			GraphicsMode = Vesa.GetGraphicsModeSupported((CVesa::TGraphicsMode)Index);
			ModeName = Vesa.GetGraphicsModeName((CVesa::TGraphicsMode)Index);
			if (GraphicsMode != -1) {
				TextUI.OutputStr(" %x: %s\n",GraphicsMode,ModeName);
				GraphicsModes[ModeCount] = (CVesa::TGraphicsMode)Index;
				ModeNames[ModeCount] = ModeName;
				++ModeCount;
			}
		}
	}
	else {
		TextUI.OutputStr("No supported graphics modes found!\n\n");
		return -1;
	}
	return 0;
}


void CData::DetectMouseTypes()
{
	CMouse Mouse;
	int Index;

	MouseTypeCount = 0;
	TextUI.OutputStr("Analyzing:");
	for (Index = 0; Index < 5; ++Index) {
		TextUI.OutputStr(" %s",Mouse.GetPortName((CMouse::TMouseType)Index));
		if (Mouse.MouseTypeSupported((CMouse::TMouseType)Index)) {
			MouseTypes[MouseTypeCount] = Index;
			MouseTypeNames[MouseTypeCount] = Mouse.GetPortName((CMouse::TMouseType)Index);
			++MouseTypeCount;
		}
	}
	MouseTypes[MouseTypeCount] = 5; // type: None
	MouseTypeNames[MouseTypeCount] = "None";
	++MouseTypeCount;

	TextUI.OutputStr("\nMouse found on:");
	for (Index = 0; Index < MouseTypeCount - 1; ++Index)
		TextUI.OutputStr(" %s",Mouse.GetPortName((CMouse::TMouseType)MouseTypes[Index]));
	TextUI.OutputStr("\n");
}


void CData::GetGraphicsModeNames(const char **&ModeNameList, int &ModeCount)
{
	ModeNameList = ModeNames;
	ModeCount = this->ModeCount;
}

void CData::GetMouseTypeNames(const char **&MouseTypeNameList, int &MouseTypeCount)
{
	MouseTypeNameList = MouseTypeNames;
	MouseTypeCount = this->MouseTypeCount;
}

void CData::DetectDrives()
{
	CDiskAccess DiskAccess;
	int Count, Index;

	Count = DiskAccess.DriveCount(0x80);
	for (Index = 0; Index < Count; ++Index)
		TextUI.OutputStr("HD%d: %s access\n",Index,DiskAccess.LBAAccessAvail(0x80 | Index) == 0 ? "LBA" : "conventional");
	LastDrive = GetDriveCount();
	TextUI.OutputStr("Available drives: [C,%c]\n",('A' - 1) + LastDrive);
	TextUI.OutputStr("Reading disk structure...");
	PartList.ReadStructure();
	TextUI.OutputStr("\n");
}


int CData::GetLastDrive()
{
	return LastDrive;
}

CVesa::TGraphicsMode CData::GetGraphicsMode(int Index)
{
	return GraphicsModes[Index];
}

CMouse::TMouseType CData::GetMouseType(int Index)
{
	return (CMouse::TMouseType)MouseTypes[Index];
}
