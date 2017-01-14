/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __quit__
#define __quit__

#include <newdefs.h>
#include <mouse.h>

class CQuit {
	public:
		CQuit(CMouse *MouseToUse);
		~CQuit();

		void Shutdown();
		void Reboot();
		void Restart();
	private:
		static void RebootASM();
		CMouse *Mouse;
};

#endif