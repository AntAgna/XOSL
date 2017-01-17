/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

/*
 * defs.h
 *
 * generic include file
 * is included by every single source file
 */

#ifndef __DEFS__
#define __DEFS__

#if !defined(__COMPACT__)
#	if defined(__TINY__)
#		error tiny memory model not supported, use compact memory model.
#	elif defined(__SMALL__)
#		error small memory model not supported, use compact memory model.
#	elif defined(__MEDIUM__)
#		error medium memory model not supported, use compact memory model.
#	elif defined(__LARGE__)
#		error large memory model not supported, use compact memory model.
#	elif defined(__HUGE__)
#		error huge memory model not supported, use compact memory model.
#	endif
#endif

#ifndef NULL
#	define NULL 0L
#endif

#ifndef TRUE
#	define TRUE (1 == 1)
#endif

#ifndef FALSE
#	define FALSE (1 == 0)
#endif

#ifndef _extern
#	define _extern extern "C"
#endif

#endif