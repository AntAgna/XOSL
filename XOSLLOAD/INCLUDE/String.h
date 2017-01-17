/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __STRING__
#define __STRING__

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

char *itoa(long Value, char *Str, int Base);


#endif