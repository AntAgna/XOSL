/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __PS2__
#define __PS2__

#include <newdefs.h>

// sampling rates
#define PS2_10Hz 0
#define PS2_20Hz 1
#define PS2_40Hz 2
#define PS2_60Hz 3
#define PS2_80Hz 4
#define PS2_100Hz 5
#define PS2_200Hz 6

// resolutions (counts per mm)
#define PS2_1CPMM 0
#define PS2_2CPMM 1
#define PS2_4CPMM 2
#define PS2_8CPMM 3

#define PS2Disable()       (PS2Call(0xc200,0x00))
#define PS2Enable()        (PS2Call(0xc200,0x01))
#define PS2Sampling(Rate)  (PS2Call(0xc202,(Rate)))
#define PS2Resolution(Res) (PS2Call(0xc203,(Res)))
#define PS2Scaling(Scale)  (PS2Call(0xc206,(Scale)))

typedef void far (* TPS2Handler)(short, char dX, char dY, short Status);

int PS2Initialize(int PackSize);
void PS2Call(unsigned short Func, int Value);
void PS2SetHandler(TPS2Handler Handler);

#endif