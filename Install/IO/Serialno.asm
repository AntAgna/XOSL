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
                .data?
DosDriveInfo    equ     this byte
Reserved        dw      ?       ;0000
SerialNumLo     dw      ?
SerialNumHi     dw      ?
VolumeLabel     db      11 dup (?)
FSType          db      8 dup (?)
                ends

                .code

                public  @CDOSDriveList@GetDOSSerialNo$qi

;unsigned long CDOSDriveList::GetDOSSerialNo(int DriveNum);
@CDOSDriveList@GetDOSSerialNo$qi proc c
                arg     @@this: dword, @@DriveNum: word

                mov     ax,6900h
                mov     bx,@@DriveNum
                add     bx,3
                mov     dx,offset DosDriveInfo
                int     21h
                jnc     GSNFound
                mov     ax,0ffffh
                mov     dx,ax
                jmp     GSNExit

GSNFound:       mov     ax,SerialNumLo
                mov     dx,SerialNumHi

GSNExit:        ret
                endp

                end
