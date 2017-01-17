/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <quit.h>
#include <disk.h>
#include <mouse.h>
#include <graph.h>
#include <apm.h>

#define AddrIpl ( (void *)0x00007c00 )
#define IplJmp() (__emit__(0xea,0x00,0x7c,0x00,0x00)) // jmp 0000:7c00

#define PostResetFlags ( *(unsigned short *)0x00000472 )
#define RebootJmp() (__emit__(0xea,0xf0,0xff,0x00,0xf0)) // jmp f000:fff0

CQuit::CQuit(CMouse *MouseToUse)
{
	Mouse = MouseToUse;
}

CQuit::~CQuit()
{
}

void CQuit::Restart()
{
	CDisk Disk;

	if (Mouse)
		delete Mouse;
	Graph->SetMode(modeText,false);
	Disk.Map(0x80,0);
	Disk.Read(0,AddrIpl,1);
	IplJmp();
}

void CQuit::Shutdown()
{
	CPowerManagement Apm;

	if (Mouse)
		delete Mouse;
	Graph->SetMode(modeText,false);
	Apm.SetPowerState(APM_DEV_ALL,APM_STATE_OFF);
}

void CQuit::Reboot()
{
	if (Mouse)
		delete Mouse;
	Graph->SetMode(modeText,false);
	PostResetFlags = 0x0000; // cold boot. 0x1234 for warm boot
	RebootJmp();
}
