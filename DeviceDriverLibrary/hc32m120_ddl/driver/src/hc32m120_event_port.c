/**
 *******************************************************************************
 * @file  hc32m120_event_port.c
 * @brief This file provides firmware functions to manage the EVENT_PORT(EP).
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32m120_event_port.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_EVENT_PORT EVENT_PORT
 * @brief EVENT_PORT Driver Library
 * @{
 */

#if (DDL_EVENT_PORT_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EVENT_PORT_Local_Macros EVENT_PORT Local Macros
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup EVENT_PORT_Check_Parameters_Validity EVENT_PORT Check Parameters Validity
 * @{
 */
/*  Parameter validity check for EVENT_PORT trigger edge. */
#define IS_EVENT_PORT_TRIGGER(TRIGGER)  (((TRIGGER) & EVENT_PORT_TIGGER_MASK) != (uint16_t)0x0000u)

/*  Parameter validity check for EVENT_PORT filter function. */
#define IS_EVENT_PORT_FE(fe)                                                    \
(   ((fe) == EVENT_PORT_FILTER_OFF)             ||                              \
    ((fe) == EVENT_PORT_FILTER_ON))

/*  Parameter validity check for EVENT_PORT filter clock division. */
#define IS_EVENT_PORT_FCLK(fclk)                                                \
(   ((fclk) == EVENT_PORT_FCLK_HCLK_DIV1)       ||                              \
    ((fclk) == EVENT_PORT_FCLK_HCLK_DIV4)       ||                              \
    ((fclk) == EVENT_PORT_FCLK_HCLK_DIV16)      ||                              \
    ((fclk) == EVENT_PORT_FCLK_HCLK_DIV64))

/*  Parameter validity check for EVENT_PORT state record times. */
#define IS_EVENT_PORT_RECORD(record)                                            \
(   ((record) == EP_STATE_CURRENT)              ||                              \
    ((record) == EP_STATE_LASTTIME)             ||                              \
    ((record) == EP_STATE_LAST2TIME)            ||                              \
    ((record) == EP_STATE_LAST3TIME))

/*  Parameter validity check for EVENT_PORT state. */
#define IS_EVENT_PORT_STATE(state)      (((state) & EP_STATE_MASK) != (uint8_t)0x00u)

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
 * @defgroup EVENT_PORT_Global_Functions EVENT_PORT Global Functions
 * @{
 */

/**
 * @brief  Initialize EVENT_PORT.
 * @param  [in] pstcEventPortInit: Pointer to a stc_event_port_init_t structure
 *                                 that contains configuration information.
 * @retval Ok: EVENT_PORT initilize successful
 *         ErrorInvalidParameter: NULL pointer
 */
en_result_t EVENT_PORT_Init(const stc_event_port_init_t *pstcEventPortInit)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcEventPortInit)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Parameter validity checking */
        DDL_ASSERT(IS_EVENT_PORT_TRIGGER(pstcEventPortInit->u32EPTrigger));
        DDL_ASSERT(IS_EVENT_PORT_FE(pstcEventPortInit->u32EPFE));
        DDL_ASSERT(IS_EVENT_PORT_FCLK(pstcEventPortInit->u32EPFClk));
        WRITE_REG32(M0P_AOS->EVPRT_CR,pstcEventPortInit->u32EPTrigger   |       \
                                    pstcEventPortInit->u32EPFE          |       \
                                    pstcEventPortInit->u32EPFClk);
    }
    return enRet;
}

/**
 * @brief  Initialize EVENT_PORT structure. Fill each pstcEventPortInit with
 *         default value
 * @param  [in] pstcEventPortInit: Pointer to a stc_event_port_init_t structure
 *                                 that contains configuration information.
 * @retval Ok: EVENT_PORT structure initilize successful
 *         ErrorInvalidParameter: NULL pointer
 */
en_result_t EVENT_PORT_StructInit(stc_event_port_init_t *pstcEventPortInit)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcEventPortInit)
    {
       enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Reset EVENT_PORT init structure parameters values */
        pstcEventPortInit->u32EPFE      = 0ul;
        pstcEventPortInit->u32EPFClk    = 0ul;
        pstcEventPortInit->u32EPTrigger = 0ul;
    }
    return enRet;
}

/**
 * @brief  Get EVENT_PORT state, can record 4 times at most
 * @param  [in] u8RecordTimes: record times for EVENT_PORT
 *   @arg  EP_STATE_CURRENT
 *   @arg  EP_STATE_LASTTIME
 *   @arg  EP_STATE_LAST2TIME
 *   @arg  EP_STATE_LAST3TIME
 * @retval uint8_t: EVENT_PORT state in different phase
 *          EP1_STATE_SET
 *          EP2_STATE_SET
 *          EP3_STATE_SET
 *          EP4_STATE_SET
 *          EP1_STATE_RESET
 *          EP2_STATE_RESET
 *          EP3_STATE_RESET
 *          EP4_STATE_RESET
 */
uint8_t EVENT_PORT_GetStatus(uint8_t u8RecordTimes)
{
    uint8_t u8EPState = 0xFFu;

    DDL_ASSERT(IS_EVENT_PORT_RECORD(u8RecordTimes));
    switch (u8RecordTimes)
    {
        case EP_STATE_CURRENT:
            u8EPState = (uint8_t)M0P_AOS->EVPRT_SR_f.PTSTS0;
            break;
        case EP_STATE_LASTTIME:
            u8EPState = (uint8_t)M0P_AOS->EVPRT_SR_f.PTSTS1;
            break;
        case EP_STATE_LAST2TIME:
            u8EPState = (uint8_t)M0P_AOS->EVPRT_SR_f.PTSTS2;
            break;
        case EP_STATE_LAST3TIME:
            u8EPState = (uint8_t)M0P_AOS->EVPRT_SR_f.PTSTS3;
            break;
        default:
            break;
    }
    return u8EPState;
}

/**
 * @brief  EVENT_PORT trigger edge configure
 * @param  [in] u8TriggerEdge: EVENT_PORT trigger edge of each channel
 *   @arg  EP1_TRIGGER_FALLING
 *   @arg  EP2_TRIGGER_FALLING
 *   @arg  EP3_TRIGGER_FALLING
 *   @arg  EP4_TRIGGER_FALLING
 *   @arg  EP1_TRIGGER_RISING
 *   @arg  EP2_TRIGGER_RISING
 *   @arg  EP3_TRIGGER_RISING
 *   @arg  EP4_TRIGGER_RISING
 * @param  [in] enNewState
 *   @arg  Enable: Enable corresponding trigger edge
 *   @arg  Disable: Disable corresponding trigger edge
 * @retval None
 */
void EVENT_PORT_TriggerEdgeCmd(uint8_t u8TriggerEdge, en_functional_state_t enNewState)
{
    if (Enable == enNewState)
    {
        SET_REG8_BIT(M0P_AOS->EVPRT_CR, u8TriggerEdge);
    }
    else
    {
        CLEAR_REG8_BIT(M0P_AOS->EVPRT_CR, u8TriggerEdge);
    }
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
