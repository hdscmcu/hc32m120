/**
 *******************************************************************************
 * @file  hc32m120_timer2.c
 * @brief This file provides firmware functions to manage the TIMER2
 *        (TIMER2).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-20       Wuze            First version
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
#include "hc32m120_timer2.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_TIMER2 TIMER2
 * @brief TIMER2 Driver Library
 * @{
 */
#if (DDL_TIMER2_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMER2_Local_Macros TIMER2 Local Macros
 * @{
 */

/**
 * @brief Definitions of TIMER2 trigger event registers.
 */
#define TIMER2_TRIG_EVT_REG                 (M0P_AOS->TMR2_HTSSR)

/**
 * @brief TIMER2 PWM configuration mask.
 */
#define TIMER2_PWM_CFG_MSK                  (TMR2_PCONR_STACA | TMR2_PCONR_STPCA | TMR2_PCONR_CMPCA)

/**
 * @brief TIMER2 PWM comparison match polarity.
 */
#define TIMER2_PWM_CM_TOGGLE                ((uint16_t)TMR2_PCONR_CMPCA)

/**
 * @brief TIMER2 filter sample clock prescaler mask.
 */
#define TIMER2_FILTER_PRESCALER_MSK         (TMR2_PCONR_NOFICKA)

/**
 * @brief TIMER2 hardware controller configuration mask.
 */
#define TIMER2_HW_CFG_MSK                   (TMR2_HCONR_HSTA0 | TMR2_HCONR_HSTA1 | TMR2_HCONR_HSTA2 | \
                                             TMR2_HCONR_HSTP0 | TMR2_HCONR_HSTP1 | TMR2_HCONR_HSTP2 | \
                                             TMR2_HCONR_HCLE0 | TMR2_HCONR_HCLE1 | TMR2_HCONR_HCLE2 | \
                                             TMR2_HCONR_HICP0 | TMR2_HCONR_HICP1 | TMR2_HCONR_HICP2)

/**
 * @brief TRIGA condition.
 */
#define TIMER2_HW_START_TRIGA_MSK           (TMR2_HCONR_HSTA0 | TMR2_HCONR_HSTA1)
#define TIMER2_HW_STOP_TRIGA_MSK            (TMR2_HCONR_HSTP0 | TMR2_HCONR_HSTP1)
#define TIMER2_HW_CLR_TRIGA_MSK             (TMR2_HCONR_HCLE0 | TMR2_HCONR_HCLE1)
#define TIMER2_HW_CAP_TRIGA_MSK             (TMR2_HCONR_HICP0 | TMR2_HCONR_HICP1)

/**
 * @brief TIMER2 basic configuration mask.
 */
#define TIMER2_CLK_CFG_MSK                  (TMR2_BCONR_SYNSA | TMR2_BCONR_CKDIVA | TMR2_BCONR_SYNCLKA | TMR2_BCONR_ASYNCLKA)

/**
 * @defgroup TIMER2_Check_Parameters_Validity TIMER2 check parameters validity
 * @{
 */
#define IS_TIMER2_CLK_SOURCE(x)                                                \
(   ((x) == TIMER2_SYNC_CS_HCLK)            ||                                 \
    ((x) == TIMER2_SYNC_CS_TRIGA_RISE)      ||                                 \
    ((x) == TIMER2_SYNC_CS_TRIGA_FALL)      ||                                 \
    ((x) == TIMER2_SYNC_CS_EVENT)           ||                                 \
    ((x) == TIMER2_ASYNC_CS_LRC)            ||                                 \
    ((x) == TIMER2_ASYNC_CS_CLKA))

#define IS_TIMER2_CLK_PRESCALER(x)                                             \
(   ((x) == TIMER2_CLK_PRESCALER_1)         ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_2)         ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_4)         ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_8)         ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_16)        ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_32)        ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_64)        ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_128)       ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_256)       ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_512)       ||                                 \
    ((x) == TIMER2_CLK_PRESCALER_1024))

#define IS_TIMER2_FILTER_PRESCALER(x)                                          \
(   ((x) == TIMER2_FILTER_PRESCALER_1)      ||                                 \
    ((x) == TIMER2_FILTER_PRESCALER_4)      ||                                 \
    ((x) == TIMER2_FILTER_PRESCALER_16)     ||                                 \
    ((x) == TIMER2_FILTER_PRESCALER_64))

#define IS_TIMER2_START_CONDITION(x)                                           \
(   ((x) == TIMER2_HW_START_INVALID)        ||                                 \
    ((x) == TIMER2_HW_START_TRIGA_RISE)     ||                                 \
    ((x) == TIMER2_HW_START_TRIGA_FALL)     ||                                 \
    ((x) == TIMER2_HW_START_EVENT))

#define IS_TIMER2_STOP_CONDITION(x)                                            \
(   ((x) == TIMER2_HW_STOP_INVALID)         ||                                 \
    ((x) == TIMER2_HW_STOP_TRIGA_RISE)      ||                                 \
    ((x) == TIMER2_HW_STOP_TRIGA_FALL)      ||                                 \
    ((x) == TIMER2_HW_STOP_EVENT))

#define IS_TIMER2_CLEAR_CONDITION(x)                                           \
(   ((x) == TIMER2_HW_CLR_INVALID)          ||                                 \
    ((x) == TIMER2_HW_CLR_TRIGA_RISE)       ||                                 \
    ((x) == TIMER2_HW_CLR_TRIGA_FALL)       ||                                 \
    ((x) == TIMER2_HW_CLR_EVENT))

#define IS_TIMER2_CAPTURE_CONDITION(x)                                         \
(   ((x) == TIMER2_HW_CAP_INVALID)          ||                                 \
    ((x) == TIMER2_HW_CAP_TRIGA_RISE)       ||                                 \
    ((x) == TIMER2_HW_CAP_TRIGA_FALL)       ||                                 \
    ((x) == TIMER2_HW_CAP_EVENT))

#define IS_TIMER2_PWM_START_POLARITY(x)                                        \
(   ((x) == TIMER2_PWM_START_LOW)           ||                                 \
    ((x) == TIMER2_PWM_START_HIGH)          ||                                 \
    ((x) == TIMER2_PWM_START_KEEP))

#define IS_TIMER2_PWM_STOP_POLARITY(x)                                         \
(   ((x) == TIMER2_PWM_STOP_LOW)            ||                                 \
    ((x) == TIMER2_PWM_STOP_HIGH)           ||                                 \
    ((x) == TIMER2_PWM_STOP_KEEP))

#define IS_TIMER2_TRIGGER_EVT(x)                                               \
(   ((x) == EVT_PORT_EIRQ0)                 ||                                 \
    ((x) == EVT_PORT_EIRQ8)                 ||                                 \
    ((x) == EVT_DMA_1_TC0)                  ||                                 \
    ((x) == EVT_DAM_1_BTC0)                 ||                                 \
    ((x) == EVT_PORT_EIRQ1)                 ||                                 \
    ((x) == EVT_PORT_EIRQ9)                 ||                                 \
    ((x) == EVT_DMA_2_TC0)                  ||                                 \
    ((x) == EVT_DAM_2_BTC0)                 ||                                 \
    ((x) == EVT_USART_1_RI)                 ||                                 \
    ((x) == EVT_PORT_EIRQ2)                 ||                                 \
    ((x) == EVT_TMR4_SCMUH)                 ||                                 \
    ((x) == EVT_TMR4_SCMUL)                 ||                                 \
    ((x) == EVT_TMR4_SCMVH)                 ||                                 \
    ((x) == EVT_TMR4_SCMVL)                 ||                                 \
    ((x) == EVT_TMR4_SCMWH)                 ||                                 \
    ((x) == EVT_TMR4_SCMWL)                 ||                                 \
    ((x) == EVT_USART_1_TI)                 ||                                 \
    ((x) == EVT_PORT_EIRQ3)                 ||                                 \
    ((x) == EVT_TMR0_GCMP)                  ||                                 \
    ((x) == EVT_AOS_STRG)                   ||                                 \
    ((x) == EVT_USART_3_RI)                 ||                                 \
    ((x) == EVT_SPI_SPRI)                   ||                                 \
    ((x) == EVT_PORT_EIRQ4)                 ||                                 \
    ((x) == EVT_TMRA_OVF)                   ||                                 \
    ((x) == EVT_TMRA_UDF)                   ||                                 \
    ((x) == EVT_TMRA_CMP)                   ||                                 \
    ((x) == EVT_TMRB_3_OVF)                 ||                                 \
    ((x) == EVT_TMRB_3_UDF)                 ||                                 \
    ((x) == EVT_TMRB_4_CMP)                 ||                                 \
    ((x) == EVT_ADC_EOCA)                   ||                                 \
    ((x) == EVT_USART_3_TI)                 ||                                 \
    ((x) == EVT_PORT_EIRQ5)                 ||                                 \
    ((x) == EVT_TMR2_GCMP)                  ||                                 \
    ((x) == EVT_TMRB_4_OVF)                 ||                                 \
    ((x) == EVT_TMRB_4_UDF)                 ||                                 \
    ((x) == EVT_TMRB_3_CMP)                 ||                                 \
    ((x) == EVT_ADC_EOCB)                   ||                                 \
    ((x) == EVT_USART_2_RI)                 ||                                 \
    ((x) == EVT_IIC_RXI)                    ||                                 \
    ((x) == EVT_PORT_EIRQ6)                 ||                                 \
    ((x) == EVT_CMP_1_IRQ)                  ||                                 \
    ((x) == EVT_EVENT_PORT)                 ||                                 \
    ((x) == EVT_TMRB_1_OVF)                 ||                                 \
    ((x) == EVT_TMRB_1_UDF)                 ||                                 \
    ((x) == EVT_TMRB_2_CMP)                 ||                                 \
    ((x) == EVT_ADC_CMP0)                   ||                                 \
    ((x) == EVT_USART_2_TI)                 ||                                 \
    ((x) == EVT_IIC_TXI)                    ||                                 \
    ((x) == EVT_PORT_EIRQ7)                 ||                                 \
    ((x) == EVT_CMP_2_IRQ)                  ||                                 \
    ((x) == EVT_FMC_OPTEND)                 ||                                 \
    ((x) == EVT_TMRB_2_OVF)                 ||                                 \
    ((x) == EVT_TMRB_2_UDF)                 ||                                 \
    ((x) == EVT_TMRB_1_CMP)                 ||                                 \
    ((x) == EVT_ADC_CMP1)                   ||                                 \
    ((x) == EVT_SPI_SPTI))

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
 * @defgroup TIMER2_Global_Functions TIMER2 Global Functions
 * @{
 */

/**
 * @brief  Set a default value for TIMER2 configuration structure.
 * @param  [in]  pstcCfg            Pointer to a stc_timer2_config_t structure that
 *                                  contains configuration information.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcCfg == NULL.
 */
en_result_t TIMER2_StructInit(stc_timer2_config_t *pstcCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcCfg != NULL)
    {
        pstcCfg->u16ClkSource           = TIMER2_SYNC_CS_HCLK;
        pstcCfg->u16ClkPrescaler        = TIMER2_CLK_PRESCALER_1;
        pstcCfg->u16HwStartCondition    = TIMER2_HW_START_INVALID;
        pstcCfg->u16HwStopCondition     = TIMER2_HW_STOP_INVALID;
        pstcCfg->u16HwClearCondition    = TIMER2_HW_CLR_INVALID;
        pstcCfg->u16HwCaptureCondition  = TIMER2_HW_CAP_INVALID;
        pstcCfg->u16PwmStartPolarity    = TIMER2_PWM_START_LOW;
        pstcCfg->u16PwmStopPolarity     = TIMER2_PWM_STOP_LOW;
        pstcCfg->u16FilterPrescaler     = TIMER2_FILTER_PRESCALER_1;
        pstcCfg->enFilterCmd            = Disable;
        pstcCfg->enMatchIntCmd          = Disable;
        pstcCfg->enOvfIntCmd            = Disable;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-initializes the TIMER2.
 * @param  None
 * @retval None
 */
void TIMER2_DeInit(void)
{
    TIMER2_Stop();

    M0P_TMR2->CNTAR = (uint32_t)0x0;
    M0P_TMR2->CMPAR = (uint32_t)0xFFFF;
    M0P_TMR2->BCONR = (uint32_t)0x0;
    M0P_TMR2->ICONR = (uint32_t)0x0;
    M0P_TMR2->PCONR = (uint32_t)0x0;
    M0P_TMR2->HCONR = (uint32_t)0x0;
    M0P_TMR2->STFLR = (uint32_t)0x0;
}

/**
 * @brief  Configures TIMER2 peripheral as a basic timer according to the specified parameters
 *         in the structure stc_timer2_config_t.
 * @param  [in]  pstcCfg            Pointer to a stc_timer2_config_t structure value that
 *                                  contains the configuration information for TIMER2.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:  -pstcCfg == NULL.
 *                                 -The stop condition is the same as the start condition.
 */
en_result_t TIMER2_TimerConfig(const stc_timer2_config_t *pstcCfg)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint16_t u16Temp;

    if (pstcCfg != NULL)
    {
        DDL_ASSERT(IS_TIMER2_CLK_SOURCE(pstcCfg->u16ClkSource));
        DDL_ASSERT(IS_TIMER2_CLK_PRESCALER(pstcCfg->u16ClkPrescaler));
        DDL_ASSERT(IS_TIMER2_START_CONDITION(pstcCfg->u16HwStartCondition));
        DDL_ASSERT(IS_TIMER2_STOP_CONDITION(pstcCfg->u16HwStopCondition));
        DDL_ASSERT(IS_TIMER2_CLEAR_CONDITION(pstcCfg->u16HwClearCondition));
        DDL_ASSERT(IS_TIMER2_FILTER_PRESCALER(pstcCfg->u16FilterPrescaler));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCfg->enFilterCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCfg->enMatchIntCmd));

        enRet = Ok;
        /* Start condition and stop condition CAN NOT be the same condition. */
        if ((pstcCfg->u16HwStartCondition != TIMER2_HW_START_INVALID) && \
            (pstcCfg->u16HwStopCondition  != TIMER2_HW_STOP_INVALID))
        {
            if ((pstcCfg->u16HwStopCondition >> TMR2_HCONR_HSTP0_POS) == pstcCfg->u16HwStartCondition)
            {
                enRet = ErrorInvalidParameter;
            }
        }

        if (enRet == Ok)
        {
            /* Set comparison value. */
            M0P_TMR2->CMPAR = (uint32_t)pstcCfg->u16CompareVal;

            /* Set start condition and stop condition. */
            u16Temp = (pstcCfg->u16HwStartCondition | \
                       pstcCfg->u16HwStopCondition  | \
                       pstcCfg->u16HwClearCondition);
            M0P_TMR2->HCONR = (uint32_t)u16Temp;

            /* Set the prescaler of TRIGA filter. */
            M0P_TMR2->PCONR = (uint32_t)pstcCfg->u16FilterPrescaler;

            /* Enable or Disable the TRIGA filter. */
            bM0P_TMR2->PCONR_b.NOFIENA = (uint32_t)pstcCfg->enFilterCmd;

            /* Enable or Disable the counter match interrupt. */
            bM0P_TMR2->ICONR_b.CMENA   = (uint32_t)pstcCfg->enMatchIntCmd;

            /* Enable or Disable the counter match interrupt. */
            bM0P_TMR2->ICONR_b.OVENA   = (uint32_t)pstcCfg->enOvfIntCmd;

            /* Set the clock source and clock prescaler. */
            u16Temp = pstcCfg->u16ClkSource | pstcCfg->u16ClkPrescaler;
            M0P_TMR2->BCONR = (uint32_t)u16Temp;
        }
    }

    return enRet;
}

/**
 * @brief  Configures TIMER2 peripheral as a PWM function according to the specified parameters
 *         in the structure stc_timer2_config_t.
 * @param  [in]  pstcCfg            Pointer to a stc_timer2_config_t structure value that
 *                                  contains the configuration information for TIMER2.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:  -pstcCfg == NULL.
 *                                 -The stop condition is the same as the start condition.
 *                                 -The start condition is the edge of TRIGA or the stop condition is the edge of TRIGA.
 */
en_result_t TIMER2_PwmConfig(const stc_timer2_config_t *pstcCfg)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint16_t u16StopCondition;
    uint16_t u16Temp;

    if (pstcCfg != NULL)
    {
        DDL_ASSERT(IS_TIMER2_CLK_SOURCE(pstcCfg->u16ClkSource));
        DDL_ASSERT(IS_TIMER2_CLK_PRESCALER(pstcCfg->u16ClkPrescaler));
        DDL_ASSERT(IS_TIMER2_START_CONDITION(pstcCfg->u16HwStartCondition));
        DDL_ASSERT(IS_TIMER2_STOP_CONDITION(pstcCfg->u16HwStopCondition));
        DDL_ASSERT(IS_TIMER2_PWM_START_POLARITY(pstcCfg->u16PwmStartPolarity));
        DDL_ASSERT(IS_TIMER2_PWM_STOP_POLARITY(pstcCfg->u16PwmStopPolarity));

        enRet = Ok;
        /* Start condition and stop condition CAN NOT be the same condition,
        and CAN NOT be the edge of TRIGA. */
        if ((pstcCfg->u16HwStartCondition != TIMER2_HW_START_INVALID) && \
            (pstcCfg->u16HwStopCondition  != TIMER2_HW_STOP_INVALID))
        {
            u16StopCondition = pstcCfg->u16HwStopCondition >> TMR2_HCONR_HSTP0_POS;
            if ((u16StopCondition == pstcCfg->u16HwStartCondition)         || \
                (pstcCfg->u16HwStartCondition & TIMER2_HW_START_TRIGA_MSK) || \
                (pstcCfg->u16HwStopCondition  & TIMER2_HW_STOP_TRIGA_MSK))
            {
                enRet = ErrorInvalidParameter;
            }
        }

        if (enRet == Ok)
        {
            /* Set comparison value. */
            M0P_TMR2->CMPAR = (uint32_t)pstcCfg->u16CompareVal;

            /* Set start condition and stop condition. */
            u16Temp = pstcCfg->u16HwStartCondition | pstcCfg->u16HwStopCondition;
            M0P_TMR2->HCONR = (uint32_t)u16Temp;

            /* Set start polarity and stop polarity for PWM. */
            u16Temp = (pstcCfg->u16PwmStartPolarity | \
                       pstcCfg->u16PwmStopPolarity  | \
                       TIMER2_PWM_CM_TOGGLE         | \
                       (uint16_t)TMR2_PCONR_OUTENA);
            M0P_TMR2->PCONR = (uint32_t)u16Temp;

            /* Set the clock source and clock prescaler. */
            u16Temp = pstcCfg->u16ClkSource | pstcCfg->u16ClkPrescaler;
            M0P_TMR2->BCONR = (uint32_t)u16Temp;
        }
    }

    return enRet;
}

/**
 * @brief  Configures TIMER2 peripheral as a capture function according to the specified parameters
 *         in the structure stc_timer2_config_t.
 * @param  [in]  pstcCfg            Pointer to a stc_timer2_config_t structure value that
 *                                  contains the configuration information for TIMER2.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:  -pstcCfg == NULL.
 *                                 -The stop condition is the same as the start condition.
 */
en_result_t TIMER2_CaptureConfig(const stc_timer2_config_t *pstcCfg)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint16_t u16Temp;

    if (pstcCfg != NULL)
    {
        DDL_ASSERT(IS_TIMER2_CLK_SOURCE(pstcCfg->u16ClkSource));
        DDL_ASSERT(IS_TIMER2_CLK_PRESCALER(pstcCfg->u16ClkPrescaler));
        DDL_ASSERT(IS_TIMER2_START_CONDITION(pstcCfg->u16HwStartCondition));
        DDL_ASSERT(IS_TIMER2_STOP_CONDITION(pstcCfg->u16HwStopCondition));
        DDL_ASSERT(IS_TIMER2_CLEAR_CONDITION(pstcCfg->u16HwClearCondition));
        DDL_ASSERT(IS_TIMER2_CAPTURE_CONDITION(pstcCfg->u16HwCaptureCondition));
        DDL_ASSERT(IS_TIMER2_FILTER_PRESCALER(pstcCfg->u16FilterPrescaler));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCfg->enFilterCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCfg->enMatchIntCmd));

        enRet = Ok;
        /* Start condition and stop condition CAN NOT be the same condition. */
        if ((pstcCfg->u16HwStartCondition != TIMER2_HW_START_INVALID) && \
            (pstcCfg->u16HwStopCondition  != TIMER2_HW_STOP_INVALID))
        {
            if ((pstcCfg->u16HwStopCondition >> TMR2_HCONR_HSTP0_POS) == pstcCfg->u16HwStartCondition)
            {
                enRet = ErrorInvalidParameter;
            }
        }

        if (enRet == Ok)
        {
            /* Set start condition and stop condition. */
            u16Temp = (pstcCfg->u16HwStartCondition | \
                       pstcCfg->u16HwStopCondition  | \
                       pstcCfg->u16HwClearCondition | \
                       pstcCfg->u16HwCaptureCondition);
            M0P_TMR2->HCONR = (uint32_t)u16Temp;

            /* Set the prescaler of TRIGA filter. */
            M0P_TMR2->PCONR = (uint32_t)pstcCfg->u16FilterPrescaler;

            /* Enable or Disable the TRIGA filter. */
            bM0P_TMR2->PCONR_b.NOFIENA = (uint32_t)pstcCfg->enFilterCmd;

            /* Enable or Disable the counter match interrupt. */
            bM0P_TMR2->ICONR_b.CMENA   = (uint32_t)pstcCfg->enMatchIntCmd;

            /* Enable or Disable the counter match interrupt. */
            bM0P_TMR2->ICONR_b.OVENA   = (uint32_t)pstcCfg->enOvfIntCmd;

            /* Set the clock source and clock prescaler. */
            u16Temp = (pstcCfg->u16ClkSource    | \
                       pstcCfg->u16ClkPrescaler | \
                       TIMER2_FUNC_CAPTURE);
            M0P_TMR2->BCONR = (uint32_t)u16Temp;
        }
    }

    return enRet;
}

/**
 * @brief  Set trigger event for TIMER2.
 * @param  [in]  enEvent            An en_event_src_t enumeration value.
 * @retval None
 */
void TIMER2_SetTrigEvent(en_event_src_t enEvent)
{
    DDL_ASSERT(IS_TIMER2_TRIGGER_EVT(enEvent));

    TIMER2_TRIG_EVT_REG = (uint32_t)enEvent;
}

/**
 * @brief  TIMER2 get flag.
 * @param  [in]  u16Flag            TIMER2 state flag.
 *                                  This parameter can be a value of @ref TIMER2_State_Flag
 *   @arg  TIMER2_FLAG_CNT_MATCH
 *   @arg  TIMER2_FLAG_CNT_OVF
 * @retval An en_flag_status_t enumeration.
 *   @arg  Set: The specified flag is set.
 *   @arg  Reset: The specified flag is not set.
 */
en_flag_status_t TIMER2_GetFlag(uint16_t u16Flag)
{
    en_flag_status_t enFlag = Reset;

    if ((M0P_TMR2->STFLR & (uint32_t)u16Flag) != 0u)
    {
        enFlag = Set;
    }

    return enFlag;
}

/**
 * @brief  TIMER2 clear flag.
 * @param  [in]  u16Flag            TIMER2 state flag.
 *                                  This parameter can be a value of @ref TIMER2_State_Flag
 *   @arg  TIMER2_FLAG_CNT_MATCH
 *   @arg  TIMER2_FLAG_CNT_OVF
 * @retval None
 */
void TIMER2_ClrFlag(uint16_t u16Flag)
{
    M0P_TMR2->STFLR &= ~((uint32_t)u16Flag);
}

/**
 * @}
 */

#endif /* DDL_TIMER2_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
