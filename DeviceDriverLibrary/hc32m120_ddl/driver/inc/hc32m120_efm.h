/**
 *******************************************************************************
 * @file  hc32m120_efm.h
 * @brief This file contains all the functions prototypes of the EFM driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-05-06       Chengy          First version
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
#ifndef __HC32M120_EFM_H__
#define __HC32M120_EFM_H__

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
 * @addtogroup DDL_EFM
 * @{
 */

#if (DDL_EFM_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup EFM_Global_Types EFM Global Types
 * @{
 */

/**
 * @brief EFM windows protect address definition
 */
typedef struct
{
    uint32_t            u32StartAddr;      /*!< The protect start address.     */
    uint32_t            u32EndAddr;        /*!< The protect end address.       */
} stc_efm_win_protect_addr_t;

/**
 * @brief EFM configure structure
 */
typedef struct
{
    uint32_t            u32Latency;     /*!< Specifies the efm latency.
                                            This parameter can be a value of @ref EFM_Latency.              */

    uint32_t            u32CacheRst;    /*!< Specifies the data cache reset on or off.
                                            This parameter can be a value of @ref EFM_CacheRst_State.       */

    uint32_t            u32InsCache;    /*!< Specifies the instruction cache on or off.
                                            This parameter can be a value of @ref EFM_InsCache_State.       */

    uint32_t            u32CacheEn;     /*!< Specifies the data cache on or off.
                                            This parameter can be a value of @ref EFM_CacheEn_State.        */

    uint32_t            u32BusState;    /*!< Specifies the bus state busy or release while program & erase.
                                            This parameter can be a value of @ref EFM_Bus_State.            */
} stc_efm_cfg_t;

/**
 * @brief EFM unique ID definition
 */
typedef struct
{
    uint32_t            u32UniqueID0;      /*!< unique ID 0.       */
    uint32_t            u32UniqueID1;      /*!< unique ID 1.       */
    uint32_t            u32UniqueID2;      /*!< unique ID 2.       */
} stc_efm_unique_id_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EFM_Global_Macros EFM Global Macros
 * @{
 */
#define EFM_TIMEOUT                 (0x1000u)

/**
 * @defgroup EFM_Address EFM address area
 * @{
 */
#define EFM_START_ADDR              (0x00000000ul)
#define EFM_END_ADDR                (0x0000FFFFul)
/**
 * @}
 */

/**
 * @defgroup EFM_Sector_Address EFM Sector Address
 * @{
 */
#define EFM_SECTOR0_ADRR            (0x00000000ul)
#define EFM_SECTOR1_ADRR            (0x00000200ul)
#define EFM_SECTOR2_ADRR            (0x00000400ul)
#define EFM_SECTOR3_ADRR            (0x00000600ul)
#define EFM_SECTOR4_ADRR            (0x00000800ul)
#define EFM_SECTOR5_ADRR            (0x00000A00ul)
#define EFM_SECTOR6_ADRR            (0x00000C00ul)
#define EFM_SECTOR7_ADRR            (0x00000E00ul)
#define EFM_SECTOR8_ADRR            (0x00001000ul)
#define EFM_SECTOR9_ADRR            (0x00001200ul)
#define EFM_SECTOR10_ADRR           (0x00001400ul)
#define EFM_SECTOR11_ADRR           (0x00001600ul)
#define EFM_SECTOR12_ADRR           (0x00001800ul)
#define EFM_SECTOR13_ADRR           (0x00001A00ul)
#define EFM_SECTOR14_ADRR           (0x00001C00ul)
#define EFM_SECTOR15_ADRR           (0x00001E00ul)
#define EFM_SECTOR16_ADRR           (0x00002000ul)
#define EFM_SECTOR17_ADRR           (0x00002200ul)
#define EFM_SECTOR18_ADRR           (0x00002400ul)
#define EFM_SECTOR19_ADRR           (0x00002600ul)
#define EFM_SECTOR20_ADRR           (0x00002800ul)
#define EFM_SECTOR21_ADRR           (0x00002A00ul)
#define EFM_SECTOR22_ADRR           (0x00002C00ul)
#define EFM_SECTOR23_ADRR           (0x00002E00ul)
#define EFM_SECTOR24_ADRR           (0x00003000ul)
#define EFM_SECTOR25_ADRR           (0x00003200ul)
#define EFM_SECTOR26_ADRR           (0x00003400ul)
#define EFM_SECTOR27_ADRR           (0x00003600ul)
#define EFM_SECTOR28_ADRR           (0x00003800ul)
#define EFM_SECTOR29_ADRR           (0x00003A00ul)
#define EFM_SECTOR30_ADRR           (0x00003C00ul)
#define EFM_SECTOR31_ADRR           (0x00003E00ul)
#define EFM_SECTOR32_ADRR           (0x00004000ul)
#define EFM_SECTOR33_ADRR           (0x00004200ul)
#define EFM_SECTOR34_ADRR           (0x00004400ul)
#define EFM_SECTOR35_ADRR           (0x00004600ul)
#define EFM_SECTOR36_ADRR           (0x00004800ul)
#define EFM_SECTOR37_ADRR           (0x00004A00ul)
#define EFM_SECTOR38_ADRR           (0x00004C00ul)
#define EFM_SECTOR39_ADRR           (0x00004E00ul)
#define EFM_SECTOR40_ADRR           (0x00005000ul)
#define EFM_SECTOR41_ADRR           (0x00005200ul)
#define EFM_SECTOR42_ADRR           (0x00005400ul)
#define EFM_SECTOR43_ADRR           (0x00005600ul)
#define EFM_SECTOR44_ADRR           (0x00005800ul)
#define EFM_SECTOR45_ADRR           (0x00005A00ul)
#define EFM_SECTOR46_ADRR           (0x00005C00ul)
#define EFM_SECTOR47_ADRR           (0x00005E00ul)
#define EFM_SECTOR48_ADRR           (0x00006000ul)
#define EFM_SECTOR49_ADRR           (0x00006200ul)
#define EFM_SECTOR50_ADRR           (0x00006400ul)
#define EFM_SECTOR51_ADRR           (0x00006600ul)
#define EFM_SECTOR52_ADRR           (0x00006800ul)
#define EFM_SECTOR53_ADRR           (0x00006A00ul)
#define EFM_SECTOR54_ADRR           (0x00006C00ul)
#define EFM_SECTOR55_ADRR           (0x00006E00ul)
#define EFM_SECTOR56_ADRR           (0x00007000ul)
#define EFM_SECTOR57_ADRR           (0x00007200ul)
#define EFM_SECTOR58_ADRR           (0x00007400ul)
#define EFM_SECTOR59_ADRR           (0x00007600ul)
#define EFM_SECTOR60_ADRR           (0x00007800ul)
#define EFM_SECTOR61_ADRR           (0x00007A00ul)
#define EFM_SECTOR62_ADRR           (0x00007C00ul)
#define EFM_SECTOR63_ADRR           (0x00007E00ul)
/**
 * @}
 */

/**
 * @defgroup EFM_State EFM State
 * @{
 */
#define EFM_ON                      (0x00000000ul)
#define EFM_OFF                     (EFM_FSTP_FSTP)
/**
 * @}
 */

/**
 * @defgroup EFM_Latency EFM latency value
 * @{
 */
#define EFM_LATENCY_0               (0x00000000ul)
#define EFM_LATENCY_1               (0x00000001ul)
/**
 * @}
 */

/**
 * @defgroup EFM_CacheRst_State EFM data cache reset state
 * @{
 */
#define EFM_CACHERST_ON             (EFM_FRMC_CRST0)
#define EFM_CACHERST_OFF            (0x00000000ul)
/**
 * @}
 */

/**
 * @defgroup EFM_InsCache_State EFM instruction cache state
 * @{
 */
#define EFM_INSCACHE_ON             (EFM_FRMC_PREFETE)
#define EFM_INSCACHE_OFF            (0x00000000ul)
/**
 * @}
 */

/**
 * @defgroup EFM_CacheEn_State EFM data cache state
 * @{
 */
#define EFM_CACHEEN_ON              (EFM_FRMC_CACHE)
#define EFM_CACHEEN_OFF             (0x00000000ul)
/**
 * @}
 */

/**
 * @defgroup EFM_Bus_State EFM Bus state while flash program or erase
 * @{
 */
#define EFM_BUS_BUSY                (0x00000000ul)
#define EFM_BUS_RELEASE             (EFM_FWMC_BUSHLDCTL)
/**
 * @}
 */

/**
 * @defgroup EFM_Mode_definition EFM program or erase mode definition
 * @{
 */
#define EFM_MODE_PROGRAMSINGLE      (0x00000010ul)         /*!< Program single at a specified address          */
#define EFM_MODE_PROGRAMREADBACK    (0x00000020ul)         /*!< Program and read back at a specified address   */
#define EFM_MODE_PROGRAMSEQUENCE    (0x00000030ul)         /*!< Program sequence at sequence address           */
#define EFM_MODE_ERASESECTOR        (0x00000040ul)         /*!< Sector erase                                   */
#define EFM_MODE_ERASECHIP          (0x00000050ul)         /*!< Chip erase                                     */
#define EFM_MODE_READONLY           (0x00000000ul)         /*!< Read only                                      */
#define EFM_MODE_READONLY1          (0x00000060ul)         /*!< Read only                                      */
#define EFM_MODE_READONLY2          (0x00000070ul)         /*!< Read only                                      */
/**
 * @}
 */

/**
 * @defgroup EFM_Flag_definition EFM Flag definition
 * @{
 */
#define EFM_FLAG_PEWERR             (EFM_FSR_PEWERR)          /*!< EFM Programming/erase error flag.              */
#define EFM_FLAG_PEPRTERR           (EFM_FSR_PEPRTERR)        /*!< EFM Programming/erase protect area error flag. */
#define EFM_FLAG_PGMISMTCHERR       (EFM_FSR_PGMISMTCH)       /*!< EFM Programming  miss match error flag         */
#define EFM_FLAG_OPTEND             (EFM_FSR_OPTEND)          /*!< EFM End of Operation flag.                     */
#define EFM_FLAG_COLERR             (EFM_FSR_RDCOLERR)        /*!< EFM Read collide error flag.                   */
#define EFM_FLAG_RDY                (EFM_FSR_RDY)             /*!< EFM ready flag.                                */

#define EFM_FLAG_CLR_PEWERRCLR      (EFM_FSCLR_PEWERRCLR)     /*!< EFM Programming/erase error flag.              */
#define EFM_FLAG_CLR_PEPRTERRCLR    (EFM_FSCLR_PEPRTERRCLR)   /*!< EFM Programming/erase protect area error flag. */
#define EFM_FLAG_CLR_PGMISMTCHCLR   (EFM_FSCLR_PGMISMTCHCLR)  /*!< EFM Programming  miss match error flag         */
#define EFM_FLAG_CLR_OPTENDCLR      (EFM_FSCLR_OPTENDCLR)     /*!< EFM End of Operation flag.                     */
#define EFM_FLAG_CLR_COLERRCLR      (EFM_FSCLR_RDCOLERRCLR)   /*!< EFM Read collide error flag.                   */

/**
 * @}
 */

/**
 * @defgroup EFM_Interrupt_definition EFM Interrupt definition
 * @{
 */
#define EFM_INT_EOP                 (EFM_FITE_OPTENDITE)      /*!< End of EFM Operation Interrupt source      */
#define EFM_INT_PEERR               (EFM_FITE_PEERRITE)       /*!< Program/erase error Interrupt source       */
#define EFM_INT_RDCOLERR            (EFM_FITE_RDCOLERRITE)    /*!< Read collide error Interrupt source        */
/**
 * @}
 */

/**
 * @defgroup EFM_Keys EFM Keys
 * @{
 */
#define EFM_KEY1                    (0x00000123ul)
#define EFM_KEY2                    (0x00003210ul)
/**
 * @}
 */

/**
 * @defgroup EFM_State EFM State
 * @{
 */
#define EFM_LOCK                    (0x00000000ul)
#define EFM_UNLOCK                  (0x00000001ul)
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
 * @addtogroup EFM_Global_Functions
 * @{
 */

void EFM_Unlock(void);
void EFM_Lock(void);

void EFM_Cmd(en_functional_state_t enNewState);
void EFM_SetLatency(uint32_t u32Latency);
void EFM_SetBusState(uint32_t u32State);
void EFM_CacheCmd(en_functional_state_t enNewState);
void EFM_DataCacheRstCmd(en_functional_state_t enNewState);
void EFM_InstructionCacheCmd(en_functional_state_t enNewState);
void EFM_SetWinProtectAddr(stc_efm_win_protect_addr_t stcAddr);
void EFM_InterruptCmd(uint32_t u32EfmInt, en_functional_state_t enNewState);

en_result_t EFM_StrucInit(stc_efm_cfg_t *pstcEfmCfg);
en_result_t EFM_Config(const stc_efm_cfg_t *pstcEfmCfg);

void EFM_SetOperateMode(uint32_t u32PeMode);
en_result_t EFM_ProgramWord(uint32_t u32Addr, uint32_t u32Data);
en_result_t EFM_ProgramHalfWord(uint32_t u32Addr, uint16_t u16Data);
en_result_t EFM_ProgramByte(uint32_t u32Addr, uint8_t u8Data);
en_result_t EFM_ProgramWordRB(uint32_t u32Addr, uint32_t u32Data);
en_result_t EFM_ProgramHalfWordRB(uint32_t u32Addr, uint16_t u16Data);
en_result_t EFM_ProgramByteRB(uint32_t u32Addr, uint8_t u8Data);
en_result_t EFM_SequenceProgram(uint32_t u32Addr, uint32_t u32Len, void *pBuf);
en_result_t EFM_SectorErase(uint32_t u32Addr);
en_result_t EFM_ChipErase(void);

void EFM_ClearFlag(uint32_t u32Flag);
en_flag_status_t EFM_GetFlagStatus(uint32_t u32Flag);
stc_efm_unique_id_t EFM_ReadUID(void);
/**
 * @}
 */

#endif /* DDL_EFM_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_EFM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
