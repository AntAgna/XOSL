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

                assume  cs:FARCODE

FARCODE         segment word public use16 'CODE'

;void WaitVRetrace()
                public  _WaitRetrace
_WaitRetrace    proc
		mov     dx,03dah
Wait1:          in      al,dx
		test    al,8
                jz      Wait1
Wait2:          in      al,dx
		test    al,8
                jnz     Wait2
                retf
_WaitRetrace    endp

FARCODE         ends
		end
