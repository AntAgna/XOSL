/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __ptab__
#define __ptab__

typedef struct {
	unsigned char Activated;
	unsigned char StartHead;
	unsigned short StartSectCyl;
	unsigned char FSType;
	unsigned char EndHead;
	unsigned short EndSectCyl;
	unsigned long StartSector;
	unsigned long SectorCount;
} TPTabEntry;

typedef struct {
	char Reserved[446];				// IPL in MBR
	TPTabEntry Table[4];				// The partition table
	unsigned short MagicNumber;	// should be 0xaa55
} TPTabSector;

class CPartList {
public:
	static const char *GetFSName(int FSID);
	static void CreateVolumeLabel(const char *RawLabel, char *VolumeLabel);

	typedef struct {
		int FSID;
		const char *FSName;
	} TFSNameEntry;
private:
};


#endif
