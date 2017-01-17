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

                public  @PS2Initialize$qi
                public  @PS2Call$qusi
                public  @PS2SetHandler$qnqszczcs$v

;int PS2Initialize(int PackSize)
@PS2Initialize$qi proc c
                arg     @@PackSize: word

                mov     ax,0c205h
                mov     bh,byte ptr @@PackSize
                int     15h
                or      ah,ah
                jnz     PS2IErr
                mov     ax,0c201h
                int     15h
                or      ah,ah
                jz      PS2IEnd

PS2IErr:        mov     ah,1
PS2IEnd:        shr     ax,8
                ret
                endp

;void PS2Call(U16B Func, int Value)
@PS2Call$qusi   proc    c
                arg     @@Func: word, @@Value: word

                mov     ax,@@Func
                mov     bh,byte ptr @@Value
                int     15h
                ret
                endp

;void PS2SetHandler(TPS2Handler Handler)
@PS2SetHandler$qnqszczcs$v proc c
                arg     @@Handler: dword

                mov     ax,0c207h
                les     bx,@@Handler
                int     15h
                ret
                endp

                end

