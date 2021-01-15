/**
 *******************************************************************************
 * @file  hc32m120_interrupts.h
 * @brief This file contains all the functions prototypes of the interrupt driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-21       Zhangxl         First version
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
#ifndef __HC32M120_INTERRUPTS_H__
#define __HC32M120_INTERRUPTS_H__

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
 * @addtogroup DDL_INTERRUPTS
 * @{
 */

#if (DDL_INTERRUPTS_ENABLE == DDL_ON) || (DDL_EXINT_NMI_ENABLE == DDL_ON) ||    \
    (DDL_EKEY_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup INTC_Global_Types INTC Global Types
 * @{
 */
/**
 * @brief  Interrupt registration structure definition
 */
typedef struct
{
    en_int_src_t    enIntSrc;   /*!< Peripheral interrupt number,
                                     can be any value except INT_PORT_EIRQ0~7 @ref en_int_src_t         */
    IRQn_Type       enIRQn;     /*!< Peripheral IRQ type, can be Int008_IRQn~Int023_IRQn @ref IRQn_Type */
    func_ptr_t      pfnCallback;/*!< Callback function for corresponding peripheral IRQ                 */
}stc_irq_regi_config_t;

/**
 * @brief  NMI initialize configuration structure definition
 */
typedef struct
{
    uint8_t     u8NmiFE;        /*!< NMI pin filter function, @ref NMI_FilterClock_Sel for details          */
    uint8_t     u8NmiFClk;      /*!< NMI pin filter clock  division, @ref NMI_FilterClock_Div for details   */
    uint8_t     u8NmiTrigger;   /*!< NMI pin trigger edge, @ref NMI_Trigger_Sel for details                 */
    uint8_t     u8NmiSrc;       /*!< NMI trigger source, @ref NMI_TriggerSrc_Sel for details                */
    func_ptr_t  pfnNmiCallback; /*!< NMI Callback function pointers                                         */
}stc_nmi_config_t;

/**
 * @brief  EXINT initialize configuration structure definition
 */
typedef struct
{
    uint16_t    u16ExIntCh;     /*!< ExInt CH.0~9 @ref EXINT_Channel_Sel                                */
    uint8_t     u8ExIntFE;      /*!< ExInt filter clock, @ref EXINT_FilterClock_Sel for details         */
    uint8_t     u8ExIntFClk;    /*!< ExInt filter clock division, @ref EXINT_FilterClock_Div for details*/
    uint8_t     u8ExIntLvl;     /*!< ExInt trigger edge, @ref EXINT_Trigger_Sel for details             */
}stc_exint_config_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup INTC_Global_Macros INTC Global Macros
 * @{
 */
/**
 * @defgroup INTC_DefaultPriority_Sel Interrupt default priority level
 * Possible values are 0 (high priority) to 3 (low priority)
 * @{
 */
#define DDL_IRQ_PRIORITY_DEFAULT    (3u)
/**
 * @}
 */

/**
 * @defgroup INTC_Priority_Sel Interrupt priority level 00 ~ 03
 * @{
 */
#define DDL_IRQ_PRIORITY_00         (0u)
#define DDL_IRQ_PRIORITY_01         (1u)
#define DDL_IRQ_PRIORITY_02         (2u)
#define DDL_IRQ_PRIORITY_03         (3u)
/**
 * @}
 */

/**
 * @defgroup INTC_Bit_mask definition Bit Mask
 * @{
 */
#define     BIT_MASK_00             (uint32_t)(1ul << 0u)
#define     BIT_MASK_01             (uint32_t)(1ul << 1u)
#define     BIT_MASK_02             (uint32_t)(1ul << 2u)
#define     BIT_MASK_03             (uint32_t)(1ul << 3u)
#define     BIT_MASK_04             (uint32_t)(1ul << 4u)
#define     BIT_MASK_05             (uint32_t)(1ul << 5u)
#define     BIT_MASK_06             (uint32_t)(1ul << 6u)
#define     BIT_MASK_07             (uint32_t)(1ul << 7u)
#define     BIT_MASK_08             (uint32_t)(1ul << 8u)
#define     BIT_MASK_09             (uint32_t)(1ul << 9u)
#define     BIT_MASK_10             (uint32_t)(1ul << 10u)
#define     BIT_MASK_11             (uint32_t)(1ul << 11u)
#define     BIT_MASK_12             (uint32_t)(1ul << 12u)
#define     BIT_MASK_13             (uint32_t)(1ul << 13u)
#define     BIT_MASK_14             (uint32_t)(1ul << 14u)
#define     BIT_MASK_15             (uint32_t)(1ul << 15u)
#define     BIT_MASK_16             (uint32_t)(1ul << 16u)
#define     BIT_MASK_17             (uint32_t)(1ul << 17u)
#define     BIT_MASK_18             (uint32_t)(1ul << 18u)
#define     BIT_MASK_19             (uint32_t)(1ul << 19u)
#define     BIT_MASK_20             (uint32_t)(1ul << 20u)
#define     BIT_MASK_21             (uint32_t)(1ul << 21u)
#define     BIT_MASK_22             (uint32_t)(1ul << 22u)
#define     BIT_MASK_23             (uint32_t)(1ul << 23u)
#define     BIT_MASK_24             (uint32_t)(1ul << 24u)
#define     BIT_MASK_25             (uint32_t)(1ul << 25u)
#define     BIT_MASK_26             (uint32_t)(1ul << 26u)
#define     BIT_MASK_27             (uint32_t)(1ul << 27u)
#define     BIT_MASK_28             (uint32_t)(1ul << 28u)
#define     BIT_MASK_29             (uint32_t)(1ul << 29u)
#define     BIT_MASK_30             (uint32_t)(1ul << 30u)
#define     BIT_MASK_31             (uint32_t)(1ul << 31u)
/**
 * @}
 */

/**
 * @defgroup INTC_Register_Protect INTC Registers Protect Code
 * @{
 */
#define INTC_REG_UNPROTECT          ((uint8_t)0xA5u)
#define INTC_REG_PROTECT            ((uint8_t)0x00u)
/**
 * @}
 */

/**
 * @defgroup NMI_FilterClock_Sel NMI pin filter selection
 * @{
 */
#define NMI_FILTER_OFF              (uint8_t)(0u << INTC_NMICR_NMIFEN_POS)
#define NMI_FILTER_ON               (uint8_t)(1u << INTC_NMICR_NMIFEN_POS)
/**
 * @}
 */

/**
 * @defgroup NMI_FilterClock_Div NMI pin filter sampling clock division selection
 * @{
 */
#define NMI_FCLK_HCLK_DIV1          (uint8_t)(0u << INTC_NMICR_NMIFCLK_POS)
#define NMI_FCLK_HCLK_DIV8          (uint8_t)(1u << INTC_NMICR_NMIFCLK_POS)
#define NMI_FCLK_HCLK_DIV32         (uint8_t)(2u << INTC_NMICR_NMIFCLK_POS)
#define NMI_FCLK_HCLK_DIV64         (uint8_t)(3u << INTC_NMICR_NMIFCLK_POS)
/**
 * @}
 */

/**
 * @defgroup NMI_Trigger_Sel NMI pin trigger method selection
 * @{
 */
#define NMI_TRIGGER_FALLING         (uint8_t)(0u << INTC_NMICR_NMITRG_POS)
#define NMI_TRIGGER_RISING          (uint8_t)(1u << INTC_NMICR_NMITRG_POS)
/**
 * @}
 */

/**
 * @defgroup NMI_TriggerSrc_Sel NMI trigger source selection
 * @{
 */
#define NMI_SRC_NMI_PIN             (uint8_t)(1u << INTC_NMIER_NMIEN_POS)
#define NMI_SRC_XTAL_STP            (uint8_t)(1u << INTC_NMIER_XTALSTPEN_POS)
#define NMI_SRC_SWDT                (uint8_t)(1u << INTC_NMIER_SWDTEN_POS)
#define NMI_SRC_LVD                 (uint8_t)(1u << INTC_NMIER_PVDEN_POS)
#define NMI_SRC_MASK                (NMI_SRC_NMI_PIN | NMI_SRC_XTAL_STP |       \
                                    NMI_SRC_SWDT | NMI_SRC_LVD)
/**
 * @}
 */

/**
 * @defgroup MNI_Register_Msk NMI register mask
 * @{
 */
#define INTC_NMICR_MASK     (uint8_t)(INTC_NMICR_NMITRG | INTC_NMICR_NMIFCLK   |\
                            INTC_NMICR_NMIFEN)
#define INTC_NMIER_MASK     (uint8_t)(INTC_NMIER_NMIEN  | INTC_NMIER_XTALSTPEN |\
                            INTC_NMIER_SWDTEN  | INTC_NMIER_PVDEN)
#define INTC_NMIFR_MASK     (uint8_t)(INTC_NMIFR_NMIF   | INTC_NMIFR_XTALSTPF  |\
                            INTC_NMIFR_SWDTF   | INTC_NMIFR_PVDF)
#define INTC_NMICLR_MASK    (uint8_t)(INTC_NMICLR_NMICL | INTC_NMICLR_XTALSTPCL|\
                            INTC_NMICLR_SWDTCL | INTC_NMICLR_PVDCL)
/**
 * @}
 */

/**
 * @defgroup EXINT_Channel_Sel External interrupt channel selection
 * @{
 */
#define EXINT_CH00                  (uint16_t)(1ul << 0u)
#define EXINT_CH01                  (uint16_t)(1ul << 1u)
#define EXINT_CH02                  (uint16_t)(1ul << 2u)
#define EXINT_CH03                  (uint16_t)(1ul << 3u)
#define EXINT_CH04                  (uint16_t)(1ul << 4u)
#define EXINT_CH05                  (uint16_t)(1ul << 5u)
#define EXINT_CH06                  (uint16_t)(1ul << 6u)
#define EXINT_CH07                  (uint16_t)(1ul << 7u)
#define EXINT_CH08                  (uint16_t)(1ul << 8u)
#define EXINT_CH09                  (uint16_t)(1ul << 9u)
#define EXINT_CH_MASK   (EXINT_CH00 | EXINT_CH01 | EXINT_CH02 | EXINT_CH03 |    \
                         EXINT_CH04 | EXINT_CH05 | EXINT_CH06 | EXINT_CH07 |    \
                         EXINT_CH08 | EXINT_CH09)
/**
 * @}
 */

/**
 * @defgroup EXINT_FilterClock_Sel External interrupt filter function selection
 * @{
 */
#define EXINT_FILTER_OFF            (uint8_t)(0u << INTC_EIRQCR_EIRQFEN_POS)
#define EXINT_FILTER_ON             (uint8_t)(1u << INTC_EIRQCR_EIRQFEN_POS)
/**
 * @}
 */

/**
 * @defgroup EXINT_FilterClock_Div External interrupt filtersampling  clock division selection
 * @{
 */
#define EXINT_FCLK_HCLK_DIV1        (uint8_t)(0u << INTC_EIRQCR_EIRQFCLK_POS)
#define EXINT_FCLK_HCLK_DIV8        (uint8_t)(1u << INTC_EIRQCR_EIRQFCLK_POS)
#define EXINT_FCLK_HCLK_DIV32       (uint8_t)(2u << INTC_EIRQCR_EIRQFCLK_POS)
#define EXINT_FCLK_HCLK_DIV64       (uint8_t)(3u << INTC_EIRQCR_EIRQFCLK_POS)
/**
 * @}
 */

/**
 * @defgroup EXINT_Trigger_Sel External interrupt trigger method selection
 * @{
 */
#define EXINT_TRIGGER_LOW           (uint8_t)(0u << INTC_EIRQCR_EIRQTRG_POS)
#define EXINT_TRIGGER_RISING        (uint8_t)(1u << INTC_EIRQCR_EIRQTRG_POS)
#define EXINT_TRIGGER_FALLING       (uint8_t)(2u << INTC_EIRQCR_EIRQTRG_POS)
#define EXINT_TRIGGER_BOTH          (uint8_t)(3u << INTC_EIRQCR_EIRQTRG_POS)
/**
 * @}
 */

/**
 * @defgroup EXINT_Register_Msk EXINT register mask
 * @{
 */
#define INTC_EIRQF_MASK             (INTC_EIRQFR_EIRQF)
#define INTC_EIRQCLR_MASK           (INTC_EIRQCLR_EIRQCL)
/**
 * @}
 */

/**
 * @defgroup INTC_EKEY_Channel INTC EKEY channel selection
 * @{
 */
#define INTC_EKEY0                  (uint8_t)(1u << INTC_EKEYCR_EKEY0EN_POS)
#define INTC_EKEY1                  (uint8_t)(1u << INTC_EKEYCR_EKEY1EN_POS)
#define INTC_EKEY2                  (uint8_t)(1u << INTC_EKEYCR_EKEY2EN_POS)
#define INTC_EKEY3                  (uint8_t)(1u << INTC_EKEYCR_EKEY3EN_POS)
#define INTC_EKEY4                  (uint8_t)(1u << INTC_EKEYCR_EKEY4EN_POS)
#define INTC_EKEY5                  (uint8_t)(1u << INTC_EKEYCR_EKEY5EN_POS)
#define INTC_EKEY_MASK              (INTC_EKEY0 | INTC_EKEY1 | INTC_EKEY2 |     \
                                     INTC_EKEY3 | INTC_EKEY4 | INTC_EKEY5)
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
 * @addtogroup INTC_Global_Functions
 * @{
 */

/**
 * @brief  AOS software trigger.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void AOS_SW_Trigger(void)
{
    WRITE_REG32(bM0P_AOS->INTC_STRGCR_b.STRG, Set);
}

/**
 * @brief  INTC lock, register write disable
 * @param  None
 * @retval None
 */
__STATIC_INLINE void INTC_Lock(void)
{
    WRITE_REG32(M0P_INTC->FPRCR, INTC_REG_PROTECT);
}

/**
 * @brief  INTC unlock, register write enable
 * @param  None
 * @retval None
 */
__STATIC_INLINE void INTC_Unlock(void)
{
    WRITE_REG32(M0P_INTC->FPRCR, INTC_REG_UNPROTECT);
}

en_result_t INTC_IrqRegistration(const stc_irq_regi_config_t *pstcIrqRegiConfig);
en_result_t INTC_IrqResign(IRQn_Type enIRQn);
en_result_t INTC_ShareIrqCmd(en_int_src_t enIntSrc, en_functional_state_t enNewState);
void INTC_WakeupSrcCmd(uint32_t u32WakeupSrc, en_functional_state_t enNewState);
void INTC_EventCmd(uint8_t u8Event, en_functional_state_t enNewState);

en_result_t NMI_Init(const stc_nmi_config_t *pstcNmiConfig);
en_result_t NMI_StructInit(stc_nmi_config_t *pstcNmiConfig);
en_flag_status_t NMI_GetNmiSrc(uint8_t u8NmiSrc);
void NMI_ClrNmiSrc(uint8_t u8NmiSrc);

en_result_t EXINT_Init(const stc_exint_config_t *pstcExIntConfig);
en_result_t EXINT_StructInit(stc_exint_config_t *pstcExIntConfig);
en_flag_status_t EXINT_GetExIntSrc(uint16_t u16ExIntCh);
void EXINT_ClrExIntSrc(uint16_t u16ExIntCh);

void INTC_EKeyCmd(uint8_t u8EKey, en_functional_state_t enNewState);

__WEAKDEF void NMI_IrqHandler(void);
__WEAKDEF void HardFault_IrqHandler(void);
__WEAKDEF void SVC_IrqHandler(void);
__WEAKDEF void PendSV_IrqHandler(void);
__WEAKDEF void SysTick_IrqHandler(void);

__WEAKDEF void Extint08_IrqHandler(void);
__WEAKDEF void Extint09_IrqHandler(void);

__WEAKDEF void DmaTc0_IrqHandler(void);
__WEAKDEF void DmaBtc0_IrqHandler(void);
__WEAKDEF void DmaTc1_IrqHandler(void);
__WEAKDEF void DmaBtc1_IrqHandler(void);
__WEAKDEF void DmaErr_IrqHandler(void);

__WEAKDEF void EfmPgmEraseErr_IrqHandler(void);
__WEAKDEF void EfmColErr_IrqHandler(void);
__WEAKDEF void EfmOpEnd_IrqHandler(void);

__WEAKDEF void XtalStop_IrqHandler(void);

__WEAKDEF void Swdt_IrqHandler(void);

__WEAKDEF void Timer0GCmp_IrqHandler(void);

__WEAKDEF void Timer2GCmp_IrqHandler(void);
__WEAKDEF void Timer2GOV_IrqHandler(void);

__WEAKDEF void Timer4GCMUH_IrqHandler(void);
__WEAKDEF void Timer4GCMUL_IrqHandler(void);
__WEAKDEF void Timer4GCMVH_IrqHandler(void);
__WEAKDEF void Timer4GCMVL_IrqHandler(void);
__WEAKDEF void Timer4GCMWH_IrqHandler(void);
__WEAKDEF void Timer4GCMWL_IrqHandler(void);
__WEAKDEF void Timer4GOV_IrqHandler(void);
__WEAKDEF void Timer4GUD_IrqHandler(void);
__WEAKDEF void Timer4ReloadU_IrqHandler(void);
__WEAKDEF void Timer4ReloadV_IrqHandler(void);
__WEAKDEF void Timer4ReloadW_IrqHandler(void);

__WEAKDEF void Emb_IrqHandler(void);

__WEAKDEF void TimerAOV_IrqHandler(void);
__WEAKDEF void TimerAUD_IrqHandler(void);
__WEAKDEF void TimerACmp_IrqHandler(void);

__WEAKDEF void TimerB1OF_IrqHandler(void);
__WEAKDEF void TimerB1UF_IrqHandler(void);
__WEAKDEF void TimerB1Cmp_IrqHandler(void);
__WEAKDEF void TimerB2OF_IrqHandler(void);
__WEAKDEF void TimerB2UF_IrqHandler(void);
__WEAKDEF void TimerB2Cmp_IrqHandler(void);
__WEAKDEF void TimerB3OF_IrqHandler(void);
__WEAKDEF void TimerB3UF_IrqHandler(void);
__WEAKDEF void TimerB3Cmp_IrqHandler(void);
__WEAKDEF void TimerB4OF_IrqHandler(void);
__WEAKDEF void TimerB4UF_IrqHandler(void);
__WEAKDEF void TimerB4Cmp_IrqHandler(void);

__WEAKDEF void Usart1RxErr_IrqHandler(void);
__WEAKDEF void Usart1RxEnd_IrqHandler(void);
__WEAKDEF void Usart1TxEmpt_IrqHandler(void);
__WEAKDEF void Usart1TxEnd_IrqHandler(void);
__WEAKDEF void Usart2RxErr_IrqHandler(void);
__WEAKDEF void Usart2RxEnd_IrqHandler(void);
__WEAKDEF void Usart2TxEmpt_IrqHandler(void);
__WEAKDEF void Usart2TxEnd_IrqHandler(void);
__WEAKDEF void Usart3RxErr_IrqHandler(void);
__WEAKDEF void Usart3RxEnd_IrqHandler(void);
__WEAKDEF void Usart3TxEmpt_IrqHandler(void);
__WEAKDEF void Usart3TxEnd_IrqHandler(void);

__WEAKDEF void I2cRxEnd_IrqHandler(void);
__WEAKDEF void I2cTxEnd_IrqHandler(void);
__WEAKDEF void I2cTxEmpt_IrqHandler(void);
__WEAKDEF void I2cErr_IrqHandler(void);

__WEAKDEF void SpiRxEnd_IrqHandler(void);
__WEAKDEF void SpiTxEmpt_IrqHandler(void);
__WEAKDEF void SpiErr_IrqHandler(void);
__WEAKDEF void SpiIdle_IrqHandler(void);

__WEAKDEF void ClockTrimErr_IrqHandler(void);

__WEAKDEF void EKey_IrqHandler(void);

__WEAKDEF void AdcSeqA_IrqHandler(void);
__WEAKDEF void AdcSeqB_IrqHandler(void);
__WEAKDEF void AdcCmp0_IrqHandler(void);
__WEAKDEF void AdcCmp1_IrqHandler(void);

__WEAKDEF void Lvd_IrqHandler(void);


/**
 * @}
 */

#endif /* DDL_INTERRUPTS_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_INTERRUPTS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
