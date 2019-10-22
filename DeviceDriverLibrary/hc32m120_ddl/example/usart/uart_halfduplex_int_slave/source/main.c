/**
 *******************************************************************************
 * @file  usart/uart_halfduplex_int_slave/source/main.c
 * @brief This example demonstrates UART half-duplex data receive and transfer
 *        by interrupt mode.
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
 * @addtogroup UART_Halfduplex_Interrupt_Slave
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* UART TX Port/Pin definition */
#define UART_SLAVE_TX_PORT              (GPIO_PORT_1)
#define UART_SLAVE_TX_PIN               (GPIO_PIN_3)      /* P13: USART3_TX */
#define UART_SLAVE_TX_GPIO_FUNC         (GPIO_FUNC_5_USART)

/* UART unit definition */
#define UART_SLAVE_UNIT                 (M0P_USART3)

/* UART unit interrupt definition */
#define UART_SLAVE_UNIT_ERR_INT         (INT_USART_3_EI)
#define UART_SLAVE_UNIT_ERR_IRQn        (Int012_IRQn)

#define UART_SLAVE_UNIT_RX_INT          (INT_USART_3_RI)
#define UART_SLAVE_UNIT_RX_IRQn         (Int014_IRQn)

#define UART_SLAVE_UNIT_TX_INT          (INT_USART_3_TI)
#define UART_SLAVE_UNIT_TX_IRQn         (Int016_IRQn)

#define UART_SLAVE_UNIT_TCI_INT         (INT_USART_3_TCI)
#define UART_SLAVE_UNIT_TCI_IRQn        (Int018_IRQn)

/* Function clock gate definition  */
#define FUNCTION_CLK_GATE               (CLK_FCG_UART3)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void UartSlaveUnitTxIrqCallback(void);
static void UartSlaveUnitTcIrqCallback(void);
static void UartSlaveUnitRxIrqCallback(void);
static void UartSlaveUnitErrIrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t m_u8UartSlaveRxData = 0u;

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
 * @brief  UART slave unit TX Complete IRQ callback.
 * @param  None.
 * @retval None
 */
static void UartSlaveUnitTxIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(UART_SLAVE_UNIT, USART_FLAG_TXE);
    en_functional_state_t enState = USART_GetFuncState(UART_SLAVE_UNIT, USART_IT_TXE);

    if ((Set == enFlag) && (Enable == enState))
    {
        USART_SendData(UART_SLAVE_UNIT, (uint16_t)m_u8UartSlaveRxData);

        /* Disable slave TX & Enable TC interrupt function */
        USART_FuncCmd(UART_SLAVE_UNIT, USART_INT_TXE, Disable);
        USART_FuncCmd(UART_SLAVE_UNIT, USART_INT_TC, Enable);
    }
}

/**
 * @brief  UART slave unit TX Complete IRQ callback.
 * @param  None.
 * @retval None
 */
static void UartSlaveUnitTcIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(UART_SLAVE_UNIT, USART_FLAG_TC);
    en_functional_state_t enState = USART_GetFuncState(UART_SLAVE_UNIT, USART_IT_TC);

    if ((Set == enFlag) && (Enable == enState))
    {
        /* Enable RX & RX no empty interrupt function && Disable TX & TC interrupt function*/
        USART_FuncCmd(UART_SLAVE_UNIT, (USART_TX | USART_INT_TC), Disable);
        USART_FuncCmd(UART_SLAVE_UNIT, (USART_RX | USART_INT_RX), Enable);
    }
}

/**
 * @brief  UART slave unit RX IRQ callback.
 * @param  None
 * @retval None
 */
static void UartSlaveUnitRxIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(UART_SLAVE_UNIT, USART_FLAG_RXNE);
    en_functional_state_t enState = USART_GetFuncState(UART_SLAVE_UNIT, USART_IT_RI);

    if ((Set == enFlag) && (Enable == enState))
    {
        m_u8UartSlaveRxData = (uint8_t)USART_RecData(UART_SLAVE_UNIT);

        /* Disable slave RX & RX no empty interrupt function && Enable TX & TC interrupt function*/
        USART_FuncCmd(UART_SLAVE_UNIT, (USART_RX | USART_INT_RX), Disable);
        USART_FuncCmd(UART_SLAVE_UNIT, (USART_TX | USART_INT_TXE), Enable);
    }
}

/**
 * @brief  UART slave RX error IRQ callback.
 * @param  None
 * @retval None
 */
static void UartSlaveUnitErrIrqCallback(void)
{
    if (Set == USART_GetFlag(UART_SLAVE_UNIT, (USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_ORE)))
    {
        USART_ClearFlag(UART_SLAVE_UNIT, (USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_ORE));
    }
}

/**
 * @brief  Main function of UART halfduplex interrupt slave project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;
    const stc_uart_init_t stcUartInit = {
        .u32Baudrate = 19200,
        .u32BitDirection = USART_LSB,
        .u32StopBit = USART_STOP_BITS_1,
        .u32Parity = USART_PARITY_NONE,
        .u32DataWidth = USART_DATA_WIDTH_BITS_8,
        .u32ClkMode = USART_INTCLK_NONE_OUTPUT,
        .u32OversamplingBits = USART_OVERSAMPLING_BITS_8,
        .u32NoiseFilterState = USART_NOISE_FILTER_DISABLE,
        .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
    };

    /* Configure system clock. */
    SystemClockConfig();

    /* Configure USART TX pin. */
    GPIO_SetFunc(UART_SLAVE_TX_PORT, UART_SLAVE_TX_PIN, UART_SLAVE_TX_GPIO_FUNC);

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Initialize UART slave half-duplex function. */
    USART_HalfDuplexInit(UART_SLAVE_UNIT, &stcUartInit);

    /* Register RX IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_SLAVE_UNIT_RX_IRQn;
    stcIrqRegiConf.enIntSrc = UART_SLAVE_UNIT_RX_INT;
    stcIrqRegiConf.pfnCallback = &UartSlaveUnitRxIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register RX error IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_SLAVE_UNIT_ERR_IRQn;
    stcIrqRegiConf.enIntSrc = UART_SLAVE_UNIT_ERR_INT;
    stcIrqRegiConf.pfnCallback = &UartSlaveUnitErrIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register TX IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_SLAVE_UNIT_TX_IRQn;
    stcIrqRegiConf.enIntSrc = UART_SLAVE_UNIT_TX_INT;
    stcIrqRegiConf.pfnCallback = &UartSlaveUnitTxIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register TC IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_SLAVE_UNIT_TCI_IRQn;
    stcIrqRegiConf.enIntSrc = UART_SLAVE_UNIT_TCI_INT;
    stcIrqRegiConf.pfnCallback = &UartSlaveUnitTcIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Enable RX function && RX interrupt function */
    USART_FuncCmd(UART_SLAVE_UNIT, (USART_RX | USART_INT_RX), Enable);

    while (1)
    {
        ;
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
