/**
 *******************************************************************************
 * @file  hc32m120_timera.h
 * @brief This file contains all the functions prototypes of the TimerA driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-01       Yangjp          First version
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
#ifndef __HC32M120_TIMERA_H__
#define __HC32M120_TIMERA_H__

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
 * @addtogroup DDL_TIMERA
 * @{
 */

#if (DDL_TIMERA_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup TIMERA_Global_Types TimerA Global Types
 * @{
 */

/**
 * @brief TimerA initialization structure definition
 */
typedef struct
{
    uint16_t u16CountMode;              /*!< Specifies the TimerA count mode.
                                             This parameter can be a value of @ref TIMERA_Count_Mode */

    uint16_t u16CountDir;               /*!< Specifies the TimerA count direction.
                                             This parameter can be a value of @ref TIMERA_Count_Direction */

    uint16_t u16ClkDiv;                 /*!< Specifies the TimerA Count clock division.
                                             This parameter can be a value of @ref TIMERA_Clock_Division */

    uint16_t u16PeriodVal;              /*!< Specifies the TimerA period value.
                                             This parameter can be a value of half-word */

    uint16_t u16OverflowAction;         /*!< Specifies the Stop/continue count when TimerA count overflow.
                                             This parameter can be a value of @ref TIMERA_Overflow_Control */

    uint16_t u16HwStartCondition;       /*!< Specifies the TimerA hardware trigger start count condition.
                                             This parameter can be any value of @ref TIMERA_Hardware_Start_Count_Condition */

    uint16_t u16HwStopCondition;        /*!< Specifies the TimerA hardware trigger stop count condition.
                                             This parameter can be any value of @ref TIMERA_Hardware_Stop_Count_Condition */

    uint16_t u16HwClearCondition;       /*!< Specifies the TimerA hardware trigger clear count condition.
                                             This parameter can be any value of @ref TIMERA_Hardware_Clear_Count_Condition */

    uint16_t u16HwUpCondition;          /*!< Specifies the TimerA hardware trigger count up condition.
                                             This parameter can be any value of @ref TIMERA_Hardware_Count_Up_Condition */

    uint16_t u16HwDownCondition;        /*!< Specifies the TimerA hardware trigger count down condition.
                                             This parameter can be any value of @ref TIMERA_Hardware_Count_Down_Condition */

    uint16_t u16TrigFilterState;        /*!< Specifies the TimerA TRIG Port noise filter state.
                                             This parameter can be a value of @ref TIMERA_TRIG_Port_Filter_State */

    uint16_t u16TrigFilterClkDiv;       /*!< Specifies the TimerA TRIG Port noise filter clock division.
                                             This parameter can be a value of @ref TIMERA_TRIG_Port_Filter_Clock_Division */

    uint16_t u16ClkAFilterState;        /*!< Specifies the TimerA CLKA Port noise filter state.
                                             This parameter can be a value of @ref TIMERA_CLKA_Port_Filter_State */

    uint16_t u16ClkAFilterClkDiv;       /*!< Specifies the TimerA CLKA Port noise filter clock division.
                                             This parameter can be a value of @ref TIMERA_CLKA_Port_Filter_Clock_Division */

    uint16_t u16ClkBFilterState;        /*!< Specifies the TimerA CLKB Port noise filter state.
                                             This parameter can be a value of @ref TIMERA_CLKB_Port_Filter_State */

    uint16_t u16ClkBFilterClkDiv;       /*!< Specifies the TimerA CLKB Port noise filter clock division.
                                             This parameter can be a value of @ref TIMERA_CLKB_Port_Filter_Clock_Division */
} stc_timera_init_t;

/**
 * @brief TimerA output compare initialization structure definition
 */
typedef struct
{
    uint16_t u16CompareVal;             /*!< Specifies the TimerA compare value.
                                             This parameter can be a value of half-word */

    uint16_t u16PortOutputState;        /*!< Specifies the TIMA_<t>_PWMn port output state.
                                             This parameter can be a value of @ref TIMERA_OC_Port_Output_State */

    uint16_t u16StartCountOutput;       /*!< Specifies the TIMA_<t>_PWMn output polarity when start count.
                                             This parameter can be a value of @ref TIMERA_OC_Start_Count_Output_Polarity */

    uint16_t u16StopCountOutput;        /*!< Specifies the TIMA_<t>_PWMn output polarity when stop count.
                                             This parameter can be a value of @ref TIMERA_OC_Stop_Count_Output_Polarity */

    uint16_t u16CompareMatchOutput;     /*!< Specifies the TIMA_<t>_PWMn output polarity when compare value match.
                                             This parameter can be a value of @ref TIMERA_OC_Compare_Match_Output_Polarity */

    uint16_t u16PeriodMatchOutput;      /*!< Specifies the TIMA_<t>_PWMn output polarity when period value match.
                                             This parameter can be a value of @ref TIMERA_OC_Period_Match_Output_Polarity */

    uint16_t u16CacheState;             /*!< Specifies the TimerA compare cache state, Only PWMn=PWM1 is valid.
                                             This parameter can be a value of @ref TIMERA_OC_Cache_State */

    uint16_t u16CacheTransmitCondition; /*!< Specifies the TimerA compare cache transmit condition.
                                             This parameter can be any value of @ref TIMERA_OC_Cache_Transmit_Condition */
} stc_timera_oc_init_t;

/**
 * @brief TimerA input capture initialization structure definition
 */
typedef struct
{
    uint16_t u16PwmFilterState;         /*!< Specifies the TimerA PWM port Noise filter state.
                                             This parameter can be a value of @ref TIMERA_IC_PWM_Port_Filter_State */

    uint16_t u16PwmFilterClkDiv;        /*!< Specifies the TimerA PWM port Noise filter clock division.
                                             This parameter can be a value of @ref TIMERA_IC_PWM_Port_Filter_Clock_Division */

    uint16_t u16CaptureCondition;       /*!< Specifies the TimerA input capture condition.
                                             This parameter can be any value of @ref TIMERA_IC_Condition */
} stc_timera_ic_init_t;

/**
 * @brief TimerA channel enumeration
 */
typedef enum
{
    TIMERA_CHANNEL_CH1 = 0x00u,         /*!< TimerA channel 1 */
    TIMERA_CHANNEL_CH2 = 0x01u,         /*!< TimerA channel 2 */
} en_timera_channel_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMERA_Global_Macros TimerA Global Macros
 * @{
 */

/**
 * @defgroup TIMERA_Count_Mode TimerA Count Mode
 * @{
 */
#define TIMERA_TRIANGLE_WAVE                    (TMRA_BCSTR_MODE)
#define TIMERA_SAWTOOTH_WAVE                    ((uint16_t)0x0000u)
/**
 * @}
 */

/**
 * @defgroup TIMERA_Count_Direction TimerA Count Direction
 * @{
 */
#define TIMERA_COUNT_UP                         (TMRA_BCSTR_DIR)
#define TIMERA_COUNT_DOWN                       ((uint16_t)0x0000u)
/**
 * @}
 */

/**
 * @defgroup TIMERA_Clock_Division TimerA Clock Division
 * @{
 */
#define TIMERA_CLKDIV_DIV1                      ((uint16_t)0x0000u)                                              /*!< HCLK      */
#define TIMERA_CLKDIV_DIV2                      (TMRA_BCSTR_CKDIV_0)                                            /*!< HCLK/2    */
#define TIMERA_CLKDIV_DIV4                      (TMRA_BCSTR_CKDIV_1)                                            /*!< HCLK/4    */
#define TIMERA_CLKDIV_DIV8                      (TMRA_BCSTR_CKDIV_1 | TMRA_BCSTR_CKDIV_0)                       /*!< HCLK/8    */
#define TIMERA_CLKDIV_DIV16                     (TMRA_BCSTR_CKDIV_2)                                            /*!< HCLK/16   */
#define TIMERA_CLKDIV_DIV32                     (TMRA_BCSTR_CKDIV_2 | TMRA_BCSTR_CKDIV_0)                       /*!< HCLK/32   */
#define TIMERA_CLKDIV_DIV64                     (TMRA_BCSTR_CKDIV_2 | TMRA_BCSTR_CKDIV_1)                       /*!< HCLK/64   */
#define TIMERA_CLKDIV_DIV128                    (TMRA_BCSTR_CKDIV_2 | TMRA_BCSTR_CKDIV_1 | TMRA_BCSTR_CKDIV_0)  /*!< HCLK/128  */
#define TIMERA_CLKDIV_DIV256                    (TMRA_BCSTR_CKDIV_3)                                            /*!< HCLK/256  */
#define TIMERA_CLKDIV_DIV512                    (TMRA_BCSTR_CKDIV_3 | TMRA_BCSTR_CKDIV_0)                       /*!< HCLK/512  */
#define TIMERA_CLKDIV_DIV1024                   (TMRA_BCSTR_CKDIV_3 | TMRA_BCSTR_CKDIV_1)                       /*!< HCLK/1024 */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Overflow_Control TimerA Overflow Control
 * @{
 */
#define TIMERA_OVERFLOW_STOP                    (TMRA_BCSTR_OVSTP)
#define TIMERA_OVERFLOW_COUNT                   ((uint16_t)0x0000u)
/**
 * @}
 */

/**
 * @defgroup TIMERA_TRIG_Port_Filter_State TimerA TRIG Port Noise Filter State
 * @note TIMA_<t>_TRIG t is unit number,value is 1.
 * @{
 */
#define TIMERA_TRIG_FILTER_ENABLE               (TMRA_FCONR_NOFIENTG)   /*!< Enable TIMA_<t>_TRIG Port noise filter function  */
#define TIMERA_TRIG_FILTER_DISABLE              ((uint16_t)0x0000u)     /*!< Disable TIMA_<t>_TRIG Port noise filter function */
/**
 * @}
 */

/**
 * @defgroup TIMERA_TRIG_Port_Filter_Clock_Division TimerA TRIG Port Noise Filter Clock Division
 * @{
 */
#define TIMERA_TRIG_CLKDIV_DIV1                 ((uint16_t)0x0000u)     /*!< HCLK */
#define TIMERA_TRIG_CLKDIV_DIV4                 (TMRA_FCONR_NOFICKTG_0) /*!< HCLK/4 */
#define TIMERA_TRIG_CLKDIV_DIV16                (TMRA_FCONR_NOFICKTG_1) /*!< HCLK/16 */
#define TIMERA_TRIG_CLKDIV_DIV64                (TMRA_FCONR_NOFICKTG)   /*!< HCLK/64 */
/**
 * @}
 */

/**
 * @defgroup TIMERA_CLKA_Port_Filter_State TimerA CLKA Port Noise Filter State
 * @note TIMA_<t>_CLKA t is unit number,value is 1.
 * @{
 */
#define TIMERA_CLKA_FILTER_ENABLE               (TMRA_FCONR_NOFIENCA)   /*!< Enable TIMA_<t>_CLKA Port noise filter function */
#define TIMERA_CLKA_FILTER_DISABLE              ((uint16_t)0x0000u)     /*!< Disable TIMA_<t>_CLKA Port noise filter function */
/**
 * @}
 */

/**
 * @defgroup TIMERA_CLKA_Port_Filter_Clock_Division TimerA CLKA Port Noise Filter Clock Division
 * @{
 */
#define TIMERA_CLKA_CLKDIV_DIV1                 ((uint16_t)0x0000u)     /*!< HCLK */
#define TIMERA_CLKA_CLKDIV_DIV4                 (TMRA_FCONR_NOFICKCA_0) /*!< HCLK/4 */
#define TIMERA_CLKA_CLKDIV_DIV16                (TMRA_FCONR_NOFICKCA_1) /*!< HCLK/16 */
#define TIMERA_CLKA_CLKDIV_DIV64                (TMRA_FCONR_NOFICKCA)   /*!< HCLK/64 */
/**
 * @}
 */

/**
 * @defgroup TIMERA_CLKB_Port_Filter_State TimerA CLKB Port Noise Filter State
 * @note TIMA_<t>_CLKB t is unit number,value is 1.
 * @{
 */
#define TIMERA_CLKB_FILTER_ENABLE               (TMRA_FCONR_NOFIENCB)   /*!< Enable TIMA_<t>_CLKB Port noise filter function */
#define TIMERA_CLKB_FILTER_DISABLE              ((uint16_t)0x0000u)     /*!< Disable TIMA_<t>_CLKB Port noise filter function */
/**
 * @}
 */

/**
 * @defgroup TIMERA_CLKB_Port_Filter_Clock_Division TimerA CLKB Port Noise Filter Clock Division
 * @{
 */
#define TIMERA_CLKB_CLKDIV_DIV1                 ((uint16_t)0x0000u)     /*!< HCLK */
#define TIMERA_CLKB_CLKDIV_DIV4                 (TMRA_FCONR_NOFICKCB_0) /*!< HCLK/4 */
#define TIMERA_CLKB_CLKDIV_DIV16                (TMRA_FCONR_NOFICKCB_1) /*!< HCLK/16 */
#define TIMERA_CLKB_CLKDIV_DIV64                (TMRA_FCONR_NOFICKCB)   /*!< HCLK/64 */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Flag TIMERA Flag
 * @{
 */
#define TIMERA_FLAG_OVF                         (TMRA_BCSTR_OVFF)
#define TIMERA_FLAG_UDF                         (TMRA_BCSTR_UDFF)
#define TIMERA_FLAG_CMP1                        (TMRA_STFLR_CMPF1)
#define TIMERA_FLAG_CMP2                        (TMRA_STFLR_CMPF2)
/**
 * @}
 */

/**
 * @defgroup TIMERA_Interrupt TIMERA Interrupts
 * @{
 */
#define TIMERA_INT_OVF                          (TMRA_BCSTR_INENOVF)
#define TIMERA_INT_UDF                          (TMRA_BCSTR_INENUDF)
#define TIMERA_INT_CMP1                         (TMRA_ICONR_ITEN1)
#define TIMERA_INT_CMP2                         (TMRA_ICONR_ITEN2)
/**
 * @}
 */

/**
 * @defgroup TIMERA_Event TimerA Event
 * @{
 */
#define TIMERA_EVENT_CMP1                       (TMRA_ECONR_ETEN1)
#define TIMERA_EVENT_CMP2                       (TMRA_ECONR_ETEN2)
/**
 * @}
 */

/**
 * @defgroup TIMERA_Function_Mode TimerA Function Mode
 * @{
 */
#define TIMERA_INPUT_CAPTURE                    (TMRA_CCONR1_CAPMD)     /*!< Capture input function */
#define TIMERA_OUTPUT_COMPARE                   ((uint16_t)0x0000u)     /*!< Compare output function */
/**
 * @}
 */

/**
 * @defgroup TIMERA_IC_Condition TimerA Input Capture Condition
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_IC_PWM_RISING                    (TMRA_CCONR1_HICP0)     /*!< Capture triggered by rising edge on timer input TIMA_<t>_PWMn */
#define TIMERA_IC_PWM_FALLING                   (TMRA_CCONR1_HICP1)     /*!< Capture triggered by falling edge on timer input TIMA_<t>_PWMn */
#define TIMERA_IC_SPECIFY_EVT                   (TMRA_CCONR1_HICP2)     /*!< Capture triggered by the timer TMRA_HTSSR specified event */
#define TIMERA_IC_INVALID                       ((uint16_t)0x0000u)     /*!< Don't occur capture action */
/**
 * @}
 */

/**
 * @defgroup TIMERA_IC_PWM_Port_Filter_State TimerA Input Capture PWM Port Filter State
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_IC_PWM_FILTER_ENABLE             (TMRA_CCONR1_NOFIENCP)  /*!< Enable TIMA_<t>_PWMn input capture noise filter function */
#define TIMERA_IC_PWM_FILTER_DISABLE            ((uint16_t)0x0000u)     /*!< Disable TIMA_<t>_PWMn input capture noise filter function */
/**
 * @}
 */

/**
 * @defgroup TIMERA_IC_PWM_Port_Filter_Clock_Division TimerA Input Capture PWM Port Filter Clock Division
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_IC_PWM_CLKDIV_DIV1               ((uint16_t)0x0000u)         /*!< HCLK */
#define TIMERA_IC_PWM_CLKDIV_DIV4               (TMRA_CCONR1_NOFICKCP_0)    /*!< HCLK/4 */
#define TIMERA_IC_PWM_CLKDIV_DIV16              (TMRA_CCONR1_NOFICKCP_1)    /*!< HCLK/16 */
#define TIMERA_IC_PWM_CLKDIV_DIV64              (TMRA_CCONR1_NOFICKCP)      /*!< HCLK/64 */
/**
 * @}
 */

/**
 * @defgroup TIMERA_OC_Start_Count_Output_Polarity TimerA Output Compare Start Count Output Polarity
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_OC_STARTCOUNT_OUTPUT_LOW         ((uint16_t)0x0000u)     /*!< TIM_<t>_PWMn output low level when TimerA start count */
#define TIMERA_OC_STARTCOUNT_OUTPUT_HIGH        (TMRA_PCONR1_STAC_0)    /*!< TIM_<t>_PWMn output high level when TimerA start count */
#define TIMERA_OC_STARTCOUNT_OUTPUT_HOLD        (TMRA_PCONR1_STAC_1)    /*!< TIM_<t>_PWMn output hold level when TimerA start count */
/**
 * @}
 */

/**
 * @defgroup TIMERA_OC_Stop_Count_Output_Polarity TimerA Output Compare Stop Count Output Polarity
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_OC_STOPCOUNT_OUTPUT_LOW          ((uint16_t)0x0000u)     /*!< TIM_<t>_PWMn output low level when TimerA stop count */
#define TIMERA_OC_STOPCOUNT_OUTPUT_HIGH         (TMRA_PCONR1_STPC_0)    /*!< TIM_<t>_PWMn output high level when TimerA stop count */
#define TIMERA_OC_STOPCOUNT_OUTPUT_HOLD         (TMRA_PCONR1_STPC_1)    /*!< TIM_<t>_PWMn output hold level when TimerA stop count */
/**
 * @}
 */

/**
 * @defgroup TIMERA_OC_Compare_Match_Output_Polarity TimerA Output Compare Match Output Polarity
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_OC_CMPMATCH_OUTPUT_LOW           ((uint16_t)0x0000u)     /*!< TIM_<t>_PWMn output low level when TimerA compare match */
#define TIMERA_OC_CMPMATCH_OUTPUT_HIGH          (TMRA_PCONR1_CMPC_0)    /*!< TIM_<t>_PWMn output high level when TimerA compare match */
#define TIMERA_OC_CMPMATCH_OUTPUT_HOLD          (TMRA_PCONR1_CMPC_1)    /*!< TIM_<t>_PWMn output hold level when TimerA compare match */
#define TIMERA_OC_CMPMATCH_OUTPUT_INVERTED      (TMRA_PCONR1_CMPC)      /*!< TIM_<t>_PWMn output inverted level when TimerA compare match */
/**
 * @}
 */

/**
 * @defgroup TIMERA_OC_Period_Match_Output_Polarity TimerA Output Compare Period Match Output Polarity
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_OC_PERIODMATCH_OUTPUT_LOW        ((uint16_t)0x0000u)     /*!< TIM_<t>_PWMn output low level when TimerA period match */
#define TIMERA_OC_PERIODMATCH_OUTPUT_HIGH       (TMRA_PCONR1_PERC_0)    /*!< TIM_<t>_PWMn output high level when TimerA period match */
#define TIMERA_OC_PERIODMATCH_OUTPUT_HOLD       (TMRA_PCONR1_PERC_1)    /*!< TIM_<t>_PWMn output hold level when TimerA period match */
#define TIMERA_OC_PERIODMATCH_OUTPUT_INVERTED   (TMRA_PCONR1_PERC)      /*!< TIM_<t>_PWMn output inverted level when TimerA period match */
/**
 * @}
 */

/**
 * @defgroup TIMERA_OC_Port_Output_State TimerA Output Compare Force Output Polarity
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_OC_FORCE_OUTPUT_INVALID          ((uint16_t)0x0000u)     /*!< Invalid */
#define TIMERA_OC_FORCE_OUTPUT_LOW              (TMRA_PCONR1_FORC_1)    /*!< Force TIM_<t>_PWMn force output low level */
#define TIMERA_OC_FORCE_OUTPUT_HIGH             (TMRA_PCONR1_FORC)      /*!< Force TIM_<t>_PWMn force output high level */
/**
 * @}
 */

/**
 * @defgroup TIMERA_OC_Cache_State TimerA Output Compare Cache State
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and Only n=1 is valid.
 * @{
 */
#define TIMERA_OC_CACHE_ENABLE                  (TMRA_BCONR_BEN)        /*!< Enable Output Compare Cache function */
#define TIMERA_OC_CACHE_DISABLE                 ((uint16_t)0x0000u)     /*!< Disable Output Compare Cache function */
/**
 * @}
 */

/**
 * @defgroup TIMERA_OC_Cache_Transmit_Condition TimerA Compare Cache Transmit Condition
 * @note TIMA_<t>_PWM<n> t is unit number,value is 1 and n is channel number of unit,range is 1-2.
 * @{
 */
#define TIMERA_OC_CACHE_TRANSMIT_CREST          (TMRA_BCONR_BSE0)       /*!< In Triangular wave crest transmit cache value */
#define TIMERA_OC_CACHE_TRANSMIT_TROUGH         (TMRA_BCONR_BSE1)       /*!< In Triangular wave trough transmit cache value */
#define TIMERA_OC_CACHE_TRANSMIT_INVALID        ((uint16_t)0x0000u)     /*!< Invalid cache transmit */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Hardware_Start_Count_Condition TimerA Hardware Start Count Condition
 * @note TIMA_<t>_TRIG t is unit number,value is 1.
 * @{
 */
#define TIMERA_HWSTART_INVALID                  ((uint16_t)0x0000u)     /*!< Hardware start is invalid */
#define TIMERA_HWSTART_TRIG_RISING              (TMRA_HCONR_HSTA0)      /*!< Hardware start by rising edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWSTART_TRIG_FALLING             (TMRA_HCONR_HSTA1)      /*!< Hardware start by falling edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWSTART_SPECIFY_EVT              (TMRA_HCONR_HSTA2)      /*!< Hardware start by the timer TMRA_HTSSR specified event */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Hardware_Stop_Count_Condition TimerA Hardware Stop Count Condition
 * @note TIMA_<t>_TRIG t is unit number,value is 1.
 * @{
 */
#define TIMERA_HWSTOP_INVALID                   ((uint16_t)0x0000u)     /*!< Hardware stop is invalid */
#define TIMERA_HWSTOP_TRIG_RISING               (TMRA_HCONR_HSTP0)      /*!< Hardware stop by rising edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWSTOP_TRIG_FALLING              (TMRA_HCONR_HSTP1)      /*!< Hardware stop by falling edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWSTOP_SPECIFY_EVT               (TMRA_HCONR_HSTP2)      /*!< Hardware stop by the timer TMRA_HTSSR specified event */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Hardware_Clear_Count_Condition TimerA Hardware Clear Count Condition
 * @note TIMA_<t>_TRIG t is unit number,value is 1.
 * @{
 */
#define TIMERA_HWCLEAR_INVALID                  ((uint16_t)0x0000u)     /*!< Hardware clear is invalid */
#define TIMERA_HWCLEAR_TRIG_RISING              (TMRA_HCONR_HCLE0)      /*!< Hardware clear count by rising edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWCLEAR_TRIG_FALLING             (TMRA_HCONR_HCLE1)      /*!< Hardware clear count by falling edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWCLEAR_SPECIFY_EVT              (TMRA_HCONR_HCLE2)      /*!< Hardware clear count by the timer TMRA_HTSSR specified event */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Hardware_Count_Up_Condition TimerA Hardware Count Up Condition
 * @note TIMA_<t>_TRIG or TIMA_<t>_CLKA or TIMA_<t>_CLKB t is unit number,value is 1.
 * @{
 */
#define TIMERA_HWUP_INVALID                     ((uint16_t)0x0000u)     /*!< Hardware count up is invalid */
#define TIMERA_HWUP_CLKA_LOW_CLKB_RISING        (TMRA_HCUPR_HCUP0)      /*!< Hardware count up by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level   */
#define TIMERA_HWUP_CLKA_LOW_CLKB_FALLING       (TMRA_HCUPR_HCUP1)      /*!< Hardware count up by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level  */
#define TIMERA_HWUP_CLKA_HIGH_CLKB_RISING       (TMRA_HCUPR_HCUP2)      /*!< Hardware count up by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level  */
#define TIMERA_HWUP_CLKA_HIGH_CLKB_FALLING      (TMRA_HCUPR_HCUP3)      /*!< Hardware count up by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level */
#define TIMERA_HWUP_CLKB_LOW_CLKA_RISING        (TMRA_HCUPR_HCUP4)      /*!< Hardware count up by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level   */
#define TIMERA_HWUP_CLKB_LOW_CLKA_FALLING       (TMRA_HCUPR_HCUP5)      /*!< Hardware count up by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level  */
#define TIMERA_HWUP_CLKB_HIGH_CLKA_RISING       (TMRA_HCUPR_HCUP6)      /*!< Hardware count up by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level  */
#define TIMERA_HWUP_CLKB_HIGH_CLKA_FALLING      (TMRA_HCUPR_HCUP7)      /*!< Hardware count up by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level */
#define TIMERA_HWUP_TRIG_RISING                 (TMRA_HCUPR_HCUP8)      /*!< Hardware count up by rising edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWUP_TRIG_FALLING                (TMRA_HCUPR_HCUP9)      /*!< Hardware count up by falling edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWUP_SPECIFY_EVT                 (TMRA_HCUPR_HCUP10)     /*!< Hardware count up by the timer TMRA_HTSSR specified event */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Hardware_Count_Down_Condition TimerA Hardware Count Down Condition
 * @note TIMA_<t>_TRIG or TIMA_<t>_CLKA or TIMA_<t>_CLKB t is unit number,value is 1.
 * @{
 */
#define TIMERA_HWDOWN_INVALID                   ((uint16_t)0x0000u)     /*!< Hardware count down is invalid */
#define TIMERA_HWDOWN_CLKA_LOW_CLKB_RISING      (TMRA_HCDOR_HCDO0)      /*!< Hardware count down by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level   */
#define TIMERA_HWDOWN_CLKA_LOW_CLKB_FALLING     (TMRA_HCDOR_HCDO1)      /*!< Hardware count down by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level  */
#define TIMERA_HWDOWN_CLKA_HIGH_CLKB_RISING     (TMRA_HCDOR_HCDO2)      /*!< Hardware count down by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level  */
#define TIMERA_HWDOWN_CLKA_HIGH_CLKB_FALLING    (TMRA_HCDOR_HCDO3)      /*!< Hardware count down by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level */
#define TIMERA_HWDOWN_CLKB_LOW_CLKA_RISING      (TMRA_HCDOR_HCDO4)      /*!< Hardware count down by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level   */
#define TIMERA_HWDOWN_CLKB_LOW_CLKA_FALLING     (TMRA_HCDOR_HCDO5)      /*!< Hardware count down by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level  */
#define TIMERA_HWDOWN_CLKB_HIGH_CLKA_RISING     (TMRA_HCDOR_HCDO6)      /*!< Hardware count down by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level  */
#define TIMERA_HWDOWN_CLKB_HIGH_CLKA_FALLING    (TMRA_HCDOR_HCDO7)      /*!< Hardware count down by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level */
#define TIMERA_HWDOWN_TRIG_RISING               (TMRA_HCDOR_HCDO8)      /*!< Hardware count down by rising edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWDOWN_TRIG_FALLING              (TMRA_HCDOR_HCDO9)      /*!< Hardware count down by falling edge on timer input TIMA_<t>_TRIG */
#define TIMERA_HWDOWN_SPECIFY_EVT               (TMRA_HCDOR_HCDO10)     /*!< Hardware count down by the timer TMRA_HTSSR specified event */
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
 * @addtogroup TIMERA_Global_Functions
 * @{
 */

/**
 * @brief  Set trigger event source.
 * @param  [in] enEvtSrc                Trigger event source @ref en_event_src_t
 * @retval None
 */
__STATIC_INLINE void TIMERA_SetTriggerSrc(en_event_src_t enEvtSrc)
{
    WRITE_REG(M0P_AOS->TMRA_HTSSR, enEvtSrc);
}

/* Initialization and configuration TimerA base functions */
en_result_t TIMERA_DeInit(M0P_TMRA_TypeDef *TMRAx);
en_result_t TIMERA_Init(M0P_TMRA_TypeDef *TMRAx,
                        const stc_timera_init_t *pstcInit);
en_result_t TIMERA_StructInit(stc_timera_init_t *pstcInit);
void TIMERA_SetCounter(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Value);
uint16_t TIMERA_GetCounter(M0P_TMRA_TypeDef *TMRAx);
void TIMERA_SetPeriod(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Value);
uint16_t TIMERA_GetPeriod(M0P_TMRA_TypeDef *TMRAx);
void TIMERA_SetClkDiv(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Div);
void TIMERA_EventCmd(M0P_TMRA_TypeDef *TMRAx, uint16_t u16EvtSource,
                     en_functional_state_t enNewSta);
void TIMERA_Cmd(M0P_TMRA_TypeDef *TMRAx, en_functional_state_t enNewSta);
void TIMERA_SetTrigFilterClkDiv(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Div);
void TIMERA_TrigFilterCmd(M0P_TMRA_TypeDef *TMRAx, en_functional_state_t enNewSta);
void TIMERA_SetClkAFilterClkDiv(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Div);
void TIMERA_ClkAFilterCmd(M0P_TMRA_TypeDef *TMRAx, en_functional_state_t enNewSta);
void TIMERA_SetClkBFilterClkDiv(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Div);
void TIMERA_ClkBFilterCmd(M0P_TMRA_TypeDef *TMRAx, en_functional_state_t enNewSta);

/* Configuration TimerA Hardware trigger functions */
void TIMERA_SetHwTriggerCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition);
uint16_t TIMERA_GetHwTriggerCondition(M0P_TMRA_TypeDef *TMRAx);
void TIMERA_SetHwStartCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition);
void TIMERA_SetHwStopCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition);
void TIMERA_SetHwClearCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition);
void TIMERA_SetHwUpCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition);
uint16_t TIMERA_GetHwUpCondition(M0P_TMRA_TypeDef *TMRAx);
void TIMERA_SetHwDownCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition);
uint16_t TIMERA_GetHwDownCondition(M0P_TMRA_TypeDef *TMRAx);

/* Initialization and configuration TimerA input capture functions */
en_result_t TIMERA_IC_DeInit(M0P_TMRA_TypeDef *TMRAx,
                             uint8_t u8Channel);
en_result_t TIMERA_IC_Init(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                           const stc_timera_ic_init_t *pstcIcInit);
en_result_t TIMERA_IC_StructInit(stc_timera_ic_init_t *pstcIcInit);
uint16_t TIMERA_IC_GetCapture(M0P_TMRA_TypeDef *TMRAx,
                              uint8_t u8Channel);
void TIMERA_IC_SetCaptureCondition(M0P_TMRA_TypeDef *TMRAx,
                                   uint8_t u8Channel,
                                   uint16_t u16Condition);
void TIMERA_IC_FilterCmd(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                         en_functional_state_t enNewSta);
void TIMERA_IC_SetFilterClkDiv(M0P_TMRA_TypeDef *TMRAx,
                               uint8_t u8Channel, uint16_t u16Div);

/* Initialization and configuration TimerA output compare functions */
en_result_t TIMERA_OC_DeInit(M0P_TMRA_TypeDef *TMRAx,
                             uint8_t u8Channel);
en_result_t TIMERA_OC_Init(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                           const stc_timera_oc_init_t *pstcOcInit);
en_result_t TIMERA_OC_StructInit(stc_timera_oc_init_t *pstcOcInit);
void TIMERA_OC_SetCompare(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                          uint16_t u16Value);
void TIMERA_OC_PwmCmd(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                      en_functional_state_t enNewSta);
void TIMERA_OC_SetStartCountOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                           uint8_t u8Channel,
                                           uint16_t u16Polarity);
void TIMERA_OC_SetStopCountOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                          uint8_t u8Channel,
                                          uint16_t u16Polarity);
void TIMERA_OC_SetCmpMatchOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                         uint8_t u8Channel,
                                         uint16_t u16Polarity);
void TIMERA_OC_SetPeriodMatchOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                            uint8_t u8Channel,
                                            uint16_t u16Polarity);
void TIMERA_OC_SetForceOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                      uint8_t u8Channel,
                                      uint16_t u16Polarity);
en_result_t TIMERA_OC_CacheCmd(M0P_TMRA_TypeDef *TMRAx,
                               uint8_t u8Channel,
                               en_functional_state_t enNewSta);
en_result_t TIMERA_OC_SetCacheTransmitCondition(M0P_TMRA_TypeDef *TMRAx,
                                                uint8_t u8Channel,
                                                uint16_t u16Condition);

/* Interrupts and flags management functions */
en_flag_status_t TIMERA_GetFlag(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Flag);
void TIMERA_ClearFlag(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Flag);
void TIMERA_IntCmd(M0P_TMRA_TypeDef *TMRAx, uint16_t u16IntSource,
                   en_functional_state_t enNewSta);

/**
 * @}
 */

#endif /* DDL_TIMERA_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_TIMERA_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
