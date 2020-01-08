/**
 *******************************************************************************
 * @file  hc32m120_adc.c
 * @brief This file provides firmware functions to manage the Analog-to-Digital
 *        Converter(ADC).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-09       Wuze            First version
   2020-01-08       Wuze            Modified the type of 'u32Event0' in structure stc_adc_trg_cfg_t,
                                    from 'uint32 enEvent0' to 'en_event_src_t enEvent0'. Same with 'u32Event1'.
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
#include "hc32m120_adc.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_ADC ADC
 * @brief Analog-to-Digital Converter Driver Library
 * @{
 */

#if (DDL_ADC_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup ADC_Local_Macros ADC Local Macros
 * @{
 */

/**
 * @brief Definitions of ADC trigger event registers.
 */
#define ADC_TRG_EVT0_REG                    (M0P_AOS->ADC_ITRGSELR0)
#define ADC_TRG_EVT1_REG                    (M0P_AOS->ADC_ITRGSELR1)

/**
 * @defgroup ADC_Check_Parameters_Validity ADC check parameters validity
 * @{
 */
#define IS_ADC_SCAN_CONV_MODE(x)                                               \
(   ((x) == ADC_MODE_SA_SINGLE)             ||                                 \
    ((x) == ADC_MODE_SA_CONTINUOUS)         ||                                 \
    ((x) == ADC_MODE_SA_SB_SINGLE)          ||                                 \
    ((x) == ADC_MODE_SA_CONTINUOUS_SB_SINGLE))

#define IS_ADC_RESOLUTION(x)                                                   \
(   ((x) == ADC_RESOLUTION_8B)              ||                                 \
    ((x) == ADC_RESOLUTION_10B)             ||                                 \
    ((x) == ADC_RESOLUTION_12B))

#define IS_ADC_AUTO_CLEAR_CMD(x)                                               \
(   ((x) == ADC_AUTO_CLR_DISABLE)           ||                                 \
    ((x) == ADC_AUTO_CLR_ENABLE))

#define IS_ADC_DATA_ALIGN(x)                                                   \
(   ((x) == ADC_DATA_ALIGN_RIGHT)           ||                                 \
    ((x) == ADC_DATA_ALIGN_LEFT))

#define IS_ADC_SA_RESTART_FROM(x)                                              \
(   ((x) == ADC_SA_RESTRT_FROM_INT_CH)      ||                                 \
    ((x) == ADC_SA_RESTRT_FROM_FIRST_CH))

#define IS_ADC_PIN_NUMBER(x)                                                   \
(   ((x) == ADC_ANI0)                       ||                                 \
    (((x) >= ADC_ANI1) && ((x) <= ADC_ANI11)))

#define IS_ADC_SEQUENCE(x)                                                     \
(   ((x) == ADC_SEQ_A)                      ||                                 \
    ((x) == ADC_SEQ_B))

#define IS_AWD_NUMBER(x)                                                       \
(   ((x) == ADC_AWD_0)                      ||                                 \
    ((x) == ADC_AWD_1))

#define IS_OPA_NUMBER(x)                                                       \
(   ((x) == ADC_OPA_1)                      ||                                 \
    ((x) == ADC_OPA_2))

#define IS_ADC_SAMPLE_TIME(x)                                                  \
(   ((x) >= 5u))

#define IS_ADC_TRIGGER_SRC(x)                                                  \
(   ((x) == ADC_TRGSRC_EX_PIN)              ||                                 \
    ((x) == ADC_TRGSRC_IN_EVT0)             ||                                 \
    ((x) == ADC_TRGSRC_IN_EVT1)             ||                                 \
    ((x) == ADC_TRGSRC_IN_EVT0_EVT1))

#define IS_ADC_EX_CH(x)                                                        \
(   ((x) == ADC_EX_CH_SEL_ANI11)            ||                                 \
    ((x) == ADC_EX_CH_SEL_INTERNAL))


#define IS_AWD_CMP_MODE(x)                                                     \
(   ((x) == ADC_AWD_CMP_OUT_RANGE)          ||                                 \
    ((x) == ADC_AWD_CMP_IN_RANGE))

#define IS_AWD_COMB_MODE(x)                                                    \
(   ((x) == ADC_AWD_COMB_INVALID)           ||                                 \
    ((x) == ADC_AWD_COMB_OR)                ||                                 \
    ((x) == ADC_AWD_COMB_AND)               ||                                 \
    ((x) == ADC_AWD_COMB_XOR))

#define IS_ADC_SA_TRIGGER_EVT(x)                                               \
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
    ((x) == EVT_USART_2_TI)                 ||                                 \
    ((x) == EVT_IIC_TXI)                    ||                                 \
    ((x) == EVT_PORT_EIRQ7)                 ||                                 \
    ((x) == EVT_CMP_2_IRQ)                  ||                                 \
    ((x) == EVT_FMC_OPTEND)                 ||                                 \
    ((x) == EVT_TMRB_2_OVF)                 ||                                 \
    ((x) == EVT_TMRB_2_UDF)                 ||                                 \
    ((x) == EVT_TMRB_1_CMP)                 ||                                 \
    ((x) == EVT_SPI_SPTI))

#define IS_ADC_SB_TRIGGER_EVT(x)                                               \
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
    ((x) == EVT_USART_2_RI)                 ||                                 \
    ((x) == EVT_IIC_RXI)                    ||                                 \
    ((x) == EVT_PORT_EIRQ6)                 ||                                 \
    ((x) == EVT_CMP_1_IRQ)                  ||                                 \
    ((x) == EVT_EVENT_PORT)                 ||                                 \
    ((x) == EVT_TMRB_1_OVF)                 ||                                 \
    ((x) == EVT_TMRB_1_UDF)                 ||                                 \
    ((x) == EVT_TMRB_2_CMP)                 ||                                 \
    ((x) == EVT_USART_2_TI)                 ||                                 \
    ((x) == EVT_IIC_TXI)                    ||                                 \
    ((x) == EVT_PORT_EIRQ7)                 ||                                 \
    ((x) == EVT_CMP_2_IRQ)                  ||                                 \
    ((x) == EVT_FMC_OPTEND)                 ||                                 \
    ((x) == EVT_TMRB_2_OVF)                 ||                                 \
    ((x) == EVT_TMRB_2_UDF)                 ||                                 \
    ((x) == EVT_TMRB_1_CMP)                 ||                                 \
    ((x) == EVT_SPI_SPTI))

#define IS_AWD_PIN_NUMBER(x)                                                   \
(   ((x) == ADC_ANI0)                       ||                                 \
    (((x) >= ADC_ANI1) && ((x) <= ADC_ANI11)))

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
 * @defgroup ADC_Global_Functions ADC Global Functions
 * @{
 */

/**
 * @brief  Initializes the ADC peripheral according to the specified parameters
 *         in the structure stc_adc_init.
 * @param  [in]  pstcInit           Pointer to a stc_adc_init_t structure value that
 *                                  contains the configuration information for the ADC.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcInit == NULL.
 */
en_result_t ADC_Init(const stc_adc_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcInit != NULL)
    {
        DDL_ASSERT(IS_ADC_SCAN_CONV_MODE(pstcInit->u16ScanConvMode));
        DDL_ASSERT(IS_ADC_RESOLUTION(pstcInit->u16Resolution));
        DDL_ASSERT(IS_ADC_AUTO_CLEAR_CMD(pstcInit->u16AutoClearCmd));
        DDL_ASSERT(IS_ADC_DATA_ALIGN(pstcInit->u16DataAlign));
        DDL_ASSERT(IS_ADC_SA_RESTART_FROM(pstcInit->u16RestrtFrom));

        /* Configures scan convert mode, resolution, data automatically cleared cmd
           and data alignment. */
        M0P_ADC->CR0 = pstcInit->u16ScanConvMode  |   \
                       pstcInit->u16Resolution    |   \
                       pstcInit->u16AutoClearCmd  |   \
                       pstcInit->u16DataAlign;

        /* Configures sampling time. */
        M0P_ADC->SSTR = pstcInit->u8SampTime;

        /* Configures the channel sequence A restart from. */
        M0P_ADC->CR1  = pstcInit->u16RestrtFrom;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-initializes the ADC peripheral.
 * @param  None
 * @retval None
 */
void ADC_DeInit(void)
{
    /* Stop the ADC. */
    ADC_Stop();

    /* Configures the registers to default value. */
    M0P_ADC->TRGSR    = 0x0u;
    M0P_ADC->CHSELRA0 = 0x0u;
    M0P_ADC->CHSELRB0 = 0x0u;
    M0P_ADC->EXCHSELR = 0x0u;
    M0P_ADC->ISR      = 0x0u;
    M0P_ADC->ICR      = 0x0u;
    M0P_ADC->AWDCR    = 0x0u;
    M0P_ADC->AWDSR    = 0x0u;
    M0P_ADC->OPACR    = 0x0u;
}

/**
 * @brief  Set a default value for the ADC initialization structure.
 * @param  [in]  pstcInit           Pointer to a stc_adc_init_t structure that
 *                                  contains configuration information.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcInit == NULL.
 */
en_result_t ADC_StructInit(stc_adc_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcInit != NULL)
    {
        pstcInit->u16ScanConvMode = ADC_MODE_SA_SINGLE;
        pstcInit->u16Resolution   = ADC_RESOLUTION_10B;
        pstcInit->u16AutoClearCmd = ADC_AUTO_CLR_DISABLE;
        pstcInit->u16DataAlign    = ADC_DATA_ALIGN_RIGHT;
        pstcInit->u8SampTime      = 20u;
        pstcInit->u16RestrtFrom   = ADC_SA_RESTRT_FROM_INT_CH;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  ADC add ADC channel(s).
 * @param  [in]  u8Seq              The specified sequence for the channel(s) to be added in.
 *                                  This parameter can be a value of @ref ADC_Sequence
 *   @arg  ADC_SEQ_A:               Add channels to sequence A.
 *   @arg  ADC_SEQ_B:               Add channels to sequence B.
 * @param  [in]  u16Channel         The specified channel(s) to be added in the specified sequence.
 *                                  This parameter can be values of @ref ADC_Channels
 *   @arg  ADC_CH0 ~ ADC_CH11
 * @retval None.
 * @note  -(1) ADC must be stoped.
 *        -(2) Sequence A and Sequence B cannot contain the same channel.
 */
void ADC_AddAdcChannel(uint8_t u8Seq, uint16_t u16Channel)
{
    uint16_t u16Ch;

    u16Ch = u16Channel & ADC_CH_ALL;

    DDL_ASSERT(IS_ADC_SEQUENCE(u8Seq));

    /* Configures the channels. */
    if (u8Seq == ADC_SEQ_A)
    {
        M0P_ADC->CHSELRA0 |= u16Ch;
    }
    else
    {
        M0P_ADC->CHSELRB0 |= u16Ch;
    }
}

/**
 * @brief  ADC set extend channel.
 * @param  [in]  u8ExCh             The specified source for the extend channel(channel 11).
 *                                  This parameter can be a value of @ref ADC_Extend_Channel_Source_Selection
 *   @arg  ADC_EX_CH_SEL_ANI11:     Select the pin ANI11 as the extend channel analog input source.
 *   @arg  ADC_EX_CH_SEL_INTERNAL:  Select the internal analog source as the extend channel analog input source.
 * @retval None.
 * @note   ADC must be stoped.
 */
void ADC_SetExChannel(uint8_t u8ExCh)
{
    DDL_ASSERT(IS_ADC_EX_CH(u8ExCh));
    M0P_ADC->EXCHSELR = u8ExCh;
}

/**
 * @brief  Configures ADC's trigger source.
 * @param  [in]  u8Seq              The sequence to be configured.
 *                                  This parameter can be a value of @ref ADC_Sequence
 *   @arg  ADC_SEQ_A
 *   @arg  ADC_SEQ_B
 * @param  [in]  pstcTrgCfg         Pointer to a stc_adc_trg_cfg_t structure that contains
 *                                  the configuration information of the trigger source.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcTrgCfg == NULL.
 * @note   ADC must be stoped.
 */
en_result_t ADC_ConfigTriggerSrc(uint8_t u8Seq, const stc_adc_trg_cfg_t *pstcTrgCfg)
{
    uint8_t u8Offset;
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcTrgCfg != NULL)
    {
        DDL_ASSERT(IS_ADC_SEQUENCE(u8Seq));
        DDL_ASSERT(IS_ADC_TRIGGER_SRC(pstcTrgCfg->u16TrgSrc));

        /* Disable the specified sequence's trigger source. */
        ADC_TriggerSrcCmd(u8Seq, Disable);

        if (u8Seq == ADC_SEQ_A)
        {
            u8Offset = 0u;
            DDL_ASSERT(IS_ADC_SA_TRIGGER_EVT(pstcTrgCfg->enEvent0));
            DDL_ASSERT(IS_ADC_SA_TRIGGER_EVT(pstcTrgCfg->enEvent1));
        }
        else
        {
            u8Offset = 8u;
            DDL_ASSERT(IS_ADC_SB_TRIGGER_EVT(pstcTrgCfg->enEvent0));
            DDL_ASSERT(IS_ADC_SB_TRIGGER_EVT(pstcTrgCfg->enEvent1));
        }

        switch (pstcTrgCfg->u16TrgSrc)
        {
            case ADC_TRGSRC_IN_EVT0:
                ADC_TRG_EVT0_REG = (uint32_t)pstcTrgCfg->enEvent0;
                break;

            case ADC_TRGSRC_IN_EVT1:
                ADC_TRG_EVT1_REG = (uint32_t)pstcTrgCfg->enEvent1;
                break;

            case ADC_TRGSRC_IN_EVT0_EVT1:
                ADC_TRG_EVT0_REG = (uint32_t)pstcTrgCfg->enEvent0;
                ADC_TRG_EVT1_REG = (uint32_t)pstcTrgCfg->enEvent1;
                break;

            default:
                break;
        }

        M0P_ADC->TRGSR &= (uint16_t)(~(ADC_TRGSRC_MASK << u8Offset));
        M0P_ADC->TRGSR |= (uint16_t)(pstcTrgCfg->u16TrgSrc << u8Offset);

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set a default value for the trigger source configuration structure.
 * @param  [in]  pstcTrgCfg         Pointer to a stc_adc_trg_cfg_t structure that
 *                                  contains configuration information.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcTrgCfg == NULL.
 */
en_result_t ADC_TriggerSrcStructInit(stc_adc_trg_cfg_t *pstcTrgCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcTrgCfg != NULL)
    {
        pstcTrgCfg->u16TrgSrc = ADC_TRGSRC_IN_EVT0;
        pstcTrgCfg->enEvent0  = EVT_AOS_STRG;
        pstcTrgCfg->enEvent1  = EVT_AOS_STRG;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Enable or disable the trigger source of the specified sequence.
 * @param  [in]  u8Seq              The sequence to be configured.
 *                                  This parameter can be a value of @ref ADC_Sequence
 *   @arg  ADC_SEQ_A
 *   @arg  ADC_SEQ_B
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable the trigger source of the specified sequence.
 *   @arg  Disable:                 Disable the trigger source of the specified sequence.
 * @retval None
 * @note   ADC must be stoped.
 */
void ADC_TriggerSrcCmd(uint8_t u8Seq, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_ADC_SEQUENCE(u8Seq));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (u8Seq == ADC_SEQ_A)
    {
        bM0P_ADC->TRGSR_b.TRGENA = (uint32_t)enNewState;
    }
    else
    {
        bM0P_ADC->TRGSR_b.TRGENB = (uint32_t)enNewState;
    }
}

/**
 * @brief  Enable or disable the interrupt of the specified sequence.
 * @param  [in]  u8Seq              The sequence to be configured.
 *                                  This parameter can be a value of @ref ADC_Sequence
 *   @arg  ADC_SEQ_A
 *   @arg  ADC_SEQ_B
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable the interrupt of the specified sequence.
 *   @arg  Disable:                 Disable the interrupt of the specified sequence.
 * @retval None
 * @note   ADC must be stoped.
 */
void ADC_AdcIntCmd(uint8_t u8Seq, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_ADC_SEQUENCE(u8Seq));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (u8Seq == ADC_SEQ_A)
    {
        bM0P_ADC->ICR_b.EOCAIEN = (uint32_t)enNewState;
    }
    else
    {
        bM0P_ADC->ICR_b.EOCBIEN = (uint32_t)enNewState;
    }
}

/**
 * @brief  Gets the EOC(End Of Conversion) flag.
 * @param  [in]  u8Flag         ADC EOC flag.
 *                              This parameter can be a value of @ref ADC_Eoc_Flag
 *   @arg  ADC_FLAG_EOCA
 *   @arg  ADC_FLAG_EOCB
 * @retval An en_flag_status_t enumeration.
 *   @arg  Set: The specified flag has set.
 *   @arg  Reset: The specified flag has not set.
 */
en_flag_status_t ADC_GetEocFlag(uint8_t u8Flag)
{
    en_flag_status_t enFlag = Reset;

    if ((M0P_ADC->ISR & u8Flag) != 0u)
    {
        enFlag = Set;
    }

    return enFlag;
}

/**
 * @brief  Clear the EOC(End Of Conversion) flag.
 * @param  [in]  u8Flag             An 8bit data that containing EOCA(End Of Conversion of Sequence A)
 *                                  flag and EOCB flag. This parameter can be values of @ref ADC_Eoc_Flag
 *   @arg  ADC_FLAG_EOCA
 *   @arg  ADC_FLAG_EOCB
 * @retval None
 */
void ADC_ClrEocFlag(uint8_t u8Flag)
{
    u8Flag &= ADC_FLAG_EOC_ALL;
    M0P_ADC->ISCLRR |= u8Flag;
}

/**
 * @brief  Starts ADC conversion of sequence A.
 * @param  [out] pu16Dest           Pointer to the buffer the ADC data to be stored.
 * @param  [in]  u8Length           The length of the array pointed by pu16Dest.
 * @param  [in]  u32Timeout         Timeout value(millisecond).
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pu16Dest == NULL or u32Timeout == 0.
 *   @arg  ErrorTimeout:            ADC works timeout.
 */
en_result_t ADC_PollingSa(uint16_t *pu16Dest, uint8_t u8Length, uint32_t u32Timeout)
{
    en_result_t   enRet = ErrorInvalidParameter;
    uint16_t      u16Channel;
    uint32_t      u32AdcTimeout;
    __IO uint32_t u32TimeCount;

    if ((pu16Dest != NULL) && (u8Length != 0u) && (u32Timeout != 0u))
    {
        /* 10 is the number of required instructions cycles for the below loop statement. */
        u32AdcTimeout = u32Timeout * (SystemCoreClock / 10u / 1000u);

        ADC_Start();

        /* Polling and check timeout. */
        u32TimeCount = 0u;
        enRet = ErrorTimeout;
        while (u32TimeCount < u32AdcTimeout)
        {
            if (bM0P_ADC->ISR_b.EOCAF != 0u)
            {
                if (u8Length >= ADC_CH_COUNT)
                {
                    ADC_GetAllData(pu16Dest, ADC_CH_COUNT);
                }
                else
                {
                    u16Channel = M0P_ADC->CHSELRA0;
                    ADC_GetChannelData(u16Channel, pu16Dest, u8Length);
                }
                enRet = Ok;
                break;
            }
            u32TimeCount++;
        }

        bM0P_ADC->ISCLRR_b.CLREOCAF = 1u;
    }

    return enRet;
}

/**
 * @brief  Get all of the ADC data.
 * @param  [out] pu16Dest           Pointer to the buffer the ADC data to be stored.
 * @param  [in]  u8Length           The length of the array pointed by the pointer pu16Dest.
 *                                  It must be no less than @ref ADC_Channel_Count
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pu16Dest == NULL or u8Length < ADC_CH_COUNT.
 */
en_result_t ADC_GetAllData(uint16_t *pu16Dest, uint8_t u8Length)
{
    uint8_t  i;
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint16_t *io16AdcDr = &M0P_ADC->DR0;

    if ((pu16Dest != NULL) && (u8Length >= ADC_CH_COUNT))
    {
        for (i = 0u; i < ADC_CH_COUNT; i++)
        {
            pu16Dest[i] = io16AdcDr[i];
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get the ADC data of the specified channel(s).
 * @param  [in]  u16Channel         The specified Channel(s).
 *                                  This parameter can be values of @ref ADC_Channels
 *   @arg  ADC_CH0 ~ ADC_CH11
 * @param  [out] pu16Dest           Pointer to the buffer the ADC data to be stored.
 * @param  [in]  u8Length           The length of the array pointed by pu16Dest.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   u16Channel == 0 or pu16Dest == NULL or u8Length == 0
 */
en_result_t ADC_GetChannelData(uint16_t u16Channel, uint16_t *pu16Dest, uint8_t u8Length)
{
    uint8_t i;
    uint8_t j;
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint16_t *io16AdcDr = NULL;

    if ((u16Channel != 0u) && (pu16Dest != NULL) && (u8Length != 0u))
    {
        i = 0u;
        j = 0u;
        io16AdcDr = &M0P_ADC->DR0;
        u16Channel &= ADC_CH_ALL;
        while (u16Channel && u8Length)
        {
            if (u16Channel & 0x1u)
            {
                pu16Dest[j++] = io16AdcDr[i];
                u8Length--;
            }

            u16Channel >>= 1u;
            i++;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get the ADC value of the specified ADC pin.
 * @param  [in]  u8AdcPin           The ADC pin number.
 *                                  This parameter can be a value of @ref ADC_Pin_Number
 * @retval An uint16_t value -- the ADC value of the specified ADC pin.
 */
uint16_t ADC_GetValue(uint8_t u8AdcPin)
{
    uint16_t u16Ret = (uint16_t)0xFFFF;
    __IO uint16_t *io16AdcDr = &M0P_ADC->DR0;

    if (u8AdcPin <= ADC_ANI11)
    {
        u16Ret = io16AdcDr[u8AdcPin];
    }

    return u16Ret;
}

/**
 * @brief  Configures an AWD's comparison mode.
 * @param  [in]  u8AwdNbr           The specified AWD to be configured.
 *                                  This parameter can be a value of @ref ADC_AWD_Number
 *   @arg  ADC_AWD_0
 *   @arg  ADC_AWD_1
 * @param  [in]  pstcAwdCfg         Pointer to a stc_awd_cfg_t structure that contains
 *                                  the configuration information for the AWD.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcAwdCfg == NULL.
 * @note   ADC must be stoped.
 */
en_result_t ADC_AwdConfig(uint8_t u8AwdNbr, const stc_awd_cfg_t *pstcAwdCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcAwdCfg != NULL)
    {
        DDL_ASSERT(IS_AWD_NUMBER(u8AwdNbr));
        DDL_ASSERT(IS_AWD_PIN_NUMBER(pstcAwdCfg->u8AdcPin));
        DDL_ASSERT(IS_AWD_CMP_MODE(pstcAwdCfg->u16AwdMd));

        /* Disable AWDx. */
        ADC_AwdCmd(u8AwdNbr, Disable);

        /* Disable combination mode. */
        M0P_ADC->AWDCR &= (uint16_t)(~ADC_AWD_COMB_MASK);

        if (u8AwdNbr == ADC_AWD_0)
        {
            bM0P_ADC->AWDCR_b.AWD0MD = (uint32_t)pstcAwdCfg->u16AwdMd;
            M0P_ADC->AWD0DR0  = pstcAwdCfg->u16Dr0;
            M0P_ADC->AWD0DR1  = pstcAwdCfg->u16Dr1;
            M0P_ADC->AWD0CHSR = pstcAwdCfg->u8AdcPin;
        }
        else
        {
            bM0P_ADC->AWDCR_b.AWD1MD = (uint32_t)pstcAwdCfg->u16AwdMd;
            M0P_ADC->AWD1DR0  = pstcAwdCfg->u16Dr0;
            M0P_ADC->AWD1DR1  = pstcAwdCfg->u16Dr1;
            M0P_ADC->AWD1CHSR = pstcAwdCfg->u8AdcPin;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Enable or Disable an AWD.
 * @param  [in]  u8AwdNbr           The specified AWD to be configured.
 *                                  This parameter can be a value of @ref ADC_AWD_Number
 *   @arg  ADC_AWD_0
 *   @arg  ADC_AWD_1
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable the specified AWD.
 *   @arg  Disable:                 Disable the specified AWD.
 * @retval None
 * @note   ADC must be stoped.
 */
void ADC_AwdCmd(uint8_t u8AwdNbr, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_AWD_NUMBER(u8AwdNbr));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (u8AwdNbr == ADC_AWD_0)
    {
        bM0P_ADC->AWDCR_b.AWD0EN = (uint32_t)enNewState;
    }
    else
    {
        bM0P_ADC->AWDCR_b.AWD1EN = (uint32_t)enNewState;
    }
}

/**
 * @brief  Enable or Disable an AWD's interrupt.
 * @param  [in]  u8AwdNbr           The specified AWD to be configured.
 *                                  This parameter can be a value of @ref ADC_AWD_Number
 *   @arg  ADC_AWD_0
 *   @arg  ADC_AWD_1
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable the interrupt of the specified AWD.
 *   @arg  Disable:                 Disable the interrupt of the specified AWD.
 * @retval None
 * @note   ADC must be stoped.
 */
void ADC_AwdIntCmd(uint8_t u8AwdNbr, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_AWD_NUMBER(u8AwdNbr));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (u8AwdNbr == ADC_AWD_0)
    {
        bM0P_ADC->AWDCR_b.AWD0IEN = (uint32_t)enNewState;
    }
    else
    {
        bM0P_ADC->AWDCR_b.AWD1IEN = (uint32_t)enNewState;
    }
}

/**
 * @brief  Configures the combined mode of the AWD.
 * @param  [in]  u16ComMd           This parameter can be a value of @ref ADC_AWD_Combination_Mode
 *   @arg  ADC_AWD_COMB_INVALID:    Analog watchdog combination mode is invalid.
 *   @arg  ADC_AWD_COMB_OR:         AWD result of combination will be AWD0 | AWD1.
 *   @arg  ADC_AWD_COMB_AND:        AWD result of combination will be AWD0 & AWD1.
 *   @arg  ADC_AWD_COMB_XOR:        AWD result of combination will be AWD0 ^ AWD1.
 * @retval None
 * @note   ADC must be stoped.
 */
void ADC_AwdConfigCombMode(uint16_t u16ComMd)
{
    DDL_ASSERT(IS_AWD_COMB_MODE(u16ComMd));

    M0P_ADC->AWDCR &= (uint16_t)(~ADC_AWD_COMB_MASK);
    M0P_ADC->AWDCR |= u16ComMd;
}

/**
 * @brief  Gets the AWD state flag.
 * @param  [in]  u8Flag         AWD state flag.
 *                              This parameter can be a value of @ref ADC_AWD_State_Flag
 *   @arg  ADC_FLAG_AWD0
 *   @arg  ADC_FLAG_AWD1
 *   @arg  ADC_FLAG_AWD_COMB
 * @retval An en_flag_status_t enumeration.
 *   @arg  Set: The specified flag has set.
 *   @arg  Reset: The specified flag has not set.
 */
en_flag_status_t ADC_AwdGetFlag(uint8_t u8Flag)
{
    en_flag_status_t enFlag = Reset;

    if ((M0P_ADC->AWDSR & u8Flag) != 0u)
    {
        enFlag = Set;
    }

    return enFlag;
}

/**
 * @brief  ADC clear AWD state flag.
 * @param  [in]  u8Flag             An 8bit data that containing AWD0, AWD1 and combination flag.
 *                                  This parameter can be values of @ref ADC_AWD_State_Flag
 *   @arg  ADC_FLAG_AWD0
 *   @arg  ADC_FLAG_AWD1
 *   @arg  ADC_FLAG_AWD_COMB
 *   @arg  ADC_FLAG_AWD_ALL
 * @retval None
 */
void ADC_AwdClrFlag(uint8_t u8Flag)
{
    u8Flag &= ADC_FLAG_AWD_ALL;
    M0P_ADC->AWDSCLRR |= u8Flag;
}

/**
 * @brief  Enale or disable specify OPA.
 * @param  [in]  u8OpaNbr           The number of the specify OPA.
 *                                  This parameter can be a value of @ref ADC_OPA_Number
 *   @arg  ADC_OPA_1
 *   @arg  ADC_OPA_2
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable the specify OPA.
 *   @arg  Disable:                 Disable the specify OPA.
 * @retval None
 */
void ADC_OpaCmd(uint8_t u8OpaNbr, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_OPA_NUMBER(u8OpaNbr));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (u8OpaNbr == ADC_OPA_1)
    {
        bM0P_ADC->OPACR_b.OPA1EN = (uint32_t)enNewState;
    }
    else
    {
        bM0P_ADC->OPACR_b.OPA2EN = (uint32_t)enNewState;
    }
}

/**
 * @}
 */

#endif /* DDL_ADC_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
