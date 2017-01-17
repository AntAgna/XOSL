/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __fatfix__
#define __fatfix__

#include <newdefs.h>

#include <fat16.h>
#include <fat32.h>
#include <ptab.h>

typedef union {
	TBootFAT16 BootFAT16;
	TBootFAT32 BootFAT32;
} TBootRecord;

class CDriveFix {
	public:
		CDriveFix();
		~CDriveFix();
		int CanFixFAT(const TPartition *Partition);
		void FixFAT(int Drive, int FSType, TBootRecord *BootRecord);
		static void SwapDrive(int Drive);
};

#endif