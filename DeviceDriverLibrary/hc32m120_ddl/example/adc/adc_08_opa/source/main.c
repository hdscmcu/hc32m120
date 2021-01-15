/**
 *******************************************************************************
 * @file  adc/adc_08_opa/source/main.c
 * @brief Main program ADC OPA for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-18       Wuze            First version
   2021-01-07       Wuze            Fixed a bug of function AdcOpaConfig().
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
 * @addtogroup ADC_Opa
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* OPA working mode definition. */
#define OPA_INDEPENDENT             (0u)
#define OPA_WITH_ADC                (1u)
#define OPA_WORK_MODE               (OPA_WITH_ADC)

/* OPAM pin definitions. */
#define OPA1_P_PORT                 (GPIO_PORT_2)
#define OPA1_P_PIN                  (GPIO_PIN_1)
#define OPA1_O_PORT                 (GPIO_PORT_2)
#define OPA1_O_PIN                  (GPIO_PIN_0)
#define OPA1_M_PORT                 (GPIO_PORT_2)
#define OPA1_M_PIN                  (GPIO_PIN_2)

#define OPA2_P_PORT                 (GPIO_PORT_14)
#define OPA2_P_PIN                  (GPIO_PIN_7)
#define OPA2_O_PORT                 (GPIO_PORT_2)
#define OPA2_O_PIN                  (GPIO_PIN_3)
#define OPA2_M_PORT                 (GPIO_PORT_1)
#define OPA2_M_PIN                  (GPIO_PIN_0)

#if (OPA_WORK_MODE == OPA_WITH_ADC)
/* ADC channels definition for this example. */
#define OPA1_IN_PIN                 (ADC_ANI1)
#define OPA1_OUT_PIN                (ADC_ANI0)
#define OPA1_IN_CH                  ((uint16_t)(1ul << OPA1_IN_PIN))
#define OPA1_OUT_CH                 ((uint16_t)(1ul << OPA1_OUT_PIN))

#define OPA2_IN_PIN                 (ADC_ANI8)
#define OPA2_OUT_PIN                (ADC_ANI3)
#define OPA2_IN_CH                  ((uint16_t)(1ul << OPA2_IN_PIN))
#define OPA2_OUT_CH                 ((uint16_t)(1ul << OPA2_OUT_PIN))

#define ADC_OPA_CHANNEL             (OPA1_IN_CH | OPA1_OUT_CH | OPA2_IN_CH | OPA2_OUT_CH)
#define ADC_NORMAL_CHANNEL          (ADC_CH10)

#define ADC_SA_CHANNEL              (ADC_OPA_CHANNEL | ADC_NORMAL_CHANNEL)
#define ADC_SA_CHANNEL_COUNT        (5u)

/* ADC channel sampling time. */
#define ADC_SAMPLE_TIME             ((uint8_t)50)

/* ADC resolution definition. */
#define ADC_RESOLUTION              (ADC_RESOLUTION_12B)

/* ADC accuracy. */
#define ADC_ACCURACY                (1ul << 12u)

/* ADC reference voltage. The voltage of pin VREFH. */
#define ADC_VREF                    (3.307f)

/* Timeout value definitions. */
#define TIMEOUT_MS                  (10u)

/* Rp(KR) and Rn(KR) definitions. The amplification factor depends on the Rp and Rn. */
#define OPA_RN                      (5u)
#define OPA1_RP                     (30u)
#define OPA2_RP                     (10u)
#define OPA1_FACTOR                 (1u + (OPA1_RP / OPA_RN))
#define OPA2_FACTOR                 (1u + (OPA2_RP / OPA_RN))

/* Values of OPA definitions. */
#define opa1_in_val                 (m_au16AdcSaVal[1u])
#define opa1_out_val                (m_au16AdcSaVal[0u])
#define opa2_in_val                 (m_au16AdcSaVal[3u])
#define opa2_out_val                (m_au16AdcSaVal[2u])
#endif // #if (OPA_WORK_MODE == OPA_WITH_ADC)

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
#if (OPA_WORK_MODE == OPA_WITH_ADC)
static void AdcConfig(void);
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);

static void AdcSetChannelPinAnalogMode(uint16_t u16Channel);
static void AdcSetPinAnalogMode(uint8_t u8PinNbr);
#endif // #if (OPA_WORK_MODE == OPA_WITH_ADC)
static void AdcOpaConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
#if (OPA_WORK_MODE == OPA_WITH_ADC)
static float m_f32VolOpa1In;
static float m_f32VolOpa2In;
static float m_f32VolOpa1Out;
static float m_f32VolOpa2Out;
static uint16_t m_au16AdcSaVal[ADC_SA_CHANNEL_COUNT];
#endif // #if (OPA_WORK_MODE == OPA_WITH_ADC)

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_08_opa project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
#if (OPA_WORK_MODE == OPA_INDEPENDENT)
    AdcOpaConfig();
#else
    #if (DDL_PRINT_ENABLE == DDL_ON)
        /* Initializes UART for debug printing. Baudrate is 115200. */
        DDL_UartInit();
    #endif // #if (DDL_PRINT_ENABLE == DDL_ON)

    /* Configures ADC. */
    AdcConfig();
#endif // #if (OPA_WORK_MODE == OPA_INDEPENDENT)

    /***************** Configuration end, application start **************/

    while (1u)
    {
#if (OPA_WORK_MODE == OPA_WITH_ADC)
        ADC_PollingSa(m_au16AdcSaVal, ADC_SA_CHANNEL_COUNT, TIMEOUT_MS);

        /* Input voltage of OPA1: */
        m_f32VolOpa1In  = ((float)opa1_in_val * ADC_VREF) / ((float)ADC_ACCURACY);
        DBG("\nOPA1 in voltage:  %.3f.", m_f32VolOpa1In);
        /* Output voltage of OPA1: */
        m_f32VolOpa1Out = ((float)opa1_out_val * ADC_VREF) / ((float)ADC_ACCURACY);
        DBG("\nAfter amplified %d times by OPA1, out voltage: %.3f.", OPA1_FACTOR, m_f32VolOpa1Out);

        /* Input voltage of OPA2: */
        m_f32VolOpa2In  = ((float)opa2_in_val * ADC_VREF) / ((float)ADC_ACCURACY);
        DBG("\nOPA2 in voltage:  %.3f.", m_f32VolOpa2In);
        /* Output voltage of OPA2: */
        m_f32VolOpa2Out = ((float)opa2_out_val * ADC_VREF) / ((float)ADC_ACCURACY);
        DBG("\nAfter amplified %d times by OPA2, out voltage: %.3f.", OPA2_FACTOR, m_f32VolOpa2Out);

#if (DDL_PRINT_ENABLE != DDL_ON)
        (void)m_f32VolOpa1In;
        (void)m_f32VolOpa2In;
        (void)m_f32VolOpa1Out;
        (void)m_f32VolOpa2Out;
#endif // #if (DDL_PRINT_ENABLE == DDL_ON)

        DDL_Delay1ms(500u);

#endif // #if (OPA_WORK_MODE == OPA_WITH_ADC)
    }
}

#if (OPA_WORK_MODE == OPA_WITH_ADC)
/**
 * @brief  ADC configuration, including clock configuration, initial configuration
 *         and channel configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
    AdcOpaConfig();
}

/**
 * @brief  Configures ADC clock.
 *         The clock of ADC: HRC32MHz / 2 = 16MHz.
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
    /* 1. Set the ADC pin to analog input mode. */
    AdcSetChannelPinAnalogMode(ADC_NORMAL_CHANNEL);

    /* 2. Add ADC channels. */
    ADC_AddAdcChannel(ADC_SEQ_A, ADC_SA_CHANNEL);
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
#endif // #if (OPA_WORK_MODE == OPA_WITH_ADC)

/**
 * @brief  ADC OPA configuration. Set the function of OPA pins and enable OPA function.
 * @param  None
 * @retval None
 */
static void AdcOpaConfig(void)
{
#if (OPA_WORK_MODE == OPA_INDEPENDENT)
    /* 1. Enable ADC peripheral. */
    CLK_FcgPeriphClockCmd(CLK_FCG_ADC, Enable);
#endif

    /* 2. Set the function of OPAP, OPAO and OPAM. */
    GPIO_SetFunc(OPA1_P_PORT, OPA1_P_PIN, GPIO_FUNC_1_OPA);
    GPIO_SetFunc(OPA1_O_PORT, OPA1_O_PIN, GPIO_FUNC_1_OPA);
    GPIO_SetFunc(OPA1_M_PORT, OPA1_M_PIN, GPIO_FUNC_1_OPA);

    GPIO_SetFunc(OPA2_P_PORT, OPA2_P_PIN, GPIO_FUNC_1_OPA);
    GPIO_SetFunc(OPA2_O_PORT, OPA2_O_PIN, GPIO_FUNC_1_OPA);
    GPIO_SetFunc(OPA2_M_PORT, OPA2_M_PIN, GPIO_FUNC_1_OPA);

    /* 3. Enable OPA. */
    ADC_OpaCmd(ADC_OPA_1, Enable);
    ADC_OpaCmd(ADC_OPA_2, Enable);
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
