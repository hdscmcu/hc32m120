/**
 *******************************************************************************
 * @file  timer4/timer4_cnt_sawtooth_wave_mode/source/main.c
 * @brief This example demonstrates Timer4 Counter sawtooth wave mode.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-24       Hongjh          First version
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
 * @addtogroup TIMER4_Counter_Sawtooth_Wave_Mode
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Green LED Port/Pin definition */
#define LED_G_PORT                      (GPIO_PORT_7)
#define LED_G_PIN                       (GPIO_PIN_0)
#define LED_G_TOGGLE()                  (GPIO_TogglePins(LED_G_PORT, LED_G_PIN))

/* Function clock gate definition */
#define FUNCTION_CLK_GATE               (CLK_FCG_TIM4)

/* Timer4 Counter period value && interrupt number definition */
#define TIMER4_CNT_CYCLE_VAL            ((uint16_t)(SystemCoreClock/256ul/2ul))    /* 500 ms */
#define TIMER4_CNT_UDF_INT              (INT_TMR4_GUDF)
#define TIMER4_CNT_UDF_IRQn             (Int017_IRQn)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void LedConfig(void);
static void Timer4ZeroMatchIrqCb(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

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
 * @brief  Configure RGB LED.
 * @param  None
 * @retval None
 */
static void LedConfig(void)
{
    stc_gpio_init_t stcGpioInit = {0};

    stcGpioInit.u16PinMode = PIN_MODE_OUT;
    stcGpioInit.u16PinState = PIN_STATE_SET;
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
}

/**
 * @brief  TIMER4 Counter zero match interrupt handler callback.
 * @param  None
 * @retval None
 */
static void Timer4ZeroMatchIrqCb(void)
{
    LED_G_TOGGLE();

    TIMER4_CNT_ClearFlag(TIMER4_CNT_FLAG_ZERO);
}

/**
 * @brief  Main function of TIMER4 counter sawtooth wave mode project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;
    stc_timer4_cnt_init_t stcTimer4CntInit;

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure RGB LED. */
    LedConfig();

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Initialize TIMER4 Counter */
    TIMER4_CNT_StructInit(&stcTimer4CntInit);
    /* Period_Value(500ms) = SystemClock(SystemCoreClock) / TIMER4_CNT_Clock_Division(256) / Frequency(2) */
    stcTimer4CntInit.u16CycleVal = TIMER4_CNT_CYCLE_VAL;
    stcTimer4CntInit.u16ClkDiv = TIMER4_CNT_CLK_DIV256;
    stcTimer4CntInit.enZeroIntCmd = Enable;
    TIMER4_CNT_Init(&stcTimer4CntInit);

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = TIMER4_CNT_UDF_IRQn;
    stcIrqRegiConf.enIntSrc = TIMER4_CNT_UDF_INT;
    stcIrqRegiConf.pfnCallback = &Timer4ZeroMatchIrqCb;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Start TIMER4 counter. */
    TIMER4_CNT_Start();

    while (1)
    {
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
