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

;int GetSVGAInfo(TSVGAInfo *Buffer)
                public  _GetSVGAInfo
_GetSVGAInfo    proc    far
                push    bp
                mov     bp,sp
                push    di
                mov     ax,4f00h
                les     di,[bp + 6]
                int     10h
                xor     ax,004fh
                pop     di
                pop     bp
                retf
_GetSVGAInfo    endp

;int GetModeInfo(int Mode, TModeInfo far *ModeInfo);
                public  _GetModeInfo
_GetModeInfo    proc    far
                push    bp
                mov     bp,sp
                push    di
                mov     ax,4f01h
                mov     cx,[bp + 6]
                les     di,[bp + 8]
                int     10h
                xor     ax,004fh
                pop     di
                pop     bp
                retf
_GetModeInfo    endp

;int SwitchTo(unsigned short Mode)
                public  _SwitchTo
_SwitchTo       proc    far
                push    bp
                mov     bp,sp
                mov     ax,4f02h
                mov     bx,[bp + 6]
                int     10h
                xor     ax,004fh
                pop     bp
                retf
_SwitchTo       endp

FARCODE         ends

                end
