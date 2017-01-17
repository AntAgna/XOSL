;
; CDriveFix::SwapDrive()
;
; This code is based on the SWAPR routine of the GAG boot manager
; Created by Sergio Costas Rodriguez
; http://www.arrakis.es/~scostas/SOFTWARE/GAG/gageng.htm
;
        
		.model  large
                .386p
                .code
		
BaseMemorySize	equ	word ptr 0413h
AddrInt13ISR	equ	dword ptr [13h * 4]

                public  @CDriveFix@SwapDrive$qi


;static void CDriveFix::SwapDrive(int Drive);
@CDriveFix@SwapDrive$qi proc c
		arg	@@Drive: word

		push	ds
	
		mov	al,byte ptr @@Drive
		mov	cs:SwapDrive,al
	
		;Allocate 1kb of memory to store SwapISR
		xor	ax,ax
		mov	es,ax
		dec	es:BaseMemorySize	;decrease amount of memory with 1K
		mov	ax,es:BaseMemorySize	;get amount of memory
		shl	ax,6			;segment for the allocated memory

		;Replace the existing int 13h ISR
		mov	ecx,es:AddrInt13ISR
		mov	cs:Int13Addr,ecx
		
		mov	es:word ptr AddrInt13ISR,SwapISREntry
		mov	es:word ptr AddrInt13ISR[2],ax

		;Copy ISR to allocated memory
		mov	es,ax			;ES:DI -> destination
		xor	di,di
	
		push	cs			;DS:SI -> source
		pop	ds			
		mov	si,offset SwapDrive
	
		mov	cx,128			;copy 256 byte
		cld
		rep	movsw
	
		pop	ds
		ret
		endp
	

; *************************************************
; *                     SWAPR                     *
; * This is the swap routine, that allows to swap *
; * hard disk drives                              *
; *************************************************

SwapISREntry	equ	offset ISREntry - offset SwapDrive
SwapISRDataB	equ	byte ptr [offset ISRData - offset SwapDrive]
SwapISRDataW0	equ	word ptr [offset ISRData - offset SwapDrive + 0]
SwapISRDataW1	equ	word ptr [offset ISRData - offset SwapDrive + 2]
SwapISRDataW2	equ	word ptr [offset ISRData - offset SwapDrive + 4]

ISRSwapDrive	equ	byte ptr 0
ISRDriveReg	equ	byte ptr [offset DriveReg - offset SwapDrive]

;xxxx:0000 (most likely is 9fc0:0000)
SwapDrive	db	0			;drive to swap
DriveReg	db	0			;here the DL register is stored
ISRData		db	6 dup (0)		;area for temporal data
		

ISREntry:
		mov	cs:SwapISRDataB,ah	;saves the subcall made to int 13h
		mov	cs:ISRDriveReg,dl	;store drive number
		
		cmp	dl,80h			;first drive?
		jne	SwapLabel1
		mov	dl,cs:ISRSwapDrive	;yes -> use swapped drive instead
		jmp	SwapLabel2
		
SwapLabel1:	cmp	dl,cs:ISRSwapDrive	;swapped drive?
		jne	SwapLabel2
		mov	dl,80h			;yes -> use first drive instead
		
SwapLabel2:		
		;call original int 13h handler to process the request
		pushf				;iret == ret + popf
		db	9ah			;far call
Int13Addr	dd	0

		pushf
		cmp	cs:SwapISRDataB,08h	;subcall 'GET DRIVE PARAMETERS'?
		je	SwapLabel3		;yes -> don't restore drive number
		cmp	cs:SwapISRDataB,15h	;subcall 'GET DISK TYPE'?
		je	SwapLabel3		;yes -> don't restore drive number
		
		mov	dl,cs:ISRDriveReg	;Restore drive number
		
SwapLabel3:	
		popf
		pop	cs:SwapISRDataW0	;IP
		pop	cs:SwapISRDataW1	;CS
		pop	cs:SwapISRDataW2	;old flags
		pushf				;store the new flags on the stack
		push	cs:SwapISRDataW1	;store CS
		push	cs:SwapISRDataW0	;store IP
		iret
	   
		end
