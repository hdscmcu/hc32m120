/**
 *******************************************************************************
 * @file  hc32m120_spi.h
 * @brief This file contains all the functions prototypes of the SPI driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-03       Wuze            First version
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
#ifndef __HC32M120_SPI_H__
#define __HC32M120_SPI_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_SPI
 * @{
 */

#if (DDL_SPI_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup SPI_Global_Types SPI Global Types
 * @{
 */

/**
 * @brief Structure definition of SPI initialization.
 */
typedef struct
{
    uint32_t u32WireMode;           /*!< SPI wire mode, 3 wire mode or 4 wire mode.
                                         This parameter can be a value of @ref SPI_Wire_Mode */
    uint32_t u32TransMode;          /*!< SPI transfer mode, send only or full duplex.
                                         This parameter can be a value of @ref SPI_Transfer_Mode */
    uint32_t u32MasterSlave;        /*!< SPI master/slave mode.
                                         This parameter can be a value of @ref SPI_Master_Slave_Mode */
    uint32_t u32Splpbk;             /*!< SPI loopback mode selection, for self test.
                                         This parameter can be a value of @ref SPI_Loopback_Selection */
    uint32_t u32Modfe;              /*!< SPI mode fault dectet command.
                                         This parameter can be a value of @ref SPI_Mode_Fault_Dectet_Command */
    uint32_t u32Pate;               /*!< SPI self diagnosis of parity check error command.
                                         This parameter can be a value of @ref SPI_Parity_Check_Error_Self_Diagnosis */
    uint32_t u32Parity;             /*!< SPI parity check selection.
                                         This parameter can be a value of @ref SPI_Parity_Check */
    uint32_t u32NssActiveLevel;     /*!< SPI NSS pin active level, active low or active high.
                                         This parameter can be a value of @ref SPI_NSS_Active_Level */
    uint32_t u32SpiMode;            /*!< SPI mode.
                                         This parameter can be a value of @ref SPI_Mode */
    uint32_t u32BaudRatePrescaler;  /*!< SPI baud rate prescaler.
                                         This parameter can be a value of @ref SPI_Baud_Rate_Prescaler */
    uint32_t u32DataSize;           /*!< SPI data size, 8 bit or 16 bit.
                                         This parameter can be a value of @ref SPI_Data_Size */
    uint32_t u32FirstBit;           /*!< MSB first or LSB first.
                                         This parameter can be a value of @ref SPI_First_Bit */
} stc_spi_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SPI_Global_Macros SPI Global Macros
 * @{
 */

/**
 * @defgroup SPI_Wire_Mode SPI Wire Mode
 * @{
 */
#define SPI_WIRE_4                  (0ul)
#define SPI_WIRE_3                  (SPI_CR1_SPIMDS)
/**
 * @}
 */

/**
 * @defgroup SPI_Transfer_Mode SPI Transfer Mode
 * @{
 */
#define SPI_FULL_DUPLEX             (0ul)               /*!< Full duplex. */
#define SPI_SEND_ONLY               (SPI_CR1_TXMDS)     /*!< Send only. */
/**
 * @}
 */

/**
 * @defgroup SPI_Master_Slave_Mode SPI Master Slave Mode
 * @{
 */
#define SPI_SLAVE                   (0ul)
#define SPI_MASTER                  (SPI_CR1_MSTR)
/**
 * @}
 */

/**
 * @defgroup SPI_Loopback_Selection SPI Loopback Selection
 * @note Loopback mode is mainly used for parity self-diagnosis in 4-wire full-duplex mode.
 * @{
 */
#define SPI_SPLPBK_INVALID          (0ul)
#define SPI_SPLPBK_MOSI_NOT         (SPI_CR1_SPLPBK)    /*!< MISO data is the inverse of the data output by MOSI. */
#define SPI_SPLPBK_MOSI             (SPI_CR1_SPLPBK2)   /*!< MISO data is the data output by MOSI. */
/**
 * @}
 */

/**
 * @defgroup SPI_Interrupt_Type SPI Interrupt Type
 * @{
 */
#define SPI_INT_ERROR               (SPI_CR1_EIE)       /*!< Including overload, underload and parity error. */
#define SPI_INT_TX_BUFFER_EMPTY     (SPI_CR1_TXIE)
#define SPI_INT_RX_BUFFER_FULL      (SPI_CR1_RXIE)
#define SPI_INT_IDLE                (SPI_CR1_IDIE)
#define SPI_INT_ALL                 (SPI_INT_ERROR           |   \
                                     SPI_INT_TX_BUFFER_EMPTY |   \
                                     SPI_INT_RX_BUFFER_FULL  |   \
                                     SPI_INT_IDLE)
/**
 * @}
 */

/**
 * @defgroup SPI_Mode_Fault_Dectet_Command SPI Mode Fault Dectet Command
 * @{
 */
#define SPI_MODFE_DISABLE           (0ul)               /*!< Disable mode fault detection. */
#define SPI_MODFE_ENABLE            (SPI_CR1_MODFE)     /*!< Enable mode fault detection. */
/**
 * @}
 */

/**
 * @defgroup SPI_Parity_Check_Error_Self_Diagnosis SPI Parity Check Error Self Diagnosis
 * @{
 */
#define SPI_PATE_DISABLE            (0ul)               /*!< Disable self diagnosis of parity check. */
#define SPI_PATE_ENABLE             (SPI_CR1_PATE)      /*!< Enable self diagnosis of parity check. */
/**
 * @}
 */

/**
 * @defgroup SPI_Parity_Check SPI Parity Check
 * @{
 */
#define SPI_PARITY_INVALID          (0ul)                           /*!< Parity check invalid. */
#define SPI_PARITY_EVEN             (SPI_CR1_PAE)                   /*!< Parity check selection even parity. */
#define SPI_PARITY_ODD              (SPI_CR1_PAE | SPI_CR1_PAOE)    /*!< Parity check selection odd parity. */
/**
 * @}
 */

/**
 * @defgroup SPI_NSS_Active_Level SPI NSS Active Level
 * @{
 */
#define SPI_NSS_ACTIVE_LOW          (0ul)               /*!< NSS pin active low. */
#define SPI_NSS_ACTIVE_HIGH         (SPI_CFG1_SS0PV)    /*!< NSS pin active high. */
/**
 * @}
 */

/**
 * @defgroup SPI_Mode SPI Mode
 * @{
 */
#define SPI_MODE_0                  (0ul)
#define SPI_MODE_1                  (SPI_CFG2_CPHA)
#define SPI_MODE_2                  (SPI_CFG2_CPOL)
#define SPI_MODE_3                  (SPI_CFG2_CPOL | SPI_CFG2_CPHA)
/**
 * @}
 */

/**
 * @defgroup SPI_Baud_Rate_Prescaler SPI Baud Rate Prescaler
 * @{
 */
#define SPI_BR_DIV_2                (0ul)                               /*!< SPI baud rate is the system clock divided by 2. */
#define SPI_BR_DIV_4                (SPI_CFG2_MBR_0)                    /*!< SPI baud rate is the system clock divided by 4. */
#define SPI_BR_DIV_8                (SPI_CFG2_MBR_1)                    /*!< SPI baud rate is the system clock divided by 8. */
#define SPI_BR_DIV_16               (SPI_CFG2_MBR_1 | SPI_CFG2_MBR_0)   /*!< SPI baud rate is the system clock divided by 16. */
#define SPI_BR_DIV_32               (SPI_CFG2_MBR_2)                    /*!< SPI baud rate is the system clock divided by 32. */
#define SPI_BR_DIV_64               (SPI_CFG2_MBR_2 | SPI_CFG2_MBR_0)   /*!< SPI baud rate is the system clock divided by 64. */
#define SPI_BR_DIV_128              (SPI_CFG2_MBR_2 | SPI_CFG2_MBR_1)   /*!< SPI baud rate is the system clock divided by 128. */
#define SPI_BR_DIV_256              (SPI_CFG2_MBR_2 | SPI_CFG2_MBR_1 | SPI_CFG2_MBR_0)  /*!< SPI baud rate is the system clock divided by 256. */
/**
 * @}
 */

/**
 * @defgroup SPI_Data_Size SPI Data Size
 * @{
 */
#define SPI_DATA_SIZE_8BIT          (0ul)
#define SPI_DATA_SIZE_16BIT         (SPI_CFG2_DSIZE)
/**
 * @}
 */

/**
 * @defgroup SPI_First_Bit SPI First Bit
 * @{
 */
#define SPI_FIRST_MSB               (0ul)
#define SPI_FIRST_LSB               (SPI_CFG2_LSBF)
/**
 * @}
 */

/**
 * @defgroup SPI_State_Flag SPI State Flag
 * @{
 */
#define SPI_FLAG_OVERLOAD           (SPI_SR_OVRERF)
#define SPI_FLAG_IDLE               (SPI_SR_IDLNF)
#define SPI_FLAG_MODE_FAULT         (SPI_SR_MODFERF)
#define SPI_FLAG_PARITY_ERROR       (SPI_SR_PERF)
#define SPI_FLAG_UNDERLOAD          (SPI_SR_UDRERF)
#define SPI_FLAG_TX_BUFFER_EMPTY    (SPI_SR_TDEF)       /*!< This flag is set when the data in the data register
                                                             is copied into the shift register, but the transmission
                                                             of the data bit may not have been completed. */
#define SPI_FLAG_RX_BUFFER_FULL     (SPI_SR_RDFF)       /*!< When this flag is set, it indicates that a data was received. */

#define SPI_FLAG_ALL                (SPI_FLAG_OVERLOAD          |   \
                                     SPI_FLAG_IDLE              |   \
                                     SPI_FLAG_MODE_FAULT        |   \
                                     SPI_FLAG_PARITY_ERROR      |   \
                                     SPI_FLAG_UNDERLOAD         |   \
                                     SPI_FLAG_TX_BUFFER_EMPTY   |   \
                                     SPI_FLAG_RX_BUFFER_FULL)

#define SPI_FLAG_CLR_ALL            (SPI_FLAG_OVERLOAD          |   \
                                     SPI_FLAG_IDLE              |   \
                                     SPI_FLAG_MODE_FAULT        |   \
                                     SPI_FLAG_PARITY_ERROR      |   \
                                     SPI_FLAG_UNDERLOAD)

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup SPI_Global_Functions
 * @{
 */

/**
 * @brief  SPI function enable or disable.
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable SPI function.
 *   @arg  Disable:                 Disable SPI function.
 * @retval None
 */
__STATIC_INLINE void SPI_FunctionCmd(en_functional_state_t enNewState)
{
    bM0P_SPI->CR1_b.SPE = (uint32_t)enNewState;
}

/**
 * @brief  SPI get state flag.
 * @param  [in]  u32Flag        SPI state flag.
 *                              This parameter can be a value of @ref SPI_State_Flag
 *   @arg  SPI_FLAG_OVERLOAD
 *   @arg  SPI_FLAG_IDLE
 *   @arg  SPI_FLAG_MODE_FAULT
 *   @arg  SPI_FLAG_PARITY_ERROR
 *   @arg  SPI_FLAG_UNDERLOAD
 *   @arg  SPI_FLAG_TX_BUFFER_EMPTY
 *   @arg  SPI_FLAG_RX_BUFFER_FULL
 *   @arg  SPI_FLAG_ALL
 * @retval An en_flag_status_t enumeration.
 *   @arg  Set: The specified flag has set.
 *   @arg  Reset: The specified flag has not set.
 */
__STATIC_INLINE en_flag_status_t SPI_GetFlag(uint32_t u32Flag)
{
    en_flag_status_t enFlag = Reset;

    if ((M0P_SPI->SR & u32Flag) != 0u)
    {
        enFlag = Set;
    }

    return enFlag;
}

/**
 * @brief  SPI clear state flag.
 * @param  [in]  u32Flag        SPI state flag.
 *                              This parameter can be values of @ref SPI_State_Flag.
 *   @arg  SPI_FLAG_OVERLOAD
 *   @arg  SPI_FLAG_IDLE
 *   @arg  SPI_FLAG_MODE_FAULT
 *   @arg  SPI_FLAG_PARITY_ERROR
 *   @arg  SPI_FLAG_UNDERLOAD
 *   @arg  SPI_FLAG_CLR_ALL
 * @retval None
 */
__STATIC_INLINE void SPI_ClearFlag(uint32_t u32Flag)
{
    u32Flag &= SPI_FLAG_CLR_ALL;
    M0P_SPI->SR &= (uint32_t)(~u32Flag);
}

/**
 * @brief  Read SPI data register.
 * @param  None
 * @retval A 32-bit data of SPI data register.
 */
__STATIC_INLINE uint32_t SPI_ReadDataReg(void)
{
    return M0P_SPI->DR;
}

/**
 * @brief  Write SPI data register.
 * @param  [in]  u32Data        The data will be written to the data register.
 * @retval None.
 */
__STATIC_INLINE void SPI_WriteDataReg(uint32_t u32Data)
{
    M0P_SPI->DR = u32Data;
}


en_result_t SPI_Init(const stc_spi_init_t *pstcInit);
void SPI_DeInit(void);
en_result_t SPI_StructInit(stc_spi_init_t *pstcInit);

void SPI_IntCmd(uint32_t u32IntType, en_functional_state_t enNewState);

en_result_t SPI_Transmit(const void *pvTxBuf, uint32_t u32TxLength);
en_result_t SPI_Receive(void *pvRxBuf, uint32_t u32RxLength);
en_result_t SPI_TransmitReceive(const void *pvTxBuf, void *pvRxBuf, uint32_t u32Length);

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

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_SPI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
