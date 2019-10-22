/**
 *******************************************************************************
 * @file  hc32m120_interrupts.c
 * @brief This file provides firmware functions to manage the Interrupt Controller
 *        (INTC).
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32m120_interrupts.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_INTERRUPTS INTERRUPTS
 * @brief INTC Driver Library
 * @{
 */

#if (DDL_INTERRUPTS_ENABLE == DDL_ON) || (DDL_EXINT_NMI_ENABLE == DDL_ON) ||    \
    (DDL_EKEY_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup INTC_Local_Macros INTC Local Macros
 * @{
 */
/**
 * @brief   Maximum IRQ handler number
 */
#define IRQ_NUM_MAX         16u

/**
 * @defgroup INTC_Check_Parameters_Validity INTC Check Parameters Validity
 * @{
 */
/*  Parameter validity check for wakeup source from stop mode. */
#define IS_INTC_WKUP_SRC(src)                                                   \
(   ((src) & (INTC_WUPENR_EIRQWUEN      | INTC_WUPENR_SWDTWUEN      |           \
             INTC_WUPENR_EKEYWUEN       | INTC_WUPENR_TMR0CMPWUEN   |           \
             INTC_WUPENR_TMR2CMPWUEN    | INTC_WUPENR_TMR2OVFWUEN   |           \
             INTC_WUPENR_CMPWUEN        | INTC_WUPENR_PVDWUEN))                 \
             != (uint32_t)0x00000000ul)

/*  Parameter validity check for event number. */
#define IS_INTC_EVENT(event)                                                    \
(   ((event) & (INTC_EVTER_EVTEN0   | INTC_EVTER_EVTEN1                 |       \
                INTC_EVTER_EVTEN2   | INTC_EVTER_EVTEN3                 |       \
                INTC_EVTER_EVTEN4   | INTC_EVTER_EVTEN5                 |       \
                INTC_EVTER_EVTEN6   | INTC_EVTER_EVTEN7)) != (uint8_t)0x00u)

/*  Parameter validity check for NMI pin filter function. */
#define IS_NMI_FE(fe)                                                           \
(   ((fe) == NMI_FILTER_OFF)                    ||                              \
    ((fe) == NMI_FILTER_ON))

/*  Parameter validity check for MNI pin filter clock division. */
#define IS_NMI_FCLK(fclk)                                                       \
(   ((fclk) == NMI_FCLK_HCLK_DIV1)              ||                              \
    ((fclk) == NMI_FCLK_HCLK_DIV8)              ||                              \
    ((fclk) == NMI_FCLK_HCLK_DIV32)             ||                              \
    ((fclk) == NMI_FCLK_HCLK_DIV64))

/*  Parameter validity check for NMI pin trigger edge. */
#define IS_NMI_TRIGGER(trigger)                                                 \
(   ((trigger) == NMI_TRIGGER_FALLING)          ||                              \
    ((trigger) == NMI_TRIGGER_RISING))

/*  Parameter validity check for NMI trigger souce. */
#define IS_NMI_SRC(src) (((src) & NMI_SRC_MASK) != (uint8_t)0x00u)

/*  Parameter validity check for get NMI trigger source. */
#define IS_GET_NMI_SRC(src)                                                     \
(   ((src) == NMI_SRC_NMI_PIN)                  ||                              \
    ((src) == NMI_SRC_XTAL_STP)                 ||                              \
    ((src) == NMI_SRC_SWDT)                     ||                              \
    ((src) == NMI_SRC_LVD))

/*  Parameter validity check for EXINT filter function. */
#define IS_EXINT_FE(fe)                                                         \
(   ((fe) == EXINT_FILTER_OFF)                  ||                              \
    ((fe) == EXINT_FILTER_ON))

/*  Parameter validity check for EXINT filter clock division. */
#define IS_EXINT_FCLK(fclk)                                                     \
(   ((fclk) == EXINT_FCLK_HCLK_DIV1)            ||                              \
    ((fclk) == EXINT_FCLK_HCLK_DIV8)            ||                              \
    ((fclk) == EXINT_FCLK_HCLK_DIV32)           ||                              \
    ((fclk) == EXINT_FCLK_HCLK_DIV64))

/*  Parameter validity check for EXINT trigger edge. */
#define IS_EXINT_TRIGGER(trigger)                                               \
(   ((trigger) == EXINT_TRIGGER_LOW)            ||                              \
    ((trigger) == EXINT_TRIGGER_RISING)         ||                              \
    ((trigger) == EXINT_TRIGGER_FALLING)        ||                              \
    ((trigger) == EXINT_TRIGGER_BOTH))

/*  Parameter validity check for EXINT channel. */
#define IS_EXINT_CH(ch)     (((ch) & EXINT_CH_MASK) != (uint16_t)0x0000u)

/*  Parameter validity check for get EXINT channel. */
#define IS_GET_EXINT_CH(ch)                                                     \
(   ((ch) == EXINT_CH00)                        ||                              \
    ((ch) == EXINT_CH01)                        ||                              \
    ((ch) == EXINT_CH02)                        ||                              \
    ((ch) == EXINT_CH03)                        ||                              \
    ((ch) == EXINT_CH04)                        ||                              \
    ((ch) == EXINT_CH05)                        ||                              \
    ((ch) == EXINT_CH06)                        ||                              \
    ((ch) == EXINT_CH07)                        ||                              \
    ((ch) == EXINT_CH08)                        ||                              \
    ((ch) == EXINT_CH09))

/*  Parameter validity check for EKEY. */
#define IS_INTC_EKEY(ekey)  (((ekey) & INTC_EKEY_MASK) != (uint8_t)0x00u)
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
/**
 * @defgroup INTC_Local_Variable INTC Local Variable
 * @{
 */
static func_ptr_t pfnIrqHandler[IRQ_NUM_MAX] = {NULL};
static func_ptr_t pfnNmiCallback;
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup INTC_Global_Functions INTC Global Functions
 * @{
 */
/**
 * @brief  IRQ registration function
 * @param  [in] pstcIrqRegiConfig: pointer of IRQ registration structure
 *   @arg  enIntSrc: can be any value except INT_PORT_EIRQ0~7 @ref en_int_src_t
 *   @arg  enIRQn: can be any value from Int008~Int023 @ref IRQn_Type
 *   @arg  pfnCallback: Callback function
 * @retval Ok: IRQ register successfully
 *         ErrorInvalidParameter: IRQ No. and Peripheral Int source are not match;
 *                                Input peripheral Int source cannot be configure;
 *                                NULL pointer.
 */
en_result_t INTC_IrqRegistration(const stc_irq_regi_config_t *pstcIrqRegiConfig)
{
    __IO stc_intc_iselar_field_t *stcIntSel;
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcIrqRegiConfig)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Checking validity for Interrupt source of Group 0~7 and IRQ No. */
        if (((pstcIrqRegiConfig->enIntSrc / 0x10u)*2u + 8u != pstcIrqRegiConfig->enIRQn) && \
            ((pstcIrqRegiConfig->enIntSrc / 0x10u)*2u + 9u != pstcIrqRegiConfig->enIRQn))
        {
            enRet = ErrorInvalidParameter;
        }
        else
        {
            /* EIRQ0~7 are fixed allocation of IRQ handler 0~7 */
            if ((0u == pstcIrqRegiConfig->enIntSrc % 0x10u) || (pstcIrqRegiConfig->enIRQn < 8u))
            {
                enRet = ErrorInvalidParameter;
            }
            else
            {
                INTC_Unlock();
                stcIntSel = (stc_intc_iselar_field_t *)((uint32_t)(&M0P_INTC->ISELAR8) +    \
                                                     (4u * (pstcIrqRegiConfig->enIRQn - 8u)));
                stcIntSel->ISEL = (pstcIrqRegiConfig->enIntSrc) % 0x10u;

                /* Callback function */
                pfnIrqHandler[pstcIrqRegiConfig->enIRQn-8u] = pstcIrqRegiConfig->pfnCallback;

                INTC_Lock();
            }
        }
    }
    return enRet;
}

/**
 * @brief  Share IRQ configure
 * @param  [in] enIntSrc: Peripheral interrupt source @ref en_int_src_t
 * @param  [in] enNewState:
 *   @arg  Enable: Enable corresponding peripheral interrupt in share IRQ handler
 *   @arg  Disable: Disable corresponding peripheral interrupt in share IRQ handler
 * @retval Ok: Share IRQ configure successfully
 *         ErrorInvalidParameter: EXINT00~07 cannot be configured into share IRQ handler
 */
en_result_t INTC_ShareIrqCmd(en_int_src_t enIntSrc, en_functional_state_t enNewState)
{
    __IO uint32_t *ISELRx;
    en_result_t enRet = Ok;

    /* EXINT0~7 cannot be configured into share IRQ */
    if (0u == enIntSrc % 0x10u)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        INTC_Unlock();
        ISELRx = (uint32_t *)(((uint32_t)&M0P_INTC->ISELBR24) + (4u * (enIntSrc / 16u)));
        if (Enable == enNewState)
        {
            SET_REG32_BIT(*ISELRx, (1ul << (enIntSrc % 16ul)));
        }
        else
        {
            CLEAR_REG32_BIT(*ISELRx, (1ul << (enIntSrc % 16ul)));
        }
        INTC_Lock();
    }
    return enRet;
}

/**
 * @brief  Stop mode wake-up source configure
 * @param  [in] u32WakeupSrc: Wake-up source
 *   @arg  INTC_WUPENR_EIRQWUEN
 *   @arg  INTC_WUPENR_EIRQWUEN_0
 *   @arg  INTC_WUPENR_EIRQWUEN_1
 *   @arg  INTC_WUPENR_EIRQWUEN_2
 *   @arg  INTC_WUPENR_EIRQWUEN_3
 *   @arg  INTC_WUPENR_EIRQWUEN_4
 *   @arg  INTC_WUPENR_EIRQWUEN_5
 *   @arg  INTC_WUPENR_EIRQWUEN_6
 *   @arg  INTC_WUPENR_EIRQWUEN_7
 *   @arg  INTC_WUPENR_EIRQWUEN_8
 *   @arg  INTC_WUPENR_EIRQWUEN_9
 *   @arg  INTC_WUPENR_SWDTWUEN
 *   @arg  INTC_WUPENR_EKEYWUEN
 *   @arg  INTC_WUPENR_TMR0CMPWUEN
 *   @arg  INTC_WUPENR_PVDWUEN
 *   @arg  INTC_WUPENR_RTCALMWUEN
 *   @arg  INTC_WUPENR_RTCPRDWUEN
 * @param  [in] enNewState
 *   @arg  Enable: Enable corresponding wake up source
 *   @arg  Disable: Disable corresponding wake up source
 * @retval None
 */
void INTC_WakeupSrcCmd(uint32_t u32WakeupSrc, en_functional_state_t enNewState)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_INTC_WKUP_SRC(u32WakeupSrc));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    INTC_Unlock();
    if (Enable == enNewState)
    {
        SET_REG32_BIT(M0P_INTC->WUPENR, u32WakeupSrc);
    }
    else
    {
        CLEAR_REG32_BIT(M0P_INTC->WUPENR, u32WakeupSrc);
    }
    INTC_Lock();
}

/**
 * @brief  Event or Interrupt output configure
 * @param  [in] u8Event: Event index
 *   @arg  INTC_EVTER_EVTEN0
 *   @arg  INTC_EVTER_EVTEN1
 *   @arg  INTC_EVTER_EVTEN2
 *   @arg  INTC_EVTER_EVTEN3
 *   @arg  INTC_EVTER_EVTEN4
 *   @arg  INTC_EVTER_EVTEN5
 *   @arg  INTC_EVTER_EVTEN6
 *   @arg  INTC_EVTER_EVTEN7
 * @param  [in] enNewState
 *   @arg  Enable: Enable corresponding event to NVIC
 *   @arg  Disable: Disable corresponding event but interrupt to NVIC
 * @retval None
 */
void INTC_EventCmd(uint8_t u8Event, en_functional_state_t enNewState)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_INTC_EVENT(u8Event));

    INTC_Unlock();
    if (Enable == enNewState)
    {
        SET_REG8_BIT(M0P_INTC->EVTER, u8Event);
    }
    else
    {
        CLEAR_REG8_BIT(M0P_INTC->EVTER, u8Event);
    }
    INTC_Lock();
}

/**
 * @brief  Initialize NMI. Fill each pstcNmiConfig with default value
 * @param  [in] pstcNmiConfig: Pointer to a stc_nmi_config_t structure that
 *                             contains configuration information.
 * @retval Ok: NMI structure initialize successful
 *         ErrorInvalidParameter: NULL pointer
 */
en_result_t NMI_StructInit(stc_nmi_config_t *pstcNmiConfig)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcNmiConfig)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcNmiConfig->u8NmiFE          = NMI_FILTER_OFF;
        pstcNmiConfig->u8NmiFClk        = NMI_FCLK_HCLK_DIV1;
        pstcNmiConfig->u8NmiTigger      = NMI_TRIGGER_FALLING;
        pstcNmiConfig->u8NmiSrc         = NMI_SRC_NMI_PIN;
        pstcNmiConfig->pfnNmiCallback   = NULL;
    }
    return enRet;
}

/**
 * @brief  Initialize NMI.
 * @param  [in] pstcNmiConfig: Pointer to a pstcNmiConfig structure that
 *                             contains configuration information.
 * @retval Ok: NMI initialize successful
 *         ErrorInvalidParameter: NULL pointer
 */
en_result_t NMI_Init(const stc_nmi_config_t *pstcNmiConfig)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if ((NULL == pstcNmiConfig) || (NULL == pstcNmiConfig->pfnNmiCallback))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Parameter validity checking */
        DDL_ASSERT(IS_NMI_FCLK(pstcNmiConfig->u8NmiFClk));
        DDL_ASSERT(IS_NMI_FE(pstcNmiConfig->u8NmiFE));
        DDL_ASSERT(IS_NMI_TRIGGER(pstcNmiConfig->u8NmiTigger));
        DDL_ASSERT(IS_NMI_SRC(pstcNmiConfig->u8NmiSrc));

        INTC_Unlock();

        /* NMI pin interrupt configure */
        WRITE_REG8(M0P_INTC->NMICR,                                             \
                  (pstcNmiConfig->u8NmiFE         |                             \
                   pstcNmiConfig->u8NmiFClk       |                             \
                   pstcNmiConfig->u8NmiTigger));

        /* Clear all NMI trigger source before set */
        WRITE_REG8(M0P_INTC->NMICLR, INTC_NMICLR_MASK);

        /* NMI trigger source configure */
        WRITE_REG8(M0P_INTC->NMIER, pstcNmiConfig->u8NmiSrc);

        /* NMI callback function configure */
        pfnNmiCallback = pstcNmiConfig->pfnNmiCallback;

        INTC_Lock();
    }
    return enRet;
}

/**
 * @brief  Get NMI trigger source
 * @param  [in] u8NmiSrc: NMI trigger source, @ref NMI_TriggerSrc_Sel for details
 *   @arg  NMI_SRC_NMI_PIN
 *   @arg  NMI_SRC_XTAL_STP
 *   @arg  NMI_SRC_SWDT
 *   @arg  NMI_SRC_LVD
 * @retval Set: NMI is triggered by corresponding source
 *         Reset: NMI is not triggered by corresponding source
 */
en_flag_status_t NMI_GetNmiSrc(uint8_t u8NmiSrc)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_GET_NMI_SRC(u8NmiSrc));

    return ((READ_REG8(M0P_INTC->NMIFR) & u8NmiSrc)) ? Set : Reset;
}

/**
 * @brief  Clear specified NMI trigger source
 * @param  [in] u8NmiSrc: NMI trigger source, @ref NMI_TriggerSrc_Sel for details
 *   @arg  NMI_SRC_NMI_PIN
 *   @arg  NMI_SRC_XTAL_STP
 *   @arg  NMI_SRC_SWDT
 *   @arg  NMI_SRC_LVD
 * @retval None
 */
void NMI_ClrNmiSrc(uint8_t u8NmiSrc)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_NMI_SRC(u8NmiSrc));

    INTC_Unlock();
    MODIFY_REG8(M0P_INTC->NMICLR, INTC_NMICLR_MASK, u8NmiSrc);
    INTC_Lock();
}

/**
 * @brief  NMI IRQ handler
 * @param  None
 * @retval None
 */
void NMI_IrqHandler(void)
{
    if (NULL != pfnNmiCallback)
    {
        pfnNmiCallback();
    }
}

/**
 * @brief  Initialize External interrupt.
 * @param  [in] pstcExIntConfig: Pointer to a pstcExIntConfig structure that
 *                             contains configuration information.
 * @retval Ok:  EXINT initialize successful
 *         ErrorInvalidParameter: NULL pointer
 */
en_result_t EXINT_Init(const stc_exint_config_t *pstcExIntConfig)
{
    uint8_t u8ExIntPos = 0u;
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcExIntConfig)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Parameter validity checking */
        DDL_ASSERT(IS_EXINT_FE(pstcExIntConfig->u8ExIntFE));
        DDL_ASSERT(IS_EXINT_FCLK(pstcExIntConfig->u8ExIntFClk));
        DDL_ASSERT(IS_EXINT_TRIGGER(pstcExIntConfig->u8ExIntLvl));
        DDL_ASSERT(IS_EXINT_CH(pstcExIntConfig->u16ExIntCh));

        INTC_Unlock();
        for (u8ExIntPos = 0u; u8ExIntPos < 10u; u8ExIntPos++)
        {
            if (pstcExIntConfig->u16ExIntCh & (1ul << u8ExIntPos))
            {
                WRITE_REG8(*(uint8_t *)((uint32_t)(&M0P_INTC->EIRQCR0) + 4u*u8ExIntPos),\
                          (pstcExIntConfig->u8ExIntFE         |                         \
                           pstcExIntConfig->u8ExIntFClk       |                         \
                           pstcExIntConfig->u8ExIntLvl));
            }
        }
        INTC_Lock();
    }
    return enRet;
}

/**
 * @brief  Initialize ExInt. Fill each pstcExintConfig with default value
 * @param  [in] pstcExintConfig: Pointer to a pstcExintConfig structure
 *                              that contains configuration information.
 * @retval Ok: EXINT structure initialize successful
 *         ErrorInvalidParameter: NULL pointer
 */
en_result_t EXINT_StructInit(stc_exint_config_t *pstcExintConfig)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcExintConfig)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcExintConfig->u16ExIntCh     = (uint16_t)0ul;
        pstcExintConfig->u8ExIntFE      = EXINT_FILTER_OFF;
        pstcExintConfig->u8ExIntFClk    = EXINT_FCLK_HCLK_DIV1;
        pstcExintConfig->u8ExIntLvl     = EXINT_TRIGGER_FALLING;
    }
    return enRet;
}

/**
 * @brief  Clear specified External interrupt trigger source
 * @param  [in] u16ExIntCh: External interrupt channel, @ref EXINT_Channel_Sel for details
 *   @arg  EXINT_CH00
 *   @arg  EXINT_CH01
 *   @arg  EXINT_CH02
 *   @arg  EXINT_CH03
 *   @arg  EXINT_CH04
 *   @arg  EXINT_CH05
 *   @arg  EXINT_CH06
 *   @arg  EXINT_CH07
 *   @arg  EXINT_CH08
 *   @arg  EXINT_CH09
 * @retval None
 */
void EXINT_ClrExIntSrc(uint16_t u16ExIntCh)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_EXINT_CH(u16ExIntCh));

    INTC_Unlock();
    SET_REG16_BIT(M0P_INTC->EIRQCLR, u16ExIntCh);
    INTC_Lock();
}

/**
 * @brief  Get specified External interrupt trigger source
 * @param  [in] u16ExIntCh: External interrupt channel, @ref EXINT_Channel_Sel for details
 *   @arg  EXINT_CH00
 *   @arg  EXINT_CH01
 *   @arg  EXINT_CH02
 *   @arg  EXINT_CH03
 *   @arg  EXINT_CH04
 *   @arg  EXINT_CH05
 *   @arg  EXINT_CH06
 *   @arg  EXINT_CH07
 *   @arg  EXINT_CH08
 *   @arg  EXINT_CH09
 * @retval Set: Specified channel of external interrupt is triggered
 *         Reset: Specified channel of external interrupt is not triggered
 */
en_flag_status_t EXINT_GetExIntSrc(uint16_t u16ExIntCh)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_EXINT_CH(u16ExIntCh));

    return (READ_REG16(M0P_INTC->EIRQFR) & u16ExIntCh) ? Set : Reset;
}

/**
 * @brief  Configure the specified EKEY
 * @param  [in] u8EKey: EKEY channel, @ref INTC_EKEY_Channel for details
 *   @arg  INTC_EKEY0
 *   @arg  INTC_EKEY1
 *   @arg  INTC_EKEY2
 *   @arg  INTC_EKEY3
 *   @arg  INTC_EKEY4
 *   @arg  INTC_EKEY5
 * @param  [in] enNewState
 *   @arg  Enable: Enable corresponding EKEY
 *   @arg  Disable: Disable corresponding EKEY
 * @retval None
 */
void INTC_EKeyCmd(uint8_t u8EKey, en_functional_state_t enNewState)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_INTC_EKEY(u8EKey));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    INTC_Unlock();
    if (Enable == enNewState)
    {
        SET_REG8_BIT(M0P_INTC->EKEYCR, u8EKey);
    }
    else
    {
        CLEAR_REG8_BIT(M0P_INTC->EKEYCR, u8EKey);
    }
    INTC_Lock();
}

/**
 * @brief  NMI IRQ handler
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
    NMI_IrqHandler();
}

/**
 * @brief  Hard Fault IRQ handler
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    HardFault_IrqHandler();
}

/**
 * @brief  SVCall IRQ handler
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
    SVC_IrqHandler();
}

/**
 * @brief  PendSV IRQ handler
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
    PendSV_IrqHandler();
}

/**
 * @brief  SysTick IRQ handler
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    SysTick_IrqHandler();
}

/**
 * @brief  Interrupt No.008 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ008_Handler(void)
{
    pfnIrqHandler[Int008_IRQn-8u]();
}

/**
 * @brief  Interrupt No.009 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ009_Handler(void)
{
    pfnIrqHandler[Int009_IRQn-8u]();
}

/**
 * @brief  Interrupt No.010 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ010_Handler(void)
{
    pfnIrqHandler[Int010_IRQn-8u]();
}


/**
 * @brief  Interrupt No.011 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ011_Handler(void)
{
    pfnIrqHandler[Int011_IRQn-8u]();
}

/**
 * @brief  Interrupt No.012 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ012_Handler(void)
{
    pfnIrqHandler[Int012_IRQn-8u]();
}

/**
 * @brief  Interrupt No.013 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ013_Handler(void)
{
    pfnIrqHandler[Int013_IRQn-8u]();
}

/**
 * @brief  Interrupt No.014 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ014_Handler(void)
{
    pfnIrqHandler[Int014_IRQn-8u]();
}

/**
 * @brief  Interrupt No.015 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ015_Handler(void)
{
    pfnIrqHandler[Int015_IRQn-8u]();
}

/**
 * @brief  Interrupt No.016 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ016_Handler(void)
{
    pfnIrqHandler[Int016_IRQn-8u]();
}

/**
 * @brief  Interrupt No.017 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ017_Handler(void)
{
    pfnIrqHandler[Int017_IRQn-8u]();
}

/**
 * @brief  Interrupt No.018 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ018_Handler(void)
{
    pfnIrqHandler[Int018_IRQn-8u]();
}

/**
 * @brief  Interrupt No.019 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ019_Handler(void)
{
    pfnIrqHandler[Int019_IRQn-8u]();
}

/**
 * @brief  Interrupt No.020 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ020_Handler(void)
{
    pfnIrqHandler[Int020_IRQn-8u]();
}

/**
 * @brief  Interrupt No.021 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ021_Handler(void)
{
    pfnIrqHandler[Int021_IRQn-8u]();
}

/**
 * @brief  Interrupt No.022 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ022_Handler(void)
{
    pfnIrqHandler[Int022_IRQn-8u]();
}

/**
 * @brief  Interrupt No.023 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ023_Handler(void)
{
    pfnIrqHandler[Int023_IRQn-8u]();
}

/**
 * @brief  Interrupt No.024 share IRQ handler
 * @param  None
 * @retval None
 */
void IRQ024_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t ISELBR24 = M0P_INTC->ISELBR24;

    /* External interrupt 08 */
    u32Tmp1 = bM0P_INTC->EIRQFR_b.EIRQF8;
    if ((ISELBR24 & BIT_MASK_01) && (u32Tmp1))
    {
        Extint08_IrqHandler();
    }
    /* DMA Ch.0 transfer complete  */
    if (Reset == bM0P_DMA->INTMASK1_b.MSKTC0)
    {
        if ((bM0P_DMA->INTSTAT1_b.TC0) && (ISELBR24 & BIT_MASK_02))
        {
            DmaTc0_IrqHandler();
        }
    }
    /* DMA Ch.0 block transfer complete  */
    if (Reset == bM0P_DMA->INTMASK1_b.MSKBTC0)
    {
        if ((bM0P_DMA->INTSTAT1_b.BTC0) && (ISELBR24 & BIT_MASK_03))
        {
            DmaBtc0_IrqHandler();
        }
    }
    /* EFM program/erase error */
    if (bM0P_EFM->FITE_b.PEERRITE)
    {
        u32Tmp1 = M0P_EFM->FSR & (EFM_FSR_PEWERR | EFM_FSR_PEPRTERR | EFM_FSR_PGMISMTCH);
        if ((ISELBR24 & BIT_MASK_04) && (u32Tmp1))
        {
            EfmPgmEraseErr_IrqHandler();
        }
    }
    /* EFM read collision */
    if (bM0P_EFM->FITE_b.RDCOLERRITE)
    {
        u32Tmp1 = bM0P_EFM->FSR_b.RDCOLERR;
        if ((ISELBR24 & BIT_MASK_05) && (u32Tmp1))
        {
            EfmColErr_IrqHandler();
        }
    }
    /* XTAL stop */
    if (bM0P_CMU->XTALSTDCR_b.XTALSTDIE)
    {
        u32Tmp1 = bM0P_CMU->XTALSTDSR_b.XTALSTDF;
        if ((ISELBR24 & BIT_MASK_06) && (u32Tmp1))
        {
            XtalStop_IrqHandler();
        }
    }
    /* SWDT underflow or fresh error */
    u32Tmp1 = M0P_SWDT->SR & (SWDT_SR_UDF | SWDT_SR_REF);
    if ((ISELBR24 & BIT_MASK_07) && (u32Tmp1))
    {
        Swdt_IrqHandler();
    }
    /* USART1 Rx ORE/FE/PE error */
    u32Tmp1 = M0P_USART1->SR & (USART_SR_PE | USART_SR_FE | USART_SR_ORE);
    u32Tmp2 = bM0P_USART1->CR1_b.RIE;
    if ((ISELBR24 & BIT_MASK_12) && (u32Tmp1) && (u32Tmp2))
    {
        Usart1RxErr_IrqHandler();
    }
    /* I2c error */
    u32Tmp1 = M0P_I2C->SR & (I2C_SR_STARTF   | I2C_SR_SLADDR0F     |            \
                             I2C_SR_SLADDR1F | I2C_SR_STOPF        |            \
                             I2C_SR_ARLOF    | I2C_SR_NACKF        |            \
                             I2C_SR_GENCALLF | I2C_SR_SMBDEFAULTF  |            \
                             I2C_SR_SMBHOSTF | I2C_SR_SMBALRTF);

    u32Tmp2 = M0P_I2C->CR2& (I2C_CR2_STARTIE   | I2C_CR2_SLADDR0IE     |        \
                             I2C_CR2_SLADDR1IE | I2C_CR2_STOPIE        |        \
                             I2C_CR2_ARLOIE    | I2C_CR2_NACKIE        |        \
                             I2C_CR2_GENCALLIE | I2C_CR2_SMBDEFAULTIE  |        \
                             I2C_CR2_SMBHOSTIE | I2C_CR2_SMBALRTIE);
    if ((ISELBR24 & BIT_MASK_13) && (u32Tmp1 & u32Tmp2))
    {
        I2cErr_IrqHandler();
    }
    /* SPI parity/overflow/underflow/mode error */
    u32Tmp1 = M0P_SPI->SR & (SPI_SR_UDRERF | SPI_SR_PERF | SPI_SR_MODFERF | SPI_SR_OVRERF);
    u32Tmp2 = bM0P_SPI->CR1_b.EIE;
    if ((ISELBR24 & BIT_MASK_14) && (u32Tmp1) && (u32Tmp2))
    {
        SpiErr_IrqHandler();
    }
}

/**
 * @brief  Interrupt No.025 share IRQ handler
 * @param  None
 * @retval None
 */
void IRQ025_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t ISELBR25 = M0P_INTC->ISELBR25;

    /* External interrupt 09 */
    u32Tmp1 = bM0P_INTC->EIRQFR_b.EIRQF9;
    if ((ISELBR25 & BIT_MASK_01) && (u32Tmp1))
    {
        Extint09_IrqHandler();
    }
    /* DMA Ch.1 transfer complete  */
    if (Reset == bM0P_DMA->INTMASK1_b.MSKTC1)
    {
        if ((bM0P_DMA->INTSTAT1_b.TC1) && (ISELBR25 & BIT_MASK_02))
        {
            DmaTc1_IrqHandler();
        }
    }
    /* DMA Ch.1 block transfer complete  */
    if (Reset == bM0P_DMA->INTMASK1_b.MSKBTC1)
    {
        if ((bM0P_DMA->INTSTAT1_b.BTC1) && (ISELBR25 & BIT_MASK_03))
        {
            DmaBtc1_IrqHandler();
        }
    }
    /* Clock trimming error */
    u32Tmp1 = M0P_CTC->STR & (CTC_STR_TRMOVF | CTC_STR_TRMUDF);
    u32Tmp2 = bM0P_CTC->CR1_b.ERRIE;
    if ((ISELBR25 & BIT_MASK_04) && (u32Tmp1) && (u32Tmp2))
    {
        ClockTrimErr_IrqHandler();
    }
    /* USART1 Rx end */
    u32Tmp1 = bM0P_USART1->SR_b.RXNE;
    u32Tmp2 = bM0P_USART1->CR1_b.RIE;
    if ((ISELBR25 & BIT_MASK_12) && (u32Tmp1) && (u32Tmp2))
    {
        Usart1RxEnd_IrqHandler();
    }
}

/**
 * @brief  Interrupt No.026 share IRQ handler
 * @param  None
 * @retval None
 */
void IRQ026_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t ISELBR26 = M0P_INTC->ISELBR26;

    /* DMA request or transfer error */
    u32Tmp1 = M0P_DMA->INTSTAT0 & (DMA_INTSTAT0_TRNERR | DMA_INTSTAT0_REQERR);
    u32Tmp2 = (uint32_t)(~(M0P_DMA->INTMASK0) & (DMA_INTMASK0_MSKTRNERR | DMA_INTMASK0_MSKREQERR));
    if ((ISELBR26 & BIT_MASK_01) && (u32Tmp1 & u32Tmp2))
    {
        DmaErr_IrqHandler();
    }
    /* Timer4 U phase higher compare match */
    u32Tmp1 = bM0P_TMR4->OCSRU_b.OCFH;
    u32Tmp2 = bM0P_TMR4->OCSRU_b.OCIEH;
    if ((ISELBR26 & BIT_MASK_02) && (u32Tmp1) && (u32Tmp2))
    {
        Timer4GCMUH_IrqHandler();
    }
    /* Timer4 U phase lower compare match */
    u32Tmp1 = bM0P_TMR4->OCSRU_b.OCFL;
    u32Tmp2 = bM0P_TMR4->OCSRU_b.OCIEL;
    if ((ISELBR26 & BIT_MASK_03) && (u32Tmp1) && (u32Tmp2))
    {
        Timer4GCMUL_IrqHandler();
    }
    /* Timer4 V phase higher compare match */
    u32Tmp1 = bM0P_TMR4->OCSRV_b.OCFH;
    u32Tmp2 = bM0P_TMR4->OCSRV_b.OCIEH;
    if ((ISELBR26 & BIT_MASK_04) && (u32Tmp1) && (u32Tmp2))
    {
        Timer4GCMVH_IrqHandler();
    }
    /* Timer4 V phase lower compare match */
    u32Tmp1 = bM0P_TMR4->OCSRV_b.OCFL;
    u32Tmp2 = bM0P_TMR4->OCSRV_b.OCIEL;
    if ((ISELBR26 & BIT_MASK_05) && (u32Tmp1) && (u32Tmp2))
    {
        Timer4GCMVL_IrqHandler();
    }
    /* Timer4 W phase higher compare match */
    u32Tmp1 = bM0P_TMR4->OCSRW_b.OCFH;
    u32Tmp2 = bM0P_TMR4->OCSRW_b.OCIEH;
    if ((ISELBR26 & BIT_MASK_06) && (u32Tmp1) && (u32Tmp2))
    {
        Timer4GCMWH_IrqHandler();
    }
    /* Timer4 W phase lower compare match */
    u32Tmp1 = bM0P_TMR4->OCSRW_b.OCFL;
    u32Tmp2 = bM0P_TMR4->OCSRW_b.OCIEL;
    if ((ISELBR26 & BIT_MASK_07) && (u32Tmp1) && (u32Tmp2))
    {
        Timer4GCMWL_IrqHandler();
    }
    /* USART1 Tx buffer empty */
    u32Tmp1 = bM0P_USART1->SR_b.TXE;
    u32Tmp2 = bM0P_USART1->CR1_b.TXEIE;
    if ((ISELBR26 & BIT_MASK_12) && (u32Tmp1) && (u32Tmp2))
    {
        Usart1TxEmpt_IrqHandler();
    }
    /* USART3 Rx ORE/FE/PE error */
    u32Tmp1 = M0P_USART3->SR & (USART_SR_PE | USART_SR_FE | USART_SR_ORE);
    u32Tmp2 = bM0P_USART3->CR1_b.RIE;
    if ((ISELBR26 & BIT_MASK_13) && (u32Tmp1) && (u32Tmp2))
    {
        Usart3RxErr_IrqHandler();
    }
}

/**
 * @brief  Interrupt No.027 share IRQ handler
 * @param  None
 * @retval None
 */
void IRQ027_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t ISELBR27 = M0P_INTC->ISELBR27;

    /* EKEY and other Interrupt source are exclusive */
    if (ISELBR27 & BIT_MASK_01)
    {
        EKey_IrqHandler();
    }
    else
    {
        /* Timer 0 compare match */
        u32Tmp1 = bM0P_TMR0->STFLR_b.CMFA;
        u32Tmp2 = bM0P_TMR0->BCONR_b.INTENA;
        if ((ISELBR27 & BIT_MASK_02) && (u32Tmp1) && (u32Tmp2))
        {
            Timer0GCmp_IrqHandler();
        }
        /* Timer4 U phase reload */
        u32Tmp1 = bM0P_TMR4->RCSR_b.RTIFU;
        u32Tmp2 = bM0P_TMR4->RCSR_b.RTIDU;
        if ((ISELBR27 & BIT_MASK_04) && (u32Tmp1) && (Reset == u32Tmp2))
        {
            Timer4ReloadU_IrqHandler();
        }
        /* Timer4 V phase reload */
        u32Tmp1 = bM0P_TMR4->RCSR_b.RTIFV;
        u32Tmp2 = bM0P_TMR4->RCSR_b.RTIDV;
        if ((ISELBR27 & BIT_MASK_05) && (u32Tmp1) && (Reset == u32Tmp2))
        {
            Timer4ReloadV_IrqHandler();
        }
        /* Timer4 W phase reload */
        u32Tmp1 = bM0P_TMR4->RCSR_b.RTIFW;
        u32Tmp2 = bM0P_TMR4->RCSR_b.RTIDW;
        if ((ISELBR27 & BIT_MASK_06) && (u32Tmp1) && (Reset == u32Tmp2))
        {
            Timer4ReloadW_IrqHandler();
        }
        /* EMB */
        u32Tmp1 = M0P_EMB->STAT & (EMB_STAT_PORTINF | EMB_STAT_PWMSF |          \
                                   EMB_STAT_CMPF    | EMB_STAT_OSF);
        u32Tmp2 = M0P_EMB->INTEN & (EMB_INTEN_PORTINTEN | EMB_INTEN_PWMINTEN |  \
                                    EMB_INTEN_CMPINTEN  | EMB_INTEN_OSINTEN);
        if ((ISELBR27 & BIT_MASK_07) && (u32Tmp1 & u32Tmp2))
        {
            Emb_IrqHandler();
        }
        /* USART1 Tx end */
        u32Tmp1 = bM0P_USART1->SR_b.TC;
        u32Tmp2 = bM0P_USART1->CR1_b.TCIE;
        if ((ISELBR27 & BIT_MASK_12) && (u32Tmp1) && (u32Tmp2))
        {
            Usart1TxEnd_IrqHandler();
        }
        /* USART3 Rx end */
        u32Tmp1 = bM0P_USART3->SR_b.RXNE;
        u32Tmp2 = bM0P_USART3->CR1_b.RIE;
        if ((ISELBR27 & BIT_MASK_13) && (u32Tmp1) && (u32Tmp2))
        {
            Usart3RxEnd_IrqHandler();
        }
        /* SPI Rx end */
        u32Tmp1 = bM0P_SPI->SR_b.RDFF;
        u32Tmp2 = bM0P_SPI->CR1_b.RXIE;
        if ((ISELBR27 & BIT_MASK_14) && (u32Tmp1) && (u32Tmp2))
        {
            SpiRxEnd_IrqHandler();
        }
    }
}

/**
 * @brief  Interrupt No.028 share IRQ handler
 * @param  None
 * @retval None
 */
void IRQ028_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t ISELBR28 = M0P_INTC->ISELBR28;

    /* TimerA overflow */
    u32Tmp1 = bM0P_TMRA->BCSTR_b.OVFF;
    u32Tmp2 = bM0P_TMRA->BCSTR_b.INENOVF;
    if ((ISELBR28 & BIT_MASK_01) && (u32Tmp1) && (u32Tmp2))
    {
        TimerAOV_IrqHandler();
    }
    /* TimerA underflow */
    u32Tmp1 = bM0P_TMRA->BCSTR_b.UDFF;
    u32Tmp2 = bM0P_TMRA->BCSTR_b.INENUDF;
    if ((ISELBR28 & BIT_MASK_02) && (u32Tmp1) && (u32Tmp2))
    {
        TimerAUD_IrqHandler();
    }
    /* TimerA compare match */
    u32Tmp1 = M0P_TMRA->STFLR & (uint32_t)(TMRA_STFLR_CMPF1 | TMRA_STFLR_CMPF2);
    u32Tmp2 = M0P_TMRA->ICONR & (uint32_t)(TMRA_ICONR_ITEN1 | TMRA_ICONR_ITEN2);
    if ((ISELBR28 & BIT_MASK_03) && (u32Tmp1 & u32Tmp2))
    {
        TimerACmp_IrqHandler();
    }
    /* Timer4 overflow */
    u32Tmp1 = bM0P_TMR4->CCSR_b.IRQPF;
    u32Tmp2 = bM0P_TMR4->CCSR_b.IRQPEN;
    if ((ISELBR28 & BIT_MASK_04) && (u32Tmp1) && (u32Tmp2))
    {
        Timer4GOV_IrqHandler();
    }
    /* Timer4 underflow */
    u32Tmp1 = bM0P_TMR4->CCSR_b.IRQZF;
    u32Tmp2 = bM0P_TMR4->CCSR_b.IRQZEN;
    if ((ISELBR28 & BIT_MASK_05) && (u32Tmp1) && (u32Tmp2))
    {
        Timer4GUD_IrqHandler();
    }
    /* TimerB3 overflow */
    u32Tmp1 = bM0P_TMRB3->BCSTR_b.OVFF;
    u32Tmp2 = bM0P_TMRB3->BCSTR_b.INENOVF;
    if ((ISELBR28 & BIT_MASK_08) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB3OF_IrqHandler();
    }
    /* TimerB3 underflow */
    u32Tmp1 = bM0P_TMRB3->BCSTR_b.UDFF;
    u32Tmp2 = bM0P_TMRB3->BCSTR_b.INENUDF;
    if ((ISELBR28 & BIT_MASK_09) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB3UF_IrqHandler();
    }
    /* TimerB4 compare match */
    u32Tmp1 = bM0P_TMRB4->STFLR_b.CMPF1;
    u32Tmp2 = bM0P_TMRB4->ICONR_b.ITEN1;
    if ((ISELBR28 & BIT_MASK_10) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB4Cmp_IrqHandler();
    }
    /* ADC seq.A convert complete */
    u32Tmp1 = bM0P_ADC->ISR_b.EOCAF;
    u32Tmp2 = bM0P_ADC->ICR_b.EOCAIEN;
    if ((ISELBR28 & BIT_MASK_11) && (u32Tmp1) && (u32Tmp2))
    {
        AdcSeqA_IrqHandler();
    }
    /* USART2 Rx ORE/FE/PE error */
    u32Tmp1 = M0P_USART2->SR & (USART_SR_PE | USART_SR_FE | USART_SR_ORE);
    u32Tmp2 = bM0P_USART2->CR1_b.RIE;
    if ((ISELBR28 & BIT_MASK_12) && (u32Tmp1) && (u32Tmp2))
    {
        Usart2RxErr_IrqHandler();
    }
    /* USART3 Tx buffer empty */
    u32Tmp1 = bM0P_USART3->SR_b.TXE;
    u32Tmp2 = bM0P_USART3->CR1_b.TXEIE;
    if ((ISELBR28 & BIT_MASK_13) && (u32Tmp1) && (u32Tmp2))
    {
        Usart3TxEmpt_IrqHandler();
    }
    /* I2c Tx buffer empty */
    u32Tmp1 = bM0P_I2C->SR_b.TEMPTYF;
    u32Tmp2 = bM0P_I2C->CR2_b.TEMPTYIE;
    if ((ISELBR28 & BIT_MASK_14) && (u32Tmp1) && (u32Tmp2))
    {
        I2cTxEmpt_IrqHandler();
    }
    /* SPI bus idle */
    u32Tmp1 = bM0P_SPI->SR_b.IDLNF;
    u32Tmp2 = bM0P_SPI->CR1_b.IDIE;
    if ((ISELBR28 & BIT_MASK_15) && (u32Tmp1) && (u32Tmp2))
    {
        SpiIdle_IrqHandler();
    }
}

/**
 * @brief  Interrupt No.029 share IRQ handler
 * @param  None
 * @retval None
 */
void IRQ029_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t ISELBR29 = M0P_INTC->ISELBR29;

    /* Timer2 compare match */
    u32Tmp1 = bM0P_TMR2->STFLR_b.CMFA;
    u32Tmp2 = bM0P_TMR2->ICONR_b.CMENA;
    if ((ISELBR29 & BIT_MASK_02) && (u32Tmp1) && (u32Tmp2))
    {
        Timer2GCmp_IrqHandler();
    }
    /* Timer2 overflow */
    u32Tmp1 = bM0P_TMR2->STFLR_b.OVFA;
    u32Tmp2 = bM0P_TMR2->ICONR_b.OVENA;
    if ((ISELBR29 & BIT_MASK_03) && (u32Tmp1) && (u32Tmp2))
    {
        Timer2GOV_IrqHandler();
    }
    /* TimerB4 overflow */
    u32Tmp1 = bM0P_TMRB4->BCSTR_b.OVFF;
    u32Tmp2 = bM0P_TMRB4->BCSTR_b.INENOVF;
    if ((ISELBR29 & BIT_MASK_08) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB4OF_IrqHandler();
    }
    /* TimerB4 underflow */
    u32Tmp1 = bM0P_TMRB4->BCSTR_b.UDFF;
    u32Tmp2 = bM0P_TMRB4->BCSTR_b.INENUDF;
    if ((ISELBR29 & BIT_MASK_09) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB4UF_IrqHandler();
    }
    /* TimerB3 compare match */
    u32Tmp1 = bM0P_TMRB3->STFLR_b.CMPF1;
    u32Tmp2 = bM0P_TMRB3->ICONR_b.ITEN1;
    if ((ISELBR29 & BIT_MASK_10) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB3Cmp_IrqHandler();
    }
    /* ADC seq.B convert complete */
    u32Tmp1 = bM0P_ADC->ISR_b.EOCBF;
    u32Tmp2 = bM0P_ADC->ICR_b.EOCBIEN;
    if ((ISELBR29 & BIT_MASK_11) && (u32Tmp1) && (u32Tmp2))
    {
        AdcSeqB_IrqHandler();
    }
    /* USART2 Rx end */
    u32Tmp1 = bM0P_USART2->SR_b.RXNE;
    u32Tmp2 = bM0P_USART2->CR1_b.RIE;
    if ((ISELBR29 & BIT_MASK_12) && (u32Tmp1) && (u32Tmp2))
    {
        Usart2RxEnd_IrqHandler();
    }
    /* USART3 Tx end */
    u32Tmp1 = bM0P_USART3->SR_b.TC;
    u32Tmp2 = bM0P_USART3->CR1_b.TCIE;
    if ((ISELBR29 & BIT_MASK_13) && (u32Tmp1) && (u32Tmp2))
    {
        Usart3TxEnd_IrqHandler();
    }
    /* I2c Rx end */
    u32Tmp1 = bM0P_I2C->SR_b.RFULLF;
    u32Tmp2 = bM0P_I2C->CR2_b.RFULLIE;
    if ((ISELBR29 & BIT_MASK_14) && (u32Tmp1) && (u32Tmp2))
    {
        I2cRxEnd_IrqHandler();
    }
}

/**
 * @brief  Interrupt No.030 share IRQ handler
 * @param  None
 * @retval None
 */
void IRQ030_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t ISELBR30 = M0P_INTC->ISELBR30;

    /* TimerB1 overflow */
    u32Tmp1 = bM0P_TMRB1->BCSTR_b.OVFF;
    u32Tmp2 = bM0P_TMRB1->BCSTR_b.INENOVF;
    if ((ISELBR30 & BIT_MASK_08) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB1OF_IrqHandler();
    }
    /* TimerB1 underflow */
    u32Tmp1 = bM0P_TMRB1->BCSTR_b.UDFF;
    u32Tmp2 = bM0P_TMRB1->BCSTR_b.INENUDF;
    if ((ISELBR30 & BIT_MASK_09) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB1UF_IrqHandler();
    }
    /* TimerB2 compare match */
    u32Tmp1 = bM0P_TMRB2->STFLR_b.CMPF1;
    u32Tmp2 = bM0P_TMRB2->ICONR_b.ITEN1;
    if ((ISELBR30 & BIT_MASK_10) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB2Cmp_IrqHandler();
    }
    /* ADC convert result in range of window 0 setting */
    u32Tmp1 = bM0P_ADC->AWDSR_b.AWD0F;
    u32Tmp2 = bM0P_ADC->AWDCR_b.AWD0IEN;
    if ((ISELBR30 & BIT_MASK_11) && (u32Tmp1) && (u32Tmp2))
    {
        AdcCmp0_IrqHandler();
    }
    /* USART2 Tx buffer empty */
    u32Tmp1 = bM0P_USART2->SR_b.TXE;
    u32Tmp2 = bM0P_USART2->CR1_b.TXEIE;
    if ((ISELBR30 & BIT_MASK_12) && (u32Tmp1) && (u32Tmp2))
    {
        Usart2TxEmpt_IrqHandler();
    }
    /* I2c Tx end */
    u32Tmp1 = bM0P_I2C->SR_b.TENDF;
    u32Tmp2 = bM0P_I2C->CR2_b.TENDIE;
    if ((ISELBR30 & BIT_MASK_13) && (u32Tmp1) && (u32Tmp2))
    {
        I2cTxEnd_IrqHandler();
    }
}

/**
 * @brief  Interrupt No.031 share IRQ handler
 * @param  None
 * @retval None
 */
void IRQ031_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t ISELBR31 = M0P_INTC->ISELBR31;

    /* LVD detected */
    if (0u == M0P_EFM->LVDICGCR_f.LVDDIS)
    {
        u32Tmp1 = bM0P_PWC->LVDCSR_b.DETF;
        if ((ISELBR31 & BIT_MASK_02) && (u32Tmp1))
        {
            Lvd_IrqHandler();
        }
    }
    /* EFM operate end */
    if (bM0P_EFM->FITE_b.OPTENDITE)
    {
        u32Tmp1 = bM0P_EFM->FSR_b.OPTEND;
        if ((ISELBR31 & BIT_MASK_04) && (u32Tmp1))
        {
            EfmOpEnd_IrqHandler();
        }
    }
    /* TimerB2 overflow */
    u32Tmp1 = bM0P_TMRB2->BCSTR_b.OVFF;
    u32Tmp2 = bM0P_TMRB2->BCSTR_b.INENOVF;
    if ((ISELBR31 & BIT_MASK_08) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB2OF_IrqHandler();
    }
    /* TimerB2 underflow */
    u32Tmp1 = bM0P_TMRB2->BCSTR_b.UDFF;
    u32Tmp2 = bM0P_TMRB2->BCSTR_b.INENUDF;
    if ((ISELBR31 & BIT_MASK_09) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB2UF_IrqHandler();
    }
    /* TimerB1 compare match */
    u32Tmp1 = bM0P_TMRB1->STFLR_b.CMPF1;
    u32Tmp2 = bM0P_TMRB1->ICONR_b.ITEN1;
    if ((ISELBR31 & BIT_MASK_10) && (u32Tmp1) && (u32Tmp2))
    {
        TimerB1Cmp_IrqHandler();
    }
    if (Set == bM0P_ADC->AWDCR_b.AWD1IEN)
    {
        /* ADC convert result in range of window 1 if dependence use */
        u32Tmp1 = bM0P_ADC->AWDSR_b.AWD1F;
        u32Tmp2 = M0P_ADC->AWDCR_f.AWDCM;
        if ((ISELBR31 & BIT_MASK_11) && (u32Tmp1) && (Reset == u32Tmp2))
        {
            AdcCmp1_IrqHandler();
        }
        /* ADC convert result combination use of window 0 & 1 */
        u32Tmp1 = bM0P_ADC->AWDSR_b.AWDCMF;
        if ((ISELBR31 & BIT_MASK_11) && (u32Tmp1) && (Reset != u32Tmp2))
        {
            AdcCmp1_IrqHandler();
        }
    }
    /* USART2 Tx end */
    u32Tmp1 = bM0P_USART2->SR_b.TC;
    u32Tmp2 = bM0P_USART2->CR1_b.TCIE;
    if ((ISELBR31 & BIT_MASK_12) && (u32Tmp1) && (u32Tmp2))
    {
        Usart2TxEnd_IrqHandler();
    }
    /* SPI Tx buffer empty */
    u32Tmp1 = bM0P_SPI->SR_b.TDEF;
    u32Tmp2 = bM0P_SPI->CR1_b.TXIE;
    if ((ISELBR31 & BIT_MASK_13) && (u32Tmp1) && (u32Tmp2))
    {
        SpiTxEmpt_IrqHandler();
    }
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
