/**
 *******************************************************************************
 * @file  hc32m120_timer4.h
 * @brief Head file for TIMER4 module.
 *
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-24       Hongjh          First version
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
#ifndef __HC32M120_TIMER4_H__
#define __HC32M120_TIMER4_H__

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
 * @addtogroup DDL_TIMER4
 * @{
 */

#if (DDL_TIMER4_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup TIMER4_Global_Types TIMER4 Global Types
 * @{
 */

/**
 * @brief Timer4 Counter(CNT) function initialization configuration
 */
typedef struct
{
    uint16_t u16CycleVal;       /*!< Timer4 count cycle value.
                                     This parameter can be a value of half-word */

    uint16_t u16CntMode;        /*!< Timer4 count mode.
                                     This parameter can be a value of @ref TIMER4_CNT_Mode */

    uint16_t u16ClkSrc;         /*!< Timer4 count clock source.
                                     This parameter can be a value of @ref TIMER4_CNT_Clock_Source */

    uint16_t u16ClkDiv;         /*!< Timer4 count clock division.
                                     This parameter can be a value of @ref TIMER4_CNT_Clock_Division */

    uint16_t u16ZeroIntMask;    /*!< Timer4 count zero interrupt mask times.
                                     This parameter can be a value of @ref TIMER4_CNT_Interrupt_Mask_Times */

    uint16_t u16PeakIntMask;    /*!< Timer4 count peak interrupt mask times.
                                     This parameter can be a value of @ref TIMER4_CNT_Interrupt_Mask_Times */

    en_functional_state_t enBufferCmd;  /*!< Timer4 count buffer state.
                                             This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enZeroIntCmd; /*!< Enable or disable zero match interrupt
                                             This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enPeakIntCmd; /*!< Enable or disable peak match interrupt
                                             This parameter can be a value of @ref en_functional_state_t */
} stc_timer4_cnt_init_t;

/**
 * @brief The configuration of OCO high channel OCMR field (OUH/OVH/OWH)
 */
typedef struct
{
    uint16_t OCFDCH : 1;    /*!< OCMRxh b0 High channel's OCF status when high channel match occurs at the condition of CNT is counting down
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_OCF_State */

    uint16_t OCFPKH : 1;    /*!< OCMRxh b1 High channel's OCF status when high channel match occurs at the condition of CNT count=Peak
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_OCF_State */

    uint16_t OCFUCH : 1;    /*!< OCMRxh b2 High channel's OCF status when high channel match occurs at the condition of CNT is counting up
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_OCF_State */

    uint16_t OCFZRH : 1;    /*!< OCMRxh b3 High channel's OCF status when high channel match occurs at the condition of CNT count=0x0000
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_OCF_State */

    uint16_t OPDCH  : 2;    /*!< OCMRxh b5~b4 High channel's OP output status when high channel match occurs at the condition of CNT is counting down
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint16_t OPPKH  : 2;    /*!< OCMRxh b7~b6 High channel's OP output status when high channel match occurs at the condition of CNT count=Peak
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint16_t OPUCH  : 2;    /*!< OCMRxh b9~b8 High channel's OP output status when high channel match occurs at the condition of CNT is counting up
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint16_t OPZRH  : 2;    /*!< OCMRxh b11~b10 High channel's OP output status when high channel match occurs at the condition of CNT count=0x0000
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint16_t OPNPKH : 2;    /*!< OCMRxh b13~b12 High channel's OP output status when high channel match doesn't occur at the condition of CNT count=Peak
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint16_t OPNZRH : 2;    /*!< OCMRxh b15~b14 High channel's OP output status when high channel match doesn't occur at the condition of CNT count=0x0000
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */
} stc_timer4_oco_ocmrh_field_t;

/**
 * @brief The configuration of OCO high channel(OUH/OVH/OWH)
 */
typedef struct
{
    union
    {
        uint16_t OCMRx;

        stc_timer4_oco_ocmrh_field_t OCMRx_f;
    };

    en_functional_state_t enExtendMatchCondCmd; /*!< Extend the match condition
                                                     This parameter can be a value of @ref en_functional_state_t */
} stc_oco_high_ch_compare_mode_t;

/**
 * @brief The configuration of OCO low channel OCMR field (OUL/OVL/OWL)
 */
typedef struct
{
    uint32_t OCFDCL  : 1;   /*!< OCMRxl b0 Low channel's OCF status when low channel match occurs at the condition of CNT is counting down
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_OCF_State */

    uint32_t OCFPKL  : 1;   /*!< OCMRxl b1 Low channel's OCF status when low channel match occurs at the condition of CNT count=Peak
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_OCF_State */

    uint32_t OCFUCL  : 1;   /*!< OCMRxl b2 Low channel's OCF status when low channel match occurs at the condition of CNT is counting up
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_OCF_State */

    uint32_t OCFZRL  : 1;   /*!< OCMRxl b3 Low channel's OCF status when low channel match occurs at the condition of CNT count=0x0000
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_OCF_State */

    uint32_t OPDCL   : 2;   /*!< OCMRxl b5~b4 Low channel's OP output status when high channel not match and low channel match occurs at the condition of CNT is counting down
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t OPPKL   : 2;   /*!< OCMRxl b7~b6 Low channel's OP output status when high channel not match and low channel match occurs at the condition of CNT count=Peak
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t OPUCL   : 2;   /*!< OCMRxl b9~b8 Low channel's OP output status when high channel not match and low channel match occurs at the condition of CNT is counting up
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t OPZRL   : 2;   /*!< OCMRxl b11~b10 Low channel's OP output status when high channel not match and low channel match occurs at the condition of CNT count=0x0000
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t OPNPKL  : 2;   /*!< OCMRxl b13~b12 Low channel's OP output status when high channel not match and low channel not match occurs at the condition of CNT count=Peak
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t OPNZRL  : 2;   /*!< OCMRxl b15~b14 Low channel's OP output status when high channel not match and low channel not match occurs at the condition of CNT count=0x0000
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t EOPNDCL : 2;   /*!< OCMRxl b17~b16 Low channel's OP output status when high channel match and low channel not match occurs at the condition of CNT is coutning down
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t EOPNUCL : 2;   /*!< OCMRxl b19~b18 Low channel's OP output status when high channel match and low channel not match occurs at the condition of CNT is counting up
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t EOPDCL  : 2;   /*!< OCMRxl b21~b20 Low channel's OP output status when high channel and low channel match occurs at the condition of CNT is counting down
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t EOPPKL  : 2;   /*!< OCMRxl b23~b22 Low channel's OP output status when high channel and low channel match occurs at the condition of CNT count=Peak
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t EOPUCL  : 2;   /*!< OCMRxl b25~b24 Low channel's OP output status when high channel and low channel match occurs at the condition of CNT is counting up
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t EOPZRL  : 2;   /*!< OCMRxl b27~b26 Low channel's OP output status when high channel and low channel match occurs at the condition of CNT count=0x0000
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t EOPNPKL : 2;   /*!< OCMRxl b29~b28 Low channel's OP output status when high channel match and low channel not match occurs at the condition of CNT count=Peak
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */

    uint32_t EOPNZRL : 2;   /*!< OCMRxl b31~b30 Low channel's OP output status when high channel match and low channel not match occurs at the condition of CNT count=0x0000
                                 This parameter can be a value of @ref TIMER4_OCO_Cnt_Match_Output_Polarity */
} stc_timer4_oco_ocmrl_field_t;

/**
 ** \brief The configuration of OCO low channel(OUL/OVL/OWL)
 */
typedef struct
{
    union
    {
        uint32_t OCMRx;

        stc_timer4_oco_ocmrl_field_t OCMRx_f;
    };

    en_functional_state_t enExtendMatchCondCmd; /*!< Extend the match condition
                                                     This parameter can be a value of @ref en_functional_state_t */
} stc_oco_low_ch_compare_mode_t;

/**
 * @brief Timer4 Output Compare(OCO) initialization configuration
 */
typedef struct
{
    uint16_t u16OccrVal;        /*!< Timer4 OCO compare match value.
                                     This parameter can be a value of half-word */

    uint16_t u16OccrBufMode;                    /*!< Buffer register function of OCCR
                                                     This parameter can be a value of @ref TIMER4_OCO_OCCR_Buffer_Mode */

    en_functional_state_t enOccrLinkTransfer;   /*!< Enable or disable the OCCR buffer linked transfer with the CNT interrupt mask counter
                                                     This parameter can be a value of @ref en_functional_state_t */

    uint16_t u16OcmrBufMode;                    /*!< Buffer register function of OCMR
                                                     This parameter can be a value of @ref TIMER4_OCO_OCMR_Buffer_Mode */

    en_functional_state_t enOcmrLinkTransfer;   /*!< Enable or disable the OCMR buffer linked transfer with the CNT interrupt mask counter
                                                     This parameter can be a value of @ref en_functional_state_t */

    uint16_t u16OcoInvalidOp;                   /*!< Port output polarity when OCO is invalid
                                                     This parameter can be a value of @ref TIMER4_OCO_Invalid_Port_Output_Polarity */

    en_functional_state_t enOcoIntCmd;          /*!< Enable or disable OCO interrupt
                                                     This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enOcoCmd;             /*!< Enable or disable OCO function
                                                     This parameter can be a value of @ref en_functional_state_t */
} stc_timer4_oco_init_t;

/**
 * @brief Timer4 PWM initialization configuration
 */
typedef struct
{
    uint16_t u16Mode;                       /*!< Select PWM mode
                                                 This parameter can be a value of @ref TIMER4_PWM_Mode */

    uint16_t u16ClkDiv;                     /*!< Clock division of PWM timer
                                                 This parameter can be a value of @ref TIMER4_PWM_Clock_Division */

    uint16_t u16PwmOutputPolarity;          /*!< TIM4_<t>_OxH and TIM4_<t>_OxL output polarity
                                                 This parameter can be a value of @ref TIMER4_PWM_Output_Polarity */

    en_functional_state_t enRtIntMaskCmd;   /*!< Enable or disable mask PWM reload-timer interrupt
                                                 This parameter can be a value of @ref en_functional_state_t */
} stc_timer4_pwm_init_t;

/**
 * @brief Timer4 Special-EVT trigger condition
 */
typedef struct
{
    en_functional_state_t enDownMatchCmd;   /*!< Enable or disable trigger signal when match with SCCR&SCMR and Timer4 CNT count down
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enPeakMatchCmd;   /*!< Enable or disable trigger signal when match with SCCR&SCMR and Timer4 CNT count peak
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enUpMatchCmd;     /*!< Enable or disable trigger signal when match with SCCR&SCMR and Timer4 CNT count up
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enZeroMatchCmd;   /*!< Enable or disable perform to compare with the CNT zero interrupt mask counter
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enCmpAmcZicCmd;   /*!< Enable or disable perform to compare with the CNT zero interrupt mask counter
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enCmpAmcPicCmd;   /*!< Enable or disable perform to compare with the CNT peak interrupt mask counter
                                                 This parameter can be a value of @ref en_functional_state_t */

    uint16_t              u16MaskTimes;     /*!< Specifies start time by the value to be compared with the CNT interrupt mask counter */
} stc_timer4_sevt_trigger_cond_t;

/**
 * @brief Timer4 Special-EVT initialization configuration
 */
typedef struct
{
    uint16_t u16SccrVal;                    /*!< Timer4 Special-EVT compare match value.
                                                 This parameter can be a value of half-word */

    uint16_t u16Mode;                       /*!< Configure Timer4 Special-EVT Running mode
                                                 This parameter can be a value of @ref TIMER4_SEVT_Mode */

    uint16_t u16TrigEvt;                    /*!< Timer4 Special-EVT trigger signal event
                                                 This parameter can be a value of @ref TIMER4_SEVT_Trigger_Event */

    uint16_t u16BufMode;                    /*!< Buffer mode
                                                 This parameter can be a value of @ref TIMER4_SEVT_Buffer_Mode */

    en_functional_state_t enLinkTransfer;   /*!< Enable or disable the buffer linked transfer with the CNT interrupt mask counter
                                                 This parameter can be a value of @ref en_functional_state_t */

    stc_timer4_sevt_trigger_cond_t  stcTriggerCond; /*!< Timer4 Special-EVT trigger event when match trigger condition 
                                                         This structure details refer @ref stc_timer4_sevt_trigger_cond_t */
} stc_timer4_sevt_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMER4_Global_Macros TIMER4 Global Macros
 * @{
 */

/**
 * @defgroup TIMER4_Result_Code TIMER4 Result Code
 * @{
 */
#define TIMER4_RESULT_ERROR                     ((uint16_t)0xFFFFu)
/**
 * @}
 */

/**
 * @defgroup TIMER4_CNT_Macros TIMER4 Counter Macros
 * @{
 */

/** @defgroup TIMER4_CNT_Clock_Division TIMER4 Count Clock Division
 * @{
 */
#define TIMER4_CNT_CLK_DIV1                     ((uint16_t)0x0000u)         /*!< PCLK      */
#define TIMER4_CNT_CLK_DIV2                     (TMR4_CCSR_CKDIV_0)         /*!< PCLK/2    */
#define TIMER4_CNT_CLK_DIV4                     (TMR4_CCSR_CKDIV_1)         /*!< PCLK/4    */
#define TIMER4_CNT_CLK_DIV8                     (TMR4_CCSR_CKDIV_1 | \
                                                 TMR4_CCSR_CKDIV_0)         /*!< PCLK/8    */
#define TIMER4_CNT_CLK_DIV16                    (TMR4_CCSR_CKDIV_2)         /*!< PCLK/16   */
#define TIMER4_CNT_CLK_DIV32                    (TMR4_CCSR_CKDIV_2 | \
                                                 TMR4_CCSR_CKDIV_0)         /*!< PCLK/32   */
#define TIMER4_CNT_CLK_DIV64                    (TMR4_CCSR_CKDIV_2 | \
                                                 TMR4_CCSR_CKDIV_1)         /*!< PCLK/64   */
#define TIMER4_CNT_CLK_DIV128                   (TMR4_CCSR_CKDIV_2 | \
                                                 TMR4_CCSR_CKDIV_1 | \
                                                 TMR4_CCSR_CKDIV_0)         /*!< PCLK/128  */
#define TIMER4_CNT_CLK_DIV256                   (TMR4_CCSR_CKDIV_3)         /*!< PCLK/256  */
#define TIMER4_CNT_CLK_DIV512                   (TMR4_CCSR_CKDIV_3 | \
                                                 TMR4_CCSR_CKDIV_0)         /*!< PCLK/512  */
#define TIMER4_CNT_CLK_DIV1024                  (TMR4_CCSR_CKDIV_3 | \
                                                 TMR4_CCSR_CKDIV_1)         /*!< PCLK/1024 */
/**
 * @}
 */

/**
 * @defgroup TIMER4_CNT_Mode Timer4 Count Mode
 * @{
 */
#define TIMER4_CNT_MODE_SAWTOOTH_WAVE           ((uint16_t)0x0000u)
#define TIMER4_CNT_MODE_TRIANGLE_WAVE           (TMR4_CCSR_MODE)
/**
 * @}
 */

/**
 * @defgroup TIMER4_CNT_Flag TIMER4 Count Flag
 * @{
 */
#define TIMER4_CNT_FLAG_PEAK                    (TMR4_CCSR_IRQPF)
#define TIMER4_CNT_FLAG_ZERO                    (TMR4_CCSR_IRQZF)
/**
 * @}
 */

/**
 * @defgroup TIMER4_CNT_Interrupt TIMER4 Count Interrupt
 * @{
 */
#define TIMER4_CNT_IT_PEAK                      (TMR4_CCSR_IRQPEN)
#define TIMER4_CNT_IT_ZERO                      (TMR4_CCSR_IRQZEN)
/**
 * @}
 */

/**
 * @defgroup TIMER4_CNT_Clock_Source TIMER4 Count Clock Source
 * @{
 */
#define TIMER4_CNT_PCLK                         ((uint16_t)0x0000u)
#define TIMER4_CNT_EXTCLK                       (TMR4_CCSR_ECKEN)
/**
 * @}
 */

/** @defgroup TIMER4_CNT_Interrupt_Mask_Times TIMER4 Count Interrupt Mask Times
 * @{
 */
#define TIMER4_CNT_INT_MASK_0                   ((uint16_t)0x0000u)     /*!< CNT interrupt flag is always set(not masked) for every CNT count at "0x0000" */
#define TIMER4_CNT_INT_MASK_1                   (TMR4_CVPR_ZIM_0)       /*!< CNT interrupt flag is set once for 2 every CNT counts at "0x0000" (skiping 1 count) */
#define TIMER4_CNT_INT_MASK_2                   (TMR4_CVPR_ZIM_1)       /*!< CNT interrupt flag is set once for 3 every CNT counts at "0x0000" (skiping 2 count) */
#define TIMER4_CNT_INT_MASK_3                   (TMR4_CVPR_ZIM_1 | \
                                                 TMR4_CVPR_ZIM_0)       /*!< CNT interrupt flag is set once for 4 every CNT counts at "0x0000" (skiping 3 count) */
#define TIMER4_CNT_INT_MASK_4                   (TMR4_CVPR_ZIM_2)       /*!< CNT interrupt flag is set once for 5 every CNT counts at "0x0000" (skiping 4 count) */
#define TIMER4_CNT_INT_MASK_5                   (TMR4_CVPR_ZIM_2 | \
                                                 TMR4_CVPR_ZIM_0)       /*!< CNT interrupt flag is set once for 6 every CNT counts at "0x0000" (skiping 5 count) */
#define TIMER4_CNT_INT_MASK_6                   (TMR4_CVPR_ZIM_2 | \
                                                 TMR4_CVPR_ZIM_1)       /*!< CNT interrupt flag is set once for 7 every CNT counts at "0x0000" (skiping 6 count) */
#define TIMER4_CNT_INT_MASK_7                   (TMR4_CVPR_ZIM_2 | \
                                                 TMR4_CVPR_ZIM_1 | \
                                                 TMR4_CVPR_ZIM_0)       /*!< CNT interrupt flag is set once for 8 every CNT counts at "0x0000" (skiping 7 count) */
#define TIMER4_CNT_INT_MASK_8                   (TMR4_CVPR_ZIM_3)       /*!< CNT interrupt flag is set once for 9 every CNT counts at "0x0000" (skiping 8 count) */
#define TIMER4_CNT_INT_MASK_9                   (TMR4_CVPR_ZIM_3 | \
                                                 TMR4_CVPR_ZIM_0)       /*!< CNT interrupt flag is set once for 10 every CNT counts at "0x0000" (skiping 9 count) */
#define TIMER4_CNT_INT_MASK_10                  (TMR4_CVPR_ZIM_3 | \
                                                 TMR4_CVPR_ZIM_1)       /*!< CNT interrupt flag is set once for 11 every CNT counts at "0x0000" (skiping 10 count) */
#define TIMER4_CNT_INT_MASK_11                  (TMR4_CVPR_ZIM_3 | \
                                                 TMR4_CVPR_ZIM_1 | \
                                                 TMR4_CVPR_ZIM_0)       /*!< CNT interrupt flag is set once for 12 every CNT counts at "0x0000" (skiping 11 count) */
#define TIMER4_CNT_INT_MASK_12                  (TMR4_CVPR_ZIM_3 | \
                                                 TMR4_CVPR_ZIM_2)       /*!< CNT interrupt flag is set once for 13 every CNT counts at "0x0000" (skiping 12 count) */
#define TIMER4_CNT_INT_MASK_13                  (TMR4_CVPR_ZIM_3 | \
                                                 TMR4_CVPR_ZIM_2 | \
                                                 TMR4_CVPR_ZIM_0)       /*!< CNT interrupt flag is set once for 14 every CNT counts at "0x0000" (skiping 13 count) */
#define TIMER4_CNT_INT_MASK_14                  (TMR4_CVPR_ZIM_3 | \
                                                 TMR4_CVPR_ZIM_2 | \
                                                 TMR4_CVPR_ZIM_1)       /*!< CNT interrupt flag is set once for 15 every CNT counts at "0x0000" (skiping 14 count) */
#define TIMER4_CNT_INT_MASK_15                  (TMR4_CVPR_ZIM)         /*!< CNT interrupt flag is set once for 16 every CNT counts at "0x0000" (skiping 15 count) */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup TIMER4_OCO_Macros TIMER4 Output Compare Macros
 * @{
 */

/** @defgroup TIMER4_OCO_Channel TIMER4 OCO Channel
 * @{
 */
#define TIMER4_OCO_UH                           ((uint32_t)0ul)   /*!< Timer4 OCO channel:UH */
#define TIMER4_OCO_UL                           ((uint32_t)1ul)   /*!< Timer4 OCO channel:UL */
#define TIMER4_OCO_VH                           ((uint32_t)2ul)   /*!< Timer4 OCO channel:VH */
#define TIMER4_OCO_VL                           ((uint32_t)3ul)   /*!< Timer4 OCO channel:VL */
#define TIMER4_OCO_WH                           ((uint32_t)4ul)   /*!< Timer4 OCO channel:WH */
#define TIMER4_OCO_WL                           ((uint32_t)5ul)   /*!< Timer4 OCO channel:WL */
/**
 * @}
 */

/**
 * @defgroup TIMER4_OCO_Invalid_Port_Output_Polarity TIMER4 OCO Invalid Port Output Polarity
 * @note TIM4_<t>_OxH/TIM4_<t>_OxL: _<t>_ is unit number, t = 1, x = U/V/W
 * @{
 */
#define TIMER4_OCO_INVAILD_OP_LOW               ((uint16_t)0x0000u)             /*!< TIM4_<t>_OxH/TIM4_<t>_OxL output low level when OCO is invalid */
#define TIMER4_OCO_INVAILD_OP_HIGH              (!TIMER4_OCO_INVAILD_OP_LOW)    /*!< TIM4_<t>_OxH/TIM4_<t>_OxL output high level when OCO is invalid */
/**
 * @}
 */

/** @defgroup TIMER4_OCO_OCCR_Buffer_Mode TIMER4 OCO OCCR Buffer Mode
 * @{
 */
#define TIMER4_OCO_OCCR_BUF_DISABLE             ((uint16_t)0x0000u)                             /*!< Disable the register OCCR buffer function */
#define TIMER4_OCO_OCCR_BUF_CNT_ZERO            (TMR4_OCER_CHBUFEN_0 >> TMR4_OCER_CHBUFEN_POS)  /*!< Register OCCR buffer transfer when counter value is 0x0000 */
#define TIMER4_OCO_OCCR_BUF_CNT_PEAK            (TMR4_OCER_CHBUFEN_1 >> TMR4_OCER_CHBUFEN_POS)  /*!< Register OCCR buffer transfer when counter value is CPSR */
#define TIMER4_OCO_OCCR_BUF_CNT_ZERO_OR_PEAK    (TMR4_OCER_CHBUFEN >> TMR4_OCER_CHBUFEN_POS)    /*!< Register OCCR buffer transfer when the value is both 0 and CPSR */
/**
 * @}
 */

/** @defgroup TIMER4_OCO_OCMR_Buffer_Mode TIMER4 OCO OCMR Buffer Mode
 * @{
 */
#define TIMER4_OCO_OCMR_BUF_DISABLE             ((uint16_t)0x0000u)                                         /*!< Disable the register OCCR buffer function */
#define TIMER4_OCO_OCMR_BUF_CNT_ZERO            ((uint16_t)(TMR4_OCER_MHBUFEN_0 >> TMR4_OCER_MHBUFEN_POS))  /*!< Register OCMR buffer transfer when counter value is 0x0000 */
#define TIMER4_OCO_OCMR_BUF_CNT_PEAK            ((uint16_t)(TMR4_OCER_MHBUFEN_1 >> TMR4_OCER_MHBUFEN_POS))  /*!< Register OCMR buffer transfer when counter value is CPSR */
#define TIMER4_OCO_OCMR_BUF_CNT_ZERO_OR_PEAK    ((uint16_t)(TMR4_OCER_MHBUFEN >> TMR4_OCER_MHBUFEN_POS))    /*!< Register OCMR buffer transfer when the value is both 0 and CPSR */
/**
 * @}
 */

/** @defgroup TIMER4_OCO_Cnt_Match_OCF_State TIMER4 OCO Cnt Match OCF State
 * @{
 */
#define TIMER4_OCO_OCF_HOLD                     ((uint16_t)0x0000u)     /*!< Hold OCF */
#define TIMER4_OCO_OCF_SET                      (TMR4_OCMRH_OCFDCH)     /*!< Set OCF */
/**
 * @}
 */

/** @defgroup TIMER4_OCO_Cnt_Match_Output_Polarity TIMER4 OCO Cnt Match Output Polarity
 * @{
 */
#define TIMER4_OCO_OP_HOLD                      ((uint16_t)0x0000u)                           /*!< Hold output when the TIMER4 OCO count match */
#define TIMER4_OCO_OP_HIGH                      (TMR4_OCMRH_OPDCH_0 >> TMR4_OCMRH_OPDCH_POS)  /*!< Output high when the TIMER4 OCO count match */
#define TIMER4_OCO_OP_LOW                       (TMR4_OCMRH_OPDCH_0 >> TMR4_OCMRH_OPDCH_POS)  /*!< Output low when the TIMER4 OCO count match */
#define TIMER4_OCO_OP_INVERT                    (TMR4_OCMRH_OPDCH >> TMR4_OCMRH_OPDCH_POS)    /*!< Invert output when the TIMER4 OCO count match */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup TIMER4_PWM_Macros TIMER4 PWM Macros
 * @{
 */

/** @defgroup TIMER4_PWM_Channel TIMER4 PWM Channel
 * @{
 */
#define TIMER4_PWM_U                            ((uint32_t)0u)   /*!< Timer4 PWM couple channel: U */
#define TIMER4_PWM_V                            ((uint32_t)1u)   /*!< Timer4 PWM couple channel: V */
#define TIMER4_PWM_W                            ((uint32_t)2u)   /*!< Timer4 PWM couple channel: W */
/**
 * @}
 */

/** @defgroup TIMER4_PWM_Clock_Division TIMER4 PWM Clock Division
 * @{
 */
#define TIMER4_PWM_CLK_DIV1                     ((uint16_t)0x0000u)     /*!< PCLK     */
#define TIMER4_PWM_CLK_DIV2                     (TMR4_POCR_DIVCK_0)     /*!< PCLK/2   */
#define TIMER4_PWM_CLK_DIV4                     (TMR4_POCR_DIVCK_1)     /*!< PCLK/4   */
#define TIMER4_PWM_CLK_DIV8                     (TMR4_POCR_DIVCK_1 | \
                                                 TMR4_POCR_DIVCK_0)     /*!< PCLK/8   */
#define TIMER4_PWM_CLK_DIV16                    (TMR4_POCR_DIVCK_2)     /*!< PCLK/16  */
#define TIMER4_PWM_CLK_DIV32                    (TMR4_POCR_DIVCK_2 | \
                                                 TMR4_POCR_DIVCK_0)     /*!< PCLK/32  */
#define TIMER4_PWM_CLK_DIV64                    (TMR4_POCR_DIVCK_2 | \
                                                 TMR4_POCR_DIVCK_1)     /*!< PCLK/64  */
#define TIMER4_PWM_CLK_DIV128                   (TMR4_POCR_DIVCK)       /*!< PCLK/128 */
/**
 * @}
 */

/**
 * @defgroup TIMER4_PWM_Mode Timer4 PWM Mode
 * @{
 */
#define TIMER4_PWM_THROUGH_MODE                 ((uint16_t)0x0000u) /*!< Through mode */
#define TIMER4_PWM_DEAD_TIMER_MODE              (TMR4_POCR_PWMMD_0) /*!< Dead timer mode */
#define TIMER4_PWM_DEAD_TIMER_FILTER_MODE       (TMR4_POCR_PWMMD_1) /*!< Dead timer filter mode */
/**
 * @}
 */

/**
 * @defgroup TIMER4_PWM_Output_Polarity Timer4 PWM Output Polarity
 * @{
 */
#define TIMER4_PWM_OP_OXH_HOLD_OXL_HOLD         ((uint16_t)0x0000u) /*!< Output PWML and PWMH signals without changing the level */
#define TIMER4_PWM_OP_OXH_INVERT_OXL_INVERT     (TMR4_POCR_LVLS_0)  /*!< Output both PWML and PWMH signals reversed */
#define TIMER4_PWM_OP_OXH_INVERT_OXL_HOLD       (TMR4_POCR_LVLS_1)  /*!< Output the PWMH signal reversed, outputs the PWML signal without changing the level. */
#define TIMER4_PWM_OP_OXH_HOLD_OXL_INVERT       (TMR4_POCR_LVLS)    /*!< Output the PWMH signal without changing the level, Outputs the PWML signal reversed. */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup TIMER4_SEVT_Macros TIMER4 Special Event Macros
 * @{
 */

/** @defgroup TIMER4_SEVT_Channel TIMER4 SEVT Channel
 * @{
 */
#define TIMER4_SEVT_UH                          ((uint32_t)0ul)   /*!< Timer4 SEVT channel:UH */
#define TIMER4_SEVT_UL                          ((uint32_t)1ul)   /*!< Timer4 SEVT channel:UL */
#define TIMER4_SEVT_VH                          ((uint32_t)2ul)   /*!< Timer4 SEVT channel:VH */
#define TIMER4_SEVT_VL                          ((uint32_t)3ul)   /*!< Timer4 SEVT channel:VL */
#define TIMER4_SEVT_WH                          ((uint32_t)4ul)   /*!< Timer4 SEVT channel:WH */
#define TIMER4_SEVT_WL                          ((uint32_t)5ul)   /*!< Timer4 SEVT channel:WL */
/**
 * @}
 */

/** @defgroup TIMER4_SEVT_Buffer_Mode TIMER4 SEVT Buffer Mode
 * @{
 */
#define TIMER4_SEVT_BUF_DISABLE                 ((uint16_t)0x0000u) /*!< Disable the register SCCR&SCMR buffer function */
#define TIMER4_SEVT_BUF_CNT_ZERO                (TMR4_SCSR_BUFEN_0) /*!< Register SCCR&SCMR buffer transfer when counter value is 0x0000 */
#define TIMER4_SEVT_BUF_CNT_PEAK                (TMR4_SCSR_BUFEN_1) /*!< Register SCCR&SCMR buffer transfer when counter value is CPSR */
#define TIMER4_SEVT_BUF_CNT_ZERO_OR_PEAK        (TMR4_SCSR_BUFEN)   /*!< Register SCCR&SCMR buffer transfer when the value is both 0 and CPSR */
/**
 * @}
 */

/** @defgroup TIMER4_SEVT_Trigger_Event TIMER4 SEVT Trigger Event
 * @{
 */
#define TIMER4_SEVT_TRIGGER_TMR4_SCMUH          ((uint16_t)0x0000u)     /*!< Timer4 Special-EVT trigger event: TMR4_SCMUH */
#define TIMER4_SEVT_TRIGGER_TMR4_SCMUL          (TMR4_SCSR_EVTOS_0)     /*!< Timer4 Special-EVT trigger event: TMR4_SCMUL */
#define TIMER4_SEVT_TRIGGER_TMR4_SCMVH          (TMR4_SCSR_EVTOS_1)     /*!< Timer4 Special-EVT trigger event: TMR4_SCMVH */
#define TIMER4_SEVT_TRIGGER_TMR4_SCMVL          (TMR4_SCSR_EVTOS_1 | \
                                                 TMR4_SCSR_EVTOS_0)     /*!< Timer4 Special-EVT trigger event: TMR4_SCMVL */
#define TIMER4_SEVT_TRIGGER_TMR4_SCMWH          (TMR4_SCSR_EVTOS_2)     /*!< Timer4 Special-EVT trigger event: TMR4_SCMWH */
#define TIMER4_SEVT_TRIGGER_TMR4_SCMWL          (TMR4_SCSR_EVTOS_2 | \
                                                 TMR4_SCSR_EVTOS_0)     /*!< Timer4 Special-EVT trigger event: TMR4_SCMWL */
/**
 * @}
 */

/** @defgroup TIMER4_SEVT_Mode TIMER4 SEVT Mode
 * @{
 */
#define TIMER4_SEVT_MODE_COMPARE_TRIGGER        ((uint16_t)0x0000u) /*!< TIMER4 SEVT mode: OCCRxh */
#define TIMER4_SEVT_MODE_DELAY_TRIGGER          (TMR4_SCSR_EVTMS)   /*!< TIMER4 SEVT mode: OCCRxl */
/**
 * @}
 */

/** @defgroup TIMER4_SEVT_Delay_Object TIMER4 SEVT Delay Object
 * @{
 */
#define TIMER4_SEVT_DELAY_OCCRXH                ((uint16_t)0x0000u) /*!< TIMER4 SEVT delay object: OCCRxh */
#define TIMER4_SEVT_DELAY_OCCRXL                (TMR4_SCSR_EVTDS)   /*!< TIMER4 SEVT delay object: OCCRxl */
/**
 * @}
 */

/** @defgroup TIMER4_SEVT_Mask_Times TIMER4 SEVT Mask Times
 * @{
 */
#define TIMER4_SEVT_MASK_0                      ((uint16_t)0x0000u)     /*!< Mask 0 time */
#define TIMER4_SEVT_MASK_1                      (TMR4_SCMR_AMC_0)       /*!< Mask 1 times */
#define TIMER4_SEVT_MASK_2                      (TMR4_SCMR_AMC_1)       /*!< Mask 2 times */
#define TIMER4_SEVT_MASK_3                      (TMR4_SCMR_AMC_1 | \
                                                 TMR4_SCMR_AMC_0)       /*!< Mask 3 times */
#define TIMER4_SEVT_MASK_4                      (TMR4_SCMR_AMC_2)       /*!< Mask 4 times */
#define TIMER4_SEVT_MASK_5                      (TMR4_SCMR_AMC_2 | \
                                                 TMR4_SCMR_AMC_0)       /*!< Mask 5 times */
#define TIMER4_SEVT_MASK_6                      (TMR4_SCMR_AMC_2 | \
                                                 TMR4_SCMR_AMC_1)       /*!< Mask 6 times */
#define TIMER4_SEVT_MASK_7                      (TMR4_SCMR_AMC_2 | \
                                                 TMR4_SCMR_AMC_1 | \
                                                 TMR4_SCMR_AMC_0)       /*!< Mask 7 times */
#define TIMER4_SEVT_MASK_8                      (TMR4_SCMR_AMC_3)       /*!< Mask 8 times */
#define TIMER4_SEVT_MASK_9                      (TMR4_SCMR_AMC_3 | \
                                                 TMR4_SCMR_AMC_0)       /*!< Mask 9 times */
#define TIMER4_SEVT_MASK_10                     (TMR4_SCMR_AMC_3 | \
                                                 TMR4_SCMR_AMC_1)       /*!< Mask 10 times */
#define TIMER4_SEVT_MASK_11                     (TMR4_SCMR_AMC_3 | \
                                                 TMR4_SCMR_AMC_1 | \
                                                 TMR4_SCMR_AMC_0)       /*!< Mask 11 times */
#define TIMER4_SEVT_MASK_12                     (TMR4_SCMR_AMC_3 | \
                                                 TMR4_SCMR_AMC_2)       /*!< Mask 12 times */
#define TIMER4_SEVT_MASK_13                     (TMR4_SCMR_AMC_3 | \
                                                 TMR4_SCMR_AMC_2 | \
                                                 TMR4_SCMR_AMC_0)       /*!< Mask 13 times */
#define TIMER4_SEVT_MASK_14                     (TMR4_SCMR_AMC_3 | \
                                                 TMR4_SCMR_AMC_2 | \
                                                 TMR4_SCMR_AMC_1)       /*!< Mask 14 times */
#define TIMER4_SEVT_MASK_15                     (TMR4_SCMR_AMC)         /*!< Mask 15 times */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup TIMER4_EMB_Macros TIMER4 Emergency Brake Macros
 * @{
 */

/** @defgroup TIMER4_EMB_Trigger_PWM_Output_Polarity TIMER4 EMB Trigger PWM Output Polarity
 * @{
 */
#define TIMER4_EMB_TRIG_PWM_OP_NORMAL           ((uint32_t)0x0000ul)        /*!< PWM output signal normally when EMB signal occurs */
#define TIMER4_EMB_TRIG_PWM_OP_HOLD             (TMR4_ECSR_EMBVAL_2)        /*!< Hold PWM output when EMB signal occurs */
#define TIMER4_EMB_TRIG_PWM_OP_HIZ              (TMR4_ECSR_EMBVAL_0)        /*!< PWM output Hiz signal when EMB signal occurs */
#define TIMER4_EMB_TRIG_PWM_OP_LOW              (TMR4_ECSR_EMBVAL_1)        /*!< PWM output low level signal when EMB signal occurs */
#define TIMER4_EMB_TRIG_PWM_OP_HIGH             (TMR4_ECSR_EMBVAL_1 | \
                                                 TMR4_ECSR_EMBVAL_0)        /*!< PWM output high level signal when EMB signal occurs */
/**
 * @}
 */

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
 * @addtogroup TIMER4_Global_Functions
 * @{
 */

/**
 * @brief Set Timer4 CNT clock source
 * @param  [in] u16ClkSrc               Timer4 CNT clock source
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_PCLK:      Uses the internal clock (PCLK) as CNT's count clock
 *           @arg TIMER4_CNT_EXTCLK:    Uses an external input clock (EXCK) as CNT's count clock
 * @retval None
 */
__STATIC_INLINE void TIMER4_CNT_SetClock(uint16_t u16ClkSrc)
{
    /* Set external clock enable bit */
    bM0P_TMR4->CCSR_b.ECKEN = u16ClkSrc ? 1ul : 0ul;
}

/**
 * @brief Get Timer4 CNT clock source
 * @param  None
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_CNT_PCLK:      Uses the internal clock (PCLK) as CNT's count clock
 *           @arg TIMER4_CNT_EXTCLK:    Uses an external input clock (EXCK) as CNT's count clock
 */
__STATIC_INLINE uint16_t TIMER4_CNT_GetClock(void)
{
    /* Get external clock enable bit */
    return READ_REG16_BIT(M0P_TMR4->CCSR, TMR4_CCSR_ECKEN);
}

/**
 * @brief Set Timer4 CNT mode
 * @param  [in] u16Mode             Timer4 CNT mode
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_MODE_SAWTOOTH_WAVE:    Timer4 count mode:sawtooth wave
 *           @arg TIMER4_CNT_MODE_TRIANGLE_WAVE:    Timer4 count mode:triangular wave
 * @retval None
 */
__STATIC_INLINE void TIMER4_CNT_SetMode(uint16_t u16Mode)
{
    /* Set external clock enable bit */
    bM0P_TMR4->CCSR_b.MODE = (!!u16Mode);
}

/**
 * @brief Get Timer4 CNT mode
 * @param  None
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_CNT_MODE_SAWTOOTH_WAVE:    Timer4 count mode:sawtooth wave
 *           @arg TIMER4_CNT_MODE_TRIANGLE_WAVE:    Timer4 count mode:triangular wave
 */
__STATIC_INLINE uint16_t TIMER4_CNT_GetMode(void)
{
    /* Get external clock enable bit */
    return READ_REG16_BIT(M0P_TMR4->CCSR, TMR4_CCSR_MODE);
}

/**
 * @brief Start Timer4 CNT
 * @param  None
 * @retval None
 */
__STATIC_INLINE void TIMER4_CNT_Start(void)
{
    bM0P_TMR4->CCSR_b.STOP = 0ul;
}

/**
 * @brief Stop Timer4 CNT
 * @param  None
 * @retval None
 */
__STATIC_INLINE void TIMER4_CNT_Stop(void)
{
    bM0P_TMR4->CCSR_b.STOP = 1ul;
}

/**
 * @brief  Get Timer4 CNT interrupt flag
 * @param  [in] u16Flag                 Timer4 flag
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_FLAG_PEAK: Overflow interrupt
 *           @arg TIMER4_CNT_FLAG_ZERO: Underflow interrupt
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Flag is set
 *           - Reset: Flag is reset
 */
__STATIC_INLINE en_flag_status_t TIMER4_CNT_GetFlag(uint16_t u16Flag)
{
    return READ_REG16_BIT(M0P_TMR4->CCSR, u16Flag) ? Set : Reset;
}

/**
 * @brief  Clear Timer4 CNT interrupt flag
 * @param  [in] u16Flag                 Timer4 flag
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_FLAG_PEAK: Overflow interrupt
 *           @arg TIMER4_CNT_FLAG_ZERO: Underflow interrupt
 * @retval None
 */
__STATIC_INLINE void TIMER4_CNT_ClearFlag(uint16_t u16Flag)
{
    CLEAR_REG16_BIT(M0P_TMR4->CCSR, u16Flag);
}

/**
 * @brief  Set Timer4 CNT cycle value
 * @param  [in] u16CycleVal             The Timer4 CNT cycle value
 *           @arg number of 16bit
 * @retval None
 */
__STATIC_INLINE void TIMER4_CNT_SetCycleVal(uint16_t u16CycleVal)
{
    WRITE_REG(M0P_TMR4->CPSR, u16CycleVal);
}

/**
 * @brief  Get Timer4 CNT cycle value
 * @param  None
 * @retval The cycle value of the Timer4 CNT
 */
__STATIC_INLINE uint16_t TIMER4_CNT_GetCycleVal(void)
{
    return READ_REG16(M0P_TMR4->CPSR);
}

/**
 * @brief  Clear Timer4 CNT count value
 * @param  None
 * @retval None
 */
__STATIC_INLINE void TIMER4_CNT_ClearCountVal(void)
{
    bM0P_TMR4->CCSR_b.CLEAR = 1ul;
}

/**
 * @brief  Set Timer4 CNT count value
 * @param  [in] u16CountVal             The Timer4 CNT count value
 *           @arg number of 16bit
 * @retval None
 */
__STATIC_INLINE void TIMER4_CNT_SetCountVal(uint16_t u16CountVal)
{
    WRITE_REG16(M0P_TMR4->CNTR, u16CountVal);
}

/**
 * @brief  Get Timer4 CNT count value
 * @param  None
 * @retval The count value of the Timer4 CNT
 */
__STATIC_INLINE uint16_t TIMER4_CNT_GetCountVal(void)
{
    return READ_REG16(M0P_TMR4->CNTR);
}

en_result_t TIMER4_CNT_Init(const stc_timer4_cnt_init_t *pstcInit);
en_result_t TIMER4_CNT_StructInit(stc_timer4_cnt_init_t *pstcInit);
en_result_t TIMER4_CNT_DeInit(void);
void TIMER4_CNT_SetClkDiv(uint16_t u16Div);
uint16_t TIMER4_CNT_GetClkDiv(void);
void TIMER4_CNT_IntCmd(uint16_t u16IntSource, en_functional_state_t enNewSta);
void TIMER4_CNT_SetIntMaskTimes(uint16_t u16IntSource,
                                uint16_t u16MaskTimes);
uint16_t TIMER4_CNT_GetIntMaskTimes(uint16_t u16IntSource);
uint16_t TIMER4_CNT_GetIntMaskCurrenTimes(uint16_t u16IntSource);
en_result_t TIMER4_OCO_Init(uint32_t u32Ch,
                                const stc_timer4_oco_init_t *pstcInit);
en_result_t TIMER4_OCO_StructInit(stc_timer4_oco_init_t *pstcInit);
en_result_t TIMER4_OCO_DeInit(uint32_t u32Ch);
en_result_t TIMER4_OCO_SetOccrBufMode(uint32_t u32Ch,
                                uint16_t u16OccrBufMode);
uint16_t TIMER4_OCO_GetOccrBufMode(uint32_t u32Ch);
en_result_t TIMER4_OCO_OccrLinkTransferCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta);
en_result_t TIMER4_OCO_SetOcmrBufMode(uint32_t u32Ch,
                                uint16_t u16OcmrBufMode);
uint16_t TIMER4_OCO_GetOcmrBufMode(uint32_t u32Ch);
en_result_t TIMER4_OCO_OcmrLinkTransferCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta);
en_result_t TIMER4_OCO_ExtMatchCondCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta);
en_result_t TIMER4_OCO_SetHighChCompareMode(uint32_t u32Ch,
                                const stc_oco_high_ch_compare_mode_t *pstcMode);
en_result_t TIMER4_OCO_SetLowChCompareMode(uint32_t u32Ch,
                                 const stc_oco_low_ch_compare_mode_t *pstcMode);
en_result_t TIMER4_OCO_OutputCompareCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta);
en_result_t TIMER4_OCO_IntCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta);
en_flag_status_t TIMER4_OCO_GetFlag(uint32_t u32Ch);
en_result_t TIMER4_OCO_ClearFlag(uint32_t u32Ch);
en_result_t TIMER4_OCO_SetOcoInvalidOp(uint32_t u32Ch,
                                uint16_t u16OutputPolarity);
uint16_t TIMER4_OCO_GetOutputPolarity(uint32_t u32Ch);
void TIMER4_OCO_SetOccrVal(uint32_t u32Ch, uint16_t u16OccrVal);
uint16_t TIMER4_OCO_GetOccrVal(uint32_t u32Ch);
en_result_t TIMER4_PWM_Init(uint32_t u32Ch,
                                const stc_timer4_pwm_init_t *pstcInit);
en_result_t TIMER4_PWM_StructInit(stc_timer4_pwm_init_t *pstcInit);
en_result_t TIMER4_PWM_DeInit(uint32_t u32Ch);
en_result_t TIMER4_PWM_SetOutputPolarity(uint32_t u32Ch,
                                                uint16_t u16OutputPolarity);
uint16_t TIMER4_PWM_GetOutputPolarity(uint32_t u32Ch);
en_result_t TIMER4_PWM_StartReloadTimer(uint32_t u32Ch);
en_result_t TIMER4_PWM_StopReloadTimer(uint32_t u32Ch);
en_result_t TIMER4_PWM_IntCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta);
en_flag_status_t TIMER4_PWM_GetFlag(uint32_t u32Ch);
en_result_t TIMER4_PWM_ClearFlag(uint32_t u32Ch);
en_result_t TIMER4_PWM_SetDeadRegionValue(uint32_t u32Ch,
                                                uint16_t u16PDAR,
                                                uint16_t u16PDBR);
en_result_t TIMER4_PWM_GetDeadRegionValue(uint32_t u32Ch,
                                                uint16_t *pu16PDAR,
                                                uint16_t *pu16PDBR);
en_result_t TIMER4_PWM_SetFilterCountValue(uint32_t u32Ch,
                                                        uint16_t u16Count);
en_result_t TIMER4_SEVT_Init(uint32_t u32Ch,
                                const stc_timer4_sevt_init_t *pstcInit);
en_result_t TIMER4_SEVT_StructInit(stc_timer4_sevt_init_t *pstcInit);
en_result_t TIMER4_SEVT_DeInit(uint32_t u32Ch);
en_result_t TIMER4_SEVT_SetTriggerEvent(uint32_t u32Ch,
                                                    uint16_t u16Event);
en_result_t TIMER4_SEVT_SetTriggerCondition(uint32_t u32Ch,
                    const stc_timer4_sevt_trigger_cond_t *pstcTriggerCond);
en_result_t TIMER4_SEVT_SetDelayObject(uint32_t u32Ch, uint16_t u16DelayObject);
uint16_t TIMER4_SEVT_GetDelayObject(uint32_t u32Ch);
en_result_t TIMER4_SEVT_SetSccrVal(uint32_t u32Ch,
                                            uint16_t u16SccrVal);
uint16_t TIMER4_SEVT_GetSccrVal(uint32_t u32Ch);
en_result_t TIMER4_SEVT_SetMaskTimes(uint32_t u32Ch,
                                            uint16_t u16MaskTimes);
uint16_t TIMER4_SEVT_GetMaskTimes(uint32_t u32Ch);
void TIMER4_EMB_SetPwmPortPolarity(uint32_t u32PortPolarity);
uint32_t TIMER4_EMB_GetPwmPortPolarity(void);

/**
 * @}
 */

#endif /* DDL_TIMER4_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_TIMER4_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
