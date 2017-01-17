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
                public  DriveNumber, DiskSectors
                public  DiskHeads, ABSSectorStart
                public  ClusterSectSize, ClusterByteSize
                public  FATStart, DataStart
                public  RootCluster

                db      1 dup (0)

LoaderName      db      'XOSLLOADXCF'


DiskSectors     dd      63
DiskHeads       dd      128

ClusterSectSize dw      0008h
ClusterByteSize dw      1000h
FATStart        dd      00000020h
DataStart       dd      000007f0h
RootCluster     dd      00000002h

FSType          db      0bh
DriveNumber     db      80h
ABSSectorStart  dd      00415680h

Password	dd	0

                end
