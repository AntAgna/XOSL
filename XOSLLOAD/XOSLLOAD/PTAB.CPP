/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <ptab.h>
#include <ctype.h>
#include <mem.h>

const char *CPartList::GetFSName(int FSID)
{
/*	int Index;

	for (Index = 0; FSNameList[Index].FSID != FSID && FSNameList[Index].FSID != 0xff; ++Index);
	return FSNameList[Index].FSName;*/

	switch (FSID) {
		case 0x01:
			return "Microsoft FAT12";
		case 0x04:
			return "Microsoft FAT16";
		case 0x05:
			return "Extended";
		case 0x06:
			return "Microsoft FAT16";
		case 0x07:
			return "HPFS or NTFS";
		case 0x0a:
			return "OS/2 Boot Manager";
		case 0x0b:
			return "Microsoft FAT32";
		case 0x0c:
			return "Microsoft FAT32 LBA";
		case 0x0e:
			return "Microsoft FAT16 LBA";
		case 0x0f:
			return "Extended LBA";
		case 0x11:
			return "Hidden FAT12";
		case 0x14:
			return "Hidden FAT16";
		case 0x16:
			return "Hidden FAT16";
		case 0x17:
			return "Hidden NTFS";
		case 0x1b:
			return "Hidden FAT32";
		case 0x1c:
			return "Hidden FAT32 LBA";
		case 0x1e:
			return "Hidden FAT16 LBA";
		case 0x1f:
			return "Hidden Extended LBA";
		case 0x38:
			return "TheOS";
		case 0x63:
			return "Unix SysV/386";
		case 0x78:
			return "XOSL FS";
		case 0x82:
			return "Linux Swap";
		case 0x83:
			return "Linux Native";
		case 0x85:
			return "Linux Extended";
		case 0xa5:
			return "FreeBSD, BSD/386";
		case 0xeb:
			return "BeOS";
		default:
			return "Unknown";

	}
}

void CPartList::CreateVolumeLabel(const char *RawLabel, char *VolumeLabel)
{
	int Index;

	memcpy(VolumeLabel,RawLabel,11);
	for (Index = 10; Index >= 0 && VolumeLabel[Index] == ' '; --Index);
	VolumeLabel[Index + 1] = '\0';
	if (VolumeLabel[0])
		for (Index = 1; VolumeLabel[Index]; ++Index)
			VolumeLabel[Index] = tolower(VolumeLabel[Index]);
}


// When the following is de-commented, tlink will be unable to create a COM file.
/*
const CPartList::TFSNameEntry FSNameList[] = {
	{0x01,"Microsoft FAT12"},
	{0x04,"Microsoft FAT16"},
	{0x05,"Extended"},
	{0x06,"Microsoft FAT16"},
	{0x07,"HPFS or NTFS"},
	{0x0a,"OS/2 Boot Manager"},
	{0x0b,"Microsoft FAT32"},
	{0x0c,"Microsoft FAT32 LBA"},
	{0x0e,"Microsoft FAT16 LBA"},
	{0x0f,"Extended LBA"},
	{0x11,"Hidden FAT12"},
	{0x14,"Hidden FAT16"},
	{0x16,"Hidden FAT16"},
	{0x17,"Hidden NTFS"},
	{0x1b,"Hidden FAT32"},
	{0x1c,"Hidden FAT32 LBA"},
	{0x1e,"Hidden FAT16 LBA"},
	{0x1f,"Hidden Extended LBA"},
	{0x38,"TheOS"},
	{0x63,"Unix SysV/386"},
	{0x78,"XOSL FS"},
	{0x82,"Linux Swap"},
	{0x83,"Linux Native"},
	{0x85,"Linux Extended"},
	{0xa5,"FreeBSD, BSD/386"},
	{0xeb,"BeOS"},
	{0xff,"Unknown"}
};
*/