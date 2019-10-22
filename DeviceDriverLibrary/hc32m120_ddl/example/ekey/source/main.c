/**
 *******************************************************************************
 * @file  ekey/source/main.c
 * @brief Main program EKEY for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-26       Zhangxl          First version
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
 * @addtogroup EKEY
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define EKEY3_PORT      (GPIO_PORT_2)
#define EKEY3_PIN       (GPIO_PIN_1)
#define EKEY4_PORT      (GPIO_PORT_2)
#define EKEY4_PIN       (GPIO_PIN_2)

#define LED_R_PORT      (GPIO_PORT_12)
#define LED_G_PORT      (GPIO_PORT_7)
#define LED_B_PORT      (GPIO_PORT_3)

#define LED_R_PIN       (GPIO_PIN_0)
#define LED_G_PIN       (GPIO_PIN_0)
#define LED_B_PIN       (GPIO_PIN_1)

#define LED_R_ON()      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_ON()      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))
#define LED_B_ON()      (GPIO_ResetPins(LED_B_PORT, LED_B_PIN))

#define LED_R_OFF()     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_OFF()     (GPIO_SetPins(LED_G_PORT, LED_G_PIN))
#define LED_B_OFF()     (GPIO_SetPins(LED_B_PORT, LED_B_PIN))

#define LED_R_TOGGLE()  (GPIO_TogglePins(LED_R_PORT, LED_R_PIN))
#define LED_G_TOGGLE()  (GPIO_TogglePins(LED_G_PORT, LED_G_PIN))
#define LED_B_TOGGLE()  (GPIO_TogglePins(LED_B_PORT, LED_B_PIN))
#define LED_RGB_TOGGLE()                                                        \
                        {LED_R_TOGGLE();LED_G_TOGGLE();LED_B_TOGGLE();}

#define LED_RGB_ON()    {LED_R_ON();LED_G_ON();LED_B_ON();}
#define LED_RGB_OFF()   {LED_R_OFF();LED_G_OFF();LED_B_OFF();}

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
 * @brief  EKEY IRQ callback function
 * @param  None
 * @retval None
 */
static void EKEY_IrqCallback(void)
{
    if (Pin_Reset == GPIO_ReadInputPortPin(EKEY3_PORT, EKEY3_PIN))
    {
        LED_R_TOGGLE();
        /* wait EKEY3 release */
        while(Pin_Reset == GPIO_ReadInputPortPin(EKEY3_PORT, EKEY3_PIN))
        {
            ;
        }
    }
    if (Pin_Reset == GPIO_ReadInputPortPin(EKEY4_PORT, EKEY4_PIN))
    {
        LED_G_TOGGLE();
        /* wait EKEY4 release */
        while(Pin_Reset == GPIO_ReadInputPortPin(EKEY4_PORT, EKEY4_PIN))
        {
            ;
        }
    }
}

/**
 * @brief  Main function of EKEY project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint8_t u8Ret;
    stc_irq_regi_config_t stcIrqRegister;
    stc_gpio_init_t stcGpioInit;

    /* RGB LED initialize */
    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
    GPIO_Init(LED_B_PORT, LED_B_PIN, &stcGpioInit);

    /* "Turn off" LED before set to output */
    LED_RGB_OFF();

    /* Output enable */
    GPIO_OE(LED_R_PORT, LED_R_PIN, Enable);
    GPIO_OE(LED_G_PORT, LED_G_PIN, Enable);
    GPIO_OE(LED_B_PORT, LED_B_PIN, Enable);

    /* Set P21/P22 to EKEY3/EKEY4 on START-KIT */
    GPIO_SetFunc(EKEY3_PORT, EKEY3_PIN, GPIO_FUNC_6_KR);
    GPIO_SetFunc(EKEY4_PORT, EKEY4_PIN, GPIO_FUNC_6_KR);

    /* Set IRQ handler 14 as the EKEY interrupt entry */
    stcIrqRegister.enIRQn       = Int014_IRQn;
    stcIrqRegister.enIntSrc     = INT_PORT_EKEY;
    stcIrqRegister.pfnCallback  = &EKEY_IrqCallback;
    u8Ret = INTC_IrqRegistration(&stcIrqRegister);
    if (Ok != u8Ret)
    {
        // check parameter
        while(1)
        {
            ;
        }
    }
    /* Enable NVIC  */
    NVIC_ClearPendingIRQ(Int014_IRQn);
    NVIC_SetPriority(Int014_IRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(Int014_IRQn);

    /* Enable EKEY3/4 */
    INTC_EKeyCmd((INTC_EKEY3 | INTC_EKEY4), Enable);

    while (1)
    {
        ;// wait EKEY pressed
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
