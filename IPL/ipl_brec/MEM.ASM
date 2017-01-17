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

                
                public  @MemCompare$qnxvt1i

;int MemCompare(const void *Mem1, const void *Mem2, int ByteCount)
@MemCompare$qnxvt1i     proc c
                arg     @@Mem1: dword, @@Mem2: dword
                arg     @@ByteCount: word


                push    si di ds

                xor     ax,ax
                les     di,@@Mem1
                lds     si,@@Mem2
                mov     cx,@@ByteCount
                cld
                rep     cmpsb
                setne   al
                pop     ds di si
                ret
                endp


                end
