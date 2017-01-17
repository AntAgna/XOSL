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
                .data
HourSec         dd      3600

                .code

                public  @ResetTimer$qv

;void ResetTimer()
@ResetTimer$qv  proc
                push    bp
                mov     bp,5

GetTime:        mov     ah,2
                int     1ah
                jnc     TimeRead
                dec     bp
                jns     GetTime
                jmp     RTDone

TimeRead:       movzx   bx,ch
                xor     ch,ch
                shr     dx,8
                push    bx
                call    BCD2Hex
                pop     bx
                push    dx
                call    BCD2Hex
                pop     dx
                push    cx
                call    BCD2Hex
                xor     eax,eax
                pop     ax

                imul    ax,60
                add     ax,dx
                imul    eax,65536
                xor     edx,edx
                idiv    HourSec

                xor     cx,cx
                mov     es,cx
                mov     es:[046ch],ax
                mov     es:[046eh],bx

RTDone:         pop     bp
                ret
                endp

BCD2Hex         proc    near
                push    bp
                mov     bp,sp
                mov     ax,[bp + 4]
                and     byte ptr [bp + 4],0fh
                shr     ax,4
                imul    ax,10
                add     [bp + 4],ax
                pop     bp
                ret
BCD2Hex         endp

                end
