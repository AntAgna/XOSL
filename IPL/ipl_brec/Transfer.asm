                .model  compact
		.386p
                .data?

                public  _Drive
                public  _StartSector

DrvHeadCount    dd      ?
DrvSectorCount  dd      ?
_Drive          dw      ?
_StartSector    dd      ?


;'local' vars for Sector2CHS
;(don't know how to control 'enter' with 'proc c')
;Cylinder        dw      ?
Sector          dw      ?
;Head            dw      ?


                .code

                public  @ConvRead$qususnvi
                public  @GetDriveInfo$qi
                public  @Sector2CHS$qlmust2

;void ConvRead(unsigned short SectCyl,unsigned short DrvHead,
;             void *Buffer, int Count);
@ConvRead$qususnvi      proc c
                arg     @@SectCyl: word, @@DrvHead: word
                arg     @@Buffer: dword, @@Count: word

                mov     ah,02h
                mov     al,byte ptr @@Count
                les     bx,@@Buffer
                mov     cx,@@SectCyl
                mov     dx,@@DrvHead

		int     13h
                ret
                endp

;void GetDriveInfo(int Drive);
@GetDriveInfo$qi        proc c
                arg     @@Drive: word

		push    di

		mov     ah,8
                mov     dl,byte ptr @@Drive
		int     13h

                and     cl,3fh
                movzx   ecx,cl
                mov     DrvSectorCount,ecx

                inc     dh
                movzx   edx,dh
                mov     DrvHeadCount,edx

		pop     di
                ret
                endp

;void Sector2CHS(long RSector, unsigned short &SectCyl, unsigned short &DrvHead)
@Sector2CHS$qlmust2     proc c
                arg     @@RSector: dword
                arg     @@SectCyl: dword, @@DrvHead: dword

                ;RSector += StartSector
                mov     eax,@@RSector
                add     eax,_StartSector

                ;Sector = RSector % DrvSectorCount + 1
                ;RSector /= DrvSectorCount
                xor     edx,edx
                div     DrvSectorCount
                inc     dx
                mov     Sector,dx

                ;Head = RSector % DrvHeadCount
                ;Cylinder = RSector / DrvHeadCount
                xor     dx,dx
                div     dword ptr DrvHeadCount

                ;DrvHead = Drive | (Head << 8)
                mov     dh,dl
                mov     dl,byte ptr _Drive
                les     bx,@@DrvHead
                mov     es:[bx],dx

                ;((Cylinder & 0xff) << 8)
                mov     dh,al

                ; ((Cylinder >> 8) << 6)
                mov     dl,ah
                shl     dl,6

                ; SectCyl = Sector | ((Cylinder & 0xff) << 8) | ....
                or      dl,byte ptr Sector
                les     bx,@@SectCyl
                mov     es:[bx],dx


                ret
                endp

		end
