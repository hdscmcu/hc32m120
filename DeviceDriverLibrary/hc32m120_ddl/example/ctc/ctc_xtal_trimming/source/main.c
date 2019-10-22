/**
 *******************************************************************************
 * @file  ctc/ctc_xtal_trimming/source/main.c
 * @brief This example demonstrates CTC trimming by XTAL clock.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-04       Hongjh          First version
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
#include <stdio.h>

#include "hc32_ddl.h"

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup CTC_Xtal_Trimming
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Key Port/Pin definition */
#define KEY_PORT                        (GPIO_PORT_2)
#define KEY_PIN                         (GPIO_PIN_1)

/* Red LED Port/Pin definition */
#define LED_R_PORT                      (GPIO_PORT_12)
#define LED_R_PIN                       (GPIO_PIN_0)
#define LED_R_ON()                      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))

/* Green LED Port/Pin definition */
#define LED_G_PORT                      (GPIO_PORT_7)
#define LED_G_PIN                       (GPIO_PIN_0)
#define LED_G_ON()                      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))

/* CTC interrupt number */
#define CTC_ERR_IRQn                    (Int010_IRQn)

/* CTC reference clock selection */
#define CTC_REFCLK_SOURCE               (CTC_REFCLK_XTAL)

/* CTC TRMVAL value */
#define CTC_TRMVAL_VALUE                (0x21ul)       /* -31 */

/* CTC reference clock freqency */
#define CTC_TRIMMING_REFCLK_FREQ        (20000000ul)     /* 20MHz */

/* Internal high speed RC freqency */
#define CTC_TRIMMING_HRC_FREQ           (CTC_TRIMMING_HRC_48MHZ)

/* Function clock gate definition  */
#define FUNCTION_CLK_GATE               (CLK_FCG_CTC)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void LedConfig(void);
static void CtcErrIrqCallback(void);

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
    stc_clk_xtal_init_t stcXTALInit;

    /* Configure XTAL */
    stcXTALInit.u8XtalState = CLK_XTAL_ON;
    stcXTALInit.u8XtalMode = CLK_XTALMODE_OSC;
    stcXTALInit.u8XtalDrv = CLK_XTALDRV_HIGH;
    stcXTALInit.u8XtalSupDrv = CLK_XTAL_SUPDRV_OFF;
    stcXTALInit.u8XtalStb = CLK_XTALSTB_8;
    CLK_XTALInit(&stcXTALInit);

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
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
}

/**
 * @brief  CTC error IRQ callback.
 * @param  None.
 * @retval None
 */
static void CtcErrIrqCallback(void)
{
    LED_R_ON();
}

/**
 * @brief  Main function of CTC xtal trimming project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /**
     ***************************************************************************
     @brief Modify macro-definition as below in hc32m120_icg.h file
     @verbatim
     #if ICG0_HRC_HARDWARE_START == ICG_FUNCTION_ON
         #define ICG0_HRC_REG_CONFIG                 ((uint32_t)ICG0_HRC_REQS)
     #else
         #define ICG0_HRC_REG_CONFIG                 ((uint32_t)ICG_HRC_FREQ_48MHZ)
     #endif
     @endverbatim
     ***************************************************************************
     */

    uint8_t InitTrmVal;
    uint8_t TrimmingTrmVal;
    stc_irq_regi_config_t stcIrqRegiConf;
    stc_ctc_init_t stcCtcInit = {
        .u32Fhrc = CTC_TRIMMING_HRC_FREQ,
        .u32Fref = CTC_TRIMMING_REFCLK_FREQ,
        .u32RefClkSel = CTC_REFCLK_SOURCE,
        .u32RefclkPrescaler = CTC_REFCLK_PRESCALER_DIV4096,
        .f32ToleranceBias = 0.005f,
        .u32TrmVal = CTC_TRMVAL_VALUE,
    };

    /* Confiure clock output system clock */
    CLK_MCOConfig(CLK_MCOSOURCCE_HRC, CLK_MCODIV_1);
    /* Confiure clock output pin */
    GPIO_SetFunc(GPIO_PORT_1, GPIO_PIN_5, GPIO_FUNC_1_PULBUZ);

    /* Configure system clock. */
    SystemClockConfig();

    /* Initialize UART for debug print function. */
    DDL_UartInit();

    /* Configure LED pin. */
    LedConfig();

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Wait CTC stop. */
    while (CTC_GetFlag(CTC_FLAG_BUSY))
    {
        ;
    }

    /* Initialize CTC function. */
    CTC_Init(&stcCtcInit);

    /* Register CTC error IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = CTC_ERR_IRQn;
    stcIrqRegiConf.enIntSrc = INT_CTC_ERR;
    stcIrqRegiConf.pfnCallback = &CtcErrIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    CTC_ErrIntCmd(Enable);

    /* User key : SW2 */
    while (Pin_Reset != GPIO_ReadInputPortPin(KEY_PORT, KEY_PIN))
    {
    }

    InitTrmVal = CTC_GetTrmVal();

    CTC_Cmd(Enable);

    while (!CTC_GetFlag(CTC_FLAG_TRMOK | CTC_FLAG_BUSY))
    {
        ;
    }

    TrimmingTrmVal = CTC_GetTrmVal();

    CTC_Cmd(Disable);

    /* Wait CTC stop. */
    while (CTC_GetFlag(CTC_FLAG_BUSY))
    {
        ;
    }

    printf("Triming value is 0x%02X; Triming result is 0x%02X. \r\n", InitTrmVal, TrimmingTrmVal);

    LED_G_ON();

    while(1)
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
