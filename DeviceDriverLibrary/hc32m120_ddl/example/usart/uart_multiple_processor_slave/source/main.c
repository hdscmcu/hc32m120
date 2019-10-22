/**
 *******************************************************************************
 * @file  usart/uart_multiple_processor_slave/source/main.c
 * @brief This example demonstrates UART multi-processor receive and transfer.
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

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup UART_Multiple_Processor_Slave
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
    uint8_t  au8Buf[50];
} stc_ring_buffer_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* UART RX/TX Port/Pin definition */
#define UART_RX_PORT                    (GPIO_PORT_1)
#define UART_RX_PIN                     (GPIO_PIN_4)      /* P14: USART3_RX */
#define UART_RX_GPIO_FUNC               (GPIO_FUNC_5_USART)

#define UART_TX_PORT                    (GPIO_PORT_1)
#define UART_TX_PIN                     (GPIO_PIN_3)      /* P13: USART3_TX */
#define UART_TX_GPIO_FUNC               (GPIO_FUNC_5_USART)

/* UART unit definition */
#define UART_UNIT                       (M0P_USART3)

/* UART unit interrupt definition */
#define UART_UNIT_ERR_INT               (INT_USART_3_EI)
#define UART_UNIT_ERR_IRQn              (Int012_IRQn)

#define UART_UNIT_RX_INT                (INT_USART_3_RI)
#define UART_UNIT_RX_IRQn               (Int014_IRQn)

#define UART_UNIT_TX_INT                (INT_USART_3_TI)
#define UART_UNIT_TX_IRQn               (Int016_IRQn)

#define UART_UNIT_TCI_INT               (INT_USART_3_TCI)
#define UART_UNIT_TCI_IRQn              (Int018_IRQn)

/* Function clock gate definition */
#define FUNCTION_CLK_GATE               (CLK_FCG_UART3)

/* UART multiple processor ID definition */
#define UART_MASTER_STATION_ID          (0x20u)
#define UART_SLAVE_STATION_ID           (0x21u)

/* Ring buffer size */
#define RING_BUFFER_SIZE                (50u)
#define IS_RING_BUFFER_EMPYT(x)         (0u == ((x)->u16UsedSize))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void UartTxIrqCallback(void);
static void UartTcIrqCallback(void);
static void UartRxIrqCallback(void);
static void UartRxErrIrqCallback(void);
static en_result_t RingBufWrite(stc_ring_buffer_t *pstcBuffer, uint8_t u8Data);
static en_result_t RingBufRead(stc_ring_buffer_t *pstcBuffer, uint8_t *pu8Data);
static void UsartSetSilenceModeState(uint32_t u32SilenceModeState);
static uint32_t UsartGetSilenceModeState(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint32_t m_u32UartSilenceMode;

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
 * @brief  UART TX Empty IRQ callback.
 * @param  None.
 * @retval None
 */
static void UartTxIrqCallback(void)
{
    uint8_t u8Data = 0u;
    en_flag_status_t enFlag = USART_GetFlag(UART_UNIT, USART_FLAG_TXE);
    en_functional_state_t enState = USART_GetFuncState(UART_UNIT, USART_INT_TXE);

    if ((Set == enFlag) && (Enable == enState))
    {
        USART_SendId(UART_UNIT, UART_MASTER_STATION_ID);

        while (Reset == USART_GetFlag(UART_UNIT, USART_FLAG_TXE))   /* Wait Tx data register empty */
        {
            ;
        }

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
}

/**
 * @brief  UART TX Complete IRQ callback.
 * @param  None.
 * @retval None
 */
static void UartTcIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(UART_UNIT, USART_FLAG_TC);
    en_functional_state_t enState = USART_GetFuncState(UART_UNIT, USART_INT_TC);

    if ((Set == enFlag) && (Enable == enState))
    {
        /* Disable TX function */
        USART_FuncCmd(UART_UNIT, (USART_TX | USART_RX | USART_INT_TC), Disable);

        /* Enable RX/TX function */
        USART_FuncCmd(UART_UNIT, (USART_RX | USART_INT_RX), Enable);
    }
}

/**
 * @brief  UART RX IRQ callback.
 * @param  None
 * @retval None
 */
static void UartRxIrqCallback(void)
{
    uint8_t u8RxData = 0u;
    en_flag_status_t enFlag = USART_GetFlag(UART_UNIT, USART_FLAG_RXNE);
    en_functional_state_t enState = USART_GetFuncState(UART_UNIT, USART_INT_RX);

    if ((Set == enFlag) && (Enable == enState))
    {
        u8RxData = (uint8_t)USART_RecData(UART_UNIT);

        if ((Reset == USART_GetFlag(UART_UNIT, USART_FLAG_MPB)) &&
            (USART_UART_NORMAL_MODE == UsartGetSilenceModeState()))
        {
            RingBufWrite(&m_stcRingBuf, u8RxData);
        }
        else
        {
            if (UART_SLAVE_STATION_ID != u8RxData)
            {
                USART_SilenceModeCmd(UART_UNIT, Enable);
                UsartSetSilenceModeState(USART_UART_SILENCE_MODE);
            }
            else
            {
                UsartSetSilenceModeState(USART_UART_NORMAL_MODE);
            }
        }
    }
}

/**
 * @brief  UART RX Error IRQ callback.
 * @param  None.
 * @retval None
 */
static void UartRxErrIrqCallback(void)
{
    USART_ClearFlag(UART_UNIT, (USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_ORE));
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
    en_result_t enRet = ErrorBufferFull;

    if (pstcBuffer->u16UsedSize < pstcBuffer->u16Capacity)
    {
        pstcBuffer->au8Buf[pstcBuffer->u16InIdx++] = u8Data;
        pstcBuffer->u16InIdx %= pstcBuffer->u16Capacity;
        pstcBuffer->u16UsedSize++;
        enRet = Ok;
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
    en_result_t enRet = ErrorNotReady;

    if (pstcBuffer->u16UsedSize)
    {
        *pu8Data = pstcBuffer->au8Buf[pstcBuffer->u16OutIdx++];
        pstcBuffer->u16OutIdx %= pstcBuffer->u16Capacity;
        pstcBuffer->u16UsedSize--;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set silence mode state.
 * @param  [in] u32SilenceModeState         Silence mode state
 *         This parameter can be one of the following values:
 *           @arg USART_UART_SILENCE_MODE:  UART silence mode
 *           @arg USART_UART_NORMAL_MODE:   UART normal mode
 * @retval None.
 */
static void UsartSetSilenceModeState(uint32_t u32SilenceModeState)
{
    m_u32UartSilenceMode = u32SilenceModeState;
}

/**
 * @brief  Write ring buffer.
 * @param  [in] None
 * @retval Returned value can be one of the following values:
 *           @arg USART_UART_SILENCE_MODE:  UART silence mode
 *           @arg USART_UART_NORMAL_MODE:   UART normal mode
 */
static uint32_t UsartGetSilenceModeState(void)
{
    return m_u32UartSilenceMode;
}

/**
 * @brief  Main function of UART multiple processor slave project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;
    const stc_uart_multiprocessor_init_t stcUartMultiProcessorInit = {
        .u32Baudrate = 9600ul,
        .u32BitDirection = USART_LSB,
        .u32StopBit = USART_STOP_BITS_1,
        .u32DataWidth = USART_DATA_WIDTH_BITS_8,
        .u32ClkMode = USART_INTCLK_NONE_OUTPUT,
        .u32ClkPrescaler = USART_CLK_PRESCALER_DIV4,
        .u32OversamplingBits = USART_OVERSAMPLING_BITS_8,
        .u32NoiseFilterState = USART_NOISE_FILTER_DISABLE,
        .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
    };

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure USART RX/TX pin. */
    GPIO_SetFunc(UART_RX_PORT, UART_RX_PIN, UART_RX_GPIO_FUNC);
    GPIO_SetFunc(UART_TX_PORT, UART_TX_PIN, UART_TX_GPIO_FUNC);

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Set silence mode */
    UsartSetSilenceModeState(USART_UART_SILENCE_MODE);

    /* Initialize UART function. */
    USART_MultiProcessorInit(UART_UNIT, &stcUartMultiProcessorInit);

    /* Register error IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_UNIT_ERR_IRQn;
    stcIrqRegiConf.enIntSrc = UART_UNIT_ERR_INT;
    stcIrqRegiConf.pfnCallback = &UartRxErrIrqCallback;
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
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_00);
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

    /* Enable RX/TX function */
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
