/**
 *******************************************************************************
 * @file  timerb/timerb_base_timer_32bit/source/main.c
 * @brief This example demonstrates TIMERB base count 32bit function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-03       Hongjh          First version
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
 * @addtogroup TIMERB_Base_Timer_32Bit
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

/* TIMERB unit & interrupt number & counter period value definition */
#define TIMERB_ODD_UNIT                 (M0P_TMRB1)
#define TIMERB_ODD_UNIT_OVF_INT         (INT_TMRB_1_OVF)
#define TIMERB_ODD_UNIT_OVF_IRQn        (Int020_IRQn)
#define TIMERB_ODD_UNIT_PERIOD_VALUE    (SystemCoreClock/512ul/TIMERB_EVEN_UNIT_PERIOD_VALUE)

#define TIMERB_EVEN_UNIT                (M0P_TMRB2)
#define TIMERB_EVEN_UNIT_OVF_INT        (INT_TMRB_2_OVF)
#define TIMERB_EVEN_UNIT_OVF_IRQn       (Int022_IRQn)
#define TIMERB_EVEN_UNIT_PERIOD_VALUE   (100u)

/* Function clock gate definition */
#define FUNCTION_CLK_GATE               (CLK_FCG_TIMB1 | CLK_FCG_TIMB2)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void TimerbOddUnitOvfIrqCallback(void);
static void TimerbEvenUnitOvfIrqCallback(void);

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
 * @brief  TIMERB odd unit overflow IRQ callback.
 * @param  None
 * @retval None
 */
static void TimerbOddUnitOvfIrqCallback(void)
{
    TIMERB_ClearFlag(TIMERB_ODD_UNIT, TIMERB_FLAG_OVF);
}

/**
 * @brief  TIMERB even unit overflow IRQ callback
 * @param  None
 * @retval None
 */
static void TimerbEvenUnitOvfIrqCallback(void)
{
    LED_G_TOGGLE();

    TIMERB_ClearFlag(TIMERB_EVEN_UNIT, TIMERB_FLAG_OVF);
}

/**
 * @brief  Main function of TIMERB base timer 32bit project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;
    static stc_timerb_init_t stcTimerbInit;

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure RGB LED. */
    LedConfig();

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Initialize TIMERB odd unit. */
    TIMERB_StructInit(&stcTimerbInit);
    stcTimerbInit.u16PeriodVal = (uint16_t)TIMERB_ODD_UNIT_PERIOD_VALUE;
    stcTimerbInit.u16ClkDiv = TIMERB_CLKDIV_DIV512;
    TIMERB_Init(TIMERB_ODD_UNIT, &stcTimerbInit);
    TIMERB_IntCmd(TIMERB_ODD_UNIT, TIMERB_IT_OVF, Enable);

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = TIMERB_ODD_UNIT_OVF_IRQn;
    stcIrqRegiConf.enIntSrc = TIMERB_ODD_UNIT_OVF_INT;
    stcIrqRegiConf.pfnCallback = &TimerbOddUnitOvfIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Initialize TIMERB even unit. */
    stcTimerbInit.u16PeriodVal = TIMERB_EVEN_UNIT_PERIOD_VALUE;
    TIMERB_Init(TIMERB_EVEN_UNIT, &stcTimerbInit);
    TIMERB_IntCmd(TIMERB_EVEN_UNIT, TIMERB_IT_OVF, Enable);
    TIMERB_SetHwUpCondition(TIMERB_EVEN_UNIT, TIMERB_HWUP_OVERFLOW); /* link odd uint for 32bit counter */

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = TIMERB_EVEN_UNIT_OVF_IRQn;
    stcIrqRegiConf.enIntSrc = TIMERB_EVEN_UNIT_OVF_INT;
    stcIrqRegiConf.pfnCallback = &TimerbEvenUnitOvfIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Start TIMERB counter. */
    TIMERB_Start(TIMERB_EVEN_UNIT);
    TIMERB_Start(TIMERB_ODD_UNIT);

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
