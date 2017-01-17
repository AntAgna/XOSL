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

                public  @CData@GetDriveCount$qv
                public  @CData@GetDosVersion$qv
                
;int CData::GetDriveCount();
@CData@GetDriveCount$qv proc
                mov     ah,52h
                int     21h
                movzx   ax,es:byte ptr [bx + 20h]
                ret
                endp

;unsigned short CData::GetDosVersion();
@CData@GetDosVersion$qv proc
                mov     ah,30h
                int     21h
                ret
                endp

                end
                
