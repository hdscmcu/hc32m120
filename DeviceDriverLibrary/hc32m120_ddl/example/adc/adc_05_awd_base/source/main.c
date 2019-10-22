/**
 *******************************************************************************
 * @file  adc/adc_05_awd_base/source/main.c
 * @brief Main program of ADC AWD base for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-17       Wuze            First version
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
 * @addtogroup ADC_AWD_Base
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* ADC channels definition for this example. */
#define AWD0_PIN                    (ADC_ANI9)
#define AWD1_PIN                    (ADC_ANI10)
#define AWD0_CH                     ((uint16_t)(1ul << AWD0_PIN))
#define AWD1_CH                     ((uint16_t)(1ul << AWD1_PIN))

/* The channels of AWD0 and AWD1 can be configured in the same sequence.
   AWD0 and AWD1 can also be configured with the same channel. */
#define ADC_SA_NORMAL_CHANNEL       (ADC_CH0)
#define ADC_SA_AWD_CHANNEL          (AWD0_CH | AWD1_CH)
#define ADC_SA_CHANNEL              (ADC_SA_NORMAL_CHANNEL | ADC_SA_AWD_CHANNEL)
#define ADC_SA_CHANNEL_COUNT        (3u)

/* AWD definition for this example. */
#define USE_AWD0                    (1u)
#define ADW0_MD                     (ADC_AWD_CMP_IN_RANGE)
#define AWD0_DR0_LOWER              ((uint16_t)1500)
#define AWD0_DR1_UPPER              ((uint16_t)2500)

#define USE_AWD1                    (1u)
#define ADW1_MD                     (ADC_AWD_CMP_OUT_RANGE)
#define AWD1_DR0_LOWER              ((uint16_t)500)
#define AWD1_DR1_UPPER              ((uint16_t)3500)

#if (USE_AWD0 && USE_AWD1)
#define AWD_COMBINATION_MODE        (ADC_AWD_COMB_OR)
#endif // #if (USE_AWD0 && USE_AWD1)

/* ADC channel sampling time. */
#define ADC_SAMPLE_TIME             ((uint8_t)30)

/* ADC resolution definition. */
#define ADC_RESOLUTION              (ADC_RESOLUTION_12B)

/* ADC accuracy. */
#define ADC_ACCURACY                (1ul << 12u)

/* ADC reference voltage. The voltage of pin VREFH. */
#define ADC_VREF                    (3.3018f)

/* ADC value definition. */
#define awd0_val                    (m_au16AdcSaVal[1u])
#define awd1_val                    (m_au16AdcSaVal[2u])

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
static void AdcConfig(void);
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);

static void AdcSetChannelPinAnalogMode(uint16_t u16Channel);
static void AdcSetPinAnalogMode(uint8_t u8PinNbr);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_au16AdcSaVal[ADC_SA_CHANNEL_COUNT];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_05_awd_base project
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

    /* Configures ADC. */
    AdcConfig();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        ADC_Start();
        /* Wait for the end of the conversion. */
        while (ADC_GetEocFlag(ADC_FLAG_EOCA) != Set)
        {
            ;
        }

        /* Get the data of all channels of sequence A. */
        ADC_GetChannelData(ADC_SA_CHANNEL, (uint16_t *)&m_au16AdcSaVal[0u], ADC_SA_CHANNEL_COUNT);
        ADC_ClrEocFlag(ADC_FLAG_EOCA);
        // TODO: Use the ADC data.
        DBG("\nAWD0: %d <= ADCValue <= %d.", AWD0_DR0_LOWER, AWD0_DR1_UPPER);
        DBG("\nAWD1: ADCValue < %d or ADCValue > %d.", AWD1_DR0_LOWER, AWD1_DR1_UPPER);
        DBG("\nAWD0 ADCValue: %d.", awd0_val);
        DBG("\nAWD1 ADCValue: %d.", awd1_val);

        /* Check AWD0. */
        if (ADC_AwdGetFlag(ADC_FLAG_AWD0) == Set)
        {
            ADC_AwdClrFlag(ADC_FLAG_AWD0);
            // TODO: Your service code.
            DBG("\nAWD0 comparison match.");
        }

        /* Check AWD1. */
        if (ADC_AwdGetFlag(ADC_FLAG_AWD1) == Set)
        {
            ADC_AwdClrFlag(ADC_FLAG_AWD1);
            // TODO: Your service code.
            DBG("\nAWD1 comparison match.");
        }

#ifdef AWD_COMBINATION_MODE
        /* Check combination flag. */
        if (ADC_AwdGetFlag(ADC_FLAG_AWD_COMB) == Set)
        {
            ADC_AwdClrFlag(ADC_FLAG_AWD_COMB);
            DBG("\nAWD0 and AWD1 combination match.");
            // TODO: Your service code.
        }
#endif // #ifdef AWD_COMBINATION_MODE

        DDL_Delay1ms(1000u);
    }
}

/**
 * @brief  ADC configuration, including clock configuration, initial configuration,
 *         channel configuration and trigger source configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
}

/**
 * @brief  Configures ADC clock.
 *         The clock of ADC: HRC8MHz / 2 = 4MHz.
 * @param  None
 * @retval None
 */
static void AdcClockConfig(void)
{
    CLK_SetADClkDiv(CLK_HCLK_DIV2);
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
    stcInit.u16Resolution = ADC_RESOLUTION;
    stcInit.u8SampTime    = ADC_SAMPLE_TIME;

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
    uint8_t u8AwdNbr;
    stc_awd_cfg_t stcAwdCfg;

    /* 1. Set the ADC pin to analog mode. */
    AdcSetChannelPinAnalogMode(ADC_SA_CHANNEL);

    /* 2. Add ADC sequence A channels. */
    ADC_AddAdcChannel(ADC_SEQ_A, ADC_SA_CHANNEL);

    /* 3. Configrues AWD. */
    u8AwdNbr = ADC_AWD_0;
    stcAwdCfg.u8AdcPin = AWD0_PIN;
    stcAwdCfg.u16AwdMd = ADW0_MD;
    stcAwdCfg.u16Dr0   = AWD0_DR0_LOWER;
    stcAwdCfg.u16Dr1   = AWD0_DR1_UPPER;
    ADC_AwdConfig(u8AwdNbr, &stcAwdCfg);

    u8AwdNbr = ADC_AWD_1;
    stcAwdCfg.u8AdcPin = AWD1_PIN;
    stcAwdCfg.u16AwdMd = ADW1_MD;
    stcAwdCfg.u16Dr0   = AWD1_DR0_LOWER;
    stcAwdCfg.u16Dr1   = AWD1_DR1_UPPER;
    ADC_AwdConfig(u8AwdNbr, &stcAwdCfg);

#ifdef AWD_COMBINATION_MODE
    /* If combination mode is valid(ADC_AWD_COMB_OR/ADC_AWD_COMB_AND/ADC_AWD_COMB_XOR) and
       the channles selected by the AWD0 and AWD1 are deferent, make sure that the channel
       of AWD1 is converted after the channel conversion of AWD0 ends. */
    ADC_AwdConfigCombMode(AWD_COMBINATION_MODE);
#endif

    ADC_AwdCmd(ADC_AWD_0, Enable);
    ADC_AwdCmd(ADC_AWD_1, Enable);
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
