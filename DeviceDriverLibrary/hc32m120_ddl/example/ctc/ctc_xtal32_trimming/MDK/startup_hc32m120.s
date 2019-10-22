;/******************************************************************************
;* Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
;*
;* This software is owned and published by:
;* Huada Semiconductor Co.,Ltd ("HDSC").
;*
;* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
;* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
;*
;* This software contains source code for use with HDSC
;* components. This software is licensed by HDSC to be adapted only
;* for use in systems utilizing HDSC components. HDSC shall not be
;* responsible for misuse or illegal use of this software for devices not
;* supported herein. HDSC is providing this software "AS IS" and will
;* not be responsible for issues arising from incorrect user implementation
;* of the software.
;*
;* Disclaimer:
;* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
;* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
;* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
;* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
;* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
;* WARRANTY OF NONINFRINGEMENT.
;* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
;* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
;* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
;* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
;* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
;* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
;* SAVINGS OR PROFITS,
;* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
;* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
;* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
;* FROM, THE SOFTWARE.
;*
;* This software may be replicated in part or whole for the licensed use,
;* with the restriction that this Disclaimer and Copyright notice must be
;* included with each copy of this software, whether used in part or whole,
;* at all times.
;*/
;/*****************************************************************************/

;/*****************************************************************************/
;/*  Startup for ARM                                                          */
;/*  Version     V1.0                                                         */
;/*  Date        2019-04-11                                                   */
;/*  Target-mcu  HC32F120                                                     */
;/*****************************************************************************/

; Stack Configuration
; Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; Heap Configuration
;  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors
                DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; Hard Fault
                DCD     0xA5FF                    ; Reserved
                DCD     0xA500                    ; Reserved
                DCD     0x40014020                ; Reserved
                DCD     0x42280190                ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV
                DCD     SysTick_Handler           ; SysTick

                DCD     EXINT00_Handler
                DCD     EXINT01_Handler
                DCD     EXINT02_Handler
                DCD     EXINT03_Handler
                DCD     EXINT04_Handler
                DCD     EXINT05_Handler
                DCD     EXINT06_Handler
                DCD     EXINT07_Handler
                DCD     IRQ008_Handler
                DCD     IRQ009_Handler
                DCD     IRQ010_Handler
                DCD     IRQ011_Handler
                DCD     IRQ012_Handler
                DCD     IRQ013_Handler
                DCD     IRQ014_Handler
                DCD     IRQ015_Handler
                DCD     IRQ016_Handler
                DCD     IRQ017_Handler
                DCD     IRQ018_Handler
                DCD     IRQ019_Handler
                DCD     IRQ020_Handler
                DCD     IRQ021_Handler
                DCD     IRQ022_Handler
                DCD     IRQ023_Handler
                DCD     IRQ024_Handler
                DCD     IRQ025_Handler
                DCD     IRQ026_Handler
                DCD     IRQ027_Handler
                DCD     IRQ028_Handler
                DCD     IRQ029_Handler
                DCD     IRQ030_Handler
                DCD     IRQ031_Handler



__Vectors_End

__Vectors_Size                     EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                LDR     R0,=__Vectors
                LDR     R1,[R0,#0x10]; unprotect
                LDR     R2,[R0,#0x14]; protect
                LDR     R3,[R0,#0x18]; PWC_FPRC
                LDR     R4,[R0,#0x1C]; PWC_RAMCR_RPERDIS
                MOVS    R5,#1
                STR     R1,[R3]
                STRB    R5,[R4]
                STR     R2,[R3]

               ;reset NVIC if in rom debug
                LDR     R0, =0x20000000
                LDR     R2, =0x0
                MOVS    R1, #0                 ; for warning,
                ADD     R1, PC,#0              ; for A1609W,
                CMP     R1, R0
                BLS     RAMCODE

              ; ram code base address.
                ADD     R2, R0,R2
RAMCODE
              ; reset Vector table address.
                LDR     R0, =0xE000ED08
                STR     R2, [R0]

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP


HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  EXINT00_Handler           [WEAK]
                EXPORT  EXINT01_Handler           [WEAK]
                EXPORT  EXINT02_Handler           [WEAK]
                EXPORT  EXINT03_Handler           [WEAK]
                EXPORT  EXINT04_Handler           [WEAK]
                EXPORT  EXINT05_Handler           [WEAK]
                EXPORT  EXINT06_Handler           [WEAK]
                EXPORT  EXINT07_Handler           [WEAK]
                EXPORT  IRQ008_Handler            [WEAK]
                EXPORT  IRQ009_Handler            [WEAK]
                EXPORT  IRQ010_Handler            [WEAK]
                EXPORT  IRQ011_Handler            [WEAK]
                EXPORT  IRQ012_Handler            [WEAK]
                EXPORT  IRQ013_Handler            [WEAK]
                EXPORT  IRQ014_Handler            [WEAK]
                EXPORT  IRQ015_Handler            [WEAK]
                EXPORT  IRQ016_Handler            [WEAK]
                EXPORT  IRQ017_Handler            [WEAK]
                EXPORT  IRQ018_Handler            [WEAK]
                EXPORT  IRQ019_Handler            [WEAK]
                EXPORT  IRQ020_Handler            [WEAK]
                EXPORT  IRQ021_Handler            [WEAK]
                EXPORT  IRQ022_Handler            [WEAK]
                EXPORT  IRQ023_Handler            [WEAK]
                EXPORT  IRQ024_Handler            [WEAK]
                EXPORT  IRQ025_Handler            [WEAK]
                EXPORT  IRQ026_Handler            [WEAK]
                EXPORT  IRQ027_Handler            [WEAK]
                EXPORT  IRQ028_Handler            [WEAK]
                EXPORT  IRQ029_Handler            [WEAK]
                EXPORT  IRQ030_Handler            [WEAK]
                EXPORT  IRQ031_Handler            [WEAK]

EXINT00_Handler
EXINT01_Handler
EXINT02_Handler
EXINT03_Handler
EXINT04_Handler
EXINT05_Handler
EXINT06_Handler
EXINT07_Handler
IRQ008_Handler
IRQ009_Handler
IRQ010_Handler
IRQ011_Handler
IRQ012_Handler
IRQ013_Handler
IRQ014_Handler
IRQ015_Handler
IRQ016_Handler
IRQ017_Handler
IRQ018_Handler
IRQ019_Handler
IRQ020_Handler
IRQ021_Handler
IRQ022_Handler
IRQ023_Handler
IRQ024_Handler
IRQ025_Handler
IRQ026_Handler
IRQ027_Handler
IRQ028_Handler
IRQ029_Handler
IRQ030_Handler
IRQ031_Handler
                B .

                ENDP

                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
