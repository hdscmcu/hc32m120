/**
 *******************************************************************************
 * @file  hc32m120_timera.c
 * @brief This file provides firmware functions to manage the TimerA.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32m120_timera.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_TIMERA TIMERA
 * @brief TimerA Driver Library
 * @{
 */

#if (DDL_TIMERA_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMERA_Local_Macros TimerA Local Macros
 * @{
 */

/**
 * @defgroup TIMERA_Private_Parameter TIMERA Private Parameter
 * @{
 */
#define TIMERA_PWM_CHANNEL_NUMBER           ((uint8_t)0x02u)         /*!< TIMERA PWM channel number */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Hardware_Trigger_Source_Mask TIMERA Hardware Trigger Source Mask
 * @{
 */
#define TIMERA_TRIG_SRC_MASK                ((uint32_t)0x0000007Ful)  /*!< TIMERA Hardware Trigger Source Mask */
/**
 * @}
 */

/**
 * @defgroup TIMERA_Register_Combination_Mask TIMERA Register Combination Mask
 * @{
 */
#define TIMERA_BCONR_BSE_MASK               (TMRA_BCONR_BSE1 | TMRA_BCONR_BSE0)
#define TIMERA_CCONR_HICP_MASK              (TMRA_CCONR1_HICP2 | TMRA_CCONR1_HICP1 | TMRA_CCONR1_HICP0)
#define TIMERA_HCONR_HSTA_MASK              (TMRA_HCONR_HSTA2 | TMRA_HCONR_HSTA1 | TMRA_HCONR_HSTA0)
#define TIMERA_HCONR_HSTP_MASK              (TMRA_HCONR_HSTP2 | TMRA_HCONR_HSTP1 | TMRA_HCONR_HSTP0)
#define TIMERA_HCONR_HCLE_MASK              (TMRA_HCONR_HCLE2 | TMRA_HCONR_HCLE1 | TMRA_HCONR_HCLE0)
#define TIMERA_HCUPR_HCUP_MASK              (TMRA_HCUPR_HCUP10 | TMRA_HCUPR_HCUP9 | TMRA_HCUPR_HCUP8 | \
                                             TMRA_HCUPR_HCUP7  | TMRA_HCUPR_HCUP6 | TMRA_HCUPR_HCUP5 | \
                                             TMRA_HCUPR_HCUP4  | TMRA_HCUPR_HCUP3 | TMRA_HCUPR_HCUP2 | \
                                             TMRA_HCUPR_HCUP1  | TMRA_HCUPR_HCUP0)
#define TIMERA_HCDOR_HCDO_MASK              (TMRA_HCDOR_HCDO10 | TMRA_HCDOR_HCDO9 | TMRA_HCDOR_HCDO8 | \
                                             TMRA_HCDOR_HCDO7  | TMRA_HCDOR_HCDO6 | TMRA_HCDOR_HCDO5 | \
                                             TMRA_HCDOR_HCDO4  | TMRA_HCDOR_HCDO3 | TMRA_HCDOR_HCDO2 | \
                                             TMRA_HCDOR_HCDO1  | TMRA_HCDOR_HCDO0)
#define TIMERA_HW_TRIG_MASK                 (TIMERA_HCONR_HSTA_MASK | TIMERA_HCONR_HSTP_MASK | TIMERA_HCONR_HCLE_MASK)
#define TIMERA_EVENT_MASK                   (TIMERA_EVENT_CMP1 | TIMERA_EVENT_CMP2)
#define TIMERA_INT_MASK                     (TIMERA_INT_OVF    | TIMERA_INT_UDF  | \
                                             TIMERA_INT_CMP1   | TIMERA_INT_CMP2)
#define TIMERA_FALG_MASK                    (TIMERA_FLAG_OVF   | TIMERA_FLAG_UDF | \
                                             TIMERA_FLAG_CMP1  | TIMERA_FLAG_CMP2)
/**
 * @}
 */

/**
 * @defgroup TIMERA_CMP_CAP_Register TIMERA Compare/Capture Register
 * @brief Get the specified Compare/Capture register address of the TimerA unit
 * @{
 */
#define TMRA_CMPARx(__TMRAx__, __CH__)      ((uint32_t)(&((__TMRAx__)->CMPAR1)) + ((uint32_t)(__CH__))*4u)
#define TMRA_BCONRx(__TMRAx__, __CH__)      ((uint32_t)(&((__TMRAx__)->BCONR))  + ((uint32_t)(__CH__))*4u)
#define TMRA_CCONRx(__TMRAx__, __CH__)      ((uint32_t)(&((__TMRAx__)->CCONR1)) + ((uint32_t)(__CH__))*4u)
#define TMRA_PCONRx(__TMRAx__, __CH__)      ((uint32_t)(&((__TMRAx__)->PCONR1)) + ((uint32_t)(__CH__))*4u)
/**
 * @}
 */

/**
 * @defgroup TIMERA_Check_Parameters_Validity TIMERA Check Parameters Validity
 * @{
 */

#define IS_TIMERA_INSTANCE(x)                   (M0P_TMRA == (x))

#define IS_TIMERA_CHANNEL(x)                                                   \
(   (TIMERA_CHANNEL_CH1 == (x))                 ||                             \
    (TIMERA_CHANNEL_CH2 == (x)))

#define IS_TIMERA_CACHE_CHANNEL(x)                                             \
(   (TIMERA_CHANNEL_CH1 == (x)))

#define IS_TIMERA_COUNT_MODE(x)                                                \
(   (TIMERA_SAWTOOTH_WAVE == (x))               ||                             \
    (TIMERA_TRIANGLE_WAVE == (x)))

#define IS_TIMERA_COUNT_DIRECTION(x)                                           \
(   (TIMERA_COUNT_UP == (x))                    ||                             \
    (TIMERA_COUNT_DOWN == (x)))

#define IS_TIMERA_CLK_DIV(x)                                                   \
(   (TIMERA_CLKDIV_DIV1 == (x))                 ||                             \
    (TIMERA_CLKDIV_DIV2 == (x))                 ||                             \
    (TIMERA_CLKDIV_DIV4 == (x))                 ||                             \
    (TIMERA_CLKDIV_DIV8 == (x))                 ||                             \
    (TIMERA_CLKDIV_DIV16 == (x))                ||                             \
    (TIMERA_CLKDIV_DIV32 == (x))                ||                             \
    (TIMERA_CLKDIV_DIV64 == (x))                ||                             \
    (TIMERA_CLKDIV_DIV128 == (x))               ||                             \
    (TIMERA_CLKDIV_DIV256 == (x))               ||                             \
    (TIMERA_CLKDIV_DIV512 == (x))               ||                             \
    (TIMERA_CLKDIV_DIV1024 == (x)))

#define IS_TIMERA_OVERFLOW_STATE(x)                                            \
(   (TIMERA_OVERFLOW_STOP == (x))               ||                             \
    (TIMERA_OVERFLOW_COUNT == (x)))

#define IS_TIMERA_HWTRIGGER_CONDITION(x)                                       \
(   (!((x) & ((uint16_t)(~TIMERA_HW_TRIG_MASK))))           ||                 \
    (TIMERA_HWSTART_INVALID == (x)))

#define IS_TIMERA_HWSTART_CONDITION(x)                                         \
(   (!((x) & ((uint16_t)(~TIMERA_HCONR_HSTA_MASK))))        ||                 \
    (TIMERA_HWSTART_INVALID == (x)))

#define IS_TIMERA_HWSTOP_CONDITION(x)                                          \
(   (!((x) & ((uint16_t)(~TIMERA_HCONR_HSTP_MASK))))        ||                 \
    (TIMERA_HWSTOP_INVALID == (x)))

#define IS_TIMERA_HWCLEAR_CONDITION(x)                                         \
(   (!((x) & ((uint16_t)(~TIMERA_HCONR_HCLE_MASK))))        ||                 \
    (TIMERA_HWCLEAR_INVALID == (x)))

#define IS_TIMERA_HWUP_CONDITION(x)                                            \
(   (!((x) & ((uint16_t)(~TIMERA_HCUPR_HCUP_MASK))))        ||                 \
    (TIMERA_HWUP_INVALID == (x)))

#define IS_TIMERA_HWDOWN_CONDITION(x)                                          \
(   (!((x) & ((uint16_t)(~TIMERA_HCDOR_HCDO_MASK))))        ||                 \
    (TIMERA_HWDOWN_INVALID == (x)))

#define IS_TIMERA_TRIG_PORT_FILTER_STATE(x)                                    \
(   (TIMERA_TRIG_FILTER_ENABLE == (x))          ||                             \
    (TIMERA_TRIG_FILTER_DISABLE == (x)))

#define IS_TIMERA_TRIG_PORT_FILTER_CLKDIV(x)                                   \
(   (TIMERA_TRIG_CLKDIV_DIV1 == (x))            ||                             \
    (TIMERA_TRIG_CLKDIV_DIV4 == (x))            ||                             \
    (TIMERA_TRIG_CLKDIV_DIV16 == (x))           ||                             \
    (TIMERA_TRIG_CLKDIV_DIV64 == (x)))

#define IS_TIMERA_CLKA_PORT_FILTER_STATE(x)                                    \
(   (TIMERA_CLKA_FILTER_ENABLE == (x))          ||                             \
    (TIMERA_CLKA_FILTER_DISABLE == (x)))

#define IS_TIMERA_CLKA_PORT_FILTER_CLKDIV(x)                                   \
(   (TIMERA_CLKA_CLKDIV_DIV1 == (x))            ||                             \
    (TIMERA_CLKA_CLKDIV_DIV4 == (x))            ||                             \
    (TIMERA_CLKA_CLKDIV_DIV16 == (x))           ||                             \
    (TIMERA_CLKA_CLKDIV_DIV64 == (x)))

#define IS_TIMERA_CLKB_PORT_FILTER_STATE(x)                                    \
(   (TIMERA_CLKB_FILTER_ENABLE == (x))          ||                             \
    (TIMERA_CLKB_FILTER_DISABLE == (x)))

#define IS_TIMERA_CLKB_PORT_FILTER_CLKDIV(x)                                   \
(   (TIMERA_CLKB_CLKDIV_DIV1 == (x))            ||                             \
    (TIMERA_CLKB_CLKDIV_DIV4 == (x))            ||                             \
    (TIMERA_CLKB_CLKDIV_DIV16 == (x))           ||                             \
    (TIMERA_CLKB_CLKDIV_DIV64 == (x)))

#define IS_TIMERA_OC_START_COUNT_OUTPUT_POLARITY(x)                            \
(   (TIMERA_OC_STARTCOUNT_OUTPUT_LOW == (x))    ||                             \
    (TIMERA_OC_STARTCOUNT_OUTPUT_HIGH == (x))   ||                             \
    (TIMERA_OC_STARTCOUNT_OUTPUT_HOLD == (x)))

#define IS_TIMERA_OC_STOP_COUNT_OUTPUT_POLARITY(x)                             \
(   (TIMERA_OC_STOPCOUNT_OUTPUT_LOW == (x))     ||                             \
    (TIMERA_OC_STOPCOUNT_OUTPUT_HIGH == (x))    ||                             \
    (TIMERA_OC_STOPCOUNT_OUTPUT_HOLD == (x)))

#define IS_TIMERA_OC_CMPMATCH_OUTPUT_POLARITY(x)                               \
(   (TIMERA_OC_CMPMATCH_OUTPUT_LOW == (x))      ||                             \
    (TIMERA_OC_CMPMATCH_OUTPUT_HIGH == (x))     ||                             \
    (TIMERA_OC_CMPMATCH_OUTPUT_HOLD == (x))     ||                             \
    (TIMERA_OC_CMPMATCH_OUTPUT_INVERTED == (x)))

#define IS_TIMERA_OC_PERIODMATCH_OUTPUT_POLARITY(x)                            \
(   (TIMERA_OC_PERIODMATCH_OUTPUT_LOW == (x))   ||                             \
    (TIMERA_OC_PERIODMATCH_OUTPUT_HIGH == (x))  ||                             \
    (TIMERA_OC_PERIODMATCH_OUTPUT_HOLD == (x))  ||                             \
    (TIMERA_OC_PERIODMATCH_OUTPUT_INVERTED == (x)))

#define IS_TIMERA_OC_FORCE_OUTPUT_POLARITY(x)                                  \
(   (TIMERA_OC_FORCE_OUTPUT_LOW == (x))         ||                             \
    (TIMERA_OC_FORCE_OUTPUT_HIGH == (x))        ||                             \
    (TIMERA_OC_FORCE_OUTPUT_INVALID == (x)))

#define IS_TIMERA_OC_CACHE_STATE(x)                                            \
(   (TIMERA_OC_CACHE_ENABLE == (x))             ||                             \
    (TIMERA_OC_CACHE_DISABLE == (x)))

#define IS_TIMERA_OC_CACHE_TRANS_CONDITION(x)                                  \
(   (!((x) & ((uint16_t)(~TIMERA_BCONR_BSE_MASK))))         ||                 \
    ((x) == TIMERA_OC_CACHE_TRANSMIT_INVALID))

#define IS_TIMERA_IC_PWM_PORT_FILTER_STATE(x)                                  \
(   (TIMERA_IC_PWM_FILTER_ENABLE == (x))        ||                             \
    (TIMERA_IC_PWM_FILTER_DISABLE == (x)))

#define IS_TIMERA_IC_PWM_PORT_FILTER_CLKDIV(x)                                 \
(   (TIMERA_IC_PWM_CLKDIV_DIV1 == (x))          ||                             \
    (TIMERA_IC_PWM_CLKDIV_DIV4 == (x))          ||                             \
    (TIMERA_IC_PWM_CLKDIV_DIV16 == (x))         ||                             \
    (TIMERA_IC_PWM_CLKDIV_DIV64 == (x)))

#define IS_TIMERA_IC_CAPTURE_CONDITION(x)                                      \
(   (!((x) & ((uint16_t)(~TIMERA_CCONR_HICP_MASK))))        ||                 \
    ((x) == TIMERA_IC_INVALID))

#define IS_TIMERA_GET_FLAG(x)                                                  \
(   (TIMERA_FLAG_OVF == (x))                    ||                             \
    (TIMERA_FLAG_UDF == (x))                    ||                             \
    (TIMERA_FLAG_CMP1 == (x))                   ||                             \
    (TIMERA_FLAG_CMP2 == (x)))

#define IS_TIMERA_CLEAR_FLAG(x)                                                \
(   (!((x) & ((uint16_t)(~TIMERA_FALG_MASK))))              &&                 \
    (0u != (x)))

#define IS_TIMERA_INT(x)                                                       \
(   (!((x) & ((uint16_t)(~TIMERA_INT_MASK))))               &&                 \
    (0u != (x)))

#define IS_TIMERA_EVENT(x)                                                     \
(   (!((x) & ((uint16_t)(~TIMERA_EVENT_MASK))))             &&                 \
    (0u != (x)))

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup TIMERA_Global_Functions TimerA Global Functions
 * @{
 */

/**
 * @brief  De-Initialize TimerA unit base function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 *           - ErrorInvalidParameter: TMRAx is invalid instance
 */
en_result_t TIMERA_DeInit(M0P_TMRA_TypeDef *TMRAx)
{
    uint8_t u8Cnt;
    __IO uint16_t *TMRA_CMPAR;
    __IO uint16_t *TMRA_BCONR;
    __IO uint16_t *TMRA_CCONR;
    __IO uint16_t *TMRA_PCONR;
    en_result_t enRet = Ok;

    /* Check TMRAx instance */
    if (!IS_TIMERA_INSTANCE(TMRAx))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configures the registers to reset value. */
        WRITE_REG16(TMRAx->BCSTR, 0x0002u);
        WRITE_REG16(TMRAx->ICONR, 0x0000u);
        WRITE_REG16(TMRAx->ECONR, 0x0000u);
        WRITE_REG16(TMRAx->CNTER, 0x0000u);
        WRITE_REG16(TMRAx->PERAR, 0xFFFFu);
        WRITE_REG16(TMRAx->FCONR, 0x0000u);
        WRITE_REG16(TMRAx->STFLR, 0x0000u);
        WRITE_REG16(TMRAx->HCONR, 0x0000u);
        WRITE_REG16(TMRAx->HCUPR, 0x0000u);
        WRITE_REG16(TMRAx->HCDOR, 0x0000u);

        for (u8Cnt = 0u; u8Cnt < TIMERA_PWM_CHANNEL_NUMBER; u8Cnt++)
        {
            TMRA_CMPAR = (__IO uint16_t *)TMRA_CMPARx(TMRAx, u8Cnt);
            WRITE_REG16(*TMRA_CMPAR, 0xFFFFu);
            TMRA_CCONR = (__IO uint16_t *)TMRA_CCONRx(TMRAx, u8Cnt);
            WRITE_REG16(*TMRA_CCONR, 0x0000u);
            TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Cnt);
            WRITE_REG16(*TMRA_PCONR, 0x0000u);

            if ((u8Cnt % 2u) == 0u)
            {
                TMRA_BCONR = (__IO uint16_t *)TMRA_BCONRx(TMRAx, u8Cnt);
                WRITE_REG16(*TMRA_BCONR, 0x0000u);
            }
        }
        WRITE_REG32(M0P_AOS->TMRA_HTSSR, TIMERA_TRIG_SRC_MASK);
    }

    return enRet;
}

/**
 * @brief  Initialize TimerA base function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] pstcInit                Pointer to a @ref stc_timera_init_t structure.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: TMRAx is invalid instance or pstcInit == NULL
 */
en_result_t TIMERA_Init(M0P_TMRA_TypeDef *TMRAx,
                        const stc_timera_init_t *pstcInit)
{
    en_result_t enRet = Ok;

    /* Check TMRAx instance and pstcInit */
    if ((!IS_TIMERA_INSTANCE(TMRAx)) || (NULL == pstcInit))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(0u != pstcInit->u16PeriodVal);
        DDL_ASSERT(IS_TIMERA_COUNT_MODE(pstcInit->u16CountMode));
        DDL_ASSERT(IS_TIMERA_COUNT_DIRECTION(pstcInit->u16CountDir));
        DDL_ASSERT(IS_TIMERA_CLK_DIV(pstcInit->u16ClkDiv));
        DDL_ASSERT(IS_TIMERA_OVERFLOW_STATE(pstcInit->u16OverflowAction));
        DDL_ASSERT(IS_TIMERA_HWSTART_CONDITION(pstcInit->u16HwStartCondition));
        DDL_ASSERT(IS_TIMERA_HWSTOP_CONDITION(pstcInit->u16HwStopCondition));
        DDL_ASSERT(IS_TIMERA_HWCLEAR_CONDITION(pstcInit->u16HwClearCondition));
        DDL_ASSERT(IS_TIMERA_HWUP_CONDITION(pstcInit->u16HwUpCondition));
        DDL_ASSERT(IS_TIMERA_HWDOWN_CONDITION(pstcInit->u16HwDownCondition));
        DDL_ASSERT(IS_TIMERA_TRIG_PORT_FILTER_STATE(pstcInit->u16TrigFilterState));
        DDL_ASSERT(IS_TIMERA_TRIG_PORT_FILTER_CLKDIV(pstcInit->u16TrigFilterClkDiv));
        DDL_ASSERT(IS_TIMERA_CLKA_PORT_FILTER_STATE(pstcInit->u16ClkAFilterState));
        DDL_ASSERT(IS_TIMERA_CLKA_PORT_FILTER_CLKDIV(pstcInit->u16ClkAFilterClkDiv));
        DDL_ASSERT(IS_TIMERA_CLKB_PORT_FILTER_STATE(pstcInit->u16ClkBFilterState));
        DDL_ASSERT(IS_TIMERA_CLKB_PORT_FILTER_CLKDIV(pstcInit->u16ClkBFilterClkDiv));

        WRITE_REG16(TMRAx->CNTER, 0u);
        WRITE_REG16(TMRAx->PERAR, pstcInit->u16PeriodVal);
        MODIFY_REG16(TMRAx->BCSTR,
                     (TMRA_BCSTR_DIR | TMRA_BCSTR_MODE | TMRA_BCSTR_OVSTP | TMRA_BCSTR_CKDIV),
                     (pstcInit->u16CountMode | pstcInit->u16CountDir |
                      pstcInit->u16OverflowAction | pstcInit->u16ClkDiv));

        /* Set External port filter */
        WRITE_REG16(TMRAx->FCONR,
                    (pstcInit->u16TrigFilterState | pstcInit->u16TrigFilterClkDiv |
                     pstcInit->u16ClkAFilterState | pstcInit->u16ClkAFilterClkDiv |
                     pstcInit->u16ClkBFilterState | pstcInit->u16ClkBFilterClkDiv));

        /* Set hardware trigger event */
        WRITE_REG16(TMRAx->HCONR,
                    (pstcInit->u16HwStartCondition | pstcInit->u16HwStopCondition |
                     pstcInit->u16HwClearCondition));
        WRITE_REG16(TMRAx->HCUPR, pstcInit->u16HwUpCondition);
        WRITE_REG16(TMRAx->HCDOR, pstcInit->u16HwDownCondition);
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_timera_init_t to default values.
 * @param  [out] pstcInit               Pointer to a @ref stc_timera_init_t structure.
 * @retval An en_result_t enumeration value:
 *           - Ok: TMRAx structure Initialize success
 *           - ErrorInvalidParameter: pstcInit == NULL
 */
en_result_t TIMERA_StructInit(stc_timera_init_t *pstcInit)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    if (NULL == pstcInit)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        pstcInit->u16PeriodVal = 0xFFFFu;
        pstcInit->u16CountDir = TIMERA_COUNT_UP;
        pstcInit->u16CountMode = TIMERA_SAWTOOTH_WAVE;
        pstcInit->u16ClkDiv = TIMERA_CLKDIV_DIV1;
        pstcInit->u16OverflowAction = TIMERA_OVERFLOW_COUNT;
        pstcInit->u16HwStartCondition = TIMERA_HWSTART_INVALID;
        pstcInit->u16HwStopCondition = TIMERA_HWSTOP_INVALID;
        pstcInit->u16HwClearCondition = TIMERA_HWCLEAR_INVALID;
        pstcInit->u16HwUpCondition = TIMERA_HWUP_INVALID;
        pstcInit->u16HwDownCondition = TIMERA_HWDOWN_INVALID;
        pstcInit->u16TrigFilterState = TIMERA_TRIG_FILTER_DISABLE;
        pstcInit->u16TrigFilterClkDiv = TIMERA_TRIG_CLKDIV_DIV1;
        pstcInit->u16ClkAFilterState = TIMERA_CLKA_FILTER_DISABLE;
        pstcInit->u16ClkAFilterClkDiv = TIMERA_CLKA_CLKDIV_DIV1;
        pstcInit->u16ClkBFilterState = TIMERA_CLKB_FILTER_DISABLE;
        pstcInit->u16ClkBFilterClkDiv = TIMERA_CLKB_CLKDIV_DIV1;
    }

    return enRet;
}

/**
 * @brief  Set the TimerA counter value.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Value                Counter value (between 0 and 0xFFFF)
 * @retval None
 */
void TIMERA_SetCounter(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Value)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));

    WRITE_REG16(TMRAx->CNTER, u16Value);
}

/**
 * @brief  Get the TimerA counter value.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @retval uint16_t                     Counter value (between 0 and 0xFFFF)
 */
uint16_t TIMERA_GetCounter(M0P_TMRA_TypeDef *TMRAx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));

    return READ_REG16(TMRAx->CNTER);
}

/**
 * @brief  Set TimerA period register value.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Value                Period value (between 0 and 0xFFFF)
 * @retval None
 */
void TIMERA_SetPeriod(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Value)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));

    WRITE_REG16(TMRAx->PERAR, u16Value);
}

/**
 * @brief  Get TimerA period register value.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @retval uint16_t                     Period value (between 0 and 0xFFFF)
 */
uint16_t TIMERA_GetPeriod(M0P_TMRA_TypeDef *TMRAx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));

    return READ_REG16(TMRAx->PERAR);
}

/**
 * @brief  Set TimerA clock division.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Div                  TimerA clock division
 *         This parameter can be one of the following values:
 *           @arg TIMERA_CLKDIV_DIV1:   HCLK
 *           @arg TIMERA_CLKDIV_DIV2:   HCLK/2
 *           @arg TIMERA_CLKDIV_DIV4:   HCLK/4
 *           @arg TIMERA_CLKDIV_DIV8:   HCLK/8
 *           @arg TIMERA_CLKDIV_DIV16:  HCLK/16
 *           @arg TIMERA_CLKDIV_DIV32:  HCLK/32
 *           @arg TIMERA_CLKDIV_DIV64:  HCLK/64
 *           @arg TIMERA_CLKDIV_DIV128: HCLK/128
 *           @arg TIMERA_CLKDIV_DIV256: HCLK/256
 *           @arg TIMERA_CLKDIV_DIV512: HCLK/512
 *           @arg TIMERA_CLKDIV_DIV1024:HCLK/1024
 * @retval None
 */
void TIMERA_SetClkDiv(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Div)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CLK_DIV(u16Div));

    MODIFY_REG16(TMRAx->BCSTR, TMRA_BCSTR_CKDIV, u16Div);
}

/**
 * @brief  Enable or disable TimerA compare match or trigger capture event output.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16EvtSource            TimerA event source
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_EVENT_CMP1: Chanel 1 compare match or trigger capture
 *           @arg TIMERA_EVENT_CMP2: Chanel 2 compare match or trigger capture
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERA_EventCmd(M0P_TMRA_TypeDef *TMRAx, uint16_t u16EvtSource,
                     en_functional_state_t enNewSta)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_EVENT(u16EvtSource));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    if (Enable == enNewSta)
    {
        SET_REG16_BIT(TMRAx->ECONR, u16EvtSource);
    }
    else
    {
        CLEAR_REG16_BIT(TMRAx->ECONR, u16EvtSource);
    }
}

/**
 * @brief  Enable or disable TimerA.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERA_Cmd(M0P_TMRA_TypeDef *TMRAx, en_functional_state_t enNewSta)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    if (Enable == enNewSta)
    {
        SET_REG16_BIT(TMRAx->BCSTR, TMRA_BCSTR_START);
    }
    else
    {
        CLEAR_REG16_BIT(TMRAx->BCSTR, TMRA_BCSTR_START);
    }
}

/**
 * @brief  Set TimerA TRIG Port noise filter clock division.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Div                  TimerA TRIG Port noise filter clock division
 *         This parameter can be one of the following values:
 *           @arg TIMERA_TRIG_CLKDIV_DIV1:   HCLK
 *           @arg TIMERA_TRIG_CLKDIV_DIV4:   HCLK/4
 *           @arg TIMERA_TRIG_CLKDIV_DIV16:  HCLK/16
 *           @arg TIMERA_TRIG_CLKDIV_DIV64:  HCLK/64
 * @retval None
 */
void TIMERA_SetTrigFilterClkDiv(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Div)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_TRIG_PORT_FILTER_CLKDIV(u16Div));

    MODIFY_REG16(TMRAx->FCONR, TMRA_FCONR_NOFICKTG, u16Div);
}

/**
 * @brief  Enable or disable TimerA TRIG Port noise filter function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERA_TrigFilterCmd(M0P_TMRA_TypeDef *TMRAx, en_functional_state_t enNewSta)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    if (Enable == enNewSta)
    {
        SET_REG16_BIT(TMRAx->FCONR, TMRA_FCONR_NOFIENTG);
    }
    else
    {
        CLEAR_REG16_BIT(TMRAx->FCONR, TMRA_FCONR_NOFIENTG);
    }
}

/**
 * @brief  Set TimerA CLKA Port noise filter clock division.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Div                  TimerA CLKA Port noise filter clock division
 *         This parameter can be one of the following values:
 *           @arg TIMERA_CLKA_CLKDIV_DIV1:   HCLK
 *           @arg TIMERA_CLKA_CLKDIV_DIV4:   HCLK/4
 *           @arg TIMERA_CLKA_CLKDIV_DIV16:  HCLK/16
 *           @arg TIMERA_CLKA_CLKDIV_DIV64:  HCLK/64
 * @retval None
 */
void TIMERA_SetClkAFilterClkDiv(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Div)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CLKA_PORT_FILTER_CLKDIV(u16Div));

    MODIFY_REG16(TMRAx->FCONR, TMRA_FCONR_NOFICKCA, u16Div);
}

/**
 * @brief  Enable or disable TimerA CLKA Port noise filter function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERA_ClkAFilterCmd(M0P_TMRA_TypeDef *TMRAx, en_functional_state_t enNewSta)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    if (Enable == enNewSta)
    {
        SET_REG16_BIT(TMRAx->FCONR, TMRA_FCONR_NOFIENCA);
    }
    else
    {
        CLEAR_REG16_BIT(TMRAx->FCONR, TMRA_FCONR_NOFIENCA);
    }
}

/**
 * @brief  Set TimerA CLKB Port noise filter clock division.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Div                  TimerA CLKB Port noise filter clock division
 *         This parameter can be one of the following values:
 *           @arg TIMERA_CLKB_CLKDIV_DIV1:   HCLK
 *           @arg TIMERA_CLKB_CLKDIV_DIV4:   HCLK/4
 *           @arg TIMERA_CLKB_CLKDIV_DIV16:  HCLK/16
 *           @arg TIMERA_CLKB_CLKDIV_DIV64:  HCLK/64
 * @retval None
 */
void TIMERA_SetClkBFilterClkDiv(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Div)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CLKB_PORT_FILTER_CLKDIV(u16Div));

    MODIFY_REG16(TMRAx->FCONR, TMRA_FCONR_NOFICKCB, u16Div);
}

/**
 * @brief  Enable or disable TimerA CLKB Port noise filter function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERA_ClkBFilterCmd(M0P_TMRA_TypeDef *TMRAx, en_functional_state_t enNewSta)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));


    if (Enable == enNewSta)
    {
        SET_REG16_BIT(TMRAx->FCONR, TMRA_FCONR_NOFIENCB);
    }
    else
    {
        CLEAR_REG16_BIT(TMRAx->FCONR, TMRA_FCONR_NOFIENCB);
    }
}

/**
 * @brief  Set TimerA hardware trigger counter Start/Stop/Clear condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Condition            TimerA hardware start counter condition
 *         This parameter can be one or any combination of the following values:
 *           @arg @ref TIMERA_Hardware_Start_Count_Condition
 *           @arg @ref TIMERA_Hardware_Stop_Count_Condition
 *           @arg @ref TIMERA_Hardware_Clear_Count_Condition
 * @retval  None
 */
void TIMERA_SetHwTriggerCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_HWTRIGGER_CONDITION(u16Condition));

    WRITE_REG16(TMRAx->HCONR, u16Condition);
}

/**
 * @brief  Get TimerA hardware trigger counter Start/Stop/Clear condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @retval Returned value can be one or any combination of the following values:
 *           @arg @ref TIMERA_Hardware_Start_Count_Condition
 *           @arg @ref TIMERA_Hardware_Stop_Count_Condition
 *           @arg @ref TIMERA_Hardware_Clear_Count_Condition
 */
uint16_t TIMERA_GetHwTriggerCondition(M0P_TMRA_TypeDef *TMRAx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));

    return READ_REG16(TMRAx->HCONR);
}

/**
 * @brief  Set TimerA hardware start counter condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Condition            TimerA hardware start counter condition
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_HWSTART_INVALID:       Hardware start is invalid
 *           @arg TIMERA_HWSTART_TRIG_RISING:   Hardware start counter by rising edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWSTART_TRIG_FALLING:  Hardware start counter by falling edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWSTART_SPECIFY_EVT:   Hardware start counter by the timer TMRA_HTSSR specified event
 * @retval  None
 */
void TIMERA_SetHwStartCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_HWSTART_CONDITION(u16Condition));

    MODIFY_REG16(TMRAx->HCONR, TIMERA_HCONR_HSTA_MASK, u16Condition);
}

/**
 * @brief  Set TimerA hardware stop counter condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Condition            TimerA hardware stop counter condition
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_HWSTOP_INVALID:        Hardware stop is invalid
 *           @arg TIMERA_HWSTOP_TRIG_RISING:    Hardware stop counter by rising edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWSTOP_TRIG_FALLING:   Hardware stop counter by falling edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWSTOP_SPECIFY_EVT:    Hardware stop counter by the timer TMRA_HTSSR specified event
 * @retval  None
 */
void TIMERA_SetHwStopCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_HWSTOP_CONDITION(u16Condition));

    MODIFY_REG16(TMRAx->HCONR, TIMERA_HCONR_HSTP_MASK, u16Condition);
}

/**
 * @brief  Set TimerA hardware clear counter condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Condition            TimerA hardware clear counter condition
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_HWCLEAR_INVALID:       Hardware clear is invalid
 *           @arg TIMERA_HWCLEAR_TRIG_RISING:   Hardware clear counter by rising edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWCLEAR_TRIG_FALLING:  Hardware clear counter by falling edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWCLEAR_SPECIFY_EVT:   Hardware clear counter by the timer TMRA_HTSSR specified event
 * @retval  None
 */
void TIMERA_SetHwClearCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_HWCLEAR_CONDITION(u16Condition));

    MODIFY_REG16(TMRAx->HCONR, TIMERA_HCONR_HCLE_MASK, u16Condition);
}

/**
 * @brief  Set TimerA hardware up counter condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Condition            TimerA hardware up counter condition
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_HWUP_INVALID:                  Hardware count up is invalid
 *           @arg TIMERA_HWUP_CLKA_LOW_CLKB_RISING:     Hardware count up by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level
 *           @arg TIMERA_HWUP_CLKA_LOW_CLKB_FALLING:    Hardware count up by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level
 *           @arg TIMERA_HWUP_CLKA_HIGH_CLKB_RISING:    Hardware count up by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level
 *           @arg TIMERA_HWUP_CLKA_HIGH_CLKB_FALLING:   Hardware count up by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level
 *           @arg TIMERA_HWUP_CLKB_LOW_CLKA_RISING:     Hardware count up by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level
 *           @arg TIMERA_HWUP_CLKB_LOW_CLKA_FALLING:    Hardware count up by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level
 *           @arg TIMERA_HWUP_CLKB_HIGH_CLKA_RISING:    Hardware count up by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level
 *           @arg TIMERA_HWUP_CLKB_HIGH_CLKA_FALLING:   Hardware count up by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level
 *           @arg TIMERA_HWUP_TRIG_RISING:              Hardware count up by rising edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWUP_TRIG_FALLING:             Hardware count up by falling edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWUP_SPECIFY_EVT:              Hardware count up by the timer TMRA_HTSSR specified event
 * @retval  None
 */
void TIMERA_SetHwUpCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_HWUP_CONDITION(u16Condition));

    WRITE_REG16(TMRAx->HCUPR, u16Condition);
}

/**
 * @brief  Get TimerA hardware up counter condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @retval Returned value can be one or any combination of the following values:
 *           @arg TIMERA_HWUP_INVALID:                  Hardware count up is invalid
 *           @arg TIMERA_HWUP_CLKA_LOW_CLKB_RISING:     Hardware count up by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level
 *           @arg TIMERA_HWUP_CLKA_LOW_CLKB_FALLING:    Hardware count up by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level
 *           @arg TIMERA_HWUP_CLKA_HIGH_CLKB_RISING:    Hardware count up by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level
 *           @arg TIMERA_HWUP_CLKA_HIGH_CLKB_FALLING:   Hardware count up by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level
 *           @arg TIMERA_HWUP_CLKB_LOW_CLKA_RISING:     Hardware count up by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level
 *           @arg TIMERA_HWUP_CLKB_LOW_CLKA_FALLING:    Hardware count up by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level
 *           @arg TIMERA_HWUP_CLKB_HIGH_CLKA_RISING:    Hardware count up by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level
 *           @arg TIMERA_HWUP_CLKB_HIGH_CLKA_FALLING:   Hardware count up by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level
 *           @arg TIMERA_HWUP_TRIG_RISING:              Hardware count up by rising edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWUP_TRIG_FALLING:             Hardware count up by falling edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWUP_SPECIFY_EVT:              Hardware count up by the timer TMRA_HTSSR specified event
 */
uint16_t TIMERA_GetHwUpCondition(M0P_TMRA_TypeDef *TMRAx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));

    return READ_REG16(TMRAx->HCUPR);
}

/**
 * @brief  Set TimerA hardware down counter condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Condition            TimerA hardware down counter condition
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_HWDOWN_INVALID:                  Hardware count down is invalid
 *           @arg TIMERA_HWDOWN_CLKA_LOW_CLKB_RISING:     Hardware count down by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level
 *           @arg TIMERA_HWDOWN_CLKA_LOW_CLKB_FALLING:    Hardware count down by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level
 *           @arg TIMERA_HWDOWN_CLKA_HIGH_CLKB_RISING:    Hardware count down by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level
 *           @arg TIMERA_HWDOWN_CLKA_HIGH_CLKB_FALLING:   Hardware count down by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level
 *           @arg TIMERA_HWDOWN_CLKB_LOW_CLKA_RISING:     Hardware count down by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level
 *           @arg TIMERA_HWDOWN_CLKB_LOW_CLKA_FALLING:    Hardware count down by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level
 *           @arg TIMERA_HWDOWN_CLKB_HIGH_CLKA_RISING:    Hardware count down by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level
 *           @arg TIMERA_HWDOWN_CLKB_HIGH_CLKA_FALLING:   Hardware count down by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level
 *           @arg TIMERA_HWDOWN_TRIG_RISING:              Hardware count down by rising edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWDOWN_TRIG_FALLING:             Hardware count down by falling edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWDOWN_SPECIFY_EVT:              Hardware count down by the timer TMRA_HTSSR specified event
 * @retval  None
 */
void TIMERA_SetHwDownCondition(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_HWDOWN_CONDITION(u16Condition));

    WRITE_REG16(TMRAx->HCDOR, u16Condition);
}

/**
 * @brief  Get TimerA hardware down counter condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @retval Returned value can be one or any combination of the following values:
 *           @arg TIMERA_HWDOWN_INVALID:                  Hardware count down is invalid
 *           @arg TIMERA_HWDOWN_CLKA_LOW_CLKB_RISING:     Hardware count down by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level
 *           @arg TIMERA_HWDOWN_CLKA_LOW_CLKB_FALLING:    Hardware count down by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is low level
 *           @arg TIMERA_HWDOWN_CLKA_HIGH_CLKB_RISING:    Hardware count down by rising edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level
 *           @arg TIMERA_HWDOWN_CLKA_HIGH_CLKB_FALLING:   Hardware count down by falling edge on timer input TIMA_<t>_CLKB when TIMA_<t>_CLKA is high level
 *           @arg TIMERA_HWDOWN_CLKB_LOW_CLKA_RISING:     Hardware count down by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level
 *           @arg TIMERA_HWDOWN_CLKB_LOW_CLKA_FALLING:    Hardware count down by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is low level
 *           @arg TIMERA_HWDOWN_CLKB_HIGH_CLKA_RISING:    Hardware count down by rising edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level
 *           @arg TIMERA_HWDOWN_CLKB_HIGH_CLKA_FALLING:   Hardware count down by falling edge on timer input TIMA_<t>_CLKA when TIMA_<t>_CLKB is high level
 *           @arg TIMERA_HWDOWN_TRIG_RISING:              Hardware count down by rising edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWDOWN_TRIG_FALLING:             Hardware count down by falling edge on timer input TIMA_<t>_TRIG
 *           @arg TIMERA_HWDOWN_SPECIFY_EVT:              Hardware count down by the timer TMRA_HTSSR specified event
 */
uint16_t TIMERA_GetHwDownCondition(M0P_TMRA_TypeDef *TMRAx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));

    return READ_REG16(TMRAx->HCDOR);
}

/**
 * @brief  De-Initialize TimerA unit input capture.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA input capture channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 *           - ErrorInvalidParameter: TMRAx is invalid instance
 */
en_result_t TIMERA_IC_DeInit(M0P_TMRA_TypeDef *TMRAx,
                             uint8_t u8Channel)
{
    __IO uint16_t *TMRA_CMPAR;
    __IO uint16_t *TMRA_BCONR;
    __IO uint16_t *TMRA_CCONR;
    en_result_t enRet = Ok;

    /* Check TMRAx instance */
    if (!IS_TIMERA_INSTANCE(TMRAx))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));

        /* Configures the registers to reset value. */
        TMRA_CMPAR = (__IO uint16_t *)TMRA_CMPARx(TMRAx, u8Channel);
        WRITE_REG16(*TMRA_CMPAR, 0xFFFFu);
        TMRA_CCONR = (__IO uint16_t *)TMRA_CCONRx(TMRAx, u8Channel);
        WRITE_REG16(*TMRA_CCONR, TIMERA_INPUT_CAPTURE);

        if ((u8Channel % 2u) == 0u)
        {
            TMRA_BCONR = (__IO uint16_t *)TMRA_BCONRx(TMRAx, u8Channel);
            WRITE_REG16(*TMRA_BCONR, 0x0000u);
        }
    }

    return enRet;
}

/**
 * @brief  Initialize TimerA input capture function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA input capture channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] pstcIcInit              Pointer to a @ref stc_timera_ic_init_t structure.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: TMRAx is invalid instance or pstcIcInit == NULL
 */
en_result_t TIMERA_IC_Init(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                           const stc_timera_ic_init_t *pstcIcInit)
{
    __IO uint16_t *TMRA_CCONR;
    en_result_t enRet = Ok;

    /* Check TMRAx instance and pstcIcInit */
    if ((!IS_TIMERA_INSTANCE(TMRAx)) || (NULL == pstcIcInit))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
        DDL_ASSERT(IS_TIMERA_IC_CAPTURE_CONDITION(pstcIcInit->u16CaptureCondition));
        DDL_ASSERT(IS_TIMERA_IC_PWM_PORT_FILTER_STATE(pstcIcInit->u16PwmFilterState));
        DDL_ASSERT(IS_TIMERA_IC_PWM_PORT_FILTER_CLKDIV(pstcIcInit->u16PwmFilterClkDiv));

        TMRA_CCONR = (__IO uint16_t *)TMRA_CCONRx(TMRAx, u8Channel);
        MODIFY_REG16(*TMRA_CCONR,
                     (TMRA_CCONR1_NOFIENCP | TMRA_CCONR1_NOFICKCP |
                      TMRA_CCONR1_CAPMD | TIMERA_CCONR_HICP_MASK),
                     (TIMERA_INPUT_CAPTURE | pstcIcInit->u16CaptureCondition |
                      pstcIcInit->u16PwmFilterState | pstcIcInit->u16PwmFilterClkDiv));
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_timera_ic_init_t to default values.
 * @param  [out] pstcIcInit             Pointer to a @ref stc_timera_ic_init_t structure.
 * @retval None
 */
en_result_t TIMERA_IC_StructInit(stc_timera_ic_init_t *pstcIcInit)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    if (NULL == pstcIcInit)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        pstcIcInit->u16PwmFilterState = TIMERA_IC_PWM_FILTER_DISABLE;
        pstcIcInit->u16PwmFilterClkDiv = TIMERA_IC_PWM_CLKDIV_DIV1;
        pstcIcInit->u16CaptureCondition = TIMERA_IC_INVALID;
    }

    return enRet;
}

/**
 * @brief  Get TimerA capture register value.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA input capture channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @retval Compare value (between 0 and 0xFFFF)
 */
uint16_t TIMERA_IC_GetCapture(M0P_TMRA_TypeDef *TMRAx,
                              uint8_t u8Channel)
{
    __IO uint16_t *TMRA_CMPAR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));

    /* Configures the registers to reset value. */
    TMRA_CMPAR = (__IO uint16_t *)TMRA_CMPARx(TMRAx, u8Channel);
    return READ_REG16(*TMRA_CMPAR);
}

/**
 * @brief  Set TimerA input capture condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA input capture channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] u16Condition            TimerA input capture condition
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_IC_PWM_RISING:    Capture triggered by rising edge on timer input TIMA_<t>_PWMn
 *           @arg TIMERA_IC_PWM_FALLING:    Capture triggered by falling edge on timer input TIMA_<t>_PWMn
 *           @arg TIMERA_IC_SPECIFY_EVT:    Capture triggered by the timer TMRA_HTSSR specified event
 *           @arg TIMERA_IC_INVALID:        Don't occur capture action
 * @retval None
 */
void TIMERA_IC_SetCaptureCondition(M0P_TMRA_TypeDef *TMRAx,
                                   uint8_t u8Channel,
                                   uint16_t u16Condition)
{
    __IO uint16_t *TMRA_CCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_TIMERA_IC_CAPTURE_CONDITION(u16Condition));

    TMRA_CCONR = (__IO uint16_t *)TMRA_CCONRx(TMRAx, u8Channel);
    MODIFY_REG16(*TMRA_CCONR, TIMERA_CCONR_HICP_MASK, u16Condition);
}

/**
 * @brief  Enable or Disable TimerA input capture noise filter function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA input capture channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERA_IC_FilterCmd(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                         en_functional_state_t enNewSta)
{
    __IO uint16_t *TMRA_CCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    TMRA_CCONR = (__IO uint16_t *)TMRA_CCONRx(TMRAx, u8Channel);
    if (Enable == enNewSta)
    {
        SET_REG16_BIT(*TMRA_CCONR, TMRA_CCONR1_NOFIENCP);
    }
    else
    {
        CLEAR_REG16_BIT(*TMRA_CCONR, TMRA_CCONR1_NOFIENCP);
    }
}

/**
 * @brief  Set TimerA PWM Port input capture noise filter clock division.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA input capture channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] u16Div                  TimerA input capture noise filter clock division
 *         This parameter can be one of the following values:
 *           @arg TIMERA_IC_PWM_CLKDIV_DIV1:    HCLK
 *           @arg TIMERA_IC_PWM_CLKDIV_DIV4:    HCLK/4
 *           @arg TIMERA_IC_PWM_CLKDIV_DIV16:   HCLK/16
 *           @arg TIMERA_IC_PWM_CLKDIV_DIV64:   HCLK/64
 * @retval None
 */
void TIMERA_IC_SetFilterClkDiv(M0P_TMRA_TypeDef *TMRAx,
                               uint8_t u8Channel, uint16_t u16Div)
{
    __IO uint16_t *TMRA_CCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_TIMERA_IC_PWM_PORT_FILTER_CLKDIV(u16Div));

    TMRA_CCONR = (__IO uint16_t *)TMRA_CCONRx(TMRAx, u8Channel);
    MODIFY_REG16(*TMRA_CCONR, TMRA_CCONR1_NOFICKCP, u16Div);
}

/**
 * @brief  De-Initialize TimerA unit output compare.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 *           - ErrorInvalidParameter: TMRAx is invalid instance
 */
en_result_t TIMERA_OC_DeInit(M0P_TMRA_TypeDef *TMRAx,
                             uint8_t u8Channel)
{
    __IO uint16_t *TMRA_CMPAR;
    __IO uint16_t *TMRA_BCONR;
    __IO uint16_t *TMRA_CCONR;
    __IO uint16_t *TMRA_PCONR;
    en_result_t enRet = Ok;

    /* Check TMRAx instance */
    if (!IS_TIMERA_INSTANCE(TMRAx))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));

        /* Configures the registers to reset value. */
        TMRA_CMPAR = (__IO uint16_t *)TMRA_CMPARx(TMRAx, u8Channel);
        WRITE_REG16(*TMRA_CMPAR, 0xFFFFu);
        TMRA_CCONR = (__IO uint16_t *)TMRA_CCONRx(TMRAx, u8Channel);
        WRITE_REG16(*TMRA_CCONR, TIMERA_OUTPUT_COMPARE);
        TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Channel);
        WRITE_REG16(*TMRA_PCONR, 0x0000u);

        if ((u8Channel % 2u) == 0u)
        {
            TMRA_BCONR = (__IO uint16_t *)TMRA_BCONRx(TMRAx, u8Channel);
            WRITE_REG16(*TMRA_BCONR, 0x0000u);
        }
    }

    return enRet;
}

/**
 * @brief  Initialize TimerA output compare function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] pstcOcInit              Pointer to a @ref stc_timera_oc_init_t structure.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: TMRAx is invalid instance or pstcOcInit == NULL
 */
en_result_t TIMERA_OC_Init(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                           const stc_timera_oc_init_t *pstcOcInit)
{
    __IO uint16_t *TMRA_CMPAR;
    __IO uint16_t *TMRA_BCONR;
    __IO uint16_t *TMRA_CCONR;
    __IO uint16_t *TMRA_PCONR;
    en_result_t enRet = Ok;

    /* Check TMRAx instance and pstcOcInit */
    if ((!IS_TIMERA_INSTANCE(TMRAx)) || (NULL == pstcOcInit))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
        DDL_ASSERT(IS_TIMERA_OC_FORCE_OUTPUT_POLARITY(pstcOcInit->u16PortOutputState));
        DDL_ASSERT(IS_TIMERA_OC_START_COUNT_OUTPUT_POLARITY(pstcOcInit->u16StartCountOutput));
        DDL_ASSERT(IS_TIMERA_OC_STOP_COUNT_OUTPUT_POLARITY(pstcOcInit->u16StopCountOutput));
        DDL_ASSERT(IS_TIMERA_OC_CMPMATCH_OUTPUT_POLARITY(pstcOcInit->u16CompareMatchOutput));
        DDL_ASSERT(IS_TIMERA_OC_PERIODMATCH_OUTPUT_POLARITY(pstcOcInit->u16PeriodMatchOutput));
        DDL_ASSERT(IS_TIMERA_OC_CACHE_STATE(pstcOcInit->u16CacheState));
        DDL_ASSERT(IS_TIMERA_OC_CACHE_TRANS_CONDITION(pstcOcInit->u16CacheTransmitCondition));

        TMRA_CMPAR = (__IO uint16_t *)TMRA_CMPARx(TMRAx, u8Channel);
        WRITE_REG16(*TMRA_CMPAR, pstcOcInit->u16CompareVal);
        /* Configure output compare */
        TMRA_CCONR = (__IO uint16_t *)TMRA_CCONRx(TMRAx, u8Channel);
        CLEAR_REG16_BIT(*TMRA_CCONR, TMRA_CCONR1_CAPMD);

        TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Channel);
        MODIFY_REG16(*TMRA_PCONR,
                     (TMRA_PCONR1_STAC | TMRA_PCONR1_STPC | TMRA_PCONR1_CMPC |
                      TMRA_PCONR1_PERC | TMRA_PCONR1_FORC),
                     (pstcOcInit->u16StartCountOutput | pstcOcInit->u16StopCountOutput |
                      pstcOcInit->u16CompareMatchOutput | pstcOcInit->u16PeriodMatchOutput |
                      pstcOcInit->u16PortOutputState));

        if ((u8Channel % 2u) == 0u)
        {
            TMRA_BCONR = (__IO uint16_t *)TMRA_BCONRx(TMRAx, u8Channel);
            WRITE_REG16(*TMRA_BCONR, (pstcOcInit->u16CacheState | pstcOcInit->u16CacheTransmitCondition));
        }
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_timera_oc_init_t to default values.
 * @param  [out] pstcOcInit             Pointer to a @ref stc_timera_oc_init_t structure (TMRAx unit output compare function configuration data structure).
 * @retval None
 */
en_result_t TIMERA_OC_StructInit(stc_timera_oc_init_t *pstcOcInit)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    if (NULL == pstcOcInit)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        pstcOcInit->u16CompareVal = 0xFFFFu;
        pstcOcInit->u16PortOutputState = TIMERA_OC_FORCE_OUTPUT_INVALID;
        pstcOcInit->u16StartCountOutput = TIMERA_OC_STARTCOUNT_OUTPUT_LOW;
        pstcOcInit->u16StopCountOutput = TIMERA_OC_STOPCOUNT_OUTPUT_LOW;
        pstcOcInit->u16CompareMatchOutput = TIMERA_OC_CMPMATCH_OUTPUT_LOW;
        pstcOcInit->u16PeriodMatchOutput = TIMERA_OC_PERIODMATCH_OUTPUT_LOW;
        pstcOcInit->u16CacheState = TIMERA_OC_CACHE_DISABLE;
        pstcOcInit->u16CacheTransmitCondition = TIMERA_OC_CACHE_TRANSMIT_INVALID;
    }

    return enRet;
}

/**
 * @brief  Set TimerA compare register value.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] u16Value Compare value (between 0 and 0xFFFF)
 * @retval None
 */
void TIMERA_OC_SetCompare(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                          uint16_t u16Value)
{
    __IO uint16_t *TMRA_CMPAR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));

    TMRA_CMPAR = (__IO uint16_t *)TMRA_CMPARx(TMRAx, u8Channel);
    WRITE_REG16(*TMRA_CMPAR, u16Value);
}

/**
 * @brief  Enable or Disable TimerA PWM output function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERA_OC_PwmCmd(M0P_TMRA_TypeDef *TMRAx, uint8_t u8Channel,
                      en_functional_state_t enNewSta)
{
    __IO uint16_t *TMRA_PCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Channel);
    if (Enable == enNewSta)
    {
        SET_REG16_BIT(*TMRA_PCONR, TMRA_PCONR1_OUTEN);
    }
    else
    {
        CLEAR_REG16_BIT(*TMRA_PCONR, TMRA_PCONR1_OUTEN);
    }
}

/**
 * @brief  Set TIMA_<t>_PWMn port output polarity when start count.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] u16Polarity             TimerA TIMA_<t>_PWMn port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERA_OC_STARTCOUNT_OUTPUT_LOW:  TIMA_<t>_PWMn output low level when TimerA start count
 *           @arg TIMERA_OC_STARTCOUNT_OUTPUT_HIGH: TIMA_<t>_PWMn output high level when TimerA start count
 *           @arg TIMERA_OC_STARTCOUNT_OUTPUT_HOLD: TIMA_<t>_PWMn output hold level when TimerA start count
 * @retval None
 */
void TIMERA_OC_SetStartCountOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                           uint8_t u8Channel,
                                           uint16_t u16Polarity)
{
    __IO uint16_t *TMRA_PCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_TIMERA_OC_START_COUNT_OUTPUT_POLARITY(u16Polarity));

    TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Channel);
    MODIFY_REG16(*TMRA_PCONR, TMRA_PCONR1_STAC, u16Polarity);
}

/**
 * @brief  Set TIMA_<t>_PWMn port output polarity for stop count.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] u16Polarity             TimerA TIMA_<t>_PWMn port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERA_OC_STOPCOUNT_OUTPUT_LOW:   TIMA_<t>_PWMn output low level when TimerA stop count
 *           @arg TIMERA_OC_STOPCOUNT_OUTPUT_HIGH:  TIMA_<t>_PWMn output high level when TimerA stop count
 *           @arg TIMERA_OC_STOPCOUNT_OUTPUT_HOLD:  TIMA_<t>_PWMn output hold level when TimerA stop count
 * @retval None
 */
void TIMERA_OC_SetStopCountOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                          uint8_t u8Channel,
                                          uint16_t u16Polarity)
{
    __IO uint16_t *TMRA_PCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_TIMERA_OC_STOP_COUNT_OUTPUT_POLARITY(u16Polarity));

    TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Channel);
    MODIFY_REG16(*TMRA_PCONR, TMRA_PCONR1_STPC, u16Polarity);
}

/**
 * @brief  Set TIMA_<t>_PWMn port output polarity for compare match.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] u16Polarity             TimerA TIMA_<t>_PWMn port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERA_OC_CMPMATCH_OUTPUT_LOW:        TIMA_<t>_PWMn output low level when TimerA comapre match
 *           @arg TIMERA_OC_CMPMATCH_OUTPUT_HIGH:       TIMA_<t>_PWMn output high level when TimerA comapre match
 *           @arg TIMERA_OC_CMPMATCH_OUTPUT_HOLD:       TIMA_<t>_PWMn output hold level when TimerA comapre match
 *           @arg TIMERA_OC_CMPMATCH_OUTPUT_INVERTED:   TIMA_<t>_PWMn output inverted level when TimerA comapre match
 * @retval None
 */
void TIMERA_OC_SetCmpMatchOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                         uint8_t u8Channel,
                                         uint16_t u16Polarity)
{
    __IO uint16_t *TMRA_PCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_TIMERA_OC_CMPMATCH_OUTPUT_POLARITY(u16Polarity));

    TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Channel);
    MODIFY_REG16(*TMRA_PCONR, TMRA_PCONR1_CMPC, u16Polarity);
}

/**
 * @brief  Set TIMA_<t>_PWMn port output polarity for period match.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] u16Polarity             TimerA TIMA_<t>_PWMn port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERA_OC_PERIODMATCH_OUTPUT_LOW:         TIMA_<t>_PWMn output low level when TimerA period match
 *           @arg TIMERA_OC_PERIODMATCH_OUTPUT_HIGH:        TIMA_<t>_PWMn output high level when TimerA period match
 *           @arg TIMERA_OC_PERIODMATCH_OUTPUT_HOLD:        TIMA_<t>_PWMn output hold level when TimerA period match
 *           @arg TIMERA_OC_PERIODMATCH_OUTPUT_INVERTED:    TIMA_<t>_PWMn output inverted level when TimerA period match
 * @retval None
 */
void TIMERA_OC_SetPeriodMatchOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                            uint8_t u8Channel,
                                            uint16_t u16Polarity)
{
    __IO uint16_t *TMRA_PCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_TIMERA_OC_PERIODMATCH_OUTPUT_POLARITY(u16Polarity));

    TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Channel);
    MODIFY_REG16(*TMRA_PCONR, TMRA_PCONR1_PERC, u16Polarity);
}

/**
 * @brief  Set TIMA_<t>_PWMn port output polarity for force.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
             @arg TIMERA_CHANNEL_CH2:   TimerA PWM Channel 2
 * @param  [in] u16Polarity             TimerA TIMA_<t>_PWMn port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERA_OC_FORCE_OUTPUT_LOW:       Force TIMA_<t>_PWMn output low level
 *           @arg TIMERA_OC_FORCE_OUTPUT_HIGH:      Force TIMA_<t>_PWMn output high level
 *           @arg TIMERA_OC_FORCE_OUTPUT_INVALID:   Force invalid
 * @retval None
 */
void TIMERA_OC_SetForceOutputPolarity(M0P_TMRA_TypeDef *TMRAx,
                                      uint8_t u8Channel,
                                      uint16_t u16Polarity)
{
    __IO uint16_t *TMRA_PCONR;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CHANNEL(u8Channel));
    DDL_ASSERT(IS_TIMERA_OC_FORCE_OUTPUT_POLARITY(u16Polarity));

    TMRA_PCONR = (__IO uint16_t *)TMRA_PCONRx(TMRAx, u8Channel);
    MODIFY_REG16(*TMRA_PCONR, TMRA_PCONR1_FORC, u16Polarity);
}

/**
 * @brief  Enable or Disable TimerA PWM output cache function.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval An en_result_t enumeration value:
 *           - Ok: Configure success
 *           - ErrorInvalidParameter: u8Channel is invalid
 */
en_result_t TIMERA_OC_CacheCmd(M0P_TMRA_TypeDef *TMRAx,
                               uint8_t u8Channel,
                               en_functional_state_t enNewSta)
{
    __IO uint16_t *TMRA_BCONRx;
    en_result_t enRet = Ok;

    /* Check channel validity */
    if ((u8Channel % 2u) != 0u)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
        DDL_ASSERT(IS_TIMERA_CACHE_CHANNEL(u8Channel));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        TMRA_BCONRx = (__IO uint16_t *)TMRA_BCONRx(TMRAx, u8Channel);
        if (Enable == enNewSta)
        {
            SET_REG16_BIT(*TMRA_BCONRx, TMRA_BCONR_BEN);
        }
        else
        {
            CLEAR_REG16_BIT(*TMRA_BCONRx, TMRA_BCONR_BEN);
        }
    }

    return enRet;
}

/**
 * @brief  Set TimerA PWM output cache transmit condition.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u8Channel               TimerA output compare channel
           This parameter can be one of the following values:
             @arg TIMERA_CHANNEL_CH1:   TimerA PWM Channel 1
 * @param  [in] u16Condition            TimerA input capture condition
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_OC_CACHE_TRANSMIT_CREST:   In Triangular wave crest transmit cache value
 *           @arg TIMERA_OC_CACHE_TRANSMIT_TROUGH:  In Triangular wave trough transmit cache value
 *           @arg TIMERA_OC_CACHE_TRANSMIT_INVALID: Invalid cache transmit
 * @retval An en_result_t enumeration value:
 *           - Ok: Configure success
 *           - ErrorInvalidParameter: u8Channel is invalid
 */
en_result_t TIMERA_OC_SetCacheTransmitCondition(M0P_TMRA_TypeDef *TMRAx,
                                                uint8_t u8Channel,
                                                uint16_t u16Condition)
{
    __IO uint16_t *TMRA_BCONRx;
    en_result_t enRet = Ok;

    /* Check channel validity */
    if ((u8Channel % 2u) != 0u)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
        DDL_ASSERT(IS_TIMERA_CACHE_CHANNEL(u8Channel));
        DDL_ASSERT(IS_TIMERA_OC_CACHE_TRANS_CONDITION(u16Condition));

        TMRA_BCONRx = (__IO uint16_t *)TMRA_BCONRx(TMRAx, u8Channel);
        MODIFY_REG16(*TMRA_BCONRx, TIMERA_BCONR_BSE_MASK, u16Condition);
    }

    return enRet;
}

/**
 * @brief  Get TimerA flag status.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Flag                 TimerA flag type
 *         This parameter can be one of the following values:
 *           @arg TIMERA_FLAG_OVF:      Overflow flag
 *           @arg TIMERA_FLAG_UDF:      Underflow flag
 *           @arg TIMERA_FLAG_CMP1:     PWM Channel 1 compare match flag
 *           @arg TIMERA_FLAG_CMP2:     PWM Channel 2 compare match flag
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Flag is set
 *           - Reset: Flag is reset
 */
en_flag_status_t TIMERA_GetFlag(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Flag)
{
    en_flag_status_t enFlag = Reset;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_GET_FLAG(u16Flag));

    switch (u16Flag)
    {
        case TIMERA_FLAG_OVF:
        case TIMERA_FLAG_UDF:
            enFlag = (READ_REG16_BIT(TMRAx->BCSTR, u16Flag)) ? Set : Reset;
            break;
        case TIMERA_FLAG_CMP1:
        case TIMERA_FLAG_CMP2:
            enFlag = (READ_REG16_BIT(TMRAx->STFLR, u16Flag)) ? Set : Reset;
            break;
        default:
            break;
    }

    return enFlag;
}

/**
 * @brief  Clear TimerA flag.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16Flag                 TimerA flag type
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_FLAG_OVF:      Overflow flag
 *           @arg TIMERA_FLAG_UDF:      Underflow flag
 *           @arg TIMERA_FLAG_CMP1:     PWM Channel 1 compare match flag
 *           @arg TIMERA_FLAG_CMP2:     PWM Channel 2 compare match flag
 * @retval None
 */
void TIMERA_ClearFlag(M0P_TMRA_TypeDef *TMRAx, uint16_t u16Flag)
{
    uint16_t u16Tmp;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_CLEAR_FLAG(u16Flag));

    /* OVF and UDF flag */
    u16Tmp = u16Flag & 0xFF00u;
    if (0u != u16Tmp)
    {
        CLEAR_REG16_BIT(TMRAx->BCSTR, u16Tmp);
    }

    /* Count match flag */
    u16Tmp = u16Flag & 0x00FFu;
    if (0u != u16Tmp)
    {
        CLEAR_REG16_BIT(TMRAx->STFLR, u16Tmp);
    }
}

/**
 * @brief  Enable or disable specified TimerA interrupt.
 * @param  [in] TMRAx                   Pointer to TimerA instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRA:             TimerA unit 1 instance register base
 * @param  [in] u16IntSource            TimerA interrupt source
 *         This parameter can be one or any combination of the following values:
 *           @arg TIMERA_INT_OVF:       Overflow interrupt
 *           @arg TIMERA_INT_UDF:       Underflow interrupt
 *           @arg TIMERA_INT_CMP1:      PWM Channel 1 Compare match interrupt
 *           @arg TIMERA_INT_CMP2:      PWM Channel 2 Compare match interrupt
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERA_IntCmd(M0P_TMRA_TypeDef *TMRAx,
                   uint16_t u16IntSource,
                   en_functional_state_t enNewSta)
{
    uint16_t u16Tmp;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERA_INSTANCE(TMRAx));
    DDL_ASSERT(IS_TIMERA_INT(u16IntSource));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    /* OVF and UDF interrupt */
    u16Tmp = u16IntSource & 0xFF00u;
    if (0u != u16Tmp)
    {
        if (Enable == enNewSta)
        {
            SET_REG16_BIT(TMRAx->BCSTR, u16Tmp);
        }
        else
        {
            CLEAR_REG16_BIT(TMRAx->BCSTR, u16Tmp);
        }
    }

    /* Count match interrupt */
    u16Tmp = u16IntSource & 0x00FFu;
    if (0u != u16Tmp)
    {
        if (Enable == enNewSta)
        {
            SET_REG16_BIT(TMRAx->ICONR, u16Tmp);
        }
        else
        {
            CLEAR_REG16_BIT(TMRAx->ICONR, u16Tmp);
        }
    }
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
