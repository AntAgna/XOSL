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


                public  @CVesa@GetSvgaInfo$qm15CVesa@CSvgaInfo
                public  @CVesa@GetModeInfo$qim15CVesa@CModeInfo

;static int CVesa::GetSvgaInfo(CVesa::CSvgaInfo &SvgaInfo);
@CVesa@GetSvgaInfo$qm15CVesa@CSvgaInfo proc c
                arg     @@SvgaInfo: dword

                push    di

                mov     ax,4f00h
                les     di,@@SvgaInfo
                int     10h
                cmp     ax,004fh
                je      GSISuccess
                mov     ax,-1
                jmp     GSIExit

GSISuccess:     xor     ax,ax
GSIExit:        pop     di
                ret
                endp

;int CVesa::GetModeInfo(int VesaMode, CModeInfo &ModeInfo)
@CVesa@GetModeInfo$qim15CVesa@CModeInfo proc c

                arg     @@VesaMode: word, @@ModeInfo: dword

                push    di
                mov     ax,4f01h
                mov     cx,@@VesaMode
                les     di,@@ModeInfo
                int     10h
                xor     ax,004fh
                jz      MIExit
                mov     ax,-1

MIExit:         pop     di
                ret
                endp



                end
