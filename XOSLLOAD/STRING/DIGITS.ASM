                .model  compact
                .386p
                .data
Digits          db      '0123456789abcdef'
                .code
                public  _GetDigit
_GetDigit       proc    c
                arg     @@Digit: word

                mov     bx,offset Digits
                add     bx,@@Digit
                mov     al,[bx]

                ret
                endp

                end
