/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __apm__
#define __apm__

#include <newdefs.h>

#define APM_DEV_ALL 1
#define APM_STATE_OFF 3

class CPowerManagement {
	public:
		CPowerManagement();
		~CPowerManagement();
		static int SetPowerState(int DevId, int State);
	private:
		static int APMInstalled();
		static int Connect();

		static int APMAvailable;

};

extern CPowerManagement *PowerManagement;

#endif