;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

;Draw Cursor eXtension

                .model  compact
                .386p

                assume  cs:FARCODE

FARCODE         segment word public use16 'CODE'

                public  _CreateCursor, _InvertCursor

OutlineColor    dw      0000h
CursorColor     dw      0f0fh


VLine           macro   Left, Top, Height
                mov     edi,Left + Top * 11
                mov     cx,Height
                call    DCXVLine
                endm

HLine           macro   Left, Top, Width
                mov     edi,Left + Top * 11
                mov     ecx,Width
                call    DCXHLine
                endm

PPixel          macro   Left, Top
                mov     [ebx + Left + Top * 11],al
                endm

P2Pixels        macro   Left, Top
                mov     [ebx + Left + Top * 11],ax
                endm


;void CreateCursor(unsigned long CursorFGnd);
_CreateCursor   proc
                push    bp
                mov     bp,sp
                push    edi
                push    ds
                push    es

                mov     ax,gs
                mov     ds,ax
                mov     es,ax
                mov     ebx,[bp + 4]

                mov     ax,cs:CursorColor
                VLine   1,2,11
                VLine   2,3,9
                VLine   3,4,7
                VLine   4,5,7
                VLine   5,6,8
                VLine   6,7,9
                VLine   7,14,4
                PPixel  7,8
                PPixel  7,9
                PPixel  8,16
                PPixel  8,17
                PPixel  8,9

                mov     ax,cs:OutlineColor
                VLine   0,0,15
                PPixel  4,12
                PPixel  4,13
                PPixel  5,14
                PPixel  5,15
                PPixel  6,16
                PPixel  6,17
                PPixel  7,12
                PPixel  7,13
                PPixel  8,14
                PPixel  8,15
                PPixel  9,16
                PPixel  9,17
                HLine   6,10,5
                P2Pixels 7,18

                PPixel  6,11

                mov     edi,ebx
                add     edi,9 + 11 * 9

CCDio1:         mov     [edi],al
                sub     edi,1 + 11 * 1
                cmp     edi,ebx
                jne     CCDio1


                mov     edi,1 + 11 * 13

CCDio2:         mov     [ebx + edi],al
                sub     di,10
                cmp     di,4 + 11 * 10
                jne     CCDio2




                pop     es
                pop     ds
                pop     edi
                pop     bp
                ret
_CreateCursor   endp

DCXVLine        proc
                add     edi,ebx

DCXVLLoop:      mov     [edi],al
                add     edi,11
                loop    DCXVLLoop
                ret
DCXVLine        endp

DCXHLine        proc
                add     edi,ebx
                cld
                rep     stos es:byte ptr [edi]
                ret
DCXHLine        endp

;void InvertCursor(int Invert)
_InvertCursor   proc
                push    bp
                mov     bp,sp
                cmp     word ptr [bp + 6],0
                jne     ICInverted
                mov     cs:OutlineColor,0000h
                mov     cs:CursorColor,0f0fh
                jmp     ICDone

ICInverted:     mov     cs:OutlineColor,0f0fh
                mov     cs:CursorColor,0000h

ICDone:         pop     bp
                retf
_InvertCursor   endp

FARCODE         ends

                end
