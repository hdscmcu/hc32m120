/**
 *******************************************************************************
 * @file  timer0/basetimer/source/main.c
 * @brief Main program of TIMER0 for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-24       Heqb         First version
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
 * @addtogroup Basetimer
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* Define for RGB LED */

#define LED_R_PORT      (GPIO_PORT_12)
#define LED_B_PORT      (GPIO_PORT_3)
#define LED_G_PORT      (GPIO_PORT_7)
#define LED_R_PIN       (GPIO_PIN_0)
#define LED_B_PIN       (GPIO_PIN_1)
#define LED_G_PIN       (GPIO_PIN_0)
#define LED_G_TOGGLE()  (GPIO_TogglePins(LED_G_PORT, LED_G_PIN))

/* TIMER0 interrupt source and number define */
#define TIMER0_IRQn     (Int014_IRQn)
#define TIMER0_SOURCE   (INT_TMR0_GCMP)

#define LRC_FRQ         (32768u)

#define SW2_PORT        (GPIO_PORT_2)
#define SW2_PIN         (GPIO_PIN_1)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void LedConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  TIMER0 compare IRQ callback
 * @param  None
 * @retval None
 */
static void Timer0CompareIrqCallback(void)
{
    LED_G_TOGGLE();
    TIMER0_ClearFlag();
}

/**
 * @brief  Main function of example project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_regi_config_t stcIrqRegiCfg;
    stc_exint_config_t  stcExintCfg;
    stc_tim0_init_t stcTimer0Ini;
    stc_gpio_init_t stcGpioIni;

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure RGB LED. */
    LedConfig();

    /* LRC ON */
    CLK_LRCInit(CLK_LRC_ON);

    /* Config EXINT (INT1 SW2)function for hardware trigger */
    GPIO_StructInit(&stcGpioIni);
    stcGpioIni.u16ExInt = PIN_EXINT_ON;
    GPIO_Init(SW2_PORT, SW2_PIN, &stcGpioIni);
    EXINT_StructInit(&stcExintCfg);
    stcExintCfg.u16ExIntCh = EXINT_CH01;
    stcExintCfg.u8ExIntFE = EXINT_FILTER_ON;
    stcExintCfg.u8ExIntFClk = EXINT_FCLK_HCLK_DIV32;
    stcExintCfg.u8ExIntLvl = EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintCfg);

    /* Enable AOS function */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);

    /* Enable timer0 peripheral clock */
    CLK_FcgPeriphClockCmd(CLK_FCG_TIM0, Enable);

    /* TIMER0 basetimer function initialize */
    TIMER0_StructInit(&stcTimer0Ini);
    stcTimer0Ini.u32ClockDivision = TIMER0_CLK_DIV256;/* Config clock division */
    stcTimer0Ini.u32ClockSource = TIMER0_CLK_SRC_LRC; /* Chose clock source */
    stcTimer0Ini.u32Tmr0Fun = TIMER0_FUNC_CMP;        /* Timer0 compare mode */
    stcTimer0Ini.u32HwTrigFunc = TIMER0_BT_HWTRG_FUNC_STOP; /* Config Hardware trigger function */
    stcTimer0Ini.u16CmpValue =  LRC_FRQ / 2u / 256u;     /* Set compara register data */
    TIMER0_Init(&stcTimer0Ini);

    /* Set internal hardware capture source */
    TIMER0_SetTriggerSrc(EVT_PORT_EIRQ1);

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegiCfg.enIRQn = TIMER0_IRQn;
    stcIrqRegiCfg.enIntSrc = TIMER0_SOURCE;
    stcIrqRegiCfg.pfnCallback = &Timer0CompareIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiCfg);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /* Timer0 interrupt function enable */
    TIMER0_IntCmd(Enable);
    /* Timer function kick start */
    TIMER0_Cmd(Enable);

    while(1)
    {
      
    };
}

/**
 * @brief  Configure system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_xtal_init_t stcXTALInit;

    /* Configure XTAL */
    stcXTALInit.u8XtalState = CLK_XTAL_ON;
    stcXTALInit.u8XtalMode = CLK_XTALMODE_OSC;
    stcXTALInit.u8XtalDrv = CLK_XTALDRV_HIGH;
    stcXTALInit.u8XtalSupDrv = CLK_XTAL_SUPDRV_OFF;
    stcXTALInit.u8XtalStb = CLK_XTALSTB_8;

    /* Initialize XTAL clock */
    CLK_XTALInit(&stcXTALInit);

    /* Set clock division */
    CLK_SetSysclkDiv(CLK_SYSCLK_DIV32);
    /* Switch system clock from HRC(default) to XTAL */
    CLK_SetSysclkSrc(CLK_SYSCLKSOURCE_XTAL);
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
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
