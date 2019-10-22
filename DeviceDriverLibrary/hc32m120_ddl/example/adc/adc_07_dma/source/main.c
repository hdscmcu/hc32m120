/**
 *******************************************************************************
 * @file  adc/adc_07_dma/source/main.c
 * @brief Main program of ADC DMA for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-10       Wuze            First version
   2019-10-21       Wuze            1. Modified DMA_ChannelCmd to DMA_ChannelEnable to enable the DMA channel.
                                    2. Modified the macro names of the trigger pins of sequence A and sequence B.
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
#include "hc32_ddl.h"

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup ADC_Dma
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* ADTRG pin definition. There 3 pins can be used to trigger ADC:
   ADTRG_A(P14), ADTRG_A(P30) and ADTRG_A(P60). */
#define ADTRG_PORT                  (GPIO_PORT_1)
#define ADTRG_PIN                   (GPIO_PIN_4)

/* ADC channels definition for this example. */
#define VAR_VOL_CHANNEL             (ADC_CH9)
#define ADC_SA_CHANNEL              (VAR_VOL_CHANNEL)
#define ADC_SA_CHANNEL_COUNT        (1u)

#define ADC_SB_CHANNEL              (ADC_CH2 | ADC_CH4)
#define ADC_SB_CHANNEL_COUNT        (2u)

/* ADC channel sampling time. */
#define ADC_SAMPLE_TIME             ((uint8_t)30)

/* ADC resolution definition. */
#define ADC_RESOLUTION              (ADC_RESOLUTION_12B)

/* Trigger pin of sequence A definition.
   Pull down the pin to generate an event(EVT_PORT_EIRQ5) which is used to trigger sequence A. */
#define SA_TRIGGER_PORT             (GPIO_PORT_1)
#define SA_TRIGGER_PIN              (GPIO_PIN_6)
#define SA_TRIGGER_EXINT_CH         (EXINT_CH05)
#define SA_TRIGGER_EVENT            (EVT_PORT_EIRQ5)

/* Trigger pin of sequence B definition.
   Pull down the pin to generate an event(EVT_PORT_EIRQ0) which is used to trigger sequence B. */
#define SB_TRIGGER_PORT             (GPIO_PORT_1)
#define SB_TRIGGER_PIN              (GPIO_PIN_7)
#define SB_TRIGGER_EXINT_CH         (EXINT_CH00)
#define SB_TRIGGER_EVENT            (EVT_PORT_EIRQ0)

/* ADC interrupt flag bit mask definition. */
#define ADC_SA_IRQ_BIT              (ADC_FLAG_EOCA)
#define ADC_SB_IRQ_BIT              (ADC_FLAG_EOCB)

/* ADC channels count. */
#define ADC_CH_COUNT                (12u)

/* DMA definition. */
#define ADC_DMA_TRNCNT              (1ul)
#define ADC_DMA_BLKSIZE             (ADC_CH_COUNT)
#define ADC_DMA_DATAWIDTH           (DMA_DATAWIDTH_16BIT)
#define ADC_DMA_SRC_ADDR            ((uint32_t)(&M0P_ADC->DR0))
#define ADC_DMA_DEST_ADDR           ((uint32_t)(&m_au16AdcVal[0u]))

#define ADC_DMA_LLP_MODE            (DMA_LLP_WAIT)
#define ADC_DMA_LLP_POS             (DMA_CH0CTL0_LLP_POS - 2u)

#define ADC_DMA_SA_CH               (DMA_CHANNEL_0)
#define ADC_DMA_SA_TRIG             (EVT_ADC_EOCA)

#define ADC_DMA_SB_CH               (DMA_CHANNEL_1)
#define ADC_DMA_SB_TRIG             (EVT_ADC_EOCB)

/* Debug printing definition. */
#if (DDL_PRINT_ENABLE == DDL_ON)
#define DBG         printf
#else
#define DBG(...)
#endif

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void TriggerPinInit(void);

static void AdcConfig(void);
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);
static void AdcTriggerConfig(void);
static void AdcDmaConfig(void);

static void AdcSetChannelPinAnalogMode(uint16_t u16Channel);
static void AdcSetPinAnalogMode(uint8_t u8PinNbr);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_au16AdcVal[ADC_CH_COUNT];
static stc_dma_llp_descriptor_t m_stcLlpDesc;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_07_dma project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The system clock is set to HRC8HMz by default. */

#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_UartInit();
#endif // #if (DDL_PRINT_ENABLE == DDL_ON)

    /* Initializes the trigger pins of sequence A and sequence B.
       In this example, pull down P16 to generate the event(EVT_PORT_EIRQ5) to trigger sequence A,
       pull down P17 to generate the event(EVT_PORT_EIRQ0) to trigger sequence B. */
    TriggerPinInit();

    /* Configures ADC. */
    AdcConfig();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        /* Check ADC SA. */
        if (DMA_GetCplFlag(ADC_DMA_SA_CH, DMA_FLAG_BTC) == Set)
        {
            DMA_ClearCplFlag(ADC_DMA_SA_CH, DMA_FLAG_BTC);
            DBG("\nSequence A DMA block transfer completed.");
            // TODO: Use the ADC value of sequence A.
        }

        /* Check ADC SB. */
        if (DMA_GetCplFlag(ADC_DMA_SB_CH, DMA_FLAG_BTC) == Set)
        {
            DMA_ClearCplFlag(ADC_DMA_SB_CH, DMA_FLAG_BTC);
            DBG("\nSequence B DMA block transfer completed.");
            // TODO: Use the ADC value of sequence B.
        }
    }
}

/**
 * @brief  Initializes the trigger pins of sequence A and sequence B.
 * @param  None
 * @retval None
 */
static void TriggerPinInit(void)
{
    stc_exint_config_t stcExintCfg;
    stc_gpio_init_t    stcGpioIni;

    /* Trigger pin of sequence A. */
    GPIO_StructInit(&stcGpioIni);
    stcGpioIni.u16ExInt = PIN_EXINT_ON;
    GPIO_Init(SA_TRIGGER_PORT, SA_TRIGGER_PIN, &stcGpioIni);
    EXINT_StructInit(&stcExintCfg);
    stcExintCfg.u16ExIntCh = SA_TRIGGER_EXINT_CH;
    EXINT_Init(&stcExintCfg);

    /* Trigger pin of sequence B. */
    GPIO_Init(SB_TRIGGER_PORT, SB_TRIGGER_PIN, &stcGpioIni);
    stcExintCfg.u16ExIntCh = SB_TRIGGER_EXINT_CH;
    EXINT_Init(&stcExintCfg);
}

/**
 * @brief  ADC configuration, including clock configuration, initial configuration,
 *         channel configuration, trigger source configuration, DMA configuration
 *         and interrupt configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
    AdcTriggerConfig();
    AdcDmaConfig();
}

/**
 * @brief  Configures ADC clock.
 *         The clock of ADC: HRC8MHz / 1 = 8MHz.
 * @param  None
 * @retval None
 */
static void AdcClockConfig(void)
{
    CLK_SetADClkDiv(CLK_HCLK_DIV1);
}
/**
 * @brief  Initializes ADC.
 * @param  None
 * @retval None
 */
static void AdcInitConfig(void)
{
    stc_adc_init_t stcInit;

    /* Set a default value. */
    ADC_StructInit(&stcInit);

    /* User configurations. */
    stcInit.u16ScanConvMode = ADC_MODE_SA_SB_SINGLE;
    stcInit.u16Resolution   = ADC_RESOLUTION;
    stcInit.u8SampTime      = ADC_SAMPLE_TIME;

    /* 1. Enable ADC peripheral. */
    CLK_FcgPeriphClockCmd(CLK_FCG_ADC, Enable);

    /* 2. Initializes ADC. */
    ADC_Init(&stcInit);
}

/**
 * @brief  Configures ADC channel(s).
 * @param  None
 * @retval None
 */
static void AdcChannelConfig(void)
{
    /* 1. Set the ADC pin to analog mode. */
    AdcSetChannelPinAnalogMode(ADC_SA_CHANNEL | ADC_SB_CHANNEL);

    /* 2. Add ADC sequence A channels. */
    ADC_AddAdcChannel(ADC_SEQ_A, ADC_SA_CHANNEL);

    /* 3. Add ADC sequence B channels. */
    ADC_AddAdcChannel(ADC_SEQ_B, ADC_SB_CHANNEL);
}

/**
 * @brief  Configures ADC trigger source.
 * @note   Sequence A and sequence B are not recommended to be set to the same trigger source.
 * @param  None
 * @retval None
 */
static void AdcTriggerConfig(void)
{
    uint8_t u8Seq;
    stc_adc_trg_cfg_t stcTrgCfg;

    /* Configrues sequence A's trigger source. */
    u8Seq = ADC_SEQ_A;
    /* Set a default value for stcTrgCfg.  */
    ADC_TriggerSrcStructInit(&stcTrgCfg);
    stcTrgCfg.u16TrgSrc = ADC_TRGSRC_IN_EVT0;
    stcTrgCfg.u32Event0 = SA_TRIGGER_EVENT;
    /* 1. Enable AOS */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);
    /* 2. Configrues the trigger source of sequence A. */
    ADC_ConfigTriggerSrc(u8Seq, &stcTrgCfg);
    /* 3. Enable the trigger source. */
    ADC_TriggerSrcCmd(u8Seq, Enable);

    /* Configrues sequence B's trigger source. */
    u8Seq = ADC_SEQ_B;
    /* Set a default value for stcTrgCfg.  */
    ADC_TriggerSrcStructInit(&stcTrgCfg);
    stcTrgCfg.u16TrgSrc = ADC_TRGSRC_IN_EVT1;
    stcTrgCfg.u32Event1 = SB_TRIGGER_EVENT;

    /* 1. Enable AOS */
    // CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);
    /* 2. Configrues the trigger source of sequence A. */
    ADC_ConfigTriggerSrc(u8Seq, &stcTrgCfg);
    /* 3. Enable the trigger source. */
    ADC_TriggerSrcCmd(u8Seq, Enable);
}

/**
 * @brief  ADC DMA configuration.
 * @param  None
 * @retval None
 */
static void AdcDmaConfig(void)
{
    stc_dma_ch_cfg_t stcDmaChCfg;

    /* Descriptor 0 */
    m_stcLlpDesc.SARx    = ADC_DMA_SRC_ADDR;
    m_stcLlpDesc.DARx    = ADC_DMA_DEST_ADDR;
    m_stcLlpDesc.CHxCTL0 = ADC_DMA_BLKSIZE                                     | \
                           ((uint32_t)(ADC_DMA_TRNCNT << DMA_CH0CTL0_CNT_POS)) | \
                           ((uint32_t)&m_stcLlpDesc << ADC_DMA_LLP_POS)        | \
                           DMA_LLP_ENABLE                                      | \
                           ADC_DMA_LLP_MODE                                    | \
                           ADC_DMA_DATAWIDTH;
    m_stcLlpDesc.CHxCTL1 = DMA_DESADDRINC_INC | DMA_SRCADDRINC_INC;

    /* Config DMA */
    stcDmaChCfg.u32DataWidth   = ADC_DMA_DATAWIDTH;
    stcDmaChCfg.u32BlockSize   = ADC_DMA_BLKSIZE;
    stcDmaChCfg.u32TransferCnt = ADC_DMA_TRNCNT;
    stcDmaChCfg.u32SrcAddr     = ADC_DMA_SRC_ADDR;
    stcDmaChCfg.u32DesAddr     = ADC_DMA_DEST_ADDR;
    stcDmaChCfg.u32SrcInc      = DMA_SRCADDRINC_INC;
    stcDmaChCfg.u32DesInc      = DMA_DESADDRINC_INC;

    CLK_FcgPeriphClockCmd(CLK_FCG_DMA, Enable);

    DMA_ChannelCfg(ADC_DMA_SA_CH, &stcDmaChCfg);
    DMA_ChannelCfg(ADC_DMA_SB_CH, &stcDmaChCfg);
    DMA_LlpInit(ADC_DMA_SA_CH, ADC_DMA_LLP_MODE, (uint32_t)&m_stcLlpDesc);
    DMA_LlpInit(ADC_DMA_SB_CH, ADC_DMA_LLP_MODE, (uint32_t)&m_stcLlpDesc);

    /* Enable AOS clock */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);

    /* Set DMA trigger source */
    DMA_SetTriggerSrc(ADC_DMA_SA_CH, ADC_DMA_SA_TRIG);
    DMA_SetTriggerSrc(ADC_DMA_SB_CH, ADC_DMA_SB_TRIG);

    DMA_ClearCplFlag(ADC_DMA_SA_CH, DMA_FLAG_BTC);
    DMA_ClearCplFlag(ADC_DMA_SB_CH, DMA_FLAG_BTC);

    /* Enable DMA channel. */
    DMA_ChannelEnable(ADC_DMA_SA_CH);
    DMA_ChannelEnable(ADC_DMA_SB_CH);

    /* Enable DMA. */
    DMA_Cmd(Enable);
}

/**
 * @brief  Set the pin(s) corresponding to the specified channel(s) to analog mode.
 * @param  [in]  u16Channel         The specified channel(s).
 * @retval None
 */
static void AdcSetChannelPinAnalogMode(uint16_t u16Channel)
{
    uint8_t u8PinNbr;

    u8PinNbr    = 0u;
    u16Channel &= ADC_CH_ALL;

    while (u16Channel != 0u)
    {
        if (u16Channel & 0x1u)
        {
            AdcSetPinAnalogMode(u8PinNbr);
        }

        u16Channel >>= 1u;
        u8PinNbr++;
    }
}

/**
 * @brief  Set specified ADC pin to analog mode.
 * @param  [in]  u8PinNbr           The ADC pin number.
 *                                  This parameter can be a value of @ref ADC_Pin_Number
 * @retval None
 */
static void AdcSetPinAnalogMode(uint8_t u8PinNbr)
{
    uint8_t u8Port = GPIO_PORT_2;
    uint8_t u8Pin  = GPIO_PIN_0;
    uint8_t u8Flag = 1u;

    switch (u8PinNbr)
    {
    case ADC_ANI0:
        u8Port = GPIO_PORT_2;
        u8Pin  = GPIO_PIN_0;
        break;

    case ADC_ANI1:
        u8Port = GPIO_PORT_2;
        u8Pin  = GPIO_PIN_1;
        break;

    case ADC_ANI2:
        u8Port = GPIO_PORT_2;
        u8Pin  = GPIO_PIN_2;
        break;

    case ADC_ANI3:
        u8Port = GPIO_PORT_2;
        u8Pin  = GPIO_PIN_3;
        break;

    case ADC_ANI4:
        u8Port = GPIO_PORT_2;
        u8Pin  = GPIO_PIN_4;
        break;

    case ADC_ANI5:
        u8Port = GPIO_PORT_2;
        u8Pin  = GPIO_PIN_5;
        break;

    case ADC_ANI6:
        u8Port = GPIO_PORT_2;
        u8Pin  = GPIO_PIN_6;
        break;

    case ADC_ANI7:
        u8Port = GPIO_PORT_2;
        u8Pin  = GPIO_PIN_7;
        break;

    case ADC_ANI8:
        u8Port = GPIO_PORT_14;
        u8Pin  = GPIO_PIN_7;
        break;

    case ADC_ANI9:
        u8Port = GPIO_PORT_0;
        u8Pin  = GPIO_PIN_0;
        break;

    case ADC_ANI10:
        u8Port = GPIO_PORT_0;
        u8Pin  = GPIO_PIN_1;
        break;

    case ADC_ANI11:
        u8Port = GPIO_PORT_12;
        u8Pin  = GPIO_PIN_0;
        break;

    default:
        u8Flag = 0u;
        break;
    }

    if (u8Flag != 0u)
    {
        GPIO_SetFunc(u8Port, u8Pin, GPIO_FUNC_1_ANIN);
    }
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
