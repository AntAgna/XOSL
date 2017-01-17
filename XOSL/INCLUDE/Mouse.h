/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __mouse__
#define __mouse__

#include <newdefs.h>
//#include <defs.h>

#define MOUSE_COM1 0
#define MOUSE_COM2 1
#define MOUSE_COM3 2
#define MOUSE_COM4 3
#define MOUSE_PS2 4

typedef union {
	struct {
		int Speed;
	} Com;
	struct {
		int Scale;
		int Sampling;
		int Resolution;
	} PS2;
} TMouseSpeed;


class CMouse {
	public:
		CMouse();
		~CMouse();
		int Detect(int Port);
		int SetMouse(int Port);
		int GetMouse();
		void SetWindow(int Left, int Top, int Width, int Height);
		void SetXY(int X, int Y);
		void GetXY(int &X, int &Y);
		int MouseDown();
		const char *MousePortName(int Port = -1);
		
		void SetSpeed(int NewSpeed);
		void SetSampling(int NewRate);


		void GetSpeed(TMouseSpeed &Speed);
		int Available();
	private:
		void RemoveMouse();
		void NewPS2Speed();
		TMouseSpeed MouseSpeed;
		int Port;
};



#endif