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
		.data?
LockedData	db	4000 dup (?)		
                .code
		public	@ClearScreen$qv
		public	@LockScreen$qv
		public	@UnlockScreen$qv

;void ClearScreen();
@ClearScreen$qv	proc
		push	edi
		
		mov	ax,0b800h
		mov	es,ax
		xor	edi,edi
		mov	eax,1f201f20h
		mov	ecx,1000
		cli
		rep	stosd
		
		;set cursor pos to (0,0)
                mov     ah,2
		xor	bh,bh
                xor     dx,dx
                int     10h

		pop	edi
		ret
		endp


@LockScreen$qv	proc
		push	edi
		push	esi
		push	ds

		mov	ax,ds
		mov	es,ax
		mov	ax,0b800h
		mov	ds,ax
		
		xor	esi,esi
		mov	edi,offset LockedData
		
		mov	ecx,1000
		cli
		rep	movsd
		

		pop	ds
		pop	esi
		pop	edi
		ret
		endp

@UnlockScreen$qv proc
		push	edi
		push	esi
		
		mov	ax,0b800h
		mov	es,ax
		
		mov	esi,offset LockedData
		xor	edi,edi
		
		mov	ecx,1000
		cli
		rep	movsd
		
		pop	esi
		pop	edi
		
		ret
		endp


                end
