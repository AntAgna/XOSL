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
; * File  : FAT32.ASM
; * Author: Geurt Vos
; * Date  : March 1999
; *
; * Reads a file from a 'mounted' FAT32 partition
; */

                .model  tiny
                .386p

TFAT32DirEntry  struc
        FileName        db      8 dup (?)
        Extension       db      3 dup (?)
        Attribute       db      ?
        NT              dw      ?
        CreateTime      dw      ?
        CreateDate      dw      ?
        Accessed        dw      ?
        StartClusterH   dw      ?
        Time            dw      ?
        Date            dw      ?
        StartClusterL   dw      ?
        FileSize        dd      ?
TFAT32DirEntry  ends

INMEMCLUST      equ     4096
INMEMCLUSTPWR   equ     12              ;4096 == 2**12
INMEMCLUSTSCALE equ     not (INMEMCLUST - 1)

FATSECTCOUNT    equ     INMEMCLUST / 128
FATSECTCNTPWR   equ     INMEMCLUSTPWR - 7       ;128 clust/sect

                .data
                extrn   ClusterSectSize: word
                extrn   ClusterByteSize: word
                extrn   FATStart: dword
                extrn   DataStart: dword
                extrn   RootCluster: dword

                .data?
FAT             dw      INMEMCLUST dup (?)
Root            db      16384 dup (?)

                .code
                public  ReadFile
                extrn   ReadSectors: near
                extrn   MemCompare: near

;int ReadFile(const char *FileName, void *Buffer)
ReadFile        proc
                push    bp
                mov     bp,sp
                push    esi
                push    edi

                call    Locate
                or      ax,ax
                jne     ReadFileExit
                mov     edi,[bp + 8]

ReadLoop:       call    ReadCluster
                mov     ax,ClusterByteSize
                add     di,ax
                call    GetNextCluster
                cmp     esi,0fffffffh           ;FAT32 has 28bit clusters
                jne     ReadLoop
                xor     ax,ax

ReadFileExit:   pop     edi
                pop     esi
                pop     bp
                ret
ReadFile        endp

;Locate
;       Locate the first cluster of a file
; Entry:
;       dword ptr [bp + 4] -> FileName
; Return:
;       esi = StartCluster
;       ax = success ? 0 : -1
Locate          proc

                mov     esi,RootCluster
                mov     di,ds
                shl     edi,16
                mov     di,offset Root

LReadLoop:      call    ReadCluster
                xor     cx,cx

LFindEntryLoop: mov     bx,cx
                add     bx,offset Root
                cmp     [bx].FileName[0],0
                je      NotFoundExit

                push    cx
                push    word ptr 11
                push    dword ptr [bp + 4]
                push    ds
                push    bx
                call    MemCompare
                add     sp,10
                pop     cx
                or      ax,ax
                jnz     LNextEntry

                mov     si,[bx].StartClusterH
                shl     esi,16
                mov     si,[bx].StartClusterL
                ret

LNextEntry:     add     cx,32
                cmp     cx,ClusterByteSize
                jb      LFindEntryLoop


                call    GetNextCluster
                cmp     esi,0fffffffh           ;FAT32 has 28bit clusters
                jne     LReadLoop
                                
NotFoundExit:   mov     ax,-1
                ret
Locate          endp

;GetNextCluster
; Entry:
;       esi = cluster
;
GetNextCluster  proc
                call    ReadFAT
                mov     ebx,esi
                sub     ebx,eax
                shl     ebx,2
                mov     esi,[ebx + offset FAT]
                ret
GetNextCluster  endp

;ReadFAT
; Entry:
;       esi = Cluster
ReadFAT         proc
                mov     eax,esi
                shr     eax,INMEMCLUSTPWR
                shl     eax,FATSECTCNTPWR
                add     eax,FATStart
                push    FATSECTCOUNT
                push    ds
                push    offset FAT
                push    eax
                call    ReadSectors
                add     sp,10

                mov     eax,esi
                and     eax,INMEMCLUSTSCALE
                ret
ReadFAT         endp

;ReadCluster:
; On entry:
;       edi -> Buffer
;       esi = Cluster
ReadCluster     proc
                mov     eax,esi
                movzx   ecx,ClusterSectSize
                sub     eax,2
                imul    eax,ecx
                add     eax,DataStart
                push    ClusterSectSize
                push    edi
                push    eax
                call    ReadSectors
                add     sp,10
                ret
ReadCluster     endp

                end
