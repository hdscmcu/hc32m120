/**
 *******************************************************************************
 * @file  timera/timera_phase_difference/source/main.c
 * @brief This example demonstrates TIMERA phase difference function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-04       Yangjp          First version
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
 * @addtogroup TIMERA_Phase_Difference
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* LED_R Port/Pin definition */
#define LED_R_PORT                      (GPIO_PORT_12)
#define LED_R_PIN                       (GPIO_PIN_0)

#define LED_R_ON()                      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_R_OFF()                     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))
#define LED_R_TOGGLE()                  (GPIO_TogglePins(LED_R_PORT, LED_R_PIN))

/* LED_G Port/Pin definition */
#define LED_G_PORT                      (GPIO_PORT_7)
#define LED_G_PIN                       (GPIO_PIN_0)

#define LED_G_ON()                      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))
#define LED_G_OFF()                     (GPIO_SetPins(LED_G_PORT, LED_G_PIN))
#define LED_G_TOGGLE()                  (GPIO_TogglePins(LED_G_PORT, LED_G_PIN))

/* TIMERA unit definition */
#define TIMERA_UNIT1                    (M0P_TMRA)
#define TIMERA_UNIT1_CLOCK              (CLK_FCG_TIMA)
#define TIMERA_UNIT1_OVF_INT            (TIMERA_INT_OVF)
#define TIMERA_UNIT1_OVF_INTn           (INT_TMRA_OVF)
#define TIMERA_UNIT1_OVF_IRQn           (Int016_IRQn)
#define TIMERA_UNIT1_UDF_INT            (TIMERA_INT_UDF)
#define TIMERA_UNIT1_UDF_INTn           (INT_TMRA_UDF)
#define TIMERA_UNIT1_UDF_IRQn           (Int017_IRQn)
#define TIMERA_UNIT1_PERIOD_VALUE       (1000u)

/* TIMERA CLKA Port/Pin definition */
#define TIMERA_UNIT1_CLKA_PORT          (GPIO_PORT_7)
#define TIMERA_UNIT1_CLKA_PIN           (GPIO_PIN_5)
#define TIMERA_UNIT1_CLKA_FUNC          (GPIO_FUNC_7_TIMA)

/* TIMERA CLKB Port/Pin definition */
#define TIMERA_UNIT1_CLKB_PORT          (GPIO_PORT_7)
#define TIMERA_UNIT1_CLKB_PIN           (GPIO_PIN_4)
#define TIMERA_UNIT1_CLKB_FUNC          (GPIO_FUNC_7_TIMA)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  TIMERA 1 unit overflow interrupt callback function.
 * @param  None
 * @retval None
 */
static void TimeraUnit1Overflow_IrqCallback(void)
{
    LED_R_TOGGLE();
    TIMERA_ClearFlag(TIMERA_UNIT1, TIMERA_FLAG_OVF);
}

/**
 * @brief  TIMERA 1 unit underflow interrupt callback function.
 * @param  None
 * @retval None
 */
static void TimeraUnit1Underflow_IrqCallback(void)
{
    LED_G_TOGGLE();
    TIMERA_ClearFlag(TIMERA_UNIT1, TIMERA_FLAG_UDF);
}

/**
 * @brief  Configure System clock.
 * @param  None
 * @retval None
 */
static void SystemClk_Config(void)
{
    /* Configure the system clock to HRC32MHz. */
    CLK_HRCInit(CLK_HRC_ON, CLK_HRCFREQ_32);
}

/**
 * @brief  Configure LED.
 * @param  None
 * @retval None
 */
static void Led_Config(void)
{
    stc_gpio_init_t stcGpioInit;

    /* Configure structure initialization */
    GPIO_StructInit(&stcGpioInit);

    /* LED Port/Pin initialization */
    stcGpioInit.u16PinMode = PIN_MODE_OUT;
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
    LED_R_OFF();
    LED_G_OFF();
}

/**
 * @brief  Configure TimerA function.
 * @param  None
 * @retval None
 */
static void Timera_Config(void)
{
    stc_timera_init_t stcTimeraInit;
    stc_irq_regi_config_t stcIrqRegiConf;

    /* Configuration structure initialization */
    TIMERA_StructInit(&stcTimeraInit);

    /* Configuration peripheral clock */
    CLK_FcgPeriphClockCmd(TIMERA_UNIT1_CLOCK, Enable);

    /* Configuration TIMERA capture Port */
    GPIO_SetFunc(TIMERA_UNIT1_CLKA_PORT, TIMERA_UNIT1_CLKA_PIN, TIMERA_UNIT1_CLKA_FUNC);
    GPIO_SetFunc(TIMERA_UNIT1_CLKB_PORT, TIMERA_UNIT1_CLKB_PIN, TIMERA_UNIT1_CLKB_FUNC);

    /* Configuration timera 1 unit structure */
    stcTimeraInit.u16CountMode = TIMERA_SAWTOOTH_WAVE;
    stcTimeraInit.u16PeriodVal = TIMERA_UNIT1_PERIOD_VALUE;
    stcTimeraInit.u16ClkAFilterState = TIMERA_CLKA_FILTER_ENABLE;
    stcTimeraInit.u16ClkAFilterClkDiv = TIMERA_CLKA_CLKDIV_DIV4;
    stcTimeraInit.u16ClkBFilterState = TIMERA_CLKB_FILTER_ENABLE;
    stcTimeraInit.u16ClkBFilterClkDiv = TIMERA_CLKB_CLKDIV_DIV4;
    stcTimeraInit.u16HwUpCondition = TIMERA_HWUP_CLKB_HIGH_CLKA_RISING;
    stcTimeraInit.u16HwDownCondition = TIMERA_HWDOWN_CLKB_LOW_CLKA_RISING;
    TIMERA_Init(TIMERA_UNIT1, &stcTimeraInit);
    TIMERA_IntCmd(TIMERA_UNIT1, TIMERA_INT_OVF | TIMERA_INT_UDF, Enable);

    /* Configuration timera 1 unit overflow interrupt */
    stcIrqRegiConf.enIntSrc = TIMERA_UNIT1_OVF_INTn;
    stcIrqRegiConf.enIRQn = TIMERA_UNIT1_OVF_IRQn;
    stcIrqRegiConf.pfnCallback = &TimeraUnit1Overflow_IrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Configuration timera 1 unit underflow interrupt */
    stcIrqRegiConf.enIntSrc = TIMERA_UNIT1_UDF_INTn;
    stcIrqRegiConf.enIRQn = TIMERA_UNIT1_UDF_IRQn;
    stcIrqRegiConf.pfnCallback = &TimeraUnit1Underflow_IrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Start TIMERA counter */
    TIMERA_Cmd(TIMERA_UNIT1, Enable);
}

/**
 * @brief  Main function of TIMERA phase difference.
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Configure system clock. */
    SystemClk_Config();

    /* Configure LED. */
    Led_Config();

    /* Configure TimerA */
    Timera_Config();

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
