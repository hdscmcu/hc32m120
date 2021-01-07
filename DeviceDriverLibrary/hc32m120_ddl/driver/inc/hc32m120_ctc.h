/**
 *******************************************************************************
 * @file  hc32m120_ctc.h
 * @brief This file contains all the functions prototypes of the CTC driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-03       Hongjh          First version
   2020-10-30       Hongjh          Refine CTC initialization structure.
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
#ifndef __HC32M120_CTC_H__
#define __HC32M120_CTC_H__

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
 * @addtogroup DDL_CTC
 * @{
 */

#if (DDL_CTC_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup CTC_Global_Types CTC Global Types
 * @{
 */

/**
 * @brief CTC initialization structure definition
 */
typedef struct stc_ctc_init
{
    uint32_t u32Fref;               /*!< Reference clock frequency */

    uint32_t u32RefClkSel;          /*!< Reference clock source selection
                                         This parameter can be a value of @ref CTC_Reference_Clock_Selection */

    uint32_t u32RefclkPrescaler;    /*!< Reference clock prescaler
                                         This parameter can be a value of @ref CTC_Reference_Clock_Prescaler_Division */

    float32_t f32ToleranceBias ;    /*!< Tolerance bias.
                                         This parameter should be less than CTC_DEFAULT_TOLERANCE_BIAS(2%) */

    uint32_t u32TrmVal;             /*!< CTC TRMVAL value */
} stc_ctc_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CTC_Global_Macros CTC Global Macros
 * @{
 */

/**
 * @defgroup CTC_Flag CTC Flag
 *        Elements values convention: 0xXXXXXXXX
 *           - 0xXXXXXXXX  : Flag mask in the CTC_STR register
 * @{
 */
#define CTC_FLAG_TRMOK                          (CTC_STR_TRIMOK)    /*!< Trimming OK flag */
#define CTC_FLAG_TRMOVF                         (CTC_STR_TRMOVF)    /*!< Trimming overflow flag */
#define CTC_FLAG_TRMUDF                         (CTC_STR_TRMUDF)    /*!< Trimming underflow flag */
#define CTC_FLAG_BUSY                           (CTC_STR_CTCBSY)    /*!< CTC busy flag */
/**
 * @}
 */

/**
 * @defgroup CTC_Reference_Clock_Prescaler_Division CTC Reference Clock Prescaler Division
 * @{
 */
#define CTC_REFCLK_PRESCALER_DIV8               ((uint32_t)0x00000000ul)        /*!< REFCLK/8 */
#define CTC_REFCLK_PRESCALER_DIV32              (CTC_CR1_REFPSC_0)              /*!< REFCLK/32 */
#define CTC_REFCLK_PRESCALER_DIV128             (CTC_CR1_REFPSC_1)              /*!< REFCLK/128 */
#define CTC_REFCLK_PRESCALER_DIV256             (CTC_CR1_REFPSC_1 |            \
                                                 CTC_CR1_REFPSC_0)              /*!< REFCLK/256 */
#define CTC_REFCLK_PRESCALER_DIV512             (CTC_CR1_REFPSC_2)                      /*!< REFCLK/512 */
#define CTC_REFCLK_PRESCALER_DIV1024            (CTC_CR1_REFPSC_2 |            \
                                                 CTC_CR1_REFPSC_0)              /*!< REFCLK/1024 */
#define CTC_REFCLK_PRESCALER_DIV2048            (CTC_CR1_REFPSC_2 |            \
                                                 CTC_CR1_REFPSC_1)              /*!< REFCLK/2048 */
#define CTC_REFCLK_PRESCALER_DIV4096            (CTC_CR1_REFPSC)                /*!< REFCLK/4096 */
/**
 * @}
 */

/**
 * @defgroup CTC_Reference_Clock_Selection CTC Reference Clock Selection
 * @{
 */
#define CTC_REFCLK_CTCREF                       (CTC_CR1_REFCKS_0)  /*!< Clock source: CTCREF */
#define CTC_REFCLK_XTAL                         (CTC_CR1_REFCKS)    /*!< Clock source: XTAL */
/**
 * @}
 */

/**
 * @defgroup CTC_Default_Tolerance_Bias CTC Default Tolerance Bias definition
 * @{
 */
#define CTC_DEFAULT_TOLERANCE_BIAS              (0.02f)             /*!< CTC default tolerance bias: 2% */
/**
 * @}
 */

/**
 * @defgroup CTC_Trimming_HRC_Frequency CTC Trimming HRC Frequency definition
 * @{
 */
#define CTC_TRIMMING_HRC_1MHZ                   ((uint32_t)1000000ul)   /*!< CTC Trimming 1MHz */
#define CTC_TRIMMING_HRC_2MHZ                   ((uint32_t)2000000ul)   /*!< CTC Trimming 2MHz */
#define CTC_TRIMMING_HRC_4MHZ                   ((uint32_t)4000000ul)   /*!< CTC Trimming 4MHz */
#define CTC_TRIMMING_HRC_8MHZ                   ((uint32_t)8000000ul)   /*!< CTC Trimming 8MHz */
#define CTC_TRIMMING_HRC_16MHZ                  ((uint32_t)16000000ul)  /*!< CTC Trimming 16MHz */
#define CTC_TRIMMING_HRC_32MHZ                  ((uint32_t)32000000ul)  /*!< CTC Trimming 32MHz */

#define CTC_TRIMMING_HRC_1P5MHZ                 ((uint32_t)1500000ul)   /*!< CTC Trimming 1.5MHz */
#define CTC_TRIMMING_HRC_3MHZ                   ((uint32_t)3000000ul)   /*!< CTC Trimming 3MHz */
#define CTC_TRIMMING_HRC_6MHZ                   ((uint32_t)6000000ul)   /*!< CTC Trimming 6MHz */
#define CTC_TRIMMING_HRC_12MHZ                  ((uint32_t)12000000ul)  /*!< CTC Trimming 12MHz */
#define CTC_TRIMMING_HRC_24MHZ                  ((uint32_t)24000000ul)  /*!< CTC Trimming 24MHz */
#define CTC_TRIMMING_HRC_48MHZ                  ((uint32_t)48000000ul)  /*!< CTC Trimming 48MHz */
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
 * @addtogroup CTC_Global_Functions
 * @{
 */

/**
 * @brief  Enable/disable CTC function.
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
__STATIC_INLINE void CTC_Cmd(en_functional_state_t enNewSta)
{
    bM0P_CTC->CR1_b.CTCEN = enNewSta;
}

/**
 * @brief  Enable/disable CTC error interrupt function.
 * @param  [in] enNewSta                The function new state.
 *           @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
__STATIC_INLINE void CTC_ErrIntCmd(en_functional_state_t enNewSta)
{
    bM0P_CTC->CR1_b.ERRIE = enNewSta;
}

en_result_t CTC_Init(const stc_ctc_init_t *pstcInit);
en_result_t CTC_StructInit(stc_ctc_init_t *pstcInit);
en_result_t CTC_DeInit(void);
void CTC_SetRefClkPrescaler(uint32_t u32Prescaler);
uint32_t CTC_GetRefClkPrescaler(void);
void CTC_SetRefClkSource(uint32_t u32RefClk);
uint32_t CTC_GetRefClkSource(void);
en_flag_status_t CTC_GetFlag(uint32_t u32Flag);
void CTC_SetTrmVal(uint8_t u8TrmVal);
uint8_t CTC_GetTrmVal(void);
void CTC_SetReloadVal(uint16_t u16ReloadVal);
uint16_t CTC_GetReloadVal(void);
void CTC_SetOffsetVal(uint8_t u8OffsetVal);
uint8_t CTC_GetOffsetVal(void);

/**
 * @}
 */

#endif /* DDL_CTC_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_CTC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
