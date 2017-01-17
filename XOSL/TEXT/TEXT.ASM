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
                .386p
                .code

                public  @gotoxy$qii
                public  @wherexy$qnit1
                public  @puts$qnxzc
                public  @putch$qi

;void gotoxy(int x, int y)
@gotoxy$qii     proc    c
                arg     @@x: word, @@y: word

                mov     dl,byte ptr @@x
                mov     dh,byte ptr @@y
                mov     ah,2
                xor     bh,bh
                int     10h
                ret
                endp

;void wherexy(int *x, int *y)
@wherexy$qnit1  proc    c
                arg     @@x:  dword, @@y: dword

                mov     ah,3
                xor     bh,bh
                int     10h
                les     bx,@@x
                movzx   ax,dl
                mov     es:[bx],ax
                les     bx,@@y
                mov     al,dh
                mov     es:[bx],ax
                ret
                endp

;void puts(const char *str);
@puts$qnxzc     proc    c
                arg     @@str: dword

                push    si
                les     si,@@str
                cld
                jmp     PSLods

Teletype:       mov     ah,0eh
                mov     bx,7
                int     10h
PSLods:         lods    es:byte ptr [si]
                or      al,al
                jnz     Teletype
                pop     si
                ret
                endp

;void putch(int ch);
@putch$qi       proc    c
                arg     @@ch: word

                mov     ah,0eh
                mov     al,byte ptr @@ch
                mov     bx,7
                int     10h
                ret
                endp

                end
