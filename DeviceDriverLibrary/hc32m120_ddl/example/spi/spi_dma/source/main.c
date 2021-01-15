/**
 *******************************************************************************
 * @file  spi/spi_dma/source/main.c
 * @brief Main program SPI DMA for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-10       Wuze            First version
   2019-10-21       Wuze            Modified DMA_ChannelCmd to DMA_ChannelEnable to enable the DMA channel.
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
 * @addtogroup SPI_Dma
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
#define SPI_BUFFER_LENGTH           (8ul)

/* Command from the master. */
#define SPI_WRITE_SLAVE             (0x51u)             /*!< Customer definition. */
#define SPI_READ_SLAVE              (0x56u)             /*!< Customer definition. */
#define SPI_DUMMY_DATA              (0xFFu)

/* SPI DMA definition. */
#define SPI_DMA_DATAWIDTH           (DMA_DATAWIDTH_8BIT)
#define SPI_DMA_BLKSIZE             (1ul)

#define SPI_DMA_RX_TRNCNT           (SPI_BUFFER_LENGTH)
#define SPI_DMA_RX_CH               (DMA_CHANNEL_0)
#define SPI_DMA_RX_TRIG             (EVT_SPI_SPRI)
#define SPI_DMA_RX_SRC_ADDR         ((uint32_t)(&M0P_SPI->DR))
#define SPI_DMA_RX_DEST_ADDR        ((uint32_t)(&m_au8SpiRxBuf[0u]))
#define SPI_DMA_RX_LLP_MODE         (DMA_LLP_WAIT)
#define SPI_DMA_RX_LLP_POS          (DMA_CH0CTL0_LLP_POS - 2u)

#define SPI_DMA_TX_TRNCNT           (SPI_BUFFER_LENGTH)
#define SPI_DMA_TX_CH               (DMA_CHANNEL_1)
#define SPI_DMA_TX_TRIG             (EVT_SPI_SPTI)
#define SPI_DMA_TX_SRC_ADDR         ((uint32_t)(&m_au8SpiTxBuf[0u]))
#define SPI_DMA_TX_DEST_ADDR        ((uint32_t)(&M0P_SPI->DR))
#define SPI_DMA_TX_LLP_MODE         (DMA_LLP_WAIT)
#define SPI_DMA_TX_LLP_POS          (DMA_CH1CTL0_LLP_POS - 2u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void DmaConfig(void);
static void DmaIrqConfig(void);
static void SpiConfig(void);
static void SpiIrqConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t u8RxFlag = 0U;

static uint8_t m_au8SpiRxBuf[SPI_BUFFER_LENGTH];
static uint8_t m_au8SpiTxBuf[SPI_BUFFER_LENGTH] = \
{
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
};
static stc_dma_llp_descriptor_t m_stcLlpDescTx;
static stc_dma_llp_descriptor_t m_stcLlpDescRx;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of spi_dma project
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

    /* Configures DMA for SPI. */
    DmaConfig();

    /* Configures SPI. */
    SpiConfig();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        if (u8RxFlag != 0u)
        {
            u8RxFlag = 0u;

            if (m_au8SpiRxBuf[0u] == SPI_WRITE_SLAVE)
            {
                // TODO: Use the data from the master.
            }

            if (m_au8SpiRxBuf[0u] == SPI_READ_SLAVE)
            {
                /* Prepare data that needs to be sent to the master. */
                m_au8SpiTxBuf[0u]++;
                m_au8SpiTxBuf[1u]++;
                m_au8SpiTxBuf[2u]++;
                m_au8SpiTxBuf[3u]++;
                m_au8SpiTxBuf[4u]++;
                m_au8SpiTxBuf[5u]++;
                m_au8SpiTxBuf[6u]++;
                m_au8SpiTxBuf[7u]++;
            }

            SPI_FunctionCmd(Disable);
            DMA_ChannelEnable(SPI_DMA_TX_CH);
            SPI_FunctionCmd(Enable);
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
 * @brief  DMA configuration for SPI.
 * @param  None
 * @retval None
 */
void DmaConfig(void)
{
    stc_dma_ch_cfg_t stcDmaChCfg;

    CLK_FcgPeriphClockCmd(CLK_FCG_DMA, Enable);

    /* Descriptor RX. This structure variable is used to reload the configuration of the RX channel. */
    m_stcLlpDescRx.SARx    = SPI_DMA_RX_SRC_ADDR;
    m_stcLlpDescRx.DARx    = SPI_DMA_RX_DEST_ADDR;
    m_stcLlpDescRx.CHxCTL0 = SPI_DMA_BLKSIZE                                        | \
                             ((uint32_t)(SPI_DMA_RX_TRNCNT << DMA_CH0CTL0_CNT_POS)) | \
                             ((uint32_t)&m_stcLlpDescRx << SPI_DMA_RX_LLP_POS)      | \
                             DMA_LLP_ENABLE                                         | \
                             SPI_DMA_RX_LLP_MODE                                    | \
                             SPI_DMA_DATAWIDTH;
    m_stcLlpDescRx.CHxCTL1 = DMA_DESADDRINC_INC | DMA_SRCADDRINC_FIX;

    /* Config DMA */
    stcDmaChCfg.u32DataWidth   = SPI_DMA_DATAWIDTH;
    stcDmaChCfg.u32BlockSize   = SPI_DMA_BLKSIZE;
    stcDmaChCfg.u32TransferCnt = SPI_DMA_RX_TRNCNT;
    stcDmaChCfg.u32SrcAddr     = SPI_DMA_RX_SRC_ADDR;
    stcDmaChCfg.u32DesAddr     = SPI_DMA_RX_DEST_ADDR;
    stcDmaChCfg.u32SrcInc      = DMA_SRCADDRINC_FIX;
    stcDmaChCfg.u32DesInc      = DMA_DESADDRINC_INC;
    DMA_ChannelCfg(SPI_DMA_RX_CH, &stcDmaChCfg);
    DMA_LlpInit(SPI_DMA_RX_CH, SPI_DMA_RX_LLP_MODE, (uint32_t)&m_stcLlpDescRx);

    /* Descriptor TX. This structure variable is used to reload the configuration of the TX channel. */
    m_stcLlpDescTx.SARx    = SPI_DMA_TX_SRC_ADDR;
    m_stcLlpDescTx.DARx    = SPI_DMA_TX_DEST_ADDR;
    m_stcLlpDescTx.CHxCTL0 = SPI_DMA_BLKSIZE                                        | \
                             ((uint32_t)(SPI_DMA_TX_TRNCNT << DMA_CH1CTL0_CNT_POS)) | \
                             ((uint32_t)&m_stcLlpDescTx << SPI_DMA_TX_LLP_POS)      | \
                             DMA_LLP_ENABLE                                         | \
                             SPI_DMA_TX_LLP_MODE                                    | \
                             SPI_DMA_DATAWIDTH;
    m_stcLlpDescTx.CHxCTL1 = DMA_DESADDRINC_FIX | DMA_SRCADDRINC_INC;

    stcDmaChCfg.u32TransferCnt = SPI_DMA_TX_TRNCNT;
    stcDmaChCfg.u32SrcAddr     = SPI_DMA_TX_SRC_ADDR;
    stcDmaChCfg.u32DesAddr     = SPI_DMA_TX_DEST_ADDR;
    stcDmaChCfg.u32SrcInc      = DMA_SRCADDRINC_INC;
    stcDmaChCfg.u32DesInc      = DMA_DESADDRINC_FIX;
    DMA_ChannelCfg(SPI_DMA_TX_CH, &stcDmaChCfg);
    DMA_LlpInit(SPI_DMA_TX_CH, SPI_DMA_TX_LLP_MODE, (uint32_t)&m_stcLlpDescTx);

    /* Enable AOS clock */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);

    /* Set DMA trigger source */
    DMA_SetTriggerSrc(SPI_DMA_RX_CH, SPI_DMA_RX_TRIG);
    DMA_SetTriggerSrc(SPI_DMA_TX_CH, SPI_DMA_TX_TRIG);

    DMA_ClearCplFlag(SPI_DMA_RX_CH, DMA_FLAG_TC);
    DMA_ClearCplFlag(SPI_DMA_TX_CH, DMA_FLAG_TC);

    DmaIrqConfig();

    /* Enable DMA channel. */
    DMA_ChannelEnable(SPI_DMA_RX_CH);
    DMA_ChannelEnable(SPI_DMA_TX_CH);

    /* Enable DMA. */
    DMA_Cmd(Enable);
}

/**
 * @brief  Interrupt configuration.
 * @param  None
 * @retval None
 */
static void DmaIrqConfig(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;

    /* Configures error interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_DAM_1_TC0;
    stcIrqRegiConf.enIRQn      = Int009_IRQn;
    stcIrqRegiConf.pfnCallback = &DmaTc0_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    stcIrqRegiConf.enIntSrc    = INT_DAM_2_TC0;
    stcIrqRegiConf.enIRQn      = Int010_IRQn;
    stcIrqRegiConf.pfnCallback = &DmaTc1_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    DMA_CplIrqCmd(SPI_DMA_RX_CH, DMA_IRQ_TC, Enable);
    DMA_CplIrqCmd(SPI_DMA_TX_CH, DMA_IRQ_TC, Enable);
}

/**
 * @brief  DMA TC0 IRQ handler.
 * @param  None
 * @retval None
 */
void DmaTc0_IrqHandler(void)
{
    DMA_ClearCplFlag(SPI_DMA_RX_CH, DMA_FLAG_TC);
    u8RxFlag = 1u;
}

/**
 * @brief  DMA TC1 IRQ handler.
 * @param  None
 * @retval None
 */
void DmaTc1_IrqHandler(void)
{
    DMA_ChannelDisable(SPI_DMA_TX_CH);
    DMA_ClearCplFlag(SPI_DMA_TX_CH, DMA_FLAG_TC);
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
    stcInit.u32MasterSlave    = SPI_SLAVE;
    stcInit.u32WireMode       = SPI_WIRE_MODE;
    stcInit.u32NssActiveLevel = SPI_NSS_ACTIVE;
    stcInit.u32SpiMode        = SPI_SPI_MODE;

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

    /* Configures error interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_SPI_SPEI;
    stcIrqRegiConf.enIRQn      = Int008_IRQn;
    stcIrqRegiConf.pfnCallback = &SpiErr_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Enable the interrupts. */
    SPI_IntCmd(SPI_INT_ERROR, Enable);
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
