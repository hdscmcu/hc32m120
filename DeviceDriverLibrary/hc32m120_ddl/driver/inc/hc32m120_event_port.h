/**
 *******************************************************************************
 * @file  hc32m120_event_port.h
 * @brief This file contains all the functions prototypes of the EVENT_PORT driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-26       Zhangxl         First version
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
#ifndef __HC32M120_EVENT_PORT_H__
#define __HC32M120_EVENT_PORT_H__

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
 * @addtogroup DDL_EVENT_PORT
 * @{
 */

#if (DDL_EVENT_PORT_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup EVENT_PORT_Global_Types EVENT_PORT Global Types
 * @{
 */
/**
 * @brief  EVENT_PORT Init structure definition
 */
typedef struct
{
    uint32_t u32EPFE;         /*!< EVENT_PORT filter clock,                   \
                                @ref EVENT_PORT_FilterClock_Sel for details */
    uint32_t u32EPFClk;       /*!< EVENT_PORT filter clock division,          \
                                @ref EVENT_PORT_FilterClock_Div for details */
    uint32_t u32EPTrigger;     /*!< EVENT_PORT trigger edge select,            \
                                @ref EVENT_PORT_Trigger_Edge_Sel for details */
} stc_event_port_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EVENT_PORT_Global_Macros EVENT_PORT Global Macros
 * @{
 */

/**
 * @defgroup EVENT_PORT_Registers_Reset_Value EVENT_PORT Registers Reset Value
 * @{
 */
#define EVENT_PORT_CR_RESET_VALUE          ((uint32_t)0x00000000ul)
/**
 * @}
 */

/**
 * @defgroup EVENT_PORT_State_Sel EVENT_PORT pin state
 * @{
 */
#define EP1_STATE_SET             (uint8_t)(AOS_EVPRT_SR_PTSTS0_0)
#define EP2_STATE_SET             (uint8_t)(AOS_EVPRT_SR_PTSTS0_1)
#define EP3_STATE_SET             (uint8_t)(AOS_EVPRT_SR_PTSTS0_2)
#define EP4_STATE_SET             (uint8_t)(AOS_EVPRT_SR_PTSTS0_3)
#define EP1_STATE_RESET           (uint8_t)(0x00u)
#define EP2_STATE_RESET           (uint8_t)(0x00u)
#define EP3_STATE_RESET           (uint8_t)(0x00u)
#define EP4_STATE_RESET           (uint8_t)(0x00u)

#define EP_STATE_MASK             (uint8_t)(0x0Fu)
/**
 * @}
 */

/**
 * @defgroup EVENT_PORT_Trigger_Edge_Sel EVENT_PORT trigger edge
 * @{
 */
#define EP1_TRIGGER_FALLING       (uint16_t)(1ul << AOS_EVPRT_CR_EVP1FEE_POS)
#define EP2_TRIGGER_FALLING       (uint16_t)(1ul << AOS_EVPRT_CR_EVP2FEE_POS)
#define EP3_TRIGGER_FALLING       (uint16_t)(1ul << AOS_EVPRT_CR_EVP3FEE_POS)
#define EP4_TRIGGER_FALLING       (uint16_t)(1ul << AOS_EVPRT_CR_EVP4FEE_POS)
#define EP1_TRIGGER_RISING        (uint16_t)(1ul << AOS_EVPRT_CR_EVP1REE_POS)
#define EP2_TRIGGER_RISING        (uint16_t)(1ul << AOS_EVPRT_CR_EVP2REE_POS)
#define EP3_TRIGGER_RISING        (uint16_t)(1ul << AOS_EVPRT_CR_EVP3REE_POS)
#define EP4_TRIGGER_RISING        (uint16_t)(1ul << AOS_EVPRT_CR_EVP4REE_POS)
#define EVENT_PORT_TIGGER_MASK    (uint16_t)(0xFFFFu)
/**
 * @}
 */

/**
 * @defgroup EVENT_PORT_Record_Time_Sel EVENT_PORT record time
 * @{
 */
#define EP_STATE_CURRENT            (uint8_t)(0x00u)
#define EP_STATE_LASTTIME           (uint8_t)(0x04u)
#define EP_STATE_LAST2TIME          (uint8_t)(0x08u)
#define EP_STATE_LAST3TIME          (uint8_t)(0x0Cu)
/**
 * @}
 */

/**
 * @defgroup EVENT_PORT_FilterClock_Sel EVENT_PORT pin filter selection
 * @{
 */
#define EVENT_PORT_FILTER_OFF       (uint32_t)(0ul << AOS_EVPRT_CR_EVPNFE_POS)
#define EVENT_PORT_FILTER_ON        (uint32_t)(1ul << AOS_EVPRT_CR_EVPNFE_POS)
/**
 * @}
 */

/**
 * @defgroup EVENT_PORT_FilterClock_Div EVENT_PORT filter sampling clock division selection
 * @{
 */
#define EVENT_PORT_FCLK_HCLK_DIV1     (uint32_t)(0ul << AOS_EVPRT_CR_EVPDIVS_POS)
#define EVENT_PORT_FCLK_HCLK_DIV4     (uint32_t)(1ul << AOS_EVPRT_CR_EVPDIVS_POS)
#define EVENT_PORT_FCLK_HCLK_DIV16    (uint32_t)(2ul << AOS_EVPRT_CR_EVPDIVS_POS)
#define EVENT_PORT_FCLK_HCLK_DIV64    (uint32_t)(3ul << AOS_EVPRT_CR_EVPDIVS_POS)
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
 * @addtogroup EVENT_PORT_Global_Functions
 * @{
 */
/**
 * @brief  De-init EVENT_PORT register to default value
 * @param  None
 * @retval None
 */
__STATIC_INLINE void EVENT_PORT_DeInit(void)
{
    WRITE_REG(M0P_AOS->EVPRT_CR, EVENT_PORT_CR_RESET_VALUE);
}

en_result_t EVENT_PORT_Init(const stc_event_port_init_t *pstcEventPortInit);
en_result_t EVENT_PORT_StructInit(stc_event_port_init_t *pstcEventPortInit);
uint8_t EVENT_PORT_GetStatus(uint8_t u8RecordTimes);
void EVENT_PORT_TriggerEdgeCmd(uint8_t u8TriggerEdge, en_functional_state_t enNewState);

/**
 * @}
 */

#endif /* DDL_EVENT_PORT_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_EVENT_PORT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
