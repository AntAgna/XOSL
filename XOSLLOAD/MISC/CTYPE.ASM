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

                public  @tolower$qi

;int tolower(int ch);
@tolower$qi     proc    c
                arg     @@ch: word

                mov     ax,@@ch
                cmp     ax,'A'
                jb      TLExit
                cmp     ax,'Z'
                ja      TLExit
                or      ax,20h
TLExit:         ret
                endp

                end
