/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <mouse.h>


const char *CMouse::PortNames[6] = {
	"COM1","COM2","COM3","COM4","PS/2","None"
};

CMouse::CMouse()
{
	ComPortAddr = (unsigned short *)COMPORT_ADDRLIST;
}

CMouse::~CMouse()
{
}

int CMouse::MouseTypeSupported(TMouseType MouseType)
{
	switch (MouseType) {
		case enumMousePs2:
			return Ps2Initialize(3) == 0;
		case enumMouseCom1:
		case enumMouseCom2:
		case enumMouseCom3:
		case enumMouseCom4:
			return MouseAvailable(ComPortAddr[MouseType]) == 1;
		default:
			return 0;
	}
}

const char *CMouse::GetPortName(TMouseType MouseType)
{
	return PortNames[MouseType];
}
