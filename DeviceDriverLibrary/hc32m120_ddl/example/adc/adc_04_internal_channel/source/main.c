/**
 *******************************************************************************
 * @file  adc/adc_04_internal_channel/source/main.c
 * @brief Main program of ADC internal channel for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-16       Wuze            First version
   2019-10-18       Wuze            Added code for calculating the temperature.
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
 * @addtogroup ADC_Internal_Channel
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Function definition for this application. */
#define APP_FUNC_GET_INREF          (1u)
#define APP_FUNC_CAL_TEMP           (2u)
#define APP_FUNCTION                (APP_FUNC_CAL_TEMP)
 
/* ADC channels definition for this example. */
#define ADC_SA_CHANNEL              (ADC_CH11)
#define ADC_SA_CHANNEL_COUNT        (1u)

/* ADC channel sampling time. */
#define ADC_SAMPLE_TIME             ((uint8_t)10)

/* ADC resolution definition. */
#define ADC_RESOLUTION              (ADC_RESOLUTION_12B)

/* ADC accuracy. */
#define ADC_ACCURACY                (1ul << 12u)

/* ADC reference voltage. The voltage of pin VREFH. */
#define ADC_VREF                    (3.29f)

/* Timeout value definitions. */
#define TIMEOUT_MS                  (10u)

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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static float m_f32Vol;
static uint16_t m_au16AdcSaVal[ADC_SA_CHANNEL_COUNT];

#if (APP_FUNCTION == APP_FUNC_CAL_TEMP)
static float m_f32Temp;
#endif

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_04_internal_channel project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The system clock is set to HRC8HMz by default. */

    /* Configures ADC. */
    AdcConfig();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        ADC_PollingSa(m_au16AdcSaVal, ADC_SA_CHANNEL_COUNT, TIMEOUT_MS);
        m_f32Vol = ((float)m_au16AdcSaVal[0u] * ADC_VREF) / ((float)ADC_ACCURACY);
#if (APP_FUNCTION == APP_FUNC_CAL_TEMP)
        /* temperature = 25 - (((adc_voltage - 1.03) * 1000) / 3.5) */
        m_f32Temp = 25.f - (((m_f32Vol - 1.03f) * 1000.f) / 3.5f);
        (void)m_f32Temp;
#else
        (void)m_f32Vol;
#endif /* #if (APP_FUNCTION == APP_FUNC_CAL_TEMP) */
    }
}

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
}

/**
 * @brief  Configures ADC clock.
 *         The clock of ADC: HRC8MHz / 4 = 2MHz.
 * @param  None
 * @retval None
 */
static void AdcClockConfig(void)
{
    CLK_SetADClkDiv(CLK_HCLK_DIV4);
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
    /* 1. Configures the source of extend channel.
          PWC_PWRMON_VINREF: Internal reference voltage.
          PWC_PWRMON_VOTS: Internal temperature sensor. */
    stc_pwc_pwrmon_init_t stcPwrMonInit =
    {
        PWC_PWRMON_ON,
#if (APP_FUNCTION == APP_FUNC_CAL_TEMP)
        PWC_PWRMON_VOTS
#else
        PWC_PWRMON_VINREF
#endif
    };
    PWC_PwrMonInit(&stcPwrMonInit);

    /* 2. Select internal analog input for ADC extend channel. */
    ADC_SetExChannel(ADC_EX_CH_SEL_INTERNAL);

    /* 3. Add ADC channels. */
    ADC_AddAdcChannel(ADC_SEQ_A, ADC_SA_CHANNEL);
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
