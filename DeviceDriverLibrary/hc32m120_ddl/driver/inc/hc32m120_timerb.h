/**
 *******************************************************************************
 * @file  hc32m120_timerb.h
 * @brief This file contains all the functions prototypes of the TimerB driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-03       Hongjh          First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 *******************************************************************************
 */
#ifndef __HC32M120_TIMERB_H__
#define __HC32M120_TIMERB_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_TIMERB
 * @{
 */

#if (DDL_TIMERB_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup TIMERB_Global_Types TimerB Global Types
 * @{
 */

/**
 * @brief TimerB initialization structure definition
 */
typedef struct
{
    uint16_t u16CntMode;                /*!< TimerB count mode.
                                             This parameter can be a value of @ref TIMERB_Count_Mode */

    uint16_t u16CntDir;                 /*!< TimerB count direction.
                                             This parameter can be a value of @ref TIMERB_Count_Direction */

    uint16_t u16SynStartState;          /*!< Enable/disable synchronization start function and only unit n(2, 4, 6, 8) is valid.
                                             This parameter can be a value of @ref TIMERB_Synchronization_Start_State */

    uint16_t u16ClkDiv;                 /*!< TimerB Count clock division select.
                                             This parameter can be a value of @ref TIMERB_ClockDivision */

    uint16_t u16PeriodVal;              /*!< TimerB period value.
                                             This parameter can be a value of half-word */

    uint16_t u16OverflowAction;         /*!< Stop/continue count when TimerB count overflow.
                                             This parameter can be a value of @ref TIMERB_Overflow_Control */

    uint16_t u16HwStartCondition;       /*!< TIMERB hardware trigger start count condition.
                                             This parameter can be a value of @ref TIMERB_Hardware_Start_Count_Condition */

    uint16_t u16HwStopCondition;        /*!< TIMERB hardware trigger stop count condition.
                                             This parameter can be a value of @ref TIMERB_Hardware_Stop_Count_Condition */

    uint16_t u16HwClearCondition;       /*!< TIMERB hardware trigger clear count condition.
                                             This parameter can be a value of @ref TIMERB_Hardware_Clear_Count_Condition */

    uint16_t u16HwUpCondition;          /*!< TIMERB hardware trigger count up condition.
                                             This parameter can be a value of @ref TIMERB_Hardware_Count_Up_Condition */

    uint16_t u16HwDownCondition;        /*!< TIMERB hardware trigger count down condition.
                                             This parameter can be a value of @ref TIMERB_Hardware_Count_Down_Condition */
} stc_timerb_init_t;

/**
 * @brief TimerB output compare initialization structure definition
 */
typedef struct
{
    uint16_t u16CompareVal;             /*!< TimerB compare value.
                                             This parameter can be a value of half-word */

    uint16_t u16PortOutputState;        /*!< TIMB_<t>_PWM1 port output state
                                             This parameter can be a value of @ref TIMERB_OC_Port_Output_State */

    uint16_t u16StartCntOutput;         /*!< TIMB_<t>_PWM1 output polarity when start count.
                                             This parameter can be a value of @ref TIMERB_OC_Start_Count_Output_Polarity */

    uint16_t u16StopCntOutput;          /*!< TIMB_<t>_PWM1 output polarity when stop count
                                             This parameter can be a value of @ref TIMERB_OC_Stop_Count_Output_Polarity */

    uint16_t u16CompareMatchOutput;     /*!< TIMB_<t>_PWM1 output polarity when compare value match
                                             This parameter can be a value of @ref TIMERB_OC_Compare_Match_Output_Polarity */

    uint16_t u16PeriodMatchOutput;      /*!< TIMB_<t>_PWM1 output polarity when period value match
                                             This parameter can be a value of @ref TIMERB_OC_Period_Match_Output_Polarity */
} stc_timerb_oc_init_t;

/**
 * @brief TimerB input capture initialization structure definition
 */
typedef struct
{
    uint16_t u16NoiseFilterClkDiv;      /*!< TimerB Noise filter clock division.
                                             This parameter can be a value of @ref TIMERB_Input_Capture_Noise_Filter_Clock_Division */

    uint16_t u16CaptureCondition;       /*!< TimerB input capture  condition.
                                             This parameter can be a value of @ref TIMERB_Input_Capture_Condition */
} stc_timerb_ic_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMERB_Global_Macros TimerB Global Macros
 * @{
 */

/**
 * @defgroup TIMERB_Count_Direction TimerB Count Direction
 * @{
 */
#define TIMERB_CNT_UP                           (TMRB_BCSTR_DIR)
#define TIMERB_CNT_DOWN                         ((uint16_t)0x0000u)
/**
 * @}
 */

/**
 * @defgroup TIMERB_Count_Mode TimerB Count Mode
 * @{
 */
#define TIMERB_TRIANGLE_WAVE                    (TMRB_BCSTR_MODE)
#define TIMERB_SAWTOOTH_WAVE                    ((uint16_t)0x0000u)
/**
 * @}
 */

/**
 * @defgroup TIMERB_Synchronization_Start_State TimerB Synchronization Start State
 * @{
 */
#define TIMERB_SYNC_START_ENABLE                (TMRB_BCSTR_SYNST)
#define TIMERB_SYNC_START_DISABLE               ((uint16_t)0x0000u)
/**
 * @}
 */

/**
 * @defgroup TIMERB_ClockDivision TimerB Clock Division
 * @{
 */
#define TIMERB_CLKDIV_DIV1                      ((uint16_t)0x0000u)                                             /*!< HCLK      */
#define TIMERB_CLKDIV_DIV2                      (TMRB_BCSTR_CKDIV_0)                                            /*!< HCLK/2    */
#define TIMERB_CLKDIV_DIV4                      (TMRB_BCSTR_CKDIV_1)                                            /*!< HCLK/4    */
#define TIMERB_CLKDIV_DIV8                      (TMRB_BCSTR_CKDIV_1 | TMRB_BCSTR_CKDIV_0)                       /*!< HCLK/8    */
#define TIMERB_CLKDIV_DIV16                     (TMRB_BCSTR_CKDIV_2)                                            /*!< HCLK/16   */
#define TIMERB_CLKDIV_DIV32                     (TMRB_BCSTR_CKDIV_2 | TMRB_BCSTR_CKDIV_0)                       /*!< HCLK/32   */
#define TIMERB_CLKDIV_DIV64                     (TMRB_BCSTR_CKDIV_2 | TMRB_BCSTR_CKDIV_1)                       /*!< HCLK/64   */
#define TIMERB_CLKDIV_DIV128                    (TMRB_BCSTR_CKDIV_2 | TMRB_BCSTR_CKDIV_1 | TMRB_BCSTR_CKDIV_0)  /*!< HCLK/128  */
#define TIMERB_CLKDIV_DIV256                    (TMRB_BCSTR_CKDIV_3)                                            /*!< HCLK/256  */
#define TIMERB_CLKDIV_DIV512                    (TMRB_BCSTR_CKDIV_3 | TMRB_BCSTR_CKDIV_0)                       /*!< HCLK/512  */
#define TIMERB_CLKDIV_DIV1024                   (TMRB_BCSTR_CKDIV_3 | TMRB_BCSTR_CKDIV_1)                       /*!< HCLK/1024 */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Overflow_Control TimerB Overflow Control
 * @{
 */
#define TIMERB_OVERFLOW_STOP                    (TMRB_BCSTR_OVSTP)
#define TIMERB_OVERFLOW_COUNT                   ((uint16_t)0x0000u)
/**
 * @}
 */

/**
 * @defgroup TIMERB_Flag TIMERB Flag
 * @{
 */
#define TIMERB_FLAG_OVF                         (TMRB_BCSTR_OVFF)
#define TIMERB_FLAG_UDF                         (TMRB_BCSTR_UDFF)
#define TIMERB_FLAG_CMP                         (TMRB_STFLR_CMPF1)
/**
 * @}
 */

/**
 * @defgroup TIMERB_Interrupt_definition TIMERB Interrupts Definition
 * @{
 */
#define TIMERB_IT_OVF                           (TMRB_BCSTR_INENOVF)
#define TIMERB_IT_UDF                           (TMRB_BCSTR_INENUDF)
#define TIMERB_IT_CMP                           (TMRB_ICONR_ITEN1)
/**
 * @}
 */

/**
 * @defgroup TIMERB_Compare_Match_Event TimerB Compare Match Event
 * @{
 */
#define TIMERB_CMP_EVT_ENABLE                   (TMRB_ECONR_ETEN1)
#define TIMERB_CMP_EVT_DISABLE                  ((uint16_t)0x0000u)
/**
 * @}
 */

/**
 * @defgroup TIMERB_Function_Mode TimerB Function Mode
 * @{
 */
#define TIMERB_INPUT_CAPTURE                    (TMRB_CCONR_CAPMDA)     /*!< Capture input function */
#define TIMERB_OUTPUT_COMPARE                   ((uint16_t)0x0000u)     /*!< Comare output function */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Input_Capture_Condition TimerB Input Capture Condition
 * @note TIMB_<t>_PWM1 _<t>_ is unit number,range 1~8
 * @{
 */
#define TIMERB_IC_RISING                        (TMRB_CCONR_HICP0)      /*!< Capture triggered by rising edge on timer input TIMB_<t>_PWM1 */
#define TIMERB_IC_FALLING                       (TMRB_CCONR_HICP1)      /*!< Capture triggered by falling edge on timer input TIMB_<t>_PWM1 */
#define TIMERB_IC_EVT                           (TMRB_CCONR_HICP2)      /*!< Capture triggered by the timer TMRB_HTSSR specified event */
#define TIMERB_IC_INVALID                       ((uint16_t)0x0000u)     /*!< Don't occur capture action */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Input_Capture_Noise_Filter_Clock_Division TimerB Input Capture Noise Filter Clock Division
 * @{
 */
#define TIMERB_IC_NOFICLKDIV_NONE               ((uint16_t)0x0000u)                             /*!< Disable filter */
#define TIMERB_IC_NOFICLKDIV_DIV1               (TMRB_CCONR_NOFIENCP)                           /*!< HCLK          */
#define TIMERB_IC_NOFICLKDIV_DIV4               (TMRB_CCONR_NOFIENCP | TMRB_CCONR_NOFICKCP_0)   /*!< HCLK/4        */
#define TIMERB_IC_NOFICLKDIV_DIV16              (TMRB_CCONR_NOFIENCP | TMRB_CCONR_NOFICKCP_1)   /*!< HCLK/16       */
#define TIMERB_IC_NOFICLKDIV_DIV64              (TMRB_CCONR_NOFIENCP | TMRB_CCONR_NOFICKCP)     /*!< HCLK/64       */
/**
 * @}
 */

/**
 * @defgroup TIMERB_OC_Start_Count_Output_Polarity TimerB Output Compare Start Count Output Polarity
 * @note TIMB_<t>_PWM1 _<t>_ is unit number,range 1~8
 * @{
 */
#define TIMERB_OC_STARTCNT_OUTPUT_LOW           ((uint16_t)0x0000u) /*!< TIM_<t>_PWM1 output low level when TimerB start count */
#define TIMERB_OC_STARTCNT_OUTPUT_HIGH          (TMRB_PCONR_STAC_0) /*!< TIM_<t>_PWM1 output high level when TimerB start count */
#define TIMERB_OC_STARTCNT_OUTPUT_HOLD          (TMRB_PCONR_STAC_1) /*!< TIM_<t>_PWM1 output hold level when TimerB start count */
/**
 * @}
 */

/**
 * @defgroup TIMERB_OC_Stop_Count_Output_Polarity TimerB Output Compare Stop Count Output Polarity
 * @note TIMB_<t>_PWM1 _<t>_ is unit number,range 1~8
 * @{
 */
#define TIMERB_OC_STOPCNT_OUTPUT_LOW            ((uint16_t)0x0000u) /*!< TIM_<t>_PWM1 output low level when TimerB stop count */
#define TIMERB_OC_STOPCNT_OUTPUT_HIGH           (TMRB_PCONR_STPC_0) /*!< TIM_<t>_PWM1 output high level when TimerB stop count */
#define TIMERB_OC_STOPCNT_OUTPUT_HOLD           (TMRB_PCONR_STPC_1) /*!< TIM_<t>_PWM1 output hold level when TimerB stop count */
/**
 * @}
 */

/**
 * @defgroup TIMERB_OC_Compare_Match_Output_Polarity TimerB Output Compare Compare Match Output Polarity
 * @note TIMB_<t>_PWM1 _<t>_ is unit number,range 1~8
 * @{
 */
#define TIMERB_OC_CMPMATCH_OUTPUT_LOW           ((uint16_t)0x0000u) /*!< TIM_<t>_PWM1 output low level when TimerB comapre match */
#define TIMERB_OC_CMPMATCH_OUTPUT_HIGH          (TMRB_PCONR_CMPC_0) /*!< TIM_<t>_PWM1 output high level when TimerB comapre match */
#define TIMERB_OC_CMPMATCH_OUTPUT_HOLD          (TMRB_PCONR_CMPC_1) /*!< TIM_<t>_PWM1 output hold level when TimerB comapre match */
#define TIMERB_OC_CMPMATCH_OUTPUT_INVERTED      (TMRB_PCONR_CMPC)   /*!< TIM_<t>_PWM1 output inverted level when TimerB comapre match */
/**
 * @}
 */

/**
 * @defgroup TIMERB_OC_Period_Match_Output_Polarity TimerB Output Compare Period Match Output Polarity
 * @note TIMB_<t>_PWM1 _<t>_ is unit number,range 1~8
 * @{
 */
#define TIMERB_OC_PERIODMATCH_OUTPUT_LOW        ((uint16_t)0x0000u) /*!< TIM_<t>_PWM1 output low level when TimerB period match */
#define TIMERB_OC_PERIODMATCH_OUTPUT_HIGH       (TMRB_PCONR_PERC_0) /*!< TIM_<t>_PWM1 output high level when TimerB period match */
#define TIMERB_OC_PERIODMATCH_OUTPUT_HOLD       (TMRB_PCONR_PERC_1) /*!< TIM_<t>_PWM1 output hold level when TimerB period match */
#define TIMERB_OC_PERIODMATCH_OUTPUT_INVERTED   (TMRB_PCONR_PERC)   /*!< TIM_<t>_PWM1 output inverted level when TimerB period match */
/**
 * @}
 */

/**
 * @defgroup TIMERB_OC_Force_Output_Polarity TimerB Output Compare Force Output Polarity
 * @note TIMB_<t>_PWM1 _<t>_ is unit number,range 1~8
 * @{
 */
#define TIMERB_OC_FORCE_OUTPUT_INVALID          ((uint16_t)0x0000u) /*!< Invalid */
#define TIMERB_OC_FORCE_OUTPUT_LOW              (TMRB_PCONR_FORC_1) /*!< Force TIM_<t>_PWM1  output low level */
#define TIMERB_OC_FORCE_OUTPUT_HIGH             (TMRB_PCONR_FORC)   /*!< Force TIM_<t>_PWM1 force output high level */
/**
 * @}
 */

/**
 * @defgroup TIMERB_OC_Port_Output_State TimerB Output Compare Port Output State
 * @note TIMB_<t>_PWM1 _<t>_ is unit number,range 1~8
 * @{
 */
#define TIMERB_OC_PORT_OUTPUT_ENABLE            (TMRB_PCONR_OUTEN)  /*!< Enable Output Compare TIMB_<t>_PWM1 output function */
#define TIMERB_OC_PORT_OUTPUT_DISABLE           ((uint16_t)0x0000u) /*!< Disable Output Compare TIMB_<t>_PWM1 output function */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Hardware_Start_Count_Condition TimerB Hardware Start Count Condition
 * @note TIMB_<T>_PWM1 _<T>_ is range 1~8 when sync start function is invalid
 * @note TIMB_<T>_PWM1 _<T>_ is 2,4,6, or 8 when sync start function is valid
 * @{
 */
#define TIMERB_HWSTART_INVALID                  ((uint16_t)0x0000u) /*!< Hardware start is invalid */
#define TIMERB_HWSTART_TIMB_T_PWM1_RISING       (TMRB_HCONR_HSTA0)  /*!< Hardware start by rising edge on timer input TIMB_<t>_PWM1 (t = 1~8) when sysnc start function is invalid or
                                                                         hardware start by rising edge on timer input TIMB_n_PWM1 (n = 1,3,5,7) when sysnc start function is invalid */
#define TIMERB_HWSTART_TIMB_T_PWM1_FALLING      (TMRB_HCONR_HSTA1)  /*!< Hardware start by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8) when sysnc start function is invalid or
                                                                         hardware start by falling edge on timer input TIMB_n_PWM1 (n = 1,3,5,7) when sysnc start function is invalid */
#define TIMERB_HWSTART_TIMB_EVT                 (TMRB_HCONR_HSTA2)  /*!< Hardware start by the timer TMRB_HTSSR specified event */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Hardware_Stop_Count_Condition TimerB Hardware Stop Count Condition
 * @note TIMB_<T>_PWM1 _<T>_ is range 1~8
 * @{
 */
#define TIMERB_HWSTOP_INVALID                   ((uint16_t)0x0000u) /*!< Hardware stop is invalid */
#define TIMERB_HWSTOP_TIMB_T_PWM1_RISING        (TMRB_HCONR_HSTP0)  /*!< Hardware stop by rising edge on timer input TIMB_<t>_PWM1 (t = 1~8) */
#define TIMERB_HWSTOP_TIMB_T_PWM1_FALLING       (TMRB_HCONR_HSTP1)  /*!< Hardware stop by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8) */
#define TIMERB_HWSTOP_TIMB_EVT                  (TMRB_HCONR_HSTP2)  /*!< Hardware stop by the timer TMRB_HTSSR specified event */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Hardware_Clear_Count_Condition TimerB Hardware Clear Count Condition
 * @note TIMB_<T>_PWM1 _<T>_ is range 1~8.
 * @note TIMB_<N>_PWM1 _<N>_ is 2/4/6/8 when m=1/3/5/7 OR TIMB_N_PWM1 _<N>_ is 1/3/5/7 when 2/4/6/8
 * @{
 */
#define TIMERB_HWCLEAR_INVALID                  ((uint16_t)0x0000u) /*!< Hardware clear is invalid */
#define TIMERB_HWCLEAR_TIMB_T_PWM1_RISING       (TMRB_HCONR_HCLE0)  /*!< Hardware clear count by rising edge on timer input TIMB_<t>_PWM1, t = 1~8 */
#define TIMERB_HWCLEAR_TIMB_T_PWM1_FALLING      (TMRB_HCONR_HCLE1)  /*!< Hardware clear count by falling edge on timer input TIMB_<t>_PWM1, t = 1~8 */
#define TIMERB_HWCLEAR_EVT                      (TMRB_HCONR_HCLE2)  /*!< Hardware clear count by the timer TMRB_HTSSR specified event */
#define TIMERB_HWCLEAR_TIMB_N_PWM1_RISING       (TMRB_HCONR_HCLE3)  /*!< Hardware clear count by rising edge on timer input TIMB_<n>_PWM1, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8 */
#define TIMERB_HWCLEAR_TIMB_N_PWM1_FALLING      (TMRB_HCONR_HCLE4)  /*!< Hardware clear count by falling edge on timer input TIMB_<n>_PWM1, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8 */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Hardware_Count_Up_Condition TimerB Hardware Count Up Condition
 * @note TIMB_<T>_PWM1 _<T>_ is range 1~8.
 * @note TIMB_<N>_PWM1 _<N>_ is 2/4/6/8 when m=1/3/5/7 OR TIMB_N_PWM1 _<N>_ is 1/3/5/7 when 2/4/6/8
 * @{
 */
#define TIMERB_HWUP_INVALID                     ((uint16_t)0x0000u) /*!< Hardware count up is invalid */
#define TIMERB_HWUP_TIMB_T_PWM1_RISING          (TMRB_HCUPR_HCUP8)  /*!< Hardware count up by rising edge on timer input TIMB_<t>_PWM1, t = 1~8 */
#define TIMERB_HWUP_TIMB_T_PWM1_FALLING         (TMRB_HCUPR_HCUP9)  /*!< Hardware count up by falling edge on timer input TIMB_<t>_PWM1, t = 1~8 */
#define TIMERB_HWUP_EVT                         (TMRB_HCUPR_HCUP10) /*!< Hardware count up by the timer TMRB_HTSSR specified event */
#define TIMERB_HWUP_OVERFLOW                    (TMRB_HCUPR_HCUP11) /*!< Hardware count up by overflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8 */
#define TIMERB_HWUP_UNDERFLOW                   (TMRB_HCUPR_HCUP12) /*!< Hardware count up by underflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8 */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Hardware_Count_Down_Condition TimerB Hardware Count Down Condition
 * @note TIMB_<T>_PWM1 _<T>_ is range 1~8.
 * @note TIMB_<N>_PWM1 _<N>_ is 2/4/6/8 when m=1/3/5/7 OR TIMB_N_PWM1 _<N>_ is 1/3/5/7 when 2/4/6/8
 * @{
 */
#define TIMERB_HWDOWN_INVALID                   ((uint16_t)0x0000u) /*!< Hardware count down is invalid */
#define TIMERB_HWDOWN_TIMB_T_PWM1_RISING        (TMRB_HCDOR_HCDO8)  /*!< Hardware count down by rising edge on timer input TIMB_<t>_PWM1, t = 1~8 */
#define TIMERB_HWDOWN_TIMB_T_PWM1_FALLING       (TMRB_HCDOR_HCDO9)  /*!< Hardware count down by falling edge on timer input TIMB_<t>_PWM1, t = 1~8 */
#define TIMERB_HWDOWN_EVT                       (TMRB_HCDOR_HCDO10) /*!< Hardware count down by the timer TMRB_HTSSR specified event */
#define TIMERB_HWDOWN_OVERFLOW                  (TMRB_HCDOR_HCDO11) /*!< Hardware count down by overflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8 */
#define TIMERB_HWDOWN_UNDERFLOW                 (TMRB_HCDOR_HCDO12) /*!< Hardware count down by underflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8 */
/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup TIMERB_Global_Functions
 * @{
 */

/**
 * @brief  Set the TimerB counter value.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Value Counter value (between Min_Data=0 and Max_Data=0xFFFF)
 * @retval None
 */
__STATIC_INLINE void TIMERB_SetCounter(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Value)
{
    WRITE_REG16(TMRBx->CNTER, u16Value);
}

/**
 * @brief  Get the TimerB counter value.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Counter value (between Min_Data=0 and Max_Data=0xFFFF)
 */
__STATIC_INLINE uint16_t TIMERB_GetCounter(M0P_TMRB_TypeDef *TMRBx)
{
    return READ_REG16(TMRBx->CNTER);
}

/**
 * @brief  Set TimerB period register value.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Value Period value (between Min_Data=0 and Max_Data=0xFFFF)
 * @retval None
 */
__STATIC_INLINE void TIMERB_SetPeriod(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Value)
{
    WRITE_REG16(TMRBx->PERAR, u16Value);
}

/**
 * @brief  Get TimerB period register value.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Period value (between Min_Data=0 and Max_Data=0xFFFF)
 */
__STATIC_INLINE uint16_t TIMERB_GetPeriod(M0P_TMRB_TypeDef *TMRBx)
{
    return READ_REG16(TMRBx->PERAR);
}

/**
 * @brief  Set TimerB compare register value.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Value Compare value (between Min_Data=0 and Max_Data=0xFFFF)
 * @retval None
 */
__STATIC_INLINE void TIMERB_SetCompare(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Value)
{
    WRITE_REG16(TMRBx->CMPAR, u16Value);
}

/**
 * @brief  Get TimerB compare register value.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Compare value (between Min_Data=0 and Max_Data=0xFFFF)
 */
__STATIC_INLINE uint16_t TIMERB_GetCompare(M0P_TMRB_TypeDef *TMRBx)
{
    return READ_REG16(TMRBx->CMPAR);
}

/**
 * @brief  Start TimerB.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_Start(M0P_TMRB_TypeDef *TMRBx)
{
    SET_REG16_BIT(TMRBx->BCSTR, TMRB_BCSTR_START);
}

/**
 * @brief  Stop TimerB.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_Stop(M0P_TMRB_TypeDef *TMRBx)
{
    CLEAR_REG16_BIT(TMRBx->BCSTR, TMRB_BCSTR_START);
}

/**
 * @brief  Enable TimerB synchronization start function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_EnableSyncStart(M0P_TMRB_TypeDef *TMRBx)
{
    SET_REG16_BIT(TMRBx->BCSTR, TMRB_BCSTR_SYNST);
}

/**
 * @brief  Disable TimerB synchronization start function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_DisableSyncStart(M0P_TMRB_TypeDef *TMRBx)
{
    CLEAR_REG16_BIT(TMRBx->BCSTR, TMRB_BCSTR_SYNST);
}

/**
 * @brief  Enable TimerB compare match event.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_EnableCmpMatchEvt(M0P_TMRB_TypeDef *TMRBx)
{
    SET_REG16_BIT(TMRBx->ECONR, TMRB_ECONR_ETEN1);
}

/**
 * @brief  Disable TimerB compare match event.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_DisableCmpMatchEvt(M0P_TMRB_TypeDef *TMRBx)
{
    CLEAR_REG16_BIT(TMRBx->ECONR, TMRB_ECONR_ETEN1);
}

/**
 * @brief  Enable TimerB input capture noise filter function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_IC_EnableNoiseFilter(M0P_TMRB_TypeDef *TMRBx)
{
    SET_REG16_BIT(TMRBx->CCONR, TMRB_CCONR_NOFIENCP);
}

/**
 * @brief  Disable TimerB input capture noise filter function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_IC_DisableNoiseFilter(M0P_TMRB_TypeDef *TMRBx)
{
    CLEAR_REG16_BIT(TMRBx->CCONR, TMRB_CCONR_NOFIENCP);
}

/**
 * @brief  Enable TimerB output compare port output function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_OC_EnablePortOutput(M0P_TMRB_TypeDef *TMRBx)
{
    SET_REG16_BIT(TMRBx->PCONR, TMRB_PCONR_OUTEN);
}

/**
 * @brief  Disable TimerB output compare port output function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval None
 */
__STATIC_INLINE void TIMERB_OC_DisablePortOutput(M0P_TMRB_TypeDef *TMRBx)
{
    CLEAR_REG16_BIT(TMRBx->PCONR, TMRB_PCONR_OUTEN);
}

/**
 * @brief  Set TimerB trigger event source.
 * @param  [in] enEvtSrc                Trigger event source @ref en_event_src_t
 * @retval None
 */
__STATIC_INLINE void TIMERB_SetTriggerSrc(en_event_src_t enEvtSrc)
{
    WRITE_REG32(M0P_AOS->TMRB_HTSSR, enEvtSrc);
}

/**
 * @brief  Get TimerB trigger event source.
 * @param  None
 * @retval An en_event_src_t enumeration value
 */
__STATIC_INLINE en_event_src_t TIMERB_GetTriggerSrc(void)
{
    return (en_event_src_t)READ_REG32(M0P_AOS->TMRB_HTSSR);
}

/* Initialization and configuration TimerB base functions */
en_result_t TIMERB_Init(M0P_TMRB_TypeDef *TMRBx,
                            const stc_timerb_init_t *pstcInit);
en_result_t TIMERB_StructInit(stc_timerb_init_t *pstcInit);
en_result_t TIMERB_DeInit(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_SetCntDirection(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Direction);
uint16_t TIMERB_GetCntDirection(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_SetCntMode(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Mode);
uint16_t TIMERB_GetCntMode(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_SetClkDiv(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Div);
uint16_t TIMERB_GetClkDiv(M0P_TMRB_TypeDef *TMRBx);
en_flag_status_t TIMERB_GetFlag(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Flag);
void TIMERB_ClearFlag(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Flag);
void TIMERB_IntCmd(M0P_TMRB_TypeDef *TMRBx,
                        uint16_t u16IntSource,
                        en_functional_state_t enNewSta);
void TIMERB_SetMode(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Mode);
uint16_t TIMERB_GetMode(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_SetHwTriggerCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition);
void TIMERB_SetHwStartCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition);
uint16_t TIMERB_GetHwStartCondition(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_SetHwStopCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition);
uint16_t TIMERB_GetHwStopCondition(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_SetHwClearCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition);
uint16_t TIMERB_GetHwClearCondition(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_SetHwUpCondition(M0P_TMRB_TypeDef *TMRBx,
                                    uint16_t u16Condition);
uint16_t TIMERB_GetHwUpCondition(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_SetHwDownCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition);
uint16_t TIMERB_GetHwDownCondition(M0P_TMRB_TypeDef *TMRBx);

/* Initialization and configuration TimerB input capture functions */
en_result_t TIMERB_IC_Init(M0P_TMRB_TypeDef *TMRBx, 
                                const stc_timerb_ic_init_t *pstcIcInit);
en_result_t TIMERB_IC_StructInit(stc_timerb_ic_init_t *pstcIcInit);
en_result_t TIMERB_IC_DeInit(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_IC_SetCaptureCondition(M0P_TMRB_TypeDef *TMRBx,
                                            uint16_t u16Condition);
uint16_t TIMERB_IC_GetCaptureCondition(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_IC_SetFilterClkDiv(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Div);
uint16_t TIMERB_IC_GetFilterClkDiv(M0P_TMRB_TypeDef *TMRBx);

/* Initialization and configuration TimerB output compare functions */
en_result_t TIMERB_OC_Init(M0P_TMRB_TypeDef *TMRBx,
                                const stc_timerb_oc_init_t *pstcOcInit);
en_result_t TIMERB_OC_StructInit(stc_timerb_oc_init_t *pstcOcInit);
en_result_t TIMERB_OC_DeInit(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_OC_SetStartCntOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                    uint16_t u16Polarity);
uint16_t TIMERB_OC_GetStartCntOutputPolarity(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_OC_SetStopCntOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                    uint16_t u16Polarity);
uint16_t TIMERB_OC_GetStopCntOutputPolarity(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_OC_SetCmpMatchOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                    uint16_t u16Polarity);
uint16_t TIMERB_OC_GetCmpMatchOutputPolarity(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_OC_SetPeriodMatchOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                        uint16_t u16Polarity);
uint16_t TIMERB_OC_GetPeriodMatchOutputPolarity(M0P_TMRB_TypeDef *TMRBx);
void TIMERB_OC_SetForceOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                uint16_t u16Polarity);
uint16_t TIMERB_OC_GetForceOutputPolarity(M0P_TMRB_TypeDef *TMRBx);

/**
 * @}
 */

#endif /* DDL_TIMERB_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_TIMERB_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
