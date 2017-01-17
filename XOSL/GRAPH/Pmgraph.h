/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __pmgraph__
#define __pmgraph__

#include <newdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

void UltraFlush(void);
void FlushArea(long Left, long Top, long Width, long Height);
void UltraFlushVBE2(void);
void FlushAreaVBE2(long Left, long Top, long Width, long Height);
void StoreBuffer(long Left, long Top, long Width, long Height);
void RestoreBuffer(long Left, long Top, long Width, long Height);
void PutPixel(long Left, long Top, long Color);
void HLine(long Left, long Top, long Width, long Color);
void VLine(long Left, long Top, long Height, long Color);
void Bar(long Left, long Top, long Width, long Height, long Color);
void DrawText(long Left, long Top, long PhysStr, long Color);
void PutImage(long Left, long Top, long Width, long Height, long IAdd, long PhysImage);
void GetImage(long Left, long Top, long Width, long Height, long IAdd, long PhysImage);

void SetClippingRegion(long PhysLeft, long PhysTop, long PhysWidth, long PhysHeight);
void GetClippingRegion(long PhysLeft, long PhysTop, long PhysWidth, long PhysHeight);
void SetViewportOrigin(long PhysLeft, long PhysTop);
void GetViewportOrigin(long PhysLeft, long PhysTop);


//extern void MapFontData();
long PMTextWidth(long PhysStr);
void CreateText(long Width, long PhysStr, long Color);
//extern void CreateText(long Left, long Top, long PhysStr, long Color);

extern long PhysFontHeader;
extern long PhysFontData;
extern int DataSize;

#ifdef __cplusplus
};
#endif

#endif
