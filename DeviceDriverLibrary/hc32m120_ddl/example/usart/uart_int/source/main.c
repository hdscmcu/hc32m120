/**
 *******************************************************************************
 * @file  usart/uart_int/source/main.c
 * @brief This example demonstrates UART data receive and transfer by interrupt.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-03       Hongjh          First version
   2019-11-04       Hongjh          TX empty interrupt IRQ callback disable TX 
                                    empyt interrupt when buffer data is empty.
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
 * @addtogroup UART_Interrupt
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @brief  Ring buffer structure definition
 */
typedef struct
{
    uint16_t u16Capacity;
    __IO uint16_t u16UsedSize;
    uint16_t u16InIdx;
    uint16_t u16OutIdx;
    uint8_t  au8Buf[500];
} stc_ring_buffer_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Red LED Port/Pin definition */
#define LED_R_PORT                      (GPIO_PORT_12)
#define LED_R_PIN                       (GPIO_PIN_0)
#define LED_R_ON()                      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))

/* UART RX/TX Port/Pin definition */
#define UART_RX_PORT                    (GPIO_PORT_1)
#define UART_RX_PIN                     (GPIO_PIN_1)      /* P11: USART2_RX */
#define UART_RX_GPIO_FUNC               (GPIO_FUNC_5_USART)

#define UART_TX_PORT                    (GPIO_PORT_1)
#define UART_TX_PIN                     (GPIO_PIN_2)      /* P12: USART2_TX */
#define UART_TX_GPIO_FUNC               (GPIO_FUNC_5_USART)

/* UART unit definition */
#define UART_UNIT                       (M0P_USART2)

/* UART unit interrupt definition */
#define UART_UNIT_ERR_INT               (INT_USART_2_EI)
#define UART_UNIT_ERR_IRQn              (Int016_IRQn)

#define UART_UNIT_RX_INT                (INT_USART_2_RI)
#define UART_UNIT_RX_IRQn               (Int018_IRQn)

#define UART_UNIT_TX_INT                (INT_USART_2_TI)
#define UART_UNIT_TX_IRQn               (Int020_IRQn)

#define UART_UNIT_TCI_INT               (INT_USART_2_TCI)
#define UART_UNIT_TCI_IRQn              (Int022_IRQn)

/* Ring buffer size */
#define RING_BUFFER_SIZE                (500u)
#define IS_RING_BUFFER_EMPYT(x)         (0u == ((x)->u16UsedSize))

/* Function clock gate definition  */
#define FUNCTION_CLK_GATE               (CLK_FCG_UART2)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void LedConfig(void);
static void SystemClockConfig(void);
static void UartRxIrqCallback(void);
static void UartTxIrqCallback(void);
static void UartTcIrqCallback(void);
static void UartErrIrqCallback(void);
static en_result_t RingBufWrite(stc_ring_buffer_t *pstcBuffer, uint8_t u8Data);
static en_result_t RingBufRead(stc_ring_buffer_t *pstcBuffer, uint8_t *pu8Data);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_ring_buffer_t m_stcRingBuf = {
    .u16InIdx = 0,
    .u16OutIdx = 0,
    .u16UsedSize = 0,
    .u16Capacity = RING_BUFFER_SIZE,
};

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
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
}

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
 * @brief  USART RX IRQ callback
 * @param  None
 * @retval None
 */
static void UartRxIrqCallback(void)
{
    uint16_t u16Data = USART_RecData(UART_UNIT);

    RingBufWrite(&m_stcRingBuf, (uint8_t)u16Data);
}

/**
 * @brief  USART transmit data register empty IRQ callback.
 * @param  None
 * @retval None
 */
static void UartTxIrqCallback(void)
{
    uint8_t u8Data = 0u;

    if (Ok == RingBufRead(&m_stcRingBuf, &u8Data))
    {
        USART_SendData(UART_UNIT, (uint16_t)u8Data);
    }

    if (IS_RING_BUFFER_EMPYT(&m_stcRingBuf))
    {
        USART_FuncCmd(UART_UNIT, USART_INT_TXE, Disable);
        USART_FuncCmd(UART_UNIT, USART_INT_TC, Enable);
    }
}

/**
 * @brief  USART transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
static void UartTcIrqCallback(void)
{
    USART_FuncCmd(UART_UNIT, (USART_TX | USART_INT_TC), Disable);
}

/**
 * @brief  USART error IRQ callback.
 * @param  None
 * @retval None
 */
static void UartErrIrqCallback(void)
{
    USART_ClearFlag(UART_UNIT, (USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_ORE));
}

/**
 * @brief  Write ring buffer.
 * @param  [in] pstcBuffer              Pointer to a @ref stc_ring_buffer_t structure
 * @param  [in] u8Data                  Data to write
 * @retval An en_result_t enumeration value:
 *           - Ok: Write success.
 *           - ErrorBufferFull: Buffer is full.
 */
static en_result_t RingBufWrite(stc_ring_buffer_t *pstcBuffer, uint8_t u8Data)
{
    en_result_t enRet = Ok;

    if (pstcBuffer->u16UsedSize >= pstcBuffer->u16Capacity)
    {
        enRet = ErrorBufferFull;
    }
    else
    {
        pstcBuffer->au8Buf[pstcBuffer->u16InIdx++] = u8Data;
        pstcBuffer->u16InIdx %= pstcBuffer->u16Capacity;
        pstcBuffer->u16UsedSize++;
    }

    return enRet;
}

/**
 * @brief  Write ring buffer.
 * @param  [in] pstcBuffer              Pointer to a @ref stc_ring_buffer_t structure
 * @param  [in] pu8Data                 Pointer to data buffer to read
 * @retval An en_result_t enumeration value:
 *           - Ok: Write success.
 *           - ErrorNotReady: Buffer is empty.
 */
static en_result_t RingBufRead(stc_ring_buffer_t *pstcBuffer, uint8_t *pu8Data)
{
    en_result_t enRet = Ok;

    if (!pstcBuffer->u16UsedSize)
    {
        enRet = ErrorNotReady;
    }
    else
    {
        *pu8Data = pstcBuffer->au8Buf[pstcBuffer->u16OutIdx++];
        pstcBuffer->u16OutIdx %= pstcBuffer->u16Capacity;
        pstcBuffer->u16UsedSize--;
    }

    return enRet;
}

/**
 * @brief  Main function of UART interrupt project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;
    const stc_uart_init_t stcUartInit = {
        .u32Baudrate = 115200ul,
        .u32BitDirection = USART_LSB,
        .u32StopBit = USART_STOP_BITS_1,
        .u32Parity = USART_PARITY_NONE,
        .u32DataWidth = USART_DATA_WIDTH_BITS_8,
        .u32ClkMode = USART_INTCLK_NONE_OUTPUT,
        .u32OversamplingBits = USART_OVERSAMPLING_BITS_8,
        .u32NoiseFilterState = USART_NOISE_FILTER_DISABLE,
        .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
    };

    /* Configure LED. */
    LedConfig();

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure USART RX/TX pin. */
    GPIO_SetFunc(UART_RX_PORT, UART_RX_PIN, UART_RX_GPIO_FUNC);
    GPIO_SetFunc(UART_TX_PORT, UART_TX_PIN, UART_TX_GPIO_FUNC);

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Initialize UART function. */
    if (Ok != USART_UartInit(UART_UNIT, &stcUartInit))
    {
        LED_R_ON();
        while(1)
        {
            ;
        }
    }

    /* Register error IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_UNIT_ERR_IRQn;
    stcIrqRegiConf.enIntSrc = UART_UNIT_ERR_INT;
    stcIrqRegiConf.pfnCallback = &UartErrIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register RX IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_UNIT_RX_IRQn;
    stcIrqRegiConf.enIntSrc = UART_UNIT_RX_INT;
    stcIrqRegiConf.pfnCallback = &UartRxIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register TX IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_UNIT_TX_IRQn;
    stcIrqRegiConf.enIntSrc = UART_UNIT_TX_INT;
    stcIrqRegiConf.pfnCallback = &UartTxIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register TC IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_UNIT_TCI_IRQn;
    stcIrqRegiConf.enIntSrc = UART_UNIT_TCI_INT;
    stcIrqRegiConf.pfnCallback = &UartTcIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Enable RX function */
    USART_FuncCmd(UART_UNIT, (USART_RX | USART_INT_RX), Enable);

    while (1)
    {
        if (!IS_RING_BUFFER_EMPYT(&m_stcRingBuf))
        {
            USART_FuncCmd(UART_UNIT, (USART_TX | USART_INT_TXE), Enable);
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
