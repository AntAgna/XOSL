/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __serial__
#define __serial__

#include <newdefs.h>

#define COMPORT ((unsigned short far *)0x00400000)
#define COMINT(Port) (((Port) & 1) ? 0x0b : 0x0c)

int ComDetect(int port);

void ComInit(int port, void far (*MouseHandler)(short P0, char dY, char dX, short Status));
void ComIRQMask(int enable);
void interrupt ComHandler(void);

extern int ComPort;
extern int MouseInt;


#endif
