/**
 *******************************************************************************
 * @file  hc32m120_adc.h
 * @brief This file contains all the functions prototypes of the ADC driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-09       Wuze            First version
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
#ifndef __HC32M120_ADC_H__
#define __HC32M120_ADC_H__

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
 * @addtogroup DDL_ADC
 * @{
 */

#if (DDL_ADC_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup ADC_Global_Types ADC Global Types
 * @{
 */

/**
 * @brief Structure definition of ADC initialization.
 */
typedef struct
{
    uint16_t u16ScanConvMode;   /*!< Configures the scan convert mode of ADC.
                                     This parameter can be a value of @ref ADC_Scan_Conv_Mode */
    uint16_t u16Resolution;     /*!< Configures the ADC resolution.
                                     This parameter can be a value of @ref ADC_Resolution */
    uint16_t u16AutoClearCmd;   /*!< Configures whether the ADC data register is
                                     automatically cleared after the data is read.
                                     This parameter can be a value of @ref ADC_Data_Auto_Clear_Cmd */
    uint16_t u16DataAlign;      /*!< Specifies ADC data alignment to right or to left.
                                     This parameter can be a value of @ref ADC_Data_Align */
    uint8_t  u8SampTime;        /*!< The channel's sampling time. */
    uint16_t u16RestrtFrom;     /*!< Specifies the restart channel of sequence A.
                                     Sequence A can be interrupted by sequence B. After the
                                     the ending of sequence B, sequence A restarts from the
                                     specified channel.
                                     This parameter can be a value of @ref ADC_Restart_From */
} stc_adc_init_t;

/**
 * @brief Structure definition of ADC trigger source configuration.
 */
typedef struct
{
    uint16_t u16TrgSrc;         /*!< Specifies the trigger source type for the specified sequence.
                                     This parameter can be a value of @ref ADC_Trigger_Source_Type */
    uint32_t u32Event0;         /*!< An @ref en_event_src_t enumeration value. */
    uint32_t u32Event1;         /*!< An @ref en_event_src_t enumeration value. */
} stc_adc_trg_cfg_t;

/**
 * @brief Structure definition of AWD(analog watchdog) configuration.
 */
typedef struct
{
    uint8_t u8AdcPin;           /*!< The ADC pin be configured as an AWD input pin.
                                     This parameter can be a value of @ref ADC_Pin_Number */
    uint16_t u16AwdMd;          /*!< The comparison mode of AWD.
                                     This parameter can be a value of @ref ADC_AWD_Comparison_Mode */
    uint16_t u16Dr0;            /*!< Threshold value to register ADC_AWD0DR0/ADC_AWD1DR0. */
    uint16_t u16Dr1;            /*!< Threshold value to register ADC_AWD0DR1/ADC_AWD1DR1. */
} stc_awd_cfg_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup ADC_Global_Macros ADC Global Macros
 * @{
 */

/**
 * @defgroup ADC_Sequence ADC Sequence
 * @{
 */
#define ADC_SEQ_A                   ((uint8_t)0u)
#define ADC_SEQ_B                   ((uint8_t)1u)
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_Number ADC AWD Number
 * @{
 */
#define ADC_AWD_0                   ((uint8_t)0u)
#define ADC_AWD_1                   ((uint8_t)1u)
/**
 * @}
 */

/**
 * @defgroup ADC_OPA_Number ADC OPA Number
 * @{
 */
#define ADC_OPA_1                   ((uint8_t)0)
#define ADC_OPA_2                   ((uint8_t)1)
/**
 * @}
 */

/**
 * @defgroup ADC_Data_Align ADC Data Align
 * @{
 */
#define ADC_DATA_ALIGN_RIGHT        ((uint16_t)0x0u)
#define ADC_DATA_ALIGN_LEFT         (ADC_CR0_DFMT)
/**
 * @}
 */

/**
 * @defgroup ADC_Data_Auto_Clear_Cmd ADC Data Auto Clear Command
 * @{
 */
#define ADC_AUTO_CLR_DISABLE        ((uint16_t)0x0u)
#define ADC_AUTO_CLR_ENABLE         (ADC_CR0_CLREN)     /*!< ADC data register is automatically
                                                             cleared after the data is read. */
/**
 * @}
 */

/**
 * @defgroup ADC_Resolution ADC Resolution
 * @{
 */
#define ADC_RESOLUTION_MASK         (ADC_CR0_ACCSEL)
#define ADC_RESOLUTION_12B          ((uint16_t)0x0u)
#define ADC_RESOLUTION_10B          (ADC_CR0_ACCSEL_0)
#define ADC_RESOLUTION_8B           (ADC_CR0_ACCSEL_1)
/**
 * @}
 */

/**
 * @defgroup ADC_Scan_Conv_Mode ADC Scan Convert Mode
 * @{
 */
#define ADC_MODE_MASK                       (ADC_CR0_MS)
#define ADC_MODE_SA_SINGLE                  ((uint16_t)0x0u)
#define ADC_MODE_SA_CONTINUOUS              (ADC_CR0_MS_0)
#define ADC_MODE_SA_SB_SINGLE               (ADC_CR0_MS_1)
#define ADC_MODE_SA_CONTINUOUS_SB_SINGLE    (ADC_CR0_MS_0 | ADC_CR0_MS_1)
/**
 * @}
 */

/**
 * @defgroup ADC_Restart_From ADC Restart From
 * @{
 */
#define ADC_SA_RESTRT_FROM_INT_CH           ((uint16_t)0x0u)
#define ADC_SA_RESTRT_FROM_FIRST_CH         (ADC_CR1_RSCHSEL)
/**
 * @}
 */

/**
 * @defgroup ADC_Trigger_Source_Type ADC Trigger Source Type
 * @{
 */
#define ADC_TRGSRC_MASK             ((uint16_t)0x3u)
#define ADC_TRGSRC_EX_PIN           ((uint16_t)0x0u)
#define ADC_TRGSRC_IN_EVT0          ((uint16_t)0x1u)
#define ADC_TRGSRC_IN_EVT1          ((uint16_t)0x2u)
#define ADC_TRGSRC_IN_EVT0_EVT1     ((uint16_t)0x3u)
/**
 * @}
 */

/**
 * @defgroup ADC_Extend_Channel_Source_Selection ADC Extend Channel Source Selection
 * @{
 */
#define ADC_EX_CH_SEL_ANI11         ((uint8_t)0x0)
#define ADC_EX_CH_SEL_INTERNAL      (ADC_EXCHSELR_EXCHSEL)
/**
 * @}
 */

/**
 * @defgroup ADC_Eoc_Flag ADC Eoc(End Of Conversion) flag
 * @{
 */
#define ADC_FLAG_EOCA               (ADC_ISR_EOCAF)
#define ADC_FLAG_EOCB               (ADC_ISR_EOCBF)
#define ADC_FLAG_EOC_ALL            (ADC_FLAG_EOCA | ADC_FLAG_EOCB)
/**
 * @}
 */

/**
 * @defgroup ADC_Pin_Number ADC Pin Number
 * @{
 */
#define ADC_ANI0                    ((uint8_t)0u)       /*!< Pin P20 */
#define ADC_ANI1                    ((uint8_t)1u)       /*!< Pin P21 */
#define ADC_ANI2                    ((uint8_t)2u)       /*!< Pin P22 */
#define ADC_ANI3                    ((uint8_t)3u)       /*!< Pin P23 */
#define ADC_ANI4                    ((uint8_t)4u)       /*!< Pin P24 */
#define ADC_ANI5                    ((uint8_t)5u)       /*!< Pin P25 */
#define ADC_ANI6                    ((uint8_t)6u)       /*!< Pin P26 */
#define ADC_ANI7                    ((uint8_t)7u)       /*!< Pin P27 */
#define ADC_ANI8                    ((uint8_t)8u)       /*!< Pin P147 */
#define ADC_ANI9                    ((uint8_t)9u)       /*!< Pin P00 */
#define ADC_ANI10                   ((uint8_t)10u)      /*!< Pin P01 */
#define ADC_ANI11                   ((uint8_t)11u)      /*!< Pin P120 */
/**
 * @}
 */

/**
 * @defgroup ADC_Channel_Count ADC Channel Count
 * @{
 */
#define ADC_CH_COUNT                (12u)
/**
 * @}
 */

/**
 * @defgroup ADC_Channels ADC Common Channels
 * @{
 */
#define ADC_CH0                     ((uint16_t)(0x1ul << ADC_ANI0))
#define ADC_CH1                     ((uint16_t)(0x1ul << ADC_ANI1))
#define ADC_CH2                     ((uint16_t)(0x1ul << ADC_ANI2))
#define ADC_CH3                     ((uint16_t)(0x1ul << ADC_ANI3))
#define ADC_CH4                     ((uint16_t)(0x1ul << ADC_ANI4))
#define ADC_CH5                     ((uint16_t)(0x1ul << ADC_ANI5))
#define ADC_CH6                     ((uint16_t)(0x1ul << ADC_ANI6))
#define ADC_CH7                     ((uint16_t)(0x1ul << ADC_ANI7))
#define ADC_CH8                     ((uint16_t)(0x1ul << ADC_ANI8))
#define ADC_CH9                     ((uint16_t)(0x1ul << ADC_ANI9))
#define ADC_CH10                    ((uint16_t)(0x1ul << ADC_ANI10))
#define ADC_CH11                    ((uint16_t)(0x1ul << ADC_ANI11))
#define ADC_CH_ALL                  ((uint16_t)0x0FFFu)
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_Comparison_Mode ADC AWD(Analog Watchdog) Comparison Mode
 * @{
 */
#define ADC_AWD_CMP_OUT_RANGE       ((uint16_t)0x0u)
#define ADC_AWD_CMP_IN_RANGE        ((uint16_t)0x1u)
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_Combination_Mode ADC AWD(Analog Watchdog) Combination Mode
 * @note If combination mode is valid(ADC_AWD_COMB_OR/ADC_AWD_COMB_AND/ADC_AWD_COMB_XOR) and
         the channles selected by the AWD0 and AWD1 are deferent, make sure that the channel
         of AWD1 is converted after the channel conversion of AWD0 ends.
 * @{
 */
#define ADC_AWD_COMB_MASK           (ADC_AWDCR_AWDCM)
#define ADC_AWD_COMB_INVALID        ((uint16_t)0x0)
#define ADC_AWD_COMB_OR             (ADC_AWDCR_AWDCM_0)
#define ADC_AWD_COMB_AND            (ADC_AWDCR_AWDCM_1)
#define ADC_AWD_COMB_XOR            (ADC_AWDCR_AWDCM_0 | ADC_AWDCR_AWDCM_1)
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_State_Flag ADC AWD(Analog Watchdog) State Flag
 * @{
 */
#define ADC_FLAG_AWD0               (ADC_AWDSR_AWD0F)
#define ADC_FLAG_AWD1               (ADC_AWDSR_AWD1F)
#define ADC_FLAG_AWD_COMB           (ADC_AWDSR_AWDCMF)
#define ADC_FLAG_AWD_ALL            (ADC_FLAG_AWD0 | ADC_AWDSR_AWD1F | ADC_AWDSR_AWDCMF)
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
 * @addtogroup ADC_Global_Functions
 * @{
 */

/**
 * @brief  Start ADC.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void ADC_Start(void)
{
    bM0P_ADC->STR_b.STRT = 0x1u;
}

/**
 * @brief  Stop ADC.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void ADC_Stop(void)
{
    bM0P_ADC->STR_b.STRT = 0x0u;
}

/**
 * @brief  Get ADC run status.
 * @param  None
 * @retval An en_flag_status_t enumeration.
 *   @arg  Set: The ADC is running.
 *   @arg  Reset: The ADC is stop.
 */
__STATIC_INLINE en_flag_status_t ADC_GetRunStatus(void)
{
    return (en_flag_status_t)(bM0P_ADC->STR_b.STRT);
}


en_result_t ADC_Init(const stc_adc_init_t *pstcInit);
void ADC_DeInit(void);
en_result_t ADC_StructInit(stc_adc_init_t *pstcInit);
void ADC_AddAdcChannel(uint8_t u8Seq, uint16_t u16Channel);
void ADC_SetExChannel(uint8_t u8ExCh);
en_result_t ADC_ConfigTriggerSrc(uint8_t u8Seq, const stc_adc_trg_cfg_t *pstcTrgCfg);
en_result_t ADC_TriggerSrcStructInit(stc_adc_trg_cfg_t *pstcTrgCfg);
void ADC_TriggerSrcCmd(uint8_t u8Seq, en_functional_state_t enNewState);
void ADC_AdcIntCmd(uint8_t u8Seq, en_functional_state_t enNewState);

en_flag_status_t ADC_GetEocFlag(uint8_t u8Flag);
void ADC_ClrEocFlag(uint8_t u8Flag);

en_result_t ADC_PollingSa(uint16_t *pu16Dest, uint8_t u8Length, uint32_t u32Timeout);
en_result_t ADC_GetAllData(uint16_t *pu16Dest, uint8_t u8Length);
en_result_t ADC_GetChannelData(uint16_t u16Channel, uint16_t *pu16Dest, uint8_t u8Length);
uint16_t ADC_GetValue(uint8_t u8AdcPin);

en_result_t ADC_AwdConfig(uint8_t u8AwdNbr, const stc_awd_cfg_t *pstcAwdCfg);
void ADC_AwdCmd(uint8_t u8AwdNbr, en_functional_state_t enNewState);
void ADC_AwdIntCmd(uint8_t u8AwdNbr, en_functional_state_t enNewState);
void ADC_AwdConfigCombMode(uint16_t u16ComMd);

en_flag_status_t ADC_AwdGetFlag(uint8_t u8Flag);
void ADC_AwdClrFlag(uint8_t u8Flag);

void ADC_OpaCmd(uint8_t u8OpaNbr, en_functional_state_t enNewState);

/**
 * @}
 */

#endif /* DDL_ADC_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_ADC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
