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
; * File  : FAT16.ASM
; * Author: Geurt Vos
; * Date  : March 1999
; *
; * Reads a file from a 'mounted' FAT16 partition
; */

                .model  tiny
                .386p

TFAT16DirEntry  struc
        FileName        db      8 dup (?)
        Extension       db      3 dup (?)
        Attribute       db      ?
        Reserved        db      10 dup (?)
        Time            dw      ?
        Date            dw      ?
        StartCluster    dw      ?
        FileSize        dd      ?
TFAT16DirEntry  ends

INMEMCLUST      equ     4096
INMEMCLUSTPWR   equ     12              ;4096 == 2**12
INMEMCLUSTSCALE equ     not (INMEMCLUST - 1)

FATSECTCOUNT    equ     INMEMCLUST / 256
FATSECTCNTPWR   equ     INMEMCLUSTPWR - 8       ;256 clust/sect

INMEMENTRIES    equ     32
INMEMENTRIESPWR equ     5

DIRSECTORCOUNT  equ     (INMEMENTRIES / 16)
DIRSECTORPWR    equ     INMEMENTRIESPWR - 4

FILE_DELETED    equ     0e5h

                .data?
Root            db      32 * INMEMENTRIES dup (?)
FAT             dw      INMEMCLUST dup (?)

                .data
                extrn   RootEntries: word
                extrn   ClusterSectSize: word
                extrn   ClusterByteSize: word
                extrn   FATStart: dword
                extrn   DirStart: dword
                extrn   DataStart: dword

                .code
                public  ReadFile
                extrn   ReadSectors: near
                extrn   MemCompare: near

;int ReadFile(const char *FileName, void *Buffer)
ReadFile        proc
                push    bp
                mov     bp,sp
                push    si

                call    Locate
                or      ax,ax
                jne     ReadFileExit

ReadLoop:       call    ReadCluster
                mov     ax,ClusterByteSize
                add     [bp + 8],ax
                call    GetNextCluster
                cmp     si,0ffffh
                jne     ReadLoop
                xor     ax,ax

ReadFileExit:   pop     si
                pop     bp
                ret
ReadFile        endp

;Locate
;       Locate the first cluster of a file
; Entry:
;       dword ptr [bp + 4] -> FileName
; Return:
;       si = StartCluster
;       ax = success ? 0 : -1
Locate          proc
                push    di

                mov     di,INMEMENTRIES
                xor     si,si
                jmp     LTestLoopEnd

LocateLoop:     cmp     di,INMEMENTRIES
                jne     LFindFile
                xor     di,di
                call    ReadDirectory

LFindFile:      mov     bx,di
                shl     bx,5
                add     bx,offset Root
                cmp     [bx].FileName[0],0
                je      NotFoundExit

                ;Should check here if file deleted

                push    word ptr 11
                push    dword ptr [bp + 4]
                push    ds
                push    bx
                call    MemCompare
                add     sp,10
                or      ax,ax
                jnz     LNextEntry
                mov     si,[bx].StartCluster
                pop     di
                ret

LNextEntry:     inc     si
                inc     di
LTestLoopEnd:   cmp     si,RootEntries
                jb      LocateLoop

NotFoundExit:   mov     ax,-1
                pop     di
                ret
Locate          endp

;
; Entry:
;       si = Index
ReadDirectory   proc
                movzx   eax,si
                shr     ax,INMEMENTRIESPWR
                shl     ax,DIRSECTORPWR
                add     eax,DirStart
                push    word ptr DIRSECTORCOUNT
                push    ds
                push    offset Root
                push    eax
                call    ReadSectors
                add     sp,10
                ret
ReadDirectory   endp

;GetNextCluster
; Entry:
;       si = cluster
;
GetNextCluster  proc
                call    ReadFAT
                mov     bx,si
                sub     bx,ax
                shl     bx,1
                mov     si,[bx + offset FAT]
                ret
GetNextCluster  endp

;ReadFAT
; Entry:
;       si = Cluster
ReadFAT         proc
                movzx   eax,si
                shr     ax,INMEMCLUSTPWR
                shl     ax,FATSECTCNTPWR
                add     eax,FATStart
                push    FATSECTCOUNT
                push    ds
                push    offset FAT
                push    eax
                call    ReadSectors
                add     sp,10

                mov     ax,si
                and     ax,INMEMCLUSTSCALE
                ret
ReadFAT         endp

;ReadCluster:
; On entry:
;       dword ptr [bp + 8] -> Buffer
;       si = Cluster
ReadCluster     proc
                movzx   eax,si
                movzx   ecx,ClusterSectSize
                sub     ax,2
                imul    eax,ecx
                add     eax,DataStart
                push    ClusterSectSize
                push    dword ptr [bp + 8]
                push    eax
                call    ReadSectors
                add     sp,10
                ret
ReadCluster     endp

                end
