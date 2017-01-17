;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

                .model  large
                .code
                public  @DosCreate$qnxzc
                public  @DosClose$qi
                public  @DosWrite$qinxvus

;int DosCreate(const char *FileName)
@DosCreate$qnxzc proc   c
                arg     @@FileName: dword

                push    ds
                mov     ah,3ch
                xor     cx,cx
                lds     dx,@@FileName
                int     21h
                pop     ds
                ret
                endp

;void DosClose(int FileHandle)
@DosClose$qi    proc    c
                arg     @@FileHandle: word

                mov     ah,3eh
                mov     bx,@@FileHandle
                int     21h
                ret
                endp

;void far DosWrite(int FileHandle, const void *Data, unsigned short Size)
@DosWrite$qinxvus proc  c
                arg     @@FileHandle: word, @@Data: dword
                arg     @@Size: word

                push    ds
                mov     ah,40h
                mov     bx,@@FileHandle
                mov     cx,@@Size
                lds     dx,@@Data
                int     21h
                pop     ds
                ret
                endp

                end

