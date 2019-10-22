/**
 *******************************************************************************
 * @file  hc32m120_timer2.h
 * @brief This file contains all the functions prototypes of the TIMER2 driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-20       Wuze            First version
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
#ifndef __HC32M120_TIMER2_H__
#define __HC32M120_TIMER2_H__

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
 * @addtogroup DDL_TIMER2
 * @{
 */
#if (DDL_TIMER2_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup TIMER2_Global_Types TIMER2 Global Types
 * @{
 */

/**
 * @brief TIMER2 timer configuration structure.
 */
typedef struct
{
    uint16_t u16ClkSource;                  /*!< Specify the clock source for TIMER2.
                                                 This parameter can be a value of @ref TIMER2_Clock_Source */

    uint16_t u16ClkPrescaler;               /*!< Specify the prescaler of the clock source.
                                                 This parameter can be a value of @ref TIMER2_Clock_Prescaler */

    uint16_t u16CompareVal;                 /*!< Specify the comparison value depends on your application. */

    uint16_t u16HwStartCondition;           /*!< Specify the start condition.
                                                 This parameter can be a value of @ref TIMER2_Hardware_Start_Condition */

    uint16_t u16HwStopCondition;            /*!< Specify the stop condition.
                                                 This parameter can be a value of @ref TIMER2_Hardware_Stop_Condition */

    uint16_t u16HwClearCondition;           /*!< Specify the clear condition.
                                                 This parameter can be a value of @ref TIMER2_Hardware_Clear_Condition */

    uint16_t u16HwCaptureCondition;         /*!< Specify the capture condition.
                                                 This parameter can be a value of @ref TIMER2_Hardware_Capture_Condition */

    uint16_t u16PwmStartPolarity;           /*!< Specify the start polarity for PWM.
                                                 This parameter can be a value of @ref TIMER2_PWM_Start_Polarity */

    uint16_t u16PwmStopPolarity;            /*!< Specify the stop polarity for PWM.
                                                 This parameter can be a value of @ref TIMER2_PWM_Stop_Polarity */

    uint16_t u16FilterPrescaler;            /*!< Specify the filter clock prescaler of TRIGA if enFilterCmd is Enable.
                                                 This parameter can be a value of @ref TIMER2_Filter_Clock_Prescaler */

    en_functional_state_t enFilterCmd;      /*!< Enable or disable the filter of TRIGA.
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enMatchIntCmd;    /*!< Enable or disable TIMER2 counter match interrupt.
                                                 This parameter can be a value of @ref en_functional_state_t */

    en_functional_state_t enOvfIntCmd;      /*!< Enable or disable TIMER2 counter overflow interrupt.
                                                 This parameter can be a value of @ref en_functional_state_t */
} stc_timer2_config_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMER2_Global_Macros TIMER2 Global Macros
 * @{
 */

/**
 * @defgroup TIMER2_Function_Mode TIMER2 Function Mode
 * @{
 */
#define TIMER2_FUNC_COMPARE             ((uint16_t)0x0)
#define TIMER2_FUNC_CAPTURE             ((uint16_t)TMR2_BCONR_CAPMDA)
/**
 * @}
 */

/**
 * @defgroup TIMER2_Clock_Prescaler TIMER2 Clock Prescaler
 * @{
 */
#define TIMER2_CLK_PRESCALER_1          ((uint16_t)0x0)                         /*!< Clock source. */
#define TIMER2_CLK_PRESCALER_2          ((uint16_t)TMR2_BCONR_CKDIVA_0)         /*!< Clock source / 2. */
#define TIMER2_CLK_PRESCALER_4          ((uint16_t)TMR2_BCONR_CKDIVA_1)         /*!< Clock source / 4. */
#define TIMER2_CLK_PRESCALER_8          ((uint16_t)(TMR2_BCONR_CKDIVA_1 | \
                                                    TMR2_BCONR_CKDIVA_0))       /*!< Clock source / 8. */
#define TIMER2_CLK_PRESCALER_16         ((uint16_t)TMR2_BCONR_CKDIVA_2)         /*!< Clock source / 16. */
#define TIMER2_CLK_PRESCALER_32         ((uint16_t)(TMR2_BCONR_CKDIVA_2 | \
                                                    TMR2_BCONR_CKDIVA_0))       /*!< Clock source / 32. */
#define TIMER2_CLK_PRESCALER_64         ((uint16_t)(TMR2_BCONR_CKDIVA_2 | \
                                                    TMR2_BCONR_CKDIVA_1))       /*!< Clock source / 64. */
#define TIMER2_CLK_PRESCALER_128        ((uint16_t)(TMR2_BCONR_CKDIVA_2 | \
                                                    TMR2_BCONR_CKDIVA_1 | \
                                                    TMR2_BCONR_CKDIVA_0))       /*!< Clock source / 128. */
#define TIMER2_CLK_PRESCALER_256        ((uint16_t)TMR2_BCONR_CKDIVA_3)         /*!< Clock source / 256. */
#define TIMER2_CLK_PRESCALER_512        ((uint16_t)(TMR2_BCONR_CKDIVA_3 | \
                                                    TMR2_BCONR_CKDIVA_0))       /*!< Clock source / 512. */
#define TIMER2_CLK_PRESCALER_1024       ((uint16_t)(TMR2_BCONR_CKDIVA_3 | \
                                                    TMR2_BCONR_CKDIVA_1))       /*!< Clock source / 1024. */
/**
 * @}
 */

/**
 * @defgroup TIMER2_Clock_Source TIMER2 Clock Source
 * @{
 */
#define TIMER2_SYNC_CS_HCLK             ((uint16_t)0x0)                         /*!< Synchronous clock source, HCLK. */
#define TIMER2_SYNC_CS_TRIGA_RISE       ((uint16_t)TMR2_BCONR_SYNCLKA_0)        /*!< Synchronous clock source, rising edge of TRIGA.
                                                                                     1 rising edge causes one count. */
#define TIMER2_SYNC_CS_TRIGA_FALL       ((uint16_t)TMR2_BCONR_SYNCLKA_1)        /*!< Synchronous clock source, falling edge of TRIGA.
                                                                                     1 falling edge causes one count. */
#define TIMER2_SYNC_CS_EVENT            ((uint16_t)TMR2_BCONR_SYNCLKA)          /*!< Synchronous clock source, peripheral event.
                                                                                     The event causes one count. */
#define TIMER2_ASYNC_CS_LRC             ((uint16_t)TMR2_BCONR_SYNSA)            /*!< Asynchronous clock source, LRC. */
#define TIMER2_ASYNC_CS_CLKA            ((uint16_t)(TMR2_BCONR_ASYNCLKA_1 | \
                                                    TMR2_BCONR_SYNSA))          /*!< Asynchronous clock source, from CLKA. */
/**
 * @}
 */

/**
 * @defgroup TIMER2_PWM_Start_Polarity TIMER2 PWM Start Polarity
 * @{
 */
#define TIMER2_PWM_START_LOW            ((uint16_t)0x0)
#define TIMER2_PWM_START_HIGH           ((uint16_t)TMR2_PCONR_STACA_0)
#define TIMER2_PWM_START_KEEP           ((uint16_t)TMR2_PCONR_STACA_1)
/**
 * @}
 */

/**
 * @defgroup TIMER2_PWM_Stop_Polarity TIMER2 PWM Stop Polarity
 * @{
 */
#define TIMER2_PWM_STOP_LOW             ((uint16_t)0x0)
#define TIMER2_PWM_STOP_HIGH            ((uint16_t)TMR2_PCONR_STPCA_0)
#define TIMER2_PWM_STOP_KEEP            ((uint16_t)TMR2_PCONR_STPCA_1)
/**
 * @}
 */

/**
 * @defgroup TIMER2_Filter_Clock_Prescaler TIMER2 Filter Clock Prescaler
 * @{
 */
#define TIMER2_FILTER_PRESCALER_1       ((uint16_t)0x0)                     /*!< Clock source. */
#define TIMER2_FILTER_PRESCALER_4       ((uint16_t)TMR2_PCONR_NOFICKA_0)    /*!< Clock source / 4. */
#define TIMER2_FILTER_PRESCALER_16      ((uint16_t)TMR2_PCONR_NOFICKA_1)    /*!< Clock source / 16. */
#define TIMER2_FILTER_PRESCALER_64      ((uint16_t)TMR2_PCONR_NOFICKA)      /*!< Clock source / 64. */
/**
 * @}
 */

/**
 * @defgroup TIMER2_Hardware_Start_Condition TIMER2 Hardware Start Condition
 * @{
 */
#define TIMER2_HW_START_INVALID         ((uint16_t)0x0)
#define TIMER2_HW_START_TRIGA_RISE      ((uint16_t)TMR2_HCONR_HSTA0)        /*!< Start TIMER2 at the rising edge of TRIGA. */
#define TIMER2_HW_START_TRIGA_FALL      ((uint16_t)TMR2_HCONR_HSTA1)        /*!< Start TIMER2 at the falling edge of TRIGA. */
#define TIMER2_HW_START_EVENT           ((uint16_t)TMR2_HCONR_HSTA2)        /*!< Start TIMER2 while the specified event occurred. */
/**
 * @}
 */

/**
 * @defgroup TIMER2_Hardware_Stop_Condition TIMER2 Hardware Stop Condition
 * @{
 */
#define TIMER2_HW_STOP_INVALID          ((uint16_t)0x0)
#define TIMER2_HW_STOP_TRIGA_RISE       ((uint16_t)TMR2_HCONR_HSTP0)        /*!< Stop TIMER2 at the rising edge of TRIGA. */
#define TIMER2_HW_STOP_TRIGA_FALL       ((uint16_t)TMR2_HCONR_HSTP1)        /*!< Stop TIMER2 at the falling edge of TRIGA. */
#define TIMER2_HW_STOP_EVENT            ((uint16_t)TMR2_HCONR_HSTP2)        /*!< Stop TIMER2 while the specified event occurred. */
/**
 * @}
 */

/**
 * @defgroup TIMER2_Hardware_Clear_Condition TIMER2 Hardware Clear Condition
 * @{
 */
#define TIMER2_HW_CLR_INVALID           ((uint16_t)0x0)
#define TIMER2_HW_CLR_TRIGA_RISE        ((uint16_t)TMR2_HCONR_HCLE0)        /*!< Clear CNTAR at the rising edge of TRIGA. */
#define TIMER2_HW_CLR_TRIGA_FALL        ((uint16_t)TMR2_HCONR_HCLE1)        /*!< Clear CNTAR at the falling edge of TRIGA. */
#define TIMER2_HW_CLR_EVENT             ((uint16_t)TMR2_HCONR_HCLE2)        /*!< Clear CNTAR while the specified event occurred. */
/**
 * @}
 */

/**
 * @defgroup TIMER2_Hardware_Capture_Condition TIMER2 Hardware Capture Condition
 * @{
 */
#define TIMER2_HW_CAP_INVALID           ((uint16_t)0x0)
#define TIMER2_HW_CAP_TRIGA_RISE        ((uint16_t)TMR2_HCONR_HICP0)        /*!< Capture at the rising edge of TRIGA. */
#define TIMER2_HW_CAP_TRIGA_FALL        ((uint16_t)TMR2_HCONR_HICP1)        /*!< Capture at the falling edge of TRIGA. */
#define TIMER2_HW_CAP_EVENT             ((uint16_t)TMR2_HCONR_HICP2)        /*!< Capture while the specified event occurred. */
/**
 * @}
 */

/**
 * @defgroup TIMER2_State_Flag TIMER2 State Flag
 * @{
 */
#define TIMER2_FLAG_CNT_MATCH           ((uint16_t)TMR2_STFLR_CMFA)         /*!< Counter match flag. */
#define TIMER2_FLAG_CNT_OVF             ((uint16_t)TMR2_STFLR_OVFA)         /*!< Counter overflow flag. */
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
 * @addtogroup TIMER2_Global_Functions
 * @{
 */

/**
 * @brief  Start TIMER2.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void TIMER2_Start(void)
{
    bM0P_TMR2->BCONR_b.CSTA = (uint32_t)0x1;
}

/**
 * @brief  Stop TIMER2.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void TIMER2_Stop(void)
{
    bM0P_TMR2->BCONR_b.CSTA = (uint32_t)0x0;
}

/**
 * @brief  TIMER2 set comparison value.
 * @param  [in]  u16Val         The comparison value for TIMER2.
 * @retval None
 */
__STATIC_INLINE void TIMER2_SetCompareVal(uint16_t u16Val)
{
    M0P_TMR2->CMPAR = (uint32_t)u16Val;
}

/**
 * @brief  TIMER2 get comparison value.
 * @param  None
 * @retval Comparison value of TIMER2.
 */
__STATIC_INLINE uint16_t TIMER2_GetCompareVal(void)
{
    return (uint16_t)M0P_TMR2->CMPAR;
}

/**
 * @brief  TIMER2 set counter value.
 * @param  [in]  u16Val         The counter value for TIMER2.
 * @retval None
 */
__STATIC_INLINE void TIMER2_SetCounterVal(uint16_t u16Val)
{
    M0P_TMR2->CNTAR = (uint32_t)u16Val;
}

/**
 * @brief  TIMER2 get counter value.
 * @param  None
 * @retval Counter value of TIMER2.
 */
__STATIC_INLINE uint16_t TIMER2_GetCounterVal(void)
{
    return (uint16_t)M0P_TMR2->CNTAR;
}

void TIMER2_DeInit(void);

en_result_t TIMER2_StructInit(stc_timer2_config_t *pstcCfg);
en_result_t TIMER2_TimerConfig(const stc_timer2_config_t *pstcCfg);
en_result_t TIMER2_PwmConfig(const stc_timer2_config_t *pstcCfg);
en_result_t TIMER2_CaptureConfig(const stc_timer2_config_t *pstcCfg);

void TIMER2_SetTrigEvent(en_event_src_t enEvent);

en_flag_status_t TIMER2_GetFlag(uint16_t u16Flag);
void TIMER2_ClrFlag(uint16_t u16Flag);

/**
 * @}
 */

#endif /* DDL_TIMER2_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_TIMER2_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
