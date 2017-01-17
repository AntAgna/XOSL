;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

;**
; * File: PMGRAPH.ASM
; * Author: Geurt Vos
; * Date: Nov 3, 1998
; * 
; * Takes care of the triple buffering in protected mode
; * 
; * reg |  size  | base (ph) |   limit  | Description
; * ---------------------------------------------------------------
; *  CS | 16-bit |    CODE   | 0000ffff | Real mode code segment  |
; *  DS | 16-bit |    DATA   | 0000ffff | Real mode data segment  |
; *  SS | 16-bit |   STACK   | 0000ffff | Real mode stack segment |
; *  ES | 32-bit | 0012:0000 | 001d5000 | First graphics buffer   |
; *  FS | 16-bit | 000a:0000 | 0000ffff | Graphics framebuffer    |
; *  GS | 32-bit | 0000:0000 | 000fffff | All addressable memory  |
; * 


		.model  compact
		.386p
                .data?
                public  _PhysFontHeader, _PhysFontData, _DataSize

_PhysFontHeader dd      ?
_PhysFontData   dd      ?
_DataSize       dw      ?

PtrX            dd      ?
PtrY            dd      ?
PtrL            dd      ?

AbsX            dd      ?
AbsY            dd      ?

ScreenBuffer    equ     00120000h       ;1875k
BGndBuffer      equ     002f4c00h       ;1875k
FontDataX       equ     00100200h       ;24k
FontDataY       equ     00106200h       ;24k
FontDataL       equ     0010c200h       ;24k
GrTextBuffer    equ     00112200h       ;20k
;ScreenBuffer    equ     00100000h       ;1875k
;BGndBuffer      equ     002d4c00h       ;1875k
;FontDataX       equ     00380200h       ;24k
;FontDataY       equ     00386200h       ;24k
;FontDataL       equ     0038c200h       ;24k
;GrTextBuffer    equ     00392200h       ;20k

		.data
TGraphQueue     struc
	PhysParams      dd      0
	Func            dd      0
TGraphQueue     ends


extrn           _GRWidth: dword
extrn           _GRHeight: dword
extrn           _GRThisBank: word

extrn           _GraphAction: near

extrn           _VPLeft: dword
extrn           _VPTop: dword
extrn           _ClipLeft: dword
extrn           _ClipTop: dword
extrn           _ClipWidth: dword
extrn           _ClipHeight: dword
extrn           _ClipRight: dword
extrn           _ClipBottom: dword

extrn           _AddrLinearFB: dword

                assume  cs:FARCODE

FARCODE         segment word public use16 'CODE'


                extrn   _BankSwitch: near
		extrn   SwitchPM: near
		extrn   SwitchRM: near


;void DoPMGraphics()
		public  _DoPMGraphics
_DoPMGraphics   proc    far
		push    esi
		call    SwitchPM
                mov     esi,seg _GraphAction
		shl     esi,4
                mov     eax,offset _GraphAction
		add     esi,eax

		mov     ebx,gs:[esi].PhysParams
		push    gs:dword ptr [ebx]
		push    gs:dword ptr [ebx + 4]
		push    gs:dword ptr [ebx + 8]
		push    gs:dword ptr [ebx + 12]
		push    gs:dword ptr [ebx + 16]
		call    gs:word ptr [esi].Func
		add     sp,20

		call    SwitchRM
		pop     esi
                retf
_DoPMGraphics   endp

PMBankSwitch    proc
                pushad
                push    es fs
		call    SwitchRM
		xor     bx,bx
                call    _BankSwitch
		call    SwitchPM
                pop     fs es
                popad
		ret
PMBankSwitch    endp


;void FlushArea(long Left, long Top, long Width, long Heigth)
		public  _FlushArea
_FlushArea      proc
		enter   4,0
		push    esi
		push    edi

		push    es              ;swap es and fs
		push    fs
		pop     es
		pop     fs

		cld
                mov     eax,[bp + 8]
                imul    eax,_GRWidth
                add     eax,[bp + 4]
                mov     esi,eax
		shld    edx,eax,16
		movzx   edi,ax

		cmp     _GRThisBank,dx
		je      FADoFlush
		push    eax
		call    PMBankSwitch
		pop     eax

FADoFlush:      mov     eax,_GRWidth
		mov     ecx,[bp + 12]
		sub     eax,ecx
		mov     [bp - 4],eax

FALoop:         push    cx
		mov     ax,di
		add     ax,cx
		jnc     FADraw
		push    ax
		sub     cx,ax

		rep     movs es:byte ptr [edi],fs:byte ptr [esi]

		xor     edi,edi
		pop     cx
		inc     dx
		call    PMBankSwitch
						  
FADraw:
		rep     movs es:byte ptr [edi],fs:byte ptr [esi]
		pop     cx

		add     esi,[bp - 4]
		add     di,[bp - 4]
		jnc     FATestEnd
		inc     dx
		call    PMBankSwitch

FATestEnd:      sub     word ptr [bp + 16],1
                jnz     FALoop
		mov     _GRThisBank,dx

		push    es              ;swap es and fs
		push    fs
		pop     es
		pop     fs

		pop     edi
		pop     esi
		leave
		ret
_FlushArea      endp

;void FlushAreaVBE2(long Left, long Top, long Width, long Height)
                public  _FlushAreaVBE2
_FlushAreaVBE2  proc
                push    bp
                mov     bp,sp
                push    esi
                push    edi
                push    es
                push    gs
                pop     es
                pop     gs

                cld
                mov     esi,[bp + 8]
                imul    esi,_GRWidth
                add     esi,[bp + 4]
                mov     edi,esi

                add     edi,_AddrLinearFB
                mov     eax,_GRWidth
                sub     eax,[bp + 12]
                jmp     FA2TestEnd

FA2DrawLoop:    mov     ecx,[bp + 12]
                shr     ecx,2
                rep     movs es:dword ptr [edi],gs:[esi]
                mov     ecx,[bp + 12]
                and     ecx,3
                jz      FA2RowDone
                rep     movs es:byte ptr [edi],gs:[esi]

FA2RowDone:     add     edi,eax
                add     esi,eax

FA2TestEnd:     dec     word ptr [bp + 16]
                jns     FA2DrawLoop


                push    es
                push    gs
                pop     es
                pop     gs
                pop     edi
                pop     esi
                pop     bp
                ret
_FlushAreaVBE2  endp

;void UltraFlush()
		public  _UltraFlush
_UltraFlush     proc
		push    esi
		push    edi
		push    es
		push    fs
		pop     es
		pop     fs
		cld

		mov     eax,_GRWidth
                imul    eax,_GRHeight
		push    ax              ;byte count last bank
		shr     eax,16

		xor     esi,esi
		mov     ecx,16384
		xor     dx,dx
		cmp     _GRThisBank,dx
		je      UFTestEnd
		call    PMBankSwitch
		jmp     UFTestEnd

UFDoFlush:      xor     edi,edi
		push    cx
		rep     movs es:dword ptr [edi],fs:[esi]
		pop     cx
		inc     dx
		call    PMBankSwitch

UFTestEnd:      dec     ax
		jns     UFDoFlush
		pop     cx
		shr     cx,2
		xor     edi,edi
		rep     movs es:dword ptr [edi],fs:[edi]
		mov     _GRThisBank,dx
		push    es
		push    fs
		pop     es
		pop     fs
		pop     edi
		pop     esi
		ret
_UltraFlush     endp

;void UltraFlushVBE2()
                public  _UltraFlushVBE2
_UltraFlushVBE2 proc
                push    bp
                mov     bp,sp
                push    esi
                push    edi
                push    es
                push    gs
                pop     es
                pop     gs
                cld
                xor     esi,esi
                mov     edi,_AddrLinearFB
                mov     ecx,_GRWidth
                imul    ecx,_GRHeight
                shr     ecx,2
                rep     movs es:dword ptr [edi],gs:[esi]
                push    es
                push    gs
                pop     es
                pop     gs
                pop     edi
                pop     esi
                pop     bp
                ret
_UltraFlushVBE2 endp


;void PMStoreBuffer(long Left, long Top, long With, long Height);
		public  _StoreBuffer
_StoreBuffer    proc
		push    bp
		mov     bp,sp
		push    esi
		push    edi


                mov     esi,ScreenBuffer
                mov     edi,BGndBuffer
		call    Buffer2Buffer

		pop     edi
		pop     esi
		pop     bp
		ret
_StoreBuffer    endp

;void PMRestoreBuffer(long Left, long Top, long Width, long Height);
		public  _RestoreBuffer
_RestoreBuffer  proc
		push    bp
		mov     bp,sp
		push    esi
		push    edi

                mov     esi,BGndBuffer
                mov     edi,ScreenBuffer
		call    Buffer2Buffer

		pop     edi
		pop     esi
		pop     bp
		ret
_RestoreBuffer  endp


Buffer2Buffer   proc
		push    es
		push    gs
		pop     es
		cld

                mov     eax,[bp +  8]
                imul    eax,_GRWidth
                add     eax,[bp + 4]
                add     esi,eax
                add     edi,eax

		mov     ecx,[bp + 12]
		mov     eax,_GRWidth
		sub     eax,ecx
		jmp     SBTestEnd

SBLoop:         push    ecx
		shr     ecx,1
		rep     movs es:word ptr [edi],gs:[esi]
		jnc     SBEven
		movs    es:byte ptr [edi],gs:[esi]
SBEven:         pop     ecx
		add     edi,eax
		add     esi,eax
SBTestEnd:      dec     word ptr [bp + 16]
		jns     SBLoop

		pop     es
		ret
Buffer2Buffer   endp
;-------------------------------------------------------------
;void PutPixel(long Left, long Top, long Color)
		public  _PutPixel
_PutPixel       proc
		push    bp
		mov     bp,sp

                mov     ebx,[bp + 8]
                imul    ebx,_GRWidth
                add     ebx,[bp + 4]
		mov     al,[bp + 12]
		mov     es:[ebx],al
		pop     bp
		ret
_PutPixel       endp

;void HLine(long Left, long Top, long Width, long Color)
		public  _HLine
_HLine          proc
		push    bp
		mov     bp,sp
		push    edi
                mov     edi,[bp + 8]
                imul    edi,_GRWidth
                add     edi,[bp + 4]

		mov     al,[bp + 16]
		mov     ah,al
		push    ax
		shl     eax,16
		pop     ax

		mov     ecx,[bp + 12]
		shr     ecx,2
		cld
		rep     stos es:dword ptr [edi]
                ;jnc     HLAllDone
		mov     ecx,[bp + 12]
		and     ecx,3
		rep     stos es:byte ptr [edi]
		
HLAllDone:      pop     edi
		pop     bp
		ret
_HLine          endp

;void VLine(long Left, long Top, long Height, long Color)
		public  _VLine
_VLine          proc
		push    bp
		mov     bp,sp

                mov     ebx,[bp + 8]
                imul    ebx,_GRWidth
                add     ebx,[bp + 4]
		mov     cx,[bp + 12]
		mov     al,[bp + 16]

VLDrawLine:     mov     es:[ebx],al
		add     ebx,_GRWidth
		loopnz  VLDrawLine

		pop     bp
		ret
_VLine          endp

;void Bar(long Left, long Top, long Width, long Height, long Color)
		public _Bar
_Bar            proc
		push    bp
		mov     bp,sp
		push    edi
		cld

                mov     edi,[bp + 8]
                imul    edi,_GRWidth
                add     edi,[bp + 4]

		mov     al,[bp + 20]
		mov     ah,al
		push    ax
		shl     eax,16
		pop     ax
		jmp     BarTestEnd

BarDraw:        mov     ecx,[bp + 12]
		shr     ecx,2
		push    edi
		rep     stos es:dword ptr [edi]
		mov     ecx,[bp + 12]
		and     ecx,3
                jz      BarLineDrawn
		rep     stos es:byte ptr [edi]

BarLineDrawn:   pop     edi
		add     edi,_GRWidth
		
BarTestEnd:     dec     word ptr [bp + 16]
		jns     BarDraw

		pop     edi
		pop     bp
		ret
_Bar            endp

;void PutImage(long Left,long Top,long Width,long Height,
;                    4          8        12         16
;              long IAdd,long PhysImage);
;                    20          24

		public  _PutImage
_PutImage       proc
		push    bp
		mov     bp,sp
		push    esi
		push    edi
		cld

                mov     edi,[bp + 8]
                imul    edi,_GRWidth
                add     edi,[bp + 4]
		mov     esi,[bp + 24]
		jmp     PITestEnd
		
PIDraw:         mov     ecx,[bp + 12]
		shr     ecx,2
		push    edi
		rep     movs es:dword ptr [edi],gs:[esi]
		;jnc     PILineDrawn
		mov     ecx,[bp + 12]
		and     ecx,3
		rep     movs es:byte ptr [edi],gs:[esi]

PILineDrawn:    pop     edi
		add     edi,_GRWidth
		add     esi,[bp + 20]
		
PITestEnd:      dec     word ptr [bp + 16]
		jns     PIDraw

		pop     edi
		pop     esi
		pop     bp
		ret
_PutImage       endp

;void GetImage(long Left,long Top,long Width,long Height,
;                    4          8        12         16
;              long IAdd,long PhysImage);
;                    20          24

		public  _GetImage
_GetImage       proc
		push    bp
		mov     bp,sp
		push    esi
		push    edi
		cld

		push    es
		push    gs
		pop     es
		pop     gs

                mov     esi,[bp + 8]
                imul    esi,_GRWidth
                add     esi,[bp + 4]
		mov     edi,[bp + 24]
		jmp     GITestEnd
		
GIDraw:         mov     ecx,[bp + 12]
		shr     ecx,2
		push    esi
		rep     movs es:dword ptr [edi],gs:[esi]
		;jnc     GILineDrawn
		mov     ecx,[bp + 12]
		and     ecx,3
		rep     movs es:byte ptr [edi],gs:[esi]

GILineDrawn:    pop     esi
		add     esi,_GRWidth
		add     edi,[bp + 20]
		
GITestEnd:      dec     word ptr [bp + 16]
		jns     GIDraw

		push    es
		push    gs
		pop     es
		pop     gs

		pop     edi
		pop     esi
		pop     bp
		ret
_GetImage       endp

;void SetClippingRegion(long Left, long Top, long Width, long Height);
		public  _SetClippingRegion
_SetClippingRegion proc
		push    bp
		mov     bp,sp

                mov     eax,[bp + 4]
                mov     ecx,[bp + 12]
                mov     edx,[bp + 16]
		mov     ebx,[bp + 8]

		or      eax,eax
		jnl     SCRLeftOK
		add     ecx,eax
		xor     eax,eax

SCRLeftOK:      or      ebx,ebx
		jnl     SCRTopOK
		add     edx,ebx
		xor     ebx,ebx

SCRTopOK:
                mov     _ClipLeft,eax
		mov     _ClipTop,ebx
		add     eax,ecx
		dec     eax
		add     ebx,edx
		dec     ebx

		cmp     eax,_GRWidth
		jl      SCRRightOK
		sub     ecx,eax
		mov     eax,_GRWidth
		dec     eax
		add     ecx,eax

SCRRightOK:     cmp     ebx,_GRHeight
		jl      SCRBottomOK
		sub     edx,ebx
		mov     ebx,_GRHeight
		dec     ebx
		add     edx,ebx

SCRBottomOK:
                mov     _ClipRight,eax
		mov     _ClipBottom,ebx
		mov     _ClipWidth,ecx
		mov     _ClipHeight,edx
		pop     bp
		ret
_SetClippingRegion endp

;void GetClippingRegion(long PhysLeft, long PhysTop, long PhysWidth, long PhysHeight);
		public  _GetClippingRegion
_GetClippingRegion proc
		push    bp
		mov     bp,sp
		mov     ebx,[bp + 4]
		mov     eax,_ClipLeft
		mov     gs:[ebx],eax
		mov     ebx,[bp + 8]
		mov     eax,_ClipTop
		mov     gs:[ebx],eax
		mov     ebx,[bp + 12]
		mov     eax,_ClipWidth
		mov     gs:[ebx],eax
		mov     ebx,[bp + 16]
		mov     eax,_ClipHeight
		mov     gs:[ebx],eax
		pop     bp
		ret
_GetClippingRegion endp

;void SetViewportOrigin(long Left, long Top);
		public  _SetViewportOrigin
_SetViewportOrigin proc
		push    bp
		mov     bp,sp
                mov     eax,[bp + 4]
		mov     ebx,[bp + 8]

		mov     ecx,_VPLeft
		sub     ecx,eax
		mov     edx,_VPTop
		sub     edx,ebx

;                add     _ClipLeft,ecx
;                add     _ClipRight,ecx
;                add     _ClipTop,edx
;                add     _ClipBottom,edx

		mov     _VPLeft,eax
		mov     _VPTop,ebx

		pop     bp
		ret
_SetViewportOrigin endp

;void GetViewportOrigin(long PhysLeft, long PhysTop)
		public  _GetViewportOrigin
_GetViewportOrigin proc
		push    bp
		mov     bp,sp
		mov     ebx,[bp + 4]
		mov     eax,_VPLeft
		mov     gs:[ebx],eax
		mov     ebx,[bp + 8]
		mov     eax,_VPTop
		mov     gs:[ebx],eax
		pop     bp
		ret
_GetViewportOrigin endp

;void MapFontData()
                public  _MapFontData
_MapFontData    proc
                ret
_MapFontData    endp

;long PMTextWidth(long PhysStr)
                public  _PMTextWidth
_PMTextWidth    proc
                push    bp
                mov     bp,sp
                push    esi

                mov     esi,[bp + 4]
                cld
                jmp     PMTWGetChar

PMTWIncLen:     movzx   eax,al
                shl     ax,2
                add     eax,_PhysFontHeader
                movzx   ax,gs:byte ptr [eax + 3]
                add     cx,ax


PMTWGetChar:    lods    gs:byte ptr [esi]
                or      al,al
                jnz     PMTWIncLen

                mov     ax,cx
                xor     dx,dx
                pop     esi
                pop     bp
                ret
_PMTextWidth    endp


;void CreateText(long Width, long PhysStr, long Color);
                public  _CreateText
_CreateText     proc
                push    bp
                mov     bp,sp
                push    esi
                push    edi
                push    es

                cld
                mov     ax,gs
                mov     es,ax

                mov     edi,GrTextBuffer
                mov     esi,[bp + 8]
                jmp     CTGetChar

CTDrawChar:     shl     ax,2
                add     eax,_PhysFontHeader
                push    eax
                movzx   ebx,gs:word ptr [eax]
                movzx   edx,gs:byte ptr [eax + 2]
                shl     bx,1
                shl     dx,1
                add     edx,ebx

                jmp     CTTestCharEnd

CTDrawLine:     push    edi
                mov     eax,_PhysFontData
                movzx   ecx,gs:word ptr [ebx + eax]

                mov     eax,ecx
                shr     ax,10
                add     edi,eax

                mov     ax,cx
                shr     ax,5
                and     ax,001fh
                imul    eax,[bp + 4]
                add     edi,eax

                and     cx,001fh
                mov     ax,[bp + 12]
                rep     stos es:byte ptr [edi]

                pop     edi
                add     ebx,2

CTTestCharEnd:  cmp     ebx,edx
                jl      CTDrawLine

                pop     eax
                movzx   eax,gs:byte ptr [eax + 3]
                add     edi,eax

CTGetChar:      xor     eax,eax
                lods    gs:byte ptr [esi]
                or      al,al
                jnz     CTDrawChar

                pop     es
                pop     edi
                pop     esi
                pop     bp
                ret
_CreateText     endp

FARCODE         ends
		end
