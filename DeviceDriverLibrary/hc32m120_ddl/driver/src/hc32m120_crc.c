/**
 *******************************************************************************
 * @file  hc32m120_crc.c
 * @brief This file provides firmware functions to manage the Cyclic Redundancy
 *        Check(CRC).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-19       Heqb            First version
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
#include "hc32m120_crc.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_CRC CRC
 * @brief Cyclic Redundancy Check Driver Library
 * @{
 */

#if (DDL_CRC_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CRC_Local_Macros CRC Local Macros
 * @{
 */

/**
 * @defgroup CRC_Register_Access_Bit_Width CRC Register Access Bit Width
 * @{
 */
/* Definition of CRC32 data register. */
#define CRC32_DAT_REG               (M0P_CRC->DAT0)

/* Definition of CRC32 initial value register. */
#define CRC32_INIT_REG              (M0P_CRC->RESLT)

/* Definition of CRC32 result value register. */
#define CRC32_RSLT_REG              (M0P_CRC->RESLT)

/* Definition of CRC16 data register. */
#define CRC16_DAT_REG               (*((__IO uint16_t *)&M0P_CRC->DAT0))

/* Definition of CRC16 initial value register. */
#define CRC16_INIT_REG              (*((__IO uint16_t *)&M0P_CRC->RESLT))

/* Definition of CRC16 result value register. */
#define CRC16_RSLT_REG              (*((__IO uint16_t *)&M0P_CRC->RESLT))

/* Definition data register in 8 bit width. */
#define CRC8_DAT_REG                (*((__IO uint8_t *)&M0P_CRC->DAT0))
/**
 * @}
 */

/**
 * @defgroup CRC_Check_Parameters_Validity CRC check parameters validity
 * @{
 */
#define IS_CRC_PROCOTOL(x)                                                     \
(   ((x) == CRC_CRC16)                      ||                                 \
    ((x) == CRC_CRC32))

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
/** 
 * @defgroup CRC_Local_Functions CRC Local Functions
 * @{
 */
static void CRC_8BitWrite(const void *pvData, uint32_t u32Length);
static void CRC_16BitWrite(const void *pvData, uint32_t u32Length);
static void CRC_32BitWrite(const void *pvData, uint32_t u32Length);
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
 * @defgroup CRC_Global_Functions CRC Global Functions
 * @{
 */

/**
 * @brief  CRC16 calculation.
 * @param  [in]   u32CrcProtocol        CRC protocol control bit.
 *                                      This parameter can be a value of @ref CRC_Protocol_Control_Bit
 * @param  [in]   pvData                Pointer to the buffer containing the data to be computed.
 * @param  [in]   u32InitVal            Initialize the CRC calculation. 
 * @param  [in]   u32Length             The length(countted in bytes or half word or word, depending on
 *                                      the bit width) of the data to be computed.
 * @param  [in]   u8BitWidth            Bit width of the data.
 *   @arg  CRC_BW_8:                    The pointer pvData points to a byte array.
 *   @arg  CRC_BW_16:                   The pointer pvData points to a half word array.
 *   @arg  CRC_BW_32:                   The pointer pvData points to a word array.
 * @retval CRC checksum.
 */
uint32_t CRC_Calculate(uint32_t u32CrcProtocol,
                       const void *pvData,
                       uint32_t u32InitVal,
                       uint32_t u32Length,
                       uint8_t u8BitWidth)
{
    uint32_t u32CheckSum = 0u;

    DDL_ASSERT(IS_CRC_PROCOTOL(u32CrcProtocol));

    if ((pvData != NULL) && (u32Length != 0u) &&
        ((u8BitWidth == CRC_BW_8) || (u8BitWidth == CRC_BW_16) || (u8BitWidth == CRC_BW_32)))
    {
         bM0P_CRC->CR_b.CR = u32CrcProtocol;

        if (u32CrcProtocol == CRC_CRC32)
        {
            CRC32_INIT_REG = u32InitVal;
        }
        else
        {
            CRC16_INIT_REG = (uint16_t)u32InitVal;
        }

        if (u8BitWidth == CRC_BW_8)
        {
            CRC_8BitWrite(pvData, u32Length);
        }
        else if (u8BitWidth == CRC_BW_16)
        {
            CRC_16BitWrite(pvData, u32Length);
        }
        else
        {
            CRC_32BitWrite(pvData, u32Length);
        }

        if (u32CrcProtocol == CRC_CRC32)
        {
            u32CheckSum = CRC32_RSLT_REG;
        }
        else
        {
            u32CheckSum = (uint32_t)CRC16_RSLT_REG;
        }

    }

      return u32CheckSum;
}

/**
 * @brief  CRC check.
 * @param  [in]  u32CrcProtocol     CRC protocol control bit.
 *                                  This parameter can be a value of @ref CRC_Protocol_Control_Bit
 * @param  [in]  u32CheckSum        The checksum of the data pointed by pointer pvData.
 * @param  [in]  pvData             Pointer to the buffer containing the data to be checked.
 * @param  [in]  u32InitVal         Initialize the CRC calculation.    
 * @param  [in]  u32Length          The length(countted in bytes or half word or word, depending on
 *                                  the bit width) of the data to be computed.
 * @param  [in] u8BitWidth          Bit width of the data.
 *   @arg  CRC_BW_8:                The pointer pvData points to a byte array.
 *   @arg  CRC_BW_16:               The pointer pvData points to a half word array.
 *   @arg  CRC_BW_32:               The pointer pvData points to a word array.
 * @retval A en_flag_status_t value.
 *   @arg  Set:                     CRC checks successfully.
 *   @arg  Reset:                   CRC checks unsuccessfully.
 */
en_flag_status_t CRC_Check(uint32_t u32CrcProtocol,
                           uint32_t u32CheckSum,
                           const void *pvData,
                           uint32_t u32InitVal,
                           uint32_t u32Length,
                           uint8_t u8BitWidth)
{
    en_flag_status_t enFlag = Reset;
    DDL_ASSERT(IS_CRC_PROCOTOL(u32CrcProtocol));

    if ((pvData != NULL) && (u32Length != 0u) &&
        ((u8BitWidth == CRC_BW_8) || (u8BitWidth == CRC_BW_16) || (u8BitWidth == CRC_BW_32)))
    {
        bM0P_CRC->CR_b.CR = u32CrcProtocol;

        if (u32CrcProtocol == CRC_CRC32)
        {
            CRC32_INIT_REG = u32InitVal;
        }
        else
        {
            CRC16_INIT_REG = (uint16_t)u32InitVal;
        }

        if (u8BitWidth == CRC_BW_8)
        {
            CRC_8BitWrite(pvData, u32Length);
        }
        else if (u8BitWidth == CRC_BW_16)
        {
            CRC_16BitWrite(pvData, u32Length);
        }
        else
        {
            CRC_32BitWrite(pvData, u32Length);
        }
    
        if (u32CrcProtocol == CRC_CRC32)
        {
            CRC32_DAT_REG = u32CheckSum;
        }
        else
        {
            CRC16_DAT_REG = (uint16_t)u32CheckSum;
        }

        if ((M0P_CRC->CR & CRC_FLAG_MASK) != 0u)
        {
            enFlag = Set;
        }

    }

    return enFlag;
}

/**
 * @}
 */

/**
 * @addtogroup CRC_Local_Functions CRC Local Functions
 * @{
 */

/**
 * @brief  Write CRC data register in bytes.
 * @param  [in]  pvData             Pointer to the buffer containing the data to be written.
 * @param  [in]  u32Length          The length of data the in bytes.
 * @retval None
 */
static void CRC_8BitWrite(const void *pvData, uint32_t u32Length)
{
    uint32_t u32Count;
    const uint8_t *pu8Data = pvData;

    for (u32Count = 0u; u32Count < u32Length; u32Count++)
    {
        CRC8_DAT_REG = pu8Data[u32Count];
    }
}

/**
 * @brief  Write CRC data register in half words.
 * @param  [in]  pvData             Pointer to the buffer containing the data to be written.
 * @param  [in]  u32Length          The length of the data in half words.
 * @retval None
 */
static void CRC_16BitWrite(const void *pvData, uint32_t u32Length)
{
    uint32_t u32Count;
    const uint16_t *pu16Data = pvData;

    for (u32Count = 0u; u32Count < u32Length; u32Count++)
    {
        CRC16_DAT_REG = pu16Data[u32Count];
    }
}

/**
 * @brief  Write CRC data register in words.
 * @param  [in]  pvData             Pointer to the buffer containing the data to be written.
 * @param  [in]  u32Length          The length of the data in words.
 * @retval None
 */
static void CRC_32BitWrite(const void *pvData, uint32_t u32Length)
{
    uint32_t u32Count;
    const uint32_t *pu32Data = pvData;

    for (u32Count = 0u; u32Count < u32Length; u32Count++)
    {
        CRC32_DAT_REG = pu32Data[u32Count];
    }
}

/**
 * @}
 */

#endif /* DDL_CRC_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
