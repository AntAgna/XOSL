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

                public  @CDiskAccess@LBAAccessAvail$qi
                public  @CDiskAccess@LBATransfer$qiimx10TLBAPacket
                public  @CDiskAccess@SetLockStatus$qii

;int CDiskAccess::LBAAccessAvail(int Drive)
@CDiskAccess@LBAAccessAvail$qi proc c
                arg     @@this: dword, @@Drive: word

                mov     ah,41h
                mov     bx,55aah
                mov     dx,@@Drive
                int     13h
                jc      NoLBA
                cmp     bx,0aa55h
                jne     NoLBA
                test    cx,1
                jz      NoLBA
                xor     ax,ax
                jmp     LBA_AAEnd

NoLBA:          mov     ax,-1
LBA_AAEnd:      ret
                endp

;int CDiskAccess::LBATransfer(int Action, int Drive, const TLBAPacket &LBAPacket)
@CDiskAccess@LBATransfer$qiimx10TLBAPacket proc c
                arg     @@this: dword, @@Action: word
                arg     @@Drive: word, @@LBAPacket: dword

                push    si ds

                mov     ax,@@Action
                or      ax,4000h
                mov     dx,@@Drive
                lds     si,@@LBAPacket
                int     13h
                sbb     ax,ax

                pop     ds si
                ret
                endp

;void CDiskAccess::SetLockStatus(int Drive, int Status)
@CDiskAccess@SetLockStatus$qii proc c
                arg     @@this: dword
                arg     @@Drive: word, @@Status: word

                mov     ah,45h
                mov     al,byte ptr @@Status
                mov     dl,byte ptr @@Drive
                int     13h
                ret
                endp

		end
