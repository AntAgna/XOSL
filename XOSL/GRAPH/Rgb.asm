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

                assume  cs:FARCODE
FARCODE         segment word public use16 'CODE'

;void SetRGB(int Index, int Red, int Green, int Blue);
                public  _SetRGB
_SetRGB         proc    far
                push    bp
                mov     bp,sp
                mov     dx,03c8h
                mov     al,[bp + 6]
                out     dx,al
                inc     dx
                mov     al,[bp + 8]
                out     dx,al
                mov     al,[bp + 10]
                out     dx,al
                mov     al,[bp + 12]
                out     dx,al
                pop     bp
                retf
_SetRGB         endp

;void far GetRGB(int Index, int &Red, int &Green, int &Blue);
                public  _GetRGB
_GetRGB         proc    far
                push    bp
                mov     bp,sp
                mov     dx,03c7h
                mov     al,[bp + 6]
                out     dx,al
                mov     dl,0c9h
                xor     ax,ax
                les     bx,[bp + 8]
                in      al,dx
                mov     es:[bx],ax
                les     bx,[bp + 12]
                in      al,dx
                mov     es:[bx],ax
                les     bx,[bp + 16]
                in      al,dx
                mov     es:[bx],ax
                pop     bp
                retf
_GetRGB         endp

FARCODE         ends
                end
