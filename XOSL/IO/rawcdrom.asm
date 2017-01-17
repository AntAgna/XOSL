                .model  large
		.386p
                .code

public		@CCdRom@CallBootExtension$quliusus
public		@CCdRom@GetExtDriveParams$qim15CExtDriveParams


;void CallBootExtension(unsigned long Address, int Func, unsigned short BX, unsigned short CX);
@CCdRom@CallBootExtension$quliusus proc c
		arg	@@this: dword
		arg	@@Address: dword, @@Func: word
		arg	@@BX: word, @@CX: word

		mov	ah,byte ptr @@Func
		mov	bx,@@BX
		mov	cx,@@CX
		call	@@Address
		ret
		endp
		

;int CCdRom::GetExtDriveParams(int Drive, CExtDriveParams &Params)
@CCdRom@GetExtDriveParams$qim15CExtDriveParams proc c
		arg	@@this: dword
		arg	@@Drive: word
		arg	@@ParamBuffer: dword
		
		push	ds
		push	si
		
		mov	ah,48h
		mov	dl,byte ptr @@Drive
		lds	si,@@ParamBuffer
		int	13h
		mov	ax,-1
		jc	GetExtExit
		inc	ax
		
GetExtExit:	pop	si
		pop	ds
		ret
		endp
		
		end

