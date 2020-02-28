/**
 *******************************************************************************
 * @file  hc32m120_timer4.c
 * @brief This file provides firmware functions to manage the TIMER4
 *        (TIMER4).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-24       Hongjh          First version
   2020-02-14       Hongjh          1. Comment revise for TIMER4_CNT_GetIntMaskTimes/
                                       TIMER4_CNT_GetIntMaskCurrentTimes API
                                    2. API name revise for TIMER4_CNT_GetIntMaskCurrentTimes
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
#include "hc32m120_timer4.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_TIMER4 TIMER4
 * @brief TIMER4 Driver Library
 * @{
 */

#if (DDL_TIMER4_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMER4_Local_Macros TIMER4 Local Macros
 * @{
 */

/**
 * @defgroup TIMER4_Check_Parameters_Validity TIMER4 Check Parameters Validity
 * @{
 */

#define IS_VALID_TIMER4_CNT_CLK_DIV(x)                                         \
(   ((x) == TIMER4_CNT_CLK_DIV1)                ||                             \
    ((x) == TIMER4_CNT_CLK_DIV2)                ||                             \
    ((x) == TIMER4_CNT_CLK_DIV4)                ||                             \
    ((x) == TIMER4_CNT_CLK_DIV8)                ||                             \
    ((x) == TIMER4_CNT_CLK_DIV16)               ||                             \
    ((x) == TIMER4_CNT_CLK_DIV32)               ||                             \
    ((x) == TIMER4_CNT_CLK_DIV64)               ||                             \
    ((x) == TIMER4_CNT_CLK_DIV128)              ||                             \
    ((x) == TIMER4_CNT_CLK_DIV256)              ||                             \
    ((x) == TIMER4_CNT_CLK_DIV512)              ||                             \
    ((x) == TIMER4_CNT_CLK_DIV1024))

#define IS_VALID_TIMER4_CNT_MODE(x)                                            \
(   ((x) == TIMER4_CNT_MODE_SAWTOOTH_WAVE)      ||                             \
    ((x) == TIMER4_CNT_MODE_TRIANGLE_WAVE))

#define IS_VALID_TIMER4_CNT_CLK_SRC(x)                                         \
(   ((x) == TIMER4_CNT_PCLK)                    ||                             \
    ((x) == TIMER4_CNT_EXTCLK))

#define IS_VALID_TIMER4_CNT_INT_MSK(x)                                         \
(   ((x) == TIMER4_CNT_INT_MASK_0)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_1)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_2)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_3)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_4)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_5)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_6)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_7)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_8)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_9)              ||                             \
    ((x) == TIMER4_CNT_INT_MASK_10)             ||                             \
    ((x) == TIMER4_CNT_INT_MASK_11)             ||                             \
    ((x) == TIMER4_CNT_INT_MASK_12)             ||                             \
    ((x) == TIMER4_CNT_INT_MASK_13)             ||                             \
    ((x) == TIMER4_CNT_INT_MASK_14)             ||                             \
    ((x) == TIMER4_CNT_INT_MASK_15))

#define IS_VALID_TIMER4_CNT_INT(x)                                             \
(   ((x) == TIMER4_CNT_IT_ZERO)                 ||                             \
    ((x) == TIMER4_CNT_IT_PEAK))

#define IS_VALID_TIMER4_CNT_FLAG(x)                                            \
(   ((x) == TIMER4_CNT_FLAG_ZERO)               ||                             \
    ((x) == TIMER4_CNT_FLAG_PEAK))

#define IS_VALID_TIMER4_OCO_CH(x)                                              \
(   ((x) == TIMER4_OCO_UH)                      ||                             \
    ((x) == TIMER4_OCO_UL)                      ||                             \
    ((x) == TIMER4_OCO_VH)                      ||                             \
    ((x) == TIMER4_OCO_VL)                      ||                             \
    ((x) == TIMER4_OCO_WH)                      ||                             \
    ((x) == TIMER4_OCO_WL))

#define IS_VALID_TIMER4_OCO_HIGH_CH(x)                                         \
(   ((x) == TIMER4_OCO_UH)                      ||                             \
    ((x) == TIMER4_OCO_VH)                      ||                             \
    ((x) == TIMER4_OCO_WH))

#define IS_VALID_TIMER4_OCO_LOW_CH(x)                                          \
(   ((x) == TIMER4_OCO_UL)                      ||                             \
    ((x) == TIMER4_OCO_VL)                      ||                             \
    ((x) == TIMER4_OCO_WL))

#define IS_VALID_TIMER4_OCO_OCCR_BUF_MODE(x)                                   \
(   ((x) == TIMER4_OCO_OCCR_BUF_DISABLE)        ||                             \
    ((x) == TIMER4_OCO_OCCR_BUF_CNT_ZERO)       ||                             \
    ((x) == TIMER4_OCO_OCCR_BUF_CNT_PEAK)       ||                             \
    ((x) == TIMER4_OCO_OCCR_BUF_CNT_ZERO_OR_PEAK))

#define IS_VALID_TIMER4_OCO_OCMR_BUF_MODE(x)                                   \
(   ((x) == TIMER4_OCO_OCMR_BUF_DISABLE)        ||                             \
    ((x) == TIMER4_OCO_OCMR_BUF_CNT_ZERO)       ||                             \
    ((x) == TIMER4_OCO_OCMR_BUF_CNT_PEAK)       ||                             \
    ((x) == TIMER4_OCO_OCMR_BUF_CNT_ZERO_OR_PEAK))

#define IS_VALID_TIMER4_OCO_PORT_INVALID_OP(x)                                 \
(   ((x) == TIMER4_OCO_INVAILD_OP_LOW)          ||                             \
    ((x) == TIMER4_OCO_INVAILD_OP_HIGH))

#define IS_VALID_TIMER4_OCO_OCF_STATE(x)                                       \
(   ((x) == TIMER4_OCO_OCF_HOLD)                ||                             \
    ((x) == TIMER4_OCO_OCF_SET))

#define IS_VALID_TIMER4_OCO_OCF_STATE(x)                                       \
(   ((x) == TIMER4_OCO_OCF_HOLD)                ||                             \
    ((x) == TIMER4_OCO_OCF_SET))

#define IS_VALID_TIMER4_OCO_OUTPUT_POLARITY(x)                                 \
(   ((x) == TIMER4_OCO_OP_HOLD)                 ||                             \
    ((x) == TIMER4_OCO_OP_HIGH)                 ||                             \
    ((x) == TIMER4_OCO_OP_LOW)                  ||                             \
    ((x) == TIMER4_OCO_OP_INVERT))

#define IS_VALID_TIMER4_PWM_CH(x)                                              \
(   ((x) == TIMER4_PWM_U)                       ||                             \
    ((x) == TIMER4_PWM_V)                       ||                             \
    ((x) == TIMER4_PWM_W))

#define IS_VALID_TIMER4_PWM_MODE(x)                                            \
(   ((x) == TIMER4_PWM_THROUGH_MODE)            ||                             \
    ((x) == TIMER4_PWM_DEAD_TIMER_MODE)         ||                             \
    ((x) == TIMER4_PWM_DEAD_TIMER_FILTER_MODE))

#define IS_VALID_TIMER4_PWM_OUTPUT_POLARITY(x)                                 \
(   ((x) == TIMER4_PWM_OP_OXH_HOLD_OXL_HOLD)    ||                             \
    ((x) == TIMER4_PWM_OP_OXH_INVERT_OXL_HOLD)  ||                             \
    ((x) == TIMER4_PWM_OP_OXH_HOLD_OXL_INVERT)  ||                             \
    ((x) == TIMER4_PWM_OP_OXH_INVERT_OXL_INVERT))

#define IS_VALID_TIMER4_PWM_CLK_DIV(x)                                         \
(   ((x) == TIMER4_PWM_CLK_DIV1)                ||                             \
    ((x) == TIMER4_PWM_CLK_DIV2)                ||                             \
    ((x) == TIMER4_PWM_CLK_DIV4)                ||                             \
    ((x) == TIMER4_PWM_CLK_DIV8)                ||                             \
    ((x) == TIMER4_PWM_CLK_DIV16)               ||                             \
    ((x) == TIMER4_PWM_CLK_DIV32)               ||                             \
    ((x) == TIMER4_PWM_CLK_DIV64)               ||                             \
    ((x) == TIMER4_PWM_CLK_DIV128))

#define IS_VALID_TIMER4_SEVT_CH(x)                                             \
(   ((x) == TIMER4_SEVT_UH)                     ||                             \
    ((x) == TIMER4_SEVT_UL)                     ||                             \
    ((x) == TIMER4_SEVT_VH)                     ||                             \
    ((x) == TIMER4_SEVT_VL)                     ||                             \
    ((x) == TIMER4_SEVT_WH)                     ||                             \
    ((x) == TIMER4_SEVT_WL))

#define IS_VALID_TIMER4_SEVT_MODE(x)                                           \
(   ((x) == TIMER4_SEVT_MODE_DELAY_TRIGGER)     ||                             \
    ((x) == TIMER4_SEVT_MODE_COMPARE_TRIGGER))

#define IS_VALID_TIMER4_SEVT_MASK(x)                                           \
(   ((x) == TIMER4_SEVT_MASK_0)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_1)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_2)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_3)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_4)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_5)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_6)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_7)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_8)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_9)                 ||                             \
    ((x) == TIMER4_SEVT_MASK_10)                ||                             \
    ((x) == TIMER4_SEVT_MASK_11)                ||                             \
    ((x) == TIMER4_SEVT_MASK_12)                ||                             \
    ((x) == TIMER4_SEVT_MASK_13)                ||                             \
    ((x) == TIMER4_SEVT_MASK_14)                ||                             \
    ((x) == TIMER4_SEVT_MASK_15))

#define IS_VALID_TIMER4_SEVT_TRIG_EVT(x)                                       \
(   ((x) == TIMER4_SEVT_TRIGGER_TMR4_SCMUH)     ||                             \
    ((x) == TIMER4_SEVT_TRIGGER_TMR4_SCMUL)     ||                             \
    ((x) == TIMER4_SEVT_TRIGGER_TMR4_SCMVH)     ||                             \
    ((x) == TIMER4_SEVT_TRIGGER_TMR4_SCMVL)     ||                             \
    ((x) == TIMER4_SEVT_TRIGGER_TMR4_SCMWH)     ||                             \
    ((x) == TIMER4_SEVT_TRIGGER_TMR4_SCMWL))

#define IS_VALID_TIMER4_SEVT_BUF_MODE(x)                                       \
(   ((x) == TIMER4_SEVT_BUF_DISABLE)            ||                             \
    ((x) == TIMER4_SEVT_BUF_CNT_ZERO)           ||                             \
    ((x) == TIMER4_SEVT_BUF_CNT_PEAK)           ||                             \
    ((x) == TIMER4_SEVT_BUF_CNT_ZERO_OR_PEAK))

#define IS_VALID_TIMER4_EMB_PWM_OP(x)                                          \
(   ((x) == TIMER4_EMB_TRIG_PWM_OP_NORMAL)      ||                             \
    ((x) == TIMER4_EMB_TRIG_PWM_OP_HOLD)        ||                             \
    ((x) == TIMER4_EMB_TRIG_PWM_OP_HIZ)         ||                             \
    ((x) == TIMER4_EMB_TRIG_PWM_OP_LOW)         ||                             \
    ((x) == TIMER4_EMB_TRIG_PWM_OP_HIGH))

/**
 * @}
 */

/**
 * @defgroup TIMER4_OCO_Register TIMER4 OCO Register
 * @brief Get the specified OCO register address of the specified Timer4 unit
 * @{
 */
#define TMR4_OCCRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->OCCRUH)) + ((uint32_t)(__CH__))*4ul))
#define TMR4_OCMRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->OCMRHUH)) + ((uint32_t)(__CH__))*4ul))
#define TMR4_OCERx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->OCERU)) + (((uint32_t)(__CH__))/2ul)*4ul))
#define TMR4_OCSRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->OCSRU)) + (((uint32_t)(__CH__))/2ul)*4ul))
/**
 * @}
 */

/**
 * @defgroup TIMER4_PWM_Register TIMER4 PWM Register
 * @brief Get the specified PWM register address of the specified Timer4 unit
 * @{
 */
#define TMR4_RCSRx(__TMR4x__)               ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->RCSR))))
#define TMR4_POCRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->POCRU)) + ((uint32_t)(__CH__))*4ul))
#define TMR4_PDARx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->PDARU)) + ((uint32_t)(__CH__))*8ul))
#define TMR4_PDBRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->PDBRU)) + ((uint32_t)(__CH__))*8ul))
#define TMR4_PFSRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->PFSRU)) + ((uint32_t)(__CH__))*8ul))
/**
 * @}
 */

/**
 * @defgroup TMR4_RCSR_Bit TMR4_RCSR Bit
 * @brief Get the specified TMR4_RCSR register bis value of the specified Timer4 PWM channel
 * @{
 */
#define TMR4_RCSR_RTIDx(__CH__)             ((uint16_t)(TMR4_RCSR_RTIDU << (__CH__)))
#define TMR4_RCSR_RTIFx(__CH__)             ((uint16_t)(TMR4_RCSR_RTIFU << ((__CH__)*4u)))
#define TMR4_RCSR_RTICx(__CH__)             ((uint16_t)(TMR4_RCSR_RTICU <<((__CH__)*4u)))
#define TMR4_RCSR_RTEx(__CH__)              ((uint16_t)(TMR4_RCSR_RTEU << ((__CH__)*4u)))
#define TMR4_RCSR_RTSx(__CH__)              ((uint16_t)(TMR4_RCSR_RTSU << ((__CH__)*4u)))
/**
 * @}
 */

/**
 * @defgroup TIMER4_SEVT_Register TIMER4 SEVT Register
 * @brief Get the specified SEVT register address of the specified Timer4 unit
 * @{
 */
#define TMR4_SCCRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->SCCRUH)) + ((uint32_t)(__CH__))*4ul))
#define TMR4_SCSRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->SCSRUH)) + ((uint32_t)(__CH__))*4ul))
#define TMR4_SCMRx(__TMR4x__, __CH__)       ((__IO uint16_t *)((uint32_t)(&((__TMR4x__)->SCMRUH)) + ((uint32_t)(__CH__))*4ul))
/**
 * @}
 */

/**
 * @defgroup TIMER4_EMB_Register TIMER4 EMB Register
 * @brief Get the specified EMB register address of the specified Timer4 unit
 * @{
 */
#define TMR4_ESCRx(__TMR4x__)               ((__IO uint32_t *)((uint32_t)(&((__TMR4x__)->ECSR))))
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
 * @defgroup TIMER4_Global_Functions TIMER4 Global Functions
 * @{
 */

/**
 * @defgroup TIMER4_CNT_Global_Functions TIMER4 Counter Global Functions
 * @{
 */

/**
 * @brief  Initialize Timer4 CNT.
 * @param  [in] pstcInit                Pointer to a @ref stc_timer4_cnt_init_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t TIMER4_CNT_Init(const stc_timer4_cnt_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (NULL != pstcInit)
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enBufferCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enZeroIntCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enPeakIntCmd));
        DDL_ASSERT(IS_VALID_TIMER4_CNT_MODE(pstcInit->u16CntMode));
        DDL_ASSERT(IS_VALID_TIMER4_CNT_CLK_DIV(pstcInit->u16ClkDiv));
        DDL_ASSERT(IS_VALID_TIMER4_CNT_CLK_SRC(pstcInit->u16ClkSrc));
        DDL_ASSERT(IS_VALID_TIMER4_CNT_INT_MSK(pstcInit->u16ZeroIntMask));
        DDL_ASSERT(IS_VALID_TIMER4_CNT_INT_MSK(pstcInit->u16PeakIntMask));

        /* Set default value */
        WRITE_REG16(M0P_TMR4->CCSR, 0x0050u);
        WRITE_REG16(M0P_TMR4->CVPR, 0x0000u);

        /* Set count clock div && cnt mode && buffer enable bit && external clock enable bit && interrupt enable bit */
        WRITE_REG16(M0P_TMR4->CCSR, \
                    (pstcInit->u16ClkDiv  | \
                     pstcInit->u16ClkSrc  | \
                     pstcInit->u16CntMode | \
                     TMR4_CCSR_STOP       | \
                     (uint16_t)((uint16_t)pstcInit->enBufferCmd  << TMR4_CCSR_BUFEN_POS)  | \
                     (uint16_t)((uint16_t)pstcInit->enZeroIntCmd << TMR4_CCSR_IRQZEN_POS) | \
                     (uint16_t)((uint16_t)pstcInit->enPeakIntCmd << TMR4_CCSR_IRQPEN_POS)));

        /* set interrupt mask times */
        WRITE_REG16(M0P_TMR4->CVPR, (pstcInit->u16ZeroIntMask | ((uint16_t)(pstcInit->u16PeakIntMask << TMR4_CVPR_PIM_POS))));

        /* Set Timer4 cycle */
        WRITE_REG16(M0P_TMR4->CPSR, pstcInit->u16CycleVal);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set the fields of structure stc_timer4_cnt_init_t to default values
 * @param  [out] pstcInit               Pointer to a @ref stc_timer4_cnt_init_t structure (M0P_TMR4 unit base function configuration data structure)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t TIMER4_CNT_StructInit(stc_timer4_cnt_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (NULL != pstcInit)
    {
        pstcInit->u16CycleVal = 0xFFFFu;
        pstcInit->u16CntMode = TIMER4_CNT_MODE_SAWTOOTH_WAVE;
        pstcInit->u16ClkSrc = TIMER4_CNT_PCLK;
        pstcInit->u16ClkDiv = TIMER4_CNT_CLK_DIV1;
        pstcInit->u16ZeroIntMask = TIMER4_CNT_INT_MASK_0;
        pstcInit->u16PeakIntMask = TIMER4_CNT_INT_MASK_0;
        pstcInit->enBufferCmd = Disable;
        pstcInit->enZeroIntCmd = Disable;
        pstcInit->enPeakIntCmd = Disable;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-Initialize Timer4 count function
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize success
 */
en_result_t TIMER4_CNT_DeInit(void)
{
    /* Configures the registers to reset value. */
    WRITE_REG16(M0P_TMR4->CCSR, 0x0050u);
    WRITE_REG16(M0P_TMR4->CPSR, 0xFFFFu);
    WRITE_REG16(M0P_TMR4->CVPR, 0x0000u);

    return Ok;
}

/**
 * @brief  Set Timer4 counter clock division
 * @param  [in] u16Div                  Timer4 clock division
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_CLK_DIV1:      HCLK
 *           @arg TIMER4_CNT_CLK_DIV2:      HCLK/2
 *           @arg TIMER4_CNT_CLK_DIV4:      HCLK/4
 *           @arg TIMER4_CNT_CLK_DIV8:      HCLK/8
 *           @arg TIMER4_CNT_CLK_DIV16:     HCLK/16
 *           @arg TIMER4_CNT_CLK_DIV32:     HCLK/32
 *           @arg TIMER4_CNT_CLK_DIV64:     HCLK/64
 *           @arg TIMER4_CNT_CLK_DIV128:    HCLK/128
 *           @arg TIMER4_CNT_CLK_DIV256:    HCLK/256
 *           @arg TIMER4_CNT_CLK_DIV512:    HCLK/512
 *           @arg TIMER4_CNT_CLK_DIV1024:   HCLK/1024
 * @retval None
 */
void TIMER4_CNT_SetClkDiv(uint16_t u16Div)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4_CNT_CLK_DIV(u16Div));

    MODIFY_REG16(M0P_TMR4->CCSR, TMR4_CCSR_CKDIV, u16Div);
}

/**
 * @brief  Get Timer4 counter clock division
 * @param  None
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_CNT_CLK_DIV1:      HCLK
 *           @arg TIMER4_CNT_CLK_DIV2:      HCLK/2
 *           @arg TIMER4_CNT_CLK_DIV4:      HCLK/4
 *           @arg TIMER4_CNT_CLK_DIV8:      HCLK/8
 *           @arg TIMER4_CNT_CLK_DIV16:     HCLK/16
 *           @arg TIMER4_CNT_CLK_DIV32:     HCLK/32
 *           @arg TIMER4_CNT_CLK_DIV64:     HCLK/64
 *           @arg TIMER4_CNT_CLK_DIV128:    HCLK/128
 *           @arg TIMER4_CNT_CLK_DIV256:    HCLK/256
 *           @arg TIMER4_CNT_CLK_DIV512:    HCLK/512
 *           @arg TIMER4_CNT_CLK_DIV1024:   HCLK/1024
 */
uint16_t TIMER4_CNT_GetClkDiv(void)
{
    return READ_REG16_BIT(M0P_TMR4->CCSR, TMR4_CCSR_CKDIV);
}

/**
 * @brief  Enable or disable specified Timer4 counter interrupt
 * @param  [in] u16IntSource            Timer4 interrupt source
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_IT_PEAK:   Overflow interrupt
 *           @arg TIMER4_CNT_IT_ZERO:   Underflow interrupt
 * @param  [in] enNewSta                The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval None
 */
void TIMER4_CNT_IntCmd(uint16_t u16IntSource, en_functional_state_t enNewSta)
{
    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));
    DDL_ASSERT(IS_VALID_TIMER4_CNT_INT(u16IntSource));

    switch (u16IntSource)
    {
        case TIMER4_CNT_IT_ZERO:
            bM0P_TMR4->CCSR_b.IRQZEN = enNewSta;
            break;
        case TIMER4_CNT_IT_PEAK:
            bM0P_TMR4->CCSR_b.IRQPEN = enNewSta;
            break;
        default:
            break;
    }
}

/**
 * @brief  Set Timer4 CNT interrupt mask times
 * @param  [in] u16IntSource            Timer4 interrupt source
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_IT_PEAK:   Overflow interrupt
 *           @arg TIMER4_CNT_IT_ZERO:   Underflow interrupt
 * @param [in] u16MaskTimes                 Timer4 CNT interrupt mask times
 *           @arg TIMER4_CNT_INT_MASK_0:    CNT interrupt flag is always set(not masked) for every CNT count at "0x0000" or peak
 *           @arg TIMER4_CNT_INT_MASK_1:    CNT interrupt flag is set once for 2 every CNT counts at "0x0000" or peak (skiping 1 count)
 *           @arg TIMER4_CNT_INT_MASK_2:    CNT interrupt flag is set once for 3 every CNT counts at "0x0000" or peak (skiping 2 count)
 *           @arg TIMER4_CNT_INT_MASK_3:    CNT interrupt flag is set once for 4 every CNT counts at "0x0000" or peak (skiping 3 count)
 *           @arg TIMER4_CNT_INT_MASK_4:    CNT interrupt flag is set once for 5 every CNT counts at "0x0000" or peak (skiping 4 count)
 *           @arg TIMER4_CNT_INT_MASK_5:    CNT interrupt flag is set once for 6 every CNT counts at "0x0000" or peak (skiping 5 count)
 *           @arg TIMER4_CNT_INT_MASK_6:    CNT interrupt flag is set once for 7 every CNT counts at "0x0000" or peak (skiping 6 count)
 *           @arg TIMER4_CNT_INT_MASK_7:    CNT interrupt flag is set once for 8 every CNT counts at "0x0000" or peak (skiping 7 count)
 *           @arg TIMER4_CNT_INT_MASK_8:    CNT interrupt flag is set once for 9 every CNT counts at "0x0000" or peak (skiping 8 count)
 *           @arg TIMER4_CNT_INT_MASK_9:    CNT interrupt flag is set once for 10 every CNT counts at "0x0000" or peak (skiping 9 count)
 *           @arg TIMER4_CNT_INT_MASK_10:   CNT interrupt flag is set once for 11 every CNT counts at "0x0000" or peak (skiping 10 count)
 *           @arg TIMER4_CNT_INT_MASK_11:   CNT interrupt flag is set once for 12 every CNT counts at "0x0000" or peak (skiping 11 count)
 *           @arg TIMER4_CNT_INT_MASK_12:   CNT interrupt flag is set once for 13 every CNT counts at "0x0000" or peak (skiping 12 count)
 *           @arg TIMER4_CNT_INT_MASK_13:   CNT interrupt flag is set once for 14 every CNT counts at "0x0000" or peak (skiping 13 count)
 *           @arg TIMER4_CNT_INT_MASK_14:   CNT interrupt flag is set once for 15 every CNT counts at "0x0000" or peak (skiping 14 count)
 *           @arg TIMER4_CNT_INT_MASK_15:   CNT interrupt flag is set once for 16 every CNT counts at "0x0000" or peak (skiping 15 count)
 * @retval None
 */
void TIMER4_CNT_SetIntMaskTimes(uint16_t u16IntSource,
                                uint16_t u16MaskTimes)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4_CNT_INT(u16IntSource));
    DDL_ASSERT(IS_VALID_TIMER4_CNT_INT_MSK(u16MaskTimes));

    switch (u16IntSource)
    {
        case TIMER4_CNT_IT_ZERO:
            MODIFY_REG16(M0P_TMR4->CVPR, TMR4_CVPR_ZIM, (uint16_t)(u16MaskTimes << TMR4_CVPR_ZIM_POS));
            break;
        case TIMER4_CNT_IT_PEAK:
            MODIFY_REG16(M0P_TMR4->CVPR, TMR4_CVPR_PIM, (uint16_t)(u16MaskTimes << TMR4_CVPR_PIM_POS));
            break;
        default:
            break;
    }
}

/**
 * @brief  Get Timer4 CNT interrupt mask times
 * @param  [in] u16IntSource            Timer4 interrupt source
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_IT_PEAK:   Overflow interrupt
 *           @arg TIMER4_CNT_IT_ZERO:   Underflow interrupt
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_CNT_INT_MASK_0:    CNT interrupt flag is always set(not masked) for every CNT count at "0x0000" or peak
 *           @arg TIMER4_CNT_INT_MASK_1:    CNT interrupt flag is set once for 2 every CNT counts at "0x0000" or peak (skiping 1 count)
 *           @arg TIMER4_CNT_INT_MASK_2:    CNT interrupt flag is set once for 3 every CNT counts at "0x0000" or peak (skiping 2 count)
 *           @arg TIMER4_CNT_INT_MASK_3:    CNT interrupt flag is set once for 4 every CNT counts at "0x0000" or peak (skiping 3 count)
 *           @arg TIMER4_CNT_INT_MASK_4:    CNT interrupt flag is set once for 5 every CNT counts at "0x0000" or peak (skiping 4 count)
 *           @arg TIMER4_CNT_INT_MASK_5:    CNT interrupt flag is set once for 6 every CNT counts at "0x0000" or peak (skiping 5 count)
 *           @arg TIMER4_CNT_INT_MASK_6:    CNT interrupt flag is set once for 7 every CNT counts at "0x0000" or peak (skiping 6 count)
 *           @arg TIMER4_CNT_INT_MASK_7:    CNT interrupt flag is set once for 8 every CNT counts at "0x0000" or peak (skiping 7 count)
 *           @arg TIMER4_CNT_INT_MASK_8:    CNT interrupt flag is set once for 9 every CNT counts at "0x0000" or peak (skiping 8 count)
 *           @arg TIMER4_CNT_INT_MASK_9:    CNT interrupt flag is set once for 10 every CNT counts at "0x0000" or peak (skiping 9 count)
 *           @arg TIMER4_CNT_INT_MASK_10:   CNT interrupt flag is set once for 11 every CNT counts at "0x0000" or peak (skiping 10 count)
 *           @arg TIMER4_CNT_INT_MASK_11:   CNT interrupt flag is set once for 12 every CNT counts at "0x0000" or peak (skiping 11 count)
 *           @arg TIMER4_CNT_INT_MASK_12:   CNT interrupt flag is set once for 13 every CNT counts at "0x0000" or peak (skiping 12 count)
 *           @arg TIMER4_CNT_INT_MASK_13:   CNT interrupt flag is set once for 14 every CNT counts at "0x0000" or peak (skiping 13 count)
 *           @arg TIMER4_CNT_INT_MASK_14:   CNT interrupt flag is set once for 15 every CNT counts at "0x0000" or peak (skiping 14 count)
 *           @arg TIMER4_CNT_INT_MASK_15:   CNT interrupt flag is set once for 16 every CNT counts at "0x0000" or peak (skiping 15 count)
 *           @arg TIMER4_RESULT_ERROR:      Timer4 interrupt source is invalid
 */
uint16_t TIMER4_CNT_GetIntMaskTimes(uint16_t u16IntSource)
{
    uint16_t u16MaskTimes;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4_CNT_INT(u16IntSource));

    switch (u16IntSource)
    {
        case TIMER4_CNT_IT_ZERO:
            u16MaskTimes = (uint16_t)(READ_REG16_BIT(M0P_TMR4->CVPR, TMR4_CVPR_ZIM) >> TMR4_CVPR_ZIM_POS);
            break;
        case TIMER4_CNT_IT_PEAK:
            u16MaskTimes = (uint16_t)(READ_REG16_BIT(M0P_TMR4->CVPR, TMR4_CVPR_PIM) >> TMR4_CVPR_PIM_POS);
            break;
        default:
            u16MaskTimes = TIMER4_RESULT_ERROR;
            break;
    }

    return u16MaskTimes;
}

/**
 * @brief  Get Timer4 CNT interrupt mask current times
 * @param  [in] u16IntSource            Timer4 interrupt source
 *         This parameter can be one of the following values:
 *           @arg TIMER4_CNT_IT_PEAK:   Overflow interrupt
 *           @arg TIMER4_CNT_IT_ZERO:   Underflow interrupt
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_CNT_INT_MASK_0:    CNT interrupt flag is always set(not masked) for every CNT count at "0x0000" or peak
 *           @arg TIMER4_CNT_INT_MASK_1:    CNT interrupt flag is set once for 2 every CNT counts at "0x0000" or peak (skiping 1 count)
 *           @arg TIMER4_CNT_INT_MASK_2:    CNT interrupt flag is set once for 3 every CNT counts at "0x0000" or peak (skiping 2 count)
 *           @arg TIMER4_CNT_INT_MASK_3:    CNT interrupt flag is set once for 4 every CNT counts at "0x0000" or peak (skiping 3 count)
 *           @arg TIMER4_CNT_INT_MASK_4:    CNT interrupt flag is set once for 5 every CNT counts at "0x0000" or peak (skiping 4 count)
 *           @arg TIMER4_CNT_INT_MASK_5:    CNT interrupt flag is set once for 6 every CNT counts at "0x0000" or peak (skiping 5 count)
 *           @arg TIMER4_CNT_INT_MASK_6:    CNT interrupt flag is set once for 7 every CNT counts at "0x0000" or peak (skiping 6 count)
 *           @arg TIMER4_CNT_INT_MASK_7:    CNT interrupt flag is set once for 8 every CNT counts at "0x0000" or peak (skiping 7 count)
 *           @arg TIMER4_CNT_INT_MASK_8:    CNT interrupt flag is set once for 9 every CNT counts at "0x0000" or peak (skiping 8 count)
 *           @arg TIMER4_CNT_INT_MASK_9:    CNT interrupt flag is set once for 10 every CNT counts at "0x0000" or peak (skiping 9 count)
 *           @arg TIMER4_CNT_INT_MASK_10:   CNT interrupt flag is set once for 11 every CNT counts at "0x0000" or peak (skiping 10 count)
 *           @arg TIMER4_CNT_INT_MASK_11:   CNT interrupt flag is set once for 12 every CNT counts at "0x0000" or peak (skiping 11 count)
 *           @arg TIMER4_CNT_INT_MASK_12:   CNT interrupt flag is set once for 13 every CNT counts at "0x0000" or peak (skiping 12 count)
 *           @arg TIMER4_CNT_INT_MASK_13:   CNT interrupt flag is set once for 14 every CNT counts at "0x0000" or peak (skiping 13 count)
 *           @arg TIMER4_CNT_INT_MASK_14:   CNT interrupt flag is set once for 15 every CNT counts at "0x0000" or peak (skiping 14 count)
 *           @arg TIMER4_CNT_INT_MASK_15:   CNT interrupt flag is set once for 16 every CNT counts at "0x0000" or peak (skiping 15 count)
 *           @arg TIMER4_RESULT_ERROR:      Timer4 interrupt source is invalid
 */
uint16_t TIMER4_CNT_GetIntMaskCurrentTimes(uint16_t u16IntSource)
{
    uint16_t u16MaskTimes;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4_CNT_INT(u16IntSource));

    switch (u16IntSource)
    {
        case TIMER4_CNT_IT_ZERO:
            u16MaskTimes = (uint16_t)(READ_REG16_BIT(M0P_TMR4->CVPR, TMR4_CVPR_ZIC) >> TMR4_CVPR_ZIC_POS);
            break;
        case TIMER4_CNT_IT_PEAK:
            u16MaskTimes = (uint16_t)(READ_REG16_BIT(M0P_TMR4->CVPR, TMR4_CVPR_PIC) >> TMR4_CVPR_PIC_POS);
            break;
        default:
            u16MaskTimes = TIMER4_RESULT_ERROR;
            break;
    }

    return u16MaskTimes;
}
/**
 * @}
 */

/**
 * @defgroup TIMER4_OCO_Global_Functions TIMER4 OCO Global Functions
 * @{
 */

/**
 * @brief  Initialize Timer4 OCO
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] pstcInit            Pointer to a @ref stc_timer4_oco_init_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: u32Ch is invalid or pstcInit = NULL
 */
en_result_t TIMER4_OCO_Init(uint32_t u32Ch,
                                const stc_timer4_oco_init_t *pstcInit)
{
    __IO uint16_t *TMR4_OCER;
    __IO uint16_t *TMR4_OCSR;
    __IO uint16_t *TMR4_OCCR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if ((IS_VALID_TIMER4_OCO_CH(u32Ch)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enOcoCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enOcoIntCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enOccrLinkTransfer));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enOcmrLinkTransfer));
        DDL_ASSERT(IS_VALID_TIMER4_OCO_OCCR_BUF_MODE(pstcInit->u16OccrBufMode));
        DDL_ASSERT(IS_VALID_TIMER4_OCO_OCMR_BUF_MODE(pstcInit->u16OcmrBufMode));
        DDL_ASSERT(IS_VALID_TIMER4_OCO_PORT_INVALID_OP(pstcInit->u16OcoInvalidOp));

        enRet = Ok;
        /* Get pointer of current channel OCO register address */
        TMR4_OCSR = TMR4_OCSRx(M0P_TMR4, u32Ch);
        TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);
        TMR4_OCCR = TMR4_OCCRx(M0P_TMR4, u32Ch);

        /* Set OCMR and OCCR buffer mode */
        if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch)) /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
        {
            /* Set port output valid && OP level && interrupt */
            MODIFY_REG16(*TMR4_OCSR, \
                         (TMR4_OCSR_OCEH  | \
                          TMR4_OCSR_OCPH  | \
                          TMR4_OCSR_OCIEH | \
                          TMR4_OCSR_OCFH),  \
                         ((uint16_t)((uint16_t)pstcInit->enOcoCmd << TMR4_OCSR_OCEH_POS)    | \
                          (uint16_t)(pstcInit->u16OcoInvalidOp << TMR4_OCSR_OCPH_POS)       | \
                          (uint16_t)((uint16_t)pstcInit->enOcoIntCmd << TMR4_OCSR_OCIEH_POS)));

            /* Set OCMR&&OCCR buffer */
            MODIFY_REG16(*TMR4_OCER, \
                         (TMR4_OCER_CHBUFEN | \
                          TMR4_OCER_MHBUFEN | \
                          TMR4_OCER_LMCH    | \
                          TMR4_OCER_LMMH),    \
                         ((uint16_t)(pstcInit->u16OccrBufMode << TMR4_OCER_CHBUFEN_POS)  | \
                          (uint16_t)(pstcInit->u16OcmrBufMode << TMR4_OCER_MHBUFEN_POS)  | \
                          (uint16_t)((uint16_t)pstcInit->enOccrLinkTransfer << TMR4_OCER_LMCH_POS) | \
                          (uint16_t)((uint16_t)pstcInit->enOcmrLinkTransfer << TMR4_OCER_LMMH_POS)));
        }
        else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
        {
            /* Set port output valid && OP level && interrupt */
            MODIFY_REG16(*TMR4_OCSR, \
                         (TMR4_OCSR_OCEL  | \
                          TMR4_OCSR_OCPL  | \
                          TMR4_OCSR_OCIEL | \
                          TMR4_OCSR_OCFL),  \
                         ((uint16_t)((uint16_t)pstcInit->enOcoCmd << TMR4_OCSR_OCEL_POS)    | \
                          (uint16_t)(pstcInit->u16OcoInvalidOp << TMR4_OCSR_OCPL_POS)       | \
                          (uint16_t)((uint16_t)pstcInit->enOcoIntCmd << TMR4_OCSR_OCIEL_POS)));

            /* Set OCMR&&OCCR buffer */
            MODIFY_REG16(*TMR4_OCER, \
                         (TMR4_OCER_CLBUFEN | \
                          TMR4_OCER_MLBUFEN | \
                          TMR4_OCER_LMCL    | \
                          TMR4_OCER_LMML),    \
                         ((uint16_t)(pstcInit->u16OccrBufMode << TMR4_OCER_CLBUFEN_POS)  | \
                          (uint16_t)(pstcInit->u16OcmrBufMode << TMR4_OCER_MLBUFEN_POS)  | \
                          (uint16_t)((uint16_t)pstcInit->enOccrLinkTransfer << TMR4_OCER_LMCL_POS) | \
                          (uint16_t)((uint16_t)pstcInit->enOcmrLinkTransfer << TMR4_OCER_LMML_POS)));
        }
        else
        {
            enRet = ErrorInvalidParameter;;
        }

        /* Set OCO compare match value */
        WRITE_REG16(*TMR4_OCCR, pstcInit->u16OccrVal);
    }

    return enRet;
}

/**
 * @brief  Initialize Timer4 OCO
 * @brief  Set the fields of structure stc_timer4_oco_init_t to default values
 * @param  [out] pstcInit               Pointer to a @ref stc_timer4_oco_init_t structure (TMRBx unit OCO function configuration data structure)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t TIMER4_OCO_StructInit(stc_timer4_oco_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (NULL != pstcInit)
    {
        pstcInit->enOcoIntCmd = Disable;
        pstcInit->enOcoCmd = Disable;
        pstcInit->enOccrLinkTransfer = Disable;
        pstcInit->enOcmrLinkTransfer = Disable;
        pstcInit->u16OccrBufMode = TIMER4_OCO_OCCR_BUF_DISABLE;
        pstcInit->u16OcmrBufMode = TIMER4_OCO_OCMR_BUF_DISABLE;
        pstcInit->u16OcoInvalidOp = TIMER4_OCO_INVAILD_OP_LOW;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-initialize Timer4 OCO
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_DeInit(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_OCER;
    __IO uint16_t *TMR4_OCSR;
    en_result_t enRet = Ok;

    /* Get pointer of current channel OCO register address */
    TMR4_OCSR = TMR4_OCSRx(M0P_TMR4, u32Ch);
    TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);

    /* Set OCMR and OCCR buffer mode */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch)) /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        CLEAR_REG16_BIT(*TMR4_OCSR, (TMR4_OCSR_OCEH | TMR4_OCSR_OCFH));

        /* Clear bits: port output valid && OP level && interrupt */
        CLEAR_REG16_BIT(*TMR4_OCSR, \
                        (TMR4_OCSR_OCEH  | TMR4_OCSR_OCPH | \
                        TMR4_OCSR_OCIEH | TMR4_OCSR_OCFH));

        /* Clear bits: OCMR&&OCCR buffer */
        CLEAR_REG16_BIT(*TMR4_OCER, \
                        (TMR4_OCER_CHBUFEN | TMR4_OCER_MHBUFEN | \
                         TMR4_OCER_LMCH    | TMR4_OCER_LMMH));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        CLEAR_REG16_BIT(*TMR4_OCSR, (TMR4_OCSR_OCEL | TMR4_OCSR_OCFL));

        /* Clear bits: port output valid && OP level && interrupt */
        CLEAR_REG16_BIT(*TMR4_OCSR, \
                        (TMR4_OCSR_OCEL  | TMR4_OCSR_OCPL | \
                         TMR4_OCSR_OCIEL | TMR4_OCSR_OCFL));

        /* Clear bits: OCMR&&OCCR buffer */
        CLEAR_REG16_BIT(*TMR4_OCER, \
                        (TMR4_OCER_CLBUFEN | TMR4_OCER_MLBUFEN | \
                         TMR4_OCER_LMCL    | TMR4_OCER_LMML));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 OCO OCCR buffer mode
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] u16OccrBufMode      Timer4 OCO OCCR buffer mode
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_OCCR_BUF_DISABLE:          Disable the register OCCR buffer function
 *           @arg TIMER4_OCO_OCCR_BUF_CNT_ZERO:         Register OCCR buffer transfer when counter value is 0x0000
 *           @arg TIMER4_OCO_OCCR_BUF_CNT_PEAK:         Register OCCR buffer transfer when counter value is CPSR
 *           @arg TIMER4_OCO_OCCR_BUF_CNT_ZERO_OR_PEAK: Register OCCR buffer transfer when the value is both 0 and CPSR
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_SetOccrBufMode(uint32_t u32Ch,
                                uint16_t u16OccrBufMode)
{
    __IO uint16_t *TMR4_OCER;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4_OCO_OCCR_BUF_MODE(u16OccrBufMode));

    /* Get pointer of current channel OCO register address */
    TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);

    /* Set OCCR buffer mode */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_CHBUFEN, (uint16_t)(u16OccrBufMode << TMR4_OCER_CHBUFEN_POS));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_CLBUFEN, (uint16_t)(u16OccrBufMode << TMR4_OCER_CLBUFEN_POS));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Get Timer4 OCO OCCR buffer mode
 *         This parameter can be one of the following values:
 * @param  [in] u32Ch               Timer4 OCO channel
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_OCO_OCCR_BUF_DISABLE:          Disable the register OCCR buffer function
 *           @arg TIMER4_OCO_OCCR_BUF_CNT_ZERO:         Register OCCR buffer transfer when counter value is 0x0000
 *           @arg TIMER4_OCO_OCCR_BUF_CNT_PEAK:         Register OCCR buffer transfer when counter value is CPSR
 *           @arg TIMER4_OCO_OCCR_BUF_CNT_ZERO_OR_PEAK: Register OCCR buffer transfer when the value is both 0 and CPSR
 *           @arg TIMER4_RESULT_ERROR:                  Timer4 OCO channel is invalid
 */
uint16_t TIMER4_OCO_GetOccrBufMode(uint32_t u32Ch)
{
    uint16_t OccrBufMode;
    __IO uint16_t *TMR4_OCER;

    /* Get pointer of current channel OCO register address */
    TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);

    /* Get OCCR buffer mode */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        OccrBufMode = (READ_REG16_BIT(*TMR4_OCER, TMR4_OCER_CHBUFEN) >> TMR4_OCER_CHBUFEN_POS);
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        OccrBufMode = (READ_REG16_BIT(*TMR4_OCER, TMR4_OCER_CLBUFEN) >> TMR4_OCER_CLBUFEN_POS);
    }
    else
    {
        OccrBufMode = TIMER4_RESULT_ERROR;
    }

    return OccrBufMode;
}

/**
 * @brief  Set Timer4 OCO OCCR buffer link transfer function
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] enNewSta            The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_OccrLinkTransferCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta)
{
    __IO uint16_t *TMR4_OCER;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    /* Get pointer of current channel OCO register address */
    TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);

    /* Set OCCR link transfer */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_LMCH, ((uint16_t)enNewSta << TMR4_OCER_LMCH_POS));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_LMCL, ((uint16_t)enNewSta << TMR4_OCER_LMCL_POS));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 OCO OCMR buffer mode
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] u16OcmrBufMode      Timer4 OCO OCCR buffer mode
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_OCMR_BUF_DISABLE:          Disable the register OCMR buffer function
 *           @arg TIMER4_OCO_OCMR_BUF_CNT_ZERO:         Register OCMR buffer transfer when counter value is 0x0000
 *           @arg TIMER4_OCO_OCMR_BUF_CNT_PEAK:         Register OCMR buffer transfer when counter value is CPSR
 *           @arg TIMER4_OCO_OCMR_BUF_CNT_ZERO_OR_PEAK: Register OCMR buffer transfer when the value is both 0 and CPSR
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_SetOcmrBufMode(uint32_t u32Ch,
                                uint16_t u16OcmrBufMode)
{
    __IO uint16_t *TMR4_OCER;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4_OCO_OCMR_BUF_MODE(u16OcmrBufMode));

    /* Get pointer of current channel OCO register address */
    TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);

    /* Set OCMR buffer mode */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_MHBUFEN, (uint16_t)(u16OcmrBufMode << TMR4_OCER_MHBUFEN_POS));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_MLBUFEN, (uint16_t)(u16OcmrBufMode << TMR4_OCER_MLBUFEN_POS));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Get Timer4 OCO OCMR buffer mode
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @retval Returned value can be one of the following values:
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_OCMR_BUF_DISABLE:          Disable the register OCMR buffer function
 *           @arg TIMER4_OCO_OCMR_BUF_CNT_ZERO:         Register OCMR buffer transfer when counter value is 0x0000
 *           @arg TIMER4_OCO_OCMR_BUF_CNT_PEAK:         Register OCMR buffer transfer when counter value is CPSR
 *           @arg TIMER4_OCO_OCMR_BUF_CNT_ZERO_OR_PEAK: Register OCMR buffer transfer when the value is both 0 and CPSR
 *           @arg TIMER4_RESULT_ERROR:                  Timer4 OCO channel is invalid
 */
uint16_t TIMER4_OCO_GetOcmrBufMode(uint32_t u32Ch)
{
    uint16_t u16OcmrBufMode;
    __IO uint16_t *TMR4_OCER;

    /* Get pointer of current channel OCO register address */
    TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);

    /* Get OCMR buffer mode */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        u16OcmrBufMode = (READ_REG16_BIT(*TMR4_OCER, TMR4_OCER_MHBUFEN) >> TMR4_OCER_MHBUFEN_POS);
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        u16OcmrBufMode = (READ_REG16_BIT(*TMR4_OCER, TMR4_OCER_MLBUFEN) >> TMR4_OCER_MLBUFEN_POS);
    }
    else
    {
        u16OcmrBufMode = TIMER4_RESULT_ERROR;
    }

    return u16OcmrBufMode;
}

/**
 * @brief  Set Timer4 OCO OCMR buffer link transfer function
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] enNewSta            The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_OcmrLinkTransferCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta)
{
    __IO uint16_t *TMR4_OCER;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    /* Get pointer of current channel OCO register address */
    TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);

    /* Set OCCR link transfer */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_LMMH, ((uint16_t)enNewSta << TMR4_OCER_LMMH_POS));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_LMML, ((uint16_t)enNewSta << TMR4_OCER_LMML_POS));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Extend the matching conditions of Timer4 OCO channel
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] enNewSta            The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_ExtMatchCondCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta)
{
    __IO uint16_t *TMR4_OCER;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    /* Get pointer of current channel OCO register address */
    TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);

    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_MCECH, ((uint16_t)enNewSta << TMR4_OCER_MCECH_POS));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_MCECL, ((uint16_t)enNewSta << TMR4_OCER_MCECL_POS));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Set the Timer4 OCO high channel mode
 * @param  [in] u32Ch               Timer4 OCO channel.
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 * @param  [in] pstcMode            Pointer to a @ref stc_oco_high_ch_compare_mode_t structure of the Timer4 OCO high channel mode
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid or pstcMode = NULL
 */
en_result_t TIMER4_OCO_SetHighChCompareMode(uint32_t u32Ch,
                                const stc_oco_high_ch_compare_mode_t *pstcMode)
{
    __IO uint16_t *TMR4_OCER;
    __IO uint16_t *TMR4_OCMR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check OCO channel && pstcMode pointer */
    if ((IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch)) && (NULL != pstcMode))
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcMode->enExtendMatchCondCmd));

        /* Get pointer of current channel OCO register address */
        TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);
        TMR4_OCMR = TMR4_OCMRx(M0P_TMR4, u32Ch);

        WRITE_REG16(*TMR4_OCMR, pstcMode->OCMRx);
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_MCECH, ((uint16_t)pstcMode->enExtendMatchCondCmd << TMR4_OCER_MCECH_POS));
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set the Timer4 OCO low channel mode
 * @param  [in] u32Ch               Timer4 OCO channel.
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 * @param  [in] pstcMode            Pointer to a @ref stc_oco_low_ch_compare_mode_t structure of the Timer4 OCO low channel mode
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid or pstcMode = NULL
 */
en_result_t TIMER4_OCO_SetLowChCompareMode(uint32_t u32Ch,
                                 const stc_oco_low_ch_compare_mode_t *pstcMode)
{
    __IO uint16_t *TMR4_OCER;
    __IO uint32_t *TMR4_OCMR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check OCO channel && pstcMode pointer */
    if ((IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) && (NULL != pstcMode))
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcMode->enExtendMatchCondCmd));

        /* Get pointer of current channel OCO register address */
        TMR4_OCER = TMR4_OCERx(M0P_TMR4, u32Ch);
        TMR4_OCMR = (__IO uint32_t*)TMR4_OCMRx(M0P_TMR4, u32Ch);

        WRITE_REG32(*TMR4_OCMR, pstcMode->OCMRx);
        MODIFY_REG16(*TMR4_OCER, TMR4_OCER_MCECL, ((uint16_t)pstcMode->enExtendMatchCondCmd << TMR4_OCER_MCECL_POS));
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set the Timer4 OCO low channel mode
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] enNewSta            The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_OutputCompareCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta)
{
    __IO uint16_t *TMR4_OCSR;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    /* Get pointer of current channel OCO register address */
    TMR4_OCSR = TMR4_OCSRx(M0P_TMR4, u32Ch);

    /* Set OCO valid */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        MODIFY_REG16(*TMR4_OCSR, TMR4_OCSR_OCEH, ((uint16_t)enNewSta << TMR4_OCSR_OCEH_POS));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        MODIFY_REG16(*TMR4_OCSR, TMR4_OCSR_OCEL, ((uint16_t)enNewSta << TMR4_OCSR_OCEL_POS));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Set the Timer4 OCO interrupt function
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] enNewSta            The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_IntCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta)
{
    __IO uint16_t *TMR4_OCSR;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    /* Get pointer of current channel OCO register address */
    TMR4_OCSR = TMR4_OCSRx(M0P_TMR4, u32Ch);

    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        MODIFY_REG16(*TMR4_OCSR, TMR4_OCSR_OCIEH, ((uint16_t)enNewSta << TMR4_OCSR_OCIEH_POS));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        MODIFY_REG16(*TMR4_OCSR, TMR4_OCSR_OCIEL, ((uint16_t)enNewSta << TMR4_OCSR_OCIEL_POS));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Set the Timer4 OCO interrupt flag
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @retval An en_flag_status_t enumeration value:
 *           - Reset:               None interrupt request flag is set on Timer4 OCO
 *           - Set:                 Detection interrupt request on Timer4 OCO
 */
en_flag_status_t TIMER4_OCO_GetFlag(uint32_t u32Ch)
{
    uint16_t u16Flag = 0u;
    __IO uint16_t *TMR4_OCSR;

    /* Get pointer of current channel OCO register address */
    TMR4_OCSR = TMR4_OCSRx(M0P_TMR4, u32Ch);

    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        u16Flag = READ_REG16_BIT(*TMR4_OCSR, TMR4_OCSR_OCFH);
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        u16Flag = READ_REG16_BIT(*TMR4_OCSR, TMR4_OCSR_OCFL);
    }
    else
    {
        /* Do nothing */
    }

    return (u16Flag ? Set : Reset);
}

/**
 * @brief  Clear the Timer4 OCO interrupt function
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @retval An en_result_t enumeration value:
 *           - Ok: Clear successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_ClearFlag(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_OCSR;
    en_result_t enRet = Ok;

    /* Get pointer of current channel OCO register address */
    TMR4_OCSR = TMR4_OCSRx(M0P_TMR4, u32Ch);

    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        CLEAR_REG16_BIT(*TMR4_OCSR, TMR4_OCSR_OCFH);
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        CLEAR_REG16_BIT(*TMR4_OCSR, TMR4_OCSR_OCFL);
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 OCO invalid output polarity
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] u16OutputPolarity   Timer4 OCO invalid output polarity.
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_INVAILD_OP_LOW:        TIM4_<t>_OxH/TIM4_<t>_OxL output low level when OCO is invalid
 *           @arg TIMER4_OCO_INVAILD_OP_HIGH:       TIM4_<t>_OxH/TIM4_<t>_OxL output high level when OCO is invalid
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_OCO_SetOcoInvalidOp(uint32_t u32Ch,
                                uint16_t u16OutputPolarity)
{
    __IO uint16_t *TMR4_OCSR;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4_OCO_PORT_INVALID_OP(u16OutputPolarity));

    /* Get pointer of current channel OCO register address */
    TMR4_OCSR = TMR4_OCSRx(M0P_TMR4, u32Ch);

    /* Set OCCR buffer mode */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        MODIFY_REG16(*TMR4_OCSR, TMR4_OCSR_OCPH, (uint16_t)(u16OutputPolarity << TMR4_OCSR_OCPH_POS));
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        MODIFY_REG16(*TMR4_OCSR, TMR4_OCSR_OCPL, (uint16_t)(u16OutputPolarity << TMR4_OCSR_OCPL_POS));
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  Get Timer4 OCO output polarity
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_OCO_INVAILD_OP_LOW:        TIM4_<t>_OxH/TIM4_<t>_OxL output low level when OCO is invalid
 *           @arg TIMER4_OCO_INVAILD_OP_HIGH:       TIM4_<t>_OxH/TIM4_<t>_OxL output high level when OCO is invalid
 *           @arg TIMER4_RESULT_ERROR:              Timer4 OCO channel is invalid
 */
uint16_t TIMER4_OCO_GetOutputPolarity(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_OCSR;
    uint16_t u16OutputPolarity;

    /* Get pointer of current channel OCO register address */
    TMR4_OCSR = TMR4_OCSRx(M0P_TMR4, u32Ch);

    /* Get OCCR buffer mode */
    if (IS_VALID_TIMER4_OCO_HIGH_CH(u32Ch))     /* channel: TIMER4_OCO_UH, TIMER4_OCO_VH, TIMER4_OCO_WH */
    {
        u16OutputPolarity = (READ_REG16_BIT(*TMR4_OCSR, TMR4_OCSR_OCPH) >> TMR4_OCSR_OCPH_POS);
    }
    else if (IS_VALID_TIMER4_OCO_LOW_CH(u32Ch)) /* channel: TIMER4_OCO_UL, TIMER4_OCO_VL, TIMER4_OCO_WL */
    {
        u16OutputPolarity = (READ_REG16_BIT(*TMR4_OCSR, TMR4_OCSR_OCPL) >> TMR4_OCSR_OCPL_POS);
    }
    else
    {
        u16OutputPolarity = TIMER4_RESULT_ERROR;
    }

    return u16OutputPolarity;
}

/**
 * @brief  Set Timer4 OCO OCCR register value
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @param  [in] u16OccrVal          The Timer4 OCO OCCR register value
 *           @arg number of 16bit
 * @retval None
 */
void TIMER4_OCO_SetOccrVal(uint32_t u32Ch, uint16_t u16OccrVal)
{
    __IO uint16_t *TMR4_OCCR;

    DDL_ASSERT(IS_VALID_TIMER4_OCO_CH(u32Ch));

    /* Get pointer of current channel OCO register address */
    TMR4_OCCR = TMR4_OCCRx(M0P_TMR4, u32Ch);

    WRITE_REG16(*TMR4_OCCR, u16OccrVal);
}

/**
 * @brief  Get Timer4 OCO OCCR register value
 * @param  [in] u32Ch               Timer4 OCO channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_OCO_UH:    Timer4 OCO channel:UH
 *           @arg TIMER4_OCO_UL:    Timer4 OCO channel:UL
 *           @arg TIMER4_OCO_VH:    Timer4 OCO channel:VH
 *           @arg TIMER4_OCO_VL:    Timer4 OCO channel:VL
 *           @arg TIMER4_OCO_WH:    Timer4 OCO channel:WH
 *           @arg TIMER4_OCO_WL:    Timer4 OCO channel:WL
 * @retval The OCCR register value of the Timer4 OCO
 */
uint16_t TIMER4_OCO_GetOccrVal(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_OCCR;

    DDL_ASSERT(IS_VALID_TIMER4_OCO_CH(u32Ch));

    /* Get pointer of current channel OCO register address */
    TMR4_OCCR = TMR4_OCCRx(M0P_TMR4, u32Ch);

    return READ_REG16(*TMR4_OCCR);
}

/**
 * @}
 */

/**
 * @defgroup TIMER4_PWM_Global_Functions TIMER4 PWM Global Functions
 * @{
 */

/**
 * @brief  Initialize Timer4 PWM
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @param  [in] pstcInit            Pointer to a @ref stc_timer4_pwm_init_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: u32Ch is invalid or pstcInit = NULL
 */
en_result_t TIMER4_PWM_Init(uint32_t u32Ch,
                                const stc_timer4_pwm_init_t *pstcInit)
{
    __IO uint16_t *TMR4_POCR;
    __IO uint16_t *TMR4_RCSR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check channel && structure pointer */
    if ((IS_VALID_TIMER4_PWM_CH(u32Ch)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_TIMER4_PWM_MODE(pstcInit->u16Mode));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enRtIntMaskCmd));
        DDL_ASSERT(IS_VALID_TIMER4_PWM_CLK_DIV(pstcInit->u16ClkDiv));
        DDL_ASSERT(IS_VALID_TIMER4_PWM_OUTPUT_POLARITY(pstcInit->u16PwmOutputPolarity));

        /* Get pointer of current channel PWM register address */
        TMR4_POCR = TMR4_POCRx(M0P_TMR4, u32Ch);
        TMR4_RCSR = TMR4_RCSRx(M0P_TMR4);

        /* Set POCR register */
        WRITE_REG16(*TMR4_POCR, (pstcInit->u16ClkDiv | pstcInit->u16Mode | pstcInit->u16PwmOutputPolarity));

        /* Set RCSR register */
        MODIFY_REG16(*TMR4_RCSR,
                      (TMR4_RCSR_RTIDx(u32Ch) | TMR4_RCSR_RTEx(u32Ch) | TMR4_RCSR_RTSx(u32Ch)), \
                      ((uint16_t)((uint16_t)pstcInit->enRtIntMaskCmd << u32Ch) | TMR4_RCSR_RTSx(u32Ch)));
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Initialize Timer4 PWM
 * @brief  Set the fields of structure stc_timer4_pwm_init_t to default values
 * @param  [out] pstcInit               Pointer to a @ref stc_timer4_pwm_init_t structure (TMRBx unit PWM function configuration data structure)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t TIMER4_PWM_StructInit(stc_timer4_pwm_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (NULL != pstcInit)
    {
        pstcInit->enRtIntMaskCmd = Disable;
        pstcInit->u16ClkDiv = TIMER4_PWM_CLK_DIV1;
        pstcInit->u16Mode = TIMER4_PWM_THROUGH_MODE;
        pstcInit->u16PwmOutputPolarity = TIMER4_PWM_OP_OXH_HOLD_OXL_HOLD;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-initialize Timer4 PWM
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_DeInit(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_POCR;
    __IO uint16_t *TMR4_RCSR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check channel */
    if (IS_VALID_TIMER4_PWM_CH(u32Ch))
    {
        /* Get pointer of current channel PWM register address */
        TMR4_POCR = TMR4_POCRx(M0P_TMR4, u32Ch);
        TMR4_RCSR = TMR4_RCSRx(M0P_TMR4);

        /* Set POCR register */
        WRITE_REG16(*TMR4_POCR, (TIMER4_PWM_CLK_DIV1 | TIMER4_PWM_THROUGH_MODE | TIMER4_PWM_OP_OXH_HOLD_OXL_HOLD));

        /* Set RCSR register */
        MODIFY_REG16(*TMR4_RCSR,
                      (TMR4_RCSR_RTIDx(u32Ch) | TMR4_RCSR_RTEx(u32Ch) | TMR4_RCSR_RTSx(u32Ch)), \
                      TMR4_RCSR_RTSx(u32Ch));
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 PWM output polarity
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @param  [in] u16OutputPolarity   Timer4 PWM output polarity
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_OP_OXH_HOLD_OXL_HOLD:      Output PWML and PWMH signals without changing the level
 *           @arg TIMER4_PWM_OP_OXH_INVERT_OXL_INVERT:  Output both PWML and PWMH signals reversed
 *           @arg TIMER4_PWM_OP_OXH_INVERT_OXL_HOLD:    Output the PWMH signal reversed, outputs the PWML signal without changing the level
 *           @arg TIMER4_PWM_OP_OXH_HOLD_OXL_INVERT:    Output the PWMH signal without changing the level, Outputs the PWML signal reversed
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_SetOutputPolarity(uint32_t u32Ch,
                                                uint16_t u16OutputPolarity)
{
    __IO uint16_t *TMR4_POCR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check channel */
    if (IS_VALID_TIMER4_PWM_CH(u32Ch))
    {
        DDL_ASSERT(IS_VALID_TIMER4_PWM_OUTPUT_POLARITY(u16OutputPolarity));

        /* Get pointer of current channel PWM register address */
        TMR4_POCR = TMR4_POCRx(M0P_TMR4, u32Ch);

        MODIFY_REG16(*TMR4_POCR, TMR4_POCR_LVLS, u16OutputPolarity);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get Timer4 PWM output polarity
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_PWM_OP_OXH_HOLD_OXL_HOLD:      Output PWML and PWMH signals without changing the level
 *           @arg TIMER4_PWM_OP_OXH_INVERT_OXL_INVERT:  Output both PWML and PWMH signals reversed
 *           @arg TIMER4_PWM_OP_OXH_INVERT_OXL_HOLD:    Output the PWMH signal reversed, outputs the PWML signal without changing the level
 *           @arg TIMER4_PWM_OP_OXH_HOLD_OXL_INVERT:    Output the PWMH signal without changing the level, Outputs the PWML signal reversed
 */
uint16_t TIMER4_PWM_GetOutputPolarity(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_POCR;

    /* Check PWM channel */
    DDL_ASSERT(IS_VALID_TIMER4_PWM_CH(u32Ch));

    /* Get pointer of current channel PWM register address */
    TMR4_POCR = TMR4_POCRx(M0P_TMR4, u32Ch);

    return READ_REG16_BIT(*TMR4_POCR, TMR4_POCR_LVLS);
}

/**
 * @brief  Start Timer4 PWM reload-timer
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @retval An en_result_t enumeration value:
 *           - Ok: Start successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_StartReloadTimer(uint32_t u32Ch)
{
    en_result_t enRet = Ok;

    switch (u32Ch)
    {
        case TIMER4_PWM_U:
            bM0P_TMR4->RCSR_b.RTEU = 1UL;
            break;
        case TIMER4_PWM_V:
            bM0P_TMR4->RCSR_b.RTEV = 1UL;
            break;
        case TIMER4_PWM_W:
            bM0P_TMR4->RCSR_b.RTEW = 1UL;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 * @brief  Stop Timer4 PWM reload-timer
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @retval An en_result_t enumeration value:
 *           - Ok: Stop successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_StopReloadTimer(uint32_t u32Ch)
{
    en_result_t enRet = Ok;

    switch (u32Ch)
    {
        case TIMER4_PWM_U:
            bM0P_TMR4->RCSR_b.RTSU = 1UL;
            break;
        case TIMER4_PWM_V:
            bM0P_TMR4->RCSR_b.RTSV = 1UL;
            break;
        case TIMER4_PWM_W:
            bM0P_TMR4->RCSR_b.RTSW = 1UL;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 * @brief  Set the Timer4 PWM interrupt function
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @param  [in] enNewSta            The function new state
 *           @arg  This parameter can be: Enable or Disable
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_IntCmd(uint32_t u32Ch,
                                en_functional_state_t enNewSta)
{
    en_result_t enRet = Ok;

    switch (u32Ch)
    {
        case TIMER4_PWM_U:
            bM0P_TMR4->RCSR_b.RTIDU = enNewSta;
            break;
        case TIMER4_PWM_V:
            bM0P_TMR4->RCSR_b.RTIDV = enNewSta;
            break;
        case TIMER4_PWM_W:
            bM0P_TMR4->RCSR_b.RTIDW = enNewSta;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 * @brief  Get PWM reload-timer interrupt flag
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @retval An en_flag_status_t enumeration value:
 *           - Reset                None interrupt request on PWM reload-timer
 *           - Set                  Detection interrupt request on PWM reload-timer
 */
en_flag_status_t TIMER4_PWM_GetFlag(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_RCSR;

    /* Check channel */
    DDL_ASSERT(IS_VALID_TIMER4_PWM_CH(u32Ch));

    /* Get pointer of current channel PWM register address */
    TMR4_RCSR = TMR4_RCSRx(M0P_TMR4);

    return (READ_REG16_BIT(*TMR4_RCSR, TMR4_RCSR_RTIFx(u32Ch)) ? Set : Reset);
}

/**
 * @brief  Clear PWM reload-timer interrupt flag
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @retval An en_result_t enumeration value:
 *           - Ok: Clear successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_ClearFlag(uint32_t u32Ch)
{
    en_result_t enRet = Ok;

    switch (u32Ch)
    {
        case TIMER4_PWM_U:
            bM0P_TMR4->RCSR_b.RTICU = 1UL;
            break;
        case TIMER4_PWM_V:
            bM0P_TMR4->RCSR_b.RTICV = 1UL;
            break;
        case TIMER4_PWM_W:
            bM0P_TMR4->RCSR_b.RTICW = 1UL;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 PWM dead region count
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @param  [in] u16PDAR             PDAR value
 *           @arg number of 16bit
 * @param  [in] u16PDBR             PDBR value
 *           @arg number of 16bit
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_SetDeadRegionValue(uint32_t u32Ch,
                                                uint16_t u16PDAR,
                                                uint16_t u16PDBR)
{
    __IO uint16_t *TMR4_PDAR;
    __IO uint16_t *TMR4_PDBR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (IS_VALID_TIMER4_PWM_CH(u32Ch))
    {
        /* Get pointer of current channel PWM register address */
        TMR4_PDAR = TMR4_PDARx(M0P_TMR4, u32Ch);
        TMR4_PDBR = TMR4_PDBRx(M0P_TMR4, u32Ch);

        WRITE_REG16(*TMR4_PDAR, u16PDAR);
        WRITE_REG16(*TMR4_PDBR, u16PDBR);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get Timer4 PWM dead region count
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @param  [out] pu16PDAR           Pointer of 16bit data
 *           @arg pointer of 16bit
 * @param  [out] pu16PDBR           Pointer of 16bit data
 *           @arg pointer of 16bit
 * @retval An en_result_t enumeration value:
 *           - Ok: Get successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_GetDeadRegionValue(uint32_t u32Ch,
                                                uint16_t *pu16PDAR,
                                                uint16_t *pu16PDBR)
{
    __IO uint16_t *TMR4_PDAR;
    __IO uint16_t *TMR4_PDBR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (IS_VALID_TIMER4_PWM_CH(u32Ch))
    {
        /* Get pointer of current channel PWM register address */
        TMR4_PDAR = TMR4_PDARx(M0P_TMR4, u32Ch);
        TMR4_PDBR = TMR4_PDBRx(M0P_TMR4, u32Ch);

        *pu16PDAR = READ_REG16(*TMR4_PDAR);
        *pu16PDBR = READ_REG16(*TMR4_PDBR);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 PWM filter count value
 * @param  [in] u32Ch               Timer4 PWM channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_PWM_U:     Timer4 PWM couple channel: U
 *           @arg TIMER4_PWM_V:     Timer4 PWM couple channel: V
 *           @arg TIMER4_PWM_W:     Timer4 PWM couple channel: W
 * @param  [in] u16Count            Timer4 PWM filter count value
 *           @arg number of 16bit
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_PWM_SetFilterCountValue(uint32_t u32Ch,
                                                        uint16_t u16Count)
{
    __IO uint16_t *TMR4_PFSR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (IS_VALID_TIMER4_PWM_CH(u32Ch))
    {
        /* Get pointer of current channel PWM register address */
        TMR4_PFSR = TMR4_PFSRx(M0P_TMR4, u32Ch);

        WRITE_REG16(*TMR4_PFSR, u16Count);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @}
 */

/**
 * @defgroup TIMER4_SEVT_Global_Functions TIMER4 SEVT Global Functions
 * @{
 */

/**
 * @brief  Initialize Timer4 SEVT
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @param  [in] pstcInit            Pointer to a @ref stc_timer4_sevt_init_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: u32Ch is invalid or pstcInit = NULL
 */
en_result_t TIMER4_SEVT_Init(uint32_t u32Ch,
                                const stc_timer4_sevt_init_t *pstcInit)
{
    __IO uint16_t *TMR4_SCCR;
    __IO uint16_t *TMR4_SCSR;
    __IO uint16_t *TMR4_SCMR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer && channel */
    if ((IS_VALID_TIMER4_SEVT_CH(u32Ch)) && (NULL != pstcInit))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_TIMER4_SEVT_MODE(pstcInit->u16Mode));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enLinkTransfer));
        DDL_ASSERT(IS_VALID_TIMER4_SEVT_TRIG_EVT(pstcInit->u16TrigEvt));
        DDL_ASSERT(IS_VALID_TIMER4_SEVT_BUF_MODE(pstcInit->u16BufMode));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->stcTriggerCond.enDownMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->stcTriggerCond.enPeakMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->stcTriggerCond.enUpMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->stcTriggerCond.enZeroMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->stcTriggerCond.enCmpAmcZicCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->stcTriggerCond.enCmpAmcPicCmd));
        DDL_ASSERT(IS_VALID_TIMER4_SEVT_MASK(pstcInit->stcTriggerCond.u16MaskTimes));

        /* Get actual address of register list of current channel */
        TMR4_SCCR = TMR4_SCCRx(M0P_TMR4, u32Ch);
        TMR4_SCSR = TMR4_SCSRx(M0P_TMR4, u32Ch);
        TMR4_SCMR = TMR4_SCMRx(M0P_TMR4, u32Ch);

        /* Configure default parameter */
        WRITE_REG16(*TMR4_SCSR, 0x0000u);
        WRITE_REG16(*TMR4_SCMR, 0xFF00u);

        /* Set SCSR register */
        WRITE_REG16(*TMR4_SCSR,             \
                    (pstcInit->u16Mode    | \
                     pstcInit->u16TrigEvt | \
                     pstcInit->u16BufMode | \
                     (uint16_t)((uint16_t)pstcInit->enLinkTransfer << TMR4_SCSR_LMC_POS)                | \
                     (uint16_t)((uint16_t)pstcInit->stcTriggerCond.enDownMatchCmd << TMR4_SCSR_DEN_POS) | \
                     (uint16_t)((uint16_t)pstcInit->stcTriggerCond.enPeakMatchCmd << TMR4_SCSR_PEN_POS) | \
                     (uint16_t)((uint16_t)pstcInit->stcTriggerCond.enUpMatchCmd << TMR4_SCSR_UEN_POS)   | \
                     (uint16_t)((uint16_t)pstcInit->stcTriggerCond.enZeroMatchCmd << TMR4_SCSR_ZEN_POS)));

        /* Set SCMR register */
        WRITE_REG16(*TMR4_SCMR, \
                    (pstcInit->stcTriggerCond.u16MaskTimes | \
                     (uint16_t)((uint16_t)pstcInit->stcTriggerCond.enCmpAmcZicCmd << TMR4_SCMR_MZCE_POS) | \
                     (uint16_t)((uint16_t)pstcInit->stcTriggerCond.enCmpAmcPicCmd << TMR4_SCMR_MPCE_POS)));

        WRITE_REG16(*TMR4_SCCR, pstcInit->u16SccrVal);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Initialize Timer4 PWM
 * @brief  Set the fields of structure stc_timer4_pwm_init_t to default values
 * @param  [out] pstcInit               Pointer to a @ref stc_timer4_pwm_init_t structure (TMRBx unit PWM function configuration data structure)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully
 *           - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t TIMER4_SEVT_StructInit(stc_timer4_sevt_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer */
    if (NULL != pstcInit)
    {
        pstcInit->u16SccrVal = 0u;
        pstcInit->u16Mode = TIMER4_SEVT_MODE_COMPARE_TRIGGER;
        pstcInit->u16TrigEvt = TIMER4_SEVT_TRIGGER_TMR4_SCMUH;
        pstcInit->u16BufMode = TIMER4_SEVT_BUF_DISABLE;
        pstcInit->enLinkTransfer = Disable;
        pstcInit->stcTriggerCond.enDownMatchCmd = Disable;
        pstcInit->stcTriggerCond.enPeakMatchCmd = Disable;
        pstcInit->stcTriggerCond.enUpMatchCmd = Disable;
        pstcInit->stcTriggerCond.enZeroMatchCmd = Disable;
        pstcInit->stcTriggerCond.enCmpAmcZicCmd = Disable;
        pstcInit->stcTriggerCond.enCmpAmcPicCmd = Disable;
        pstcInit->stcTriggerCond.u16MaskTimes = TIMER4_SEVT_MASK_0;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-initialize Timer4 PWM
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @retval An en_result_t enumeration value:
 *           - Ok: De-Initialize successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_SEVT_DeInit(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_SCCR;
    __IO uint16_t *TMR4_SCSR;
    __IO uint16_t *TMR4_SCMR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check channel */
    if (IS_VALID_TIMER4_SEVT_CH(u32Ch))
    {
        /* Get actual address of register list of current channel */
        TMR4_SCCR = TMR4_SCCRx(M0P_TMR4, u32Ch);
        TMR4_SCSR = TMR4_SCSRx(M0P_TMR4, u32Ch);
        TMR4_SCMR = TMR4_SCMRx(M0P_TMR4, u32Ch);

        /* Configure default parameter */
        WRITE_REG16(*TMR4_SCCR, 0x0u);
        WRITE_REG16(*TMR4_SCSR, 0x0000u);
        WRITE_REG16(*TMR4_SCMR, 0xFF00u);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 SEVT trigger event
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @param  [in] u16Event            Timer4 SEVT trigger event
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_TRIGGER_TMR4_SCMUH    Timer4 Special-EVT trigger event: TMR4_SCMUH
 *           @arg TIMER4_SEVT_TRIGGER_TMR4_SCMUL    Timer4 Special-EVT trigger event: TMR4_SCMUL
 *           @arg TIMER4_SEVT_TRIGGER_TMR4_SCMVH    Timer4 Special-EVT trigger event: TMR4_SCMVH
 *           @arg TIMER4_SEVT_TRIGGER_TMR4_SCMVL    Timer4 Special-EVT trigger event: TMR4_SCMVL
 *           @arg TIMER4_SEVT_TRIGGER_TMR4_SCMWH    Timer4 Special-EVT trigger event: TMR4_SCMWH
 *           @arg TIMER4_SEVT_TRIGGER_TMR4_SCMWL    Timer4 Special-EVT trigger event: TMR4_SCMWL
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid or pstcInit = NULL
 */
en_result_t TIMER4_SEVT_SetTriggerEvent(uint32_t u32Ch,
                                                    uint16_t u16Event)
{
    __IO uint16_t *TMR4_SCSR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer && channel */
    if (IS_VALID_TIMER4_SEVT_CH(u32Ch))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_TIMER4_SEVT_TRIG_EVT(u16Event));

        /* Get actual address of register list of current channel */
        TMR4_SCSR = TMR4_SCSRx(M0P_TMR4, u32Ch);

        /* Set SCSR register */
        MODIFY_REG16(*TMR4_SCSR, TMR4_SCSR_EVTOS, u16Event);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 SEVT trigger condition
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @param  [in] pstcTriggerCond     Pointer to a @ref stc_timer4_sevt_trigger_cond_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid or pstcTriggerCond = NULL
 */
en_result_t TIMER4_SEVT_SetTriggerCondition(uint32_t u32Ch,
                    const stc_timer4_sevt_trigger_cond_t *pstcTriggerCond)
{
    __IO uint16_t *TMR4_SCSR;
    __IO uint16_t *TMR4_SCMR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer && channel */
    if ((IS_VALID_TIMER4_SEVT_CH(u32Ch)) && (NULL != pstcTriggerCond))
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTriggerCond->enDownMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTriggerCond->enPeakMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTriggerCond->enUpMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTriggerCond->enZeroMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTriggerCond->enCmpAmcZicCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTriggerCond->enCmpAmcPicCmd));
        DDL_ASSERT(IS_VALID_TIMER4_SEVT_MASK(pstcTriggerCond->u16MaskTimes));

        /* Get actual address of register list of current channel */
        TMR4_SCSR = TMR4_SCSRx(M0P_TMR4, u32Ch);
        TMR4_SCMR = TMR4_SCMRx(M0P_TMR4, u32Ch);

        /* Set SCSR register */
        MODIFY_REG16(*TMR4_SCSR,                      \
                     (TMR4_SCSR_DEN | TMR4_SCSR_PEN | \
                      TMR4_SCSR_UEN | TMR4_SCSR_ZEN), \
                     ((uint16_t)((uint16_t)pstcTriggerCond->enDownMatchCmd << TMR4_SCSR_DEN_POS) | \
                      (uint16_t)((uint16_t)pstcTriggerCond->enPeakMatchCmd << TMR4_SCSR_PEN_POS) | \
                      (uint16_t)((uint16_t)pstcTriggerCond->enUpMatchCmd << TMR4_SCSR_UEN_POS)   | \
                      (uint16_t)((uint16_t)pstcTriggerCond->enZeroMatchCmd << TMR4_SCSR_ZEN_POS)));

        /* Set SCMR register */
        WRITE_REG16(*TMR4_SCMR,                      \
                    (pstcTriggerCond->u16MaskTimes | \
                     (uint16_t)((uint16_t)pstcTriggerCond->enCmpAmcZicCmd << TMR4_SCMR_MZCE_POS) | \
                     (uint16_t)((uint16_t)pstcTriggerCond->enCmpAmcPicCmd << TMR4_SCMR_MPCE_POS)));
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set Timer4 SEVT delay object
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @param  [in] u16DelayObject      Timer4 SEVT delay object
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_DELAY_OCCRXH  TIMER4 SEVT delay object: OCCRxh
 *           @arg TIMER4_SEVT_DELAY_OCCRXL  TIMER4 SEVT delay object: OCCRxl
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_SEVT_SetDelayObject(uint32_t u32Ch, uint16_t u16DelayObject)
{
    __IO uint16_t *TMR4_SCSR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer && channel */
    if (IS_VALID_TIMER4_SEVT_CH(u32Ch))
    {
        /* Get actual address of register list of current channel */
        TMR4_SCSR = TMR4_SCSRx(M0P_TMR4, u32Ch);

        /* Set SCSR register */
        MODIFY_REG16(*TMR4_SCSR, TMR4_SCSR_EVTDS, u16DelayObject);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get Timer4 SEVT SCCR register value
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_SEVT_DELAY_OCCRXH  TIMER4 SEVT delay object: OCCRxh
 *           @arg TIMER4_SEVT_DELAY_OCCRXL  TIMER4 SEVT delay object: OCCRxl
 */
uint16_t TIMER4_SEVT_GetDelayObject(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_SCSR;

    /* Check channel */
    DDL_ASSERT(IS_VALID_TIMER4_SEVT_CH(u32Ch));

    /* Get actual address of register list of current channel */
    TMR4_SCSR = TMR4_SCSRx(M0P_TMR4, u32Ch);

    return READ_REG16_BIT(*TMR4_SCSR, TMR4_SCSR_EVTDS);
}

/**
 * @brief  Set Timer4 SEVT SCCR register value
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @param  [in] u16SccrVal          SCCR register value
 *           @arg number of 16bit
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_SEVT_SetSccrVal(uint32_t u32Ch,
                                            uint16_t u16SccrVal)
{
    __IO uint16_t *TMR4_SCCR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check structure pointer && channel */
    if (IS_VALID_TIMER4_SEVT_CH(u32Ch))
    {
        /* Get actual address of register list of current channel */
        TMR4_SCCR = TMR4_SCCRx(M0P_TMR4, u32Ch);

        /* Set SCCR register */
        WRITE_REG16(*TMR4_SCCR, u16SccrVal);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get Timer4 SEVT SCCR register value
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @retval SCCR register value
 */
uint16_t TIMER4_SEVT_GetSccrVal(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_SCCR;

    /* Check channel */
    DDL_ASSERT(IS_VALID_TIMER4_SEVT_CH(u32Ch));

    /* Get actual address of register list of current channel */
    TMR4_SCCR = TMR4_SCCRx(M0P_TMR4, u32Ch);

    return READ_REG16(*TMR4_SCCR);
}

/**
 * @brief  Set Timer4 SEVT trigger event.
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @param  [in] u16MaskTimes        Mask times
 *           @arg TIMER4_SEVT_MASK_0:   Mask 0 times
 *           @arg TIMER4_SEVT_MASK_1:   Mask 1 times
 *           @arg TIMER4_SEVT_MASK_2:   Mask 2 times
 *           @arg TIMER4_SEVT_MASK_3:   Mask 3 times
 *           @arg TIMER4_SEVT_MASK_4:   Mask 4 times
 *           @arg TIMER4_SEVT_MASK_5:   Mask 5 times
 *           @arg TIMER4_SEVT_MASK_6:   Mask 6 times
 *           @arg TIMER4_SEVT_MASK_7:   Mask 7 times
 *           @arg TIMER4_SEVT_MASK_8:   Mask 8 times
 *           @arg TIMER4_SEVT_MASK_9:   Mask 9 times
 *           @arg TIMER4_SEVT_MASK_10:  Mask 10 times
 *           @arg TIMER4_SEVT_MASK_11:  Mask 11 times
 *           @arg TIMER4_SEVT_MASK_12:  Mask 12 times
 *           @arg TIMER4_SEVT_MASK_13:  Mask 13 times
 *           @arg TIMER4_SEVT_MASK_14:  Mask 14 times
 *           @arg TIMER4_SEVT_MASK_15:  Mask 15 times
 * @retval An en_result_t enumeration value:
 *           - Ok: Set successfully
 *           - ErrorInvalidParameter: u32Ch is invalid
 */
en_result_t TIMER4_SEVT_SetMaskTimes(uint32_t u32Ch,
                                            uint16_t u16MaskTimes)
{
    __IO uint16_t *TMR4_SCMR;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check channel */
    if (IS_VALID_TIMER4_SEVT_CH(u32Ch))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_TIMER4_SEVT_MASK(u16MaskTimes));
        /* Get actual address of register list of current channel */
        TMR4_SCMR = TMR4_SCMRx(M0P_TMR4, u32Ch);

        /* Set SCMR register */
        MODIFY_REG16(*TMR4_SCMR, TMR4_SCMR_AMC, u16MaskTimes);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Get Timer4 SEVT SCCR register value
 * @param  [in] u32Ch               Timer4 SEVT channel
 *         This parameter can be one of the following values:
 *           @arg TIMER4_SEVT_UH    Timer4 SEVT channel:UH
 *           @arg TIMER4_SEVT_UL    Timer4 SEVT channel:UL
 *           @arg TIMER4_SEVT_VH    Timer4 SEVT channel:VH
 *           @arg TIMER4_SEVT_VL    Timer4 SEVT channel:VL
 *           @arg TIMER4_SEVT_WH    Timer4 SEVT channel:WH
 *           @arg TIMER4_SEVT_WL    Timer4 SEVT channel:WL
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_SEVT_MASK_0:   Mask 0 times
 *           @arg TIMER4_SEVT_MASK_1:   Mask 1 times
 *           @arg TIMER4_SEVT_MASK_2:   Mask 2 times
 *           @arg TIMER4_SEVT_MASK_3:   Mask 3 times
 *           @arg TIMER4_SEVT_MASK_4:   Mask 4 times
 *           @arg TIMER4_SEVT_MASK_5:   Mask 5 times
 *           @arg TIMER4_SEVT_MASK_6:   Mask 6 times
 *           @arg TIMER4_SEVT_MASK_7:   Mask 7 times
 *           @arg TIMER4_SEVT_MASK_8:   Mask 8 times
 *           @arg TIMER4_SEVT_MASK_9:   Mask 9 times
 *           @arg TIMER4_SEVT_MASK_10:  Mask 10 times
 *           @arg TIMER4_SEVT_MASK_11:  Mask 11 times
 *           @arg TIMER4_SEVT_MASK_12:  Mask 12 times
 *           @arg TIMER4_SEVT_MASK_13:  Mask 13 times
 *           @arg TIMER4_SEVT_MASK_14:  Mask 14 times
 *           @arg TIMER4_SEVT_MASK_15:  Mask 15 times
 */
uint16_t TIMER4_SEVT_GetMaskTimes(uint32_t u32Ch)
{
    __IO uint16_t *TMR4_SCMR;

    /* Check channel */
    DDL_ASSERT(IS_VALID_TIMER4_SEVT_CH(u32Ch));

    /* Get actual address of register list of current channel */
    TMR4_SCMR = TMR4_SCMRx(M0P_TMR4, u32Ch);

    return READ_REG16_BIT(*TMR4_SCMR, TMR4_SCMR_AMC);
}

/**
 * @}
 */

/**
 * @defgroup TIMER4_EMB_Global_Functions TIMER4 EMB Global Functions
 * @{
 */

/**
 * @brief  Set Timer4 EMB PWM port polarity when EMB signal occurs
 * @param  [in] u32PortPolarity         Timer4 EMB PWM port polarity
 *         This parameter can be one of the following values:
 *           @arg TIMER4_EMB_TRIG_PWM_OP_NORMAL PWM output signal normally when EMB signal occurs
 *           @arg TIMER4_EMB_TRIG_PWM_OP_HOLD   Hold PWM output when EMB signal occurs
 *           @arg TIMER4_EMB_TRIG_PWM_OP_HIZ    PWM output Hiz signal when EMB signal occurs
 *           @arg TIMER4_EMB_TRIG_PWM_OP_LOW    PWM output low level signal when EMB signal occurs
 *           @arg TIMER4_EMB_TRIG_PWM_OP_HIGH   PWM output high level signal when EMB signal occurs
 * @retval None
 */
void TIMER4_EMB_SetPwmPortPolarity(uint32_t u32PortPolarity)
{
    DDL_ASSERT(IS_VALID_TIMER4_EMB_PWM_OP(u32PortPolarity));

    WRITE_REG32(*TMR4_ESCRx(M0P_TMR4), u32PortPolarity);
}

/**
 * @brief  Get PWM port polarity when EMB signal occurs
 * @param  None
 * @retval Returned value can be one of the following values:
 *           @arg TIMER4_EMB_TRIG_PWM_OP_NORMAL PWM output signal normally when EMB signal occurs
 *           @arg TIMER4_EMB_TRIG_PWM_OP_HOLD   Hold PWM output when EMB signal occurs
 *           @arg TIMER4_EMB_TRIG_PWM_OP_HIZ    PWM output Hiz signal when EMB signal occurs
 *           @arg TIMER4_EMB_TRIG_PWM_OP_LOW    PWM output low level signal when EMB signal occurs
 *           @arg TIMER4_EMB_TRIG_PWM_OP_HIGH   PWM output high level signal when EMB signal occurs
 */
uint32_t TIMER4_EMB_GetPwmPortPolarity(void)
{
    return READ_REG32(*TMR4_ESCRx(M0P_TMR4));
}

/**
 * @}
 */

/**
 * @}
 */

#endif /* DDL_TIMER4_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
