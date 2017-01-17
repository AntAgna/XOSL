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

                public  @LBARead$qimx10TLBAPacket


;void LBARead(int Drive, const TLBAPacket &LBAPacket)
@LBARead$qimx10TLBAPacket proc c
                arg     @@Drive: word, @@LBAPacket: dword

                push    si ds

                mov     ah,42h
                mov     dx,@@Drive
                lds     si,@@LBAPacket
                int     13h

                pop     ds si
                ret
                endp

		end
