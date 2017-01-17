/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __rawkey__
#define __rawkey__

#include <defs.h>

extern int far kbhit();
extern int far getch();
extern void far storech(int key);

#endif