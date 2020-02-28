/**
 *******************************************************************************
 * @file  hc32m120_swdt.c
 * @brief This file provides firmware functions to manage the Special Watch Dog
 *        Timer(SWDT).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-26       Yangjp          First version
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
#include "hc32m120_swdt.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_SWDT SWDT
 * @brief Special Watch Dog Timer
 * @{
 */

#if (DDL_SWDT_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SWDT_Local_Macros SWDT Local Macros
 * @{
 */

/**
 * @defgroup SWDT_Registers_Clear_Mask SWDT Registers Clear Mask
 * @{
 */
#define SWDT_CR_CLEAR_MASK              (SWDT_CR_PERI | SWDT_CR_CKS | SWDT_CR_WDPT | \
                                         SWDT_CR_SLPOFF | SWDT_CR_ITS)
/**
 * @}
 */

/**
 * @defgroup SWDT_Refresh_Key SWDT Refresh Key
 * @{
 */
#define SWDT_REFRESH_KEY_START          ((uint16_t)0x0123u)
#define SWDT_REFRESH_KEY_END            ((uint16_t)0x3210u)
/**
 * @}
 */

/**
 * @defgroup SWDT_Check_Parameters_Validity SWDT Check Parameters Validity
 * @{
 */
#define IS_SWDT_COUNTER_CYCLE(x)                                               \
(   (SWDT_COUNTER_CYCLE_256 == (x))             ||                             \
    (SWDT_COUNTER_CYCLE_4096 == (x))            ||                             \
    (SWDT_COUNTER_CYCLE_16384 == (x))           ||                             \
    (SWDT_COUNTER_CYCLE_65536 == (x)))

#define IS_SWDT_CLOCK_DIVISION(x)                                              \
(   (SWDT_CLOCK_DIV1 == (x))                    ||                             \
    (SWDT_CLOCK_DIV16 == (x))                   ||                             \
    (SWDT_CLOCK_DIV32 == (x))                   ||                             \
    (SWDT_CLOCK_DIV64 == (x))                   ||                             \
    (SWDT_CLOCK_DIV128 == (x))                  ||                             \
    (SWDT_CLOCK_DIV256 == (x))                  ||                             \
    (SWDT_CLOCK_DIV2048 == (x)))

#define IS_SWDT_ALLOW_REFRESH_RANGE(x)                                         \
(   (SWDT_RANGE_100PCT == (x))                  ||                             \
    (SWDT_RANGE_0TO25PCT == (x))                ||                             \
    (SWDT_RANGE_25TO50PCT == (x))               ||                             \
    (SWDT_RANGE_0TO50PCT == (x))                ||                             \
    (SWDT_RANGE_50TO75PCT == (x))               ||                             \
    (SWDT_RANGE_0TO25PCT_50TO75PCT == (x))      ||                             \
    (SWDT_RANGE_25TO75PCT == (x))               ||                             \
    (SWDT_RANGE_0TO75PCT == (x))                ||                             \
    (SWDT_RANGE_75TO100PCT == (x))              ||                             \
    (SWDT_RANGE_0TO25PCT_75TO100PCT == (x))     ||                             \
    (SWDT_RANGE_25TO50PCT_75TO100PCT == (x))    ||                             \
    (SWDT_RANGE_0TO50PCT_75TO100PCT == (x))     ||                             \
    (SWDT_RANGE_50TO100PCT == (x))              ||                             \
    (SWDT_RANGE_0TO25PCT_50TO100PCT == (x))     ||                             \
    (SWDT_RANGE_25TO100PCT == (x))              ||                             \
    (SWDT_RANGE_0TO100PCT == (x)))

#define IS_SWDT_LPW_MODE_COUNT(x)                                              \
(   (SWDT_LPW_MODE_COUNT_CONTINUE == (x))   ||                                 \
    (SWDT_LPW_MODE_COUNT_STOP == (x)))

#define IS_SWDT_REQUEST_TYPE(x)                                                \
(   (SWDT_TRIG_EVENT_INT == (x))                ||                             \
    (SWDT_TRIG_EVENT_RESET == (x)))

#define IS_SWDT_FLAG(x)                                                        \
(   (SWDT_FLAG_COUNT_UNDERFLOW == (x))          ||                             \
    (SWDT_FLAG_REFRESH_ERROR == (x)))
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
 * @defgroup SWDT_Global_Functions SWDT Global Functions
 * @{
 */

/**
 * @brief  Initialize SWDT.
 * @param  [in] pstcSwdtInit            Pointer to a stc_swdt_init_t structure
 *                                      that contains configuration information.
 *   @arg  See the struct @ref stc_swdt_init_t.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t SWDT_Init(const stc_swdt_init_t *pstcSwdtInit)
{
    en_result_t enRet = Ok;

    if(NULL == pstcSwdtInit)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_SWDT_COUNTER_CYCLE(pstcSwdtInit->u32CountCycle));
        DDL_ASSERT(IS_SWDT_CLOCK_DIVISION(pstcSwdtInit->u32ClockDivision));
        DDL_ASSERT(IS_SWDT_ALLOW_REFRESH_RANGE(pstcSwdtInit->u32RefreshRange));
        DDL_ASSERT(IS_SWDT_LPW_MODE_COUNT(pstcSwdtInit->u32LPModeCountEn));
        DDL_ASSERT(IS_SWDT_REQUEST_TYPE(pstcSwdtInit->u32RequestType));

        /* SWDT CR Configuration(Software Start Mode) */
        MODIFY_REG32(M0P_SWDT->CR, SWDT_CR_CLEAR_MASK,
                    (pstcSwdtInit->u32CountCycle   | pstcSwdtInit->u32ClockDivision |
                     pstcSwdtInit->u32RefreshRange | pstcSwdtInit->u32LPModeCountEn |
                     pstcSwdtInit->u32RequestType));
    }

    return enRet;
}

/**
 * @brief  SWDT reload counter
 * @note   In software startup mode, Start counter when refreshing for the first time.
 * @param  None
 * @retval None
 */
void SWDT_ReloadCounter(void)
{
    M0P_SWDT->RR = SWDT_REFRESH_KEY_START;
    M0P_SWDT->RR = SWDT_REFRESH_KEY_END;
}

/**
 * @brief  Get SWDT flag status.
 * @param  [in] u32Flag                 Specifies the SWDT flag type.
 *                                      This parameter can be one of the following values:
 *   @arg  SWDT_FLAG_COUNT_UNDERFLOW: Count Underflow flag
 *   @arg  SWDT_FLAG_REFRESH_ERROR:   Refresh Error flag
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Flag is set
 *           - Reset: Flag is reset
 */
en_flag_status_t SWDT_GetFlag(uint32_t u32Flag)
{
    en_flag_status_t enFlagSta = Reset;

    /* Check parameters */
    DDL_ASSERT(IS_SWDT_FLAG(u32Flag));

    if (Reset != (READ_REG32_BIT(M0P_SWDT->SR, u32Flag)))
    {
        enFlagSta = Set;
    }

    return enFlagSta;
}

/**
 * @brief  Clear SWDT flag.
 * @param  [in] u32Flag                 Specifies the SWDT flag type.
 *                                      This parameter can be one or a combination of the following values:
 *   @arg  SWDT_FLAG_COUNT_UNDERFLOW: Count Underflow flag
 *   @arg  SWDT_FLAG_REFRESH_ERROR:   Refresh Error flag
 * @retval An en_result_t enumeration value:
 *           - Ok: Clear flag success
 *           - ErrorTimeout: Clear flag timeout
 */
en_result_t SWDT_ClearFlag(uint32_t u32Flag)
{
    uint32_t u32Timeout, u32TimeCnt = 0ul;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_SWDT_FLAG(u32Flag));

    /* Waiting for FLAG bit clear */
    u32Timeout = SystemCoreClock >> 8u;
    do
    {
        CLEAR_REG32_BIT(M0P_SWDT->SR, u32Flag);
        u32TimeCnt++;
    } while ((Reset != (READ_REG32_BIT(M0P_SWDT->SR, u32Flag))) && (u32TimeCnt < u32Timeout));

    if (u32TimeCnt >= u32Timeout)
    {
        enRet = ErrorTimeout;
    }

    return enRet;
}

/**
 * @}
 */

#endif /* DDL_SWDT_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
