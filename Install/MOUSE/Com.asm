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
                .code

                public  @CMouse@MouseAvailable$qi

;CMouse::MouseAvailable(int PortAddr);
@CMouse@MouseAvailable$qi proc c
                arg     @@PortAddr: word

                mov     dx,@@PortAddr
                or      dx,dx
                jz      _52ed
                add     dx,2
                in      al,dx
                call    Timeout
                and     al,0f8h
                je      _52de
                and     al,0c0h
                je      _52ed
                xor     al,al
                out     dx,al
                call    Timeout
                in      al,dx
                call    Timeout
                and     al,0f8h
                jne     _52ed
                xor     al,al

_52de:          sub     dx,2
                call    TestAndSet
                mov     al,1
                jnb     _52ea
                xor     al,al
_52ea:          jmp     _52f4
_52ed:          mov     al,0ffh
_52f4:          xor     ah,ah
                movsx   ax,al
                ret
                endp
                

TestAndSet      proc
                push    cx
                push    dx
                push    si
                push    ds
                mov     bx,dx
                add     dx,3
                mov     al,80h
                out     dx,al
                jmp     _5853
_5853:          sub     dx,2
                mov     al,0
                out     dx,al
                jmp     _585b
_585b:          dec     dx
                mov     al,60h
                out     dx,al
                jmp     _5861
_5861:          add     dx,3
                mov     al,6
                out     dx,al
                jmp     _5869
_5869:          sub     dx,2
                xor     al,al
                out     dx,al
                add     dx,3
                mov     al,0
                out     dx,al
                jmp     _5877
_5877:          mov     ax,40h
                mov     ds,ax
                cli
                mov     cl,ds:[006ch]
                add     cl,9
                sti
_5885:          cmp     cl,ds:[006ch]
                jne     _5885

                mov     al,1
                out     dx,al
                jmp     _5890
_5890:          mov     dx,bx
                cli
                mov     cl,ds:[006ch]
                add     cl,5
                sti
_589b:          in      al,dx
                cmp     cl,ds:[006ch]
                jne     _589b
                add     dx,4
                mov     al,3
                out     dx,al
                jmp     _58aa
_58aa:          mov     dx,bx
                mov     ah,ds:[006ch]
                add     ah,5
_58b3:          add     dx,5
                in      al,dx
                test    al,1
                je      _58db
                mov     dx,bx
                in      al,dx
                and     al,7fh
                cmp     al,4dh
                jne     _58c9
                mov     ah,2
                jmp     _58e4
_58c9:          cmp     al,6dh
                jne     _58d2
                mov     ah,3
                jmp     _58e4

_58d2:          cmp     al,48h
                jne     _58db
                mov     ah,3
                jmp     _58e4
_58db:          mov     dx,bx
                cmp     ah,ds:[006ch]
                jne     _58b3
                stc
_58e4:          pop     ds
                pop     si
                pop     dx
                pop     cx
                ret
TestAndSet      endp

Timeout         proc
                jmp     TO1
TO1:            jmp     TO2
TO2:            ret
Timeout         endp


                end
