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
                public  _PutS, _GetCh, _PutCh

;void PutS(char *Str);
_PutS           proc    
                push    bp
                mov     bp,sp
                push    si
                les     si,[bp + 4]
                cld
                jmp     PSLods

Teletype:       mov     ah,0eh
                mov     bx,7
                int     10h
PSLods:         lods    es:byte ptr [si]
                or      al,al
                jnz     Teletype
                pop     si
                pop     bp
                ret
_PutS           endp

;void PutCh(int Ch);
_PutCh          proc    c
                arg     @@ch: word

                mov     ah,0eh
                mov     al,byte ptr @@ch
                mov     bx,7
                int     10h
                ret
                endp



;int GetCh(void);
_GetCh          proc
                xor     ah,ah
                int     16h
                ret
_GetCh          endp

                end
