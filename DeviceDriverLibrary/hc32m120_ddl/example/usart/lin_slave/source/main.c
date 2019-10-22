/**
 *******************************************************************************
 * @file  usart/lin_slave/source/main.c
 * @brief This example demonstrates LIN data receive and transfer.
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
#include "hc32_ddl.h"
#include "lin.h"

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup LIN_Slave
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Red LED Port/Pin definition */
#define LED_R_PORT                      (GPIO_PORT_12)
#define LED_R_PIN                       (GPIO_PIN_0)
#define LED_R_ON()                      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_R_OFF()                     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))

/* USART RX/TX Port/Pin definition */
#define LIN_RX_PORT                     (GPIO_PORT_1)
#define LIN_RX_PIN                      (GPIO_PIN_6)      /* P17: USART1_RX */

#define LIN_TX_PORT                     (GPIO_PORT_1)
#define LIN_TX_PIN                      (GPIO_PIN_7)      /* P16: USART1_TX */

/* LIN transceiver chip sleep Port/Pin definition */
#define LIN_SLEEP_PORT                  (GPIO_PORT_6)
#define LIN_SLEEP_PIN                   (GPIO_PIN_2)      /* P62: LINSLP_N */

/* Interrupt number definition */
#define LIN_UNIT_RX_IRQn                (Int010_IRQn)
#define LIN_UNIT_ERR_IRQn               (Int008_IRQn)
#define LIN_TIMERB_UNIT_CMP_IRQn        (Int016_IRQn)

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
}

/**
 * @brief  Main function of LIN slave project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    en_result_t enRet;
    stc_gpio_init_t stcGpioInit = {0};
    static stc_lin_frame_t stcFrame;
    static stc_lin_hanlde_t stcLinHandle = {
        .stcLinInit = {
            .u32Baudrate = 9600ul,
            .u32ClkMode = USART_INTCLK_NONE_OUTPUT,
            .u32ClkPrescaler = USART_CLK_PRESCALER_DIV4,
            .u32OversamplingBits = USART_OVERSAMPLING_BITS_8,
            .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
        },
        .stcPinCfg = {
            .u8RxPort = LIN_RX_PORT,
            .u8RxPin = LIN_RX_PIN,
            .u8TxPort = LIN_TX_PORT,
            .u8TxPin = LIN_TX_PIN,
        },
        .stcIrqnCfg = {
            .UsartRxIRQn = LIN_UNIT_RX_IRQn,
            .UsartErrIRQn = LIN_UNIT_ERR_IRQn,
            .TimerbCmpIRQn = LIN_TIMERB_UNIT_CMP_IRQn,
        },
        .enLinState = LinStateSleep,
    };

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure LED. */
    LedConfig();

    /* Configure LIN transceiver chip sleep pin. */
    stcGpioInit.u16PinMode = PIN_MODE_OUT;
    stcGpioInit.u16PinState = PIN_STATE_SET;
    GPIO_Init(LIN_SLEEP_PORT, LIN_SLEEP_PIN, &stcGpioInit);

    /* Initialize LIN slave function. */
    LIN_SLAVE_Init(&stcLinHandle);

    while (1)
    {
        /* Wait wakeup. */
        while (LinStateSleep == LIN_GetState(&stcLinHandle))
        {
            ;
        }

        /* Start LIN slave receive frame header function(blocking mode). */
        enRet = LIN_SLAVE_RecFrameHeader(&stcLinHandle, &stcFrame , LIN_REC_WAITING_FOREVER);
        if (Ok != enRet)
        {
            LED_R_ON();
        }
        else
        {
            LED_R_OFF();
        }

        /* Start LIN slave receive frame data function. */
        enRet = LIN_SLAVE_RecFrameResponse(&stcLinHandle, &stcFrame, 200);
        if (Ok != enRet)
        {
            LED_R_ON();
        }
        else
        {
            LED_R_OFF();
        }

        /* Start LIN slave receive frame header function(blocking mode). */
        enRet = LIN_SLAVE_RecFrameHeader(&stcLinHandle, &stcFrame , LIN_REC_WAITING_FOREVER);
        if (Ok != enRet)
        {
            LED_R_ON();
        }
        else
        {
            LED_R_OFF();
        }

        /* LIN slave send frame response. */
        enRet = LIN_SLAVE_SendFrameResponse(&stcLinHandle, &stcFrame);
        if (Ok != enRet)
        {
            LED_R_ON();
        }
        else
        {
            LED_R_OFF();
        }

        /* Enter sleep. */
        LIN_Sleep(&stcLinHandle);
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
