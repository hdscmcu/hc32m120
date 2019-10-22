/**
 *******************************************************************************
 * @file  timer2/timer2_hardware_control/source/main.c
 * @brief Main program TIMER2 hardware control for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-20       Wuze            First version
   2019-10-21       Wuze            Modified the pin of SW1 from P17 to P22.
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
 * @addtogroup Timer2_Hardware_Control
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Event pin definition.
   SW1 definition. Press SW1 to generating the event EVT_PORT_EIRQ2 to start TIMER2. */
#define SW1_PORT                    (GPIO_PORT_2)
#define SW1_PIN                     (GPIO_PIN_2)
#define SW1_EXINT_CH                (EXINT_CH02)
#define SW1_EVENT                   (EVT_PORT_EIRQ2)

/* Start condition definition of TIMER2. */
#define TIMER2_START_CONDITION      (TIMER2_HW_START_EVENT)
#define TIMER2_START_EVENT          (SW1_EVENT)

/* Stop condition definition of TIMER2.
   Use the falling edge of TRIGA to stop TIMER2.
   One of these pins P00, P01, P30, P31, P41 and P50 can be used as TRIGA input pin. */
#define TIMER2_TRIGA_PORT           (GPIO_PORT_3)
#define TIMER2_TRIGA_PIN            (GPIO_PIN_0)
#define TIMER2_STOP_CONDITION       (TIMER2_HW_STOP_TRIGA_FALL)

/* Set HCLK as TIMER2 clock source and HCLK is same as system clock(HRC 32MHz).
   Set TIMER2 clock prescaler is TIMER2_CLK_PRESCALER_32, then TIMER2 clock is 32/32 = 1MHz.
   CompareVal = TimerPeriod(us) * TIMER2ClockFrequency(MHz).
   In this example:
   TimerPeriod(50ms) = 50000us;
   TIMER2ClockFrequency = HCLK(32MHz) / Timer2ClockPrescaler(32) = 1MHz;
   CompareVal = 50000 * 1 = 50000. */
#define TIMER2_CLK_PRESCALER        (TIMER2_CLK_PRESCALER_32)
#define TIMER2_COMPARE_VALUE        (50000u)

/* LED definition. */
#define LED_R_PORT                  (GPIO_PORT_12)
#define LED_R_PIN                   (GPIO_PIN_0)
#define LED_R_ON()                  GPIO_ResetPins(LED_R_PORT, LED_R_PIN)
#define LED_R_OFF()                 GPIO_SetPins(LED_R_PORT, LED_R_PIN)
#define LED_R_TOGGLE()              GPIO_TogglePins(LED_R_PORT, LED_R_PIN)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void EventPinInit(void);
static void Timer2Config(void);
static void Timer2IrqConfig(void);
static void LedConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_hardware_control project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Configure the system clock to HRC32MHz. */
    SystemClockConfig();

    /* Initializes the event pin. The event is used to start TIMER2. */
    EventPinInit();

    /* Configures LED. */
    LedConfig();

    /* Configures TIMER2. */
    Timer2Config();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        /* Press SW1(genterate the event EVT_PORT_EIRQ2) to start TIMER2 and the red LED starts flashing.
           Making a falling edge of TRIGA to stop TIMER2 and the red LED stop flashing. */
    }
}

/**
 * @brief  Configures a new system clock -- HRC32MHz.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    /* Set EFM read latency when system clock greater than 24MHz. */
    EFM_SetLatency(EFM_LATENCY_1);

    /* Configure the system clock to HRC32MHz. */
    CLK_HRCInit(CLK_HRC_ON, CLK_HRCFREQ_32);
}

/**
 * @brief  Initializes the event pin.
 * @param  None
 * @retval None
 */
static void EventPinInit(void)
{
    stc_exint_config_t stcExintCfg;
    stc_gpio_init_t    stcGpioIni;

    GPIO_StructInit(&stcGpioIni);
    stcGpioIni.u16ExInt = PIN_EXINT_ON;
    GPIO_Init(SW1_PORT, SW1_PIN, &stcGpioIni);
    EXINT_StructInit(&stcExintCfg);
    stcExintCfg.u16ExIntCh = SW1_EXINT_CH;
    EXINT_Init(&stcExintCfg);
}

/**
 * @brief  TIMER2 configuration.
 * @param  None
 * @retval None
 */
static void Timer2Config(void)
{
    stc_timer2_config_t stcCfg;

    /* 1. Enable TIMER2 peripheral clock. */
    CLK_FcgPeriphClockCmd(CLK_FCG_TIM2, Enable);

    /* 2. Set a default configuration value for stcCfg.
          Default configuration values for basic timer is:
            Clock sourc(u16ClkSource): TIMER2_SYNC_CS_HCLK, default clock is HCLK.
            Clock prescaler(u16ClkPrescaler): TIMER2_CLK_PRESCALER_1, prescaler of clock source is 1. 
            Start condition(u16HwStartCondition): TIMER2_HW_START_INVALID, do not use the start condition.
            Stop condition(u16HwStopCondition): TIMER2_HW_STOP_INVALID, do not use the stop condition.
            Clear condition(u16HwClearCondition): TIMER2_HW_CLR_INVALID, do not use the clear condition.
            TRIGA filter clock prescaler(u16FilterPrescaler): TIMER2_FILTER_PRESCALER_1, valid while enFilterCmd == Enable.
            TRIGA filter function(enFilterCmd): Disable.
            Counter match interrupt(enMatchIntCmd): Disable.
            Counter overflow interrupt(enOvfIntCmd): Disable.
          All of the above configuration values can be modified based on the application. */
    TIMER2_StructInit(&stcCfg);

    /* 3. Modify the configuration value depends on the application. */
    stcCfg.u16ClkPrescaler     = TIMER2_CLK_PRESCALER;
    stcCfg.u16CompareVal       = TIMER2_COMPARE_VALUE;
    stcCfg.u16HwStartCondition = TIMER2_START_CONDITION;
    stcCfg.u16HwStopCondition  = TIMER2_STOP_CONDITION;
    /* Set clock prescaler of TRIGA filter and enable the filter, if needed. */
    stcCfg.u16FilterPrescaler  = TIMER2_FILTER_PRESCALER_4;
    stcCfg.enFilterCmd         = Enable;
    stcCfg.enMatchIntCmd       = Enable;

    /* 4. Configures TIMER2 according the configuration value. */
    TIMER2_TimerConfig(&stcCfg);

    /* 5. Set the event as the start condition of TIMER2.
          TIMER2 will be started when the specified event(EVT_PORT_EIRQ2 for this example) occurs. */
    /* Enable AOS function. */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);
    /* Set the start event.  */
    TIMER2_SetTrigEvent(TIMER2_START_EVENT);

    /* 6. Set the specified pin as TRIGA input pin, for generating the stop condition. */
    GPIO_SetFunc(TIMER2_TRIGA_PORT, TIMER2_TRIGA_PIN, GPIO_FUNC_3_TIM2);

    /* 7. Configures IRQ if needed. */
    Timer2IrqConfig();
}

/**
 * @brief  TIMER2 interrupt configuration.
 * @param  None
 * @retval None
 * @note   All TIMER2 interrupts can be configured as independent interrupt or shared interrupt.
 *         INT_TMR2_GCMP: Counter comparison match(CNTAR == CMPAR) or a capturing occurred.
 *                        Independent vec[Int018_IRQn, Int019_IRQn]
 *                        Share vec [Int029_IRQn]
 *         INT_TMR2_GOVF: Counter overflow(CNTAR == 0xFFFF).
 *                        Independent vec[Int018_IRQn, Int019_IRQn]
 *                        Share vec[Int029_IRQn]
 */
static void Timer2IrqConfig(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;

    /* Configures counter match interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_TMR2_GCMP;
    stcIrqRegiConf.enIRQn      = Int018_IRQn;
    stcIrqRegiConf.pfnCallback = &Timer2GCmp_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

/**
 * @brief  TIMER2 counter comparison match interrupt callback function.
 * @param  None
 * @retval None
 */
void Timer2GCmp_IrqHandler(void)
{
    if (TIMER2_GetFlag(TIMER2_FLAG_CNT_MATCH) == Set)
    {
        TIMER2_ClrFlag(TIMER2_FLAG_CNT_MATCH);
        LED_R_TOGGLE();
    }
}

/**
 * @brief  Led configuration.
 * @param  None
 * @retval None
 */
static void LedConfig(void)
{
    stc_gpio_init_t stcGpioInit;

    GPIO_StructInit(&stcGpioInit);

    /* LED R initialize */
    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);

    /* "Turn off" LED before set to output */
    LED_R_OFF();

    /* Output enable */
    GPIO_OE(LED_R_PORT, LED_R_PIN, Enable);
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
