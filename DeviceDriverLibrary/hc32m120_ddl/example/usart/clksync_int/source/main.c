/**
 *******************************************************************************
 * @file  usart/clksync_int/source/main.c
 * @brief This example demonstrates CLKSYNC data receive and transfer by interrupt.
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
 * @addtogroup CLKSYNC_Interrupt
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @brief USART buffer Structure definition
 */
typedef struct
{
    uint8_t         *pu8TxBuffPtr;      /*!< Pointer to USART Tx transfer Buffer */

    uint16_t        u16TxXferSize;      /*!< USART Tx Transfer size              */

    __IO uint16_t   u16TxXferCount;     /*!< USART Tx Transfer Counter           */

    uint8_t         *pu8RxBuffPtr;      /*!< Pointer to Usart Rx transfer Buffer */

    uint16_t        u16RxXferSize;      /*!< USART Rx Transfer size              */

    __IO uint16_t   u16RxXferCount;     /*!< USART Rx Transfer Counter           */

} stc_buffer_handle_t;

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
#define CLKSYNC_CK_PIN                  (GPIO_PIN_0)        /* P10: USART2_CK */
#define CLKSYNC_CK_GPIO_FUNC            (GPIO_FUNC_5_USART)

#define CLKSYNC_RX_PORT                 (GPIO_PORT_1)
#define CLKSYNC_RX_PIN                  (GPIO_PIN_1)        /* P11: USART2_RX */
#define CLKSYNC_RX_GPIO_FUNC            (GPIO_FUNC_5_USART)

#define CLKSYNC_TX_PORT                 (GPIO_PORT_1)
#define CLKSYNC_TX_PIN                  (GPIO_PIN_2)        /* P12: USART2_TX */
#define CLKSYNC_TX_GPIO_FUNC            (GPIO_FUNC_5_USART)

/* UART unit definition */
#define CLKSYNC_UNIT                    (M0P_USART2)

/* UART unit interrupt definition */
#define USART_UNIT_ERR_INT              (INT_USART_2_EI)
#define USART_UNIT_ERR_IRQn             (Int016_IRQn)

#define USART_UNIT_RX_INT               (INT_USART_2_RI)
#define USART_UNIT_RX_IRQn              (Int018_IRQn)

#define USART_UNIT_TX_INT               (INT_USART_2_TI)
#define USART_UNIT_TX_IRQn              (Int020_IRQn)

#define USART_UNIT_TCI_INT              (INT_USART_2_TCI)
#define USART_UNIT_TCI_IRQn             (Int022_IRQn)

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
static void UsartTxIrqCallback(void);
static void UsartTcIrqCallback(void);
static void UsartRxIrqCallback(void);
static void UsartRxErrIrqCallback(void);
static void TransmitReceive_IT(M0P_USART_TypeDef *USARTx,
                               stc_buffer_handle_t *pstcBufHandle);
static en_result_t CLKSYNC_TransmitReceive_IT(M0P_USART_TypeDef *USARTx,
                                             stc_buffer_handle_t *pstcBufHandle,
                                             uint8_t *pu8TxData,
                                             uint8_t *pu8RxData,
                                             uint16_t u16Size);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_buffer_handle_t m_stcBufHandle;

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
 * @brief  USART TX Empty IRQ callback.
 * @param  None.
 * @retval None
 */
static void UsartTxIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(CLKSYNC_UNIT, USART_FLAG_TXE);
    en_functional_state_t enState = USART_GetFuncState(CLKSYNC_UNIT, USART_IT_TXE);

    if ((Set == enFlag) && (Enable == enState))
    {
        TransmitReceive_IT(CLKSYNC_UNIT, &m_stcBufHandle);
    }
}

/**
 * @brief  USART TX Complete IRQ callback.
 * @param  None.
 * @retval None
 */
static void UsartTcIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(CLKSYNC_UNIT, USART_FLAG_TC);
    en_functional_state_t enState = USART_GetFuncState(CLKSYNC_UNIT, USART_IT_TC);

    if ((Set == enFlag) && (Enable == enState))
    {
        TransmitReceive_IT(CLKSYNC_UNIT, &m_stcBufHandle);
    }
}

/**
 * @brief  USART RX IRQ callback.
 * @param  None
 * @retval None
 */
static void UsartRxIrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(CLKSYNC_UNIT, USART_FLAG_RXNE);
    en_functional_state_t enState = USART_GetFuncState(CLKSYNC_UNIT, USART_IT_RI);

    if ((Set == enFlag) && (Enable == enState))
    {
        TransmitReceive_IT(CLKSYNC_UNIT, &m_stcBufHandle);
    }
}

/**
 * @brief  USART RX Error IRQ callback.
 * @param  None.
 * @retval None
 */
static void UsartRxErrIrqCallback(void)
{
    USART_ClearFlag(CLKSYNC_UNIT, (USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_ORE));
}

/**
 * @brief  Send receive an amount of data in full-duplex mode (non-blocking) in IRQ handler.
 * @param  [in] USARTx                  pointer to a USART instance.
 * @param  [in] pstcBufHandle           pointer to a stc_buffer_handle_t structure.
 * @retval None
 */
static void TransmitReceive_IT(M0P_USART_TypeDef *USARTx,
                               stc_buffer_handle_t *pstcBufHandle)
{
    if (pstcBufHandle->u16RxXferCount != 0u)
    {
        if (USART_GetFlag(USARTx, USART_FLAG_RXNE) != Reset)
        {
            *pstcBufHandle->pu8RxBuffPtr++ = (uint8_t)USART_RecData(USARTx);
            pstcBufHandle->u16RxXferCount--;
        }
    }

    /* Check the latest data received */
    if (pstcBufHandle->u16RxXferCount == 0u)
    {
        /* Disable the USART RXNE && Error Interrupt */
        USART_FuncCmd(USARTx, USART_INT_RX, Disable);
    }
    else
    {
        if (pstcBufHandle->u16TxXferCount != 0u)
        {
            if (USART_GetFlag(USARTx, USART_FLAG_TXE) != Reset)
            {
                USART_SendData(USARTx, (uint16_t)(*pstcBufHandle->pu8TxBuffPtr++));
                pstcBufHandle->u16TxXferCount--;

                /* Check the latest data transmitted */
                if (pstcBufHandle->u16TxXferCount == 0u)
                {
                    USART_FuncCmd(USARTx, (USART_INT_TXE | USART_INT_TC), Disable);
                }
            }
        }
    }
}

/**
 * @brief  Send&&receive an amount of data in full-duplex mode (non-blocking).
 * @param  [in] USARTx                  pointer to a USART instance.
 * @param  [in] pstcBufHandle           pointer to a stc_buffer_handle_t structure.
 * @param  [in] pu8TxData               Pointer to data transmitted buffer
 * @param  [out] pu8RxData              Pointer to data received buffer
 * @param  [in] u16Size                 Amount of data to be received
 * @retval An en_result_t enumeration value:
 *           - Ok: success
 *           - ErrorInvalidParameter: Invalid parameter
 */
static en_result_t CLKSYNC_TransmitReceive_IT(M0P_USART_TypeDef *USARTx,
                                             stc_buffer_handle_t *pstcBufHandle,
                                             uint8_t *pu8TxData,
                                             uint8_t *pu8RxData,
                                             uint16_t u16Size)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32Func = USART_RX | USART_INT_RX | USART_TX;

    if ((USARTx) && (pu8TxData) && (pu8RxData) && (u16Size))
    {
        pstcBufHandle->pu8RxBuffPtr = pu8RxData;
        pstcBufHandle->u16RxXferSize = u16Size;
        pstcBufHandle->u16RxXferCount = u16Size;
        pstcBufHandle->pu8TxBuffPtr = pu8TxData;
        pstcBufHandle->u16TxXferSize = u16Size;
        pstcBufHandle->u16TxXferCount = u16Size;

        USART_FuncCmd(USARTx, (u32Func | USART_INT_TXE | USART_INT_TC), Disable);

        if (USART_EXTCLK == USART_GetClockMode(USARTx))
        {
            USART_FuncCmd(USARTx, (u32Func | USART_INT_TXE), Enable);
        }
        else
        {
            USART_FuncCmd(USARTx, (u32Func | USART_INT_TC), Enable);
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Main function of CLKSYNC interrupt project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint16_t u16TxXferCount = 0u;
    uint16_t u16RxXferCount = 0u;
    stc_irq_regi_config_t stcIrqRegiConf;
    stc_clksync_init_t stcClksyncInit;
    /* Buffer used for transmission */
    char au8TxData[] = "CLKSYNC TX/RX example: Communication between two boards using usart interface!";
    /* Buffer used for reception */
    uint8_t au8RxData[ARRAY_SZ(au8TxData)];

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

    /* Register error IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = USART_UNIT_ERR_IRQn;
    stcIrqRegiConf.enIntSrc = USART_UNIT_ERR_INT;
    stcIrqRegiConf.pfnCallback = &UsartRxErrIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register RX IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = USART_UNIT_RX_IRQn;
    stcIrqRegiConf.enIntSrc = USART_UNIT_RX_INT;
    stcIrqRegiConf.pfnCallback = &UsartRxIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_00);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register TX IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = USART_UNIT_TX_IRQn;
    stcIrqRegiConf.enIntSrc = USART_UNIT_TX_INT;
    stcIrqRegiConf.pfnCallback = &UsartTxIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Register TC IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = USART_UNIT_TCI_IRQn;
    stcIrqRegiConf.enIntSrc = USART_UNIT_TCI_INT;
    stcIrqRegiConf.pfnCallback = &UsartTcIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Enable RX/TX function */
    USART_FuncCmd(CLKSYNC_UNIT, (USART_RX | USART_TX), Enable);

    /* User key : SW2 */
    while (Pin_Reset != GPIO_ReadInputPortPin(KEY_PORT, KEY_PIN))
    {
        ;
    }

    /* Start the transmission process*/
    CLKSYNC_TransmitReceive_IT(CLKSYNC_UNIT, &m_stcBufHandle, (uint8_t *)au8TxData, au8RxData, (uint16_t)ARRAY_SZ(au8TxData));

    /* Wait tranmission complete */
    do
    {
        u16TxXferCount = m_stcBufHandle.u16TxXferCount;
        u16RxXferCount = m_stcBufHandle.u16RxXferCount;
    } while ((u16TxXferCount != 0u) || (u16RxXferCount != 0u));

    /* Compare m_u8TxBuffer and m_u8RxBuffer data */
    if (memcmp(au8TxData , au8RxData, (uint32_t)m_stcBufHandle.u16RxXferSize) == 0)
    {
        LED_G_ON();
    }
    else
    {
        LED_R_ON();
    }

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
