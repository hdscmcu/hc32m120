/**
 *******************************************************************************
 * @file  w25qxx.c
 * @brief This midware file provides firmware functions to W25QXX group spi flash.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-08       Wuze            First version
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
#include "hc32m120_clk.h"
#include "hc32m120_gpio.h"
#include "hc32m120_spi.h"
#include "hc32m120_utility.h"
#include "w25qxx.h"

/**
 * @addtogroup HC32M120_DDL_Midware
 * @{
 */

/**
 * @defgroup MW_W25QXX W25QXX
 * @brief W25Q Midware Library
 * @{
 */

#if (MW_W25QXX_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup W25QXX_Local_Macros W25QXX Local Macros
 * @{
 */
#define W25Q_BIT_0                  (1ul << 0)
#define W25Q_BIT_1                  (1ul << 1)
#define W25Q_BIT_2                  (1ul << 2)
#define W25Q_BIT_3                  (1ul << 3)
#define W25Q_BIT_4                  (1ul << 4)
#define W25Q_BIT_5                  (1ul << 5)
#define W25Q_BIT_6                  (1ul << 6)
#define W25Q_BIT_7                  (1ul << 7)
#define W25Q_BIT_8                  (1ul << 8)
#define W25Q_BIT_9                  (1ul << 9)
#define W25Q_BIT_10                 (1ul << 10)
#define W25Q_BIT_11                 (1ul << 11)
#define W25Q_BIT_12                 (1ul << 12)
#define W25Q_BIT_13                 (1ul << 13)
#define W25Q_BIT_14                 (1ul << 14)
#define W25Q_BIT_15                 (1ul << 15)


#define W25_CS_PORT                 (GPIO_PORT_1)
#define W25_CS_PIN                  (GPIO_PIN_3)
#define W25_SCK_PORT                (GPIO_PORT_1)
#define W25_SCK_PIN                 (GPIO_PIN_4)
#define W25_MOSI_PORT               (GPIO_PORT_1)
#define W25_MOSI_PIN                (GPIO_PIN_1)
#define W25_MISO_PORT               (GPIO_PORT_1)
#define W25_MISO_PIN                (GPIO_PIN_2)

#define LOAD_CMD(a, cmd, addr)  do  {                                       \
                                        (a)[0u] = (cmd);                    \
                                        (a)[1u] = (uint8_t)((addr) >> 16u); \
                                        (a)[2u] = (uint8_t)((addr) >> 8u);  \
                                        (a)[3u] = (uint8_t)(addr);          \
                                    } while (0u)


#define W25Q_SIZE_1K                (1024u)                 /*!< 1KB */
#define W25Q_SIZE_PAGE              (256u)                  /*!< 256B/page */
#define W25Q_SIZE_SECTOR            (W25Q_SIZE_1K * 4u)     /*!< 4KB/sector */
#define W25Q_SIZE_BLOCK             (W25Q_SIZE_1K * 64u)    /*!< 64KB/block */


#define W25Q_ST_BUSY                ((uint16_t)W25Q_BIT_0)
#define W25Q_ST_WEL                 ((uint16_t)W25Q_BIT_1)  /*<! Write enable latch. */


#define W25Q_DELAY_MS(x)            DDL_Delay1ms((x))
#define W25Q_CS_ACTIVE()            GPIO_ResetPins(W25_CS_PORT, W25_CS_PIN)
#define W25Q_CS_INACTIVE()          GPIO_SetPins(W25_CS_PORT, W25_CS_PIN)
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
 * @defgroup W25QXX_Local_Functions W25QXX Local Functions
 * @{
 */
static void W25InitCsPin(void);
static void W25Q_WriteCmd(uint8_t u8Cmd, const uint8_t *pu8CmdData, uint32_t u32CmdDataLength);
static void W25Q_ReadCmd(uint8_t u8Cmd, uint8_t *pu8CmdData, uint32_t u32CmdDataLength,
                         uint8_t *pu8Info, uint8_t u8InfoLength);

static void W25Q_Wt(uint8_t u8Cmd, uint32_t u32Address, const uint8_t *pu8Data, uint32_t u32DataLength);
static void W25Q_Rd(uint8_t u8Cmd, uint32_t u32Address, uint8_t *pu8Data, uint32_t u32DataLength);

static void W25Q_WaitBusy(void);

static void W25Q_WriteEnable(void);
static void W25Q_WriteDisable(void);

static void W25Q_WritePage(uint32_t u32Address, const uint8_t *pu8Data, uint32_t u32DataLength);
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup W25QXX_Global_Functions W25QXX Global Functions
 * @{
 */
/**
 * @brief  Initializes W25QXX.
 * @param  [out] pstcW25qxx         Pointer to a stc_w25qxx_t structure which contains the information of the SPI flash.
 * @retval None
 */
void W25Q_Init(stc_w25qxx_t *pstcW25qxx)
{
    stc_spi_init_t stcspiInit;

    /* Configuration structure initialization. */
    SPI_StructInit(&stcspiInit);

    /* Enable SPI peripheral. */
    CLK_FcgPeriphClockCmd(CLK_FCG_SPI, Enable);

    W25InitCsPin();
    GPIO_SetFunc(W25_SCK_PORT, W25_SCK_PIN, GPIO_FUNC_7_SPI);     /* SPI_SCK -- P73 */
    GPIO_SetFunc(W25_MOSI_PORT, W25_MOSI_PIN, GPIO_FUNC_7_SPI);   /* SPI_MOSI -- P71 */
    GPIO_SetFunc(W25_MISO_PORT, W25_MISO_PIN, GPIO_FUNC_7_SPI);   /* SPI_MISO -- P72 */

    /* Configuration SPI structure */
    stcspiInit.u32WireMode          = SPI_WIRE_3;
    stcspiInit.u32BaudRatePrescaler = SPI_BR_DIV_32;
    SPI_Init(&stcspiInit);

    /* Enable SPI function. */
    SPI_FunctionCmd(Enable);

    if (pstcW25qxx != NULL)
    {
        /* Read Flash ID */
        pstcW25qxx->u16ManId = W25Q_ReadManDeviceId();

        switch (pstcW25qxx->u16ManId)
        {
            case W25Q64:
                pstcW25qxx->u32PageCount       = 32768u;    /* W25Q64 contains 32768 pages. */
                pstcW25qxx->u32SectorCount     = 2048U;     /* W25Q64 contains 2048 sectors. */
                pstcW25qxx->u32BlockCount32k   = 0u;        /* DO NOT support 32K block. */
                pstcW25qxx->u32BlockCount64k   = 128u;
                pstcW25qxx->u32CapacityInBytes = pstcW25qxx->u32PageCount * W25Q_SIZE_PAGE;
                pstcW25qxx->u32CapacityInKB    = pstcW25qxx->u32CapacityInBytes * W25Q_SIZE_1K;
                break;

            default:
                break;
        }
    }
}

/**
 * @brief  Read manufacturer device ID.
 * @param  None
 * @retval 16 bit manufacturer device ID.
 */
uint16_t W25Q_ReadManDeviceId(void)
{
    uint8_t au8TempId[2u];
    uint8_t au8Dummy[3u] = {0};
    uint16_t u16ManID;

    W25Q_ReadCmd(W25Q_MANUFACTURER_DEVICE_ID, au8Dummy, 3u, au8TempId, 2u);

    u16ManID = (uint16_t)au8TempId[0] << 8u;
    u16ManID |= au8TempId[1u];

    return u16ManID;
}

/**
 * @brief  Read unique ID.
 * @param  [out] pu8UniqueId        Pointer to a buffer the 64 bit unique ID to be stored.
 * @retval None
 */
void W25Q_ReadUniqueId(uint8_t *pu8UniqueId)
{
    uint8_t au8Dummy[4u] = {0};

    W25Q_ReadCmd(W25Q_READ_UNIQUE_ID, au8Dummy, 4u, pu8UniqueId, 8u);
}

/**
 * @brief  W25QXX read status register.
 * @param  None
 * @retval 16 bit W25QXX status.
 */
uint16_t W25Q_ReadStatus(void)
{
    uint8_t  u8TempStatus;
    uint16_t u16RetStatus;

    W25Q_ReadCmd(W25Q_READ_STATUS_REG_2, NULL, 0u, &u8TempStatus, 1u);

    u16RetStatus = u8TempStatus;

    W25Q_ReadCmd(W25Q_READ_STATUS_REG_1, NULL, 0u, &u8TempStatus, 1u);

    u16RetStatus <<= 8u;
    u16RetStatus |= u8TempStatus;

    return u16RetStatus;
}

/**
 * @brief  W25QXX write status register
 * @param  [in]  u16Status          Specified status.
 * @retval None
 */
void W25Q_WriteStatus(uint16_t u16Status)
{
    uint8_t au8Data[2u];

    au8Data[0u] = (uint8_t)u16Status;
    au8Data[1u] = (uint8_t)(u16Status >> 8u);

    W25Q_WriteCmd(W25Q_WRITE_STATUS_REG, au8Data, 2u);
}

/**
 * @brief  W25QXX power down.
 * @param  None
 * @retval None
 */
void W25Q_PowerDown(void)
{
    W25Q_WriteCmd(W25Q_POWER_DOWN, NULL, 0u);

    W25Q_DELAY_MS(1u);
}

/**
 * @brief  W25QXX release power down.
 * @param  None
 * @retval None
 */
void W25Q_ReleasePowerDown(void)
{
    W25Q_WriteCmd(W25Q_RELEASE_POWER_DOWN, NULL, 0u);

    W25Q_DELAY_MS(1u);
}

/**
 * @brief  W25QXX chip ease.
 * @param  None
 * @retval None
 */
void W25Q_EraseChip(void)
{
    W25Q_WriteEnable();
    W25Q_WaitBusy();
    W25Q_WriteCmd(W25Q_CHIP_ERASE, NULL, 0u);
    W25Q_WaitBusy();
}

/**
 * @brief  W25QXX sector ease.
 * @param  [in]  u32SectorAddress   The address of the specified sector.
 * @retval None
 */
void W25Q_EraseSector(uint32_t u32SectorAddress)
{
    u32SectorAddress *= W25Q_SIZE_SECTOR;

    W25Q_WriteEnable();
    W25Q_WaitBusy();

    W25Q_Wt(W25Q_SECTOR_ERASE, u32SectorAddress, NULL, 0u);

    W25Q_WaitBusy();
    W25Q_WriteDisable();
}

/**
 * @brief  W25QXX block ease.
 * @param  [in]  u32BlockAddress    The address of the specified block.
 * @retval None
 */
void W25Q_EraseBlock(uint32_t u32BlockAddress)
{
    W25Q_Wt(W25Q_BLOCK_ERASE_64K, u32BlockAddress, NULL, 0u);
}

/**
 * @brief  W25QXX write data.
 * @param  [in]  u32Address         The start address of the data to be written.
 * @param  [in]  pu8WriteBuf        The pointer to the buffer contains the data to be written.
 * @param  [in]  u32NumByteToWrite  Buffer size in bytes.
 * @retval None
 */
void W25Q_WriteData(uint32_t u32Address, const uint8_t *pu8WriteBuf, uint32_t u32NumByteToWrite)
{
    uint32_t u32SecAddr;
    uint32_t u32SecStart;
    uint32_t u32SecStop;
    uint32_t u32PageRemain;
    uint32_t u32PageAddr;
    const uint8_t *pu8Wt;

    u32SecStart = u32Address / W25Q_SIZE_SECTOR;
    u32SecStop  = (u32Address + u32NumByteToWrite) / W25Q_SIZE_SECTOR;

    for (u32SecAddr = u32SecStart; u32SecAddr <= u32SecStop; u32SecAddr++)
    {
        W25Q_EraseSector(u32SecAddr);
    }

    pu8Wt         = pu8WriteBuf;
    u32PageAddr   = u32Address;
    u32PageRemain = W25Q_SIZE_PAGE - u32Address % W25Q_SIZE_PAGE;
    if (u32PageRemain >= u32NumByteToWrite)
    {
        u32PageRemain = u32NumByteToWrite;
    }

    while (u32NumByteToWrite)
    {
        W25Q_WritePage(u32PageAddr, pu8Wt, u32PageRemain);

        u32PageAddr += u32PageRemain;
        pu8Wt = pu8WriteBuf + u32PageRemain;
        u32NumByteToWrite -= u32PageRemain;

        if (u32NumByteToWrite >= W25Q_SIZE_PAGE)
        {
            u32PageRemain = W25Q_SIZE_PAGE;
        }
        else
        {
            u32PageRemain = u32NumByteToWrite;
        }
    }
}

/**
 * @brief  W25QXX read data.
 * @param  [in]  u32Address         The start address of the data to be read.
 * @param  [in]  pu8ReadBuf         The pointer to the buffer contains the data to be stored.
 * @param  [in]  u32NumByteToRead   Buffer size in bytes.
 * @retval None
 */
void W25Q_ReadData(uint32_t u32Address, uint8_t *pu8ReadBuf, uint32_t u32NumByteToRead)
{
    W25Q_Rd(W25Q_READ_DATA, u32Address, pu8ReadBuf, u32NumByteToRead);
}

/**
 * @}
 */

/**
 * @addtogroup W25QXX_Local_Functions W25QXX Local Functions
 * @{
 */
/**
 * @brief  Initializes SPI CS pin.
 * @param  None
 * @retval None
 */
static void W25InitCsPin(void)
{
    stc_gpio_init_t stcGpioInit;

    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(W25_CS_PORT, W25_CS_PIN, &stcGpioInit);
    GPIO_OE(W25_CS_PORT, W25_CS_PIN, Enable);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX write command.
 * @param  [in]  u8Cmd              Command of W25QXX.
 * @param  [in]  pu8CmdData         Pointer to a buffer that contains the data following the command.
 * @param  [in]  u32CmdDataLength   The length of the command data in bytes.
 * @retval None
 */
static void W25Q_WriteCmd(uint8_t u8Cmd, const uint8_t *pu8CmdData, uint32_t u32CmdDataLength)
{
    W25Q_CS_ACTIVE();
    SPI_Transmit(&u8Cmd, 1u);
    SPI_Transmit(pu8CmdData, u32CmdDataLength);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX read command.
 * @param  [in]  u8Cmd              Command of W25QXX.
 * @param  [in]  pu8CmdData         Pointer to a buffer that contains the data following the command.
 * @param  [in]  u32CmdDataLength   The length of the command data in bytes.
 * @param  [in]  pu8Info            The information of the command.
 * @param  [in]  u8InfoLength       The length of the information.
 * @retval None
 */
static void W25Q_ReadCmd(uint8_t u8Cmd, uint8_t *pu8CmdData, uint32_t u32CmdDataLength,
                         uint8_t *pu8Info, uint8_t u8InfoLength)
{
    W25Q_CS_ACTIVE();
    SPI_Transmit(&u8Cmd, 1u);
    SPI_Transmit(pu8CmdData, u32CmdDataLength);
    SPI_Receive(pu8Info, (uint32_t)u8InfoLength);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX write data.
 * @param  [in]  u8Cmd              Command of W25QXX.
 * @param  [in]  u32Address         The start address of the data to be written.
 * @param  [in]  pu8Data            The data to be written.
 * @param  [in]  u32DataLength      The length of the data in bytes.
 * @retval None
 */
static void W25Q_Wt(uint8_t u8Cmd, uint32_t u32Address, const uint8_t *pu8Data, uint32_t u32DataLength)
{
    uint8_t au8Cmd[4u];

    LOAD_CMD(au8Cmd, u8Cmd, u32Address);

    W25Q_CS_ACTIVE();
    SPI_Transmit(au8Cmd, 4u);
    SPI_Transmit(pu8Data, u32DataLength);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX read data.
 * @param  [in]  u8Cmd              Command of W25QXX.
 * @param  [in]  u32Address         The start address of the data to be written.
 * @param  [in]  pu8Data            The data to be stored.
 * @param  [in]  u32DataLength      The length of the data in bytes.
 * @retval None
 */
static void W25Q_Rd(uint8_t u8Cmd, uint32_t u32Address, uint8_t *pu8Data, uint32_t u32DataLength)
{
    uint8_t au8Cmd[4u];

    LOAD_CMD(au8Cmd, u8Cmd, u32Address);

    W25Q_CS_ACTIVE();
    SPI_Transmit(au8Cmd, 4u);
    SPI_Receive(pu8Data, u32DataLength);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX Write enable.
 * @param  None
 * @retval None
 */
static void W25Q_WriteEnable(void)
{
    W25Q_WriteCmd(W25Q_WRITE_ENABLE, NULL, 0u);
}

/**
 * @brief  W25QXX Write disable.
 * @param  None
 * @retval None
 */
static void W25Q_WriteDisable(void)
{
    W25Q_WriteCmd(W25Q_WRITE_DISABLE, NULL, 0u);
}

/**
 * @brief  Wait while W25QXX is busy.
 * @param  None
 * @retval None
 */
static void W25Q_WaitBusy(void)
{
    while ((W25Q_ReadStatus() & W25Q_ST_BUSY) == W25Q_ST_BUSY)
    {
        ;
    }
}

/**
 * @brief  W25QXX page program.
 * @param  [in]  u32Address         Start address of the page.
 * @param  [in]  pu8Data            Pointer to a buffer that contains the data to be written.
 * @param  [in]  u32DataLength      Size of the buffer in bytes.
 * @retval None
 */
static void W25Q_WritePage(uint32_t u32Address, const uint8_t *pu8Data, uint32_t u32DataLength)
{
    W25Q_WriteEnable();
    W25Q_Wt(W25Q_PAGE_PROGRAM, u32Address, pu8Data, u32DataLength);
    W25Q_WaitBusy();
}

/**
 * @}
 */

#endif // #if (MW_W25QXX_ENABLE == DDL_ON)

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
