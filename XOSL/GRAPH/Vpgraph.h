/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __vpgraph__
#define __vpraph__

#include <newdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

void VPUltraFlush(void);
void VPFlushArea(long Left, long Top, long Width, long Height);
void VPStoreBuffer(long Left, long Top, long Width, long Height);
void VPRestoreBuffer(long Left, long Top, long Width, long Height);
void VPPutPixel(long Left, long Top, long Color);
void VPHLine(long Left, long Top, long Width, long Color);
void VPVLine(long Left, long Top, long Height, long Color);
void VPRectangle(long Left, long Top, long Width, long Height, long Color);
void VPBar(long Left, long Top, long Width, long Height, long Color);
void VPPutImage(long Left, long Top, long Width, long Height, long PhysImage);
void VPGetImage(long Left, long Top, long Width, long Height, long PhysImage);
void VPLine(long X1, long Y1, long X2, long Y2, long Color);
void VPTextOut(long Left, long Top, long PhysStr, long Color);

#ifdef __cplusplus
};
#endif

#endif