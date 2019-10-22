/**
 *******************************************************************************
 * @file  spi/spi_slave_interrupt/source/main.c
 * @brief Main program SPI slave interrupt for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-06       Wuze            First version
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
 * @addtogroup SPI_Slave_Interrupt
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* SPI pin group definition. */
#define SPI_PIN_GROUP_A             (1u)
#define SPI_PIN_GROUP_B             (2u)
#define SPI_PIN_GROUP               (SPI_PIN_GROUP_A)

#if (SPI_PIN_GROUP == SPI_PIN_GROUP_A)
#define SPI_NSS_PORT                (GPIO_PORT_1)
#define SPI_NSS_PIN                 (GPIO_PIN_3)
#define SPI_SCK_PORT                (GPIO_PORT_1)
#define SPI_SCK_PIN                 (GPIO_PIN_4)
#define SPI_MOSI_PORT               (GPIO_PORT_1)
#define SPI_MOSI_PIN                (GPIO_PIN_1)
#define SPI_MISO_PORT               (GPIO_PORT_1)
#define SPI_MISO_PIN                (GPIO_PIN_2)
#elif (SPI_PIN_GROUP == SPI_PIN_GROUP_B)
#define SPI_NSS_PORT                (GPIO_PORT_2)
#define SPI_NSS_PIN                 (GPIO_PIN_2)
#define SPI_SCK_PORT                (GPIO_PORT_2)
#define SPI_SCK_PIN                 (GPIO_PIN_3)
#define SPI_MOSI_PORT               (GPIO_PORT_2)
#define SPI_MOSI_PIN                (GPIO_PIN_0)
#define SPI_MISO_PORT               (GPIO_PORT_2)
#define SPI_MISO_PIN                (GPIO_PIN_1)
#else
#error "SPI pin group not exists."
#endif // #if (SPI_PIN_GROUP == SPI_PIN_GROUP_A)

/* SPI wire mode definition. */
#define SPI_APP_3_WIRE              (3u)
#define SPI_APP_4_WIRE              (4u)
#define SPI_APP_X_WIRE              (SPI_APP_4_WIRE)

#if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)
#define SPI_WIRE_MODE               (SPI_WIRE_4)
#define SPI_SPI_MODE                (SPI_MODE_1)
#else
#define SPI_WIRE_MODE               (SPI_WIRE_3)
/* CPHA CAN NOT be zero while in 3-wire slave mode.
   Only SPI_MODE_1 and SPI_MODE_3 can be used in 3-wire slave mode. */
#define SPI_SPI_MODE                (SPI_MODE_1)
#endif // #if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)


/* SPI NSS pin active level definition. */
#define SPI_NSS_ACTIVE              (SPI_NSS_ACTIVE_LOW)


/* SPI data buffer size definition. */
#define SPI_BUFFER_LENGTH           (8u)
#define SPI_IDLE_TIME               (400u)              /*!< Customer definition. */

/* Command from the master. */
#define SPI_WRITE_SLAVE             (0x51u)             /*!< Customer definition. */
#define SPI_READ_SLAVE              (0x56u)             /*!< Customer definition. */
#define SPI_DUMMY_DATA              (0xFFu)

/* Share interrupt definition. */
// #define SHARE_INTERRUPT

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void SpiConfig(void);
static void SpiIrqConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static __IO uint32_t m_u32RxIdle    = 0u;
static __IO uint8_t m_u8RxStart     = 0u;
static __IO uint8_t m_u8RxDataCount = 0u;
static __IO uint8_t m_u8TxDataCount = 0u;
static __IO uint8_t m_au8SpiRxBuf[SPI_BUFFER_LENGTH];   /*!< All of the data is valid. */
static __IO uint8_t m_au8SpiTxBuf[SPI_BUFFER_LENGTH] =  /*!< Valid data starts at offset 0 and length is SPI_BUFFER_LENGTH-2. */
{
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of spi_slave_interrupt project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The maximum transfer rate of the slave is PCLK/6.
       The SPI clock of the slave is from the master, so the SCK frequency
       of the master SPI is at most PCLK/6. */

    /* Configure the system clock to HRC32MHz. */
    SystemClockConfig();

    /* Configures SPI. */
    SpiConfig();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        if (m_u8RxStart)
        {
            if (++m_u32RxIdle >= SPI_IDLE_TIME)
            {
                if (m_au8SpiRxBuf[0u] == SPI_WRITE_SLAVE)
                {
                    // TODO: Use the data from the master.
                }

                if (m_au8SpiRxBuf[0u] == SPI_READ_SLAVE)
                {
                    /* Prepare data that needs to be sent to the master.
                       Valid data starts at offset 0 and length is SPI_BUFFER_LENGTH-2. */
                    m_au8SpiTxBuf[0u]++;
                    m_au8SpiTxBuf[1u]++;
                    m_au8SpiTxBuf[2u]++;
                    m_au8SpiTxBuf[3u]++;
                    m_au8SpiTxBuf[4u]++;
                    m_au8SpiTxBuf[5u]++;
                }
                m_u32RxIdle = 0u;
                m_u8RxStart = 0u;
                m_u8RxDataCount = 0u;
                m_u8TxDataCount = 0u;
            }
        }
    }
}

/**
 * @brief  Configures a new system clock -- HRC32MHz.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    /* Set EFM read latency when system clock greater than 24MHz. */
    EFM_SetLatency(EFM_LATENCY_1);

    /* Configure the system clock to HRC32MHz. */
    CLK_HRCInit(CLK_HRC_ON, CLK_HRCFREQ_32);
}

/**
 * @brief  SPI configuration, including initialization, pin configuration
 *         and interrupt configuration.
 * @param  None
 * @retval None
 */
static void SpiConfig(void)
{
    stc_spi_init_t stcInit;

    /* Set a default value. */
    SPI_StructInit(&stcInit);

    /* User configuration value. */
    stcInit.u32MasterSlave       = SPI_SLAVE;
    stcInit.u32WireMode          = SPI_WIRE_MODE;
    stcInit.u32NssActiveLevel    = SPI_NSS_ACTIVE;
    stcInit.u32SpiMode           = SPI_SPI_MODE;

    /* The SPI register can be written only after the SPI peripheral is enabled. */
    CLK_FcgPeriphClockCmd(CLK_FCG_SPI, Enable);

    /* Initializes SPI. */
    SPI_Init(&stcInit);

    /* Set the pins to SPI function. */
#if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)
    GPIO_SetFunc(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_FUNC_7_SPI);
#endif // #if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)
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
 *         INT_SPI_SPEI: ERROR interrupt,
 *                       Independent vec[Int008_IRQn, Int009_IRQn]
 *                       Share vec[Int024_IRQn]
 *         INT_SPI_SPRI: RX buffer full interrupt,
 *                       Independent vec[Int014_IRQn, Int015_IRQn]
 *                       Share vec[Int027_IRQn]
 *         INT_SPI_SPII: IDLE interrupt,
 *                       Independent vec[Int016_IRQn, Int017_IRQn]
 *                       Share vec[Int028_IRQn]
 *         INT_SPI_SPTI: TX buffer empty interrupt,
 *                       Independent vec[Int022_IRQn, Int023_IRQn]
 *                       Share vec[Int031_IRQn]
 */
static void SpiIrqConfig(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;

    /* Configures RX interrupt.
       The following 2 configurations of interrupt are both valid. */
#ifdef SHARE_INTERRUPT
    /* Share interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_SPI_SPRI;
    stcIrqRegiConf.enIRQn      = Int027_IRQn;
    INTC_ShareIrqCmd(stcIrqRegiConf.enIntSrc, Enable);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
#else
    /* Independent interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_SPI_SPRI;
    stcIrqRegiConf.enIRQn      = Int014_IRQn;
    stcIrqRegiConf.pfnCallback = &SpiRxEnd_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
#endif // #ifdef SHARE_INTERRUPT

    /* Configures TX interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_SPI_SPTI;
    stcIrqRegiConf.enIRQn      = Int022_IRQn;
    stcIrqRegiConf.pfnCallback = &SpiTxEmpt_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Configures error interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_SPI_SPEI;
    stcIrqRegiConf.enIRQn      = Int008_IRQn;
    stcIrqRegiConf.pfnCallback = &SpiErr_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Enable the interrupts. */
    SPI_IntCmd(SPI_INT_ERROR | SPI_INT_RX_BUFFER_FULL | SPI_INT_TX_BUFFER_EMPTY, Enable);
}

/**
 * @brief  SPI RX buffer full interrupt callback function. Read and save the data received from master via MOSI.
 * @param  None
 * @retval None
 */
void SpiRxEnd_IrqHandler(void)
{
    if (SPI_GetFlag(SPI_FLAG_RX_BUFFER_FULL) == Set)
    {
        m_u8RxStart = 1u;
        m_u32RxIdle = 0u;
        if (m_u8RxDataCount < SPI_BUFFER_LENGTH)
        {
            m_au8SpiRxBuf[m_u8RxDataCount] = (uint8_t)SPI_ReadDataReg();
            m_u8RxDataCount++;
        }
    }
}

/**
 * @brief  SPI TX buffer empty interrupt callback function. Load the data that will be sent to master via MISO.
 * @param  None
 * @retval None
 */
void SpiTxEmpt_IrqHandler(void)
{
    if (SPI_GetFlag(SPI_FLAG_TX_BUFFER_EMPTY) == Set)
    {
        SPI_WriteDataReg((uint32_t)(m_au8SpiTxBuf[m_u8TxDataCount++]));
        if (m_u8TxDataCount >= SPI_BUFFER_LENGTH)
        {
            m_u8TxDataCount = 0u;
        }
    }
}

/**
 * @brief  SPI error interrupt callback function.
 * @param  None
 * @retval None
 */
void SpiErr_IrqHandler(void)
{
    uint32_t u32Data;

    if (SPI_GetFlag(SPI_FLAG_OVERLOAD) == Set)
    {
        u32Data  = (uint8_t)SPI_ReadDataReg();
        u32Data |= SPI_FLAG_OVERLOAD;
        SPI_ClearFlag(u32Data & SPI_FLAG_OVERLOAD);
    }

    if (SPI_GetFlag(SPI_FLAG_MODE_FAULT) == Set)
    {
        SPI_ClearFlag(SPI_FLAG_MODE_FAULT);
    }

    if (SPI_GetFlag(SPI_FLAG_PARITY_ERROR) == Set)
    {
        SPI_ClearFlag(SPI_FLAG_PARITY_ERROR);
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
