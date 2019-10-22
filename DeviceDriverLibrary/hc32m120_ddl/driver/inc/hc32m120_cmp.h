/**
 *******************************************************************************
 * @file  hc32m120_cmp.h
 * @brief Head file for CMP module.
 *
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-25       Wangmin         First version
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
#ifndef __HC32M120_CMP_H__
#define __HC32M120_CMP_H__

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
 * @addtogroup DDL_CMP
 * @{
 */

#if (DDL_CMP_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup CMP_Global_Types CMP Global Types
 * @{
 */

/**
 * @brief CMP configuration structure
 */
typedef struct
{
    uint8_t u8CmpVol;              /*!< Compare voltage for normal mode
                                        @ref CMP1_CVSL_Source or @ref CMP2_CVSL_Source */
    uint8_t u8RefVol;              /*!< Reference voltage for normal mode ,
                                        @ref CMP1_RVSL_Source or @ref CMP2_RVSL_Source*/

    uint8_t u8WinVolLow;           /*!< CMP reference low voltage for window mode
                                        @ref CMP1_RVSL_Source */
    uint8_t u8WinVolHigh;          /*!< CMP reference high voltage for window mode
                                        @ref CMP2_RVSL_Source */

    uint8_t u8OutPolarity;         /*!< Output polarity select, @ref CMP_Out_Polarity_Select */
    uint8_t u8OutDetectEdges;      /*!< Output detecte edge, @ref CMP_Out_Detect_Edge */
    uint8_t u8OutFilter;           /*!< Output Filter, @ref CMP_Out_Filter */
}stc_cmp_init_t;


/**
 * @brief CMP timer windows function configuration structure
 */
typedef struct
{
    uint8_t u8TWMode;               /*!< Timer windows mode enable, @ref CMP_TimerWin_func */
    uint8_t u8TWSelect;             /*!< Timer windows source select, @ref CMP1_TimerWin_Select or @ref CMP2_TimerWin_Select */
    uint8_t u8TWOutLevel;           /*!< Timer windows mode output level @ref CMP_TimerWin_output_Level*/
    uint8_t u8TWInvalidLevel;       /*!< Output level when timer windows invalid, @ref CMP_TimerWin_Invalid_Level */
}stc_cmp_timerwindows_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 * @defgroup CMP_Global_Macros CMP Global Macros
 * @{
 */

/** @defgroup CMP_Unit CMP Unit definition
  * @{
  */
#define CMP_UNIT1                (M0P_CMP1)
#define CMP_UNIT2                (M0P_CMP2)
/**
 * @}
 */

/** @defgroup CMP_Mode CMP compare mode
  * @{
  */
#define CMP_MODE_NORMAL          0u
#define CMP_MODE_WINDOW          (CMP_MDR_CWDE)
/**
 * @}
 */

/** @defgroup CMP1_CVSL_Source CMP compare voltage selection for CMP1
  * @{
  */
/* Don't input compare voltage */
#define CMP1_CVSL_NONE           0u
/* Select pin VCMP1_0 as compare voltage */
#define CMP1_CVSL_VCMP1_0        (CMP_VSR_CVSL_0)
/* Select pin VCMP1_1 as compare voltage */
#define CMP1_CVSL_VCMP1_1        (CMP_VSR_CVSL_1)
/* Select pin VCMP2_0 as compare voltage */
#define CMP1_CVSL_VCMP2_0        (CMP_VSR_CVSL_2)
/**
 * @}
 */

/** @defgroup CMP2_CVSL_Source CMP compare voltage selection for CMP2
  * @{
  */
/* Don't input compare voltage */
#define CMP2_CVSL_NONE           0u
/* Select pin VCMP2_0 as compare voltage */
#define CMP2_CVSL_VCMP2_0        (CMP_VSR_CVSL_0)
/* Select pin VCMP2_1 as compare voltage */
#define CMP2_CVSL_VCMP2_1        (CMP_VSR_CVSL_1)
/* Select pin VCMP2_2 as compare voltage */
#define CMP2_CVSL_VCMP2_2        (CMP_VSR_CVSL_2)
/**
 * @}
 */

/** @defgroup CMP1_RVSL_Source CMP reference voltage selection for CMP1
  * @{
  */
/* Don't input reference voltage */
#define CMP1_RVSL_NONE           0u
/* Select pin IVREF1 as reference voltage */
#define CMP1_RVSL_IVREF1         (CMP_VSR_RVSL_0)
/* Select internal reference voltage (Vref) as reference voltage */
#define CMP1_RVSL_VREF           (CMP_VSR_RVSL_1)
/**
 * @}
 */

/** @defgroup CMP2_RVSL_Source CMP reference voltage selection for CMP2
  * @{
  */
/* Don't input reference voltage */
#define CMP2_RVSL_NONE           0u
/* Select pin IVREF2 as reference voltage */
#define CMP2_RVSL_IVREF2         (CMP_VSR_RVSL_0)
/* Select internal reference voltage (Vref) as reference voltage */
#define CMP2_RVSL_VREF           (CMP_VSR_RVSL_1)
/* Select pin IVCMP2_2 as reference voltage */
#define CMP2_RVSL_IVCMP2_2       (CMP_VSR_RVSL_2)
/**
 * @}
 */

/** @defgroup CMP_Out_Polarity_Select CMP output polarity selection
  * @{
  */
/* CMP output don't reverse */
#define CMP_OUT_REVERSE_OFF          0u
/* CMP output level reverse */
#define CMP_OUT_REVERSE_ON           (CMP_OCR_COPS)
/**
 * @}
 */

/** @defgroup CMP_Out_Detect_Edge CMP output detect edge selection
  * @{
  */
#define CMP_DETECT_EDGS_NONE        ((uint8_t)0ul)
#define CMP_DETECT_EDGS_RISING      ((uint8_t)(1ul << CMP_FIR_EDGS_POS))
#define CMP_DETECT_EDGS_FALLING     ((uint8_t)(2ul << CMP_FIR_EDGS_POS))
#define CMP_DETECT_EDGS_BOTH        ((uint8_t)(3ul << CMP_FIR_EDGS_POS))
/**
 * @}
 */

/** @defgroup CMP_Out_Filter CMP output filter configuration
  * @{
  */
#define CMP_OUT_FILTER_NONE         0u
#define CMP_OUT_FILTER_PCLK         1u
#define CMP_OUT_FILTER_PCLKDIV8     2u
#define CMP_OUT_FILTER_PCLKDIV32    3u
/**
 * @}
 */

/** @defgroup CMP_TimerWin_func CMP timer window function configuration
  * @{
  */
#define CMP_TIMERWIN_OFF            0u
#define CMP_TIMERWIN_ON             (CMP_OCR_TWOE)
/**
 * @}
 */

/** @defgroup CMP1_TimerWin_Select CMP output timer windows function control signal definition for CMP1
  * @{
  */
#define CMP1_TIMERWIN_TIMB_1_PWM     (CMP_TWR1_CTWS0)
#define CMP1_TIMERWIN_TIMB_2_PWM     (CMP_TWR1_CTWS1)
#define CMP1_TIMERWIN_TIM4_OWL       (CMP_TWR1_CTWS2)
#define CMP1_TIMERWIN_TIM4_OWH       (CMP_TWR1_CTWS3)
#define CMP1_TIMERWIN_TIM4_OVL       (CMP_TWR1_CTWS4)
#define CMP1_TIMERWIN_TIM4_OVH       (CMP_TWR1_CTWS5)
#define CMP1_TIMERWIN_TIM4_OUL       (CMP_TWR1_CTWS6)
#define CMP1_TIMERWIN_TIM4_OUH       (CMP_TWR1_CTWS7)
/**
 * @}
 */

/** @defgroup CMP2_TimerWin_Select CMP output timer windows function control signal definition for CMP2
  * @{
  */
#define CMP2_TIMERWIN_TIMB_3_PWM     (CMP_TWR1_CTWS0)
#define CMP2_TIMERWIN_TIMB_4_PWM     (CMP_TWR1_CTWS1)
#define CMP2_TIMERWIN_TIM4_OWL       (CMP_TWR1_CTWS2)
#define CMP2_TIMERWIN_TIM4_OWH       (CMP_TWR1_CTWS3)
#define CMP2_TIMERWIN_TIM4_OVL       (CMP_TWR1_CTWS4)
#define CMP2_TIMERWIN_TIM4_OVH       (CMP_TWR1_CTWS5)
#define CMP2_TIMERWIN_TIM4_OUL       (CMP_TWR1_CTWS6)
#define CMP2_TIMERWIN_TIM4_OUH       (CMP_TWR1_CTWS7)
/**
 * @}
 */

/** @defgroup CMP_TimerWin_Invalid_Level CMP output level when timer window invalid
  * @{
  */
#define CMP_TIMERWIN_INVALID_LEVEL_LOW   0u
#define CMP_TIMERWIN_INVALID_LEVEL_HIGH  (CMP_OCR_TWOL)
/**
 * @}
 */

/** @defgroup CMP_TimerWin_output_Level CMP output level in timer windows mode
  * @{
  */
#define CMP_TIMERWIN_OUT_LEVEL_LOW       0u
#define CMP_TIMERWIN_OUT_LEVEL_HIGH      1u
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
 * @addtogroup CMP_Global_Functions
 * @{
 */
en_result_t CMP_StructInit(stc_cmp_init_t* pstcCMP_InitStruct);
en_result_t CMP_DeInit(M0P_CMP_TypeDef *CMPx);
en_result_t CMP_NormalModeInit(M0P_CMP_TypeDef *CMPx,
                     const stc_cmp_init_t* pstcCMP_InitStruct);
en_result_t CMP_WindowModeInit(const stc_cmp_init_t* pstcCMP_InitStruct);

en_result_t CMP_TimerWindowsCfg(M0P_CMP_TypeDef *CMPx,
                                 const stc_cmp_timerwindows_t* pstcCMP_TimerWinStruct);

en_result_t CMP_FuncCmd(M0P_CMP_TypeDef *CMPx, en_functional_state_t enNewSta);
en_result_t CMP_IntCmd(M0P_CMP_TypeDef *CMPx, en_functional_state_t enNewSta);
en_result_t CMP_OutputCmd(M0P_CMP_TypeDef *CMPx, en_functional_state_t enNewSta);
en_result_t CMP_VCOUTCmd(M0P_CMP_TypeDef *CMPx, en_functional_state_t enNewSta);

en_result_t CMP_ResultGet(M0P_CMP_TypeDef *CMPx, en_flag_status_t* penFunSta);

/**
 * @}
 */

#endif /* DDL_CMP_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_CMP_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
