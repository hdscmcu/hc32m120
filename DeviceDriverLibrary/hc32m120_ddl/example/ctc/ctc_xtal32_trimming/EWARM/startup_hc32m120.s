;*******************************************************************************
; Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
;
; This software is owned and published by:
; Huada Semiconductor Co.,Ltd ("HDSC").
;
; BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
; BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
;
; This software contains source code for use with HDSC
; components. This software is licensed by HDSC to be adapted only
; for use in systems utilizing HDSC components. HDSC shall not be
; responsible for misuse or illegal use of this software for devices not
; supported herein. HDSC is providing this software "AS IS" and will
; not be responsible for issues arising from incorrect user implementation
; of the software.
;
; Disclaimer:
; HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
; REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
; ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
; WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
; WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
; WARRANTY OF NONINFRINGEMENT.
; HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
; NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
; LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
; LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
; INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
; INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
; SAVINGS OR PROFITS,
; EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
; YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
; INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
; FROM, THE SOFTWARE.
;
; This software may be replicated in part or whole for the licensed use,
; with the restriction that this Disclaimer and Copyright notice must be
; included with each copy of this software, whether used in part or whole,
; at all times.
;/
;/*****************************************************************************/
;/*  Startup for IAR                                                          */
;/*  Version     V1.0                                                         */
;/*  Date        2019-04-04                                                   */
;/*  Target-mcu  M0+ Device                                                   */
;/*****************************************************************************/


                MODULE  ?cstartup

                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                SECTION .intvec:CODE:NOROOT(2)

                EXTERN  __iar_program_start
                EXTERN  SystemInit
                PUBLIC  __vector_table

                SECTION .intvec:CODE:NOROOT(2)
                DATA
__vector_table
                DCD     sfe(CSTACK)               ; Top of Stack
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

; Numbered IRQ handler vectors

; Note: renaming to device dependent ISR function names are done in
;       ddl.h (section "IRQ name definition for all type MCUs")
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


                THUMB
; Dummy Exception Handlers (infinite loops which can be modified)

                PUBWEAK Reset_Handler
                SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
                LDR     R0,=__vector_table
                LDR     R1,[R0,#0x10]; unprotect
                LDR     R2,[R0,#0x14]; protect
                LDR     R3,[R0,#0x18]; PWC_FPRC
                LDR     R4,[R0,#0x1C]; PWC_RAMCR_RPERDIS
                MOVS    R5,#1
                STR     R1,[R3]
                STRB    R5,[R4]
                STR     R2,[R3]

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0

                PUBWEAK NMI_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
                B       NMI_Handler

                PUBWEAK HardFault_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
                B       HardFault_Handler

                PUBWEAK MemManage_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
MemManage_Handler
                B       MemManage_Handler

                PUBWEAK BusFault_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
BusFault_Handler
                B       BusFault_Handler

                PUBWEAK UsageFault_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
UsageFault_Handler
                B       UsageFault_Handler

                PUBWEAK SVC_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
                B       SVC_Handler

                PUBWEAK DebugMon_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
DebugMon_Handler
                B       DebugMon_Handler

                PUBWEAK PendSV_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
                B       PendSV_Handler

                PUBWEAK SysTick_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
                B       SysTick_Handler



                PUBWEAK EXINT00_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXINT00_Handler
                B       EXINT00_Handler


                PUBWEAK EXINT01_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXINT01_Handler
                B       EXINT01_Handler


                PUBWEAK EXINT02_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXINT02_Handler
                B       EXINT02_Handler


                PUBWEAK EXINT03_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXINT03_Handler
                B       EXINT03_Handler


                PUBWEAK EXINT04_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXINT04_Handler
                B       EXINT04_Handler


                PUBWEAK EXINT05_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXINT05_Handler
                B       EXINT05_Handler


                PUBWEAK EXINT06_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXINT06_Handler
                B       EXINT06_Handler


                PUBWEAK EXINT07_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXINT07_Handler
                B       EXINT07_Handler


                PUBWEAK IRQ008_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ008_Handler
                B       IRQ008_Handler


                PUBWEAK IRQ009_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ009_Handler
                B       IRQ009_Handler


                PUBWEAK IRQ010_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ010_Handler
                B       IRQ010_Handler


                PUBWEAK IRQ011_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ011_Handler
                B       IRQ011_Handler


                PUBWEAK IRQ012_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ012_Handler
                B       IRQ012_Handler


                PUBWEAK IRQ013_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ013_Handler
                B       IRQ013_Handler


                PUBWEAK IRQ014_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ014_Handler
                B       IRQ014_Handler


                PUBWEAK IRQ015_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ015_Handler
                B       IRQ015_Handler


                PUBWEAK IRQ016_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ016_Handler
                B       IRQ016_Handler


                PUBWEAK IRQ017_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ017_Handler
                B       IRQ017_Handler


                PUBWEAK IRQ018_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ018_Handler
                B       IRQ018_Handler


                PUBWEAK IRQ019_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ019_Handler
                B       IRQ019_Handler


                PUBWEAK IRQ020_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ020_Handler
                B       IRQ020_Handler


                PUBWEAK IRQ021_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ021_Handler
                B       IRQ021_Handler


                PUBWEAK IRQ022_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ022_Handler
                B       IRQ022_Handler


                PUBWEAK IRQ023_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ023_Handler
                B       IRQ023_Handler


                PUBWEAK IRQ024_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ024_Handler
                B       IRQ024_Handler


                PUBWEAK IRQ025_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ025_Handler
                B       IRQ025_Handler


                PUBWEAK IRQ026_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ026_Handler
                B       IRQ026_Handler


                PUBWEAK IRQ027_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ027_Handler
                B       IRQ027_Handler


                PUBWEAK IRQ028_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ028_Handler
                B       IRQ028_Handler


                PUBWEAK IRQ029_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ029_Handler
                B       IRQ029_Handler


                PUBWEAK IRQ030_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ030_Handler
                B       IRQ030_Handler


                PUBWEAK IRQ031_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ031_Handler
                B       IRQ031_Handler

                END
