;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

                .model  compact
                .386p
		.stack  6000h
                .data
		.code
                extrn   @AllocInit$qul: far
                extrn   _main: far
                extrn   @ResetTimer$qv: far
                extrn   _EnableA20: far
				extrn   _DisableA20: far
		

		.startup

		push    dword ptr 50000000h
		call    @AllocInit$qul
		pop     eax

		call 	_EnableA20
		
		call    _main

		push    ax              ;boot drive 
		call    @ResetTimer$qv

		call	_DisableA20

		pop     dx              ;Solaris needs drive no. in dl

        ; Flush keyboard (included fix from XOSL-OW 1.1.6)
        mov     bx,0040h
        mov     es,bx         ;set segment to 0040h
        cli                   ;need head not to be changed
        mov     bx,es:[001Ah] ;read head
        mov     es:[001Ch],bx ;set tail
        sti                   ;now restore IRQs
        xor     bx, bx
        
		db      0eah
		dd      00007c00h
                
		end
