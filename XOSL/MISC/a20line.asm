;
; Extended Operating System Loader (XOSL)
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

; A20 Implementation borrowed (slighly modified) from
;       http://eliteinformatiker.de/2015/10/17/die-a20-line-aktivieren
; which itself has been done along the lines of
;       http://wiki.osdev.org/A20_Line

; All credit and criticism goes to these two sites... ;-)

                .model  compact
                .386p
                .code
                                
                public  _EnableA20
                public  _DisableA20

                
; Returns AX ==1 if A20 is active, and AX == 0 if A20 is inactive
; see http://wiki.osdev.org/A20_Line#Testing_the_A20_line

; Precondition: Interrupts are already disabled (cli) by the enclosing _EnableA20/_DisableA20

a20_active:
                pushf
                push ds
                push es
                push di
                push si

                xor ax, ax                  ; ax = 0
                mov es, ax

                not ax                      ; ax = 0xffff
                mov ds, ax
                
                mov di, 0500h
                mov si, 0510h

                mov al, byte [es:di]        ; save the old values from memory
                push ax
                
                mov al, byte [ds:si]
                push ax

                mov byte ptr [es:di], 00h   ; write 0x00 to one and 0xff to the other location
                mov byte ptr [ds:si], 0ffh
                
                cmp byte ptr [es:di], 0ffh  ; check if the address we set to 0x00 was
                                            ; set to 0xff later, then we have only 20 bit
                                            ; addresses
                
                pop ax                      ; restore the bytes we set before
                mov byte ptr [ds:si], al
                
                pop ax
                mov byte ptr [es:di], al
                
                mov ax, 0
                je a20_active_end
                mov ax, 1

a20_active_end:
                pop si
                pop di
                pop es
                pop ds
                popf
                ret

a20_wait_inbuf:
                in al, 64h
                test al, 2
                jnz a20_wait_inbuf
                ret

a20_wait_outbuf:
                in al, 64h
                test al, 1
                jz a20_wait_outbuf
                ret


; A20 Enable                
    
_EnableA20      proc far

a20_enable:
                cli
                call a20_stop_if_active
                jnz a20e_done

a20e_bios:
                mov ax, 2401h
                int 15h

                call a20_stop_if_active
                jnz a20e_done

a20e_keyboard:
                call a20_wait_inbuf         ; disable the keyboard
                mov al, 0adh
                out 64h, al
                
                call a20_wait_inbuf         ; tell the controller we want to read data
                mov al, 0d0h
                out 64h, al
                
                call a20_wait_outbuf        ; read the P2 port provided by the controller
                in al, 60h
                push ax

                call a20_wait_inbuf         ; tell the controller we want to write data
                mov al, 0d1h
                out 64h, al
                
                call a20_wait_inbuf         ; write the new P2 port with A20 line active
                pop ax
                or al, 2
                out 60h, al

                call a20_wait_inbuf         ; re-enable the keyboard
                mov al, 0aeh
                out 64h, al

                call a20_wait_inbuf

                call a20_stop_if_active_loop
                jnz a20e_done

a20e_fast:
                in al, 92h
                test al, 2
                jnz a20e_fast_done          ; A20 Fast Gate is already activated
                or al, 2
                and al, 0feh
                out 92h, al
    
a20e_fast_done: call a20_stop_if_active_loop
                jnz a20e_done

                hlt                         ; what should we do here?

a20e_done:
                sti
                ret

_EnableA20      endp


; non-zero, i.e. ZF == 0 means A20 is active
                
a20_stop_if_active:
                call a20_active
                test ax, ax                 ; check if all bits are 0, then ZF = 1
                ret

a20_stop_if_active_loop:                    ; try in a loop if a20 is active for k times
                mov bx, 0ffh
a20_stop_if_active_loop_iterator:
                dec bx
                call a20_stop_if_active
                test ax, ax
                jnz a20_stop_if_active_loop_exit

                test bx, bx                 ; check if bx 0
                jnz a20_stop_if_active_loop_iterator

a20_stop_if_active_loop_exit:                
                ret





_DisableA20     proc far

a20_disable:
                cli
                call a20_stop_if_inactive
                jz a20d_done

a20d_bios:
                mov ax, 2400h
                int 15h

                call a20_stop_if_inactive
                jz a20d_done

a20d_keyboard:
                call a20_wait_inbuf         ; disable the keyboard
                mov al, 0adh
                out 64h, al
                
                call a20_wait_inbuf         ; tell the controller we want to read data
                mov al, 0d0h
                out 64h, al
                
                call a20_wait_outbuf        ; read the P2 port provided by the controller
                in al, 60h
                push ax

                call a20_wait_inbuf         ; tell the controller we want to write data
                mov al, 0d1h
                out 64h, al
                
                call a20_wait_inbuf         ; write the new P2 port with A20 line inactive
                pop ax
                and al, 0fdh
                out 60h, al

                call a20_wait_inbuf         ; re-enable the keyboard
                mov al, 0aeh
                out 64h, al

                call a20_wait_inbuf

                call a20_stop_if_inactive_loop
                jz a20d_done

a20d_fast:
                in al, 92h
                and al, 2
                test al, 2
                jz a20d_fast_done           ; A20 Fast Gate is already deactivated
                ; and al, 0fdh
                ; and al, 0feh
                and al, 0fch
                out 92h, al
    
a20d_fast_done: call a20_stop_if_inactive_loop
                jz a20d_done

                hlt                         ; what should we do here?

a20d_done:
                sti
                ret

_DisableA20     endp


; zero, i.e. ZF == 1 means A20 is inactive
                
a20_stop_if_inactive:
                call a20_active
                test ax, ax                 ; check if all bits are 0, then ZF = 1
                ret

a20_stop_if_inactive_loop:                  ; try in a loop if a20 is active for k times
                mov bx, 0ffh
a20_stop_if_inactive_loop_iterator:
                dec bx
                call a20_stop_if_inactive
                test ax, ax
                jz a20_stop_if_inactive_loop_exit

                test bx, bx                 ; check if bx 0
                jz a20_stop_if_inactive_loop_iterator

a20_stop_if_inactive_loop_exit:
                ret


                end
