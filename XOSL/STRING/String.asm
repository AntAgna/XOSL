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
Digits          db      '0123456789abcdef'
                .code
;int strlen(char far *str);
                public  _strlen
_strlen         proc    far
                
                push    bp
                mov     bp,sp
                push    di
                les     di,[bp + 6]
                mov     cx,0ffffh
                xor     al,al
                cld
                repne   scasb
                mov     ax,0fffeh
                sub     ax,cx
                pop     di
                pop     bp
                retf
_strlen         endp

;char far *strcpy(char far *dest, char far *src)
                public  _strcpy
_strcpy         proc    far
                push    bp
                mov     bp,sp
                push    ds
                push    si
                push    di
                lds     si,[bp + 10]
                les     di,[bp + 6]
                cld

SCCopy:         lodsb
                stosb
                or      al,al
                jnz     SCCopy
                mov     ax,[bp + 6]
                mov     dx,[bp + 8]
                pop     di
                pop     si
                pop     ds
                pop     bp
                retf
_strcpy         endp

;char far *strcat(char far *dest, char far *src)
                public  _strcat
_strcat         proc    far
                push    bp
                mov     bp,sp

                push    dword ptr [bp + 6]
                call    _strlen
                pop     ecx
                mov     ecx,[bp + 6]
                add     cx,ax
                push    dword ptr [bp + 10]
                push    ecx
                call    _strcpy
                add     sp,8
                mov     ax,[bp + 6]
                mov     dx,[bp + 8]
                pop     bp
                retf
_strcat         endp

;char far *strchr(char far *str, int c);
                public  _strchr
_strchr        proc     far
                push    bp
                mov     bp,sp
                push    si
                les     si,[bp + 6]
                mov     ah,[bp + 10]
                jmp     SC2

SC1:            cmp     al,ah
                je      SCFound
                inc     si
SC2:            mov     al,es:[si]
                or      al,al
                jnz     SC1
                xor     ax,ax
                xor     dx,dx
                jmp     SCEnd

SCFound:        mov     ax,si
                mov     dx,es

SCEnd:          pop     si
                pop     bp
                retf
_strchr        endp

;char far *itoa(long value, char far *str, int base);
;                       4       8               12
                public  _itoa
_itoa           proc    far
                enter   33,0
                push    si
                push    di
                mov     si,-33
                les     di,[bp + 10]
                mov     eax,[bp + 6]
                movzx   ecx,word ptr [bp + 14]

                or      eax,eax
                jnl     EAXPos
                push    word ptr '-'
                neg     eax
                jmp     LDigits

EAXPos:         push    word ptr 0

LDigits:        xor     edx,edx
                div     ecx
                mov     bx,dx
                mov     bl,[bx + Digits]
                mov     [bp + si],bl
                inc     si
                or      eax,eax
                jnz     LDigits

                cld
                pop     ax
                or      ax,ax
                jz      LInvert
                mov     [bp + si],ax
                inc     si

LInvert:        dec     si
                mov     al,[bp + si]
                stosb
                cmp     si,-33
                jne     LInvert
                mov     es:byte ptr [di],0
                mov     ax,[bp + 10]
                mov     dx,[bp + 12]
                pop     di
                pop     si
                leave
                retf
_itoa           endp

;int strcmp(const char far *s1, const char far *s2);
                public  _strcmp
_strcmp         proc    far
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds

                cld
                xor     ax,ax
                xor     bx,bx
                les     di,[bp + 10]
                push    di
                mov     cx,0ffffh
                repne   scasb
                not     cx
                pop     di
                lds     si,[bp + 6]
                rep     cmpsb
                mov     al,[si - 1]
                mov     bl,[di - 1]
                sub     ax,bx
                pop     ds
                pop     di
                pop     si
                pop     bp
                retf
_strcmp         endp

;int stricmp(const char far *s1, const char far *s2);
                public  _stricmp
_stricmp        proc    far
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds

                cld
                les     si,[bp + 6]
                les     di,[bp + 10]
                xor     ax,ax
                xor     bx,bx
                mov     cx,617ah

SICCompLoop:    lodsb
                mov     bl,es:[di]
                or      al,al
                jz      SICEnd
                scasb
                je      SICCompLoop
                cmp     al,ch
                jb      SICCheckCap
                cmp     al,cl
                ja      SICEnd
                sub     al,20h
                jmp     SICncComp

SICCheckCap:    cmp     bl,ch
                jb      SICEnd
                cmp     bl,cl
                ja      SICEnd
                sub     bl,20h

SICncComp:      cmp     al,bl
                je      SICCompLoop

SICEnd:         sub     ax,bx
                pop     ds
                pop     di
                pop     si
                pop     bp
                retf
_stricmp        endp

;long atol(const char far *str);
                public  _atol
_atol           proc    far
                push    bp
                mov     bp,sp
                push    si

                cld
                les     si,[bp + 6]
                xor     edx,edx
                xor     eax,eax
                jmp     ALGetChar

ALConvert:      sub     al,'0'
                imul    edx,10
                add     edx,eax

ALGetChar:      lods    es:byte ptr [si]
                or      al,al
                jnz     ALConvert

                mov     ax,dx
                shr     edx,16

                pop     si
                pop     bp
                retf
_atol           endp

                end
