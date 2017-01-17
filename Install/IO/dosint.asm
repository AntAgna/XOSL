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
ExecParamBlock	dw	0
ExecProgArgsPtr	dd	0
		dd	0
		dd	0
		dd	0
		dd	0


                .code
                public  @CDosFile@Create$qnxzc
                public  @CDosFile@Open$qnxzc11TFileAccess
                public  @CDosFile@Close$qi
                public  @CDosFile@Read$qinvus
                public  @CDosFile@Write$qinxvus
                public  @CDosFile@SetAttrib$qnxzci
                public  @CDosFile@Unlink$qnxzc
                public  @CDosFile@LSeek$qil7TWhence
		

;CDosFile::Create(const char *FileName);
@CDosFile@Create$qnxzc   proc c
                arg     @@FileName: dword

                push    ds
                mov     ah,3ch
                xor     cx,cx
                lds     dx,@@FileName
                int     21h
                jnc     CreatOk
                mov     ax,-1
CreatOk:        pop     ds
                ret
                endp

;int CDosFile::Open(const char *FileName, TFileAccess Access);
@CDosFile@Open$qnxzc11TFileAccess proc c
                arg     @@FileName: dword, @@Access: word

                push    ds
                mov     ah,3dh
                mov     al,byte ptr @@Access
                lds     dx,@@FileName
                xor     cx,cx
                int     21h
                jnc     OpenOk
                mov     ax,-1
OpenOk:         pop     ds
                ret
                endp

;void CDosFile::Close(int Handle);
@CDosFile@Close$qi      proc c
                arg     @@Handle: word

                mov     ah,3eh
                mov     bx,@@Handle
                int     21h
                ret
                endp

;unsigned short CDosFile::Read(int Handle, void *Buffer, unsigned short Length)
@CDosFile@Read$qinvus   proc c
                arg     @@Handle: word, @@Buffer: dword, @@Length: word

                push    ds
                mov     ah,3fh
                mov     bx,@@Handle
                mov     cx,@@Length
                lds     dx,@@Buffer
                int     21h
                jnc     ReadOK
                xor     ax,ax
ReadOk:         pop     ds
                ret
                endp

;unsigned short CDosFile::Write(int Handle, void *Buffer, unsigned short Len);
@CDosFile@Write$qinxvus proc c
                arg     @@Handle: word, @@Buffer: dword, @@Length: word

                push    ds
                mov     ah,40h
                mov     bx,@@Handle
                mov     cx,@@Length
                lds     dx,@@Buffer
                int     21h
                jnc     WriteOK
                xor     ax,ax
WriteOk:        pop     ds
                ret
                endp

;int CDosFile::SetAttrib(const char *FileName, int Attributes)
@CDosFile@SetAttrib$qnxzci proc c
                arg     @@FileName: dword, @@Attributes: word

                push    ds
                mov     ax,4301h
                mov     cx,@@Attributes
                lds     dx,@@FileName
                int     21h
                sbb     ax,ax
                pop     ds
                ret
                endp

;int CDosFile::Unlink(const char *FileName);
@CDosFile@Unlink$qnxzc   proc c
                arg     @@FileName: dword

                push    ds
                mov     ah,41h
                lds     dx,@@FileName
                xor     cx,cx
                int     21h
                sbb     ax,ax
                pop     ds
                ret
                endp

;long CDosFile::LSeek(int Handle, long Offset, TWhence Whence)
@CDosFile@LSeek$qil7TWhence proc c
                arg     @@Handle: word, @@OffsetLow: word
                arg     @@OffsetHigh: word, @@Whence: word

                mov     ah,42h
                mov     al,byte ptr @@Whence
                mov     bx,@@Handle
                mov     cx,@@OffsetHigh
                mov     dx,@@OffsetLow
                int     21h
                jnc     LSeekDone
                mov     ax,-1
                mov     dx,ax

LSeekDone:      ret
                endp


		end
		       
