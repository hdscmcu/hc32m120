/**
 *******************************************************************************
 * @file  hc32m120_utility.c
 * @brief This file provides utility functions for DDL.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-28       Yangjp          First version
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
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_UTILITY UTILITY
 * @brief DDL Utility Driver
 * @{
 */

#if (DDL_UTILITY_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
#if (DDL_PRINT_ENABLE == DDL_ON)
static en_result_t SetUartBaudrate(M0P_USART_TypeDef *USARTx,
                                        uint32_t u32Baudrate);
#endif

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup UTILITY_Local_Variables UTILITY Local Variables
 * @{
 */

static uint32_t u32TickStep = 0ul;
__IO static uint32_t u32TickCount = 0ul;

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup UTILITY_Global_Functions UTILITY Global Functions
 * @{
 */

/**
 * @brief Delay function, delay 1ms approximately
 * @param [in] u32Cnt                   ms
 * @retval None
 */
void DDL_Delay1ms(uint32_t u32Cnt)
{
    __IO uint32_t i = 0ul;
    uint32_t u32Cyc = 0ul;

    u32Cyc = SystemCoreClock;
    u32Cyc = u32Cyc / 10000ul;
    while (u32Cnt-- > 0ul)
    {
        i = u32Cyc;
        while (i-- > 0ul)
        {
            ;
        }
    }
}

/**
 * @brief This function Initializes the interrupt frequency of the SysTick.
 * @param [in] u32Freq                  SysTick interrupt frequency.Value range 1 to 1000.
 * @retval An en_result_t enumeration value:
 *           - Ok: SysTick Initializes succeed
 *           - Error: SysTick Initializes failed
 */
__WEAKDEF en_result_t SysTick_Init(uint32_t u32Freq)
{
    en_result_t enRet = Error;

    if (u32Freq)
    {
        u32TickStep = 1000ul / u32Freq;
        /* Configure the SysTick interrupt */
        if (0ul == SysTick_Config(SystemCoreClock / u32Freq))
        {
            enRet = Ok;
        }
    }

    return enRet;
}

/**
 * @brief This function provides minimum delay (in milliseconds).
 * @param [in] u32Delay                 Delay specifies the delay time.
 * @retval None
 */
__WEAKDEF void SysTick_Delay(uint32_t u32Delay)
{
    uint32_t tickStart = SysTick_GetTick();
    uint32_t tickEnd = u32Delay;

    /* Add a freq to guarantee minimum wait */
    if (tickEnd < 0xFFFFFFFFul)
    {
        tickEnd += u32TickStep;
    }

    while ((SysTick_GetTick() - tickStart) < tickEnd)
    {
    }
}

/**
 * @brief This function is called to increment a global variable "u32TickCount".
 * @note  This variable is incremented in SysTick ISR.
 * @param None
 * @retval None
 */
__WEAKDEF void SysTick_IncTick(void)
{
    u32TickCount += u32TickStep;
}

/**
 * @brief Provides a tick value in millisecond.
 * @param None
 * @retval Tick value
 */
__WEAKDEF uint32_t SysTick_GetTick(void)
{
    return u32TickCount;
}

/**
 * @brief Suspend SysTick increment.
 * @param None
 * @retval None
 */
__WEAKDEF void SysTick_Suspend(void)
{
    /* Disable SysTick Interrupt */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief Resume SysTick increment.
 * @param None
 * @retval None
 */
__WEAKDEF void SysTick_Resume(void)
{
    /* Enable SysTick Interrupt */
    SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;
}

#ifdef __DEBUG
/**
 * @brief DDL assert error handle function
 * @param [in] file                     Point to the current assert the wrong file.
 * @param [in] line                     Point line assert the wrong file in the current.
 * @retval None
 */
__WEAKDEF void DDL_AssertHandler(const uint8_t *file, int16_t line)
{
    /* Users can re-implement this function to print information */
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
#endif

    while (1)
    {
        ;
    }
}
#endif /* __DEBUG */

#if (DDL_PRINT_ENABLE == DDL_ON)
/**
 * @brief  Re-target fputc function.
 * @param  [in] ch
 * @param  [in] f
 * @retval int32_t
 */
int32_t fputc(int32_t ch, FILE *f)
{
    /* Wait TX data register empty */
    while (!READ_REG32_BIT(M0P_USART2->SR, USART_SR_TXE))
    {
        ;
    }

    WRITE_REG32(M0P_USART2->DR,  ((uint32_t)ch & 0x01FFul));

    return (ch);
}

/**
 * @brief  Initialize UART for debug printf function
 * @param  None
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - Error: Initialize unsuccessfully
 */
en_result_t DDL_UartInit(void)
{
    en_result_t enRet = Error;

    /* Configure USART TX pin. */
    WRITE_REG16(M0P_PORT->PWPR, 0xA501u);  /* Unlock */
    MODIFY_REG16(M0P_PORT->PCR12, PORT_PCR_FSEL, (0x05ul << PORT_PCR_FSEL_POS));  /* P12: USART2_TX */
    WRITE_REG16(M0P_PORT->PWPR, 0xA500u);  /* Lock */

    /* Enable USART1 function clock gate */
    WRITE_REG16(M0P_PWC->FPRC, 0xA501u);  /* Unlock */
    CLEAR_REG32_BIT(M0P_CMU->FCG, CMU_FCG_UART2);
    WRITE_REG16(M0P_PWC->FPRC, 0xA500u);  /* Lock */

    /* Disbale TX/RX && clear interrupt flag */
    CLEAR_REG32_BIT(M0P_USART2->CR1, (USART_CR1_TE | USART_CR1_RE));

    /***************************************************************************
     * Configure UART
     ***************************************************************************
     * Baud rate: 115200
     * Bit direction: LSB
     * Data bits: 8
     * Stop bits: 1
     * Parity: None
     * Sampling bits: 8
     **************************************************************************/

    /* Set CR1 */
    MODIFY_REG32(M0P_USART2->CR1,
                 (USART_CR1_SLME | USART_CR1_PS | USART_CR1_PCE |                \
                  USART_CR1_M | USART_CR1_OVER8 | USART_CR1_MS  |                \
                  USART_CR1_ML | USART_CR1_NFE | USART_CR1_SBS),                 \
                 USART_CR1_OVER8);

    /* Set CR2: reset value */
    WRITE_REG32(M0P_USART2->CR2, 0x00ul);

    /* Set CR3: reset value */
    WRITE_REG32(M0P_USART2->CR3, 0x00ul);

    /* Set baudrate */
    if (Ok == SetUartBaudrate(M0P_USART2, 115200u))
    {
        /* Enable TX function */
        SET_REG32_BIT(M0P_USART2->CR1, USART_CR1_TE);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set USART baudrate.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg M0P_USART1:           USART unit 1 instance register base
 *           @arg M0P_USART2:           USART unit 2 instance register base
 *           @arg M0P_USART3:           USART unit 3 instance register base
 * @param  [in] u32Baudrate             UART baudrate
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidMode: USART instance mode is not UART
 *           - ErrorInvalidParameter: u32Baudrate is invalid or USARTx is valid USART instance
 */
static en_result_t SetUartBaudrate(M0P_USART_TypeDef *USARTx,
                                        uint32_t u32Baudrate)
{
    uint32_t B = 0ul;
    uint32_t C = 0ul;
    uint32_t OVER8 = 0ul;
    uint32_t DIV_Integer = 0ul;
    uint32_t u32Prescaler = 0ul;
    uint32_t u32CR1 = 0ul;
    en_result_t enRet = ErrorInvalidParameter;

    if (u32Baudrate)
    {
        enRet = ErrorInvalidMode;
        u32CR1 = READ_REG32(USARTx->CR1);
        if (!(u32CR1 & USART_CR1_MS))
        {
            B = u32Baudrate;
            OVER8 = (u32CR1 & USART_CR1_OVER8) ? 1ul : 0ul;

            for (u32Prescaler = 0ul; u32Prescaler <= USART_PR_PSC; u32Prescaler++)
            {
                C = (SystemCoreClock / (1ul << (u32Prescaler * 2ul)));

                /* UART Mode Calculation Formula */
                /* B = C / (8 * (2 - OVER8) * (DIV_Integer + 1)) */
                DIV_Integer = (C * 10ul) / (B * 8ul * (2ul - OVER8));

                /* Calibrate rounding error */
                if (DIV_Integer % 10ul < 5ul)
                {
                    DIV_Integer -= 10ul;
                }

                if (DIV_Integer <= 2550ul)    /* 2550 = 0xFF * 10 */
                {
                    break;
                }
            }

            /* Check validation : DIV_Integer */
            if (DIV_Integer <= 2550ul)         /* 2550 = 0xFF * 10 */
            {
                DIV_Integer /= 10ul;
                /* Set clock prescaler */
                WRITE_REG32(USARTx->PR, u32Prescaler);

                /* Set USART_BRR register bits:DIV_Integer */
                WRITE_REG32(USARTx->BRR, (DIV_Integer << USART_BRR_DIV_INTEGER_POS));
                enRet = Ok;
            }
        }
    }

    return enRet;
}

#endif /* DDL_PRINT_ENABLE */

/**
 * @}
 */

#endif /* DDL_UTILITY_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
