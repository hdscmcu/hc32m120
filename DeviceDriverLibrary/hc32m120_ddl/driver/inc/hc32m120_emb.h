/**
 *******************************************************************************
 * @file  hc32m120_emb.h
 * @brief Head file for EMB module.
 *
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-27       Hongjh          First version
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
#ifndef __HC32M120_EMB_H__
#define __HC32M120_EMB_H__

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
 * @addtogroup DDL_EMB
 * @{
 */

#if (DDL_EMB_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup EMB_Global_Types EMB Global Types
 * @{
 */

/**
 * @brief EMB detect Timer4 PWM level configuration
 */
typedef struct
{
    en_functional_state_t enPwmWHighLvl;    /*!< Enable or disable EMB detect Timer4 PWM W channel polarity level
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enPwmVHighLvl;    /*!< Enable or disable EMB detect Timer4 PWM V channel polarity level
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enPwmUHighLvl;    /*!< Enable or disable EMB detect Timer4 PWM U channel polarity level
                                                 This parameter can be a value of @ref en_functional_state_t */
} stc_emb_timer4_pwm_level_t;

/**
 * @brief EMB control Timer4 initialization configuration
 */
typedef struct
{
    en_functional_state_t enPortCmd;        /*!< Enable or disable EMB detect port in control function
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enCmp1Cmd;        /*!< Enable or disable EMB detect CMP1 function
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enCmp2Cmd;        /*!< Enable or disable EMB detect CMP2 function
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enOscCmd;         /*!< Enable or disable EMB detect OSC failure function
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enTimer4PwmUCmd;  /*!< Enable or disable EMB detect Timer4 PWM U channel same phase function
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enTimer4PwmVCmd;  /*!< Enable or disable EMB detect Timer4 PWM V channel same phase function
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enTimer4PwmWCmd;  /*!< Enable or disable EMB detect Timer4 PWM W channel same phase function
                                                 This parameter can be a value of @ref en_functional_state_t */

    uint32_t u32PortLevel;                  /*!< EMB detect port level
                                                 This parameter can be a value of @ref EMB_Port_Level */

    uint32_t u32PortFilterDiv;              /*!< EMB port filter division
                                                 This parameter can be a value of @ref EMB_Port_Filter_Clock_Division */
} stc_emb_ctrl_timer4_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EMB_Global_Macros EMB Global Macros
 * @{
 */

/** @defgroup EMB_Port_Filter_Clock_Division EMB Port Filter Clock Division
 * @{
 */
#define EMB_PORT_FILTER_NONE                    (0x00ul)                            /*!< Disable filter */
#define EMB_PORT_FILTER_CLK_DIV1                (EMB_CTL_NFEN)                      /*!< PCLK           */
#define EMB_PORT_FILTER_CLK_DIV8                (EMB_CTL_NFEN | EMB_CTL_NFSEL_0)    /*!< PCLK/8         */
#define EMB_PORT_FILTER_CLK_DIV32               (EMB_CTL_NFEN | EMB_CTL_NFSEL_1)    /*!< PCLK/32        */
#define EMB_PORT_FILTER_CLK_DIV128              (EMB_CTL_NFEN | EMB_CTL_NFSEL)      /*!< PCLK/128       */
/**
 * @}
 */

/**
 * @defgroup EMB_Port_Level EMB Port Level
 * @{
 */
#define EMB_PORT_LEVEL_HIGH                     (0x00ul)
#define EMB_PORT_LEVEL_LOW                      (EMB_CTL_INVSEL)
/**
 * @}
 */

/**
 * @defgroup EMB_Flag EMB Flag
 * @{
 */
#define EMB_FLAG_PORT                           (EMB_STAT_PORTINF)
#define EMB_FLAG_PWM                            (EMB_STAT_PWMSF)
#define EMB_FLAG_CMP                            (EMB_STAT_CMPF)
#define EMB_FLAG_OSC                            (EMB_STAT_OSF)
#define EMB_FLAG_PORT_STATE                     (EMB_STAT_PORTINST)
#define EMB_FLAG_PWM_STATE                      (EMB_STAT_PWMST)
/**
 * @}
 */

/**
 * @defgroup EMB_Interrupt EMB Interrupt
 * @{
 */
#define EMB_INT_PORT                            (EMB_INTEN_PORTINTEN)
#define EMB_INT_PWM                             (EMB_INTEN_PWMINTEN)
#define EMB_INT_CMP                             (EMB_INTEN_CMPINTEN)
#define EMB_INT_OSC                             (EMB_INTEN_OSINTEN)
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
 * @addtogroup EMB_Global_Functions EMB Global Functions
 * @{
 */

/**
 * @brief  Get EMB status
 * @param  [in] u32Status               EMB flag
 *         This parameter can be one of the following values:
 *           @arg EMB_FLAG_PORT: Port input trigger stop PWM
 *           @arg EMB_FLAG_PWM:  PWM same phase trigger stop PWM
 *           @arg EMB_FLAG_CMP: CMP trigger stop PWM
 *           @arg EMB_FLAG_OSC: OSC trigger stop PWM
 *           @arg EMB_FLAG_PORT_STATE: Port input control state
 *           @arg EMB_FLAG_PWM_STATE: PWM same phase occur
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Flag is set
 *           - Reset: Flag is reset
 */
__STATIC_INLINE en_flag_status_t EMB_GetStatus(uint32_t u32Status)
{
    return READ_REG32_BIT(M0P_EMB->STAT, u32Status) ? Set : Reset;
}

/**
 * @brief  Clear EMB status
 * @param  [in] u32Status           EMB status
 *         This parameter can be one of the following values:
 *           @arg EMB_FLAG_PORT: Port input trigger stop PWM
 *           @arg EMB_FLAG_PWM:  PWM same phase trigger stop PWM
 *           @arg EMB_FLAG_CMP: CMP trigger stop PWM
 *           @arg EMB_FLAG_OSC: OSC trigger stop PWM
 * @retval None
 */
__STATIC_INLINE void EMB_ClearStatus(uint32_t u32Status)
{
    SET_REG32_BIT(M0P_EMB->STATCLR, u32Status);
}

/**
 * @brief  Get EMB detection port level
 * @param  None
 * @retval Returned value can be one of the following values:
 *           @arg EMB_PORT_LEVEL_HIGH: Detect high level
 *           @arg EMB_PORT_LEVEL_LOW: Detect low level
 */
__STATIC_INLINE uint32_t EMB_GetDetectPortLevel(void)
{
    return bM0P_EMB->CTL_b.INVSEL;
}

/**
 * @brief  EMB software brake
 * @param  [in] enNewSta                The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval None
 */
__STATIC_INLINE void EMB_SwBrake(en_functional_state_t enNewSta)
{
    bM0P_EMB->SOE_b.SOE = enNewSta;
}

en_result_t EMB_Init(const stc_emb_ctrl_timer4_init_t *pstcInit);
en_result_t EMB_StructInit(stc_emb_ctrl_timer4_init_t *pstcInit);
en_result_t EMB_DeInit(void);
void EMB_IntCmd(uint32_t u32IntSource, en_functional_state_t enNewSta);
uint32_t EMB_GetPortFilterClkDiv(void);
void EMB_SetDetectPwmLevel(stc_emb_timer4_pwm_level_t stcPwmLevel);
stc_emb_timer4_pwm_level_t EMB_GetDetectPwmLevel(void);
/**
 * @}
 */

#endif /* DDL_EMB_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_EMB_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
