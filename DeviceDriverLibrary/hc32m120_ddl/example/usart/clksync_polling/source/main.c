/**
 *******************************************************************************
 * @file  usart/clksync_polling/source/main.c
 * @brief This example demonstrates CLKSYNC data receive and transfer by polling.
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
#include <string.h>
#include "hc32_ddl.h"

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup CLKSYNC_Polling
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
#define KEY_PIN                         (GPIO_PIN_2)

/* Red LED Port/Pin definition */
#define LED_R_PORT                      (GPIO_PORT_12)
#define LED_R_PIN                       (GPIO_PIN_0)
#define LED_R_ON()                      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))

/* Green LED Port/Pin definition */
#define LED_G_PORT                      (GPIO_PORT_7)
#define LED_G_PIN                       (GPIO_PIN_0)
#define LED_G_ON()                      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))

/* CLKSYNC CK/RX/TX Port/Pin definition */
#define CLKSYNC_CK_PORT                 (GPIO_PORT_1)
#define CLKSYNC_CK_PIN                  (GPIO_PIN_0)      /* P10: USART2_CK */
#define CLKSYNC_CK_GPIO_FUNC            (GPIO_FUNC_5_USART)

#define CLKSYNC_RX_PORT                 (GPIO_PORT_1)
#define CLKSYNC_RX_PIN                  (GPIO_PIN_1)      /* P11: USART2_RX */
#define CLKSYNC_RX_GPIO_FUNC            (GPIO_FUNC_5_USART)

#define CLKSYNC_TX_PORT                 (GPIO_PORT_1)
#define CLKSYNC_TX_PIN                  (GPIO_PIN_2)      /* P12: USART2_TX */
#define CLKSYNC_TX_GPIO_FUNC            (GPIO_FUNC_5_USART)

/* USART unit definition */
#define CLKSYNC_UNIT                    (M0P_USART2)

/* Function clock gate definition  */
#define FUNCTION_CLK_GATE               (CLK_FCG_UART2)

/* Timeout max definition  */
#define TIMEOUT_MAX                     (0xFFFFFFFFul)

/* CLKSYNC device mode definition */
#define CLKSYNC_MASTER_MODE             (0u)
#define CLKSYNC_SLAVE_MODE              (1u)

/* USART master or slave mode selection */
#define CLKSYNC_DEVICE_MODE             (CLKSYNC_MASTER_MODE)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void LedConfig(void);
static void SystemClockConfig(void);
static en_result_t USART_WaitOnFlagUntilTimeout(M0P_USART_TypeDef *USARTx,
                                                uint32_t u32Flag,
                                                en_flag_status_t enStatus,
                                                uint32_t u32TickStart,
                                                uint32_t u32Timeout);
static en_result_t CLKSYNC_TransmitReceive(M0P_USART_TypeDef *USARTx,
                                           const uint8_t au8TxData[],
                                           uint8_t au8RxData[],
                                           uint32_t u32Size,
                                           uint32_t u32Timeout);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

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
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
}

/**
 * @brief  Configure system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_xtal_init_t stcXTALInit;

    /* Initialize XTAL clock */
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
 * @brief  This function handles USART Communication Timeout.
 * @param  [in] USARTx                  pointer to a USART instance.
 * @param  [in] u32Flag                 specifies the USART flag to check.
 * @param  [in] enStatus                The new Flag status (Set or Reset).
 * @param  [in] u32TickStart            Tick start value.
 * @param  [in] u32Timeout              Timeout duration.
 * @retval An en_result_t enumeration value:
 *           - Ok: success
 *           - ErrorTimeout: timeout
 */
static en_result_t USART_WaitOnFlagUntilTimeout(M0P_USART_TypeDef *USARTx,
                                                uint32_t u32Flag,
                                                en_flag_status_t enStatus,
                                                uint32_t u32TickStart,
                                                uint32_t u32Timeout)
{
    en_result_t enRet = Ok;

    /* Wait until flag is set */
    while((USART_GetFlag(USARTx, u32Flag) ? Set : Reset) == enStatus)
    {
        /* Check for the Timeout */
        if (TIMEOUT_MAX - u32Timeout)
        {
            if (((SysTick_GetTick() - u32TickStart) > u32Timeout) || (!u32Timeout))
            {
                enRet = ErrorTimeout;
                break;
            }
        }
    }

    return enRet;
}

/**
 * @brief  Send && receive an amount of data in full-duplex mode (blocking mode).
 * @param  [in] USARTx                  Pointer to a USART instance.
 * @param  [in] au8TxData               Data transmitted buffer
 * @param  [out] au8RxData              Data received buffer
 * @param  [in] u32Size                 Amount of data to be sent
 * @param  [in] u32Timeout              Timeout duration
 * @retval An en_result_t enumeration value:
 *           - Ok: success
 *           - ErrorInvalidParameter: USARTx/pu8TxData/pu8RxData/u16Size is invalid
 */
static en_result_t CLKSYNC_TransmitReceive(M0P_USART_TypeDef *USARTx,
                                           const uint8_t au8TxData[],
                                           uint8_t au8RxData[],
                                           uint32_t u32Size,
                                           uint32_t u32Timeout)
{
    uint32_t i = 0ul;
    uint32_t u32Flag;
    uint32_t u32TickStart = 0ul;
    uint32_t u32XferCount = u32Size;
    en_result_t enRet = ErrorInvalidParameter;

    if((USARTx) && (au8TxData) && (au8RxData) && (u32Size))
    {
        enRet = Ok;

        /* Init tickstart for timeout managment */
        u32TickStart = SysTick_GetTick();

        /* Check the remain data to be received */
        while (u32XferCount > 0ul)
        {
            u32XferCount--;

            /* Wait for TX empty or TX complete flag in order to write data in DR */
            u32Flag = (USART_INTCLK_OUTPUT == USART_GetClockMode(USARTx)) ? USART_FLAG_TC : USART_FLAG_TXE;
            if (USART_WaitOnFlagUntilTimeout(USARTx, u32Flag, Reset, u32TickStart, u32Timeout) != Ok)
            {
                enRet = ErrorTimeout;
            }
            else
            {
                USART_SendData(USARTx, (uint16_t)(au8TxData[i]));

                /* Wait for RXNE Flag */
                if (USART_WaitOnFlagUntilTimeout(USARTx, USART_FLAG_RXNE, Reset, u32TickStart, u32Timeout) != Ok)
                {
                    enRet = ErrorTimeout;
                }
                else
                {
                    /* Receive data */
                    au8RxData[i] = (uint8_t)USART_RecData(USARTx);
                }
                i++;
            }

            if (enRet != Ok)
            {
                break;
            }
        }
    }

    return enRet;
}

/**
 * @brief  Main function of CLKSYNC polling project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_clksync_init_t stcClksyncInit;
    /* Buffer used for transmission */
    char au8TxBuffer[] = "CLKSYNC TX/RX example: Communication between two boards using usart interface!";
    /* Buffer used for reception */
    uint8_t au8RxBuffer[(ARRAY_SZ(au8TxBuffer))];

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure system tick. */
    SysTick_Init(100ul);

    /* Configure LED pin. */
    LedConfig();

    /* Configure USART RX/TX pin. */
    GPIO_SetFunc(CLKSYNC_CK_PORT, CLKSYNC_CK_PIN, CLKSYNC_CK_GPIO_FUNC);
    GPIO_SetFunc(CLKSYNC_RX_PORT, CLKSYNC_RX_PIN, CLKSYNC_CK_GPIO_FUNC);
    GPIO_SetFunc(CLKSYNC_TX_PORT, CLKSYNC_TX_PIN, CLKSYNC_CK_GPIO_FUNC);

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Initialize CLKSYNC function. */
    USART_ClkSyncStructInit(&stcClksyncInit);
    stcClksyncInit.u32Baudrate = 38400ul;
#if (CLKSYNC_DEVICE_MODE == CLKSYNC_MASTER_MODE)
    stcClksyncInit.u32ClkMode = USART_INTCLK_OUTPUT;
#else
    stcClksyncInit.u32ClkMode = USART_EXTCLK;
#endif
    USART_ClkSyncInit(CLKSYNC_UNIT, &stcClksyncInit);

    /* Enable RX/TX function */
    USART_FuncCmd(CLKSYNC_UNIT, (USART_RX | USART_TX), Enable);

    /* User key : SW2 */
    while (Pin_Reset != GPIO_ReadInputPortPin(KEY_PORT, KEY_PIN))
    {
    }

    /* Start the transmission process*/
    CLKSYNC_TransmitReceive(CLKSYNC_UNIT, (uint8_t *)au8TxBuffer, au8RxBuffer, (ARRAY_SZ(au8TxBuffer)), TIMEOUT_MAX);

    /* Compare m_u8TxBuffer and m_u8RxBuffer data */
    if (memcmp(au8TxBuffer, au8RxBuffer, (ARRAY_SZ(au8TxBuffer))) == 0)
    {
        LED_G_ON();
    }
    else
    {
        LED_R_ON();
    }

    while (1)
    {
    }
}

/**
 * @brief  systick callback function
 * @param  [in]  None
 * @retval None
 *
 */
void SysTick_IrqHandler(void)
{
    SysTick_IncTick();
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
