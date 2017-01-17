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
		.data
extrn           _GRWidth: word
extrn           _GRHeight: word
extrn           _GRThisBank: word

                .code
                assume  cs:FARCODE

FARCODE         segment word public use16 'CODE'

		public  _BankSwitch
_BankSwitch     proc
		db      12 dup (90h)
                ret
_BankSwitch     endp

FARCODE         ends

		end
