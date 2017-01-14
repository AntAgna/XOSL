;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

; reboot.asm as of fdapm-2009sep11.zip
;
; ### This file is part of FDAPM, a tool for APM power management and
; ### energy saving. (c) by Eric Auer <mceric@users.sf.net> 2003-2005.
; FDAPM is free software; you can redistribute it and/or modify it
; under the terms of the GNU General Public License as published
; by the Free Software Foundation; either version 2 of the License,
; or (at your option) any later version.
; ### FDAPM is distributed in the hope that it will be useful, but
; ### WITHOUT ANY WARRANTY; without even the implied warranty of
; ### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; ### GNU General Public License for more details.
; You should have received a copy of the GNU General Public License
; along with FDAPM; if not, write to the Free Software Foundation,
; Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
; (or try http://www.gnu.org/licenses/ at http://www.gnu.org).

	.model  large
	.386p

	.code

	public  @CQuit@RebootASM$qv

@CQuit@RebootASM$qv proc

; Note: as this code does not run from a DOS environment, but from XOSL BIOS-only,
; we must avoid all calls into any DOS (SW) interrupts...

			; mov ah,0dh	; DOS filesystem reset (never fails)
			; int 21h

			; mov dx,80h	; harddisks (and floppies, if any)
			; xor ax,ax	; reset disks
			; int 13h

			; mov cx,1	; wait 1 second
			; call countBar

			mov dx,3f2h	; floppy motor control
			xor ax,ax
			out dx,al	; stop all floppy / diskette drive motors

			push ds
			mov ax,40h
			mov ds,ax
			mov word ptr [ds:72h],0000h	; full cold boot
			pop ds
			
			mov al,0feh		; keyboard reboot
			out 64h,al		; try keyboard...
			
			mov cx,1	; wait 1 second
			call countBar

			jmp far ptr [cs:bootpt]	; ...and normal reboot 
			
		countBar:	; wait N seconds (destroys AX CX DX)
			push cx
			call getTicks
			add ax,18	; 1 second
			mov dx,ax
			sti		; some int 13, 15, 16, 21 or 2f "flush" call can have
					; accidentally disabled interrupts :-( (fix 1/2007)
		countWaiting:		; we do NOT call int 28h / idling while waiting!
			call getTicks
			cmp ax,dx
			jnz countWaiting
			pop cx
			loop countBar
			ret

		getTicks:	; get low word of timer tick count into AX
			push ds
			xor ax,ax
			mov ds,ax
			mov ax,[ds:46ch]
			pop ds
			ret
			
		bootpt	dw 0,0ffffh	; boot entry point is FFFF:0000
	
@CQuit@RebootASM$qv endp

	end
