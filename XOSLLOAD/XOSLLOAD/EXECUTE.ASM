                .model  compact
                .386p
                .code
                public  _Execute

;void Execute(unsigned short StartSeg, unsigned short ReloSS,
;             unsigned short ExeSP, unsigned short ReloCS,
;             unsigned short ExeIP);

_Execute        proc    c
                arg     @@StartSeg: word, @@ReloSS: word
                arg     @@ExeSP: word, @@ReloCS: word
                arg     @@ExeIP: word

                mov     ax,@@StartSeg
                sub     ax,0010h
                mov     ds,ax
                mov     es,ax

                mov     ax,@@ReloSS
                add     ax,@@StartSeg
                mov     bx,@@ExeSP
                mov     cx,@@ReloCS
                add     cx,@@StartSeg
                mov     dx,@@ExeIP
                mov     ss,ax
                mov     sp,bx
                push    cx
                push    dx
                retf
_Execute        endp
                
                end
