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
                public  @GetTimerTicks$qv

;unsigned long GetTimerTicks(void);
@GetTimerTicks$qv proc
                xor     ax,ax
                mov     es,ax
                mov     ax,es:[046ch]
                mov     dx,es:[046eh]
                ret
                endp

                end

