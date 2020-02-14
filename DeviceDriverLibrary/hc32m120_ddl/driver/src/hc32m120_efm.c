/**
 *******************************************************************************
 * @file  hc32m120_efm.c
 * @brief This file provides firmware functions to manage the Embedded Flash
 *        Management unit (EFM).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-05-06       Chengy          First version
   2020-02-14       Heqb            Modify EFM_InterruptCmd function for efm.c
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
#include "hc32m120_efm.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_EFM EFM
 * @brief Embedded Flash Management Driver Library
 * @{
 */

#if (DDL_EFM_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EFM_Local_Macros EFM Local Macros
 * @{
 */

/**
 * @defgroup EFM_Check_Parameters_Validity EFM Check Parameters Validity
 * @{
 */
/*  Parameter validity check for pointer. */
#define IS_VALID_POINTER(x)                     (NULL != (x))

/*  Parameter validity check for flash latency. */
#define IS_VALID_EFM_LATENCY(latency)                                          \
(       ((latency) == EFM_LATENCY_0)            ||                             \
        ((latency) == EFM_LATENCY_1))

/*  Parameter validity check for operate mode. */
#define IS_VALID_EFM_OPERATE_MD(MD)                                            \
(       ((MD) == EFM_MODE_PROGRAMSINGLE)        ||                             \
        ((MD) == EFM_MODE_PROGRAMREADBACK)      ||                             \
        ((MD) == EFM_MODE_PROGRAMSEQUENCE)      ||                             \
        ((MD) == EFM_MODE_ERASESECTOR)          ||                             \
        ((MD) == EFM_MODE_ERASECHIP)            ||                             \
        ((MD) == EFM_MODE_READONLY)             ||                             \
        ((MD) == EFM_MODE_READONLY1)            ||                             \
        ((MD) == EFM_MODE_READONLY2))

/*  Parameter validity check for flash interrupt select. */
#define IS_VALID_EFM_INT_SEL(SEL)                                              \
(       ((SEL) == EFM_INT_EOP)                  ||                             \
        ((SEL) == EFM_INT_PEERR)                ||                             \
        ((SEL) == EFM_INT_RDCOLERR)             ||                             \
        ((SEL) == EFM_INT_EOP_PEERR)            ||                             \
        ((SEL) == EFM_INT_EOP_RDCOLERR)         ||                             \
        ((SEL) == EFM_INT_PEERR_RDCOLERR)       ||                             \
        ((SEL) == EFM_INT_ALL))

/*  Parameter validity check for flash flag. */
#define IS_VALID_EFM_FLAG(flag)                                                \
(       ((flag) == EFM_FLAG_PEWERR)             ||                             \
        ((flag) == EFM_FLAG_PEPRTERR)           ||                             \
        ((flag) == EFM_FLAG_PGMISMTCHERR)       ||                             \
        ((flag) == EFM_FLAG_OPTEND)             ||                             \
        ((flag) == EFM_FLAG_COLERR)             ||                             \
        ((flag) == EFM_FLAG_RDY))

/*  Parameter validity check for bus state while flash program or erase. */
#define IS_VALID_EFM_BUS_STATE(state)                                          \
(       ((state) == EFM_BUS_BUSY)               ||                             \
        ((state) == EFM_BUS_RELEASE))

/*  Parameter validity check for efm data cache reset state. */
#define IS_VALID_EFM_CACHERST_STATE(state)                                     \
(       ((state) == EFM_CACHERST_ON)            ||                             \
        ((state) == EFM_CACHERST_OFF))

/*  Parameter validity check for efm instruction cache state. */
#define IS_VALID_EFM_INSCACHE_STATE(state)                                     \
(       ((state) == EFM_INSCACHE_ON)            ||                             \
        ((state) == EFM_INSCACHE_OFF))

/*  Parameter validity check for efm data cache state. */
#define IS_VALID_EFM_CACHEEN_STATE(state)                                      \
(       ((state) == EFM_CACHEEN_ON)             ||                             \
        ((state) == EFM_CACHEEN_OFF))

#define IS_VALID_EFM_ADDR(addr)                                                \
(       (((addr) == EFM_START_ADDR)             ||                             \
        ((addr) >= (EFM_START_ADDR + 1u)))      &&                             \
        ((addr) <= EFM_END_ADDR))

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
 * @defgroup EFM_Global_Functions EFM Global Functions
 * @{
 */

/**
 * @brief  EFM unlock.
 * @param  None
 * @retval None
 */
void EFM_Unlock(void)
{
    M0P_EFM->FAPRT = EFM_KEY1;
    M0P_EFM->FAPRT = EFM_KEY2;
}
/**
 * @brief  EFM Lock.
 * @param  None
 * @retval None
 */
void EFM_Lock(void)
{
    M0P_EFM->FAPRT = 0x00001111U;
}

/**
 * @brief  Enable or disable EFM.
 * @param  [in] enNewState                The function new state.
 *   @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void EFM_Cmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    M0P_EFM->FSTP = ((Enable == enNewState) ? EFM_ON : EFM_OFF);
}

/**
 * @brief  Init efm config structure with default value.
 * @param  [in] pstcEfmCfg         The pointer of efm config structure.
 *   @arg u32Latency    Specifies the efm latency.
 *   @arg u32InsCache   Specifies the instruction cache on or off.
 *   @arg u32BusState   Specifies the bus state busy or release while program & erase.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t EFM_StrucInit(stc_efm_cfg_t *pstcEfmCfg)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcEfmCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcEfmCfg->u32Latency  = EFM_LATENCY_1;
        pstcEfmCfg->u32InsCache = EFM_INSCACHE_OFF;
        pstcEfmCfg->u32BusState = EFM_BUS_BUSY;
    }

    return enRet;
}

/**
 * @brief  Efm Config.
 * @param  [in] pstcEfmCfg         The pointer of efm config structure.
 *   @arg u32Latency    Specifies the efm latency.
 *   @arg u32InsCache   Specifies the instruction cache on or off.
 *   @arg u32BusState   Specifies the bus state busy or release while program & erase.
 * @retval An en_result_t enumeration value:
 *           - Ok: Configure success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t EFM_Config(const stc_efm_cfg_t *pstcEfmCfg)
{
    en_result_t enRet = Ok;
    /* Check if pointer is NULL */
    if (NULL == pstcEfmCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Param valid check */
        DDL_ASSERT(IS_VALID_EFM_LATENCY(pstcEfmCfg->u32Latency));
        DDL_ASSERT(IS_VALID_EFM_BUS_STATE(pstcEfmCfg->u32BusState));
        DDL_ASSERT(IS_VALID_EFM_CACHERST_STATE(pstcEfmCfg->u32CacheRst));
        DDL_ASSERT(IS_VALID_EFM_INSCACHE_STATE(pstcEfmCfg->u32InsCache));
        DDL_ASSERT(IS_VALID_EFM_CACHEEN_STATE(pstcEfmCfg->u32CacheEn));

        /* Config efm. */
        MODIFY_REG(M0P_EFM->FRMC, EFM_FRMC_FLWT | EFM_FRMC_CACHE | EFM_FRMC_PREFETE | EFM_FRMC_CRST0,
                pstcEfmCfg->u32Latency | pstcEfmCfg->u32CacheEn |
                pstcEfmCfg->u32InsCache | pstcEfmCfg->u32CacheRst);
        MODIFY_REG(M0P_EFM->FWMC, EFM_FWMC_BUSHLDCTL, pstcEfmCfg->u32BusState);
    }

    return enRet;
}

/**
 * @brief  Set the code latency value.
 * @param  [in] u32Latency              specifies the FLASH Latency value.
 *  This parameter can be one of the following values:
 *   @arg    EFM_LATENCY_0               FLASH 0 Latency cycle
 *   @arg    EFM_LATENCY_1               FLASH 1 Latency cycle
 * @retval None
 */
void EFM_SetLatency(uint32_t u32Latency)
{
    /* Param valid check */
    DDL_ASSERT(IS_VALID_EFM_LATENCY(u32Latency));

    /* Set the code latency value. */
    MODIFY_REG(M0P_EFM->FRMC, EFM_FRMC_FLWT, u32Latency);
}

/**
 * @brief  Enable or disable the flash data cache reset.
 * @param  [in] enNewState                The new state of the flash data cache reset.
 *   @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void EFM_DataCacheRstCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_BIT(M0P_EFM->FRMC, EFM_FRMC_CRST0);
    }
    else
    {
        CLEAR_BIT(M0P_EFM->FRMC, EFM_FRMC_CRST0);
    }
}

/**
 * @brief  Enable or disable the flash instruction cache.
 * @param  [in] enNewState                The new state of the flash instruction cache.
 *   @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void EFM_InstructionCacheCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_BIT(M0P_EFM->FRMC, EFM_FRMC_PREFETE);
    }
    else
    {
        CLEAR_BIT(M0P_EFM->FRMC, EFM_FRMC_PREFETE);
    }
}

/**
 * @brief  Enable or disable the flash data cache.
 * @param  [in] enNewState                The new state of the flash data cache.
 *   @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void EFM_CacheCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_BIT(M0P_EFM->FRMC, EFM_FRMC_CACHE);
    }
    else
    {
        CLEAR_BIT(M0P_EFM->FRMC, EFM_FRMC_CACHE);
    }
}

/**
 * @brief  Checks whether the specified FLASH flag is set or not..
 * @param  [in] u32PeMode                Specifies the FLASH flag to check.
 *  This parameter can be one of the following values:
 *   @arg  EFM_MODE_PROGRAMSINGLE           Program single at a specified address
 *   @arg  EFM_MODE_PROGRAMREADBACK         Program and read back at a specified address
 *   @arg  EFM_MODE_PROGRAMSEQUENCE         Program sequence at sequence address
 *   @arg  EFM_MODE_ERASESECTOR             Sector erase
 *   @arg  EFM_MODE_ERASECHIP               Chip erase
 *   @arg  EFM_MODE_READONLY                Read only
 *   @arg  EFM_MODE_READONLY1               Read only
 *   @arg  EFM_MODE_READONLY2               Read only
 * @retval None
 */
void EFM_SetOperateMode(uint32_t u32PeMode)
{
    DDL_ASSERT(IS_VALID_EFM_OPERATE_MD(u32PeMode));

    /* Enable operate mode modified. */
    SET_BIT(M0P_EFM->FWMC, EFM_FWMC_PEMODE);
    /* Set the program or erase mode. */
    MODIFY_REG(M0P_EFM->FWMC, EFM_FWMC_PEMOD, u32PeMode);
    /* Disable operate mode modified. */
    CLEAR_BIT(M0P_EFM->FWMC, EFM_FWMC_PEMODE);
}

/**
 * @brief  Enable or Disable EFM interrupt.
 * @param  [in] u32EfmInt               Specifies the FLASH flag to check.
 *  This parameter can be one of the following values:
 *   @arg  EFM_INT_EOP                  End of EFM Operation Interrupt source
 *   @arg  EFM_INT_PEERR                Program/erase error Interrupt source
 *   @arg  EFM_INT_RDCOLERR             Read collide error Interrupt source
 *   @arg  EFM_INT_EOP_PEERR            End of EFM Operation and Program/erase error Interrupt
 *   @arg  EFM_INT_EOP_RDCOLERR         End of EFM Operation and Read collide error Interrupt
 *   @arg  EFM_INT_PEERR_RDCOLERR       Program/erase error and Read collide error Interrupt
 *   @arg  EFM_INT_ALL                  All Interrupt source
 * @param  [in] enNewState              The new state of specified interrupt.
 *  This parameter can be: Enable or Disable.
 * @retval None
 */
void EFM_InterruptCmd(uint32_t u32EfmInt, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_VALID_EFM_INT_SEL(u32EfmInt));

    if(Enable == enNewState)
    {
        SET_REG32_BIT(M0P_EFM->FITE, u32EfmInt);
    }
    else
    {
        CLEAR_REG32_BIT(M0P_EFM->FITE, u32EfmInt);
    }
}

/**
 * @brief  Checks whether the specified FLASH flag is set or not.
 * @param  [in] u32flag                  Specifies the FLASH flag to check.
 *  This parameter can be one of the following values:
 *   @arg  EFM_FLAG_PEWERR               Flash wirte protect error flag.
 *   @arg  EFM_FLAG_PEPRTERR             Flash program protect area error flag.
 *   @arg  EFM_FLAG_PGMISMTCHERR         Flash program miss match flag.
 *   @arg  EFM_FLAG_OPTEND               Flash end of program flag.
 *   @arg  EFM_FLAG_COLERR               Flash read write error flag.
 *   @arg  EFM_FLAG_RDY                  Flash ready flag.
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Interrupt flag is set
 *           - Reset: Interrupt flag is reset
 */
en_flag_status_t EFM_GetFlagStatus(uint32_t u32flag)
{
    DDL_ASSERT(IS_VALID_EFM_FLAG(u32flag));

    return ((0ul == (M0P_EFM->FSR & u32flag)) ? Reset :Set);
}

/**
 * @brief  Clear the flash flag.
 * @param  [in] u32flag                  Specifies the FLASH flag to clear.
 *  This parameter can be one of the following values:
 *   @arg  EFM_FLAG_CLR_PEWERRCLR        Flash wirte protect error flag.
 *   @arg  EFM_FLAG_CLR_PEPRTERRCLR      Flash program protect area error flag.
 *   @arg  EFM_FLAG_CLR_PGMISMTCHCLR     Flash program miss match flag.
 *   @arg  EFM_FLAG_CLR_OPTENDCLR        Flash end of program flag.
 *   @arg  EFM_FLAG_CLR_COLERRCLR         Flash read write error flag.
 * @retval None
 */
void EFM_ClearFlag(uint32_t u32flag)
{
    SET_BIT(M0P_EFM->FSCLR, u32flag);
}

/**
 * @brief  Set flash protect area.
 * @param  [in] stcAddr                  Specifies the FLASH protect area.
 *   @arg  u32StartAddr                     Start address of protect area.
 *   @arg  u32EndAddr                       End address of protect area.
 * @retval None
 */
void EFM_SetWinProtectAddr(stc_efm_win_protect_addr_t stcAddr)
{
    /* Set protect area start address */
    MODIFY_REG(M0P_EFM->FPMTSW, EFM_FPMTSW_FPMTSW, stcAddr.u32StartAddr);
    /* Set protect area end address */
    MODIFY_REG(M0P_EFM->FPMTEW, EFM_FPMTEW_FPMTEW, stcAddr.u32EndAddr);
}

/**
 * @brief  Set bus state while flash program or erase.
 * @param  [in] u32State                  Specifies the new bus state while flash program or erase.
 *  This parameter can be one of the following values:
 *   @arg  EFM_BUS_BUSY                   Bus busy while flash program or erase.
 *   @arg  EFM_BUS_RELEASE                Bus releas while flash program or erase.
 * @retval None
 */
void EFM_SetBusState(uint32_t u32State)
{
    DDL_ASSERT(IS_VALID_EFM_BUS_STATE(u32State));

    MODIFY_REG(M0P_EFM->FWMC, EFM_FWMC_BUSHLDCTL, u32State);
}

/**
 * @brief  Flash single program word without read back.
 * @param  [in] u32Addr                   The specified program address.
 * @param  [in] u32Data                   The specified program data.
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - Error: program error
 *           - ErrorTimeout: program error timeout
 */
en_result_t EFM_ProgramWord(uint32_t u32Addr, uint32_t u32Data)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;

    DDL_ASSERT(IS_VALID_EFM_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set single program mode. */
    EFM_SetOperateMode(EFM_MODE_PROGRAMSINGLE);
    /* program data. */
    *(uint32_t*)u32Addr = u32Data;

    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }

    if(u32Data != *(uint32_t*)u32Addr)
    {
        enRet = Error;
    }

    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    return enRet;
}

/**
 * @brief  Flash single program half word without read back.
 * @param  [in] u32Addr                   The specified program address.
 * @param  [in] u16Data                   The specified program data.
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - Error: program error
 *           - ErrorTimeout: program error timeout
 */
en_result_t EFM_ProgramHalfWord(uint32_t u32Addr, uint16_t u16Data)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;

    DDL_ASSERT(IS_VALID_EFM_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set single program mode. */
    EFM_SetOperateMode(EFM_MODE_PROGRAMSINGLE);
    /* program data. */
    *((volatile uint16_t*)u32Addr) = u16Data;

    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }

    if(u16Data != *((volatile uint16_t*)u32Addr))
    {
        enRet = Error;
    }

    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    return enRet;
}

/**
 * @brief  Flash single program byte without read back.
 * @param  [in] u32Addr                   The specified program address.
 * @param  [in] u8Data                    The specified program data.
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - Error: program error
 *           - ErrorTimeout: program error timeout
 */
en_result_t EFM_ProgramByte(uint32_t u32Addr, uint8_t u8Data)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;

    DDL_ASSERT(IS_VALID_EFM_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set single program mode. */
    EFM_SetOperateMode(EFM_MODE_PROGRAMSINGLE);
    /* program data. */
    *((volatile uint8_t*)u32Addr) = u8Data;

    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }

    if(u8Data != *((volatile uint8_t*)u32Addr))
    {
        enRet = Error;
    }

    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    return enRet;
}

/**
 * @brief  Flash single program word with read back.
 * @param  [in] u32Addr                   The specified program address.
 * @param  [in] u32Data                   The specified program data.
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - Error: program error
 *           - ErrorTimeout: program error timeout
 */
en_result_t EFM_ProgramWordRB(uint32_t u32Addr, uint32_t u32Data)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;

    DDL_ASSERT(IS_VALID_EFM_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set single program mode. */
    EFM_SetOperateMode(EFM_MODE_PROGRAMREADBACK);
    /* program data. */
    *(uint32_t*)u32Addr = u32Data;

    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }

    if(Set == EFM_GetFlagStatus(EFM_FLAG_PGMISMTCHERR))
    {
        /* clear flag PGMISMTCH */
        EFM_ClearFlag(EFM_FLAG_CLR_PGMISMTCHCLR);
        enRet = Error;
    }

    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    return enRet;
}

/**
 * @brief  Flash single program half word with read back.
 * @param  [in] u32Addr                   The specified program address.
 * @param  [in] u16Data                   The specified program data.
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - Error: program error
 *           - ErrorTimeout: program error timeout
 */
en_result_t EFM_ProgramHalfWordRB(uint32_t u32Addr, uint16_t u16Data)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;

    DDL_ASSERT(IS_VALID_EFM_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set single program mode. */
    EFM_SetOperateMode(EFM_MODE_PROGRAMREADBACK);
    /* program data. */
    *((volatile uint16_t*)u32Addr) = u16Data;

    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }

    if(Set == EFM_GetFlagStatus(EFM_FLAG_PGMISMTCHERR))
    {
        /* clear flag PGMISMTCH */
        EFM_ClearFlag(EFM_FLAG_CLR_PGMISMTCHCLR);
        enRet = Error;
    }

    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    return enRet;
}

/**
 * @brief  Flash single program byte with read back.
 * @param  [in] u32Addr                   The specified program address.
 * @param  [in] u8Data                    The specified program data.
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - Error: program error
 *           - ErrorTimeout: program error timeout
 */
en_result_t EFM_ProgramByteRB(uint32_t u32Addr, uint8_t u8Data)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;

    DDL_ASSERT(IS_VALID_EFM_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set single program mode. */
    EFM_SetOperateMode(EFM_MODE_PROGRAMREADBACK);
    /* program data. */
    *((volatile uint8_t*)u32Addr) = u8Data;

    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }

    if(Set == EFM_GetFlagStatus(EFM_FLAG_PGMISMTCHERR))
    {
        /* clear flag PGMISMTCH */
        EFM_ClearFlag(EFM_FLAG_CLR_PGMISMTCHCLR);
        enRet = Error;
    }

    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    return enRet;
}

/**
 * @brief  Flash sequence program.
 * @param  [in] u32Addr                   The specified program address.
 * @param  [in] u32Len                    The len of specified program data.
 * @param  [in] *pBuf                     The pointer of specified program data.
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - ErrorTimeout: program error timeout
 */
en_result_t EFM_SequenceProgram(uint32_t u32Addr, uint32_t u32Len, void *pBuf)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;
    uint32_t *u32pSrc = pBuf;
    uint32_t *u32pDest = (uint32_t *)u32Addr;
    uint32_t u32LoopWords = u32Len >> 2;
    uint32_t u32RemainBytes = u32Len % 4ul;

    DDL_ASSERT(IS_VALID_EFM_ADDR(u32Addr));
    DDL_ASSERT(IS_VALID_POINTER(pBuf));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set sequence program mode. */
    EFM_SetOperateMode(EFM_MODE_PROGRAMSEQUENCE);

    /* program data. */
    while(u32LoopWords--)
    {
        *u32pDest++ = *u32pSrc++;
        /* wait operate end. */
        while(Set != EFM_GetFlagStatus(EFM_FLAG_OPTEND))
        {
            u16tmp++;
            if(u16tmp > EFM_TIMEOUT)
            {
                enRet = ErrorTimeout;
            }
        }
        /* clear end flag. */
        EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    }
    if(u32RemainBytes)
    {
        uint8_t *u8pDest = (uint8_t*)u32pDest;
        uint8_t *u8pSrc = (uint8_t*)u32pSrc;
        while(u32RemainBytes--)
        {
            *u8pDest++ = *u8pSrc++;
        }
    }

    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    u16tmp = 0u;
    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }
    /* CLear the end of operate flag */
    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);

    return enRet;
}

/**
 * @brief  Flash sector erase.
 * @param  [in] u32Addr                   The uncertain(random) address in the specified sector.
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - ErrorTimeout: program error timeout
 * @note   The address should be word align.
 */
en_result_t EFM_SectorErase(uint32_t u32Addr)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;

    DDL_ASSERT(IS_VALID_EFM_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set sector erase mode. */
    EFM_SetOperateMode(EFM_MODE_ERASESECTOR);

    *(uint32_t*)u32Addr = 0ul;

    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }
    /* CLear the end of operate flag */
    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    return enRet;
}

/**
 * @brief  Flash chip erase.
 * @param  None
 * @retval An en_result_t enumeration value:
 *           - Ok: program success
 *           - ErrorTimeout: program error timeout
 * @note   The address should be word align.
 */
en_result_t EFM_ChipErase(void)
{
    en_result_t enRet = Ok;
    uint16_t u16tmp = 0u;

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_CLR_PEWERRCLR    | EFM_FLAG_CLR_PEPRTERRCLR |
                  EFM_FLAG_CLR_PGMISMTCHCLR | EFM_FLAG_CLR_OPTENDCLR   |
                  EFM_FLAG_CLR_COLERRCLR);

    /* Set sector erase mode. */
    EFM_SetOperateMode(EFM_MODE_ERASECHIP);

    *(uint32_t*)0 = 0ul;

    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        u16tmp ++;
        if(u16tmp > EFM_TIMEOUT)
        {
            enRet = ErrorTimeout;
        }
    }
    /* CLear the end of operate flag */
    EFM_ClearFlag(EFM_FLAG_CLR_OPTENDCLR);
    /* Set read only mode. */
    EFM_SetOperateMode(EFM_MODE_READONLY);

    return enRet;
}

/**
 * @brief  Read unique ID.
 * @param  None
 * @retval An stc_efm_unique_id_t structure value:
 *           - uniqueID0: unique ID 0
 *           - uniqueID1: unique ID 1
 *           - uniqueID2: unique ID 2
 */
stc_efm_unique_id_t EFM_ReadUID(void)
{
    stc_efm_unique_id_t stcUID;

    stcUID.u32UniqueID0 = M0P_EFM->UQID0;
    stcUID.u32UniqueID1 = M0P_EFM->UQID1;
    stcUID.u32UniqueID2 = M0P_EFM->UQID2;

    return stcUID;
}

/**
 * @}
 */

#endif  /* DDL_EFM_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
