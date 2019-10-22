/**
 *******************************************************************************
 * @file  swdt/swdt_interrupt_sw_startup/source/main.c
 * @brief Main program of SWDT Interrupt for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-27       Yangjp          First version
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
 * @addtogroup SWDT_Interrupt
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

/* SW1 Port/Pin definition */
#define SW1_PORT                        (GPIO_PORT_2)
#define SW1_PIN                         (GPIO_PIN_2)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t u8ExIntCnt = 0u;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  SWDT interrupt callback function.
 * @param  None
 * @retval None
 */
static void SWDT_IrqCallback(void)
{
    en_flag_status_t enFlagSta;

    enFlagSta = SWDT_GetFlag(SWDT_FLAG_COUNT_UNDERFLOW);
    /* SWDT underflow interrupt */
    if (Set == enFlagSta)
    {
        SWDT_ClearFlag(SWDT_FLAG_COUNT_UNDERFLOW);
        /* Normal mode */
        if (0u == u8ExIntCnt)
        {
            LED_R_TOGGLE();
        }
        /* Sleep mode */
        else if (1u == u8ExIntCnt)
        {
            LED_G_TOGGLE();
        }
        /* Stop mode */
        else
        {
            LED_R_TOGGLE();
            LED_G_TOGGLE();
        }
    }
    SWDT_ReloadCounter();
}

/**
 * @brief  SW1 interrupt callback function.
 * @param  None
 * @retval None
 */
void EXINT02_Handler(void)
{
    if (Set == EXINT_GetExIntSrc(EXINT_CH02))
    {
        u8ExIntCnt++;
        if (u8ExIntCnt >= 3u)
        {
            u8ExIntCnt = 0u;
        }
        LED_R_OFF();
        LED_G_OFF();
        EXINT_ClrExIntSrc(EXINT_CH02);
    }
}

/**
 * @brief  SW1 configuration.
 * @param  None
 * @retval None
 */
static void SW1_Config(void)
{
    stc_gpio_init_t stcGpioInit;
    stc_exint_config_t stcExIntInit;

    /* configure structure initialization */
    GPIO_StructInit(&stcGpioInit);
    EXINT_StructInit(&stcExIntInit);

    /* External interrupt Ch.2 initialize */
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    GPIO_Init(SW1_PORT, SW1_PIN, &stcGpioInit);

    /* EXINT Channel 2 (SW1) configure */
    stcExIntInit.u16ExIntCh = (uint16_t)EXINT_CH02;
    stcExIntInit.u8ExIntFE = EXINT_FILTER_OFF;
    stcExIntInit.u8ExIntLvl = EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExIntInit);

    /* Clear pending */
    NVIC_ClearPendingIRQ(EXINT02_IRQn);
    /* Set priority */
    NVIC_SetPriority(EXINT02_IRQn, DDL_IRQ_PRIORITY_DEFAULT);
    /* Enable NVIC */
    NVIC_EnableIRQ(EXINT02_IRQn);

    /* Enable stop mode wakeup */
    INTC_WakeupSrcCmd(INTC_WUPENR_EIRQWUEN_2, Enable);
}

/**
 * @brief  SWDT configuration.
 * @param  None
 * @retval None
 */
static void SWDT_Config(void)
{
    uint8_t u8Ret;
    stc_swdt_init_t stcSwdtInit;
    stc_irq_regi_config_t stcIrqRegister;

    /* SWDT structure parameters configure */
    stcSwdtInit.u32CountCycle = SWDT_COUNTER_CYCLE_256;
    stcSwdtInit.u32ClockDivision = SWDT_CLOCK_DIV128;
    stcSwdtInit.u32RefreshRange = SWDT_RANGE_0TO100PCT;
    stcSwdtInit.u32LPModeCountEn = SWDT_LPW_MODE_COUNT_CONTINUE;
    stcSwdtInit.u32RequestType = SWDT_TRIG_EVENT_INT;
    SWDT_Init(&stcSwdtInit);

    /* NVIC configure of SWDT */
    stcIrqRegister.enIntSrc = INT_SWDT_NMIUNDF;
    stcIrqRegister.enIRQn = Int008_IRQn;
    stcIrqRegister.pfnCallback = &SWDT_IrqCallback;
    u8Ret = INTC_IrqRegistration(&stcIrqRegister);
    if (Ok != u8Ret)
    {
        /* check parameter */
        while (1)
        {
        }
    }

    /* Clear pending */
    NVIC_ClearPendingIRQ(stcIrqRegister.enIRQn);
    /* Set priority */
    NVIC_SetPriority(stcIrqRegister.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegister.enIRQn);

    /* Enable stop mode wakeup */
    INTC_WakeupSrcCmd(INTC_WUPENR_SWDTWUEN, Enable);
}

/**
 * @brief  Main function of SWDT Interrupt.
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
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

    /* SWDT configuration */
    SWDT_Config();
    /* SW1 configuration */
    SW1_Config();
    /* First reload counter to start SWDT */
    SWDT_ReloadCounter();

    while (1)
    {
        /* Sleep mode */
        if (1u == u8ExIntCnt)
        {
            PWC_EnterSleepMode();
        }
        /* Stop mode */
        else if (2u == u8ExIntCnt)
        {
            PWC_EnterStopMode();
        }
        else
        {
            /* Reserved */
        }
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
