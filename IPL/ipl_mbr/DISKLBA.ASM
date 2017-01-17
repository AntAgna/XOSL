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

                extrn   LBAPacket: byte
                extrn   SectorCount: word
                extrn   TransferBuffer: dword
                extrn   Sector: qword

                .code
                public  ReadSectors

;int ReadSectors(long Sector, void *Buffer, int Count)
ReadSectors     proc
                push    bp
                mov     bp,sp
                push    si
                
                mov     eax,[bp + 4]
                add     eax,ABSSectorStart
                mov     dword ptr Sector,eax
                mov     eax,[bp + 8]
                mov     TransferBuffer,eax
                mov     ax,[bp + 12]
                mov     SectorCount,ax
                mov     ah,42h
                mov     dl,DriveNumber
                mov     si,offset LBAPacket
                int     13h
                sbb     ax,ax                  ;ax = cf ? -1 : 0

                pop     si
                pop     bp
                ret
ReadSectors     endp

                end
