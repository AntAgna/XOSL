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

;void memcpy (void far *dest, void far *str, U16B size);
                public  _memcpy
_memcpy         proc    
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds
                les     di,[bp + 4]
                lds     si,[bp + 8]
                mov     cx,[bp + 12]
                shr     cx,1
                cld
                rep     movsw
                jnc     MemCpyDone
                movsb

MemCpyDone:     pop     ds
                pop     di
                pop     si
                pop     bp
                ret
_memcpy         endp

;void memset(void far *dest, U8B value, U16B count);
                public  _memset
_memset         proc    
                push    bp
                mov     bp,sp
                push    di
                les     di,[bp + 4]
                mov     ax,[bp + 8]
                mov     ah,al
                mov     cx,[bp + 10]
                shr     cx,1
                cld
                rep     stosw
                jnc     MemSetDone
                stosb

MemSetDone:     pop     di
                pop     bp
                ret
_memset         endp

;int memcmp(const void far *s1, const void far *s2, U16B count); 
                public  _memcmp
_memcmp         proc    
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds

                xor     ax,ax
                les     di,[bp + 4]
                lds     si,[bp + 8]
                mov     cx,[bp + 12]
                cld
                rep     cmpsb
                setne   al
                pop     ds
                pop     di
                pop     si
                pop     bp
                ret
_memcmp         endp


                end
