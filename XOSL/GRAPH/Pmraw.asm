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
TDesc           struc
		LimitLow        dw ?
		BaseLow         dw ?
		BaseMid         db ?
		Access          db ?
		LimitHigh       db ?
		BaseHigh        db ?
TDesc           ends

TIGate          struc
	Offset_0_15     dw      0
	Selector        dw      0
	iAccess         dw      0
	Offset_16_31    dw      0
TIGate          ends


CodeSelect      equ     08h
DataSelect      equ     10h
StackSelect     equ     18h
GraphSelect     equ     20h
BufferSelect    equ     28h
PhysicalSelect  equ     30h

BeginGDT        equ     this byte
DummyDesc       TDesc   <>
CodeDesc        TDesc   <0ffffh,0,0,9ah,0,0>
DataDesc        TDesc   <0ffffh,0,0,92h,0,0>
StackDesc       TDesc   <0ffffh,0,0,92h,0,0>
GraphDesc       TDesc   <0ffffh,0,0ah,92h,0,0>
BufferDesc      TDesc   <01d5h,0,12h,92h,0c0h,0>
PhysicalDesc    TDesc   <0ffffh,0,0,92h,0cfh,0>
GDTR            equ     this fword
GDTLimit        dw      EndGDT - BeginGDT
GDTBase         dd      ?

;IDTR            equ     this fword
;IDTLimit        dw      EndIDT - BeginIDT
;IDTBase         dd      ?

;RM_IDTR         df      0

EndGDT          equ     this byte

                assume  cs:FARCODE

FARCODE         segment word public use16 'CODE'

;void InitDesc()
		public  _InitDesc
_InitDesc       proc    far
		mov     eax,DGROUP
		shl     eax,4
		or      dword ptr DataDesc.BaseLow,eax
                add     eax,offset BeginGDT
		mov     GDTBase,eax

		xor     eax,eax
		mov     ax,cs
		shl     eax,4
		or      dword ptr CodeDesc.BaseLow,eax

		xor     eax,eax
		mov     ax,ss
		shl     eax,4
		or      dword ptr StackDesc.BaseLow,eax

                retf
_InitDesc       endp



;void SwitchPM()
		public  SwitchPM
SwitchPM        proc
		cli


		mov     cs:RealModeSS,ss
		mov     cs:RealModeDS,ds
		mov     cs:RealModeCS,cs
		lgdt    GDTR

                mov     eax,cr0
		or      al,1
		mov     cr0,eax


		db      0eah
		dw      $+4
		dw      CodeSelect


		mov     ax,DataSelect
		mov     ds,ax
		mov     ax,StackSelect
		mov     ss,ax
		mov     ax,BufferSelect
		mov     es,ax
		mov     ax,GraphSelect
		mov     fs,ax
		mov     ax,PhysicalSelect
		mov     gs,ax

		ret
SwitchPM        endp

RealModeSS      dw      ?
RealModeDS      dw      ?

;void SwitchRM()
		public  SwitchRM
SwitchRM        proc

		mov     ax,ds
		mov     fs,ax
		mov     gs,ax

		mov     eax,cr0
                and     al,0feh
		mov     cr0,eax

		db      0eah
		dw      $+4
RealModeCS      dw      ?

		mov     ds,cs:RealModeDS
		mov     ss,cs:RealModeSS


		sti

		ret
SwitchRM        endp

FARCODE         ends

		end
		
