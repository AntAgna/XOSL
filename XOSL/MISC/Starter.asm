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
		

		.startup
		call 	_EnableA20

                push    dword ptr 50000000h
                call    @AllocInit$qul
                pop     eax
                call    _main

                push    ax              ;boot drive 
                call    @ResetTimer$qv
		call	_DisableA20
		
                pop     dx              ;Solaris needs drive no. in dl


                db      0eah
                dd      00007c00h
                
;;;;;;;;;;

include		A20LINE.ASM



		 end
