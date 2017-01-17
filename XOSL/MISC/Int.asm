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

                public  @GetVect$qi
                public  @SetVect$qinqv$v

;void interrupt (*GetVect(int Vect))(void);
@GetVect$qi     proc    c
                arg     @@Vect: word

                xor     ax,ax
                mov     es,ax
                mov     bx,@@Vect
                shl     bx,2
                mov     ax,es:[bx]
                mov     dx,es:[bx + 2]
                ret
                endp

;void interrupt SetVect(int Vect, void interrupt (*Handler)(void));
@SetVect$qinqv$v proc    c
                arg     @@Vect: word, @@Handler: dword

                xor     ax,ax
                mov     es,ax
                mov     bx,@@Vect
                shl     bx,2
                mov     eax,@@Handler
                cli
                mov     es:[bx],eax
                sti
                ret
                endp

                end
