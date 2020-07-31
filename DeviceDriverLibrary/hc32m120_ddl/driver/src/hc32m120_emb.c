/**
 *******************************************************************************
 * @file  hc32m120_emb.c
 * @brief This file provides firmware functions to manage the EMB
 *        (Emergency Brake).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-27       Hongjh          First version
   2020-03-14       Hongjh          Delete unused macro-define
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
#include "hc32m120_emb.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_EMB EMB
 * @brief Emergency Brake Driver Library
 * @{
 */

#if (DDL_EMB_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EMB_Local_Macros EMB Local Macros
 * @{
 */

/**
 * @defgroup EMB_Check_Parameters_Validity EMB Check Parameters Validity
 * @{
 */

#define IS_VALID_EMB_PORT_LEVEL(x)                                             \
(   ((x) == EMB_PORT_LEVEL_LOW)                 ||                             \
    ((x) == EMB_PORT_LEVEL_HIGH))

#define IS_VALID_EMB_INT(x)                                                    \
(   ((x) == EMB_INT_OSC)                        ||                             \
    ((x) == EMB_INT_CMP)                        ||                             \
    ((x) == EMB_INT_PWM)                        ||                             \
    ((x) == EMB_INT_PORT))

#define IS_VALID_EMB_PORT_FILTER_DIV(x)                                        \
(   ((x) == EMB_PORT_FILTER_NONE)               ||                             \
    ((x) == EMB_PORT_FILTER_CLK_DIV1)           ||                             \
    ((x) == EMB_PORT_FILTER_CLK_DIV8)           ||                             \
    ((x) == EMB_PORT_FILTER_CLK_DIV32)          ||                             \
    ((x) == EMB_PORT_FILTER_CLK_DIV128))

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
 * @defgroup EMB_Global_Functions EMB Global Functions
 * @{
 */

/**
 * @brief  Initialize EMB.
 * @param  [in] pstcInit                Pointer to a @ref stc_emb_ctrl_timer4_init_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t EMB_Init(const stc_emb_ctrl_timer4_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (NULL != pstcInit)
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enPortCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enCmp1Cmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enCmp2Cmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enOscCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enTimer4PwmWCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enTimer4PwmVCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enTimer4PwmUCmd));
        DDL_ASSERT(IS_VALID_EMB_PORT_LEVEL(pstcInit->u32PortLevel));
        DDL_ASSERT(IS_VALID_EMB_PORT_FILTER_DIV(pstcInit->u32PortFilterDiv));

        /* Set default value */
        WRITE_REG32(M0P_EMB->STATCLR, 0x0Ful);
        WRITE_REG32(M0P_EMB->INTEN, 0x00ul);
        WRITE_REG32(M0P_EMB->SOE, 0x00ul);

        WRITE_REG32(M0P_EMB->CTL,                                               \
                    ((pstcInit->enPortCmd << EMB_CTL_PORTINEN_POS)            | \
                     (pstcInit->enCmp1Cmd << EMB_CTL_CMPEN_POS)               | \
                     (pstcInit->enCmp2Cmd << (EMB_CTL_CMPEN_POS + 1ul))       | \
                     (pstcInit->enOscCmd << EMB_CTL_OSCSTPEN_POS)             | \
                     (pstcInit->enTimer4PwmWCmd << EMB_CTL_PWMSEL_POS)        | \
                     (pstcInit->enTimer4PwmVCmd << (EMB_CTL_PWMSEL_POS + 1ul))| \
                     (pstcInit->enTimer4PwmUCmd << (EMB_CTL_PWMSEL_POS + 2ul))| \
                     pstcInit->u32PortLevel                                   | \
                     pstcInit->u32PortFilterDiv));
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_emb_ctrl_timer4_init_t to default values
 * @param  [out] pstcInit               Pointer to a @ref stc_emb_ctrl_timer4_init_t structure (M0P_EMB unit function configuration data structure)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t EMB_StructInit(stc_emb_ctrl_timer4_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (NULL != pstcInit)
    {
        pstcInit->enPortCmd = Disable;
        pstcInit->enCmp1Cmd = Disable;
        pstcInit->enCmp2Cmd = Disable;
        pstcInit->enOscCmd = Disable;
        pstcInit->enTimer4PwmWCmd = Disable;
        pstcInit->enTimer4PwmVCmd = Disable;
        pstcInit->enTimer4PwmUCmd = Disable;

        pstcInit->u32PortLevel = EMB_PORT_LEVEL_HIGH;
        pstcInit->u32PortFilterDiv = EMB_PORT_FILTER_NONE;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-Initialize EMB function
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 */
en_result_t EMB_DeInit(void)
{
    /* Configures the registers to reset value. */
    WRITE_REG32(M0P_EMB->STATCLR, 0x0FUL);
    WRITE_REG32(M0P_EMB->INTEN, 0x00UL);
    WRITE_REG32(M0P_EMB->SOE, 0x00UL);

    return Ok;
}

/**
 * @brief  Set the EMB interrupt function
 * @param  [in] u32IntSource            TimerB interrupt source
 *         This parameter can be one of the following values:
 *           @arg EMB_INT_PORT: Port input trigger stop PWM
 *           @arg EMB_INT_PWM:  PWM same phase trigger stop PWM
 *           @arg EMB_INT_CMP:  CMP trigger stop PWM
 *           @arg EMB_INT_OSC:  OSC trigger stop PWM
 * @param  [in] enNewSta                The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval None
 */
void EMB_IntCmd(uint32_t u32IntSource, en_functional_state_t enNewSta)
{
    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));
    DDL_ASSERT(IS_VALID_EMB_INT(u32IntSource));

    switch (u32IntSource)
    {
        case EMB_INT_PORT:
            bM0P_EMB->INTEN_b.PORTINTEN = enNewSta;
            break;
        case EMB_INT_PWM:
            bM0P_EMB->INTEN_b.PWMINTEN = enNewSta;
            break;
        case EMB_INT_CMP:
            bM0P_EMB->INTEN_b.CMPINTEN = enNewSta;
            break;
        case EMB_INT_OSC:
            bM0P_EMB->INTEN_b.OSINTEN = enNewSta;
            break;
        default:
            break;
    }
}

/**
 * @brief  Get EMB port filter clock division
 * @param  None
 * @retval Returned value can be one of the following values:
 *           @arg EMB_PORT_FILTER_NONE:            Disable filter
 *           @arg EMB_PORT_FILTER_CLK_DIV1:        PCLK
 *           @arg EMB_PORT_FILTER_CLK_DIV8:        PCLK/2
 *           @arg EMB_PORT_FILTER_CLK_DIV32:       PCLK/4
 *           @arg EMB_PORT_FILTER_CLK_DIV128:      PCLK/8
 */
uint32_t EMB_GetPortFilterClkDiv(void)
{
    return READ_REG32_BIT(M0P_EMB->CTL, (EMB_CTL_NFEN | EMB_CTL_NFSEL));
}

/**
 * @brief  Set EMB detection port level
 * @param  [in] stcPwmLevel             Timer4 PWM level a @ref stc_emb_timer4_pwm_level_t structure
 * @retval None
 */
void EMB_SetDetectPwmLevel(stc_emb_timer4_pwm_level_t stcPwmLevel)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(stcPwmLevel.enPwmWHighLvl));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(stcPwmLevel.enPwmVHighLvl));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(stcPwmLevel.enPwmUHighLvl));

    WRITE_REG32(M0P_EMB->PWMLV, \
      (((uint32_t)(stcPwmLevel.enPwmWHighLvl)) << EMB_PWMLV_PWMLV_POS)         | \
      (((uint32_t)(stcPwmLevel.enPwmVHighLvl)) << (EMB_PWMLV_PWMLV_POS + 1ul)) | \
      (((uint32_t)(stcPwmLevel.enPwmUHighLvl)) << (EMB_PWMLV_PWMLV_POS + 2ul)));
}

/**
 * @brief  Get EMB detection port level
 * @param  None
 * @retval Timer4 PWM level a @ref stc_emb_timer4_pwm_level_t structure
 */
stc_emb_timer4_pwm_level_t EMB_GetDetectPwmLevel(void)
{
    stc_emb_timer4_pwm_level_t stcPwmLevel;

    stcPwmLevel.enPwmWHighLvl = (en_functional_state_t)(bM0P_EMB->PWMLV_b.PWMLV0);
    stcPwmLevel.enPwmVHighLvl = (en_functional_state_t)(bM0P_EMB->PWMLV_b.PWMLV1);
    stcPwmLevel.enPwmUHighLvl = (en_functional_state_t)(bM0P_EMB->PWMLV_b.PWMLV2);

    return stcPwmLevel;
}

/**
 * @}
 */

#endif /* DDL_EMB_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
