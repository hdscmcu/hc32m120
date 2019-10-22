/**
 *******************************************************************************
 * @file  usart/uart_halfduplex_int_master/source/main.c
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
 * @addtogroup UART_Halfduplex_Interrupt_Master
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @brief Key state definition
 */
typedef enum
{
    KeyIdle,
    KeyRelease,
} en_key_state_t;

/**
 * @brief Key instance structure definition
 */
typedef struct
{
    uint8_t u8Port;                     /*!< GPIO_PORT_x: x can be (0~7, 12~14) to select the GPIO peripheral */

    uint8_t u8Pin;                      /*!< GPIO_PIN_x: x can be (0~7) to select the PIN index */

    en_pin_state_t enPressPinState;     /*!< Pin level state when key is pressed */
} stc_key_t;

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
#define LED_R_OFF()                     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))

/* Green LED Port/Pin definition */
#define LED_G_PORT                      (GPIO_PORT_7)
#define LED_G_PIN                       (GPIO_PIN_0)
#define LED_G_TOGGLE()                  (GPIO_TogglePins(LED_G_PORT, LED_G_PIN))
#define LED_G_OFF()                     (GPIO_SetPins(LED_G_PORT, LED_G_PIN))

/* UART TX Port/Pin definition */
#define UART_MASTER_TX_PORT             (GPIO_PORT_1)
#define UART_MASTER_TX_PIN              (GPIO_PIN_3)      /* P13: USART3_TX */
#define UART_MASTER_TX_GPIO_FUNC        (GPIO_FUNC_5_USART)

/* UART unit definition */
#define UART_MASTER_UNIT                (M0P_USART3)

/* UART unit interrupt definition */
#define UART_MASTER_UNIT_ERR_INT        (INT_USART_3_EI)
#define UART_MASTER_UNIT_ERR_IRQn       (Int012_IRQn)

#define UART_MASTER_UNIT_RX_INT         (INT_USART_3_RI)
#define UART_MASTER_UNIT_RX_IRQn        (Int014_IRQn)

#define UART_MASTER_UNIT_TX_INT         (INT_USART_3_TI)
#define UART_MASTER_UNIT_TX_IRQn        (Int016_IRQn)

#define UART_MASTER_UNIT_TCI_INT        (INT_USART_3_TCI)
#define UART_MASTER_UNIT_TCI_IRQn       (Int018_IRQn)

/* Function clock gate definition  */
#define FUNCTION_CLK_GATE               (CLK_FCG_UART3)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void LedConfig(void);
static en_key_state_t KeyGetState(const stc_key_t *pstcKey);
static void UartMasterUnitTxIrqCallback(void);
static void UartMasterUnitTcIrqCallback(void);
static void UartMasterUnitRxIrqCallback(void);
static void UartMasterUnitErrIrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t m_u8UartMasterTxData;
static uint8_t m_u8UartMasterRxData;
static __IO uint8_t m_u8UartMasterRxFlag;

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
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
}

/**
 * @brief  Get key state.
 * @param  [in] pstcKey    Pointer to stc_key_t structure
 * @retval An en_result_t enumeration value:
 *           - KeyIdle: Key isn't pressed.
 *           - KeyRelease: Released after key is pressed.
 */
static en_key_state_t KeyGetState(const stc_key_t *pstcKey)
{
    en_key_state_t enKeyState = KeyIdle;

    if (NULL != pstcKey)
    {
        if (pstcKey->enPressPinState == GPIO_ReadInputPortPin(pstcKey->u8Port, pstcKey->u8Pin))
        {
            DDL_Delay1ms(20ul);

            if (pstcKey->enPressPinState == GPIO_ReadInputPortPin(pstcKey->u8Port, pstcKey->u8Pin))
            {
                while (pstcKey->enPressPinState == GPIO_ReadInputPortPin(pstcKey->u8Port, pstcKey->u8Pin))
                {
                    ;
                }
                enKeyState = KeyRelease;
            }
        }
    }

    return enKeyState;
}

/**
 * @brief  UART master unit TX IRQ callback.
 * @param  None
 * @retval None
 */
static void UartMasterUnitTxIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(UART_MASTER_UNIT, USART_FLAG_TXE);
    en_functional_state_t enState = USART_GetFuncState(UART_MASTER_UNIT, USART_IT_TXE);

    if ((Set == enFlag) && (Enable == enState))
    {
        USART_SendData(UART_MASTER_UNIT, (uint16_t)m_u8UartMasterTxData);

        /* Enable master RX & RX no empty interrupt function && Disable master TX & TC interrupt function*/
        USART_FuncCmd(UART_MASTER_UNIT, USART_INT_TXE, Disable);
        USART_FuncCmd(UART_MASTER_UNIT, USART_INT_TC, Enable);
    }
}

/**
 * @brief  UART master unit TX Complete IRQ callback.
 * @param  None
 * @retval None
 */
static void UartMasterUnitTcIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(UART_MASTER_UNIT, USART_FLAG_TC);
    en_functional_state_t enState = USART_GetFuncState(UART_MASTER_UNIT, USART_IT_TC);

    if ((Set == enFlag) && (Enable == enState))
    {
        /* Enable master RX & RX no empty interrupt function && Disable master TX & TC interrupt function*/
        USART_FuncCmd(UART_MASTER_UNIT, (USART_RX | USART_INT_RX), Enable);
        USART_FuncCmd(UART_MASTER_UNIT, (USART_TX | USART_INT_TC), Disable);
    }
}

/**
 * @brief  UART master unit RX IRQ callback.
 * @param  None
 * @retval None
 */
static void UartMasterUnitRxIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(UART_MASTER_UNIT, USART_FLAG_RXNE);
    en_functional_state_t enState = USART_GetFuncState(UART_MASTER_UNIT, USART_IT_RI);

    if ((Set == enFlag) && (Enable == enState))
    {
        m_u8UartMasterRxData = (uint8_t)USART_RecData(UART_MASTER_UNIT);
        m_u8UartMasterRxFlag = Set;

        /* Disable master RX & RX no empty interrupt function */
        USART_FuncCmd(UART_MASTER_UNIT, (USART_RX | USART_INT_RX), Disable);
    }
}

/**
 * @brief  UART master RX error IRQ callback.
 * @param  None
 * @retval None
 */
static void UartMasterUnitErrIrqCallback(void)
{
    if (Set == USART_GetFlag(UART_MASTER_UNIT, (USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_ORE)))
    {
        USART_ClearFlag(UART_MASTER_UNIT, (USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_ORE));
    }
}

/**
 * @brief  Main function of UART halfduplex interrupt master project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;
    stc_key_t stcKeySw2 = {
        .u8Port = KEY_PORT,
        .u8Pin = KEY_PIN,
        .enPressPinState = Pin_Reset,
    };
    const stc_uart_init_t stcUartInit = {
        .u32Baudrate = 19200ul,
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

    /* Initialize UART for debug print function. */
    DDL_UartInit();

    /* Configure LED. */
    LedConfig();

    /* Configure USART TX pin. */
    GPIO_SetFunc(UART_MASTER_TX_PORT, UART_MASTER_TX_PIN, UART_MASTER_TX_GPIO_FUNC);

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Initialize UART master half-duplex function. */
    USART_HalfDuplexInit(UART_MASTER_UNIT, &stcUartInit);

    /* Register RX IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_MASTER_UNIT_RX_IRQn;
    stcIrqRegiConf.enIntSrc = UART_MASTER_UNIT_RX_INT;
    stcIrqRegiConf.pfnCallback = &UartMasterUnitRxIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register RX error IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_MASTER_UNIT_ERR_IRQn;
    stcIrqRegiConf.enIntSrc = UART_MASTER_UNIT_ERR_INT;
    stcIrqRegiConf.pfnCallback = &UartMasterUnitErrIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register TX IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_MASTER_UNIT_TX_IRQn;
    stcIrqRegiConf.enIntSrc = UART_MASTER_UNIT_TX_INT;
    stcIrqRegiConf.pfnCallback = &UartMasterUnitTxIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register TC IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = UART_MASTER_UNIT_TCI_IRQn;
    stcIrqRegiConf.enIntSrc = UART_MASTER_UNIT_TCI_INT;
    stcIrqRegiConf.pfnCallback = &UartMasterUnitTcIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    while (1)
    {
        /* Wait key release */
        while (KeyRelease !=  KeyGetState(&stcKeySw2))
        {
            ;
        }

        /* Master unit send data */
        USART_FuncCmd(UART_MASTER_UNIT, (USART_TX | USART_INT_TXE), Enable);

        /* Master unit receive data */
        while (Reset == m_u8UartMasterRxFlag)    /* Wait Master unit Rx data */
        {
            ;
        }

        if (m_u8UartMasterRxData == m_u8UartMasterTxData)
        {
            LED_G_TOGGLE();
            LED_R_OFF();
        }
        else
        {
            LED_G_OFF();
            LED_R_ON();
        }

        printf("Master send:%d; master receive:%d \r\n", m_u8UartMasterTxData, m_u8UartMasterRxData);

        m_u8UartMasterTxData++;
        m_u8UartMasterRxFlag = Reset;
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
