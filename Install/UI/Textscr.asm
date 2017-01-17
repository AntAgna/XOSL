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
TextScreenPtr   equ     0b8000000h

TextScreenSeg   dw      0b800h
                .data?

CursorPos       dw      ?
CursorSize      dw      ?
ScreenBuffer    db      160 * 25 dup (?)
                .code

                extrn   _MemCopy: near

                public  @CTextScreen@$bdtr$qv
                public  @CTextScreen@$bctr$qi

                public  @CTextScreen@PutStr$qiinxzci
                public  @CTextScreen@PutChar$qiiii
                public  @CTextScreen@FillX$qiiiii
                public  @CTextScreen@FillY$qiiiii
                public  @CTextScreen@PutImage$qiiiinus
                public  @CTextScreen@GetImage$qiiiinus

;CTextScreen(int Attr)
@CTextScreen@$bctr$qi   proc    c

                arg     @@this: dword, @@Attr: word

                push    di

                ;get cursor pos en size
                mov     ah,3
                xor     bh,bh
                int     10h
                mov     CursorPos,dx
                mov     CursorSize,cx

                ;hide cursor
                mov     ax,0103h
                mov     cx,2000h
                int     10h

                ;backup text screen
                push    160 * 25
                push    TextScreenPtr
                push    ds
                push    offset ScreenBuffer
                call    _MemCopy
                add     sp,10

                ;clear screen
                mov     es,TextScreenSeg
                xor     di,di
                mov     cx,80 * 25
                mov     ax,@@Attr
                cld
                rep     stosw

                pop     di
                ret
                endp                

;~ClearScreen()
@CTextScreen@$bdtr$qv   proc    c

                ;restore text screen
                push    160 * 25
                push    ds
                push    offset ScreenBuffer
                push    TextScreenPtr
                call    _MemCopy
                add     sp,10

                ;restore cursor pos
                mov     ah,2
		xor	bh,bh
                mov     dx,CursorPos
                int     10h

                ;restore cursor size
                mov     ah,1
                mov     cx,CursorSize
                int     10h

                ret
                endp

;void CTextScreen::PutStr(int X, int Y, int Attr, const char *Str);
@CTextScreen@PutStr$qiinxzci proc    c
                arg     @@this: dword, @@X: word, @@Y: word, @@Str: dword, @@Attr: word

                push    si di ds

                mov     es,TextScreenSeg
                mov     di,@@Y
                imul    di,80
                add     di,@@X
                shl     di,1
                lds     si,@@Str
                mov     ax,@@Attr
                jmp     PSXYGetChar

PSXYPutChar:    stosw
PSXYGetChar:    lodsb
                or      al,al
                jnz     PSXYPutChar

                pop     ds di si
                ret
                endp

;void CTextScreen::PutChar(int X, int Y, int Ch, int Attr);
@CTextScreen@PutChar$qiiii proc c
                arg     @@this: dword, @@X: word, @@Y: word
                arg     @@Ch: word, @@Attr: word

                mov     es,TextScreenSeg
                mov     bx,@@Y
                imul    bx,80
                add     bx,@@X
                shl     bx,1
                mov     ax,@@Ch
                or      ax,@@Attr
                mov     es:[bx],ax
                ret
                endp

;void CTextScreen::FillX(int X, int Y, int Ch, int Attr, int Count);
@CTextScreen@FillX$qiiiii proc  c
                arg     @@this: dword, @@X: word, @@Y: word
                arg     @@Ch: word, @@Attr: word, @@Count: word

                push    di

                mov     es,TextScreenSeg
                mov     di,@@Y
                imul    di,80
                add     di,@@X
                shl     di,1

                mov     ax,@@Ch
                or      ax,@@Attr
                mov     cx,@@Count
                cld
                rep     stosw

                pop     di
                ret
                endp

;void FillY(int X, int Y, int Ch, int Attr, int Count);
@CTextScreen@FillY$qiiiii proc  c
                arg     @@this: dword, @@X: word, @@Y: word
                arg     @@Ch: word, @@Attr: word, @@Count: word

                mov     es,TextScreenSeg
                mov     bx,@@Y
                imul    bx,80
                add     bx,@@X
                shl     bx,1

                mov     ax,@@Ch
                or      ax,@@Attr
                mov     cx,@@Count
                inc     cx
                jmp     FYTestEnd

FYDrawChar:     mov     es:[bx],ax
                add     bx,160

FYTestEnd:      loop    FYDrawChar
                ret
                endp

;void PutImage(int X, int Y, int Width, int Height, unsigned short *Image);
@CTextScreen@PutImage$qiiiinus proc c
                arg     @@this: dword, @@X: word, @@Y: word
                arg     @@Width: word, @@Height: word, @@Image: dword

                push    si di ds

                mov     es,TextScreenSeg
                mov     di,@@Y
                imul    di,80
                add     di,@@X
                shl     di,1
                lds     si,@@Image
                cld
                jmp     PITestEnd

PIDrawLine:     push    di
                mov     cx,@@Width
                rep     movsw
                pop     di
                add     di,160

PITestEnd:      dec     @@Height
                jns     PIDrawLine


                pop     ds di si
                ret
                endp


;void GetImage(int X, int Y, int Width, int Height, unsigned short *Image);
@CTextScreen@GetImage$qiiiinus proc c
                arg     @@this: dword, @@X: word, @@Y: word
                arg     @@Width: word, @@Height: word, @@Image: dword

                push    si di ds

                mov     ds,TextScreenSeg
                mov     si,@@Y
                imul    si,80
                add     si,@@X
                shl     si,1

                les     di,@@Image
                cld
                jmp     GITestEnd

GIDrawLine:     push    si
                mov     cx,@@Width
                rep     movsw
                pop     si
                add     si,160

GITestEnd:      dec     @@Height
                jns     GIDrawLine


                pop     ds di si
                ret
                endp
                end
