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
	        public	_MemCopy
		public	_MemSet
		public	_MemCompare 		


;void memcpy (void far *dest, void far *src, U16B size);
                
_MemCopy        proc    c
		arg	@@Dest: dword
		arg	@@Src: dword
		arg	@@Count: word

                push    si
                push    di
                push    ds
                les     di,@@Dest
                lds     si,@@Src
                mov     cx,@@Count
                shr     cx,1
                cld
                rep     movsw
                jnc     MemCpyDone
                movsb

MemCpyDone:     pop     ds
                pop     di
                pop     si
                ret
		endp

;void memset(void far *dest, U8B value, U16B count);
_MemSet        	proc    c
		arg	@@Dest: dword
		arg	@@Value: word
		arg	@@Count: word
		
                push    di
                les     di,@@Dest
                mov     ax,@@Value
                mov     ah,al
                mov     cx,@@Count
                shr     cx,1
                cld
                rep     stosw
                jnc     MemSetDone
                stosb

MemSetDone:     pop     di
                ret
		endp

;int memcmp(const void far *s1, const void far *s2, U16B count); 
_MemCompare	proc    c
		arg	@@Ptr1: dword
		arg	@@Ptr2: dword
		arg	@@Count: word

                push    si
                push    di
                push    ds

                xor     ax,ax
                les     di,@@Ptr1
                lds     si,@@Ptr2
                mov     cx,@@Count
                cld
                rep     cmpsb
                setne   al
                pop     ds
                pop     di
                pop     si
                ret
		endp


                end
