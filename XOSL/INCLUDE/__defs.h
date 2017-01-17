/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __DEFS__
#define __DEFS__

#if !defined(__COMPACT__)
	#if !defined(__LARGE__)
		#if !defined(__HUGE__)
			#if defined(__TINY__)
				#error tiny memory model not supported, use compact, large or huge.
			#elif defined(__SMALL__)
				#error small memory model not supported, use compact, large or huge.
			#else
				#error medium memory model not supported, use compact, large or huge.
			#endif
		#endif
	#endif
#endif

#ifndef NULL
#define NULL 0L
#endif

#define true 1
#define false 0

#define FONT_SMALL 0
#define FONT_LARGE 1

#ifndef _extern
#	ifdef __cplusplus
#		define _extern extern "C"
#	else
#		define _extern extern
#	endif
#endif


#ifdef __cplusplus
#define extern extern "C"
#endif

typedef unsigned char U8B;
typedef unsigned short U16B;
typedef unsigned long U32B;

typedef struct {
	int Left, Top;
	int Width, Height;
} TRect;


#endif