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

                .code

                public  @CKeyboard@$bctr$qv
                public  @CKeyboard@$bdtr$qv
                public  @CKeyboard@GetCh$qv
                public  @CKeyboard@KbHit$qv
                public  @CKeyboard@Flush$qv
                public  @CKeyboard@PutCh$qi

;constructure and destructor are not needed
@CKeyboard@$bctr$qv:
@CKeyboard@$bdtr$qv: ret


;int CKeyboard::Getch()
@CKeyboard@GetCh$qv     proc
                xor     ah,ah
                int     16h
                ret
                endp

;int CKeyboard::KbHit()
@CKeyboard@KbHit$qv     proc
                mov     ah,1
                int     16h
                jz      KBHNoKey
                mov     ax,0ffffh
                ret
KBHNoKey:       xor     ax,ax
                ret
                endp


;void CKeyboard::Flush()
@CKeyboard@Flush$qv     proc
                jmp     FCheckBuffer
FGetKey:        xor     ah,ah
                int     16h
FCheckBuffer:   mov     ah,1
                int     16h
                jnz     FGetKey
                ret
                endp

;void CKeyboard::StoreCh()
@CKeyboard@PutCh$qi     proc    c
                arg     @@this: dword, @@Key: word

                mov     ah,5
                mov     cx,@@Key
                int     16h
                ret
                endp

                end
