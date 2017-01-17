;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

;/*
; * File  : MAIN.ASM
; * Author: Geurt Vos
; * Date  : March 1999
; *
; * Main routine for either FAT IPL
; */

                .model  tiny
                .386p
                .data
LOADERADDR      equ     80000100h
LOADERSEG       equ     8000h

                .data
                extrn   LoaderName: near

		.code
                extrn   ReadFile: near

		.startup
                db      0eah            ;jmp 07b0:0105
                dd      07b00105h
;07b0:0105:
                mov     ax,07b0h
                mov     ds,ax
                mov     ss,ax
                mov     sp,0fffeh


                push    LOADERADDR
                push    ds
                push    offset LoaderName
                call    ReadFile
                or      ax,ax           ;ax == 0 if success
                jz      RunLoader
                xor     ah,ah
                int     16h             ;wait for keypress

                xor     ax,ax           ;reboot
                mov     ds,ax
                mov     ds:word ptr [0472h],1234h
                db      0eah            ;jmp f000:fff0
                dd      0f000fff0h      


RunLoader:      mov     ax,LOADERSEG
                mov     ds,ax
                mov     ss,ax
                mov     sp,0fffeh

                db      0eah            ;execute com file
                dd      LOADERADDR

                end
