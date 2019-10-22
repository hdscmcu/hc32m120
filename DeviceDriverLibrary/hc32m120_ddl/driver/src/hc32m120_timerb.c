/**
 *******************************************************************************
 * @file  hc32m120_timerb.c
 * @brief This file provides firmware functions to manage the TimerB.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32m120_timerb.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_TIMERB TIMERB
 * @brief TimerB Driver Library
 * @{
 */

#if (DDL_TIMERB_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMERB_Local_Macros TimerB Local Macros
 * @{
 */

/**
 * @defgroup TIMERB_Hardware_Trigger_Source_Mask TIMERB Hardware Trigger Source Mask
 * @{
 */
#define TIMERB_TRIG_SRC_MASK                    ((uint32_t)0x0000007F)   /*!< TIMERB Hardware Trigger Source Mask */
/**
 * @}
 */

/**
 * @defgroup TIMERB_Hardware_Trigger_Condition_Mask TIMERB Hardware Trigger Condition Mask
 * @{
 */
#define TMRB_CCONR_HICP                     (TMRB_CCONR_HICP2 |                \
                                             TMRB_CCONR_HICP1 |                \
                                             TMRB_CCONR_HICP0)
#define TMRB_HCONR_HSTA                     (TMRB_HCONR_HSTA2 |                \
                                             TMRB_HCONR_HSTA1 |                \
                                             TMRB_HCONR_HSTA0)
#define TMRB_HCONR_HSTP                     (TMRB_HCONR_HSTP2 |                \
                                             TMRB_HCONR_HSTP1 |                \
                                             TMRB_HCONR_HSTP0)
#define TMRB_HCONR_HCLE                     (TMRB_HCONR_HCLE4 |                \
                                             TMRB_HCONR_HCLE3 |                \
                                             TMRB_HCONR_HCLE2 |                \
                                             TMRB_HCONR_HCLE1 |                \
                                             TMRB_HCONR_HCLE0)
#define TMRB_HCUPR_HCUP                     (TMRB_HCUPR_HCUP12 |               \
                                             TMRB_HCUPR_HCUP11 |               \
                                             TMRB_HCUPR_HCUP10 |               \
                                             TMRB_HCUPR_HCUP9  |               \
                                             TMRB_HCUPR_HCUP8)
#define TMRB_HCDOR_HCDO                     (TMRB_HCDOR_HCDO12 |               \
                                             TMRB_HCDOR_HCDO11 |               \
                                             TMRB_HCDOR_HCDO10 |               \
                                             TMRB_HCDOR_HCDO9  |               \
                                             TMRB_HCDOR_HCDO8)
/**
 * @}
 */

/**
 * @defgroup TIMERB_Check_Parameters_Validity TIMERB Check Parameters Validity
 * @{
 */

#define IS_TIMERB_INSTANCE(x)                                                  \
(   (M0P_TMRB1 == (x))                          ||                             \
    (M0P_TMRB2 == (x))                          ||                             \
    (M0P_TMRB3 == (x))                          ||                             \
    (M0P_TMRB4 == (x)))

#define IS_TIMERB_CNT_DIRECTION(x)                                             \
(   (TIMERB_CNT_UP == (x))                      ||                             \
    (TIMERB_CNT_DOWN == (x)))

#define IS_TIMERB_CNT_MODE(x)                                                  \
(   (TIMERB_SAWTOOTH_WAVE == (x))               ||                             \
    (TIMERB_TRIANGLE_WAVE == (x)))

#define IS_TIMERB_SYNC_START_STATE(x)                                          \
(   (TIMERB_SYNC_START_ENABLE == (x))           ||                             \
    (TIMERB_SYNC_START_DISABLE == (x)))

#define IS_TIMERB_CLOCK_DIV(x)                                                 \
(   (TIMERB_CLKDIV_DIV1 == (x))                 ||                             \
    (TIMERB_CLKDIV_DIV2 == (x))                 ||                             \
    (TIMERB_CLKDIV_DIV4 == (x))                 ||                             \
    (TIMERB_CLKDIV_DIV8 == (x))                 ||                             \
    (TIMERB_CLKDIV_DIV16 == (x))                ||                             \
    (TIMERB_CLKDIV_DIV32 == (x))                ||                             \
    (TIMERB_CLKDIV_DIV64 == (x))                ||                             \
    (TIMERB_CLKDIV_DIV128 == (x))               ||                             \
    (TIMERB_CLKDIV_DIV256 == (x))               ||                             \
    (TIMERB_CLKDIV_DIV512 == (x))               ||                             \
    (TIMERB_CLKDIV_DIV1024 == (x)))

#define IS_TIMERB_OVERFLOW_STATE(x)                                            \
(   (TIMERB_OVERFLOW_STOP == (x))               ||                             \
    (TIMERB_OVERFLOW_COUNT == (x)))

#define IS_TIMERB_FLAG(x)                                                      \
(   (TIMERB_FLAG_OVF == (x))                    ||                             \
    (TIMERB_FLAG_UDF == (x))                    ||                             \
    (TIMERB_FLAG_CMP == (x)))

#define IS_TIMERB_INT(x)                                                       \
(   (TIMERB_IT_OVF == (x))                      ||                             \
    (TIMERB_IT_UDF == (x))                      ||                             \
    (TIMERB_IT_CMP == (x)))

#define IS_TIMERB_CMP_EVT_STATE(x)                                             \
(   (TIMERB_CMP_EVT_ENABLE == (x))              ||                             \
    (TIMERB_CMP_EVT_DISABLE == (x)))

#define IS_TIMERB_FUNC_MODE(x)                                                 \
(   (TIMERB_INPUT_CAPTURE == (x))               ||                             \
    (TIMERB_OUTPUT_COMPARE == (x)))

#define IS_TIMERB_IC_NOISE_FILTER_CLKDIV(x)                                    \
(   (TIMERB_IC_NOFICLKDIV_NONE == (x))          ||                             \
    (TIMERB_IC_NOFICLKDIV_DIV1 == (x))          ||                             \
    (TIMERB_IC_NOFICLKDIV_DIV4 == (x))          ||                             \
    (TIMERB_IC_NOFICLKDIV_DIV16 == (x))         ||                             \
    (TIMERB_IC_NOFICLKDIV_DIV64 == (x)))

#define IS_TIMERB_OC_START_CNT_OUTPUT_POLARITY(x)                              \
(   (TIMERB_OC_STARTCNT_OUTPUT_LOW == (x))      ||                             \
    (TIMERB_OC_STARTCNT_OUTPUT_HIGH == (x))     ||                             \
    (TIMERB_OC_STARTCNT_OUTPUT_HOLD == (x)))

#define IS_TIMERB_OC_STOP_CNT_OUTPUT_POLARITY(x)                               \
(   (TIMERB_OC_STOPCNT_OUTPUT_LOW == (x))       ||                             \
    (TIMERB_OC_STOPCNT_OUTPUT_HIGH == (x))      ||                             \
    (TIMERB_OC_STOPCNT_OUTPUT_HOLD == (x)))

#define IS_TIMERB_OC_CMPMATCH_OUTPUT_POLARITY(x)                               \
(   (TIMERB_OC_CMPMATCH_OUTPUT_LOW == (x))      ||                             \
    (TIMERB_OC_CMPMATCH_OUTPUT_HIGH == (x))     ||                             \
    (TIMERB_OC_CMPMATCH_OUTPUT_HOLD == (x))     ||                             \
    (TIMERB_OC_CMPMATCH_OUTPUT_INVERTED == (x)))

#define IS_TIMERB_OC_PERIODMATCH_OUTPUT_POLARITY(x)                            \
(   (TIMERB_OC_PERIODMATCH_OUTPUT_LOW == (x))   ||                             \
    (TIMERB_OC_PERIODMATCH_OUTPUT_HIGH == (x))  ||                             \
    (TIMERB_OC_PERIODMATCH_OUTPUT_HOLD == (x))  ||                             \
    (TIMERB_OC_PERIODMATCH_OUTPUT_INVERTED == (x)))

#define IS_TIMERB_OC_FORCE_OUTPUT_POLARITY(x)                                  \
(   (TIMERB_OC_FORCE_OUTPUT_LOW == (x))         ||                             \
    (TIMERB_OC_FORCE_OUTPUT_HIGH == (x))        ||                             \
    (TIMERB_OC_FORCE_OUTPUT_INVALID == (x)))

#define IS_TIMERB_OC_PORT_OUTPUT_STATE(x)                                      \
(   (TIMERB_OC_PORT_OUTPUT_ENABLE == (x))       ||                             \
    (TIMERB_OC_PORT_OUTPUT_DISABLE == (x)))

#define IS_TIMERB_IC_CAPTURE_CONDITION(x)   ((!((x) & ((uint16_t)(~TMRB_CCONR_HICP)))) || ((x) == TIMERB_IC_INVALID))

#define IS_TIMERB_IC_HSTART_CONDITION(x)    ((!((x) & ((uint16_t)(~TMRB_HCONR_HSTA)))) || ((x) == TIMERB_HWSTART_INVALID))

#define IS_TIMERB_IC_HSTOP_CONDITION(x)     ((!((x) & ((uint16_t)(~TMRB_HCONR_HSTP)))) || ((x) == TIMERB_HWSTOP_INVALID))

#define IS_TIMERB_IC_HCLEAR_CONDITION(x)    ((!((x) & ((uint16_t)(~TMRB_HCONR_HCLE)))) || ((x) == TIMERB_HWCLEAR_INVALID))

#define IS_TIMERB_IC_HCUP_CONDITION(x)      ((!((x) & ((uint16_t)(~TMRB_HCUPR_HCUP)))) || ((x) == TIMERB_HWUP_INVALID))

#define IS_TIMERB_IC_HCDOWN_CONDITION(x)    ((!((x) & ((uint16_t)(~TMRB_HCDOR_HCDO)))) || ((x) == TIMERB_HWDOWN_INVALID))
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
 * @defgroup TIMERB_Global_Functions TimerB Global Functions
 * @{
 */

/**
 * @brief  Initialize TimerB base function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] pstcInit                Pointer to a @ref stc_timerb_init_t structure (TMRBx unit base function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: TMRBx is invalid instance or pstcInit = NULL
 */
en_result_t TIMERB_Init(M0P_TMRB_TypeDef *TMRBx,
                            const stc_timerb_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMRBx instance and pstcInit */
    if ((IS_TIMERB_INSTANCE(TMRBx)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(0u != pstcInit->u16PeriodVal);
        DDL_ASSERT(IS_TIMERB_CNT_MODE(pstcInit->u16CntMode));
        DDL_ASSERT(IS_TIMERB_CLOCK_DIV(pstcInit->u16ClkDiv));
        DDL_ASSERT(IS_TIMERB_CNT_DIRECTION(pstcInit->u16CntDir));
        DDL_ASSERT(IS_TIMERB_OVERFLOW_STATE(pstcInit->u16OverflowAction));
        DDL_ASSERT(IS_TIMERB_SYNC_START_STATE(pstcInit->u16SynStartState));

        WRITE_REG16(TMRBx->CNTER, 0u);
        WRITE_REG16(TMRBx->PERAR, pstcInit->u16PeriodVal);
        MODIFY_REG16(TMRBx->BCSTR,                                             \
                     (TMRB_BCSTR_DIR   | TMRB_BCSTR_MODE  |                    \
                      TMRB_BCSTR_SYNST | TMRB_BCSTR_CKDIV | TMRB_BCSTR_OVSTP), \
                     (pstcInit->u16CntDir        | pstcInit->u16CntMode |      \
                      pstcInit->u16SynStartState | pstcInit->u16ClkDiv  |      \
                      pstcInit->u16OverflowAction));

        /* Set hardware trigger event */
        WRITE_REG16(TMRBx->HCONR,                                              \
                    (pstcInit->u16HwStartCondition |                           \
                     pstcInit->u16HwStopCondition  |                           \
                     pstcInit->u16HwClearCondition));
        WRITE_REG16(TMRBx->HCUPR, pstcInit->u16HwUpCondition);
        WRITE_REG16(TMRBx->HCDOR, pstcInit->u16HwDownCondition);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_timerb_init_t to default values.
 * @param  [out] pstcInit               Pointer to a @ref stc_timerb_init_t structure (TMRBx unit base function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t TIMERB_StructInit(stc_timerb_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (NULL != pstcInit)
    {
        pstcInit->u16PeriodVal = 0xFFFFu;
        pstcInit->u16CntDir = TIMERB_CNT_UP;
        pstcInit->u16CntMode = TIMERB_SAWTOOTH_WAVE;
        pstcInit->u16SynStartState = TIMERB_SYNC_START_DISABLE;
        pstcInit->u16ClkDiv = TIMERB_CLKDIV_DIV1;
        pstcInit->u16OverflowAction = TIMERB_OVERFLOW_COUNT;
        pstcInit->u16HwStartCondition = TIMERB_HWSTART_INVALID;
        pstcInit->u16HwStopCondition = TIMERB_HWSTOP_INVALID;
        pstcInit->u16HwClearCondition = TIMERB_HWCLEAR_INVALID;
        pstcInit->u16HwUpCondition = TIMERB_HWUP_INVALID;
        pstcInit->u16HwDownCondition = TIMERB_HWDOWN_INVALID;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-Initialize TimerB unit base function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 *           - ErrorInvalidParameter: TMRBx is invalid instance
 */
en_result_t TIMERB_DeInit(M0P_TMRB_TypeDef *TMRBx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMRBx instance */
    if (IS_TIMERB_INSTANCE(TMRBx))
    {
        /* Configures the registers to reset value. */
        WRITE_REG16(TMRBx->CNTER, 0x0000u);
        WRITE_REG16(TMRBx->PERAR, 0xFFFFu);
        WRITE_REG16(TMRBx->BCSTR, 0x0002u);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set TimerB count direction.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Direction            TimerB count direction
 *         This parameter can be one of the following values:
 *           @arg TIMERB_CNT_UP:        TimerB count up
 *           @arg TIMERB_CNT_DOWN:      TimerB count down
 * @retval None
 */
void TIMERB_SetCntDirection(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Direction)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_CNT_DIRECTION(u16Direction));

    MODIFY_REG16(TMRBx->BCSTR, TMRB_BCSTR_DIR, u16Direction);
}

/**
 * @brief  Get TimerB count direction.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_CNT_UP:        TimerB count up
 *           @arg TIMERB_CNT_DOWN:      TimerB count down
 */
uint16_t TIMERB_GetCntDirection(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->BCSTR, TMRB_BCSTR_DIR);
}

/**
 * @brief  Set TimerB count mode.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Mode                 TimerB count mode
 *         This parameter can be one of the following values:
 *           @arg TIMERB_TRIANGLE_WAVE: TimerB triangle wave mode
 *           @arg TIMERB_SAWTOOTH_WAVE: TimerB sawtooth wave mode
 * @retval None
 */
void TIMERB_SetCntMode(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Mode)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_CNT_MODE(u16Mode));

    MODIFY_REG16(TMRBx->BCSTR, TMRB_BCSTR_MODE, u16Mode);
}

/**
 * @brief  Get TimerB count mode.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_TRIANGLE_WAVE: TimerB triangle wave mode
 *           @arg TIMERB_SAWTOOTH_WAVE: TimerB sawtooth wave mode
 */
uint16_t TIMERB_GetCntMode(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->BCSTR, TMRB_BCSTR_MODE);
}

/**
 * @brief  Set TimerB clock division.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Div                  TimerB clock division
 *         This parameter can be one of the following values:
 *           @arg TIMERB_CLKDIV_DIV1:   HCLK
 *           @arg TIMERB_CLKDIV_DIV2:   HCLK/2
 *           @arg TIMERB_CLKDIV_DIV4:   HCLK/4
 *           @arg TIMERB_CLKDIV_DIV8:   HCLK/8
 *           @arg TIMERB_CLKDIV_DIV16:  HCLK/16
 *           @arg TIMERB_CLKDIV_DIV32:  HCLK/32
 *           @arg TIMERB_CLKDIV_DIV64:  HCLK/64
 *           @arg TIMERB_CLKDIV_DIV128: HCLK/128
 *           @arg TIMERB_CLKDIV_DIV256: HCLK/256
 *           @arg TIMERB_CLKDIV_DIV512: HCLK/512
 *           @arg TIMERB_CLKDIV_DIV1024:HCLK/1024
 * @retval None
 */
void TIMERB_SetClkDiv(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Div)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_CLOCK_DIV(u16Div));

    MODIFY_REG16(TMRBx->BCSTR, TMRB_BCSTR_CKDIV, u16Div);
}

/**
 * @brief  Get TimerB clock division.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_CLKDIV_DIV1:   HCLK
 *           @arg TIMERB_CLKDIV_DIV2:   HCLK/2
 *           @arg TIMERB_CLKDIV_DIV4:   HCLK/4
 *           @arg TIMERB_CLKDIV_DIV8:   HCLK/8
 *           @arg TIMERB_CLKDIV_DIV16:  HCLK/16
 *           @arg TIMERB_CLKDIV_DIV32:  HCLK/32
 *           @arg TIMERB_CLKDIV_DIV64:  HCLK/64
 *           @arg TIMERB_CLKDIV_DIV128: HCLK/128
 *           @arg TIMERB_CLKDIV_DIV256: HCLK/256
 *           @arg TIMERB_CLKDIV_DIV512: HCLK/512
 *           @arg TIMERB_CLKDIV_DIV1024:HCLK/1024
 */
uint16_t TIMERB_GetClkDiv(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->BCSTR, TMRB_BCSTR_CKDIV);
}

/**
 * @brief  Get TimerB flag status.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Flag                 TimerB flag type
 *         This parameter can be one of the following values:
 *           @arg TIMERB_FLAG_OVF:      Overflow flag
 *           @arg TIMERB_FLAG_UDF:      Underflow flag
 *           @arg TIMERB_FLAG_CMP:      Compare match flag
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Flag is set
 *           - Reset: Flag is reset
 */
en_flag_status_t TIMERB_GetFlag(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Flag)
{
    en_flag_status_t enFlag = Reset;

    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_FLAG(u16Flag));

    switch (u16Flag)
    {
        case TIMERB_FLAG_OVF:
        case TIMERB_FLAG_UDF:
            enFlag = (READ_REG16_BIT(TMRBx->BCSTR, u16Flag)) ? Set : Reset;
            break;
        case TIMERB_FLAG_CMP:
            enFlag = (READ_REG16_BIT(TMRBx->STFLR, u16Flag)) ? Set : Reset;
            break;
        default:
            break;
    }

    return enFlag;
}

/**
 * @brief  Clear TimerB flag.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Flag                 TimerB flag type
 *         This parameter can be one of the following values:
 *           @arg TIMERB_FLAG_OVF:      Overflow flag
 *           @arg TIMERB_FLAG_UDF:      Underflow flag
 *           @arg TIMERB_FLAG_CMP:      Compare match flag
 * @retval None
 */
void TIMERB_ClearFlag(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Flag)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_FLAG(u16Flag));

    switch (u16Flag)
    {
        case TIMERB_FLAG_OVF:
        case TIMERB_FLAG_UDF:
            CLEAR_REG16_BIT(TMRBx->BCSTR, u16Flag);
            break;
        case TIMERB_FLAG_CMP:
            CLEAR_REG16_BIT(TMRBx->STFLR, u16Flag);
            break;
        default:
            break;
    }
}

/**
 * @brief  Enable or disable specified TimerB interrupt.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16IntSource            TimerB interrupt source
 *         This parameter can be one of the following values:
 *           @arg TIMERB_INT_OVF:       Overflow interrupt
 *           @arg TIMERB_INT_UDF:       Underflow interrupt
 *           @arg TIMERB_INT_CMP:       Compare match interrupt
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void TIMERB_IntCmd(M0P_TMRB_TypeDef *TMRBx,
                        uint16_t u16IntSource,
                        en_functional_state_t enNewSta)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_INT(u16IntSource));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    switch (u16IntSource)
    {
        case TIMERB_IT_OVF:
        case TIMERB_IT_UDF:
            if (Enable == enNewSta)
            {
                SET_REG16_BIT(TMRBx->BCSTR, u16IntSource);
            }
            else
            {
                CLEAR_REG16_BIT(TMRBx->BCSTR, u16IntSource);
            }
            break;
        case TIMERB_IT_CMP:
            if (Enable == enNewSta)
            {
                SET_REG16_BIT(TMRBx->ICONR, u16IntSource);
            }
            else
            {
                CLEAR_REG16_BIT(TMRBx->ICONR, u16IntSource);
            }
            break;
        default:
            break;
    }
}

/**
 * @brief  Set TimerB function mode.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Mode                 TimerB function mode
 *         This parameter can be one of the following values:
 *           @arg TIMERB_INPUT_CAPTURE: Capture input function
 *           @arg TIMERB_OUTPUT_COMPARE:Comare output function
 * @retval None
 */
void TIMERB_SetMode(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Mode)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_FUNC_MODE(u16Mode));

    MODIFY_REG16(TMRBx->CCONR, TMRB_CCONR_CAPMDA, u16Mode);
}

/**
 * @brief  Get TimerB function mode.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_INPUT_CAPTURE: Capture input function
 *           @arg TIMERB_OUTPUT_COMPARE:Comare output function
 */
uint16_t TIMERB_GetMode(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->CCONR, TMRB_CCONR_CAPMDA);
}

/**
 * @brief  Set TimerB hardware trigger counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Condition            TimerB hardware start counter condition
 *         This parameter can be any composed value of the following values:
 *           @arg @ref TIMERB_Hardware_Start_Count_Condition
 *           @arg @ref TIMERB_Hardware_Stop_Count_Condition
 *           @arg @ref TIMERB_Hardware_Clear_Count_Condition
 * @retval  None
 */
void TIMERB_SetHwTriggerCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    WRITE_REG16(TMRBx->HCONR, u16Condition);
}

/**
 * @brief  Set TimerB hardware start counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Condition            TimerB hardware start counter condition
 *         This parameter can be any composed value of the following values:
 *           @arg TIMERB_HWSTART_INVALID:               Hardware start is invalid
 *           @arg TIMERB_HWSTART_TIMB_T_PWM1_RISING:    Hardware start counter by rising edge on timer input TIMB_<t>_PWM1 (t = 1~8) when sysnc start function is invalid or
 *                                                      hardware start counter by rising edge on timer input TIMB_n_PWM1 (n = 1,3,5,7) when sysnc start function is invalid
 *           @arg TIMERB_HWSTART_TIMB_T_PWM1_FALLING:   Hardware start counter by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8) when sysnc start function is invalid or
 *                                                      hardware start counter by falling edge on timer input TIMB_n_PWM1 (n = 1,3,5,7) when sysnc start function is invalid
 *           @arg TIMERB_HWSTART_TIMB_EVT:              Hardware start counter by the timer TMRB_HTSSR specified event
 * @retval  None
 */
void TIMERB_SetHwStartCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_IC_HSTART_CONDITION(u16Condition));

    MODIFY_REG16(TMRBx->HCONR, TMRB_HCONR_HSTA, u16Condition);
}

/**
 * @brief  Get TimerB hardware start counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be any composed value of the following values:
 *           @arg TIMERB_HWSTART_INVALID:               Hardware start is invalid
 *           @arg TIMERB_HWSTART_TIMB_T_PWM1_RISING:    Hardware start counter by rising edge on timer input TIMB_<t>_PWM1 (t = 1~8) when sysnc start function is invalid or
 *                                                      hardware start counter by rising edge on timer input TIMB_n_PWM1 (n = 1,3,5,7) when sysnc start function is invalid
 *           @arg TIMERB_HWSTART_TIMB_T_PWM1_FALLING:   Hardware start counter by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8) when sysnc start function is invalid or
 *                                                      hardware start counter by falling edge on timer input TIMB_n_PWM1 (n = 1,3,5,7) when sysnc start function is invalid
 *           @arg TIMERB_HWSTART_TIMB_EVT:              Hardware start counter by the timer TMRB_HTSSR specified event
 */
uint16_t TIMERB_GetHwStartCondition(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->HCONR, TMRB_HCONR_HSTA);
}

/**
 * @brief  Set TimerB hardware stop counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Condition            TimerB hardware stop counter condition
 *         This parameter can be any composed value of the following values:
 *           @arg TIMERB_HWSTOP_INVALID:                Hardware stop is invalid
 *           @arg TIMERB_HWSTOP_TIMB_T_PWM1_RISING:     Hardware stop counter by rising edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWSTOP_TIMB_T_PWM1_FALLING:    Hardware stop counter by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWSTOP_TIMB_EVT:               Hardware stop counter by the timer TMRB_HTSSR specified event
 * @retval  None
 */
void TIMERB_SetHwStopCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_IC_HSTOP_CONDITION(u16Condition));

    MODIFY_REG16(TMRBx->HCONR, TMRB_HCONR_HSTP, u16Condition);
}

/**
 * @brief  Get TimerB hardware stop counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be any composed value of the following values:
 *           @arg TIMERB_HWSTOP_INVALID:                Hardware stop is invalid
 *           @arg TIMERB_HWSTOP_TIMB_T_PWM1_RISING:     Hardware stop counter by rising edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWSTOP_TIMB_T_PWM1_FALLING:    Hardware stop counter by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWSTOP_TIMB_EVT:               Hardware stop counter by the timer TMRB_HTSSR specified event
 */
uint16_t TIMERB_GetHwStopCondition(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->HCONR, TMRB_HCONR_HSTP);
}

/**
 * @brief  Set TimerB hardware clear counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Condition            TimerB hardware clear counter condition
 *         This parameter can be any composed value of the following values:
 *           @arg TIMERB_HWCLEAR_INVALID:               Hardware clear is invalid
 *           @arg TIMERB_HWCLEAR_TIMB_T_PWM1_RISING:    Hardware clear counter by rising edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWCLEAR_TIMB_T_PWM1_FALLING:   Hardware clear counter by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWCLEAR_EVT:                   Hardware clear counter by the timer TMRB_HTSSR specified event
 *           @arg TIMERB_HWCLEAR_TIMB_N_PWM1_RISING:    Hardware clear counter by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWCLEAR_TIMB_N_PWM1_FALLING:   Hardware clear counter by the timer TMRB_HTSSR specified event
 * @retval  None
 */
void TIMERB_SetHwClearCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_IC_HCLEAR_CONDITION(u16Condition));

    MODIFY_REG16(TMRBx->HCONR, TMRB_HCONR_HCLE, u16Condition);
}

/**
 * @brief  Get TimerB hardware clear counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be any composed value of the following values:
 *           @arg TIMERB_HWCLEAR_INVALID:               Hardware clear is invalid
 *           @arg TIMERB_HWCLEAR_TIMB_T_PWM1_RISING:    Hardware clear counter by rising edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWCLEAR_TIMB_T_PWM1_FALLING:   Hardware clear counter by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWCLEAR_EVT:                   Hardware clear counter by the timer TMRB_HTSSR specified event
 *           @arg TIMERB_HWCLEAR_TIMB_N_PWM1_RISING:    Hardware clear counter by falling edge on timer input TIMB_<t>_PWM1 (t = 1~8)
 *           @arg TIMERB_HWCLEAR_TIMB_N_PWM1_FALLING:   Hardware clear counter by the timer TMRB_HTSSR specified event
 */
uint16_t TIMERB_GetHwClearCondition(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->HCONR, TMRB_HCONR_HCLE);
}

/**
 * @brief  Set TimerB hardware up counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Condition            TimerB hardware up counter condition
 *         This parameter can be any composed value of the following values:
 *           @arg TIMERB_HWUP_INVALID:              Hardware count up is invalid
 *           @arg TIMERB_HWUP_TIMB_T_PWM1_RISING:   Hardware count up by rising edge on timer input TIMB_<t>_PWM1, t = 1~8
 *           @arg TIMERB_HWUP_TIMB_T_PWM1_FALLING:  Hardware count up by falling edge on timer input TIMB_<t>_PWM1, t = 1~8
 *           @arg TIMERB_HWUP_EVT:                  Hardware count up by the timer TMRB_HTSSR specified event
 *           @arg TIMERB_HWUP_OVERFLOW:             Hardware count up by overflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8
 *           @arg TIMERB_HWUP_UNDERFLOW:            Hardware count up by underflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8
 * @retval  None
 */
void TIMERB_SetHwUpCondition(M0P_TMRB_TypeDef *TMRBx,
                                    uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_IC_HCUP_CONDITION(u16Condition));

    MODIFY_REG16(TMRBx->HCUPR, TMRB_HCUPR_HCUP, u16Condition);
}

/**
 * @brief  Get TimerB hardware up counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be any composed value of the following values:
 *           @arg TIMERB_HWUP_INVALID:              Hardware count up is invalid
 *           @arg TIMERB_HWUP_TIMB_T_PWM1_RISING:   Hardware count up by rising edge on timer input TIMB_<t>_PWM1, t = 1~8
 *           @arg TIMERB_HWUP_TIMB_T_PWM1_FALLING:  Hardware count up by falling edge on timer input TIMB_<t>_PWM1, t = 1~8
 *           @arg TIMERB_HWUP_EVT:                  Hardware count up by the timer TMRB_HTSSR specified event
 *           @arg TIMERB_HWDOWN_OVERFLOW:           Hardware count up by overflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8
 *           @arg TIMERB_HWDOWN_UNDERFLOW:          Hardware count up by underflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8
 */
uint16_t TIMERB_GetHwUpCondition(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->HCUPR, TMRB_HCUPR_HCUP);
}

/**
 * @brief  Set TimerB hardware down counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Condition            TimerB hardware down counter condition
 *         This parameter can be any composed value of the following values:
 *           @arg TIMERB_HWDOWN_INVALID:             Hardware count down is invalid
 *           @arg TIMERB_HWDOWN_TIMB_T_PWM1_RISING:  Hardware count down by rising edge on timer input TIMB_<t>_PWM1, t = 1~8
 *           @arg TIMERB_HWDOWN_TIMB_T_PWM1_FALLING: Hardware count down by falling edge on timer input TIMB_<t>_PWM1, t = 1~8
 *           @arg TIMERB_HWDOWN_EVT:                 Hardware count down by the timer TMRB_HTSSR specified event
 *           @arg TIMERB_HWDOWN_OVERFLOW:            Hardware count down by overflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8
 *           @arg TIMERB_HWDOWN_UNDERFLOW:           Hardware count down by underflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8
 * @retval  None
 */
void TIMERB_SetHwDownCondition(M0P_TMRB_TypeDef *TMRBx,
                                        uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_IC_HCUP_CONDITION(u16Condition));

    MODIFY_REG16(TMRBx->HCDOR, TMRB_HCDOR_HCDO, u16Condition);
}

/**
 * @brief  Get TimerB hardware down counter condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be any composed value of the following values:
 *           @arg TIMERB_HWDOWN_INVALID:             Hardware count down is invalid
 *           @arg TIMERB_HWDOWN_TIMB_T_PWM1_RISING:  Hardware count down by rising edge on timer input TIMB_<t>_PWM1, t = 1~8
 *           @arg TIMERB_HWDOWN_TIMB_T_PWM1_FALLING: Hardware count down by falling edge on timer input TIMB_<t>_PWM1, t = 1~8
 *           @arg TIMERB_HWDOWN_EVT:                 Hardware count down by the timer TMRB_HTSSR specified event
 *           @arg TIMERB_HWDOWN_OVERFLOW:            Hardware count down by overflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8
 *           @arg TIMERB_HWDOWN_UNDERFLOW:           Hardware count down by underflow by TIMB_<n>, _<n>_=2/4/6/8 when m=1/3/5/7 or n=1/3/5/7 when m=2/4/6/8
 */
uint16_t TIMERB_GetHwDownCondition(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->HCDOR, TMRB_HCDOR_HCDO);
}

/**
 * @brief  Initialize TimerB input capture function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] pstcIcInit              Pointer to a @ref stc_timerb_ic_init_t structure (TMRBx unit input capture function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: TMRBx is invalid instance or pstcIcInit == NULL
 */
en_result_t TIMERB_IC_Init(M0P_TMRB_TypeDef *TMRBx,
                                const stc_timerb_ic_init_t *pstcIcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMRBx instance and pstcIcInit */
    if ((IS_TIMERB_INSTANCE(TMRBx)) && (NULL != pstcIcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_TIMERB_IC_CAPTURE_CONDITION(pstcIcInit->u16CaptureCondition));
        DDL_ASSERT(IS_TIMERB_IC_NOISE_FILTER_CLKDIV(pstcIcInit->u16NoiseFilterClkDiv));

        MODIFY_REG16(TMRBx->CCONR,                                               \
                     (TMRB_CCONR_CAPMDA   | TMRB_CCONR_HICP |                    \
                      TMRB_CCONR_NOFIENCP | TMRB_CCONR_NOFICKCP),                \
                     (TIMERB_INPUT_CAPTURE            |                          \
                      pstcIcInit->u16CaptureCondition |                          \
                      pstcIcInit->u16NoiseFilterClkDiv));
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_timerb_ic_init_t to default values.
 * @param  [out] pstcIcInit             Pointer to a @ref stc_timerb_ic_init_t structure (TMRBx unit input capture function configuration data structure).
 * @retval None
 */
en_result_t TIMERB_IC_StructInit(stc_timerb_ic_init_t *pstcIcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (NULL != pstcIcInit)
    {
        pstcIcInit->u16NoiseFilterClkDiv = TIMERB_IC_NOFICLKDIV_NONE;
        pstcIcInit->u16CaptureCondition = TIMERB_IC_INVALID;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-Initialize TimerB unit input capture.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 *           - ErrorInvalidParameter: TMRBx is invalid instance
 */
en_result_t TIMERB_IC_DeInit(M0P_TMRB_TypeDef *TMRBx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMRBx instance */
    if (IS_TIMERB_INSTANCE(TMRBx))
    {
        /* Configures the registers to reset value. */
        WRITE_REG16(TMRBx->PERAR, 0xFFFFu);
        WRITE_REG16(TMRBx->CCONR, 0x0001u);
        WRITE_REG16(TMRBx->HCONR, 0x0000u);
        WRITE_REG16(TMRBx->HCUPR, 0x0000u);
        WRITE_REG16(TMRBx->HCDOR, 0x0000u);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set TimerB input capture condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Condition            TimerB input capture condition
 *         This parameter can be any composed value of the following values:
 *           @arg TIMERB_IC_RISING:     Capture triggered by rising edge on timer input TIMB_<t>_PWM1
 *           @arg TIMERB_IC_FALLING:    Capture triggered by falling edge on timer input TIMB_<t>_PWM1
 *           @arg TIMERB_IC_EVT:        Capture triggered by the timer TMRB_HTSSR specified event
 *           @arg TIMERB_IC_INVALID:    Don't occur capture action
 * @retval None
 */
void TIMERB_IC_SetCaptureCondition(M0P_TMRB_TypeDef *TMRBx,
                                            uint16_t u16Condition)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_IC_CAPTURE_CONDITION(u16Condition));

    MODIFY_REG16(TMRBx->CCONR, TMRB_CCONR_HICP, u16Condition);
}

/**
 * @brief  Get TimerB input capture condition.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be any composed value of the following values:
 *           @arg TIMERB_IC_RISING:     Capture triggered by rising edge on timer input TIMB_<t>_PWM1
 *           @arg TIMERB_IC_FALLING:    Capture triggered by falling edge on timer input TIMB_<t>_PWM1
 *           @arg TIMERB_IC_EVT:        Capture triggered by the timer TMRB_HTSSR specified event
 *           @arg TIMERB_IC_INVALID:    Don't occur capture action
 */
uint16_t TIMERB_IC_GetCaptureCondition(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->CCONR, TMRB_CCONR_HICP);
}

/**
 * @brief  Set TimerB input capture noise filter clock division.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Div                  TimerB input capture noise filter clock division
 *         This parameter can be one of the following values:
 *           @arg TIMERB_IC_NOFICLKDIV_NONE:    Disable filter
 *           @arg TIMERB_IC_NOFICLKDIV_DIV1:    HCLK
 *           @arg TIMERB_IC_NOFICLKDIV_DIV4:    HCLK/4
 *           @arg TIMERB_IC_NOFICLKDIV_DIV16:   HCLK/16
 *           @arg TIMERB_IC_NOFICLKDIV_DIV64:   HCLK/64
 * @retval None
 */
void TIMERB_IC_SetFilterClkDiv(M0P_TMRB_TypeDef *TMRBx, uint16_t u16Div)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_IC_NOISE_FILTER_CLKDIV(u16Div));

    MODIFY_REG16(TMRBx->CCONR, (TMRB_CCONR_NOFIENCP | TMRB_CCONR_NOFICKCP), u16Div);
}

/**
 * @brief  Get TimerB input capture noise filter clock division.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_IC_NOFICLKDIV_NONE:    Disable filter
 *           @arg TIMERB_IC_NOFICLKDIV_DIV1:    HCLK
 *           @arg TIMERB_IC_NOFICLKDIV_DIV4:    HCLK/4
 *           @arg TIMERB_IC_NOFICLKDIV_DIV16:   HCLK/16
 *           @arg TIMERB_IC_NOFICLKDIV_DIV64:   HCLK/64
 */
uint16_t TIMERB_IC_GetFilterClkDiv(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->CCONR, (TMRB_CCONR_NOFIENCP | TMRB_CCONR_NOFICKCP));
}

/**
 * @brief  Initialize TimerB output compare function.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] pstcOcInit              Pointer to a @ref stc_timerb_oc_init_t structure (TMRBx unit output compare function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: TMRBx is invalid instance or pstcOcInit == NULL
 */
en_result_t TIMERB_OC_Init(M0P_TMRB_TypeDef *TMRBx,
                                const stc_timerb_oc_init_t *pstcOcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMRBx instance and pstcOcInit */
    if ((IS_TIMERB_INSTANCE(TMRBx)) && (NULL != pstcOcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_TIMERB_OC_PORT_OUTPUT_STATE(pstcOcInit->u16PortOutputState));
        DDL_ASSERT(IS_TIMERB_OC_START_CNT_OUTPUT_POLARITY(pstcOcInit->u16StartCntOutput));
        DDL_ASSERT(IS_TIMERB_OC_STOP_CNT_OUTPUT_POLARITY(pstcOcInit->u16StopCntOutput));
        DDL_ASSERT(IS_TIMERB_OC_CMPMATCH_OUTPUT_POLARITY(pstcOcInit->u16CompareMatchOutput));
        DDL_ASSERT(IS_TIMERB_OC_PERIODMATCH_OUTPUT_POLARITY(pstcOcInit->u16PeriodMatchOutput));

        WRITE_REG16(TMRBx->CMPAR, pstcOcInit->u16CompareVal);
        WRITE_REG16(TMRBx->PCONR,                                              \
                    (pstcOcInit->u16StartCntOutput     |                       \
                     pstcOcInit->u16StopCntOutput      |                       \
                     pstcOcInit->u16CompareMatchOutput |                       \
                     pstcOcInit->u16PeriodMatchOutput  |                       \
                     pstcOcInit->u16PortOutputState));
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_timerb_oc_init_t to default values.
 * @param  [out] pstcOcInit             Pointer to a @ref stc_timerb_oc_init_t structure (TMRBx unit output compare function configuration data structure).
 * @retval None
 */
en_result_t TIMERB_OC_StructInit(stc_timerb_oc_init_t *pstcOcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (NULL != pstcOcInit)
    {
        pstcOcInit->u16CompareVal = 0xFFFFu;
        pstcOcInit->u16PortOutputState = TIMERB_OC_PORT_OUTPUT_DISABLE;
        pstcOcInit->u16StartCntOutput = TIMERB_OC_STARTCNT_OUTPUT_LOW;
        pstcOcInit->u16StopCntOutput = TIMERB_OC_STOPCNT_OUTPUT_LOW;
        pstcOcInit->u16CompareMatchOutput = TIMERB_OC_CMPMATCH_OUTPUT_LOW;
        pstcOcInit->u16PeriodMatchOutput = TIMERB_OC_PERIODMATCH_OUTPUT_LOW;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-Initialize TimerB unit output compare.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 *           - ErrorInvalidParameter: TMRBx is invalid instance
 */
en_result_t TIMERB_OC_DeInit(M0P_TMRB_TypeDef *TMRBx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMRBx instance */
    if (IS_TIMERB_INSTANCE(TMRBx))
    {
        /* Configures the registers to reset value. */
        WRITE_REG16(TMRBx->PCONR, 0x0000u);
        WRITE_REG16(TMRBx->PERAR, 0xFFFFu);
        WRITE_REG16(TMRBx->CMPAR, 0xFFFFu);
        WRITE_REG16(TMRBx->BCSTR, 0x0000u);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set TIMB_<t>_PWM1 port output polarity when start count.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Polarity             TimerB TIMB_<t>_PWM1 port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERB_OC_STARTCNT_OUTPUT_LOW     TIM_<t>_PWM1 output low level when TimerB start count
 *           @arg TIMERB_OC_STARTCNT_OUTPUT_HIGH    TIM_<t>_PWM1 output high level when TimerB start count
 *           @arg TIMERB_OC_STARTCNT_OUTPUT_HOLD    TIM_<t>_PWM1 output hold level when TimerB start count
 * @retval None
 */
void TIMERB_OC_SetStartCntOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                    uint16_t u16Polarity)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_OC_START_CNT_OUTPUT_POLARITY(u16Polarity));

    MODIFY_REG16(TMRBx->PCONR, TMRB_PCONR_STAC, u16Polarity);
}

/**
 * @brief  Get TIMB_<t>_PWM1 port output polarity for start count.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_OC_STARTCNT_OUTPUT_LOW     TIM_<t>_PWM1 output low level when TimerB start count
 *           @arg TIMERB_OC_STARTCNT_OUTPUT_HIGH    TIM_<t>_PWM1 output high level when TimerB start count
 *           @arg TIMERB_OC_STARTCNT_OUTPUT_HOLD    TIM_<t>_PWM1 output hold level when TimerB start count
 */
uint16_t TIMERB_OC_GetStartCntOutputPolarity(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->PCONR, TMRB_PCONR_STAC);
}

/**
 * @brief  Set TIMB_<t>_PWM1 port output polarity for stop count.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Polarity             TimerB TIMB_<t>_PWM1 port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERB_OC_STOPCNT_OUTPUT_LOW      TIM_<t>_PWM1 output low level when TimerB stop count
 *           @arg TIMERB_OC_STOPCNT_OUTPUT_HIGH     TIM_<t>_PWM1 output high level when TimerB stop count
 *           @arg TIMERB_OC_STOPCNT_OUTPUT_HOLD     TIM_<t>_PWM1 output hold level when TimerB stop count
 * @retval None
 */
void TIMERB_OC_SetStopCntOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                    uint16_t u16Polarity)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_OC_STOP_CNT_OUTPUT_POLARITY(u16Polarity));

    MODIFY_REG16(TMRBx->PCONR, TMRB_PCONR_STPC, u16Polarity);
}

/**
 * @brief  Get TIMB_<t>_PWM1 port output polarity for stop count.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_OC_STOPCNT_OUTPUT_LOW      TIM_<t>_PWM1 output low level when TimerB stop count
 *           @arg TIMERB_OC_STOPCNT_OUTPUT_HIGH     TIM_<t>_PWM1 output high level when TimerB stop count
 *           @arg TIMERB_OC_STOPCNT_OUTPUT_HOLD     TIM_<t>_PWM1 output hold level when TimerB stop count
 */
uint16_t TIMERB_OC_GetStopCntOutputPolarity(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->PCONR, TMRB_PCONR_STPC);
}

/**
 * @brief  Set TIMB_<t>_PWM1 port output polarity for compare match.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Polarity             TimerB TIMB_<t>_PWM1 port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERB_OC_CMPMATCH_OUTPUT_LOW         TIM_<t>_PWM1 output low level when TimerB comapre match
 *           @arg TIMERB_OC_CMPMATCH_OUTPUT_HIGH        TIM_<t>_PWM1 output high level when TimerB comapre match
 *           @arg TIMERB_OC_CMPMATCH_OUTPUT_HOLD        TIM_<t>_PWM1 output hold level when TimerB comapre match
 *           @arg TIMERB_OC_CMPMATCH_OUTPUT_INVERTED    TIM_<t>_PWM1 output inverted level when TimerB comapre match
 * @retval None
 */
void TIMERB_OC_SetCmpMatchOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                    uint16_t u16Polarity)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_OC_CMPMATCH_OUTPUT_POLARITY(u16Polarity));

    MODIFY_REG16(TMRBx->PCONR, TMRB_PCONR_CMPC, u16Polarity);
}

/**
 * @brief  Get TIMB_<t>_PWM1 port output polarity for compare match.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_OC_CMPMATCH_OUTPUT_LOW         TIM_<t>_PWM1 output low level when TimerB comapre match
 *           @arg TIMERB_OC_CMPMATCH_OUTPUT_HIGH        TIM_<t>_PWM1 output high level when TimerB comapre match
 *           @arg TIMERB_OC_CMPMATCH_OUTPUT_HOLD        TIM_<t>_PWM1 output hold level when TimerB comapre match
 *           @arg TIMERB_OC_CMPMATCH_OUTPUT_INVERTED    TIM_<t>_PWM1 output inverted level when TimerB comapre match
 */
uint16_t TIMERB_OC_GetCmpMatchOutputPolarity(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->PCONR, TMRB_PCONR_CMPC);
}

/**
 * @brief  Set TIMB_<t>_PWM1 port output polarity for period match.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Polarity             TimerB TIMB_<t>_PWM1 port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERB_OC_PERIODMATCH_OUTPUT_LOW          TIM_<t>_PWM1 output low level when TimerB period match
 *           @arg TIMERB_OC_PERIODMATCH_OUTPUT_HIGH         TIM_<t>_PWM1 output high level when TimerB period match
 *           @arg TIMERB_OC_PERIODMATCH_OUTPUT_HOLD         TIM_<t>_PWM1 output hold level when TimerB period match
 *           @arg TIMERB_OC_PERIODMATCH_OUTPUT_INVERTED     TIM_<t>_PWM1 output inverted level when TimerB period match
 * @retval None
 */
void TIMERB_OC_SetPeriodMatchOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                        uint16_t u16Polarity)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_OC_PERIODMATCH_OUTPUT_POLARITY(u16Polarity));

    MODIFY_REG16(TMRBx->PCONR, TMRB_PCONR_PERC, u16Polarity);
}

/**
 * @brief  Get TIMB_<t>_PWM1 port output polarity for period match.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_OC_PERIODMATCH_OUTPUT_LOW          TIM_<t>_PWM1 output low level when TimerB period match
 *           @arg TIMERB_OC_PERIODMATCH_OUTPUT_HIGH         TIM_<t>_PWM1 output high level when TimerB period match
 *           @arg TIMERB_OC_PERIODMATCH_OUTPUT_HOLD         TIM_<t>_PWM1 output hold level when TimerB period match
 *           @arg TIMERB_OC_PERIODMATCH_OUTPUT_INVERTED     TIM_<t>_PWM1 output inverted level when TimerB period match
 */
uint16_t TIMERB_OC_GetPeriodMatchOutputPolarity(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->PCONR, TMRB_PCONR_PERC);
}

/**
 * @brief  Set TIMB_<t>_PWM1 port output polarity for force.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @param  [in] u16Polarity             TimerB TIMB_<t>_PWM1 port output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMERB_OC_FORCE_OUTPUT_LOW        Force TIM_<t>_PWM1 output low level
 *           @arg TIMERB_OC_FORCE_OUTPUT_HIGH       Force TIM_<t>_PWM1 output high level
 *           @arg TIMERB_OC_FORCE_OUTPUT_INVALID    Force invalid
 * @retval None
 */
void TIMERB_OC_SetForceOutputPolarity(M0P_TMRB_TypeDef *TMRBx,
                                                uint16_t u16Polarity)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));
    DDL_ASSERT(IS_TIMERB_OC_FORCE_OUTPUT_POLARITY(u16Polarity));

    MODIFY_REG16(TMRBx->PCONR, TMRB_PCONR_FORC, u16Polarity);
}

/**
 * @brief  Get TIMB_<t>_PWM1 port output polarity for force.
 * @param  [in] TMRBx                   Pointer to TimerB instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_TMRB1:            TimerB unit 1 instance register base
 *           @arg M0P_TMRB2:            TimerB unit 2 instance register base
 *           @arg M0P_TMRB3:            TimerB unit 3 instance register base
 *           @arg M0P_TMRB4:            TimerB unit 4 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg TIMERB_OC_FORCE_OUTPUT_LOW        Force TIM_<t>_PWM1 output low level
 *           @arg TIMERB_OC_FORCE_OUTPUT_HIGH       Force TIM_<t>_PWM1 output high level
 *           @arg TIMERB_OC_FORCE_OUTPUT_INVALID    Force invalid
 */
uint16_t TIMERB_OC_GetForceOutputPolarity(M0P_TMRB_TypeDef *TMRBx)
{
    /* Check parameters */
    DDL_ASSERT(IS_TIMERB_INSTANCE(TMRBx));

    return READ_REG16_BIT(TMRBx->PCONR, TMRB_PCONR_FORC);
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
