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
                .data?

                public  @CPowerManagement@APMAvailable
                public  _PowerManagement

@CPowerManagement@APMAvailable dw ?
_PowerManagement        dd      ?

                .code

                public  @CPowerManagement@APMInstalled$qv
                public  @CPowerManagement@Connect$qv
                public  @CPowerManagement@SetPowerState$qii

;static int CPowerManagement::APMInstalled();
@CPowerManagement@APMInstalled$qv proc
                mov     ax,5300h
                xor     bx,bx
                int     15h
                sbb     ax,ax
                inc     ax
                ret
                endp

;static int CPowerManagement::Connect();
@CPowerManagement@Connect$qv proc
                mov     ax,5301h
                xor     bx,bx
                int     15h
                sbb     ax,ax
                ret
                endp

;static int CPowerManagement::SetPowerState(int DevId, int State);
@CPowerManagement@SetPowerState$qii proc c
                arg     @@DevId: word, @@State: word

                cmp     @CPowerManagement@APMAvailable,0
                jne     SPSProceed
                mov     ax,-1
                jmp     SPSExit

SPSProceed:     mov     ax,5307h
                mov     bx,@@DevId
                mov     cx,@@State
                int     15h
                sbb     ax,ax

SPSExit:        ret
                endp

                end
