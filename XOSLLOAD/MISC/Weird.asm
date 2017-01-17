;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

; Weird stuff to satisfy Borland C++ 

                .model  compact
                .386p
                .code

                public  __npure_error_
__npure_error_  proc
                ret
__npure_error_  endp

                end
