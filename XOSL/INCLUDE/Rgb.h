/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __rgb__
#define __rgb__

#include <newdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

void far SetRGB(int Index, int Red, int Green, int Blue);
void far GetRGB(int Index, int &Red, int &Green, int &Blue);

#ifdef __cplusplus
};
#endif

#endif