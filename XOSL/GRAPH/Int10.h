/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef __int10__
#define __int10__

#include <newdefs.h>

typedef struct {
	int mode_attr;        // mode attribute (bit 0: mode supported)
	char attr_wina;       // attribute window A
	char attr_winb;       // attribute window B
	int granularity;
	int winsize;          // window size
	unsigned short seg_wina;        // segment window A
	unsigned short seg_winb;        // segment window B
	unsigned long switch_addr;     // -> FAR call bankswitch function
	int bytes_scanline;   // bytes per scanline
	int width;            // screen width
	int height;           // screen height
	char charcell_w;      // charcell with in pixels
	char charcell_h;      // charcell height in pixels
	char mem_planes;      // number of memory planes
	char bits_pixel;      // number of bits per pixel
	char banks;           // number of memory banks
	char memory_model;
	char bank_size;
	char image_pages;
	char reserved0;
	char redsize;         // red mask size
	char redpos;          // red field position
	char greensize;       // green mask size
	char greenpos;        // green field position
	char bluesize;        // blue mask size
	char bluepos;         // blue field position
	char ressize;         // reserved mask size
	char respos;          // reserved field position
	char direct_mode_info;
	unsigned long linear_buffer;   // physical address of linear frame buffer
	unsigned long offscrn_mem;     // address of offscreen memory
	int offscrn_size;     // offscreen memory size (kb)
	char reserved1[206];
} TModeInfo;

typedef struct {
	char vesa[4];     // "VESA"
	char vermin;      // minor version number
	char vermaj;      // major version number
	char far *name;   // videocard OEM name
	long capabilities;// capabilities flag
	unsigned short far *modes;  // -> list of available videmodes
	int memory;       // amount of installed video memory (in 64kb blocks)
	char reserved[236];
	char scratchpad[256];
} TSVGAInfo;

#ifdef __cplusplus
extern "C" {
#endif

void far BankSwitch(void);
void far DoPMGraphics(void);
void far InitDesc(void);
int far GetSVGAInfo(TSVGAInfo *SVGAInfo);
int far GetModeInfo(int Mode, TModeInfo *ModeInfo);
int far SwitchTo(unsigned short Mode);

#ifdef __cplusplus
};
#endif

#endif