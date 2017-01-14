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
; * File  : DISK.ASM
; * Author: Geurt Vos
; * Date  : March 1999
; *
; * Reads sectors from the 'mapped' partition
; */

                .model  tiny
                .386p
                .data

                extrn   DriveNumber: byte
                extrn   DiskSectors: dword
                extrn   DiskHeads: dword
                extrn   ABSSectorStart: dword

                .code
                public  ReadSectors

;int ReadSectors(unsigned long Sector, void *Buffer, int Count)
ReadSectors     proc
                push    bp
                mov     bp,sp
                
                mov     eax,[bp + 4]

                ; Locate the sector on current partition
                add     eax,ABSSectorStart
                xor     edx,edx
                div     DiskSectors
                inc     dx
                mov     cx,dx
                xor     edx,edx
                div     DiskHeads

                mov     dh,dl
                mov     dl,DriveNumber          ;dx = drvhead

                xchg    ah,al
                shl     al,6
                or      cx,ax                   ;cx = sectcyl
                xor     ax,ax

                mov     ah,02h
                mov     al,[bp + 12]
                les     bx,[bp + 8]
                int     13h
                sbb     ax,ax                  ;ax = cf ? -1 : 0

                pop     bp
                ret
ReadSectors     endp

                end
