/**
 *******************************************************************************
 * @file  hc32m120_usart.h
 * @brief This file contains all the functions prototypes of the USART driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-03       Hongjh          First version
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
#ifndef __HC32M120_USART_H__
#define __HC32M120_USART_H__

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
 * @addtogroup DDL_USART
 * @{
 */

#if (DDL_USART_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup USART_Global_Types USART Global Types
 * @{
 */

/**
 * @brief UART mode initialization structure definition
 */
typedef struct
{
    uint32_t u32Baudrate;               /*!< UART baudrate */

    uint32_t u32ClkMode;                /*!< Clock mode.
                                             This parameter can be a value of @ref USART_Clock_Mode */

    uint32_t u32ClkPrescaler;           /*!< Clock prescaler.
                                             This parameter can be a value of @ref USART_Clock_Prescaler_Division */

    uint32_t u32BitDirection;           /*!< Significant bit.
                                             This parameter can be a value of @ref USART_Significant_Bit */

    uint32_t u32DataWidth;              /*!< Data width.
                                             This parameter can be a value of @ref USART_Data_Width_Bits */

    uint32_t u32StopBit;                /*!< Stop Bits.
                                             This parameter can be a value of @ref USART_Stop_Bits */

    uint32_t u32Parity;                 /*!< Parity format.
                                             This parameter can be a value of @ref USART_Parity_Control */

    uint32_t u32OversamplingBits;       /*!< Oversampling Bits.
                                             This parameter can be a value of @ref USART_Oversampling_Bits */

    uint32_t u32NoiseFilterState;       /*!< RX pin noise filter state.
                                             This parameter can be a value of @ref USART_Noise_Filter_State */

    uint32_t u32SbDetectPolarity;       /*!< Start Bit Detect Polarity.
                                             This parameter can be a value of @ref USART_Start_Bit_Detect_Polarity */

    uint32_t u32HwFlowCtrl;             /*!< Hardware flow control.
                                             This parameter can be a value of @ref USART_HWFLOWCTRL */
} stc_uart_init_t;

/**
 * @brief UART multiple-processor initialization structure definition
 */
typedef struct
{
    uint32_t u32Baudrate;               /*!< UART baudrate */

    uint32_t u32ClkMode;                /*!< Clock mode.
                                             This parameter can be a value of @ref USART_Clock_Mode */

    uint32_t u32ClkPrescaler;           /*!< Clock prescaler.
                                             This parameter can be a value of @ref USART_Clock_Prescaler_Division */

    uint32_t u32BitDirection;           /*!< Significant bit.
                                             This parameter can be a value of @ref USART_Significant_Bit */

    uint32_t u32DataWidth;              /*!< Data width.
                                             This parameter can be a value of @ref USART_Data_Width_Bits */

    uint32_t u32StopBit;                /*!< Stop Bits.
                                             This parameter can be a value of @ref USART_Stop_Bits */

    uint32_t u32OversamplingBits;       /*!< Oversampling Bits.
                                             This parameter can be a value of @ref USART_Oversampling_Bits */

    uint32_t u32NoiseFilterState;       /*!< RX pin noise filter state.
                                             This parameter can be a value of @ref USART_Noise_Filter_State */

    uint32_t u32SbDetectPolarity;       /*!< Start Bit Detect Polarity.
                                             This parameter can be a value of @ref USART_Start_Bit_Detect_Polarity */

    uint32_t u32HwFlowCtrl;             /*!< Hardware flow control.
                                             This parameter can be a value of @ref USART_HWFLOWCTRL */
} stc_uart_multiprocessor_init_t;

/**
 * @brief LIN mode initialization structure definition
 */
typedef struct
{
    uint32_t u32Baudrate;               /*!< UART baudrate */

    uint32_t u32ClkMode;                /*!< Clock mode.
                                             This parameter can be a value of @ref USART_Clock_Mode */

    uint32_t u32ClkPrescaler;           /*!< Clock prescaler.
                                             This parameter can be a value of @ref USART_Clock_Prescaler_Division */

    uint32_t u32OversamplingBits;       /*!< Oversampling Bits.
                                             This parameter can be a value of @ref USART_Oversampling_Bits */

    uint32_t u32NoiseFilterState;       /*!< RX pin noise filter state.
                                             This parameter can be a value of @ref USART_Noise_Filter_State */

    uint32_t u32SbDetectPolarity;       /*!< Start Bit Detect Polarity.
                                             This parameter can be a value of @ref USART_Start_Bit_Detect_Polarity */

    uint32_t u32HwFlowCtrl;             /*!< Hardware flow control.
                                             This parameter can be a value of @ref USART_HWFLOWCTRL */
} stc_lin_init_t;

/**
 * @brief CLKSYNC mode initialization structure definition
 */
typedef struct
{
    uint32_t u32Baudrate;               /*!< UART baudrate */

    uint32_t u32ClkMode;                /*!< Clock mode.
                                             This parameter can be a value of @ref USART_Clock_Mode */

    uint32_t u32ClkPrescaler;           /*!< Clock prescaler.
                                             This parameter can be a value of @ref USART_Clock_Prescaler_Division */

    uint32_t u32BitDirection;           /*!< Significant bit.
                                             This parameter can be a value of @ref USART_Significant_Bit */

    uint32_t u32HwFlowCtrl;             /*!< Hardware flow control.
                                             This parameter can be a value of @ref USART_HWFLOWCTRL */
} stc_clksync_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup USART_Global_Macros USART Global Macros
 * @{
 */

/**
 * @defgroup USART_Flag USART Flag
 *        Elements values convention: 0xXXXXXXXX
 *           - 0xXXXXXXXX  : Flag mask in the USART_SR register
 * @{
 */
#define USART_FLAG_PE                           (USART_SR_PE)   /*!< Parity error flag */
#define USART_FLAG_FE                           (USART_SR_FE)   /*!< Framing error flag */
#define USART_FLAG_ORE                          (USART_SR_ORE)  /*!< Overrun error flag */
#define USART_FLAG_RXNE                         (USART_SR_RXNE) /*!< Receive data register not empty flag */
#define USART_FLAG_TC                           (USART_SR_TC)   /*!< Transmission complete flag */
#define USART_FLAG_TXE                          (USART_SR_TXE)  /*!< Transmit data register empty flag */
#define USART_FLAG_MPB                          (USART_SR_MPB)  /*!< Receive processor ID flag */
/**
 * @}
 */

/**
 * @defgroup USART_Transmission_Type USART Transmission Type
 * @{
 */
#define USART_TRANSMISSION_ID                   (USART_DR_MPID)
#define USART_TRANSMISSION_DATA                 ((uint32_t)0x00000000ul)
/**
 * @}
 */

/**
 * @defgroup USART_Clear_Flag USART Clear Flag
 * @{
 */
#define USART_CLEAR_FLAG_PE                     (USART_CR1_CPE)   /*!< Clear Parity error flag */
#define USART_CLEAR_FLAG_FE                     (USART_CR1_CFE)   /*!< Clear Framing error flag */
#define USART_CLEAR_FLAG_ORE                    (USART_CR1_CORE)  /*!< Clear Overrun error flag */
/**
 * @}
 */

/**
 * @defgroup USART_Interrupt_definition USART Interrupts Definition
 * @{
 */
#define USART_IT_RI                             (USART_CR1_RIE)   /*!< Receive data register not empty && Receive error interrupt */
#define USART_IT_TC                             (USART_CR1_TCIE)  /*!< Transmission complete interrupt */
#define USART_IT_TXE                            (USART_CR1_TXEIE) /*!< Transmit data register empty interrupt */
/**
 * @}
 */

/**
 * @defgroup USART_Transmit_Receive_Function USART Transmit/Receive Function
 * @{
 */
#define USART_RX                                (USART_CR1_RE)    /*!< USART RX function */
#define USART_TX                                (USART_CR1_TE)    /*!< USART TX function */
#define USART_INT_RX                            (USART_CR1_RIE)   /*!< USART RX interrupt function */
#define USART_INT_TC                            (USART_CR1_TCIE)  /*!< USART transmission complete interrupt */
#define USART_INT_TXE                           (USART_CR1_TXEIE) /*!< Transmit data register empty interrupt */
/**
 * @}
 */

/**
 * @defgroup USART_Silence_Mode_State USART Silence Mode State
 * @{
 */
#define USART_UART_SILENCE_MODE                 (USART_CR1_SLME)          /*!< UART silence mode */
#define USART_UART_NORMAL_MODE                  ((uint32_t)0x00000000ul)  /*!< UART normal mode */
/**
 * @}
 */

/**
 * @defgroup USART_Parity_Control USART Parity Control
 * @{
 */
#define USART_PARITY_NONE                       ((uint32_t)0x00000000ul)        /*!< Parity control disabled */
#define USART_PARITY_EVEN                       (USART_CR1_PCE)                 /*!< Parity control enabled and Even Parity is selected */
#define USART_PARITY_ODD                        (USART_CR1_PCE | USART_CR1_PS)  /*!< Parity control enabled and Odd Parity is selected */
/**
 * @}
 */

/**
 * @defgroup USART_Data_Width_Bits Data Width Bits
 * @{
 */
#define USART_DATA_WIDTH_BITS_9                 (USART_CR1_M)           /*!< 9 bits word length : Start bit, 9 data bits, n stop bits */
#define USART_DATA_WIDTH_BITS_8                 ((uint32_t)0x00000000ul)/*!< 8 bits word length : Start bit, 8 data bits, n stop bits */
/**
 * @}
 */

/**
 * @defgroup USART_Oversampling_Bits USART Oversampling Bits
 * @{
 */
#define USART_OVERSAMPLING_BITS_8                (USART_CR1_OVER8)        /*!< Oversampling by 8 bits */
#define USART_OVERSAMPLING_BITS_16               ((uint32_t)0x00000000ul) /*!< Oversampling by 16 bits */
/**
 * @}
 */

/**
 * @defgroup USART_Mode_Selection USART Mode Selection
 * @{
 */
#define USART_MODE_UART                          ((uint32_t)0x00000000ul) /*!< UART mode */
#define USART_MODE_CLKSYNC                       (USART_CR1_MS)           /*!< Clock synchronization */
/**
 * @}
 */

/**
 * @defgroup USART_Significant_Bit USART Significant Bit
 * @{
 */
#define USART_MSB                               (USART_CR1_ML)            /*!< MSB(Most Significant Bit) */
#define USART_LSB                               ((uint32_t)0x00000000ul)  /*!< LSB(Least Significant Bit) */
/**
 * @}
 */

/**
 * @defgroup USART_Noise_Filter_State USART Noise Filter State
 * @{
 */
#define USART_NOISE_FILTER_ENABLE               (USART_CR1_NFE)           /*!< Enable noise filter */
#define USART_NOISE_FILTER_DISABLE              ((uint32_t)0x00000000ul)  /*!< Disable noise filter */
/**
 * @}
 */

/**
 * @defgroup USART_Start_Bit_Detect_Polarity USART Start Bit Detect Polarity
 * @{
 */
#define USART_SB_DETECT_FALLING                 (USART_CR1_SBS)           /*!< Detect RX pin falling edge */
#define USART_SB_DETECT_LOW                     ((uint32_t)0x00000000ul)  /*!< Detect RX pin low level */
/**
 * @}
 */

/**
 * @defgroup USART_Multiple_Processor_State USART Multiple Processor State
 * @{
 */
#define USART_MULTIPLE_PROCESSOR_ENABLE         (USART_CR2_MPE)           /*!< Enable multiple processor function */
#define USART_MULTIPLE_PROCESSOR_DISABLE        ((uint32_t)0x00000000ul)  /*!< Disable multiple processor function */
/**
 * @}
 */

/**
 * @defgroup USART_Clock_Mode USART Clock Mode Selection
 * @{
 */
#define USART_EXTCLK                            (USART_CR2_CLKC_1)        /*!< Select external clock source. */
#define USART_INTCLK_OUTPUT                     (USART_CR2_CLKC_0)        /*!< Select internal clock source and output clock. */
#define USART_INTCLK_NONE_OUTPUT                ((uint32_t)0x00000000ul)  /*!< Select internal clock source and don't output clock */
/**
 * @}
 */

/**
 * @defgroup USART_Stop_Bits USART Stop Bits
 * @{
 */
#define USART_STOP_BITS_2                       (USART_CR2_STOP)          /*!< 2 stop bit */
#define USART_STOP_BITS_1                       ((uint32_t)0x00000000ul)  /*!< 1 stop bit */
/**
 * @}
 */

/**
 * @defgroup USART_LIN_Mode_State USART LIN Mode State
 * @{
 */
#define USART_LIN_MODE_ENABLE                   (USART_CR2_LINEN)         /*!< Enable LIN mode */
#define USART_LIN_MODE_DISABLE                  ((uint32_t)0x00000000ul)  /*!< Disable LIN mode */
/**
 * @}
 */

/**
 * @defgroup USART_HALFDUPLEX_FULLDUPLEX_SEL USART Half-duplex/Full-duplex Selection
 * @{
 */
#define USART_HALFDUPLEX_MODE                   (USART_CR3_HDSEL)         /*!< USART half-duplex mode */
#define USART_FULLDUPLEX_MODE                   ((uint32_t)0x00000000ul)  /*!< USART full-duplex mode */
/**
 * @}
 */

/**
 * @defgroup USART_HWFLOWCTRL USART Hardware Flow Control CTS/RTS
 * @{
 */
#define USART_HWFLOWCTRL_CTS                    (USART_CR3_CTSE)          /*!< UART hardware flow control CTS mode */
#define USART_HWFLOWCTRL_RTS                    ((uint32_t)0x00000000ul)  /*!< UART hardware flow control RTS mode */
/**
 * @}
 */

/**
 * @defgroup USART_Clock_Prescaler_Division USART Clock Prescaler Division
 * @{
 */
#define USART_CLK_PRESCALER_DIV1                ((uint32_t)0x00000000ul)  /*!< PCLK */
#define USART_CLK_PRESCALER_DIV4                (USART_PR_PSC_0)          /*!< PCLK/4 */
#define USART_CLK_PRESCALER_DIV16               (USART_PR_PSC_1)          /*!< PCLK/16 */
#define USART_CLK_PRESCALER_DIV64               (USART_PR_PSC)            /*!< PCLK/64 */
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
 * @addtogroup USART_Global_Functions
 * @{
 */

/**
 * @brief  Enable USART noise filter.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval None
 */
__STATIC_INLINE void USART_EnableNoiseFilter(M0P_USART_TypeDef *USARTx)
{
    SET_REG32_BIT(USARTx->CR1, USART_CR1_NFE);
}

/**
 * @brief  Disable USART noise filter.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval None
 */
__STATIC_INLINE void USART_DisableNoiseFilter(M0P_USART_TypeDef *USARTx)
{
    CLEAR_REG32_BIT(USARTx->CR1, USART_CR1_NFE);
}

/**
 * @brief  Enable UART multiple processor function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval None
 */
__STATIC_INLINE void USART_EnableMultProcessor(M0P_USART_TypeDef *USARTx)
{
    SET_REG32_BIT(USARTx->CR2, USART_CR2_MPE);
}

/**
 * @brief  Disable UART multiple processor function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval None
 */
__STATIC_INLINE void USART_DisableMultProcessor(M0P_USART_TypeDef *USARTx)
{
    CLEAR_REG32_BIT(USARTx->CR2, USART_CR2_MPE);
}

/**
 * @brief  Enable UART LIN function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval None
 */
__STATIC_INLINE void USART_EnableLIN(M0P_USART_TypeDef *USARTx)
{
    SET_REG32_BIT(USARTx->CR1, USART_CR2_LINEN);
}

/**
 * @brief  Disable UART LIN function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval None
 */
__STATIC_INLINE void USART_DisableLIN(M0P_USART_TypeDef *USARTx)
{
    CLEAR_REG32_BIT(USARTx->CR2, USART_CR2_LINEN);
}

/**
 * @brief  USART receive data.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Receive data
 */
__STATIC_INLINE uint16_t USART_RecData(M0P_USART_TypeDef *USARTx)
{
    return (uint16_t)(READ_REG32(USARTx->DR) >> USART_DR_RDR_POS);
}

/**
 * @brief  USART send data.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 ** \param [in] u16Data                 Transmit data
 * @retval None
 */
__STATIC_INLINE void USART_SendData(M0P_USART_TypeDef *USARTx,
                                            uint16_t u16Data)
{
    WRITE_REG32(USARTx->DR,  ((uint32_t)u16Data & 0x01FFul));
}

/**
 * @brief  USART send processor ID.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 ** \param [in] u16ID                   Processor ID
 * @retval None
 */
__STATIC_INLINE void USART_SendId(M0P_USART_TypeDef *USARTx,
                                        uint16_t u16ID)
{
    WRITE_REG32(USARTx->DR, (USART_DR_MPID | ((uint32_t)u16ID & 0x01FFul)));
}

en_result_t USART_UartInit(M0P_USART_TypeDef *USARTx,
                            const stc_uart_init_t *pstcInit);
en_result_t USART_HalfDuplexInit(M0P_USART_TypeDef *USARTx,
                                            const stc_uart_init_t *pstcInit);
en_result_t USART_UartStructInit(stc_uart_init_t *pstcInit);
en_result_t USART_MultiProcessorInit(M0P_USART_TypeDef *USARTx,
                                const stc_uart_multiprocessor_init_t *pstcInit);
en_result_t UART_MultiProcessorStructInit(stc_uart_multiprocessor_init_t *pstcInit);
en_result_t USART_LinInit(M0P_USART_TypeDef *USARTx,
                                const stc_lin_init_t *pstcInit);
en_result_t USART_LinStructInit(stc_lin_init_t *pstcInit);
en_result_t USART_ClkSyncInit(M0P_USART_TypeDef *USARTx,
                                        const stc_clksync_init_t *pstcInit);
en_result_t USART_ClkSyncStructInit(stc_clksync_init_t *pstcInit);
en_result_t USART_DeInit(M0P_USART_TypeDef *USARTx);

en_result_t USART_FuncCmd(M0P_USART_TypeDef *USARTx,
                            uint32_t u32Func,
                            en_functional_state_t enNewSta);
en_functional_state_t USART_GetFuncState(M0P_USART_TypeDef *USARTx,
                            uint32_t u32Func);
en_flag_status_t USART_GetFlag(M0P_USART_TypeDef *USARTx,
                                        uint32_t u32Flag);
en_result_t USART_SilenceModeCmd(M0P_USART_TypeDef *USARTx, en_functional_state_t enNewSta);
en_functional_state_t USART_GetSilenceModeState(M0P_USART_TypeDef *USARTx);
en_result_t USART_ClearFlag(M0P_USART_TypeDef *USARTx, uint32_t u32Flag);
en_result_t USART_SetTransmissionType(M0P_USART_TypeDef *USARTx,
                                            uint32_t u32Type);
uint32_t USART_GetTransmissionType(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetParity(M0P_USART_TypeDef *USARTx, uint32_t u32Parity);
uint32_t USART_GetParity(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetDataWidth(M0P_USART_TypeDef *USARTx,
                                    uint32_t u32DataWidth);
uint32_t USART_GetDataWidth(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetOversmaplingBits(M0P_USART_TypeDef *USARTx,
                                            uint32_t u32OversamplingBits);
uint32_t USART_GetOversmaplingBits(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetMode(M0P_USART_TypeDef *USARTx, uint32_t u32Mode);
uint32_t USART_GetMode(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetBitDirection(M0P_USART_TypeDef *USARTx,
                                        uint32_t u32BitDir);
uint32_t USART_GetBitDirection(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetSbDetectPolarity(M0P_USART_TypeDef *USARTx,
                                            uint32_t u32Polarity);
uint32_t USART_GetSbDetectPolarity(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetClockMode(M0P_USART_TypeDef *USARTx, uint32_t u32ClkMode);
uint32_t USART_GetClockMode(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetStopBits(M0P_USART_TypeDef *USARTx, uint32_t u32StopBits);
uint32_t USART_GetStopBits(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetDuplexMode(M0P_USART_TypeDef *USARTx,
                                uint32_t u32Mode);
uint32_t USART_GetDuplexMode(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetHwFlowCtrl(M0P_USART_TypeDef *USARTx,
                                uint32_t u32HwFlowCtrl);
uint32_t USART_GetHwFlowCtrl(M0P_USART_TypeDef *USARTx);
en_result_t USART_SetClkPrescaler(M0P_USART_TypeDef *USARTx,
                                    uint32_t u32PrescalerDiv);
uint32_t USART_GetClkPrescaler(M0P_USART_TypeDef *USARTx);

en_result_t USART_SetBaudrate(M0P_USART_TypeDef *USARTx,
                                    uint32_t u32Baudrate,
                                    float32_t *pf32Err);

/**
 * @}
 */

#endif /* DDL_USART_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_USART_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
