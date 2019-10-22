/**
 *******************************************************************************
 * @file  timer4/timer4_sevt_delay_trigger/source/main.c
 * @brief This example demonstrates how to use the delay trigger function of 
 *        Timer4 SEVT function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-25       Hongjh          First version
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
 * @addtogroup TIMER4_SEVT_Delay_Trigger
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Wave I/O Port/Pin definition */
#define WAVE_IO_PORT                    (GPIO_PORT_6)
#define WAVE_IO_PIN                     (GPIO_PIN_3)
#define WAVE_IO_TOGGLE()                do {        \
    GPIO_TogglePins(WAVE_IO_PORT, WAVE_IO_PIN);     \
    GPIO_TogglePins(WAVE_IO_PORT, WAVE_IO_PIN);     \
} while(0)

/* Function clock gate definition */
#define FUNCTION_CLK_GATE               (CLK_FCG_ADC | CLK_FCG_AOS | CLK_FCG_TIM4)

/* Timer4 Counter period value && interrupt number definition */
#define TIMER4_CNT_CYCLE_VAL            ((uint16_t)(SystemCoreClock/512ul))    /* 1000 ms */

/* Timer4 OCO Channel definition */
#define TIMER4_OCO_CH                   (TIMER4_OCO_UL)

/* Timer4 SEVT channel definition */
#define TIMER4_SEVT_CH                  (TIMER4_SEVT_UL)

/* ADC channel */
#define ADC_SB_CHANNEL                  (ADC_CH2)

/* ADC channel sampling time. */
#define ADC_SAMPLE_TIME                 (10u)

/* ADC resolution definition. */
#define ADC_RESOLUTION                  (ADC_RESOLUTION_12B)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void WaveIoConfig(void);

static void AdcConfig(void);
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);
static void AdcTriggerConfig(void);
static void AdcSetChannelPinAnalogMode(uint16_t u16Channel);
static void AdcSetPinAnalogMode(uint8_t u8PinNbr);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of TIMER4 SEVT delay trigger
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_timer4_cnt_init_t stcTimer4CntInit;
    stc_timer4_oco_init_t stcTimer4OcoInit;
    stc_timer4_sevt_init_t stcTimer4SevtInit;
    static uint16_t m_u16AdcSbVal = 0u;

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure Wave I/O. */
    WaveIoConfig();

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Configure ADC. */
    AdcConfig();

    /* Initialize TIMER4 Counter. */
    TIMER4_CNT_StructInit(&stcTimer4CntInit);
    stcTimer4CntInit.u16ClkDiv = TIMER4_CNT_CLK_DIV512;
    /* Period_Value(1000ms) = SystemClock(SystemCoreClock) / TIMER4_CNT_Clock_Division(512) / Frequency(1) */
    stcTimer4CntInit.u16CycleVal = TIMER4_CNT_CYCLE_VAL;
    TIMER4_CNT_Init(&stcTimer4CntInit);

    /* Initialize TIMER4 OCO */
    TIMER4_OCO_StructInit(&stcTimer4OcoInit);
    stcTimer4OcoInit.enOcoCmd = Enable;
    stcTimer4OcoInit.u16OccrVal = TIMER4_CNT_CYCLE_VAL/4u;
    TIMER4_OCO_Init(TIMER4_OCO_CH, &stcTimer4OcoInit);

    /* Initialize TIMER4 SEVT. */
    TIMER4_SEVT_StructInit(&stcTimer4SevtInit);
    stcTimer4SevtInit.u16SccrVal = TIMER4_CNT_CYCLE_VAL/4u;
    stcTimer4SevtInit.stcTriggerCond.enUpMatchCmd = Enable;
    stcTimer4SevtInit.u16Mode = TIMER4_SEVT_MODE_DELAY_TRIGGER;
    stcTimer4SevtInit.u16TrigEvt = TIMER4_SEVT_TRIGGER_TMR4_SCMUL;
    TIMER4_SEVT_Init(TIMER4_SEVT_CH, &stcTimer4SevtInit);
    TIMER4_SEVT_SetDelayObject(TIMER4_SEVT_CH, TIMER4_SEVT_DELAY_OCCRXL);

    /* Start TIMER4 counter. */
    TIMER4_CNT_Start();

    while (1u)
    {
        if (ADC_GetEocFlag(ADC_FLAG_EOCB) == Set)
        {
            ADC_GetChannelData(ADC_SB_CHANNEL, &m_u16AdcSbVal, 1u);
            ADC_ClrEocFlag(ADC_FLAG_EOCB);
            WAVE_IO_TOGGLE();
        }
    }
}

/**
 * @brief  Configure system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    /* Configure the system clock to HRC32MHz. */
    CLK_HRCInit(CLK_HRC_ON, CLK_HRCFREQ_32);
}

/**
 * @brief  Configure Wave I/O.
 * @param  None
 * @retval None
 */
static void WaveIoConfig(void)
{
    stc_gpio_init_t stcGpioInit = {0};

    stcGpioInit.u16PinMode = PIN_MODE_OUT;
    stcGpioInit.u16PinState = PIN_STATE_RESET;
    GPIO_Init(WAVE_IO_PORT, WAVE_IO_PIN, &stcGpioInit);
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
    AdcTriggerConfig();
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
    stcInit.u16ScanConvMode = ADC_MODE_SA_SB_SINGLE;
    stcInit.u16Resolution   = ADC_RESOLUTION;
    stcInit.u8SampTime      = ADC_SAMPLE_TIME;
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
    AdcSetChannelPinAnalogMode(ADC_SB_CHANNEL);

    /* 2. Add ADC sequence B channels. */
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

    /* Set a default value for stcTrgCfg.  */
    ADC_TriggerSrcStructInit(&stcTrgCfg);

    /* Configrues sequence B's trigger source.
       Set EOCA(End Of Conversion of sequence A) as the trigger source of sequence B.
       Sequence B can only be started by the trigger source. */
    u8Seq = ADC_SEQ_B;
    /* The trigger source of sequence B is not recommended to be set to ADC_TRGSRC_EX_PIN at here. */
    stcTrgCfg.u16TrgSrc = ADC_TRGSRC_IN_EVT0;
    stcTrgCfg.u32Event0 = EVT_TMR4_SCMUL;

    /* 1. Configrues the trigger source of sequence B. */
    ADC_ConfigTriggerSrc(u8Seq, &stcTrgCfg);
    /* 2. Enable the trigger source. */
    ADC_TriggerSrcCmd(u8Seq, Enable);
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
    uint8_t u8Port = GPIO_PORT_0;
    uint8_t u8Pin = GPIO_PIN_MASK;
    en_result_t enRet = Ok;

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
            enRet = ErrorInvalidParameter;
            break;
    }

    if (Ok == enRet)
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
