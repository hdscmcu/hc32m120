/**
 *******************************************************************************
 * @file  hc32m120_usart.c
 * @brief This file provides firmware functions to manage the USART.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-03       Hongjh          First version
   2020-02-27       Hongjh          Delete struct member u32HwFlowCtrl of stc_lin_init_t
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
#include "hc32m120_usart.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_USART USART
 * @brief USART Driver Library
 * @{
 */

#if (DDL_USART_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup USART_Local_Macros USART Local Macros
 * @{
 */

/**
 * @defgroup USART_Check_Parameters_Validity USART Check Parameters Validity
 * @{
 */

#define IS_USART_INSTANCE(x)                                                   \
(   (M0P_USART1 == (x))                         ||                             \
    (M0P_USART2 == (x))                         ||                             \
    (M0P_USART3 == (x)))

#define IS_USART_FLAG(x)                                                       \
(   (x) & (USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_TC |                     \
    USART_FLAG_ORE | USART_FLAG_MPB | USART_FLAG_TXE | USART_FLAG_RXNE))

#define IS_USART_CLEAR_FLAG(x)                                                 \
(    (x) & (USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_ORE))

#define IS_USART_TRANSMISSION_TYPE(x)                                          \
(   (USART_TRANSMISSION_ID == (x))              ||                             \
    (USART_TRANSMISSION_DATA == (x)))

#define IS_USART_TRANSMIT_RECEIVE_FUNCTION(x)                                  \
(    (x) & (USART_RX | USART_TX | USART_INT_RX | USART_INT_TC | USART_INT_TXE))

#define IS_USART_PARITY_CONTROL(x)                                             \
(   (USART_PARITY_ODD == (x))                   ||                             \
    (USART_PARITY_EVEN == (x))                  ||                             \
    (USART_PARITY_NONE == (x)))

#define IS_USART_DATA_WIDTH(x)                                                 \
(   (USART_DATA_WIDTH_BITS_8 == (x))            ||                             \
    (USART_DATA_WIDTH_BITS_9 == (x)))

#define IS_USART_NOISE_FILTER(x)                                               \
(   (USART_NOISE_FILTER_ENABLE == (x))          ||                             \
    (USART_NOISE_FILTER_DISABLE == (x)))

#define IS_USART_OVERSAMPLING_BITS(x)                                          \
(   (USART_OVERSAMPLING_BITS_8 == (x))          ||                             \
    (USART_OVERSAMPLING_BITS_16 == (x)))

#define IS_USART_MODE_SEL(x)                                                   \
(   (USART_MODE_UART == (x))                    ||                             \
    (USART_MODE_CLKSYNC == (x)))

#define IS_USART_SIGNIFICANT_BIT(x)                                            \
(   (USART_MSB == (x))                          ||                             \
    (USART_LSB == (x)))

#define IS_USART_SB_DETECT_POLARITY(x)                                         \
(   (USART_SB_DETECT_LOW == (x))                ||                             \
    (USART_SB_DETECT_FALLING == (x)))

#define IS_USART_SILENCE_MODE(x)                                               \
(   (USART_UART_NORMAL_MODE == (x))             ||                             \
    (USART_UART_SILENCE_MODE == (x)))

#define IS_USART_CLOCK_MODE(x)                                                 \
(   (USART_EXTCLK == (x))                       ||                             \
    (USART_INTCLK_OUTPUT == (x))                ||                             \
    (USART_INTCLK_NONE_OUTPUT == (x)))

#define IS_USART_STOP_BITS(x)                                                  \
(   (USART_STOP_BITS_1 == (x))                  ||                             \
    (USART_STOP_BITS_2 == (x)))

#define IS_UART_DUPLEX_MODE(x)                                                 \
(   (USART_HALFDUPLEX_MODE == (x))              ||                             \
    (USART_FULLDUPLEX_MODE == (x)))

#define IS_USART_HWFLOWCTRL(x)                                                 \
(   (USART_HWFLOWCTRL_CTS == (x))               ||                             \
    (USART_HWFLOWCTRL_RTS == (x)))

#define IS_USART_CLOCK_PRESCALER_DIV(x)                                        \
(   (USART_CLK_PRESCALER_DIV1 == (x))           ||                             \
    (USART_CLK_PRESCALER_DIV4 == (x))           ||                             \
    (USART_CLK_PRESCALER_DIV16 == (x))          ||                             \
    (USART_CLK_PRESCALER_DIV64 == (x)))

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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/** 
 * @defgroup USART_Global_Functions USART Global Functions
 * @{
 */

/**
 * @brief  Initialize UART function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] pstcInit                Pointer to a @ref stc_uart_init_t structure (USARTx unit UART function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance or pstcInit == NULL
 */
en_result_t USART_UartInit(M0P_USART_TypeDef *USARTx,
                            const stc_uart_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance and pstcInit */
    if ((IS_USART_INSTANCE(USARTx)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_STOP_BITS(pstcInit->u32StopBit));
        DDL_ASSERT(IS_USART_CLOCK_MODE(pstcInit->u32ClkMode));
        DDL_ASSERT(IS_USART_DATA_WIDTH(pstcInit->u32DataWidth));
        DDL_ASSERT(IS_USART_HWFLOWCTRL(pstcInit->u32HwFlowCtrl));
        DDL_ASSERT(IS_USART_PARITY_CONTROL(pstcInit->u32Parity));
        DDL_ASSERT(IS_USART_SIGNIFICANT_BIT(pstcInit->u32BitDirection));
        DDL_ASSERT(IS_USART_NOISE_FILTER(pstcInit->u32NoiseFilterState));
        DDL_ASSERT(IS_USART_CLOCK_PRESCALER_DIV(pstcInit->u32ClkPrescaler));
        DDL_ASSERT(IS_USART_OVERSAMPLING_BITS(pstcInit->u32OversamplingBits));
        DDL_ASSERT(IS_USART_SB_DETECT_POLARITY(pstcInit->u32SbDetectPolarity));

        /* Disbale TX/RX && clear interrupt flag */
        WRITE_REG32(USARTx->CR1, (USART_CR1_CPE | USART_CR1_CFE | USART_CR1_CORE));

        /* Set CR1 */
        MODIFY_REG32(USARTx->CR1,
                     (USART_CR1_SLME | USART_CR1_PS | USART_CR1_PCE |          \
                      USART_CR1_M | USART_CR1_OVER8 | USART_CR1_MS |           \
                      USART_CR1_ML | USART_CR1_NFE | USART_CR1_SBS),           \
                     (USART_MODE_UART | pstcInit->u32Parity | pstcInit->u32DataWidth | \
                      pstcInit->u32OversamplingBits | pstcInit->u32BitDirection      | \
                      pstcInit->u32NoiseFilterState | pstcInit->u32SbDetectPolarity));

        /* Set CR2 */
        WRITE_REG32(USARTx->CR2, (pstcInit->u32ClkMode | pstcInit->u32StopBit));

        /* Set CR3 */
        WRITE_REG32(USARTx->CR3, pstcInit->u32HwFlowCtrl);

        /* Set PR */
        WRITE_REG32(USARTx->PR, pstcInit->u32ClkPrescaler);

        /* Set baudrate */
        enRet = USART_SetBaudrate(USARTx, pstcInit->u32Baudrate, NULL);
    }

    return enRet;
}

/**
 * @brief  Initialize UART half dulplex function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] pstcInit                Pointer to a @ref stc_uart_init_t structure (USARTx unit UART half dulplex function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance or pstcInit == NULL
 */
en_result_t USART_HalfDuplexInit(M0P_USART_TypeDef *USARTx,
                                            const stc_uart_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance and pstcInit */
    if ((IS_USART_INSTANCE(USARTx)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_STOP_BITS(pstcInit->u32StopBit));
        DDL_ASSERT(IS_USART_CLOCK_MODE(pstcInit->u32ClkMode));
        DDL_ASSERT(IS_USART_DATA_WIDTH(pstcInit->u32DataWidth));
        DDL_ASSERT(IS_USART_HWFLOWCTRL(pstcInit->u32HwFlowCtrl));
        DDL_ASSERT(IS_USART_PARITY_CONTROL(pstcInit->u32Parity));
        DDL_ASSERT(IS_USART_SIGNIFICANT_BIT(pstcInit->u32BitDirection));
        DDL_ASSERT(IS_USART_NOISE_FILTER(pstcInit->u32NoiseFilterState));
        DDL_ASSERT(IS_USART_CLOCK_PRESCALER_DIV(pstcInit->u32ClkPrescaler));
        DDL_ASSERT(IS_USART_OVERSAMPLING_BITS(pstcInit->u32OversamplingBits));
        DDL_ASSERT(IS_USART_SB_DETECT_POLARITY(pstcInit->u32SbDetectPolarity));

        /* Disbale TX/RX && clear interrupt flag */
        WRITE_REG32(USARTx->CR1, (USART_CR1_CPE | USART_CR1_CFE | USART_CR1_CORE));

        /* Set CR1 */
        MODIFY_REG32(USARTx->CR1,
                     (USART_CR1_SLME | USART_CR1_PS | USART_CR1_PCE |          \
                      USART_CR1_M | USART_CR1_OVER8 | USART_CR1_MS |           \
                      USART_CR1_ML | USART_CR1_NFE | USART_CR1_SBS),           \
                     (USART_MODE_UART | pstcInit->u32Parity | pstcInit->u32DataWidth | \
                      pstcInit->u32OversamplingBits | pstcInit->u32BitDirection      | \
                      pstcInit->u32NoiseFilterState | pstcInit->u32SbDetectPolarity));

        /* Set CR2 */
        WRITE_REG32(USARTx->CR2, (pstcInit->u32ClkMode | pstcInit->u32StopBit));

        /* Set CR3 */
        WRITE_REG32(USARTx->CR3, (pstcInit->u32HwFlowCtrl | USART_CR3_HDSEL));

        /* Set PR */
        WRITE_REG32(USARTx->PR, pstcInit->u32ClkPrescaler);

        /* Set baudrate */
        enRet = USART_SetBaudrate(USARTx, pstcInit->u32Baudrate, NULL);
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_uart_init_t to default values.
 * @param  [out] pstcInit               Pointer to a @ref stc_uart_init_t structure (USARTx unit UART function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit == NULL
 */
en_result_t USART_UartStructInit(stc_uart_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (NULL != pstcInit)
    {
        pstcInit->u32Baudrate = 9600ul;
        pstcInit->u32BitDirection = USART_LSB;
        pstcInit->u32Parity = USART_PARITY_NONE;
        pstcInit->u32StopBit = USART_STOP_BITS_1;
        pstcInit->u32HwFlowCtrl = USART_HWFLOWCTRL_RTS;
        pstcInit->u32ClkMode = USART_INTCLK_NONE_OUTPUT;
        pstcInit->u32ClkPrescaler = USART_CLK_PRESCALER_DIV1;
        pstcInit->u32DataWidth = USART_DATA_WIDTH_BITS_8;
        pstcInit->u32SbDetectPolarity = USART_SB_DETECT_FALLING;
        pstcInit->u32NoiseFilterState = USART_NOISE_FILTER_DISABLE;
        pstcInit->u32OversamplingBits = USART_OVERSAMPLING_BITS_16;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Initialize UART multiple processor function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] pstcInit                Pointer to a @ref stc_uart_multiprocessor_init_t structure (USARTx unit UART multiple processor function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance or pstcInit == NULL
 */
en_result_t USART_MultiProcessorInit(M0P_USART_TypeDef *USARTx,
                                const stc_uart_multiprocessor_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance and pstcInit */
    if ((IS_USART_INSTANCE(USARTx)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_STOP_BITS(pstcInit->u32StopBit));
        DDL_ASSERT(IS_USART_CLOCK_MODE(pstcInit->u32ClkMode));
        DDL_ASSERT(IS_USART_DATA_WIDTH(pstcInit->u32DataWidth));
        DDL_ASSERT(IS_USART_HWFLOWCTRL(pstcInit->u32HwFlowCtrl));
        DDL_ASSERT(IS_USART_SIGNIFICANT_BIT(pstcInit->u32BitDirection));
        DDL_ASSERT(IS_USART_NOISE_FILTER(pstcInit->u32NoiseFilterState));
        DDL_ASSERT(IS_USART_CLOCK_PRESCALER_DIV(pstcInit->u32ClkPrescaler));
        DDL_ASSERT(IS_USART_OVERSAMPLING_BITS(pstcInit->u32OversamplingBits));
        DDL_ASSERT(IS_USART_SB_DETECT_POLARITY(pstcInit->u32SbDetectPolarity));

        /* Disbale TX/RX && clear interrupt flag */
        WRITE_REG32(USARTx->CR1, (USART_CR1_CPE | USART_CR1_CFE | USART_CR1_CORE));

        /* Set CR1 */
        MODIFY_REG32(USARTx->CR1, \
                     (USART_CR1_SLME | USART_CR1_PS | USART_CR1_PCE |          \
                      USART_CR1_M | USART_CR1_OVER8 | USART_CR1_MS  |          \
                      USART_CR1_ML | USART_CR1_NFE | USART_CR1_SBS),           \
                     (USART_MODE_UART | USART_CR1_SLME | pstcInit->u32DataWidth | \
                      pstcInit->u32OversamplingBits | pstcInit->u32BitDirection | \
                      pstcInit->u32NoiseFilterState | pstcInit->u32SbDetectPolarity));

        /* Set CR2 */
        WRITE_REG32(USARTx->CR2, (USART_CR2_MPE | pstcInit->u32ClkMode | pstcInit->u32StopBit));

        /* Set CR3 */
        WRITE_REG32(USARTx->CR3, pstcInit->u32HwFlowCtrl);

        /* Set PR */
        WRITE_REG32(USARTx->PR, pstcInit->u32ClkPrescaler);

        /* Set baudrate */
        enRet = USART_SetBaudrate(USARTx, pstcInit->u32Baudrate, NULL);
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_uart_init_t to default values.
 * @param  [out] pstcInit               Pointer to a @ref stc_uart_multiprocessor_init_t structure (USARTx unit UART multiple processor function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcInit == NULL
 */
en_result_t UART_MultiProcessorStructInit(stc_uart_multiprocessor_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (NULL != pstcInit)
    {
        pstcInit->u32Baudrate = 9600ul;
        pstcInit->u32BitDirection = USART_LSB;
        pstcInit->u32StopBit = USART_STOP_BITS_1;
        pstcInit->u32HwFlowCtrl = USART_HWFLOWCTRL_RTS;
        pstcInit->u32ClkMode = USART_INTCLK_NONE_OUTPUT;
        pstcInit->u32ClkPrescaler = USART_CLK_PRESCALER_DIV1;
        pstcInit->u32DataWidth = USART_DATA_WIDTH_BITS_8;
        pstcInit->u32SbDetectPolarity = USART_SB_DETECT_FALLING;
        pstcInit->u32NoiseFilterState = USART_NOISE_FILTER_DISABLE;
        pstcInit->u32OversamplingBits = USART_OVERSAMPLING_BITS_16;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Initialize LIN function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] pstcInit                Pointer to a @ref stc_lin_init_t structure (USARTx unit LIN function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: USARTx is invalid instance or pstcInit == NULL
 */
en_result_t USART_LinInit(M0P_USART_TypeDef *USARTx,
                                const stc_lin_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance and pstcInit */
    if ((IS_USART_INSTANCE(USARTx)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_CLOCK_MODE(pstcInit->u32ClkMode));
        DDL_ASSERT(IS_USART_NOISE_FILTER(pstcInit->u32NoiseFilterState));
        DDL_ASSERT(IS_USART_CLOCK_PRESCALER_DIV(pstcInit->u32ClkPrescaler));
        DDL_ASSERT(IS_USART_OVERSAMPLING_BITS(pstcInit->u32OversamplingBits));
        DDL_ASSERT(IS_USART_SB_DETECT_POLARITY(pstcInit->u32SbDetectPolarity));

        /* Disbale TX/RX && clear interrupt flag */
        WRITE_REG32(USARTx->CR1, (USART_CR1_CPE | USART_CR1_CFE | USART_CR1_CORE));

        /* Set CR1 */
        MODIFY_REG32(USARTx->CR1,
                     (USART_CR1_SLME | USART_CR1_PS | USART_CR1_PCE |          \
                      USART_CR1_M | USART_CR1_OVER8 | USART_CR1_MS  |          \
                      USART_CR1_ML | USART_CR1_NFE | USART_CR1_SBS),           \
                     (USART_MODE_UART | pstcInit->u32OversamplingBits |        \
                      pstcInit->u32NoiseFilterState | pstcInit->u32SbDetectPolarity));

        /* Set CR2 */
        WRITE_REG32(USARTx->CR2, (pstcInit->u32ClkMode | USART_CR2_LINEN));

        /* Set CR3 */
        WRITE_REG32(USARTx->CR3, 0UL);

        /* Set PR */
        WRITE_REG32(USARTx->PR, pstcInit->u32ClkPrescaler);

        /* Set baudrate */
        enRet = USART_SetBaudrate(USARTx, pstcInit->u32Baudrate, NULL);
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_lin_init_t to default values.
 * @param  [out] pstcInit               Pointer to a @ref stc_lin_init_t structure (USARTx unit LIN function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: UpstcInit == NULL
 */
en_result_t USART_LinStructInit(stc_lin_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (NULL != pstcInit)
    {
        pstcInit->u32Baudrate = 9600ul;
        pstcInit->u32ClkMode = USART_INTCLK_NONE_OUTPUT;
        pstcInit->u32ClkPrescaler = USART_CLK_PRESCALER_DIV1;
        pstcInit->u32NoiseFilterState = USART_NOISE_FILTER_DISABLE;
        pstcInit->u32OversamplingBits = USART_OVERSAMPLING_BITS_16;
        pstcInit->u32SbDetectPolarity = USART_SB_DETECT_LOW;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Initialize CLKSYNC function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] pstcInit                Pointer to a @ref stc_clksync_init_t structure (USARTx unit CLKSYNC function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance or pstcInit == NULL
 */
en_result_t USART_ClkSyncInit(M0P_USART_TypeDef *USARTx,
                                        const stc_clksync_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMRBx instance and pstcInit */
    if ((IS_USART_INSTANCE(USARTx)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_CLOCK_MODE(pstcInit->u32ClkMode));
        DDL_ASSERT(IS_USART_HWFLOWCTRL(pstcInit->u32HwFlowCtrl));
        DDL_ASSERT(IS_USART_SIGNIFICANT_BIT(pstcInit->u32BitDirection));
        DDL_ASSERT(IS_USART_CLOCK_PRESCALER_DIV(pstcInit->u32ClkPrescaler));

        /* Disbale TX/RX && clear interrupt flag */
        WRITE_REG32(USARTx->CR1, (USART_CR1_CPE | USART_CR1_CFE | USART_CR1_CORE));

        /* Set CR1 */
        MODIFY_REG32(USARTx->CR1,
                     (USART_CR1_SLME | USART_CR1_PS | USART_CR1_PCE |          \
                      USART_CR1_M | USART_CR1_OVER8 | USART_CR1_MS  |          \
                      USART_CR1_ML | USART_CR1_NFE | USART_CR1_SBS),           \
                     (USART_MODE_CLKSYNC | pstcInit->u32BitDirection));

        /* Set CR2 */
        WRITE_REG32(USARTx->CR2, pstcInit->u32ClkMode);

        /* Set CR3 */
        WRITE_REG32(USARTx->CR3, pstcInit->u32HwFlowCtrl);

        /* Set PR */
        WRITE_REG32(USARTx->PR, pstcInit->u32ClkPrescaler);

        /* Set baudrate */
        enRet = USART_SetBaudrate(USARTx, pstcInit->u32Baudrate, NULL);
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_clksync_init_t to default values.
 * @param  [out] pstcInit               Pointer to a @ref stc_clksync_init_t structure (USARTx unit CLKSYNC function configuration data structure).
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit == NULL
 */
en_result_t USART_ClkSyncStructInit(stc_clksync_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (NULL != pstcInit)
    {
        pstcInit->u32Baudrate = 9600ul;
        pstcInit->u32ClkMode = USART_INTCLK_NONE_OUTPUT;
        pstcInit->u32ClkPrescaler = USART_CLK_PRESCALER_DIV1;
        pstcInit->u32BitDirection = USART_LSB;
        pstcInit->u32HwFlowCtrl = USART_HWFLOWCTRL_RTS;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-Initialize USART function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_DeInit(M0P_USART_TypeDef *USARTx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Configures the registers to reset value. */
        WRITE_REG32(USARTx->CR1, (USART_CR1_CPE | USART_CR1_CFE | USART_CR1_CORE));
        WRITE_REG32(USARTx->PR, 0x00000000ul);
        WRITE_REG32(USARTx->BRR, 0x0000FF00ul);
        WRITE_REG32(USARTx->CR1, 0x80000000ul);
        WRITE_REG32(USARTx->CR2, 0x00000000ul);
        WRITE_REG32(USARTx->CR3, 0x00000000ul);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Enable/disable USART Transmit/Receive Function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Func                 USART function type
 *         This parameter can be any composed value of the following values:
 *           @arg USART_RX:             USART RX function
 *           @arg USART_TX:             USART TX function
 *           @arg USART_INT_RX:         USART RX interrupt function
 *           @arg USART_INT_TC:         USART transmission complete interrupt
 *           @arg USART_INT_TXE:        Transmit data register empty interrupt
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_FuncCmd(M0P_USART_TypeDef *USARTx,
                            uint32_t u32Func,
                            en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));
        DDL_ASSERT(IS_USART_TRANSMIT_RECEIVE_FUNCTION(u32Func));

        (Enable == enNewSta) ? SET_REG32_BIT(USARTx->CR1, u32Func) : CLEAR_REG32_BIT(USARTx->CR1, u32Func);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART interrupt state.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Func                 USART function type
 *         This parameter can be one of the following values:
 *           @arg USART_RX:             USART RX function
 *           @arg USART_TX:             USART TX function
 *           @arg USART_INT_RX:         USART RX interrupt function
 *           @arg USART_INT_TC:         USART transmission complete interrupt
 *           @arg USART_INT_TXE:        Transmit data register empty interrupt
 * @retval An en_functional_state_t enumeration value:
 *           - Enable: Enable function
 *           - Disable: Disable function
 */
en_functional_state_t USART_GetFuncState(M0P_USART_TypeDef *USARTx,
                            uint32_t u32Func)
{
    /* Check parameters */
    DDL_ASSERT (IS_USART_INSTANCE(USARTx));
    DDL_ASSERT(IS_USART_TRANSMIT_RECEIVE_FUNCTION(u32Func));

    return (READ_REG32_BIT(USARTx->CR1, u32Func) ? Enable : Disable);
}

/**
 * @brief  Get USART flag.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Flag                 USART flag type
 *         This parameter can be one of the following values:
 *           @arg USART_FLAG_PE:        Parity error flag
 *           @arg USART_FLAG_FE:        Framing error flag
 *           @arg USART_FLAG_ORE:       Overrun error flag
 *           @arg USART_FLAG_RXNE:      Receive data register not empty flag
 *           @arg USART_FLAG_TC:        Transmission complete flag
 *           @arg USART_FLAG_TXE:       Transmit data register empty flag
 *           @arg USART_FLAG_MPB:       Receive processor ID flag
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Flag is set
 *           - Reset: Flag is reset
 */
en_flag_status_t USART_GetFlag(M0P_USART_TypeDef *USARTx, uint32_t u32Flag)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));
    DDL_ASSERT(IS_USART_FLAG(u32Flag));

    return (READ_REG32_BIT(USARTx->SR, u32Flag) ? Set : Reset);
}

/**
 * @brief  Get USART flag.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Flag                 USART flag type
 *         This parameter can be any composed value of the following values:
 *           @arg USART_CLEAR_FLAG_PE:  Clear Parity error flag
 *           @arg USART_CLEAR_FLAG_FE:  Clear Framing error flag
 *           @arg USART_CLEAR_FLAG_ORE: Clear Overrun error flag
 * @retval An en_result_t enumeration value:
 *           - Ok: Clear successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_ClearFlag(M0P_USART_TypeDef *USARTx, uint32_t u32Flag)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_CLEAR_FLAG(u32Flag));

        SET_REG32_BIT(USARTx->CR1, u32Flag);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set USART silence mode.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SilenceModeCmd(M0P_USART_TypeDef *USARTx, en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        (Enable == enNewSta) ? SET_REG32_BIT(USARTx->CR1, USART_CR1_SLME) : CLEAR_REG32_BIT(USARTx->CR1, USART_CR1_SLME);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART silence mode.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg Enable:               UART silence mode
 *           @arg Disable:              UART normal mode
 */
en_functional_state_t USART_GetSilenceModeState(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR1, USART_CR1_SLME) ? Enable : Disable;
}

/**
 * @brief  Set USART transmission type.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Type                 USART transmission content type
 *         This parameter can be one of the following values:
 *           @arg USART_TRANSMISSION_ID     USART transmission content type is processor ID
 *           @arg USART_TRANSMISSION_DATA   USART transmission content type is frame data
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetTransmissionType(M0P_USART_TypeDef *USARTx, uint32_t u32Type)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_TRANSMISSION_TYPE(u32Type));

        MODIFY_REG32(USARTx->DR, USART_DR_MPID, u32Type);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART transmission type.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_TRANSMISSION_ID     USART transmission content type is processor ID
 *           @arg USART_TRANSMISSION_DATA   USART transmission content type is frame data
 */
uint32_t USART_GetTransmissionType(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->DR, USART_DR_MPID);
}

/**
 * @brief  Set USART parity.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Parity               USART parity
 *         This parameter can be one of the following values:
 *           @arg USART_PARITY_NONE:    Parity control disabled
 *           @arg USART_PARITY_ODD:     Parity control enabled and Odd Parity is selected
 *           @arg USART_PARITY_EVEN:    Parity control enabled and Even Parity is selected
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetParity(M0P_USART_TypeDef *USARTx, uint32_t u32Parity)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_PARITY_CONTROL(u32Parity));

        MODIFY_REG32(USARTx->CR1, (USART_CR1_PS | USART_CR1_PCE), u32Parity);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART parity.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_PARITY_NONE:    Parity control disabled
 *           @arg USART_PARITY_ODD:     Parity control enabled and Odd Parity is selected
 *           @arg USART_PARITY_EVEN:    Parity control enabled and Even Parity is selected
 */
uint32_t USART_GetParity(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR1, (USART_CR1_PS | USART_CR1_PCE));
}

/**
 * @brief  Set USART data width.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32DataWidth            USART data width
 *         This parameter can be one of the following values:
 *           @arg USART_DATA_WIDTH_BITS_8:  8 bits word length : Start bit, 8 data bits, n stop bits
 *           @arg USART_DATA_WIDTH_BITS_9:  9 bits word length : Start bit, 9 data bits, n stop bits
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetDataWidth(M0P_USART_TypeDef *USARTx, uint32_t u32DataWidth)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_DATA_WIDTH(u32DataWidth));

        MODIFY_REG32(USARTx->CR1, USART_CR1_M, u32DataWidth);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART data width.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_DATA_WIDTH_BITS_8:  8 bits word length : Start bit, 8 data bits, n stop bits
 *           @arg USART_DATA_WIDTH_BITS_9:  9 bits word length : Start bit, 9 data bits, n stop bits
 */
uint32_t USART_GetDataWidth(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR1, USART_CR1_M);
}

/**
 * @brief  Set USART oversampling bits.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32OversamplingBits     USART oversampling bits
 *         This parameter can be one of the following values:
 *           @arg USART_OVERSAMPLING_BITS_8:    Oversampling by 8 bits
 *           @arg USART_OVERSAMPLING_BITS_16:   Oversampling by 16 bits
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetOversmaplingBits(M0P_USART_TypeDef *USARTx, uint32_t u32OversamplingBits)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_OVERSAMPLING_BITS(u32OversamplingBits));

        MODIFY_REG32(USARTx->CR1, USART_CR1_OVER8, u32OversamplingBits);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART oversampling bits.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_OVERSAMPLING_BITS_8:    Oversampling by 8 bits
 *           @arg USART_OVERSAMPLING_BITS_16:   Oversampling by 16 bits
 */
uint32_t USART_GetOversmaplingBits(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR1, USART_CR1_OVER8);
}

/**
 * @brief  Set USART mode.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Mode                 USART mode
 *         This parameter can be one of the following values:
 *           @arg USART_MODE_UART:      UART mode
 *           @arg USART_MODE_CLKSYNC:   Clock synchronization
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetMode(M0P_USART_TypeDef *USARTx, uint32_t u32Mode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_MODE_SEL(u32Mode));

        MODIFY_REG32(USARTx->CR1, USART_CR1_MS, u32Mode);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART mode.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_MODE_UART:      UART mode
 *           @arg USART_MODE_CLKSYNC:   Clock synchronization
 */
uint32_t USART_GetMode(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR1, USART_CR1_MS);
}

/**
 * @brief  Set USART bit direction.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32BitDir               USART bit direction
 *         This parameter can be one of the following values:
 *           @arg USART_MSB:            MSB(Most Significant Bit)
 *           @arg USART_LSB:            LSB(Least Significant Bit)
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetBitDirection(M0P_USART_TypeDef *USARTx,
                                        uint32_t u32BitDir)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_SIGNIFICANT_BIT(u32BitDir));

        MODIFY_REG32(USARTx->CR1, USART_CR1_ML, u32BitDir);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART bit direction.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_MSB:            MSB(Most Significant Bit)
 *           @arg USART_LSB:            LSB(Least Significant Bit)
 */
uint32_t USART_GetBitDirection(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR1, USART_CR1_ML);
}

/**
 * @brief  Set USART start bit detect polarity.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Polarity             USART start bit detect polarity
 *         This parameter can be one of the following values:
 *           @arg USART_SB_DETECT_LOW:      Detect RX pin low level
 *           @arg USART_SB_DETECT_FALLING:  Detect RX pin falling edge
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetSbDetectPolarity(M0P_USART_TypeDef *USARTx,
                                            uint32_t u32Polarity)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_SB_DETECT_POLARITY(u32Polarity));

        MODIFY_REG32(USARTx->CR1, USART_CR1_SBS, u32Polarity);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART start bit detect polarity.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_SB_DETECT_LOW:      Detect RX pin low level
 *           @arg USART_SB_DETECT_FALLING:  Detect RX pin falling edge
 */
uint32_t USART_GetSbDetectPolarity(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR1, USART_CR1_SBS);
}

/**
 * @brief  Set USART clock mode.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32ClkMode              USART clock mode
 *         This parameter can be one of the following values:
 *           @arg USART_EXTCLK:             Select external clock source.
 *           @arg USART_INTCLK_OUTPUT:      Select internal clock source and output clock.
 *           @arg USART_INTCLK_NONE_OUTPUT: Select internal clock source and don't output clock
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetClockMode(M0P_USART_TypeDef *USARTx, uint32_t u32ClkMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_CLOCK_MODE(u32ClkMode));

        MODIFY_REG32(USARTx->CR2, USART_CR2_CLKC, u32ClkMode);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART clock mode.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_EXTCLK:             Select external clock source.
 *           @arg USART_INTCLK_OUTPUT:      Select internal clock source and output clock.
 *           @arg USART_INTCLK_NONE_OUTPUT: Select internal clock source and don't output clock
 */
uint32_t USART_GetClockMode(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR2, USART_CR2_CLKC);
}

/**
 * @brief  Set USART stop bits.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32StopBits             USART stop bits
 *         This parameter can be one of the following values:
 *           @arg USART_STOP_BITS_1:    1 stop bit
 *           @arg USART_STOP_BITS_2:    2 stop bits
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetStopBits(M0P_USART_TypeDef *USARTx, uint32_t u32StopBits)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_STOP_BITS(u32StopBits));

        MODIFY_REG32(USARTx->CR2, USART_CR2_STOP, u32StopBits);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART clock mode.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_STOP_BITS_1:    1 stop bit
 *           @arg USART_STOP_BITS_2:    2 stop bits
 */
uint32_t USART_GetStopBits(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR2, USART_CR2_STOP);
}

/**
 * @brief  Set UART half/full duplex selection.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Mode                 USART half/full duplex selection
 *         This parameter can be one of the following values:
 *           @arg USART_HALFDUPLEX_MODE:USART half-duplex mode
 *           @arg USART_FULLDUPLEX_MODE:USART full-duplex mode
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetDuplexMode(M0P_USART_TypeDef *USARTx,
                                uint32_t u32Mode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_UART_DUPLEX_MODE(u32Mode));

        MODIFY_REG32(USARTx->CR3, USART_CR3_HDSEL, u32Mode);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART half/full duplex.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_HALFDUPLEX_MODE:USART half-duplex mode
 *           @arg USART_FULLDUPLEX_MODE:USART full-duplex mode
 */
uint32_t USART_GetDuplexMode(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR3, USART_CR3_HDSEL);
}

/**
 * @brief  Set UART hardware flow control CTS/RTS selection.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32HwFlowCtrl           USART hardware flow control CTS/RTS selection
 *         This parameter can be one of the following values:
 *           @arg USART_HWFLOWCTRL_CTS: UART hardware flow control CTS mode
 *           @arg USART_HWFLOWCTRL_RTS: UART hardware flow control RTS mode
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetHwFlowCtrl(M0P_USART_TypeDef *USARTx,
                                uint32_t u32HwFlowCtrl)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_HWFLOWCTRL(u32HwFlowCtrl));

        MODIFY_REG32(USARTx->CR3, USART_CR3_CTSE, u32HwFlowCtrl);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART hardware flow control CTS/RTS selection.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_HWFLOWCTRL_CTS: UART hardware flow control CTS mode
 *           @arg USART_HWFLOWCTRL_RTS: UART hardware flow control RTS mode
 */
uint32_t USART_GetHwFlowCtrl(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->CR3, USART_CR3_CTSE);
}

/**
 * @brief  Set USART clock prescaler division.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32PrescalerDiv         USART clock prescaler division.
 *         This parameter can be one of the following values:
 *           @arg USART_CLK_PRESCALER_DIV1:     PCLK
 *           @arg USART_CLK_PRESCALER_DIV4:     PCLK/4
 *           @arg USART_CLK_PRESCALER_DIV16:    PCLK/16
 *           @arg USART_CLK_PRESCALER_DIV64:    PCLK/64
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance
 */
en_result_t USART_SetClkPrescaler(M0P_USART_TypeDef *USARTx,
                                    uint32_t u32PrescalerDiv)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx instance */
    if (IS_USART_INSTANCE(USARTx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_USART_CLOCK_PRESCALER_DIV(u32PrescalerDiv));

        MODIFY_REG32(USARTx->PR, USART_PR_PSC, u32PrescalerDiv);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get USART clock prescaler division.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @retval Returned value can be one of the following values:
 *           @arg USART_CLK_PRESCALER_DIV1:     PCLK
 *           @arg USART_CLK_PRESCALER_DIV4:     PCLK/4
 *           @arg USART_CLK_PRESCALER_DIV16:    PCLK/16
 *           @arg USART_CLK_PRESCALER_DIV64:    PCLK/64
 */
uint32_t USART_GetClkPrescaler(M0P_USART_TypeDef *USARTx)
{
    /* Check parameters */
    DDL_ASSERT(IS_USART_INSTANCE(USARTx));

    return READ_REG32_BIT(USARTx->PR, USART_PR_PSC);
}

/**
 * @brief  Set USART baudrate.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Baudrate             UART baudrate
 * @param  [in] pf32Err                 E(%) baudrate error rate
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: USARTx is invalid instance or DIV_Integer is out of range
 */
en_result_t USART_SetBaudrate(M0P_USART_TypeDef *USARTx,
                                    uint32_t u32Baudrate,
                                    float32_t *pf32Err)
{
    uint32_t B = 0ul;
    uint32_t C = 0ul;
    uint32_t OVER8 = 0ul;
    uint32_t DIV_Integer = 0ul;
    uint32_t u32Temp = 0ul;
    uint32_t u32Prescaler = 0ul;
    uint32_t u32Mode = READ_REG32_BIT(USARTx->CR1, USART_CR1_MS);
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_USART_INSTANCE(USARTx) && u32Baudrate)
    {
        u32Prescaler = READ_REG32_BIT(USARTx->PR, USART_PR_PSC);

        B = u32Baudrate;
        C = (SystemCoreClock / (1ul << (u32Prescaler * 2ul)));
        OVER8 = READ_REG32_BIT(USARTx->CR1, USART_CR1_OVER8) ? 1ul : 0ul;

        if (USART_MODE_UART == u32Mode)
        {
            /* UART Baudrate Calculation Formula */
            /* B = C / (8 * (2 - OVER8) * (DIV_Integer + 1)) */
            DIV_Integer = (C * 10ul) / (B * 8ul * (2ul - OVER8));
        }
        else
        {
            /* Clock Sync Baudrate Calculation Formula */
            /* B = C / (4 * (DIV_Integer + 1)) */
            DIV_Integer = (C * 10ul) / (B * 4ul);
        }

        if (DIV_Integer % 10ul < 5ul)
        {
            DIV_Integer -= 10ul;
        }

        DIV_Integer /= 10ul;

        if (DIV_Integer <= 0xFFul)
        {
            if (pf32Err)
            {
                if (USART_MODE_UART == u32Mode)
                {
                    /* UART Baudrate Error Calculation Formula */
                    /* E(%) = C / (8 * (2 - OVER8) * (DIV_Integer + 1) * B) - 1 */
                    u32Temp = (8ul * (2ul - OVER8) * (DIV_Integer + 1ul) * B);
                }
                else
                {
                    /* Clock Sync Baudrate Error Calculation Formula */
                    /* E(%) = C / (4 * (DIV_Integer + 1) * B) - 1 */
                    u32Temp = (4ul * (DIV_Integer + 1ul) * B);
                }

                *pf32Err = (float32_t)(((float64_t)C) / (float64_t)u32Temp) - 1.0f;
            }

            /* Set USART_BRR register bits:DIV_Integer */
            WRITE_REG32(USARTx->BRR, (DIV_Integer << USART_BRR_DIV_INTEGER_POS));
            enRet = Ok;
        }
    }

    return enRet;
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
