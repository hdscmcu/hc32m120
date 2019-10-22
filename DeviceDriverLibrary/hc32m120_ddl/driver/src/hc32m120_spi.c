/**
 *******************************************************************************
 * @file  hc32m120_spi.c
 * @brief This file provides firmware functions to manage the Serial Peripheral
 *        Interface(SPI).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-03       Wuze            First version
   2019-10-17       Wuze            Delete the interval between consecutively sent data.
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
#include "hc32m120_spi.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_SPI SPI
 * @brief Serial Peripheral Interface Driver Library
 * @{
 */

#if (DDL_SPI_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SPI_Local_Macros SPI Local Macros
 * @{
 */

/**
 * @defgroup SPI_Check_Parameters_Validity SPI check parameters validity
 * @{
 */
#define IS_SPI_WIRE_MODE(x)                                                    \
(   ((x) == SPI_WIRE_4)                     ||                                 \
    ((x) == SPI_WIRE_3))

#define IS_SPI_TRANS_MODE(x)                                                   \
(   ((x) == SPI_FULL_DUPLEX)                ||                                 \
    ((x) == SPI_SEND_ONLY))

#define IS_SPI_MASTER_SLAVE(x)                                                 \
(   ((x) == SPI_SLAVE)                      ||                                 \
    ((x) == SPI_MASTER))

#define IS_SPI_SPLPBK(x)                                                       \
(   ((x) == SPI_SPLPBK_INVALID)             ||                                 \
    ((x) == SPI_SPLPBK_MOSI_NOT)            ||                                 \
    ((x) == SPI_SPLPBK_MOSI))

#define IS_SPI_MODFE_CMD(x)                                                    \
(   ((x) == SPI_MODFE_DISABLE)              ||                                 \
    ((x) == SPI_MODFE_ENABLE))

#define IS_SPI_PATE_CMD(x)                                                     \
(   ((x) == SPI_PATE_DISABLE)               ||                                 \
    ((x) == SPI_PATE_ENABLE))

#define IS_SPI_PARITY_CHECK(x)                                                 \
(   ((x) == SPI_PARITY_INVALID)             ||                                 \
    ((x) == SPI_PARITY_EVEN)                ||                                 \
    ((x) == SPI_PARITY_ODD))

#define IS_SPI_NSS_ACTIVE_LEVEL(x)                                             \
(   ((x) == SPI_NSS_ACTIVE_LOW)             ||                                 \
    ((x) == SPI_NSS_ACTIVE_HIGH))

#define IS_SPI_SPI_MODE(x)                                                     \
(   ((x) == SPI_MODE_0)                     ||                                 \
    ((x) == SPI_MODE_1)                     ||                                 \
    ((x) == SPI_MODE_2)                     ||                                 \
    ((x) == SPI_MODE_3))

#define IS_SPI_BIT_RATE_DIV(x)                                                 \
(   ((x) == SPI_BR_DIV_2)                   ||                                 \
    ((x) == SPI_BR_DIV_4)                   ||                                 \
    ((x) == SPI_BR_DIV_8)                   ||                                 \
    ((x) == SPI_BR_DIV_16)                  ||                                 \
    ((x) == SPI_BR_DIV_32)                  ||                                 \
    ((x) == SPI_BR_DIV_64)                  ||                                 \
    ((x) == SPI_BR_DIV_128)                 ||                                 \
    ((x) == SPI_BR_DIV_256))

#define IS_SPI_DATA_SIZE(x)                                                    \
(   ((x) == SPI_DATA_SIZE_8BIT)             ||                                 \
    ((x) == SPI_DATA_SIZE_16BIT))

#define IS_SPI_FIRST_BIT(x)                                                    \
(   ((x) == SPI_FIRST_MSB)                  ||                                 \
    ((x) == SPI_FIRST_LSB))

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @defgroup SPI_Local_Functions SPI Local Functions
 * @{
 */
static en_result_t SPI_TxRx(const void *pvTxBuf, void *pvRxBuf, uint32_t u32Length);
static en_result_t SPI_Tx(const void *pvTxBuf, uint32_t u32Length);

/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup SPI_Local_Variables SPI Local Variables
 * @{
 */
static uint32_t m_u32Timeout = 0u;

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup SPI_Global_Functions SPI Global Functions
 * @{
 */

/**
 * @brief  Initializes the SPI peripheral according to the specified parameters
 *         in the structure stc_spi_init.
 * @param  [in]  pstcInit           Pointer to a stc_spi_init_t structure that contains
 *                                  the configuration information for the SPI.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorInvalidParameter:   pstcInit == NULL
 */
en_result_t SPI_Init(const stc_spi_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32Div;

    if (pstcInit != NULL)
    {
        DDL_ASSERT(IS_SPI_WIRE_MODE(pstcInit->u32WireMode));
        DDL_ASSERT(IS_SPI_TRANS_MODE(pstcInit->u32TransMode));
        DDL_ASSERT(IS_SPI_MASTER_SLAVE(pstcInit->u32MasterSlave));
        DDL_ASSERT(IS_SPI_SPLPBK(pstcInit->u32Splpbk));
        DDL_ASSERT(IS_SPI_MODFE_CMD(pstcInit->u32Modfe));
        DDL_ASSERT(IS_SPI_PATE_CMD(pstcInit->u32Pate));
        DDL_ASSERT(IS_SPI_PARITY_CHECK(pstcInit->u32Parity));
        DDL_ASSERT(IS_SPI_NSS_ACTIVE_LEVEL(pstcInit->u32NssActiveLevel));
        DDL_ASSERT(IS_SPI_SPI_MODE(pstcInit->u32SpiMode));
        DDL_ASSERT(IS_SPI_BIT_RATE_DIV(pstcInit->u32BaudRatePrescaler));
        DDL_ASSERT(IS_SPI_DATA_SIZE(pstcInit->u32DataSize));
        DDL_ASSERT(IS_SPI_FIRST_BIT(pstcInit->u32FirstBit));

        u32Div = pstcInit->u32BaudRatePrescaler >> SPI_CFG2_MBR_POS;
        m_u32Timeout  = (2ul << u32Div) * ((uint32_t)M0P_CMU->SCKDIVR) * 16ul;

        M0P_SPI->CR1  = pstcInit->u32WireMode          |   \
                        pstcInit->u32TransMode         |   \
                        pstcInit->u32MasterSlave       |   \
                        pstcInit->u32Splpbk            |   \
                        pstcInit->u32Modfe             |   \
                        pstcInit->u32Pate              |   \
                        pstcInit->u32Parity;

        M0P_SPI->CFG1 = pstcInit->u32NssActiveLevel;

        M0P_SPI->CFG2 = pstcInit->u32SpiMode           |   \
                        pstcInit->u32BaudRatePrescaler |   \
                        pstcInit->u32DataSize          |   \
                        pstcInit->u32FirstBit;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-initializes the SPI peripheral.
 * @param  None
 * @retval None
 */
void SPI_DeInit(void)
{
    SPI_FunctionCmd(Disable);

    M0P_SPI->CR1   = 0x0ul;
    M0P_SPI->CFG1  = 0x0ul;
    M0P_SPI->CFG2  = 0x0ul;
    M0P_SPI->SR    = 0x0ul;
}

/**
 * @brief  Set a default value for the SPI initialization structure.
 * @param  [in]  pstcInit           Pointer to a stc_spi_init_t structure that
 *                                  contains configuration information.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcInit == NULL.
 */
en_result_t SPI_StructInit(stc_spi_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcInit != NULL)
    {
        pstcInit->u32WireMode          = SPI_WIRE_4;
        pstcInit->u32TransMode         = SPI_FULL_DUPLEX;
        pstcInit->u32MasterSlave       = SPI_MASTER;
        pstcInit->u32Splpbk            = SPI_SPLPBK_INVALID;
        pstcInit->u32Modfe             = SPI_MODFE_DISABLE;
        pstcInit->u32Pate              = SPI_PATE_DISABLE;
        pstcInit->u32Parity            = SPI_PARITY_INVALID;
        pstcInit->u32NssActiveLevel    = SPI_NSS_ACTIVE_LOW;
        pstcInit->u32SpiMode           = SPI_MODE_0;
        pstcInit->u32BaudRatePrescaler = SPI_BR_DIV_8;
        pstcInit->u32DataSize          = SPI_DATA_SIZE_8BIT;
        pstcInit->u32FirstBit          = SPI_FIRST_MSB;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Enable or disable SPI interrupt.
 * @param  [in]  u32IntType         SPI interrupt type.
 *                                  This parameter can be values of @ref SPI_Interrupt_Type
 *   @arg  SPI_INT_ERROR
 *   @arg  SPI_INT_TX_BUFFER_EMPTY
 *   @arg  SPI_INT_RX_BUFFER_FULL
 *   @arg  SPI_INT_IDLE
 *   @arg  SPI_INT_ALL
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable the specified interrupt of SPI.
 *   @arg  Disable:                 Disable the specified interrupt of SPI.
 * @retval None
 */
void SPI_IntCmd(uint32_t u32IntType, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    u32IntType &= SPI_INT_ALL;

    if (enNewState == Enable)
    {
        M0P_SPI->CR1 |= u32IntType;
    }
    else
    {
        M0P_SPI->CR1 &= ~u32IntType;
    }
}

/**
 * @brief  SPI transmit data.
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 * @param  [in]  u32TxLength        The length of the data to be sent.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorTimeout:            SPI transmit timeout.
 *   @arg  ErrorInvalidParameter:   pvTxBuf == NULL or u32TxLength == 0u
 * @note   -No NSS pin active and inactive operation in 3-wire mode. Add operations of NSS pin depending on your application.
 *         -This function supports full duplex mode and send only mode.
 */
en_result_t SPI_Transmit(const void *pvTxBuf, uint32_t u32TxLength)
{
    uint32_t u32Flags;
    en_result_t enRet = ErrorInvalidParameter;

    if ((pvTxBuf != NULL) && (u32TxLength != 0u))
    {
        u32Flags = M0P_SPI->CR1 & SPI_SEND_ONLY;
        if (u32Flags == SPI_FULL_DUPLEX)
        {
            /* Transmit data in full duplex mode. */
            enRet = SPI_TxRx(pvTxBuf, NULL, u32TxLength);
        }
        else
        {
            /* Transmit data in send only mode. */
            enRet = SPI_Tx(pvTxBuf, u32TxLength);
        }
    }

    return enRet;
}

/**
 * @brief  SPI receive data.
 * @param  [in]  pvRxBuf            The pointer to the buffer which the received data to be stored.
 * @param  [in]  u32RxLength        The length of the data to be received.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorTimeout:            SPI receive timeout.
 *   @arg  ErrorInvalidParameter:   pvRxBuf == NULL or u32RxLength == 0u
 * @note   -No NSS pin active and inactive operation in 3-wire mode. Add operations of NSS pin depending on your application.
 *         -This function only works in full duplex master mode.
 */
en_result_t SPI_Receive(void *pvRxBuf, uint32_t u32RxLength)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((pvRxBuf != NULL) && (u32RxLength != 0u))
    {
        /* Receives data in full duplex master mode. */
        enRet = SPI_TxRx(NULL, pvRxBuf, u32RxLength);
    }

    return enRet;
}

/**
 * @brief  SPI transmit and receive data.
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 *                                  If this pointer is NULL and the pvRxBuf is NOT NULL, the MOSI output high
 *                                  and the the received data will be stored in the buffer pointed by pvRxBuf.
 * @param  [out] pvRxBuf            The pointer to the buffer which the received data will be stored.
 *                                  This for full duplex transfer.
 * @param  [in]  u32Length          The length of the data(in byte or half word) to be sent and received.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorTimeout:            SPI transmit and receive timeout.
 *   @arg  ErrorInvalidParameter:   pvRxBuf == NULL or pvRxBuf == NULL or u32Length == 0u
 * @note   SPI receives data while sending data. Only works in full duplex master mode.
 */
en_result_t SPI_TransmitReceive(const void *pvTxBuf, void *pvRxBuf, uint32_t u32Length)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((pvTxBuf != NULL) && (pvRxBuf != NULL) && (u32Length != 0u))
    {
        /* Transmit and receive data in full duplex master mode. */
        enRet = SPI_TxRx(pvTxBuf, pvRxBuf, u32Length);
    }

    return enRet;
}

/**
 * @}
 */

/**
 * @addtogroup SPI_Local_Functions SPI Local Functions
 * @{
 */
/**
 * @brief  SPI transmit and receive data in full duplex mode.
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 * @param  [out] pvRxBuf            The pointer to the buffer which the received data will be stored.
 * @param  [in]  u32Length          The length of the data in byte or half word.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorTimeout:            SPI transmit and receive timeout.
 */
static en_result_t SPI_TxRx(const void *pvTxBuf, void *pvRxBuf, uint32_t u32Length)
{
    uint32_t u32BitSize;
    uint32_t u32Timecount;
    uint32_t u32Count = 0u;
    en_result_t enRet = Ok;

    u32BitSize = M0P_SPI->CFG2 & SPI_DATA_SIZE_16BIT;

    while (u32Count < u32Length)
    {
        if (pvTxBuf != NULL)
        {
            if (u32BitSize)
            {
                M0P_SPI->DR = ((const uint16_t *)pvTxBuf)[u32Count];
            }
            else
            {
                M0P_SPI->DR = ((const uint8_t *)pvTxBuf)[u32Count];
            }
        }
        else
        {
            M0P_SPI->DR = (uint16_t)0xFFFF;
        }

        u32Timecount = m_u32Timeout;
        do
        {
            if (M0P_SPI->SR & SPI_FLAG_RX_BUFFER_FULL)
            {
                break;
            }
            u32Timecount--;
        } while (u32Timecount != 0u);

        if (pvRxBuf != NULL)
        {
            if (u32BitSize)
            {
                ((uint16_t *)pvRxBuf)[u32Count] = (uint16_t)M0P_SPI->DR;
            }
            else
            {
                ((uint8_t *)pvRxBuf)[u32Count] = (uint8_t)M0P_SPI->DR;
            }
        }

        if (u32Timecount == 0u)
        {
            enRet = ErrorTimeout;
            break;
        }

        u32Count++;
    }

    return enRet;
}

/**
 * @brief  SPI send data only.
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 * @param  [in]  u32Length          The length of the data in byte or half word.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorTimeout:            SPI transmit timeout.
 */
static en_result_t SPI_Tx(const void *pvTxBuf, uint32_t u32Length)
{
    uint32_t u32Count = 0u;
    uint32_t u32Timecount;
    uint32_t u32BitSize;
    en_result_t enRet = Ok;

    u32BitSize = M0P_SPI->CFG2 & SPI_DATA_SIZE_16BIT;

    while (u32Count < u32Length)
    {
        if (u32BitSize)
        {
            M0P_SPI->DR = ((const uint16_t *)pvTxBuf)[u32Count];
        }
        else
        {
            M0P_SPI->DR = ((const uint8_t *)pvTxBuf)[u32Count];
        }

        u32Timecount = m_u32Timeout;
        do
        {
            if (M0P_SPI->SR & SPI_FLAG_TX_BUFFER_EMPTY)
            {
                break;
            }
            u32Timecount--;
        } while (u32Timecount != 0u);
        
        if (u32Timecount == 0u)
        {
            enRet = ErrorTimeout;
        }

        u32Count++;
    }

    return enRet;
}

/**
 * @}
 */

#endif /* DDL_SPI_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
