;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

                .model  large
		.386p
		.data
;Scratchpad      dd      90008000h
Scratchpad      dd      00008000h
                .code

                public  @CDiskAccess@$bctr$qv
                public  @CDiskAccess@$bdtr$qv
                public  @CDiskAccess@DriveCount$qi
                public  @CDiskAccess@Transfer$qiususnvi
                public  @CDiskAccess@GetDriveInfo$qimit2
                public  @CDiskAccess@CopyFromScratchpad$qnvi
                public  @CDiskAccess@CopyToScratchpad$qnxvi

;CDiskAccess::CDiskAccess()
@CDiskAccess@$bctr$qv proc
                ret
                endp

;CDiskAccess::~CDiskAccess()
@CDiskAccess@$bdtr$qv proc
                ret
                endp

;int CDiskAccess::DriveCount(int Drive)
@CDiskAccess@DriveCount$qi proc c
                arg     @@this: dword, @@Drive: word

		mov     ah,8
                mov     dl,byte ptr @@Drive
		int     13h
		movzx   ax,dl
                ret
                endp

;int CDiskAccess::Transfer(int Action, unsigned short SectCyl,
;                          unsigned short DrvHead, void *Buffer, int Count);
@CDiskAccess@Transfer$qiususnvi proc c
                arg     @@this: dword, @@Action: word, @@SectCyl: word
                arg     @@DrvHead: word, @@Buffer: dword, @@Count: word

                test    byte ptr @@DrvHead,80h
		jz      FloppyTransfer

                mov     ax,@@Action
                or      ax,@@Count
                les     bx,@@Buffer
                mov     cx,@@SectCyl
                mov     dx,@@DrvHead

		int     13h
                sbb     ax,ax
                jmp     TransDone

FloppyTransfer:
                push    di
                jmp     TransTestEnd

TransLoop:      mov     di,3

TransSector:    mov     ax,@@Action
                mov     al,1
                les     bx,@@Buffer
                mov     cx,@@SectCyl
                mov     dx,@@DrvHead
                int     13h
                jnc     TransNextSect
                xor     ah,ah                   ;reset drive
                mov     dl,byte ptr @@DrvHead            
                int     13h
                dec     di
                jns     TransSector

                mov     ax,-1
                jmp     FloppyDone

TransNextSect:  add     word ptr @@Buffer,512

TransTestEnd:   dec     @@Count
                jns     TransLoop
                xor     ax,ax
FloppyDone:     pop     di

TransDone:      ret
                endp

;int CDiskAccess::GetDriveInfo(int Drive, int &Heads, int &Sectors);
@CDiskAccess@GetDriveInfo$qimit2 proc c
                arg     @@this: dword, @@Drive: word
                arg     @@Heads: dword, @@Sectors: dword

		push    di

		mov     ah,8
                mov     dl,byte ptr @@Drive
		int     13h
                sbb     ax,ax
                les     bx,@@Sectors
		and     cx,3fh
		mov     es:[bx],cx
                les     bx,@@Heads
		shr     dx,8
		inc     dx
		mov     es:[bx],dx

		pop     di
                ret
                endp

;void CDiskAccess::CopyFromScratchpad(void *Buffer, int Sectors)
@CDiskAccess@CopyFromScratchpad$qnvi proc c
                arg     @@this: dword, @@Buffer: dword, @@Sectors: word

                push    si di ds

		xor     esi,esi
		xor     edi,edi

		lds     si,Scratchpad
                les     di,@@Buffer

                movzx   ecx,@@Sectors
		shl     cx,7

		cld
		rep     movsd

                pop     ds di si
                ret
                endp

;void CDiskAccess::CopyToScratchpad(void *Buffer, int Sectors)
@CDiskAccess@CopyToScratchpad$qnxvi proc c
                arg     @@this: dword, @@Buffer: dword, @@Sectors: word

                push    si di ds

		xor     esi,esi
		xor     edi,edi

		les     di,Scratchpad
                lds     si,@@Buffer

                movzx   ecx,@@Sectors
		shl     cx,7

		cld
		rep     movsd

                pop     ds di si
                ret
                endp

		end
