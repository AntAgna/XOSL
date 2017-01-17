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

                public  @CApplication@Reboot$qv

;CApplication::Reboot();
@CApplication@Reboot$qv proc c
                xor     ax,ax
                mov     ds,ax
                mov     ds:word ptr [0472h],0000h   ;do a cold reboot...just for sure
                db      0eah
                dd      0f000fff0h
                endp
                end
