/**
 *******************************************************************************
 * @file  hc32m120_swdt.h
 * @brief This file contains all the functions prototypes of the SWDT driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-26       Yangjp          First version
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
#ifndef __HC32M120_SWDT_H__
#define __HC32M120_SWDT_H__

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
 * @addtogroup DDL_SWDT
 * @{
 */

#if (DDL_SWDT_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup SWDT_Global_Types SWDT Global Types
 * @{
 */

/**
 * @brief SWDT Init structure definition
 */
typedef struct
{
    uint32_t u32CountCycle;     /*!< Specifies the SWDT Count Cycle.
                                     This parameter can be a value of @ref SWDT_Counter_Cycle */
    uint32_t u32ClockDivision;  /*!< Specifies the SWDT Clock Division.
                                     This parameter can be a value of @ref SWDT_Clock_Division */
    uint32_t u32RefreshRange;   /*!< Specifies the SWDT Allow Refresh Range.
                                     This parameter can be a value of @ref SWDT_Refresh_Percent_Range */
    uint32_t u32LPModeCountEn;  /*!< Specifies the SWDT Count Enable/Disable In Low Power Mode(Sleep/Stop Mode).
                                     This parameter can be a value of @ref SWDT_LPW_Mode_Count */
    uint32_t u32RequestType;    /*!< Specifies the SWDT Refresh Error or Count Underflow Request Type.
                                     This parameter can be a value of @ref SWDT_Trigger_Event_Type */
} stc_swdt_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SWDT_Global_Macros SWDT Global Macros
 * @{
 */

/**
 * @defgroup SWDT_Counter_Cycle SWDT Counter Cycle
 * @{
 */
#define SWDT_COUNTER_CYCLE_256                  ((uint32_t)0x00000000ul)    /*!< 256 clock cycle */
#define SWDT_COUNTER_CYCLE_4096                 ((uint32_t)SWDT_CR_PERI_0)  /*!< 4096 clock cycle */
#define SWDT_COUNTER_CYCLE_16384                ((uint32_t)SWDT_CR_PERI_1)  /*!< 16384 clock cycle */
#define SWDT_COUNTER_CYCLE_65536                ((uint32_t)SWDT_CR_PERI)    /*!< 65536 clock cycle */
/**
 * @}
 */

/**
 * @defgroup SWDT_Clock_Division SWDT Clock Division
 * @{
 */
#define SWDT_CLOCK_DIV1                         ((uint32_t)0x00000000ul)       /*!< SWDTCLK */
#define SWDT_CLOCK_DIV16                        ((uint32_t)SWDT_CR_CKS_2)      /*!< SWDTCLK/16 */
#define SWDT_CLOCK_DIV32                        ((uint32_t)(SWDT_CR_CKS_0 | \
                                                            SWDT_CR_CKS_2))    /*!< SWDTCLK/32 */
#define SWDT_CLOCK_DIV64                        ((uint32_t)(SWDT_CR_CKS_1 | \
                                                            SWDT_CR_CKS_2))    /*!< SWDTCLK/64 */
#define SWDT_CLOCK_DIV128                       ((uint32_t)(SWDT_CR_CKS_0 | \
                                                            SWDT_CR_CKS_1 | \
                                                            SWDT_CR_CKS_2))    /*!< SWDTCLK/128 */
#define SWDT_CLOCK_DIV256                       ((uint32_t)SWDT_CR_CKS_3)      /*!< SWDTCLK/256 */
#define SWDT_CLOCK_DIV2048                      ((uint32_t)(SWDT_CR_CKS_0 | \
                                                            SWDT_CR_CKS_1 | \
                                                            SWDT_CR_CKS_3))    /*!< SWDTCLK/2048 */
/**
 * @}
 */

/**
 * @defgroup SWDT_Refresh_Percent_Range SWDT Refresh Percent Range
 * @{
 */
#define SWDT_RANGE_100PCT                       ((uint32_t)0x00000000ul)       /*!< 100% */
#define SWDT_RANGE_0TO25PCT                     ((uint32_t)SWDT_CR_WDPT_0)     /*!< 0%~25% */
#define SWDT_RANGE_25TO50PCT                    ((uint32_t)SWDT_CR_WDPT_1)     /*!< 25%~50% */
#define SWDT_RANGE_0TO50PCT                     ((uint32_t)(SWDT_CR_WDPT_0 | \
                                                            SWDT_CR_WDPT_1))   /*!< 0%~50% */
#define SWDT_RANGE_50TO75PCT                    ((uint32_t)SWDT_CR_WDPT_2)     /*!< 50%~75% */
#define SWDT_RANGE_0TO25PCT_50TO75PCT           ((uint32_t)(SWDT_CR_WDPT_0 | \
                                                            SWDT_CR_WDPT_2))   /*!< 0%~25% & 50%~75% */
#define SWDT_RANGE_25TO75PCT                    ((uint32_t)(SWDT_CR_WDPT_1 | \
                                                            SWDT_CR_WDPT_2))   /*!< 25%~75% */
#define SWDT_RANGE_0TO75PCT                     ((uint32_t)(SWDT_CR_WDPT_0 | \
                                                            SWDT_CR_WDPT_1 | \
                                                            SWDT_CR_WDPT_2))   /*!< 0%~75% */
#define SWDT_RANGE_75TO100PCT                   ((uint32_t)SWDT_CR_WDPT_3)     /*!< 75%~100% */
#define SWDT_RANGE_0TO25PCT_75TO100PCT          ((uint32_t)(SWDT_CR_WDPT_0 | \
                                                            SWDT_CR_WDPT_3))   /*!< 0%~25% & 75%~100% */
#define SWDT_RANGE_25TO50PCT_75TO100PCT         ((uint32_t)(SWDT_CR_WDPT_1 | \
                                                            SWDT_CR_WDPT_3))   /*!< 25%~50% & 75%~100% */
#define SWDT_RANGE_0TO50PCT_75TO100PCT          ((uint32_t)(SWDT_CR_WDPT_0 | \
                                                            SWDT_CR_WDPT_1 | \
                                                            SWDT_CR_WDPT_3))   /*!< 0%~50% & 75%~100% */
#define SWDT_RANGE_50TO100PCT                   ((uint32_t)(SWDT_CR_WDPT_2 | \
                                                            SWDT_CR_WDPT_3))   /*!< 50%~100% */
#define SWDT_RANGE_0TO25PCT_50TO100PCT          ((uint32_t)(SWDT_CR_WDPT_0 | \
                                                            SWDT_CR_WDPT_2 | \
                                                            SWDT_CR_WDPT_3))   /*!< 0%~25% & 50%~100% */
#define SWDT_RANGE_25TO100PCT                   ((uint32_t)(SWDT_CR_WDPT_1 | \
                                                            SWDT_CR_WDPT_2 | \
                                                            SWDT_CR_WDPT_3))   /*!< 25%~100% */
#define SWDT_RANGE_0TO100PCT                    ((uint32_t)SWDT_CR_WDPT)       /*!< 0%~100% */
/**
 * @}
 */

/**
 * @defgroup SWDT_LPW_Mode_Count SWDT Low Power Mode Count
 * @brief SWDT count control in the sleep/stop mode
 * @{
 */
#define SWDT_LPW_MODE_COUNT_CONTINUE            ((uint32_t)0x00000000ul)    /*!< SWDT count continue in the sleep/stop mode */
#define SWDT_LPW_MODE_COUNT_STOP                ((uint32_t)SWDT_CR_SLPOFF)  /*!< SWDT count stop in the sleep/stop mode */
/**
 * @}
 */

/**
* @defgroup SWDT_Trigger_Event_Type SWDT Trigger Event Type
* @{
*/
#define SWDT_TRIG_EVENT_INT                     ((uint32_t)0x00000000ul)  /*!< SWDT trigger interrupt */
#define SWDT_TRIG_EVENT_RESET                   ((uint32_t)SWDT_CR_ITS)   /*!< SWDT trigger reset */
/**
 * @}
 */

/**
 * @defgroup SWDT_Flag SWDT Flag
 * @{
 */
#define SWDT_FLAG_COUNT_UNDERFLOW               ((uint32_t)SWDT_SR_UDF)
#define SWDT_FLAG_REFRESH_ERROR                 ((uint32_t)SWDT_SR_REF)
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
 * @addtogroup SWDT_Global_Functions
 * @{
 */

/**
 * @brief  Get SWDT count value.
 * @param  None
 * @retval Count value
 */
__STATIC_INLINE uint16_t SWDT_GetCountValue(void)
{
    return (uint16_t)(READ_REG(M0P_SWDT->SR) & SWDT_SR_CNT);
}

/* Initialization and configuration functions */
en_result_t SWDT_Init(const stc_swdt_init_t *pstcSwdtInit);
void SWDT_ReloadCounter(void);

/* Flags management functions */
en_flag_status_t SWDT_GetFlag(uint32_t u32Flag);
en_result_t SWDT_ClearFlag(uint32_t u32Flag);

/**
 * @}
 */

#endif /* DDL_SWDT_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_SWDT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
