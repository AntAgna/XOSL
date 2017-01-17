;
; Extended Operating System Loader (XOSL)
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found in the GPL.TXT file,
; or at http://www.gnu.org
;

;/*
; * File  : INITDATA.ASM
; * Author: Geurt Vos
; * Date  : March 1999
; *
; * All pre-initialized IPL data.
; */

                .model  tiny
                .386p
                .code

                public  LoaderName
                public  DriveNumber, ABSSectorStart
                public  RootEntries, ClusterSectSize
                public  ClusterByteSize, FATStart
                public  DirStart, DataStart

                public  LBAPacket, SectorCount
                public  TransferBuffer, Sector

                db      13 dup (0)
LoaderName      db      'XOSLLOADXCF'

LBAPacket       equ     this byte
                dw      0010h
SectorCount     dw      0
TransferBuffer  dd      0
Sector          dq      0

RootEntries     dw      0200h
ClusterSectSize dw      0010h
ClusterByteSize dw      2000h
FATStart        dd      00000001h
DirStart        dd      000001f5h
DataStart       dd      00000215h

FSType          db      06h
DriveNumber     db      80h
ABSSectorStart  dd      0031b600h

Password	dd	0

                end
