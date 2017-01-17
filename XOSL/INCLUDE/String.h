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

#include <newdefs.h>

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

#ifdef __cplusplus
extern "C" {
#endif

int far strlen(const char far *str);
char *far strcpy(char far *dest, const char far *src);
char *far strcat(char far *dest, const char far *src);
char *far strchr(const char far *str, int c);
char *far itoa(long value, char far *str, int base);
int far strcmp(const char far *s1, const char far *s2);
int far stricmp(const char far *s1, const char far *s2);
long far atol(const char far *str);

#ifdef __cplusplus
};
#endif

#endif