/**
 *******************************************************************************
 * @file  timer2/timer2_base_timer/source/main.c
 * @brief Main program TIMER2 base timer for the Device Driver Library.
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
#include "hc32_ddl.h"

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup Timer2_Base_Timer
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* If use interrupt. */
#define TIMER2_USE_INTERRUPT        (1u)

/* USE_TIMER2_INTERRUPT definition. */
#define SHARE_INTERRUPT             (1u)

/* System clock definition. */
#define CLK_HRC_32M                 (0u)
#define CLK_XTAL                    (1u)    /*!< 4MHz ~ 20MHz can be used. 20MHz for this example. */
#define SYSTEM_CLOCK                (CLK_XTAL)

/* Compare value definition. */
#if (SYSTEM_CLOCK == CLK_HRC_32M)
/* Set HCLK as TIMER2 clock source and HCLK is same as system clock(HRC 32MHz).
   Set TIMER2 clock prescaler is TIMER2_CLK_PRESCALER_16, then TIMER2 clock is 32/16 = 2MHz.
   CompareVal = TimerPeriod(us) * TIMER2ClockFrequency(MHz).
   In this example:
   TimerPeriod(1ms) = 1000us;
   TIMER2ClockFrequency = HCLK(32MHz) / Timer2ClockPrescaler(16) = 2MHz;
   CompareVal = 1000 * 2 = 2000. */
#define TIMER2_CLK_PRESCALER        (TIMER2_CLK_PRESCALER_16)
#define TIMER2_COMPARE_VALUE        (2000u)
#elif (SYSTEM_CLOCK == CLK_XTAL)
/* Set HCLK as TIMER2 clock source and HCLK is same as system clock(XTAL 20MHz).
   Set TIMER2 clock prescaler is TIMER2_CLK_PRESCALER_8, then TIMER2 clock is 20/8 = 2.5MHz.
   CompareVal = TimerPeriod(us) * TIMER2ClockFrequency(MHz).
   In this example:
   TimerPeriod(1ms) = 1000us;
   TIMER2ClockFrequency = HCLK(20MHz) / Timer2ClockPrescaler(8) = 2.5MHz;
   CompareVal = 1000 * 2.5 = 2500. */
#define TIMER2_CLK_PRESCALER        (TIMER2_CLK_PRESCALER_8)
#define TIMER2_COMPARE_VALUE        (2500u)
#else
#warning "If you use the other clock, calculate the comparison value yourself."
#endif

/* LED definition. */
#define LED_R_PORT                  (GPIO_PORT_3)
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
static void LedConfig(void);
static void Timer2Config(void);
#if TIMER2_USE_INTERRUPT
static void Timer2IrqConfig(void);
#endif

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_base_timer project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Configures the system clock. */
    SystemClockConfig();

    /* Configures LED. */
    LedConfig();

    /* Configures TIMER2. */
    Timer2Config();

    /* Starts TIMER2. */
    TIMER2_Start();

    /***************** Configuration end, application start **************/

    while (1u)
    {
#if TIMER2_USE_INTERRUPT
        /* See Timer2GCmp_IrqHandler in this file. */
#else
        /* Call TIMER2_GetFlag to check the flag state. */
        if (TIMER2_GetFlag(TIMER2_FLAG_CNT_MATCH) == Set)
        {
            TIMER2_ClrFlag(TIMER2_FLAG_CNT_MATCH);
            LED_R_TOGGLE();
        }
#endif
    }
}

/**
 * @brief  Configures a new system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
#if (SYSTEM_CLOCK == CLK_HRC_32M)
    /* Set EFM read latency when system clock greater than 24MHz. */
    EFM_SetLatency(EFM_LATENCY_1);

    /* Configure the system clock to HRC32MHz. */
    CLK_HRCInit(CLK_HRC_ON, CLK_HRCFREQ_32);
#elif (SYSTEM_CLOCK == CLK_XTAL)
    stc_clk_xtal_init_t stcXTALInit;

    /* Configure XTAL */
    stcXTALInit.u8XtalState  = CLK_XTAL_ON;
    stcXTALInit.u8XtalMode   = CLK_XTALMODE_OSC;
    stcXTALInit.u8XtalDrv    = CLK_XTALDRV_HIGH;
    stcXTALInit.u8XtalSupDrv = CLK_XTAL_SUPDRV_OFF;
    stcXTALInit.u8XtalStb    = CLK_XTALSTB_8;
    CLK_XTALInit(&stcXTALInit);

    /* Switch system clock from HRC(default) to XTAL */
    CLK_SetSysclkSrc(CLK_SYSCLKSOURCE_XTAL);
#else
#warning "If you use the other clock, configure it yourself."
#endif
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

    /* 3. Modify the configuration values depends on the application. */
    stcCfg.u16ClkPrescaler = TIMER2_CLK_PRESCALER;
    stcCfg.u16CompareVal   = TIMER2_COMPARE_VALUE;
#if TIMER2_USE_INTERRUPT
    /* Enable counter match interrupt. */
    stcCfg.enMatchIntCmd   = Enable;
#endif

    /* 4. Configures TIMER2 according the configuration value. */
    TIMER2_TimerConfig(&stcCfg);

#if TIMER2_USE_INTERRUPT
    /* 5. Configures IRQ if needed. */
    Timer2IrqConfig();
#endif
}

#if TIMER2_USE_INTERRUPT
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

    /* Configures counter match interrupt.
       The following 2 configurations of interrupt are both valid. */
#if SHARE_INTERRUPT
    /* Share interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_TMR2_GCMP;
    stcIrqRegiConf.enIRQn      = Int029_IRQn;
    INTC_ShareIrqCmd(stcIrqRegiConf.enIntSrc, Enable);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
#else
    /* Independent interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_TMR2_GCMP;
    stcIrqRegiConf.enIRQn      = Int018_IRQn;
    stcIrqRegiConf.pfnCallback = &Timer2GCmp_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
#endif // #if SHARE_INTERRUPT
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
#endif // #if TIMER2_USE_INTERRUPT

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
