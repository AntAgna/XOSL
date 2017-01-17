; EnableA20/DisableA20 code is take from Ranish Partition Manager 2.38 Beta 1.9
		.code
		
;		public	_EnableA20
;		public	_DisableA20
		

_EnableA20	proc	near

		call	A20wait_write
		jc	@@Eerror

		cli			; Only need for reading value from KBC
		mov	al, 0D0h
		out	64h, al
		call	A20wait_read
		jc	@@Eenable_int

		in	al, 60h		; Get current status
		mov	ah, al
		call	A20wait_write
@@Eenable_int:	sti
		jc	@@Eerror

		mov	al, 0D1h	; Tell the KBC we want to write to the
		out	64h, al		; Out Put Port next
		call	A20wait_write
		jc	@@Eerror

		mov	al, ah
		or	al, 2			; Enable line A20
		out	60h, al
		call	A20wait_write
		jc	@@Eerror

		mov	al, 0FFh		; NOP
		out	64h, al
		call	A20wait_write
@@Eerror:
		ret
		endp

_DisableA20	proc	near

		call	A20wait_write
		jc	@@Derror

		cli			; Only need for reading value from KBC
		mov	al, 0D0h
		out	64h, al
		call	A20wait_read
		jc	@@Denable_int

		in	al, 60h
		mov	ah, al
		call	A20wait_write
@@Denable_int:	sti
		jc	@@Derror

		mov	al, 0D1h
		out	64h, al
		call	A20wait_write
		jc	@@Derror

		mov	al, ah
		and	al, 0FDh		; Disable line A20
		out	60h, al
		call	A20wait_write
		jc	@@Derror

		mov	al, 0FFh
		out	64h, al
		call	A20wait_write
@@Derror:
		ret
		endp
		


A20wait_write	proc	near

	; Waits until the Input register is empty or until a short amount
	; of time has elapsed. It waits till bit 1 of the KBC Read Status

		xor	cx, cx		; Reasonable wait
@@Wagain:
 		in	al, 64h		; Get KBC read status
		test	al, 02		; See if buffer empty, bit 1 clear
		jz	@@Wclear
		inc	cx
		jnc	@@Wagain
		ret
@@Wclear:
 		clc
 		ret

A20wait_write	endp


A20wait_read	proc	near

	; Waits until the Read Status Port, 64h, contains data by checking the state
	; of bit 0. It waits till it is set or too much time has gone by.
		xor	cx, cx
@@Ragain:
 		in	al, 64h
		test	al, 01		; If the 'output buffer' is full, has
		jnz	@@Rclear		; something for me
		inc	cx
		jnc	@@Ragain
		ret
@@Rclear:
		clc
		ret

A20wait_read	endp

		end
