/**
 *******************************************************************************
 * @file  crc/crc_base/source/main.c
 * @brief Main program of CRC base for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-20       Heqb            First version
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
 * @addtogroup CRC_Base
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CRC16_InitVal                 (0xFFFFu)
#define CRC32_InitVal                 (0xFFFFFFFFul)
   
/* Enable CRC. */
#define ENABLE_CRC()                (CLK_FcgPeriphClockCmd(CLK_FCG_CRC, Enable))

/* Disable CRC. */
#define DISABLE_CRC()               (CLK_FcgPeriphClockCmd(CLK_FCG_CRC, Disable))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void CrcConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of template project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint8_t  au8SrcData[1u];
    uint16_t au16SrcData[1u];
    uint32_t au32SrcData[1u];
    uint16_t u16Checksum;
    uint32_t u32Checksum;

    /* Configures CRC. */
    CrcConfig();

    /***************** Configuration end, application start **************/
    while (1)
    {
        /* Calculates byte data's CRC16 checksum and CRC32 checksum. */
        au8SrcData[0u]  = 0x12u;
        CRC_Calculate(CRC_CRC16, (uint8_t *)&au8SrcData[0u], CRC16_InitVal, 1u, CRC_BW_8);
        CRC_Calculate(CRC_CRC32, (uint8_t *)&au8SrcData[0u], CRC32_InitVal, 1u, CRC_BW_8);

        /* Calculates half word data's CRC16 checksum and CRC32 checksum. */
        au16SrcData[0u] = 0x2019u;
        CRC_Calculate(CRC_CRC16, (uint16_t *)&au16SrcData[0u], CRC16_InitVal, 1u, CRC_BW_16);
        CRC_Calculate(CRC_CRC32, (uint16_t *)&au16SrcData[0u], CRC32_InitVal, 1u, CRC_BW_16);

        /* Calculates word data's CRC16 checksum and CRC32 checksum. */
        au32SrcData[0u] = 0x20190610ul;
        CRC_Calculate(CRC_CRC16, (uint32_t *)&au32SrcData[0u], CRC16_InitVal, 1u, CRC_BW_32);
        CRC_Calculate(CRC_CRC32, (uint32_t *)&au32SrcData[0u], CRC32_InitVal, 1u, CRC_BW_32);

        /* Checks byte data's CRC16 checksum and CRC32 checksum. */
        au8SrcData[0u]  = 0xC8u;
        u16Checksum     = 0xBA3Cu;
        CRC_Check(CRC_CRC16, (uint32_t)u16Checksum, (uint8_t *)&au8SrcData[0u], CRC16_InitVal, 1u, CRC_BW_8);
        u32Checksum     = 0x47BDA50Ful;
        CRC_Check(CRC_CRC32, u32Checksum, (uint8_t *)&au8SrcData[0u], CRC32_InitVal, 1u, CRC_BW_8);

        /* Checks half word data's CRC16 checksum and CRC32 checksum. */
        au16SrcData[0u] = 0x1234u;
        u16Checksum     = 0xED16u;
        CRC_Check(CRC_CRC16, (uint32_t)u16Checksum, (uint16_t *)&au16SrcData[0u], CRC16_InitVal, 1u, CRC_BW_16);
        u32Checksum     = 0x094A9040ul;
        CRC_Check(CRC_CRC32, u32Checksum, (uint16_t *)&au16SrcData[0u], CRC32_InitVal, 1u, CRC_BW_16);

        /* Checks word data's CRC16 checksum and CRC32 checksum. */
        au32SrcData[0u] = 0x12345678ul;
        u16Checksum     = 0xF428u;
        CRC_Check(CRC_CRC16, (uint32_t)u16Checksum, (uint32_t *)&au32SrcData[0u], CRC16_InitVal, 1u, CRC_BW_32);
        u32Checksum     = 0xAF6D87D2ul;
        CRC_Check(CRC_CRC32, u32Checksum, (uint32_t *)&au32SrcData[0u], CRC32_InitVal, 1u, CRC_BW_32);
        
    }
}

/**
 * @brief  CRC configuration.
 * @param  None
 * @retval None
 */
static void CrcConfig(void)
{
    ENABLE_CRC();

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
