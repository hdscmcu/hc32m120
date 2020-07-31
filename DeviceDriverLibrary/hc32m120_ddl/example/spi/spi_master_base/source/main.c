/**
 *******************************************************************************
 * @file  spi/spi_master_base/source/main.c
 * @brief Main program SPI master base for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-05       Wuze            First version
   2020-06-24       Wuze            Changed default mode to master 3-wire mode.
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
 * @addtogroup SPI_Master_Base
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Slave test definition. */
//#define SLAVE_TEST

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
#define SPI_APP_X_WIRE              (SPI_APP_3_WIRE)

#if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)
#define SPI_WIRE_MODE               (SPI_WIRE_4)
#define SPI_SPI_MODE                (SPI_MODE_1)    /*!< Depends on your application. */
#else
#define SPI_WIRE_MODE               (SPI_WIRE_3)
#ifdef SLAVE_TEST
#define SPI_SPI_MODE                (SPI_MODE_1)    /*!< HC32M120: Only SPI_MODE_1 and SPI_MODE_3 can be used in 3-wire slave mode. */
#else
#define SPI_SPI_MODE                (SPI_MODE_1)    /*!< Depends on your application. */
#endif
#define SPI_APP_CUSTOM_NSS
#endif // #if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)

#ifdef SPI_APP_CUSTOM_NSS
#define SPI_CUSTOM_NSS_PORT         (SPI_NSS_PORT)
#define SPI_CUSTOM_NSS_PIN          (SPI_NSS_PIN)

//#define SPI_APP_RECEIVE_WHILE_TRANSMIT
#endif // #ifdef SPI_APP_CUSTOM_NSS


/* SPI communication mode definition */
#define SPI_APP_SEND_ONLY           (1u)        /*!< Send only. */
#define SPI_APP_FULL_DUPLEX         (2u)        /*!< Send and receive. */
#ifdef SLAVE_TEST
#define SPI_APP_TRANS_MODE          (SPI_APP_FULL_DUPLEX)
#else
#define SPI_APP_TRANS_MODE          (SPI_APP_FULL_DUPLEX)
#endif

#if (SPI_APP_TRANS_MODE == SPI_APP_FULL_DUPLEX)
#define SPI_TRANS_MODE              (SPI_FULL_DUPLEX)
#else
#define SPI_TRANS_MODE              (SPI_SEND_ONLY)
#endif // #if (SPI_APP_TRANS_MODE == SPI_APP_FULL_DUPLEX)


/* Baud rate division definition. */
#ifdef SLAVE_TEST
/* The maximum transmission baud rate of the slave is 6 divisions of its PCLK.
   Master baud rate depends on slave PCLK frequency. */
#define SPI_BR_DIV                  (SPI_BR_DIV_8)
#else
#define SPI_BR_DIV                  (SPI_BR_DIV_8)
#endif


/* SPI NSS pin active level definition. */
#define SPI_NSS_ACTIVE_LO           (0u)
#define SPI_NSS_ACTIVE_HI           (1u)
#define SPI_NSS_ACTIVE_LEVEL        (SPI_NSS_ACTIVE_LO)

#if (SPI_NSS_ACTIVE_LEVEL == SPI_NSS_ACTIVE_LO)
#define SPI_NSS_ACTIVE              (SPI_NSS_ACTIVE_LOW)
#else
#define SPI_NSS_ACTIVE              (SPI_NSS_ACTIVE_HIGH)
#endif


/* SPI data buffer size definition. */
#define SPI_BUFFER_LENGTH           (8u)
#ifdef SLAVE_TEST
#define SPI_WRITE_SLAVE             (0x51u)     /*!< Customer definition. */
#define SPI_READ_SLAVE              (0x56u)     /*!< Customer definition. */
#endif

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void SpiConfig(void);

#ifdef SPI_APP_CUSTOM_NSS
static void SpiInitNssPin(void);
static void SpiNssActive(void);
static void SpiNssInactive(void);
static void SpiTransmitData(uint8_t *pu8TxBuf, uint32_t u32TxLength);
static void SpiReceiveData(uint8_t *pu8RxBuf, uint32_t u32RxLength);
static void SpiTransmitReceiveData(uint8_t *pu8TxBuf,       \
                                   uint8_t *pu8RxBuf,       \
                                   uint32_t u32TxLength,    \
                                   uint32_t u32RxLength);
#endif // #ifdef SPI_APP_CUSTOM_NSS

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
#ifdef SLAVE_TEST
static uint8_t m_au8SpiTxBuf[SPI_BUFFER_LENGTH] = {0x50, 0x51, 0x52, 0x53, 0x54, 0x55};
static uint8_t m_au8SpiRxBuf[SPI_BUFFER_LENGTH];     /*!< Valid data starts at offset 2 and length is SPI_BUFFER_LENGTH-2. */
#else
static uint8_t m_au8SpiTxBuf[SPI_BUFFER_LENGTH] = {0x50, 0x51, 0x52, 0x53, 0x54, 0x55};
#if (SPI_APP_TRANS_MODE == SPI_APP_FULL_DUPLEX)
static uint8_t m_au8SpiRxBuf[SPI_BUFFER_LENGTH];
#endif
#endif // #ifdef SLAVE_TEST

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of spi_master_base project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Configure the system clock to HRC32MHz. */
    SystemClockConfig();

    /* Configures SPI. */
    SpiConfig();

    /***************** Configuration end, application start **************/

    while (1u)
    {
#ifdef SLAVE_TEST
        /* Write data to the slave. */
        m_au8SpiTxBuf[0u] = SPI_WRITE_SLAVE;
        SPI_Transmit((uint8_t *)&m_au8SpiTxBuf[0u], SPI_BUFFER_LENGTH);
        /* Delay for slave handling data. */
        DDL_Delay1ms(2u);

        /* Read data from the slave. */
        m_au8SpiTxBuf[0u] = SPI_READ_SLAVE;
        SPI_Transmit((uint8_t *)&m_au8SpiTxBuf[0u], SPI_BUFFER_LENGTH);
        /* Delay for slave handling data. */
        DDL_Delay1ms(2u);
        SPI_Receive((uint8_t *)&m_au8SpiRxBuf[0u], SPI_BUFFER_LENGTH);
        // TODO: Use data received from the slave. Valid data starts at offset 2.
#else // If not defined SLAVE_TEST
#if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)
    #if (SPI_APP_TRANS_MODE == SPI_APP_FULL_DUPLEX)
        /* SPI send and receive in 4-wire master mode. */
        SPI_TransmitReceive((uint8_t *)&m_au8SpiTxBuf[0u],      \
                            (uint8_t *)&m_au8SpiRxBuf[0u],      \
                            SPI_BUFFER_LENGTH);
    #else
        /* SPI send only in 4-wire master mode. */
        SPI_Transmit((uint8_t *)&m_au8SpiTxBuf[0u], SPI_BUFFER_LENGTH);
    #endif
#else
    #ifdef SPI_APP_CUSTOM_NSS
        /* SPI send and receive in 3-wire master mode, with custom NSS pin. */
        SpiTransmitData((uint8_t *)&m_au8SpiTxBuf[0u], SPI_BUFFER_LENGTH);
        SpiReceiveData((uint8_t *)&m_au8SpiRxBuf[0u], SPI_BUFFER_LENGTH);
        SpiTransmitReceiveData((uint8_t *)&m_au8SpiTxBuf[0u],   \
                               (uint8_t *)&m_au8SpiRxBuf[0u],   \
                                SPI_BUFFER_LENGTH,              \
                                SPI_BUFFER_LENGTH);
    #else
        #if (SPI_APP_TRANS_MODE == SPI_APP_FULL_DUPLEX)
            /* SPI send and receive in 3-wire master mode. */
            SPI_TransmitReceive((uint8_t *)&m_au8SpiTxBuf[0u],  \
                                (uint8_t *)&m_au8SpiRxBuf[0u],  \
                                SPI_BUFFER_LENGTH);
        #else
            /* SPI send only in 3-wire master mode. */
            SPI_Transmit((uint8_t *)&m_au8SpiTxBuf[0u], SPI_BUFFER_LENGTH);
        #endif
    #endif // #ifdef SPI_APP_CUSTOM_NSS
#endif
#endif // #ifdef SLAVE_TEST
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
 * @brief  SPI configuration, including initialization and pin configuration.
 * @param  None
 * @retval None
 */
static void SpiConfig(void)
{
    stc_spi_init_t stcInit;

    /* Set a default value. */
    SPI_StructInit(&stcInit);

    /* User configuration value. */
    stcInit.u32WireMode          = SPI_WIRE_MODE;
    stcInit.u32TransMode         = SPI_TRANS_MODE;
    stcInit.u32NssActiveLevel    = SPI_NSS_ACTIVE;
    stcInit.u32SpiMode           = SPI_SPI_MODE;
    stcInit.u32BaudRatePrescaler = SPI_BR_DIV;

    /* The SPI register can be written only after the SPI peripheral is enabled. */
    CLK_FcgPeriphClockCmd(CLK_FCG_SPI, Enable);

    /* Initializes SPI. */
    SPI_Init(&stcInit);

    /* Set the pins to SPI function. */
#if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)
    GPIO_SetFunc(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_FUNC_7_SPI);
#elif defined SPI_APP_CUSTOM_NSS
    SpiInitNssPin();
#endif // #if (SPI_APP_X_WIRE == SPI_APP_4_WIRE)
    GPIO_SetFunc(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_FUNC_7_SPI);
    GPIO_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_FUNC_7_SPI);
    GPIO_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, GPIO_FUNC_7_SPI);

    /* Enable SPI function. */
    SPI_FunctionCmd(Enable);
}

#ifdef SPI_APP_CUSTOM_NSS
/**
 * @brief  Initializes custom NSS pin.
 * @param  None
 * @retval None
 */
static void SpiInitNssPin(void)
{
    stc_gpio_init_t stcGpioInit;

    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(SPI_CUSTOM_NSS_PORT, SPI_CUSTOM_NSS_PIN, &stcGpioInit);
    GPIO_OE(SPI_CUSTOM_NSS_PORT, SPI_CUSTOM_NSS_PIN, Enable);
    SpiNssInactive();
}

/**
 * @brief  Active custom NSS pin.
 * @param  None
 * @retval None
 */
static void SpiNssActive(void)
{
#if (SPI_NSS_ACTIVE_LEVEL == SPI_NSS_ACTIVE_LO)
    GPIO_ResetPins(SPI_CUSTOM_NSS_PORT, SPI_CUSTOM_NSS_PIN);
#else
    GPIO_SetPins(SPI_CUSTOM_NSS_PORT, SPI_CUSTOM_NSS_PIN);
#endif
}

/**
 * @brief  Inactive custom NSS pin.
 * @param  None
 * @retval None
 */
static void SpiNssInactive(void)
{
#if (SPI_NSS_ACTIVE_LEVEL == SPI_NSS_ACTIVE_LO)
    GPIO_SetPins(SPI_CUSTOM_NSS_PORT, SPI_CUSTOM_NSS_PIN);
#else
    GPIO_ResetPins(SPI_CUSTOM_NSS_PORT, SPI_CUSTOM_NSS_PIN);
#endif
}

/**
 * @brief  SPI transmit data with custom NSS pin.
 * @param  [in]  pu8TxBuf           Pointer to the data to be sent.
 * @param  [in]  u32TxLength        The length of the data.
 * @retval None
 */
static void SpiTransmitData(uint8_t *pu8TxBuf, uint32_t u32TxLength)
{
    SpiNssActive();
    SPI_Transmit(pu8TxBuf, u32TxLength);
    SpiNssInactive();
}

/**
 * @brief  SPI receive data with custom NSS pin.
 * @param  [in]  pu8RxBuf           Pointer to the buffer that the received data to be stored.
 * @param  [in]  u32RxLength        The length of the data to be received.
 * @retval None
 */
static void SpiReceiveData(uint8_t *pu8RxBuf, uint32_t u32RxLength)
{
    SpiNssActive();
    SPI_Receive(pu8RxBuf, u32RxLength);
    SpiNssInactive();
}

/**
 * @brief  SPI transmit and receive data with custom NSS pin.
 * @param  [in]  pu8TxBuf           Pointer to the data to be sent.
 * @param  [in]  pu8RxBuf           Pointer to the buffer that the received data to be stored.
 * @param  [in]  u32TxLength        The length of the data to be sent.
 * @param  [in]  u32RxLength        The length of the data to be received.
 * @retval None
 */
static void SpiTransmitReceiveData(uint8_t *pu8TxBuf,       \
                                   uint8_t *pu8RxBuf,       \
                                   uint32_t u32TxLength,    \
                                   uint32_t u32RxLength)
{
    SpiNssActive();
#ifdef SPI_APP_RECEIVE_WHILE_TRANSMIT
    SPI_TransmitReceive(pu8TxBuf, pu8RxBuf, u32TxLength);
#else
    SPI_Transmit(pu8TxBuf, u32TxLength);
    SPI_Receive(pu8RxBuf, u32RxLength);
#endif
    SpiNssInactive();
}

#endif // #ifdef SPI_APP_CUSTOM_NSS

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
