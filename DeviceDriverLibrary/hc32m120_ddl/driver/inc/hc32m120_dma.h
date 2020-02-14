/**
 *******************************************************************************
 * @file  hc32m120_dma.h
 * @brief This file contains all the functions prototypes of the DMA driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-08       Chengy          First version
   2020-02-13       Chengy          Fixed spelling error and format in comment
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
#ifndef __HC32M120_DMA_H__
#define __HC32M120_DMA_H__

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
 * @addtogroup DDL_DMA
 * @{
 */

#if (DDL_DMA_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup DMA_Global_Types DMA Global Types
 * @{
 */

/**
 * @brief  DMA channel configuration
 */
typedef struct
{
    uint32_t    u32DataWidth;   /*!< Specifies the DMA transfer data width.
                                    This parameter can be a value of @ref DMA_DataWidth                 */

    uint32_t    u32BlockSize;   /*!< Specifies the DMA block size.                                      */

    uint32_t    u32TransferCnt; /*!< Specifies the DMA transfer count.                                  */

    uint32_t    u32SrcAddr;     /*!< Specifies the DMA source address.                                  */

    uint32_t    u32DesAddr;     /*!< Specifies the DMA destination address.                             */

    uint32_t    u32SrcInc;      /*!< Specifies the source address increment mode.
                                    This parameter can be a value of @ref DMA_SrcAddr_Incremented_Mode  */

    uint32_t    u32DesInc;      /*!< Specifies the destination address increment mode.
                                    This parameter can be a value of @ref DMA_DesAddr_Incremented_Mode  */

} stc_dma_ch_cfg_t;

/**
 * @brief  Dma LLP(linked list pointer) descriptor structure definition
 */
typedef struct
{
    uint32_t SARx;                              /*!< DMA source address register.       */
    uint32_t DARx;                              /*!< DMA destination address register.  */
    union
    {
        uint32_t CHxCTL0;
        stc_dma_ch0ctl0_field_t CHxCTL0_f;      /*!< DMA data control register.         */
    };
    union
    {
        uint32_t CHxCTL1;
        stc_dma_ch0ctl1_field_t CHxCTL1_f;      /*!< DMA channel control register.      */
    };
} stc_dma_llp_descriptor_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CLK_Global_Macros CLK Global Macros
 * @{
 */

/**
 * @defgroup DMA_Channel_En DMA Channel enable or disable
 * @{
 */
#define DMA_CHANNEL_ENABLE          (DMA_CHEN_CHEN_0)         /*!< DMA channel 0 enable      */
#define DMA_CHANNEL_DISABLE         ((uint32_t)0x00000000ul)  /*!< DMA channel disable       */
/**
 * @}
 */

/**
 * @defgroup DMA_Channel_selection DMA Channel selection
 * @{
 */
#define DMA_CHANNEL_0               (0x00u)       /*!< DMA Channel 0              */
#define DMA_CHANNEL_1               (0x01u)       /*!< DMA Channel 1              */
/**
 * @}
 */

/**
 * @defgroup DMA_Irq_Sel DMA interrupt selection
 * @{
 */
#define DMA_IRQ_REQERR              (DMA_INTMASK0_MSKREQERR_0)   /*!< DMA request error interrupt             */
#define DMA_IRQ_TRERR               (DMA_INTMASK0_MSKTRNERR_0)   /*!< DMA transfer error interrupt            */
#define DMA_IRQ_BTC                 (DMA_INTMASK1_MSKBTC_0)      /*!< DMA block transfer complete interrupt   */
#define DMA_IRQ_TC                  (DMA_INTMASK1_MSKTC_0)       /*!< DMA transfer complete interrupt         */
/**
 * @}
 */

/**
 * @defgroup DMA_Flag DMA Flag
 * @{
 */
#define DMA_FLAG_REQERR             (DMA_INTSTAT0_REQERR_0)   /*!< DMA request error flag             */
#define DMA_FLAG_TRERR              (DMA_INTSTAT0_TRNERR_0)   /*!< DMA transfer error flag            */
#define DMA_FLAG_BTC                (DMA_INTSTAT1_BTC_0)      /*!< DMA block transfer complete flag   */
#define DMA_FLAG_TC                 (DMA_INTSTAT1_TC_0)       /*!< DMA transfer complete flag         */
/**
 * @}
 */

/**
 * @defgroup DMA_Status DMA Status
 * @{
 */
#define DMA_STATUS_CH0BUSY          (DMA_CHSTAT_CHACT_0)      /*!< DMA channel 0 busy status       */
#define DMA_STATUS_CH1BUSY          (DMA_CHSTAT_CHACT_1)      /*!< DMA channel 1 busy status       */
#define DMA_STATUS_DMABUSY          (DMA_CHSTAT_DMAACT)       /*!< DMA busy status                 */
/**
 * @}
 */

/**
 * @defgroup DMA_DataWidth DMA transfer data width
 * @{
 */
#define DMA_DATAWIDTH_8BIT          ((uint32_t)0x00000000ul)  /*!< DMA transfer data width 8bit       */
#define DMA_DATAWIDTH_16BIT         (DMA_CH0CTL0_HSIZE_0)     /*!< DMA transfer data width 16bit      */
#define DMA_DATAWIDTH_32BIT         (DMA_CH0CTL0_HSIZE_1)     /*!< DMA transfer data width 32bit      */
/**
 * @}
 */

/**
 * @defgroup DMA_Llp_En DMA LLP(linked list pointer) enable or disable
 * @{
 */
#define DMA_LLP_ENABLE              (DMA_CH0CTL0_LLPEN)       /*!< DMA LLP(linked list pointer) enable     */
#define DMA_LLP_DISABLE             ((uint32_t)0x00000000ul)  /*!< DMA LLP(linked list pointer) disable    */
/**
 * @}
 */

/**
 * @defgroup DMA_Llp_Mode DMA linked list pointer mode while transferring complete
 * @{
 */
#define DMA_LLP_RUN                 (DMA_CH0CTL0_LLPRUN)      /*!< DMA Llp run right now while transfering complete     */
#define DMA_LLP_WAIT                ((uint32_t)0x00000000ul)  /*!< DMA Llp wait next request while transfering complete */
/**
 * @}
 */

/**
 * @defgroup DMA_SrcAddr_Incremented_Mode DMA source address increment mode
 * @{
 */
#define DMA_SRCADDRINC_FIX          ((uint32_t)0x00000000ul)  /*!< DMA source address fix             */
#define DMA_SRCADDRINC_INC          (DMA_CH0CTL1_SINC_0)      /*!< DMA source address increment       */
#define DMA_SRCADDRINC_DEC          (DMA_CH0CTL1_SINC_1)      /*!< DMA source address decrement       */
/**
 * @}
 */

/**
 * @defgroup DMA_DesAddr_Incremented_Mode DMA destination address increment mode
 * @{
 */
#define DMA_DESADDRINC_FIX          ((uint32_t)0x00000000ul)  /*!< DMA destination address fix        */
#define DMA_DESADDRINC_INC          (DMA_CH0CTL1_DINC_0)      /*!< DMA destination address increment  */
#define DMA_DESADDRINC_DEC          (DMA_CH0CTL1_DINC_1)      /*!< DMA destination address decrement  */
/**
 * @}
 */

/**
 * @defgroup DMA_Rpt_Nonseq_State DMA source & destination address repeat & non_sequence state
 * @{
 */
#define DMA_RPTNSSEL_DISABLE        ((uint32_t)0x00000000ul)  /*!< DMA disbale address repeat or non_sequence */
#define DMA_RPTNSSEL_ENABLE         (DMA_CH0CTL1_RPTNSEN)     /*!< DMA enable address repeat or non_sequence  */
/**
 * @}
 */

/**
 * @defgroup DMA_Rpt_Nonseq_Sel DMA source & destination address repeat & non_sequence selection
 * @{
 */
#define DMA_RPTNSSEL_SRCRPT         ((uint32_t)0x00000000ul)  /*!< DMA source address repeat              */
#define DMA_RPTNSSEL_DESRPT         (DMA_CH0CTL1_RPTNSSEL_0)  /*!< DMA destination address repeat         */
#define DMA_RPTNSSEL_SRCNSEQ        (DMA_CH0CTL1_RPTNSSEL_1)  /*!< DMA source address non_sequence        */
#define DMA_RPTNSSEL_DESNSEQ        (DMA_CH0CTL1_RPTNSSEL)    /*!< DMA destination address non_sequence   */
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
 * @addtogroup DMA_Global_Functions
 * @{
 */
void DMA_Cmd(en_functional_state_t enNewState);
void DMA_ChannelEnable(uint8_t u8Ch);
void DMA_ChannelDisable(uint8_t u8Ch);
void DMA_ErrIrqCmd(uint8_t u8Ch, uint32_t u32Irq, en_functional_state_t enNewState);
void DMA_CplIrqCmd(uint8_t u8Ch, uint32_t u32Irq, en_functional_state_t enNewState);

void DMA_ChannelCfg(uint8_t u8Ch, const stc_dma_ch_cfg_t* pstcChCfg);

void DMA_RepeatInit(uint8_t u8Ch, uint32_t u32RptSel, uint32_t u32Cnt);
void DMA_NonSeqInit(uint8_t u8Ch, uint32_t u32NSeqSel, uint32_t u32Offset, uint32_t u32Cnt);
void DMA_LlpInit(uint8_t u8Ch, uint32_t u32LlpRun, uint32_t u32Llp);

void DMA_SetSrcAddress(uint8_t u8Ch, uint32_t u32Address);
void DMA_SetDesAddress(uint8_t u8Ch, uint32_t u32Address);
void DMA_SetBlockSize(uint8_t u8Ch, uint8_t u8BlkSize);
void DMA_SetTransferCnt(uint8_t u8Ch, uint8_t u8Cnt);
void DMA_SetTriggerSrc( uint8_t u8Ch, en_event_src_t enSrc);

void DMA_ClearErrFlag(uint8_t u8Ch, uint32_t u32Flag);
void DMA_ClearCplFlag(uint8_t u8Ch, uint32_t u32Flag);
en_flag_status_t DMA_GetErrFlag(uint8_t u8Ch, uint32_t u32Flag);
en_flag_status_t DMA_GetCplFlag(uint8_t u8Ch, uint32_t u32Flag);
en_flag_status_t DMA_GetStatus(uint32_t u32Status);
/**
 * @}
 */

#endif /* DDL_DMA_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_DMA_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
