/**
 *******************************************************************************
 * @file  hc32m120_dma.c
 * @brief This file provides firmware functions to manage the Dynamic Memory
 *        Access (DMA).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-08       Chengy          First version
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
#include "hc32m120_dma.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_DMA DMA
 * @brief Dynamic Memory Access Driver Library
 * @{
 */

#if (DDL_DMA_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup DMA_Local_Macros DMA Local Macros
 * @{
 */
#define DMA_CH_REG(reg_base, ch)    (*(volatile uint32_t *)((uint32_t)(&(reg_base)) + ((ch) * 0x40ul)))

/**
 * @defgroup DMA_Check_Parameters_Validity DMA Check Parameters Validity
 * @{
 */
/* Parameter valid check for DMA Channel. */
#define IS_VALID_DMA_CH(x)                                                     \
(       ((x) == DMA_CHANNEL_0)                  ||                             \
        ((x) == DMA_CHANNEL_1))

/* Parameter valid check for DMA transfer data width. */
#define IS_VALID_DMA_TRN_WIDTH(x)                                              \
(       ((x) == DMA_DATAWIDTH_8BIT)             ||                             \
        ((x) == DMA_DATAWIDTH_16BIT)            ||                             \
        ((x) == DMA_DATAWIDTH_32BIT))

/* Parameter valid check for DMA source address mode. */
#define IS_VALID_DMA_SADDR_MODE(x)                                             \
(       ((x) == DMA_SRCADDRINC_FIX)             ||                             \
        ((x) == DMA_SRCADDRINC_INC)             ||                             \
        ((x) == DMA_SRCADDRINC_DEC))

/* Parameter valid check for DMA destination address mode. */
#define IS_VALID_DMA_DADDR_MODE(x)                                             \
(       ((x) == DMA_DESADDRINC_FIX)             ||                             \
        ((x) == DMA_DESADDRINC_INC)             ||                             \
        ((x) == DMA_DESADDRINC_DEC))

/* Parameter valid check for DMA link-list-pointer mode. */
#define IS_VALID_DMA_LLP_MODE(x)                                               \
(       ((x) == DMA_LLP_RUN)                    ||                             \
        ((x) == DMA_LLP_WAIT))

/* Parameter valid check for DMA source & destination address repeat selection. */
#define IS_VALID_DMA_RPT_SEL(x)                                                \
(       ((x) == DMA_RPTNSSEL_SRCRPT)            ||                             \
        ((x) == DMA_RPTNSSEL_DESRPT))

/* Parameter valid check for DMA source & destination address non_sequence selection. */
#define IS_VALID_DMA_NSEQ_SEL(x)                                               \
(       ((x) == DMA_RPTNSSEL_SRCNSEQ)           ||                             \
        ((x) == DMA_RPTNSSEL_DESNSEQ))

/* Parameter valid check for DMA error interrupt selection.  */
#define IS_VALID_DMA_ERR_IRQ_SEL(x)                                            \
(       ((x) == DMA_IRQ_REQERR)                ||                              \
        ((x) == DMA_IRQ_TRERR))

/* Parameter valid check for DMA complete interrupt selection.  */
#define IS_VALID_DMA_CPL_IRQ_SEL(x)                                            \
(       ((x) == DMA_IRQ_BTC)                   ||                              \
        ((x) == DMA_IRQ_TC))

/* Parameter valid check for DMA error flag.  */
#define IS_VALID_DMA_ERR_FLAG(x)                                               \
(       ((x) == DMA_FLAG_REQERR)                ||                             \
        ((x) == DMA_FLAG_TRERR))

/* Parameter valid check for DMA complete flag.  */
#define IS_VALID_DMA_CPL_FLAG(x)                                               \
(       ((x) == DMA_FLAG_BTC)                   ||                             \
        ((x) == DMA_FLAG_TC))

/* Parameter valid check for DMA status.  */
#define IS_VALID_DMA_STATUS(x)                                                 \
(       ((x) == DMA_STATUS_CH0BUSY)             ||                             \
        ((x) == DMA_STATUS_CH1BUSY)             ||                             \
        ((x) == DMA_STATUS_DMABUSY))
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
 * @defgroup DMA_Global_Functions DMA Global Functions
 * @{
 */

/**
 * @brief  Enable or disable the DMA.
 * @param  enNewState              The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void DMA_Cmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    /* Enable or Disable DMA */
    MODIFY_REG(M0P_DMA->EN, DMA_EN_EN, enNewState);
}

/**
 * @brief  Enable the specified DAM channel.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @retval None
 */
void DMA_ChannelEnable(uint8_t u8Ch)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));

    M0P_DMA->CHEN = 1ul << u8Ch;
}

/**
 * @brief  Disable the specified DAM channel.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @retval None
 */
void DMA_ChannelDisable(uint8_t u8Ch)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));

    M0P_DMA->CHENCLR = 1ul << u8Ch;
}

/**
 * @brief  Enable or disable the specified DMA error interrupt.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  enNewState           The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @param  u32Irq               The specified DMA interrupt.
 *           @arg DMA_IRQ_REQERR
 *           @arg DMA_IRQ_TRERR
 * @retval None
 */
void DMA_ErrIrqCmd(uint8_t u8Ch, uint32_t u32Irq, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_VALID_DMA_ERR_IRQ_SEL(u32Irq));

    switch (enNewState)
    {
        case Enable:
            CLEAR_BIT(M0P_DMA->INTMASK0, u32Irq << u8Ch);
            break;
        case Disable:
            SET_BIT(M0P_DMA->INTMASK0, u32Irq << u8Ch);
            break;
        default:
            break;
    }
}

/**
 * @brief  Enable or disable the specified DMA complete interrupt.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  enNewState           The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @param  u32Irq               The specified DMA interrupt.
 *           @arg DMA_IRQ_BTC
 *           @arg DMA_IRQ_TC
 * @retval None
 */
void DMA_CplIrqCmd(uint8_t u8Ch, uint32_t u32Irq, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_VALID_DMA_CPL_IRQ_SEL(u32Irq));

    switch (enNewState)
    {
        case Enable:
            CLEAR_BIT(M0P_DMA->INTMASK1, u32Irq << u8Ch);
            break;
        case Disable:
            SET_BIT(M0P_DMA->INTMASK1, u32Irq << u8Ch);
            break;
        default:
            break;
    }
}
/**
 * @brief  The configuration of the specified DAM channel.
 * @param  u8Ch                  The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  pstcChCfg             The configuration pointer.
 *           @arg u32DataWidth   The DMA transfer data width.
 *           @arg u32BlockSize   The DMA block size.
 *           @arg u32TransferCnt The DMA transfer count.
 *           @arg u32SrcAddr     The DMA source address.
 *           @arg u32DesAddr     The DMA destination address.
 *           @arg u32SrcInc      The source address increment mode.
 *           @arg u32DesInc      The destination address increment mode.
 * @retval None
 */
void DMA_ChannelCfg(uint8_t u8Ch, const stc_dma_ch_cfg_t* pstcChCfg)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DMA_TRN_WIDTH(pstcChCfg->u32DataWidth));
    DDL_ASSERT(IS_VALID_DMA_SADDR_MODE(pstcChCfg->u32SrcInc));
    DDL_ASSERT(IS_VALID_DMA_DADDR_MODE(pstcChCfg->u32DesInc));

    /* Set data width, transfer count, block size. */
    MODIFY_REG(DMA_CH_REG(M0P_DMA->CH0CTL0, u8Ch),
               DMA_CH0CTL0_BLKSIZE | DMA_CH0CTL0_CNT |DMA_CH0CTL0_HSIZE,
               pstcChCfg->u32BlockSize |
               (pstcChCfg->u32TransferCnt << DMA_CH0CTL0_CNT_POS) |
               pstcChCfg->u32DataWidth);
    /* Set source & destination address increment mode */
    MODIFY_REG(DMA_CH_REG(M0P_DMA->CH0CTL1, u8Ch),
               DMA_CH0CTL1_SINC | DMA_CH0CTL1_DINC,
               pstcChCfg->u32SrcInc | pstcChCfg->u32DesInc);
    /* Set source address. */
    WRITE_REG(DMA_CH_REG(M0P_DMA->SAR0, u8Ch), pstcChCfg->u32SrcAddr);
    /* Set destination address. */
    WRITE_REG(DMA_CH_REG(M0P_DMA->DAR0, u8Ch), pstcChCfg->u32DesAddr);
}

/**
 * @brief  Initialize the DMA repeat transfer.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32Cnt               The specified repeat count
 * @param  u32RptSel
 *           @arg DMA_RPTNSSEL_SRCRPT
 *           @arg DMA_RPTNSSEL_DESRPT
 * @retval None
 */
void DMA_RepeatInit(uint8_t u8Ch, uint32_t u32RptSel, uint32_t u32Cnt)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DMA_RPT_SEL(u32RptSel));

    /* Set repeat selection & repeat count, and enable repeat transfer */
    MODIFY_REG(DMA_CH_REG(M0P_DMA->CH0CTL1, u8Ch),
               DMA_CH0CTL1_RPTNSEN | DMA_CH0CTL1_RPTNSSEL | DMA_CH0CTL1_RPTNSCNT,
               DMA_RPTNSSEL_ENABLE | u32RptSel | u32Cnt << DMA_CH0CTL1_RPTNSCNT_POS);
}

/**
 * @brief  Initialize the DMA non_sequence transfer.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32Offset             The specified offset
 * @param  u32Cnt                The specified non_sequence count.
 * @param  u32NSeqSel
 *           @arg DMA_RPTNSSEL_SRCNSEQ
 *           @arg DMA_RPTNSSEL_DESNSEQ
 * @retval None
 */
void DMA_NonSeqInit(uint8_t u8Ch, uint32_t u32NSeqSel, uint32_t u32Offset, uint32_t u32Cnt)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DMA_NSEQ_SEL(u32NSeqSel));

    /* Set non_sequence selection & offset, and enable non_sequence transfer */
    MODIFY_REG(DMA_CH_REG(M0P_DMA->CH0CTL1, u8Ch),
               DMA_CH0CTL1_RPTNSEN | DMA_CH0CTL1_RPTNSSEL | DMA_CH0CTL1_RPTNSCNT | DMA_CH0CTL1_OFFSET,
               DMA_RPTNSSEL_ENABLE | u32NSeqSel | u32Cnt << DMA_CH0CTL1_RPTNSCNT_POS |
               u32Offset << DMA_CH0CTL1_OFFSET_POS);
}

/**
 * @brief  Initialize the DMA LLP(link listed pointer) transfer.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32LlpRun            The specified the DMA llp transfer mode.
 *           @arg DMA_LLP_RUN
 *           @arg DMA_LLP_WAIT
 * @param  u32Llp               The next transfer descriptor address.
 * @retval None
 */
void DMA_LlpInit(uint8_t u8Ch, uint32_t u32LlpRun, uint32_t u32Llp)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DMA_LLP_MODE(u32LlpRun));

    /* Set llp mode & next descriptor address, and enable llp transfer */
    MODIFY_REG(DMA_CH_REG(M0P_DMA->CH0CTL0, u8Ch),
               DMA_LLP_ENABLE | DMA_CH0CTL0_LLPRUN | DMA_CH0CTL0_LLP,
               DMA_LLP_ENABLE | u32LlpRun | u32Llp << (DMA_CH0CTL0_LLP_POS - 2ul));
}

/**
 * @brief  Set the source address of the specified ADM channel.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32Address           The specified source address.
 * @retval None
 */
void DMA_SetSrcAddress(uint8_t u8Ch, uint32_t u32Address)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));

    /* Set source address. */
    WRITE_REG(DMA_CH_REG(M0P_DMA->SAR0, u8Ch), u32Address);
}

/**
 * @brief  Set the destination address of the specified DMA channel.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32Address           The specified destination address.
 * @retval None
 */
void DMA_SetDesAddress(uint8_t u8Ch, uint32_t u32Address)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));

    /* Set destination address. */
    WRITE_REG(DMA_CH_REG(M0P_DMA->DAR0, u8Ch), u32Address);
}

/**
 * @brief  Set the block size of the specified DMA channel.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u8BlkSize           The specified block size.
 * @retval None
 */
void DMA_SetBlockSize(uint8_t u8Ch, uint8_t u8BlkSize)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));

    /* Set block size. */
    MODIFY_REG(DMA_CH_REG(M0P_DMA->CH0CTL0, u8Ch), DMA_CH0CTL0_BLKSIZE, u8BlkSize);
}

/**
 * @brief  Set the transfer count of the specified DMA channel.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u8Cnt                The specified transfer count.
 * @retval None
 */
void DMA_SetTransferCnt(uint8_t u8Ch, uint8_t u8Cnt)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));

    /* Set transfer count. */
    MODIFY_REG(DMA_CH_REG(M0P_DMA->CH0CTL0, u8Ch),
               DMA_CH0CTL0_CNT,
               (uint32_t)u8Cnt << DMA_CH0CTL0_CNT_POS);
}

/**
 * @brief  Set the DMA trigger source.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  enSrc                The DMA trigger source.
 * @retval None
 */
void DMA_SetTriggerSrc( uint8_t u8Ch, en_event_src_t enSrc)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));

    switch (u8Ch)
    {
        case DMA_CHANNEL_0:
            WRITE_REG(M0P_AOS->DMA0_TRGSEL, enSrc);
            break;
        case DMA_CHANNEL_1:
            WRITE_REG(M0P_AOS->DMA1_TRGSEL, enSrc);
            break;
    }
}

/**
 * @brief  Get the DMA error flag.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32Flag              The DMA error flag.
 *           @arg DMA_FLAG_REQERR
 *           @arg DMA_FLAG_TRERR
 * @retval en_flag_status_t
 */
en_flag_status_t DMA_GetErrFlag(uint8_t u8Ch, uint32_t u32Flag)
{
    uint32_t u32FlagState;

    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DMA_ERR_FLAG(u32Flag));

    u32FlagState = M0P_DMA->INTSTAT0 & (u32Flag << (uint32_t)u8Ch);

    return ((u32FlagState == 0ul) ? Reset : Set);
}

/**
 * @brief  Get the DMA complete flag.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32Flag              The DMA complete flag.
 *           @arg DMA_FLAG_BTC
 *           @arg DMA_FLAG_TC
 * @retval en_flag_status_t
 */
en_flag_status_t DMA_GetCplFlag(uint8_t u8Ch, uint32_t u32Flag)
{
    uint32_t u32FlagState;

    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DMA_CPL_FLAG(u32Flag));

    u32FlagState = M0P_DMA->INTSTAT1 & (u32Flag << (uint32_t)u8Ch);

    return ((u32FlagState == 0ul) ? Reset : Set);
}

/**
 * @brief  Clear the DMA error flag.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32Flag              The DMA error flag.
 *           @arg DMA_FLAG_REQERR
 *           @arg DMA_FLAG_TRERR
 * @retval en_flag_status_t
 */
void DMA_ClearErrFlag(uint8_t u8Ch, uint32_t u32Flag)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DMA_ERR_FLAG(u32Flag));

    SET_BIT(M0P_DMA->INTCLR1, (u32Flag << u8Ch));
}

/**
 * @brief  Clear the DMA complete flag.
 * @param  u8Ch                 The specified DMA channel.
 *           @arg  This parameter can be: DMA_CHANNEL_0 or DMA_CHANNEL_1.
 * @param  u32Flag              The DMA complete flag.
 *           @arg DMA_FLAG_BTC
 *           @arg DMA_FLAG_TC
 * @retval en_flag_status_t
 */
void DMA_ClearCplFlag(uint8_t u8Ch, uint32_t u32Flag)
{
    DDL_ASSERT(IS_VALID_DMA_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DMA_CPL_FLAG(u32Flag));

    SET_BIT(M0P_DMA->INTCLR1, (u32Flag << u8Ch));
}

/**
 * @brief  Get the DMA and DMA channel status.
 * @param  u32Status            The specified DMA and DMA channel status.
 *           @arg DMA_STATUS_CH0BUSY
 *           @arg DMA_STATUS_CH1BUSY
 *           @arg DMA_STATUS_DMABUSY
 * @retval en_flag_status_t
 */
en_flag_status_t DMA_GetStatus(uint32_t u32Status)
{
    uint32_t u32Stat;

    DDL_ASSERT(IS_VALID_DMA_STATUS(u32Status));

    u32Stat = M0P_DMA->CHSTAT & (u32Status);

    return ((u32Stat == 0ul) ? Reset : Set);
}


/**
 * @}
 */

#endif  /* DDL_DMA_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
