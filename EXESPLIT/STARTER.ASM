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
                .stack  16384
                .386p
                .code
                extrn   _main: near

                .startup
                call    _main
                xor     ax,ax
                .exit

                end
