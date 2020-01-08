/**
 *******************************************************************************
 * @file  hc32m120_timer0.c
 * @brief This file provides firmware functions to manage the TIMER0
 *        (TIMER0).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-20       Heqb          First version
   2019-12-11       Heqb          Add timeout function for register write
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
#include "hc32m120_timer0.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_TIMER0 TIMER0
 * @brief TIMER0 Driver Library
 * @{
 */

#if (DDL_TIMER0_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TIMER0_Local_Macros TIMER0 Local Macros
 * @{
 */
/* Delay count for time out */
#define TIMER0_TMOUT 0x2000ul

/**
 * @defgroup TIMER0_Check_Parameters_Validity TIMER0 Check Parameters Validity
 * @{
 */

#define IS_VALID_CLK_DIVISION(x)                                               \
(   ((x) == TIMER0_CLK_DIV1)                       ||                          \
    ((x) == TIMER0_CLK_DIV2)                       ||                          \
    ((x) == TIMER0_CLK_DIV4)                       ||                          \
    ((x) == TIMER0_CLK_DIV8)                       ||                          \
    ((x) == TIMER0_CLK_DIV16)                      ||                          \
    ((x) == TIMER0_CLK_DIV32)                      ||                          \
    ((x) == TIMER0_CLK_DIV64)                      ||                          \
    ((x) == TIMER0_CLK_DIV128)                     ||                          \
    ((x) == TIMER0_CLK_DIV256)                     ||                          \
    ((x) == TIMER0_CLK_DIV512)                     ||                          \
    ((x) == TIMER0_CLK_DIV1024))

#define IS_VALID_CLK_SRC(x)                                                    \
(   ((x) == TIMER0_CLK_SRC_HCLK)                   ||                          \
    ((x) == TIMER0_CLK_SRC_INTHWTRIG)              ||                          \
    ((x) == TIMER0_CLK_SRC_LRC))

#define IS_VALID_HWTRG_FUNC(x)                                                 \
(   ((x) == TIMER0_BT_HWTRG_FUNC_START)            ||                          \
    ((x) == TIMER0_BT_HWTRG_FUNC_CLEAR)            ||                          \
    ((x) == TIMER0_BT_HWTRG_FUNC_STOP)             ||                          \
    ((x) == TIMER0_BT_HWTRG_FUNC_NONE))

#define IS_VALID_TMR0_FUNC(x)                                                  \
(   ((x) == TIMER0_FUNC_CMP)                       ||                          \
    ((x) == TIMER0_FUNC_CAPTURE))

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
 * @defgroup TIMER0_Local_Functions TIMER0 Local Functions
 * @{
 */

/**
 * @brief  Get Clock mode
 * @param  None
 *
 * @retval TIMER0 Clock mode
 *         - TIMER0_CLK_SYNC
 *         - TIMER0_CLK_ASYNC
 */
static uint32_t TIMER0_GetClkMode(void)
{
    return (M0P_TMR0->BCONR & TIMER0_CLK_SRC_LRC);
}

/**
 * @brief  Time delay for register write in asynchronous mode
 * @param  None
 *
 * @retval None
 */
static void AsyncDelay(void)
{
    uint32_t i;
    if(TIMER0_CLK_SRC_LRC == TIMER0_GetClkMode())
    {
        for(i=0u; i<SystemCoreClock/10000u; i++)
        {
            __NOP();
        }
    }
}

/**
 * @}
 */

/**
 * @defgroup TIMER0_Global_Functions TIMER0 Global Functions
 * @{
 */

/**
 * @brief  Timer0 peripheral initialization stucture clear
 * @param  [in] pstcInitStruct    Timer0 function structure
 *   @arg  See the structure definition for @ref stc_tim0_init_t
 * @retval Ok: Success
 * @retval ErrorInvalidParameter: Parameter error
 */
en_result_t TIMER0_StructInit(stc_tim0_init_t* pstcInitStruct)
{
    en_result_t enRet = ErrorInvalidParameter;
    
    if (pstcInitStruct != NULL)
    {
        pstcInitStruct->u32ClockDivision = TIMER0_CLK_DIV1;
        pstcInitStruct->u32ClockSource = TIMER0_CLK_SRC_HCLK;
        pstcInitStruct->u32Tmr0Fun = TIMER0_FUNC_CMP;
        pstcInitStruct->u16CmpValue = 0xFFFFu;
        pstcInitStruct->u32HwTrigFunc = TIMER0_BT_HWTRG_FUNC_NONE;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Timer0 peripheral function initialize
 * @param  [in] pstcTmr0Init    Timer0 function base parameter structure
 *   @arg  See the structure definition for @ref stc_tim0_init_t
 * @retval Ok: Success
 * @retval ErrorInvalidParameter: Parameter error
 * @retval ErrorTimeout: Process timeout
 * @note   In capture mode, don't need configure member u32HwTrigFunc and u16CmpValue
 */
en_result_t TIMER0_Init(const stc_tim0_init_t* pstcTmr0Init)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32TimeOut = 0ul;
    if (pstcTmr0Init != NULL)
    {
        enRet = Ok;
        DDL_ASSERT(IS_VALID_CLK_DIVISION(pstcTmr0Init->u32ClockDivision));
        DDL_ASSERT(IS_VALID_CLK_SRC(pstcTmr0Init->u32ClockSource));
        DDL_ASSERT(IS_VALID_TMR0_FUNC(pstcTmr0Init->u32Tmr0Fun));
        DDL_ASSERT(IS_VALID_HWTRG_FUNC(pstcTmr0Init->u32HwTrigFunc));

        /* Configure register to default value, TIMER0 enter synchronous mode */
        TIMER0_DeInit();
        AsyncDelay();
        while(0x00000000ul != M0P_TMR0->BCONR)
        {
            if(u32TimeOut++ > TIMER0_TMOUT)
            {
                enRet = ErrorTimeout;
                break;
            }
        }

        /* Set timer compare value */
        M0P_TMR0->CMPAR = pstcTmr0Init->u16CmpValue;

        /* Configure clock division, function mode, Hardware trigger function */
        M0P_TMR0->BCONR = (pstcTmr0Init->u32ClockDivision
                        + pstcTmr0Init->u32HwTrigFunc
                        + pstcTmr0Init->u32Tmr0Fun);
        AsyncDelay();

        /* Configure clock source and clock mode*/
        if(TIMER0_CLK_SRC_LRC == pstcTmr0Init->u32ClockSource)
        {
            bM0P_TMR0->BCONR_b.SYNSA = 1u;
            AsyncDelay();
            u32TimeOut = 0ul;
            while(1u != bM0P_TMR0->BCONR_b.SYNSA)
            {
                if(u32TimeOut++ > TIMER0_TMOUT)
                {
                    enRet = ErrorTimeout;
                    break;
                }
            }
        }
        else
        {
            MODIFY_REG32(M0P_TMR0->BCONR, TMR0_BCONR_SYNCLKA, pstcTmr0Init->u32ClockSource);
            bM0P_TMR0->BCONR_b.SYNSA = 0u;
            AsyncDelay();
            u32TimeOut = 0ul;
            while(1u != bM0P_TMR0->BCONR_b.SYNSA)
            {
                if(u32TimeOut++ > TIMER0_TMOUT)
                {
                    enRet = ErrorTimeout;
                    break;
                }
            }
        }   

    }
    
    return enRet;
}

/**
 * @brief  Get Timer0 status flag
 * @param  [in] None
 *
 * @retval Set                   Flag is set
 *         Reset                 Flag is reset
 * @note   Macksure timer0 function is disable in asynchronous mode
 */
en_flag_status_t TIMER0_GetFlag(void)
{
    return (en_flag_status_t)bM0P_TMR0->STFLR_b.CMFA;
}

/**
 * @brief  Clear Timer0 status flag
 * @param  [in] None
 *
 * @retval Ok: Success
 * @retval ErrorTimeout: Process timeout
 */
en_result_t TIMER0_ClearFlag(void)
{
    en_result_t enRet = Ok;
    uint32_t u32TimeOut = 0ul;
    bM0P_TMR0->STFLR_b.CMFA = 0u;
    AsyncDelay();
    while(0u != bM0P_TMR0->STFLR_b.CMFA)
    {
        if(u32TimeOut++ > TIMER0_TMOUT)
        {
            enRet = ErrorTimeout;
            break;
        }
    }
    return enRet;
}

/**
 * @brief  Command the timer0 function
 * @param  [in] enNewState    Disable or Enable the function
 *
 * @retval Ok: Success
 * @retval ErrorTimeout: Process timeout
 */
en_result_t TIMER0_Cmd(en_functional_state_t enNewState)
{
    en_result_t enRet = Ok;
    uint32_t u32TimeOut = 0ul;
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_TMR0->BCONR_b.CSTA = enNewState;
    AsyncDelay();
    while(enNewState != bM0P_TMR0->BCONR_b.CSTA)
    {
        if(u32TimeOut++ > TIMER0_TMOUT)
        {
            enRet = ErrorTimeout;
            break;
        }
    }
    return enRet;
}

/**
 * @brief  Timer0 interrupt function command
 * @param  [in] enNewState    Disable or Enable the function
 *
 * @retval Ok: Success
 * @retval ErrorTimeout: Process timeout
 */
en_result_t TIMER0_IntCmd(en_functional_state_t enNewState)
{
    en_result_t enRet = Ok;
    uint32_t u32TimeOut = 0ul;
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_TMR0->BCONR_b.INTENA = enNewState;
    AsyncDelay();
    while(enNewState != bM0P_TMR0->BCONR_b.INTENA)
    {
        if(u32TimeOut++ > TIMER0_TMOUT)
        {
            enRet = ErrorTimeout;
            break;
        }
    }
    return enRet;
}

/**
 * @brief  Get Timer0 counter register
 * @param  [in] None
 *
 * @retval The counter register data
 * @note   Macksure timer0 function is disable in asynchronous mode
 */
uint16_t TIMER0_GetCntReg(void)
{
    return (uint16_t)M0P_TMR0->CNTAR&0xFFFFu;
}

/**
 * @brief  Write Timer0 counter register
 * @param  [in] u16Cnt The data to write to the counter register
 *
 * @retval Ok: Success
 * @retval ErrorTimeout: Process timeout
 */
en_result_t TIMER0_WriteCntReg(uint16_t u16Cnt)
{
    en_result_t enRet = Ok;
    uint32_t u32TimeOut = 0ul;
    M0P_TMR0->CNTAR = (uint32_t)u16Cnt;
    AsyncDelay();
    while((uint32_t)u16Cnt != M0P_TMR0->CNTAR)
    {
        if(u32TimeOut++ > TIMER0_TMOUT)
        {
            enRet = ErrorTimeout;
            break;
        }
    }
    return enRet;
}

/**
 * @brief  Get Timer0 base compare count register
 * @param  [in] None
 *
 * @retval The compare register data
 * @note   Macksure timer0 function is disable in asynchronous mode
 */
uint16_t TIMER0_GetCmpReg(void)
{
    return (uint16_t)M0P_TMR0->CMPAR&0xFFFFu;
}

/**
 * @brief  Write Timer0 compare count register
 * @param  [in] u16Cnt  The data to write to the compare register
 *
 * @retval Ok: Success
 * @retval ErrorTimeout: Process timeout
 */
en_result_t TIMER0_WriteCmpReg(uint16_t u16Cnt)
{
    en_result_t enRet = Ok;
    uint32_t u32TimeOut = 0ul;
    M0P_TMR0->CMPAR = (uint32_t)u16Cnt;
    AsyncDelay();
    while((uint32_t)u16Cnt != M0P_TMR0->CMPAR)
    {
        if(u32TimeOut++ > TIMER0_TMOUT)
        {
            enRet = ErrorTimeout;
            break;
        }
    }
    return enRet;
}

/**
 * @brief  Timer0 peripheral base function initialize
 * @param  [in] None
 *
 * @retval Ok: Success
 * @retval ErrorTimeout: Process timeout
 */
en_result_t TIMER0_DeInit(void)
{
    en_result_t enRet = Ok;
    uint32_t u32TimeOut = 0ul;
    M0P_TMR0->BCONR = 0x00000000ul;
    AsyncDelay();
    while(0x00000000ul != M0P_TMR0->BCONR)
    {
        if(u32TimeOut++ > TIMER0_TMOUT)
        {
            enRet = ErrorTimeout;
            break;
        }
    }
    M0P_TMR0->CMPAR = 0x0000FFFFul;
    M0P_TMR0->CNTAR = 0x00000000ul;
    M0P_TMR0->STFLR = 0x00000000ul;
    return enRet;
}

/**
 * @brief  Set external trigger source for Timer0
 * @param  [in] enEvent         External event source, @ref en_event_src_t
 *
 * @retval None
 */
void TIMER0_SetTriggerSrc(en_event_src_t enEvent)
{
    M0P_AOS->TMR0_HTSSR_f.TRGSEL = enEvent;  
}

/**
 * @}
 */

#endif /* DDL_TIMER0_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
