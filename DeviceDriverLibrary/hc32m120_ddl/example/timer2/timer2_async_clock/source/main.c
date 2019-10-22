/**
 *******************************************************************************
 * @file  timer2/timer2_async_clock/source/main.c
 * @brief Main program TIMER2 asynchronous clock for the Device Driver Library.
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
 * @addtogroup Timer2_Async_Clock
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Asynchronous clock source definition. */
#define ASYNC_CLOCK_LRC             (0u)    /*!< Asynchronous clock source is LRC(32.768KHz) */
#define ASYNC_CLOCK_CLKA            (1u)    /*!< Asynchronous clock source input from pin CLKA. */
#define TIMER2_ASYNC_CLOCK          (ASYNC_CLOCK_CLKA)

/* Comparison value definition.
   CompareVal = TimerPeriod(us) * TIMER2ClockFrequency(MHz).
   In this example:
        TimerPeriod(50ms) = 50000us;
        TIMER2 clock prescaler = 1;
        TIMER2 Clock source:
   LRC:
        TIMER2ClockFrequency = LRC(32.768KHz) / Prescaler(1) = 0.032768MHz;
        CompareVal = 50000 * 0.032768 = 1638.
   CLKA:
        TIMER2ClockFrequency = CLKA(10KHz, denpends on the application) / Prescaler(1) = 0.01MHz;
        CompareVal = 50000 * 0.01 = 500. */
#if (TIMER2_ASYNC_CLOCK == ASYNC_CLOCK_LRC)
#define TIMER2_ASYNC_CLOCK_SOURCE   (TIMER2_ASYNC_CS_LRC)
#define TIMER2_COMPARE_VALUE        (1638u)
#elif (TIMER2_ASYNC_CLOCK == ASYNC_CLOCK_CLKA)
#define TIMER2_ASYNC_CLOCK_SOURCE   (TIMER2_ASYNC_CS_CLKA)
#define TIMER2_COMPARE_VALUE        (500u)
#define TIMER2_CLKA_PORT            (GPIO_PORT_6)
#define TIMER2_CLKA_PIN             (GPIO_PIN_2)
#else
#error "Please choose a correct asynchronous clock source!!!"
#endif

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
 * @brief  Main function of timer2_async_clock project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The system clock is set to HRC8MHz by default. */

    /* Configures LED. */
    LedConfig();

    /* Configures TIMER2. */
    Timer2Config();

    /* Starts TIMER2. TIMER2 will start after 3 asynchronous clock cycles. */
    TIMER2_Start();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        /* If you want to write a TIMER2 register, you need to wait for at least 3 asynchronous clock cycles
           after the last write operation! e.g.:
           DDL_Delay1ms(xxx); // Wait at least 3 asynchronous clock cycles.
           TIMER2_SetCompareVal(111);
           TIMER2_SetCounterVal(222);
           DDL_Delay1ms(xxx); // Wait at least 3 asynchronous clock cycles.
           TIMER2_SetCompareVal(333);
           TIMER2_SetCounterVal(444); */
    }
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

    /* 2. Set a default configuration value for stcCfg. */
    TIMER2_StructInit(&stcCfg);

    /* 3. Modify the configuration value depends on the application. */
    stcCfg.u16ClkSource    = TIMER2_ASYNC_CLOCK_SOURCE;
    stcCfg.u16CompareVal   = TIMER2_COMPARE_VALUE;
    /* Enable counter match interrupt if needed. */
    stcCfg.enMatchIntCmd   = Enable;

    /* 4. Configures TIMER2 according the configuration value. */
    TIMER2_TimerConfig(&stcCfg);

#if (TIMER2_ASYNC_CLOCK == ASYNC_CLOCK_LRC)
    /* 5. If asynchronous clock is LRC, enable the LRC. */
    CLK_LRCInit(CLK_LRC_ON);
#else
    /* 5. If asynchronous clock is input from pin CLKA,
          set the specified pin as asynchronous clock input pin. */
    GPIO_SetFunc(TIMER2_CLKA_PORT, TIMER2_CLKA_PIN, GPIO_FUNC_3_TIM2);
#endif

    /* 6. Configures IRQ if needed. */
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

    /* Configures INT_TMR2_GCMP interrupt. */
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

    /* TURN OFF LED before set to output */
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
