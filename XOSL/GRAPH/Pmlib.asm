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


                public  _PMMemCopy

;void PMMemCopy(unsigned long Dest, unsigned long Src, unsigned long Count)
_PMMemCopy      proc
                push    bp
                mov     bp,sp
                push    esi
                push    edi
                push    es

                push    gs
                pop     es
                mov     edi,[bp + 4]
                mov     esi,[bp + 8]
                mov     ecx,[bp + 12]
                shr     ecx,1
                cld
                rep     movs es:word ptr [edi],gs:[esi]
                jnc     PMMCDone
                movs    es:byte ptr [edi],gs:[esi]

PMMCDone:       pop     es
                pop     edi
                pop     esi
                pop     bp
                ret
_PMMemCopy      endp

FARCODE         ends

                end


