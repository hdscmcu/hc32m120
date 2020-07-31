/**
 *******************************************************************************
 * @file  hc32m120_ctc.c
 * @brief This file provides firmware functions to manage the CTC.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-03       Hongjh          First version
   2020-07-27       Hongjh          Correct the macro define:IS_CTC_OFFSET_VAL.
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
#include "hc32m120_ctc.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_CTC CTC
 * @brief CTC Driver Library
 * @{
 */

#if (DDL_CTC_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CTC_Local_Macros CTC Local Macros
 * @{
 */

/**
 * @defgroup CTC_Check_Parameters_Validity CTC Check Parameters Validity
 * @{
 */
#define IS_CTC_FREF(x)                          ((x) <= 25000000UL)  /* 25MHz */

#define IS_CTC_FHRC(x)                                                         \
(   (CTC_TRIMMING_HRC_1MHZ == (x))              ||                             \
    (CTC_TRIMMING_HRC_2MHZ == (x))              ||                             \
    (CTC_TRIMMING_HRC_3MHZ == (x))              ||                             \
    (CTC_TRIMMING_HRC_4MHZ == (x))              ||                             \
    (CTC_TRIMMING_HRC_6MHZ == (x))              ||                             \
    (CTC_TRIMMING_HRC_8MHZ == (x))              ||                             \
    (CTC_TRIMMING_HRC_12MHZ == (x))             ||                             \
    (CTC_TRIMMING_HRC_16MHZ == (x))             ||                             \
    (CTC_TRIMMING_HRC_24MHZ == (x))             ||                             \
    (CTC_TRIMMING_HRC_32MHZ == (x))             ||                             \
    (CTC_TRIMMING_HRC_48MHZ == (x))             ||                             \
    (CTC_TRIMMING_HRC_1P5MHZ == (x)))

#define IS_CTC_OFFSET_VAL(x)                    ((x) <= CTC_CR2_OFSVAL)

#define IS_CTC_RELOAD_VAL(x)                    ((x) <= (CTC_CR2_RLDVAL >> CTC_CR2_RLDVAL_POS))

#define IS_CTC_TRM_VAL(x)                       ((x) <= (CTC_CR1_TRMVAL >> CTC_CR1_TRMVAL_POS))

#define IS_CTC_TOLERANCE_BIAS(x)                ((x) <= CTC_DEFAULT_TOLERANCE_BIAS)

#define IS_CTC_FLAG(x)                                                         \
(    (x) & (CTC_FLAG_TRMOK | CTC_FLAG_TRMOVF | CTC_FLAG_TRMUDF | CTC_FLAG_BUSY))

#define IS_CTC_REFCLK_PRESCALER_DIV(x)                                         \
(   (CTC_REFCLK_PRESCALER_DIV8 == (x))          ||                             \
    (CTC_REFCLK_PRESCALER_DIV32 == (x))         ||                             \
    (CTC_REFCLK_PRESCALER_DIV128 == (x))        ||                             \
    (CTC_REFCLK_PRESCALER_DIV256 == (x))        ||                             \
    (CTC_REFCLK_PRESCALER_DIV512 == (x))        ||                             \
    (CTC_REFCLK_PRESCALER_DIV1024 == (x))       ||                             \
    (CTC_REFCLK_PRESCALER_DIV2048 == (x))       ||                             \
    (CTC_REFCLK_PRESCALER_DIV4096 == (x)))

#define IS_CTC_REFCLK_SEL(x)                                                   \
(   (CTC_REFCLK_XTAL == (x))                    ||                             \
    (CTC_REFCLK_CTCREF == (x)))

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
 * @defgroup CTC_Global_Functions CTC Global Functions
 * @{
 */

/**
 * @brief  Initialize CTC function.
 * @param  [in] pstcInit                Pointer to a @ref stc_ctc_init_t structure (CTC function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorNotReady: CTC state is busy
 *           - ErrorInvalidParameter:   If one of following cases matches:
 *                                      - pstcInit == NULL
 *                                      - Referenc frequency is out of range
 *                                      - Calculate reload & offset value out of range
 */
en_result_t CTC_Init(const stc_ctc_init_t *pstcInit)
{
    float f32OffsetVal = 0.0f;
    uint32_t u32ReloadVal = 0ul;
    uint32_t u32OffsetVal = 0ul;
    uint32_t u32RefclkDiv = 0ul;
    uint32_t u32Multiplier = 0ul;
    uint64_t u64InterRefclk = 0ul;
    en_result_t enRet = ErrorNotReady;

    /* Check CTC status */
    if (!(READ_REG32_BIT(M0P_CTC->STR, CTC_FLAG_BUSY)))
    {
        enRet = ErrorInvalidParameter;

        /* Check parameters */
        if ((NULL != pstcInit) && (IS_CTC_FREF(pstcInit->u32Fref)))
        {
            /* Check parameters */
            DDL_ASSERT(IS_CTC_FHRC(pstcInit->u32Fhrc));
            DDL_ASSERT(IS_CTC_FREF(pstcInit->u32Fref));
            DDL_ASSERT(IS_CTC_TRM_VAL(pstcInit->u32TrmVal));
            DDL_ASSERT(IS_CTC_REFCLK_SEL(pstcInit->u32RefClkSel));
            DDL_ASSERT(IS_CTC_TOLERANCE_BIAS(pstcInit->f32ToleranceBias));
            DDL_ASSERT(IS_CTC_REFCLK_PRESCALER_DIV(pstcInit->u32RefclkPrescaler));

            if (pstcInit->u32RefclkPrescaler < CTC_REFCLK_PRESCALER_DIV128)
            {
                u32RefclkDiv = (8ul << (2ul * pstcInit->u32RefclkPrescaler));
            }
            else
            {
                u32RefclkDiv = (32ul << pstcInit->u32RefclkPrescaler);
            }
            u64InterRefclk = ((uint64_t)(pstcInit->u32Fhrc)) * ((uint64_t)(u32RefclkDiv));
            u32Multiplier = (uint32_t)(u64InterRefclk / pstcInit->u32Fref);

            /* Calculate offset value formula: OFSVAL = (Fhrc / (Fref * Fref_divsion)) * TA */
            f32OffsetVal = ((float)u32Multiplier) * (pstcInit->f32ToleranceBias);
            u32OffsetVal = (uint32_t)(f32OffsetVal);

            /* Calculate reload value formula: RLDVAL = (Fhrc / (Fref * Fref_divsion)) + OFSVAL */
            u32ReloadVal = u32Multiplier + u32OffsetVal;

            if ((IS_CTC_OFFSET_VAL(u32OffsetVal)) && (IS_CTC_RELOAD_VAL(u32ReloadVal)))
            {
                /* Set CR1 */
                WRITE_REG32(M0P_CTC->CR1,                                      \
                            (pstcInit->u32RefclkPrescaler |                    \
                             pstcInit->u32RefClkSel       |                    \
                             (pstcInit->u32TrmVal << CTC_CR1_TRMVAL_POS)));

                /* Set CR2 */
                WRITE_REG32(M0P_CTC->CR2, ((u32ReloadVal << CTC_CR2_RLDVAL_POS) | u32OffsetVal));
                enRet = Ok;
            }
        }
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_uart_init_t to default values.
 * @param  [out] pstcInit               Pointer to a @ref stc_ctc_init_t structure (CTC function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcInit is NULL pointer
 */
en_result_t CTC_StructInit(stc_ctc_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (NULL != pstcInit)
    {
        pstcInit->u32Fhrc = 0ul;
        pstcInit->u32Fref = 0ul;
        pstcInit->u32RefClkSel = CTC_REFCLK_CTCREF;
        pstcInit->f32ToleranceBias = CTC_DEFAULT_TOLERANCE_BIAS;
        pstcInit->u32RefclkPrescaler = CTC_REFCLK_PRESCALER_DIV8;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-Initialize CTC function.
 * @param  None
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 *           - ErrorNotReady: CTC state is busy
 */
en_result_t CTC_DeInit(void)
{
    en_result_t enRet = ErrorNotReady;

    /* Check CTC status */
    if (!(READ_REG32_BIT(M0P_CTC->STR, CTC_FLAG_BUSY)))
    {
        /* Configures the registers to reset value. */
        WRITE_REG32(M0P_CTC->CR1, 0x80000000ul);
        WRITE_REG32(M0P_CTC->CR2, 0x00000000ul);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set CTC reference clock prescaler.
 * @param  [in] u32Prescaler                    CTC reference clock prescaler
 *         This parameter can be one of the following values:
 *           @arg CTC_REFCLK_PRESCALER_DIV8:    REFCLK/8
 *           @arg CTC_REFCLK_PRESCALER_DIV32:   REFCLK/32
 *           @arg CTC_REFCLK_PRESCALER_DIV128:  REFCLK/128
 *           @arg CTC_REFCLK_PRESCALER_DIV256:  REFCLK/256
 *           @arg CTC_REFCLK_PRESCALER_DIV512:  REFCLK/512
 *           @arg CTC_REFCLK_PRESCALER_DIV1024: REFCLK/1024
 *           @arg CTC_REFCLK_PRESCALER_DIV2048: REFCLK/2048
 *           @arg CTC_REFCLK_PRESCALER_DIV4096: REFCLK/4096
 * @retval None
 */
void CTC_SetRefClkPrescaler(uint32_t u32Prescaler)
{
    /* Check parameters */
    DDL_ASSERT(IS_CTC_REFCLK_PRESCALER_DIV(u32Prescaler));

    MODIFY_REG32(M0P_CTC->CR1, CTC_CR1_REFPSC, u32Prescaler);
}

/**
 * @brief  Get CTC reference clock prescaler.
 * @param  None
 * @retval Returned value can be one of the following values:
 *           @arg CTC_REFCLK_PRESCALER_DIV8:    REFCLK/8
 *           @arg CTC_REFCLK_PRESCALER_DIV32:   REFCLK/32
 *           @arg CTC_REFCLK_PRESCALER_DIV128:  REFCLK/128
 *           @arg CTC_REFCLK_PRESCALER_DIV256:  REFCLK/256
 *           @arg CTC_REFCLK_PRESCALER_DIV512:  REFCLK/512
 *           @arg CTC_REFCLK_PRESCALER_DIV1024: REFCLK/1024
 *           @arg CTC_REFCLK_PRESCALER_DIV2048: REFCLK/2048
 *           @arg CTC_REFCLK_PRESCALER_DIV4096: REFCLK/4096
 */
uint32_t CTC_GetRefClkPrescaler(void)
{
    return READ_REG32_BIT(M0P_CTC->CR1, CTC_CR1_REFPSC);
}

/**
 * @brief  Set CTC reference clock source.
 * @param  [in] u32RefClk                       CTC reference clock source
 *         This parameter can be one of the following values:
 *           @arg CTC_REFCLK_CTCREF:            Clock source CTCREF
 *           @arg CTC_REFCLK_XTAL:              Clock source XTAL
 * @retval None
 */
void CTC_SetRefClkSource(uint32_t u32RefClk)
{
    /* Check parameters */
    DDL_ASSERT(IS_CTC_REFCLK_SEL(u32RefClk));

    MODIFY_REG32(M0P_CTC->CR1, CTC_CR1_REFCKS, u32RefClk);
}

/**
 * @brief  Get CTC reference clock source.
 * @param  None
 * @retval Returned value can be one of the following values:
 *           @arg CTC_REFCLK_CTCREF:            Clock source CTCREF
 *           @arg CTC_REFCLK_XTAL:              Clock source XTAL
 */
uint32_t CTC_GetRefClkSource(void)
{
    return READ_REG32_BIT(M0P_CTC->CR1, CTC_CR1_REFCKS);
}

/**
 * @brief  Get CTC flag.
 * @param  [in] u32Flag                         CTC flag
 *         This parameter can be any composed value of the following values::
 *           @arg CTC_FLAG_TRMOK:               Trimming OK flag
 *           @arg CTC_FLAG_TRMOVF:              Trimming overflow flag
 *           @arg CTC_FLAG_TRMUDF:              Trimming underflow flag
 *           @arg CTC_FLAG_BUSY:                CTC busy flag
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Flag is set
 *           - Reset: Flag is reset
 */
en_flag_status_t CTC_GetFlag(uint32_t u32Flag)
{
    return ((u32Flag == READ_REG32_BIT(M0P_CTC->STR, u32Flag)) ? Set : Reset);
}

/**
 * @brief  Set CTC trimming value.
 * @param  [in] u8TrmVal                CTC trimming value
 *         This parameter can be min=0 && max=0x3F
 * @retval None
 */
void CTC_SetTrmVal(uint8_t u8TrmVal)
{
    /* Check parameters */
    DDL_ASSERT(IS_CTC_TRM_VAL(u8TrmVal));

    MODIFY_REG32(M0P_CTC->CR1, CTC_CR1_TRMVAL, (((uint32_t)u8TrmVal) << CTC_CR1_TRMVAL_POS));
}

/**
 * @brief  Get CTC trimming value.
 * @param  None
 * @retval CTC trimming value(between Min_Data=0 and Max_Data=0x3F)
 */
uint8_t CTC_GetTrmVal(void)
{
    return ((uint8_t)(READ_REG32(M0P_CTC->CR1) >> CTC_CR1_TRMVAL_POS));
}

/**
 * @brief  Set CTC reload value.
 * @param  [in] u16ReloadVal                CTC reload value
 *         This parameter can be between Min_Data=0 and Max_Data=0xFFFF
 * @retval None
 */
void CTC_SetReloadVal(uint16_t u16ReloadVal)
{
    MODIFY_REG32(M0P_CTC->CR2, CTC_CR2_RLDVAL, (((uint32_t)u16ReloadVal) << CTC_CR2_RLDVAL_POS));
}

/**
 * @brief  Get CTC reload value.
 * @param  None
 * @retval CTC reload value (between Min_Data=0 and Max_Data=0xFFFF)
 */
uint16_t CTC_GetReloadVal(void)
{
    return ((uint16_t)(READ_REG32(M0P_CTC->CR2) >> CTC_CR2_RLDVAL_POS));
}

/**
 * @brief  Set CTC offset value.
 * @param  [in] u8OffsetVal                CTC offset value
 *         This parameter can be between Min_Data=0 and Max_Data=0xFF
 * @retval None
 */
void CTC_SetOffsetVal(uint8_t u8OffsetVal)
{
    MODIFY_REG32(M0P_CTC->CR2, CTC_CR2_OFSVAL, (((uint32_t)u8OffsetVal) << CTC_CR2_OFSVAL_POS));
}

/**
 * @brief  Get CTC offset value.
 * @param  None
 * @retval CTC offset value (between Min_Data=0 and Max_Data=0xFF)
 */
uint8_t CTC_GetOffsetVal(void)
{
    return ((uint8_t)(READ_REG32(M0P_CTC->CR2) >> CTC_CR2_OFSVAL_POS));
}

/**
 * @}
 */

#endif /* DDL_CTC_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
