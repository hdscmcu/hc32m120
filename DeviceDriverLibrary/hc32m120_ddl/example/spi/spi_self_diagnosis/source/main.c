/**
 *******************************************************************************
 * @file  spi/spi_self_diagnosis/source/main.c
 * @brief Main program SPI self diagnosis for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-06       Wuze            First version
   2020-12-04       Wuze            Refined this example.
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
 * @addtogroup SPI_Self_Diagnosis
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* SPI pin definition. */
#define SPI_NSS_PORT                (GPIO_PORT_1)
#define SPI_NSS_PIN                 (GPIO_PIN_3)
#define SPI_SCK_PORT                (GPIO_PORT_1)
#define SPI_SCK_PIN                 (GPIO_PIN_4)
#define SPI_MOSI_PORT               (GPIO_PORT_2)
#define SPI_MOSI_PIN                (GPIO_PIN_0)
#define SPI_MISO_PORT               (GPIO_PORT_1)
#define SPI_MISO_PIN                (GPIO_PIN_2)

/* Interrupt flag bit mask. */
#define SPI_IRQ_PARITY_ERROR_OCCURRED   (1ul << 0u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SpiConfig(void);
static void SpiIrqConfig(void);
static void SpiParityTx(uint8_t u8TxData);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint32_t u32SpiIrqFlag = 0u;
static stc_spi_init_t m_stcInit;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of spi_self_diagnosis project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint8_t u8Temp;

    /* The system clock is set to HRC8HMz by default. */

    /* Configures SPI. */
    SpiConfig();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        m_stcInit.u32Pate = SPI_PATE_ENABLE;
        SpiConfig();
        SpiParityTx(0x56u);
        if (u32SpiIrqFlag & SPI_IRQ_PARITY_ERROR_OCCURRED)
        {
            // !!!SPI parity check function exception.
            while (1u)
            {
                ;
            }
        }

        SpiParityTx(0x57u);
        if ((u32SpiIrqFlag & SPI_IRQ_PARITY_ERROR_OCCURRED) == 0U)
        {
            // !!!SPI parity check function exception.
            while (1u)
            {
                ;
            }
        }

        /* Disable parity check self diagnosis. */
        m_stcInit.u32Pate = SPI_PATE_DISABLE;
        SpiConfig();
        SpiParityTx(0x56u);
        if (u32SpiIrqFlag & SPI_IRQ_PARITY_ERROR_OCCURRED)
        {
            // !!!SPI parity check function exception.
            while (1u)
            {
                ;
            }
        }

        u8Temp = (uint8_t)SPI_ReadDataReg();
        if (u8Temp == (uint8_t)0x57u)
        {
            // !!!SPI parity check function exception.
            while (1u)
            {
                ;
            }
        }

        /* Parity check is success. The following statement is only used for breakpoint. */
        SpiParityTx(0x56u);
    }
}

/**
 * @brief  SPI configuration, including initialization, pin configuration
 *         and interrupt configuration.
 * @param  None
 * @retval None
 */
static void SpiConfig(void)
{
    /* The SPI register can be written only after the SPI peripheral is enabled. */
    CLK_FcgPeriphClockCmd(CLK_FCG_SPI, Enable);

    SPI_StructInit(&m_stcInit);
    m_stcInit.u32Splpbk = SPI_SPLPBK_MOSI;
    m_stcInit.u32Pate   = SPI_PATE_ENABLE;
    m_stcInit.u32Parity = SPI_PARITY_EVEN;

    /* Initializes SPI. */
    SPI_Init(&m_stcInit);

    /* Set the pins to SPI function. */
    GPIO_SetFunc(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_FUNC_7_SPI);
    GPIO_SetFunc(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_FUNC_7_SPI);
    GPIO_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_FUNC_7_SPI);
    GPIO_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, GPIO_FUNC_7_SPI);

    /* SPI interrupt configuration. */
    SpiIrqConfig();

    /* Enable SPI function. */
    SPI_FunctionCmd(Enable);
}

/**
 * @brief  SPI interrupt configuration.
 * @param  None
 * @retval None
 * @note   All SPI interrupts can be configured as independent interrupt or shared interrupt.
 *         -INT_SPI_SPEI: ERROR interrupt.
 *                       --Independent vec[Int008_IRQn, Int009_IRQn]
 *                       --Share vec [Int024_IRQn]
 *         -INT_SPI_SPRI: RX buffer full interrupt.
 *                       --Independent vec[Int014_IRQn, Int015_IRQn]
 *                       --Share vec[Int027_IRQn]
 *         -INT_SPI_SPII: IDLE interrupt.
 *                       --Independent vec[Int016_IRQn, Int017_IRQn]
 *                       --Share vec[Int028_IRQn]
 *         -INT_SPI_SPTI: TX buffer empty interrupt.
 *                       --Independent vec[Int022_IRQn, Int023_IRQn]
 *                       --Share vec[Int031_IRQn]
 */
static void SpiIrqConfig(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;

    /* Configures error interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_SPI_SPEI;
    stcIrqRegiConf.enIRQn      = Int008_IRQn;
    stcIrqRegiConf.pfnCallback = &SpiErr_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Enable the interrupt. */
    SPI_IntCmd(SPI_INT_ERROR, Enable);
}

/**
 * @brief  Send data for self diagnosis of parity check.
 * @param  [in]  u8TxData       The data to be sent.
 * @retval None
 */
static void SpiParityTx(uint8_t u8TxData)
{
    u32SpiIrqFlag = 0u;
    SPI_Transmit(&u8TxData, 1u);
}

/**
 * @brief  SPI error interrupt callback function.
 * @param  None
 * @retval None
 */
void SpiErr_IrqHandler(void)
{
    if (SPI_GetFlag(SPI_FLAG_OVERLOAD) == Set)
    {
        SPI_ClearFlag(SPI_FLAG_OVERLOAD);
    }

    if (SPI_GetFlag(SPI_FLAG_MODE_FAULT) == Set)
    {
        SPI_ClearFlag(SPI_FLAG_MODE_FAULT);
    }

    if (SPI_GetFlag(SPI_FLAG_PARITY_ERROR) == Set)
    {
        SPI_ClearFlag(SPI_FLAG_PARITY_ERROR);
        u32SpiIrqFlag |= SPI_IRQ_PARITY_ERROR_OCCURRED;
    }

    if (SPI_GetFlag(SPI_FLAG_UNDERLOAD) == Set)
    {
        SPI_ClearFlag(SPI_FLAG_UNDERLOAD);
        SPI_FunctionCmd(Enable);
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
