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

                .model  large
                .386p
                .code

                public  __npure_error_
__npure_error_  proc    near
                ret
__npure_error_  endp

                public  __fpure_error_
__fpure_error_  proc    far
                retf
__fpure_error_  endp


                public  @ClearCDNW$qv
@ClearCDNW$qv   proc
;                mov     eax,cr0
;                and     eax,9fffffffh
;                mov     cr0,eax
                ret
                endp

                end
