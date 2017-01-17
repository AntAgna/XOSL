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

                .code

		public	@CKeyboard@WaitKeyStrokeDef$qv
		public	@CKeyboard@WaitKeyStrokeEnh$qv
		public	@CKeyboard@StoreKeyStroke$qus
		public	@CKeyboard@KeyStrokeAvailDef$qv
		public	@CKeyboard@KeyStrokeAvailEnh$qv




;unsigned short CKeyboard::WaitKeyStrokeDef()
@CKeyboard@WaitKeyStrokeDef$qv proc
                xor     ah,ah
                int     16h
                ret
                endp

@CKeyboard@WaitKeyStrokeEnh$qv proc
		mov	ah,10h
		int	16h
		ret
		endp


;void CKeyboard::StoreKeyStroke(unsigned short KeyCode)
@CKeyboard@StoreKeyStroke$qus proc c
                arg     @@KeyCode: word

                mov     ah,5
                mov     cx,@@KeyCode
                int     16h
                ret
                endp


;bool CKeyboard::KeyStrokeAvailDef();
@CKeyboard@KeyStrokeAvailDef$qv proc
                mov     ah,1
                int     16h
                mov     ax,1
                jnz     KHDEnd
                dec     ax
KHDEnd:         ret
                endp


;bool CKeyboard::KeyStrokeAvailEnh();
@CKeyboard@KeyStrokeAvailEnh$qv proc
                mov     ah,11h
                int     16h
                mov     ax,1
                jnz     KHEEnd
                dec     ax
KHEEnd:         ret
                endp



                end
