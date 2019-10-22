/**
 *******************************************************************************
 * @file  icg/icg_lvd_interrupt_hw_startup/source/main.c
 * @brief Main program of ICG LVD Interrupt for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-25       Yangjp          First version
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
 * @addtogroup ICG_LVD_Interrupt
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
 * @brief  LVD interrupt callback function.
 * @param  None
 * @retval None
 */
static void LVD_IrqCallback(void)
{
    if (Set == PWC_GetLvdFlag(PWC_LVD_FLAG_DET))
    {
        PWC_ClearLvdDetFlag();
        if (Set == PWC_GetLvdFlag(PWC_LVD_FLAG_LVI))
        {
            LED_G_OFF();
            LED_R_ON();
        }
        else
        {
            LED_G_ON();
            LED_R_OFF();
        }
    }
}

/**
 * @brief  LVD configuration.
 * @param  None
 * @retval None
 */
static void LVD_Config(void)
{
    uint8_t u8Ret;
    stc_irq_regi_config_t stcIrqRegister;

    /* NVIC configure of LVD */
    stcIrqRegister.enIntSrc = INT_PVD_DET;
    stcIrqRegister.enIRQn = Int022_IRQn;
    stcIrqRegister.pfnCallback = &LVD_IrqCallback;
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
}

/**
 * @brief  Main function of ICG LVD Interrupt.
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /**
     ***************************************************************************
     @brief Modify hc32m120_icg.h file of define
     @verbatim
     #define ICG1_LVD_HARDWARE_START      ICG_FUNCTION_ON

     #define ICG1_LVD_DFS                 ICG_LVD_FILTER_CLOCK_LRC16
     #define ICG1_LVD_DFDIS               ICG_LVD_DIGITAL_FILTER_ENABLE
     #define ICG1_LVD_LVDLVL              ICG_LVD_BELOW2P96_OR_ABOVE3P02
     #define ICG1_LVD_NMIS                ICG_LVD_INT_TYPE_INTR
     #define ICG1_LVD_IRS                 ICG_LVD_TRIG_EVENT_INT
     #define ICG1_LVD_IRDIS               ICG_LVD_INT_AND_RESET_ENABLE
     #define ICG1_LVD_LVDDIS              ICG_LVD_VOLTAGE_DETECTION_ENABLE
     @endverbatim
    ***************************************************************************
    */
    stc_gpio_init_t stcGpioInit;

    /* Configure structure initialization */
    GPIO_StructInit(&stcGpioInit);

    /* LED Port/Pin initialization */
    stcGpioInit.u16PinMode = PIN_MODE_OUT;
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
    LED_R_OFF();
    LED_G_ON();

    /* LVD configuration */
    LVD_Config();

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
