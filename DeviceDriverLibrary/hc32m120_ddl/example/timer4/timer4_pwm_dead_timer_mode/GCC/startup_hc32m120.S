/*
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
*/
/*****************************************************************************/
/*  Startup for GCC                                                          */
/*  Version     V1.0                                                         */
/*  Date        2019-03-13                                                   */
/*  Target-mcu  HC32M120                                                     */
/*****************************************************************************/

/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

                .syntax     unified
                .arch       armv6-m
                .cpu        cortex-m0plus
                .thumb

/*
;<h> Stack Configuration
;  <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>
*/
                .equ        Stack_Size, 0x00000200

                .section    .stack
                .align      3
                .globl      __StackTop
                .globl      __StackLimit
__StackLimit:
                .space      Stack_Size
                .size       __StackLimit, . - __StackLimit
__StackTop:
                .size       __StackTop, . - __StackTop


/*
;<h> Heap Configuration
;  <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>
*/
                .equ        Heap_Size, 0x00000200

                .if         Heap_Size != 0                     /* Heap is provided */
                .section    .heap
                .align      3
                .globl      __HeapBase
                .globl      __HeapLimit
__HeapBase:
                .space      Heap_Size
                .size       __HeapBase, . - __HeapBase
__HeapLimit:
                .size       __HeapLimit, . - __HeapLimit
                .endif

/*
;<h> Interrupt vector table start.
*/
                .section    .vectors, "a", %progbits
                .align      2
                .type       __Vectors, %object
                .globl      __Vectors
                .globl      __Vectors_End
                .globl      __Vectors_Size
__Vectors:
                .long       __StackTop                         /*     Top of Stack */
                .long       Reset_Handler                      /*     Reset Handler */
                .long       NMI_Handler                        /* -14 NMI Handler */
                .long       HardFault_Handler                  /* -13 Hard Fault Handler */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       SVC_Handler                        /*  -5 SVCall Handler */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       PendSV_Handler                     /*  -2 PendSV Handler */
                .long       SysTick_Handler                    /*  -1 SysTick Handler */

                /* Interrupts */
                .long       EXINT00_Handler
                .long       EXINT01_Handler
                .long       EXINT02_Handler
                .long       EXINT03_Handler
                .long       EXINT04_Handler
                .long       EXINT05_Handler
                .long       EXINT06_Handler
                .long       EXINT07_Handler
                .long       IRQ008_Handler
                .long       IRQ009_Handler
                .long       IRQ010_Handler
                .long       IRQ011_Handler
                .long       IRQ012_Handler
                .long       IRQ013_Handler
                .long       IRQ014_Handler
                .long       IRQ015_Handler
                .long       IRQ016_Handler
                .long       IRQ017_Handler
                .long       IRQ018_Handler
                .long       IRQ019_Handler
                .long       IRQ020_Handler
                .long       IRQ021_Handler
                .long       IRQ022_Handler
                .long       IRQ023_Handler
                .long       IRQ024_Handler
                .long       IRQ025_Handler
                .long       IRQ026_Handler
                .long       IRQ027_Handler
                .long       IRQ028_Handler
                .long       IRQ029_Handler
                .long       IRQ030_Handler
                .long       IRQ031_Handler
__Vectors_End:
                .equ        __Vectors_Size, __Vectors_End - __Vectors
                .size       __Vectors, . - __Vectors
/*
;<h> Interrupt vector table end.
*/

/*
;<h> Reset handler start.
*/
                .section    .text.Reset_Handler
                .align      2
                .weak       Reset_Handler
                .type       Reset_Handler, %function
                .globl      Reset_Handler
Reset_Handler:
                /* Set stack top pointer. */
                /*ldr         sp, =__StackTop*/
                ldr         r2, =0x40014020     /* PWC_FPRC */
                ldr         r0, =0xA502
                str         r0, [r2]            /* PWC_FPRC = 0xA502 */

                movs        r0, #1
                ldr         r1, =0x42280190     /* PWC_RAMCR_RPERDIS */
                strb        r0, [r1]            /* PWC_RAMCR_RPERDIS = 1 */

                ldr         r1, =0x42280004     /* PWC_STPMCR_CKSHRC */
                strb        r0, [r1]            /* PWC_STPMCR_CKSHRC = 1 */

                ldr         r0, =0xA500
                str         r0, [r2]            /* PWC_FPRC = 0xA500 */

/* Single section scheme.
 *
 * The ranges of copy from/to are specified by following symbols
 *   __etext: LMA of start of the section to copy from. Usually end of text
 *   __data_start__: VMA of start of the section to copy to
 *   __data_end__: VMA of end of the section to copy to
 *
 * All addresses must be aligned to 4 bytes boundary.
 */
                /* Copy data from read only memory to RAM. */
CopyData:
                ldr         r1, =__etext
                ldr         r2, =__data_start__
                ldr         r3, =__data_end__

                subs        r3, r2
                ble         CopyLoopExit
CopyLoop:
                subs        r3, #4
                ldr         r0, [r1,r3]
                str         r0, [r2,r3]
                bgt         CopyLoop
CopyLoopExit:

/* This part of work usually is done in C library startup code.
 * Otherwise, define this macro to enable it in this startup.
 *
 * There are two schemes too.
 * One can clear multiple BSS sections. Another can only clear one section.
 * The former is more size expensive than the latter.
 *
 * Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 * Otherwise define macro __STARTUP_CLEAR_BSS to choose the later.
 */
/* Single BSS section scheme.
 *
 * The BSS section is specified by following symbols
 *   __bss_start__: start of the BSS section.
 *   __bss_end__: end of the BSS section.
 *
 * Both addresses must be aligned to 4 bytes boundary.
 */
                /* Clear BSS section. */
ClearBss:
                ldr         r1, =__bss_start__
                ldr         r2, =__bss_end__

                movs        r0, 0
                subs        r2, r1
                ble         ClearLoopExit
ClearLoop:
                subs        r2, #4
                str         r0, [r1, r2]
                bgt         ClearLoop
ClearLoopExit:
                /* Call the clock system initialization function. */
                bl          SystemInit
                /* Call the application's entry point. */
                bl          main
                bx          lr
                .size       Reset_Handler, . - Reset_Handler
/*
;<h> Reset handler end.
*/

/*
;<h> Default handler start.
*/
                .section    .text.Default_Handler, "ax", %progbits
                .align      2
Default_Handler:
                b           .
                .size       Default_Handler, . - Default_Handler
/*
;<h> Default handler end.
*/

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro      Set_Default_Handler  Handler_Name
                .weak       \Handler_Name
                .set        \Handler_Name, Default_Handler
                .endm

/* Default exception/interrupt handler */

                Set_Default_Handler    NMI_Handler
                Set_Default_Handler    HardFault_Handler
                Set_Default_Handler    SVC_Handler
                Set_Default_Handler    PendSV_Handler
                Set_Default_Handler    SysTick_Handler

                Set_Default_Handler    EXINT00_Handler
                Set_Default_Handler    EXINT01_Handler
                Set_Default_Handler    EXINT02_Handler
                Set_Default_Handler    EXINT03_Handler
                Set_Default_Handler    EXINT04_Handler
                Set_Default_Handler    EXINT05_Handler
                Set_Default_Handler    EXINT06_Handler
                Set_Default_Handler    EXINT07_Handler
                Set_Default_Handler    IRQ008_Handler
                Set_Default_Handler    IRQ009_Handler
                Set_Default_Handler    IRQ010_Handler
                Set_Default_Handler    IRQ011_Handler
                Set_Default_Handler    IRQ012_Handler
                Set_Default_Handler    IRQ013_Handler
                Set_Default_Handler    IRQ014_Handler
                Set_Default_Handler    IRQ015_Handler
                Set_Default_Handler    IRQ016_Handler
                Set_Default_Handler    IRQ017_Handler
                Set_Default_Handler    IRQ018_Handler
                Set_Default_Handler    IRQ019_Handler
                Set_Default_Handler    IRQ020_Handler
                Set_Default_Handler    IRQ021_Handler
                Set_Default_Handler    IRQ022_Handler
                Set_Default_Handler    IRQ023_Handler
                Set_Default_Handler    IRQ024_Handler
                Set_Default_Handler    IRQ025_Handler
                Set_Default_Handler    IRQ026_Handler
                Set_Default_Handler    IRQ027_Handler
                Set_Default_Handler    IRQ028_Handler
                Set_Default_Handler    IRQ029_Handler
                Set_Default_Handler    IRQ030_Handler
                Set_Default_Handler    IRQ031_Handler

                .end
