/**
 *******************************************************************************
 * @file  lin.c
 * @brief This midware file provides firmware functions to manage the LIN.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-04       Hongjh          First version
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
#include "lin.h"

/**
 * @addtogroup HC32M120_DDL_Midware
 * @{
 */

/**
 * @defgroup MW_LIN LIN
 * @brief LIN Midware Library
 * @{
 */

#if (MW_LIN_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup LIN_Local_Macros LIN Local Macros
 * @{
 */

/* BIT operation */
#define BIT(x, pos)                             (((x) >> (pos)) & 0x01u)

/* LIN wakeup/break/sync data definition */
#define LIN_WAKEUP_DATA                         (0x80u)
#define LIN_BREAK_DATA                          (0x00u)
#define LIN_SYNC_DATA                           (0x55u)

/* LIN Timerb capture count/bits definition */
#define LIN_SYNC_CAPTURE_EDGE_CNT               (5u)
#define LIN_SYNC_CAPTURE_BITS                   (8u)

/* LIN Timerb period value definition */
#define LIN_TIMERB_UNIT_PERIOD_VALUE            (0xFFFFu)

/**
 * @defgroup LIN_Delay LIN Delay
 * @{
 */
#define LIN_INTER_FRAME_DELAY                   (10ul) // Pause (Frame->Frame)   (ca. 10ms)
#define LIN_FRAME_RESPONSE_DELAY                (2ul)  // Pause (Header->Data)   (ca.  2ms)
#define LIN_BREAKFIELD_DELAY                    (4ul)  // Pause (Breakfield)     (ca.  4ms)
#define LIN_DATA_BYTE_DELAY                     (1ul)  // Pause (Data->Data)     (ca.  1ms)
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
static en_result_t LIN_SendBreak(stc_lin_hanlde_t *pstcLinHandle);
static en_result_t LIN_SendChar(M0P_USART_TypeDef *USARTx, uint8_t u8Char);
static uint8_t LIN_CalcParity(uint8_t u8PID);
static en_result_t LIN_MASTER_SendFrameHeader(stc_lin_hanlde_t *pstcLinHandle,
                                            stc_lin_frame_t *pstcFrame);
static uint8_t LIN_SLAVE_GetFrameDataLenbyPID(uint8_t u8PID);
static void UsartDisableRxTX(stc_lin_hanlde_t *pstcLinHandle);
static void UsartEnableTX(stc_lin_hanlde_t *pstcLinHandle);
static void UartRxIrqCallback(void);
static void UartErrIrqCallback(void);
static uint32_t TimerbGetClk(const M0P_TMRB_TypeDef *TMRBx);
static void TimerbLinUnitCmpIrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_lin_hanlde_t *m_pstcLinHandle = NULL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup LIN_Global_Functions LIN Global Functions
 * @{
 */

/**
 * @brief  Initialize LIN master function.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_MASTER_Init(stc_lin_hanlde_t *pstcLinHandle)
{
    stc_exint_config_t stcExIntInit;
    stc_irq_regi_config_t stcIrqRegiConf;
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcLinHandle)
    {
        m_pstcLinHandle = pstcLinHandle;

        /* Configure USART RX/TX pin. */
        GPIO_SetFunc(pstcLinHandle->stcPinCfg.u8RxPort, pstcLinHandle->stcPinCfg.u8RxPin, GPIO_FUNC_5_USART);
        GPIO_SetFunc(pstcLinHandle->stcPinCfg.u8TxPort, pstcLinHandle->stcPinCfg.u8TxPin, GPIO_FUNC_5_USART);

        /* Enable peripheral clock */
        CLK_FcgPeriphClockCmd(CLK_FCG_UART1, Enable);

        /* Initialize LIN */
        USART_LinInit(M0P_USART1, &pstcLinHandle->stcLinInit);

        /* Register RX IRQ handler && configure NVIC. */
        stcIrqRegiConf.enIRQn = pstcLinHandle->stcIrqnCfg.UsartRxIRQn;
        stcIrqRegiConf.enIntSrc = INT_USART_1_RI;
        stcIrqRegiConf.pfnCallback = &UartRxIrqCallback;
        INTC_IrqRegistration(&stcIrqRegiConf);
        NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
        NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
        NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

        /* Register error IRQ handler && configure NVIC. */
        stcIrqRegiConf.enIRQn = pstcLinHandle->stcIrqnCfg.UsartErrIRQn;
        stcIrqRegiConf.enIntSrc = INT_USART_1_EI;
        stcIrqRegiConf.pfnCallback = &UartErrIrqCallback;
        INTC_IrqRegistration(&stcIrqRegiConf);
        NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
        NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
        NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

        /* EXINT Channel 1 configure */
        EXINT_StructInit(&stcExIntInit);
        stcExIntInit.u16ExIntCh = EXINT_CH01;
        stcExIntInit.u8ExIntFE = EXINT_FILTER_ON;
        stcExIntInit.u8ExIntFClk = EXINT_FCLK_HCLK_DIV8;
        stcExIntInit.u8ExIntLvl = EXINT_TRIGGER_RISING;
        EXINT_Init(&stcExIntInit);

        if (LinStateSleep == pstcLinHandle->enLinState)
        {
            LIN_Sleep(pstcLinHandle);
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Master send frame.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 */
en_result_t LIN_MASTER_SendFrame(stc_lin_hanlde_t *pstcLinHandle,
                                        stc_lin_frame_t *pstcFrame)
{
    uint8_t u8Checksum;
    en_result_t enRet = Ok;

    enRet = LIN_MASTER_SendFrameHeader(pstcLinHandle, pstcFrame);
    if (Ok == enRet)
    {
        DDL_Delay1ms(LIN_FRAME_RESPONSE_DELAY);

        /* Send data */
        for (pstcFrame->u8XferCnt = 0u; pstcFrame->u8XferCnt < pstcFrame->u8Length ; pstcFrame->u8XferCnt++)
        {
            LIN_SendChar(M0P_USART1, pstcFrame->au8Data[pstcFrame->u8XferCnt]);
            DDL_Delay1ms(LIN_DATA_BYTE_DELAY);
        }

        /* Calculate Checksum */
        u8Checksum = LIN_CalcChecksum(pstcFrame->u8PID, pstcFrame->au8Data, pstcFrame->u8Length);

        LIN_SendChar(M0P_USART1, u8Checksum);
        DDL_Delay1ms(LIN_INTER_FRAME_DELAY);

    }

    return enRet;
}

/**
 * @brief  LIN master receive frame(blocking mode).
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @param  [in] i32Timeout              Timeout ms
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 */
en_result_t LIN_MASTER_RecFrame(stc_lin_hanlde_t *pstcLinHandle,
                                        stc_lin_frame_t *pstcFrame,
                                        int32_t i32Timeout)
{
    uint8_t i;
    uint8_t u8Checksum;
    __IO uint32_t u32Cyc;
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcFrame != NULL)
    {
        enRet = LIN_MASTER_SendFrameHeader(pstcLinHandle, pstcFrame);
        if (Ok == enRet)
        {
            pstcFrame->u8XferCnt = 0u;
            pstcFrame->u8Checksum = 0u;
            pstcFrame->u8Error = 0u;
            for (i = 0u; i < 8u; i++)
            {
                pstcFrame->au8Data[i] = 0u;
            }

            /* Enable receive data */
            UsartDisableRxTX(pstcLinHandle);
            USART_FuncCmd(M0P_USART1, USART_RX | USART_INT_RX, Enable);

            u32Cyc = (i32Timeout < 0) ? 0ul : (uint32_t)i32Timeout * (SystemCoreClock / 10000ul);  /* i32Timeout * 1ms */

            /* Wait checksum */
            while (u32Cyc || (i32Timeout < 0))
            {
                u32Cyc--;
                if (LinFrameStateChecksum == pstcFrame->u8State)
                {
                    break;
                }
            }

            if ((0ul == u32Cyc) && (i32Timeout >= 0))
            {
                enRet = ErrorTimeout;
            }
            else
            {
                u8Checksum = LIN_CalcChecksum(pstcFrame->u8PID, pstcFrame->au8Data, pstcFrame->u8Length);

                if (u8Checksum != pstcFrame->u8Checksum)
                {
                    enRet = Error;
                }

                DDL_Delay1ms(LIN_INTER_FRAME_DELAY);
            }
        }
    }

    return enRet;
}

/**
 * @brief  Initialize LIN slave function.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_SLAVE_Init(stc_lin_hanlde_t *pstcLinHandle)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_exint_config_t stcExIntInit;

    stc_irq_regi_config_t stcIrqRegiConf;

    const stc_timerb_init_t stcTimerbInit = {
        .u16CntDir = TIMERB_CNT_UP,
        .u16ClkDiv = TIMERB_CLKDIV_DIV1,
        .u16CntMode = TIMERB_SAWTOOTH_WAVE,
        .u16PeriodVal = LIN_TIMERB_UNIT_PERIOD_VALUE,
        .u16OverflowAction = TIMERB_OVERFLOW_STOP,
    };

    const stc_timerb_ic_init_t stcTimerbIcInit = {
        .u16CaptureCondition = TIMERB_IC_RISING,
    };

    if (NULL != pstcLinHandle)
    {
        m_pstcLinHandle = pstcLinHandle;

        /* Configure USART RX/TX pin. */
        GPIO_SetFunc(pstcLinHandle->stcPinCfg.u8RxPort, pstcLinHandle->stcPinCfg.u8RxPin, GPIO_FUNC_5_USART);
        GPIO_SetFunc(pstcLinHandle->stcPinCfg.u8TxPort, pstcLinHandle->stcPinCfg.u8TxPin, GPIO_FUNC_5_USART);

        /* Enable peripheral clock */
        CLK_FcgPeriphClockCmd((CLK_FCG_UART1 | CLK_FCG_TIMB4), Enable);

        /* Initialize LIN function */
        USART_LinInit(M0P_USART1, &pstcLinHandle->stcLinInit);

        /* Register RX IRQ handler && configure NVIC. */
        stcIrqRegiConf.enIRQn = pstcLinHandle->stcIrqnCfg.UsartRxIRQn;
        stcIrqRegiConf.enIntSrc = INT_USART_1_RI;
        stcIrqRegiConf.pfnCallback = &UartRxIrqCallback;
        INTC_IrqRegistration(&stcIrqRegiConf);
        NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
        NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
        NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

        /* Register error IRQ handler && configure NVIC. */
        stcIrqRegiConf.enIRQn = pstcLinHandle->stcIrqnCfg.UsartErrIRQn;
        stcIrqRegiConf.enIntSrc = INT_USART_1_EI;
        stcIrqRegiConf.pfnCallback = &UartErrIrqCallback;
        INTC_IrqRegistration(&stcIrqRegiConf);
        NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
        NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
        NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

        /* Initialize TIMERB unit. */
        TIMERB_Init(M0P_TMRB4, &stcTimerbInit);

        /* Initialize TIMERB unit input capture function . */
        TIMERB_IC_Init(M0P_TMRB4, &stcTimerbIcInit);
        TIMERB_IntCmd(M0P_TMRB4, TIMERB_IT_CMP, Enable);

        /* Register IRQ handler && configure NVIC. */
        stcIrqRegiConf.enIRQn = pstcLinHandle->stcIrqnCfg.TimerbCmpIRQn;
        stcIrqRegiConf.enIntSrc = INT_TMRB_4_CMP;
        stcIrqRegiConf.pfnCallback = &TimerbLinUnitCmpIrqCallback;
        INTC_IrqRegistration(&stcIrqRegiConf);
        NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
        NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
        NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

        /* EXINT Channel 1 configure */
        EXINT_StructInit(&stcExIntInit);
        stcExIntInit.u16ExIntCh = EXINT_CH01;
        stcExIntInit.u8ExIntFE = EXINT_FILTER_ON;
        stcExIntInit.u8ExIntFClk = EXINT_FCLK_HCLK_DIV8;
        stcExIntInit.u8ExIntLvl = EXINT_TRIGGER_RISING;
        EXINT_Init(&stcExIntInit);

        if (LinStateSleep == pstcLinHandle->enLinState)
        {
            LIN_Sleep(pstcLinHandle);
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  LIN slave receive frame header(blocking mode).
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @param  [in] i32Timeout              Timeout duration (unit: ms)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 */
en_result_t LIN_SLAVE_RecFrameHeader(stc_lin_hanlde_t *pstcLinHandle,
                                        stc_lin_frame_t *pstcFrame,
                                        int32_t i32Timeout)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t u32Cyc = (i32Timeout < 0) ? 0u : ((uint32_t)i32Timeout) * (SystemCoreClock / 10000ul);  /* i32Timeout * 1ms */

    if ((pstcFrame != NULL) &&
        (pstcLinHandle != NULL))
    {
        pstcFrame->u8PID = 0u;
        pstcFrame->u8Error = 0u;
        pstcFrame->u8Length = 0u;
        pstcFrame->u8XferCnt = 0u;
        pstcFrame->u8Checksum = 0u;
        pstcFrame->u8State = LinFrameStateIdle;

        pstcLinHandle->pstcFrame = pstcFrame;

        /* Data state */
        UsartDisableRxTX(pstcLinHandle);

        TIMERB_SetCounter(M0P_TMRB4, 0u);
        TIMERB_SetHwTriggerCondition(M0P_TMRB4, TIMERB_HWSTART_TIMB_T_PWM1_FALLING | TIMERB_HWSTOP_TIMB_T_PWM1_RISING | TIMERB_HWCLEAR_TIMB_T_PWM1_FALLING);
        TIMERB_IC_SetCaptureCondition(M0P_TMRB4, TIMERB_IC_RISING);

        while (u32Cyc || (i32Timeout < 0))
        {
            u32Cyc--;
            if (pstcFrame->u8State >= LinFrameStatePID)
            {
                break;
            }
        }

        if ((0ul == u32Cyc) && (i32Timeout >= 0))
        {
            enRet = ErrorTimeout;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  LIN slave receive frame response data(blocking mode).
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @param  [in] i32Timeout              Timeout duration (unit: ms)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 * @note   Firstly call LIN_SLAVE_RecFrameHeader(), and then call this function.
 */
en_result_t LIN_SLAVE_RecFrameResponse(const stc_lin_hanlde_t *pstcLinHandle,
                                        stc_lin_frame_t *pstcFrame,
                                        int32_t i32Timeout)
{
    uint8_t u8Checksum = 0u;
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t u32Cyc = (i32Timeout < 0) ? 0u : ((uint32_t)i32Timeout) * (SystemCoreClock / 10000ul);  /* i32Timeout * 1ms */

    if ((pstcFrame != NULL) &&
        (pstcLinHandle != NULL))
    {
        if (pstcFrame->u8State == LinFrameStatePID)
        {
            while (u32Cyc || (i32Timeout < 0))
            {
                u32Cyc--;

                if (LinFrameStateChecksum == pstcFrame->u8State)
                {
                    u8Checksum = LIN_CalcChecksum(pstcFrame->u8PID, pstcFrame->au8Data, pstcFrame->u8Length);
                    if (u8Checksum == pstcFrame->u8Checksum)
                    {
                        pstcFrame->u8State = LinFrameStateIdle;
                    }
                    else
                    {
                        enRet = Error;
                        pstcFrame->u8Error = LinErrChecksum;
                    }

                    break;
                }
            }

            if ((0ul == u32Cyc) && (i32Timeout >= 0))
            {
                enRet = ErrorTimeout;
            }

            enRet = Ok;
        }
    }

    return enRet;
}

/**
 * @brief  LIN slave send frame response data.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 * @note   Firstly call LIN_SLAVE_RecFrameHeader(), and then call this function.
 */
en_result_t LIN_SLAVE_SendFrameResponse(stc_lin_hanlde_t *pstcLinHandle,
                                        stc_lin_frame_t *pstcFrame)
{
    uint8_t u8Checksum;
    en_result_t enRet = ErrorInvalidParameter;

    if (!((pstcFrame == NULL) ||
        (pstcLinHandle == NULL) ||
        (pstcLinHandle->pstcFrame == NULL) ||
        (pstcLinHandle->pstcFrame->u8PID == 0xFFu) ||
        (pstcLinHandle->pstcFrame->u8PID == 0x00u)))
    {
        if (pstcLinHandle->pstcFrame->u8State == LinFrameStatePID)
        {
            pstcLinHandle->pstcFrame = pstcFrame;

            UsartDisableRxTX(pstcLinHandle);
            UsartEnableTX(pstcLinHandle);

            DDL_Delay1ms(LIN_FRAME_RESPONSE_DELAY);

            /* Send data */
            for (pstcFrame->u8XferCnt = 0u; pstcFrame->u8XferCnt < pstcFrame->u8Length ; pstcFrame->u8XferCnt++)
            {
                LIN_SendChar(M0P_USART1, pstcFrame->au8Data[pstcFrame->u8XferCnt]);
                DDL_Delay1ms(LIN_DATA_BYTE_DELAY);
            }

            /* Calculate Checksum */
            u8Checksum = LIN_CalcChecksum(pstcFrame->u8PID, pstcFrame->au8Data, pstcFrame->u8Length);

            LIN_SendChar(M0P_USART1, u8Checksum);
            enRet = Ok;
        }
    }

    return enRet;
}

/**
 * @brief  Set LIN state.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] enState                 LIN state
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_SetState(stc_lin_hanlde_t *pstcLinHandle,
                            en_lin_state_t enState)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcLinHandle != NULL)
    {
        pstcLinHandle->enLinState = enState;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Wait wakeup signal.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_lin_state_t LIN_GetState(const stc_lin_hanlde_t *pstcLinHandle)
{
    DDL_ASSERT(pstcLinHandle != NULL);

    return pstcLinHandle->enLinState;
}

/**
 * @brief  LIN sleep.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_Sleep(stc_lin_hanlde_t *pstcLinHandle)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcLinHandle != NULL)
    {
        pstcLinHandle->enLinState = LinStateSleep;

        /* Clear flag */
        EXINT_ClrExIntSrc(EXINT_CH01);

        /* Configure NVIC */
        NVIC_ClearPendingIRQ(EXINT01_IRQn);
        NVIC_SetPriority(EXINT01_IRQn, DDL_IRQ_PRIORITY_03);
        NVIC_EnableIRQ(EXINT01_IRQn);

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  LIN send wakeup signal.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_SendWakeupSignal(stc_lin_hanlde_t *pstcLinHandle)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcLinHandle != NULL)
    {
        UsartEnableTX(pstcLinHandle);

        USART_SendData(M0P_USART1, LIN_WAKEUP_DATA);

        while (!USART_GetFlag(M0P_USART1, USART_FLAG_TC))
        {
            ;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Calculate PID && data field checksum.
 * @param  [in] u8PID                   PID number
 * @param  [in] au8Data                 Data buffer
 * @param  [in] u8Len                   Data length
 * @retval Checksum
 */
uint8_t LIN_CalcChecksum(uint8_t u8PID, const uint8_t au8Data[], uint8_t u8Len)
{
    uint8_t i;
    uint16_t u16Checksum = 0u;

    if ((u8PID - 0x3Cu) || (u8PID - 0x3Du))  /* 0x3C 0x3D Classic Checksum */
    {
        u16Checksum = 0u;
    }
    else /* Enhanced Checksum */
    {
        u16Checksum  = (uint16_t)u8PID ;
    }

    for (i = 0u; i < u8Len; i++)
    {
        u16Checksum += au8Data[i];

        if (u16Checksum > 0xFFu)      /* Carry bit */
        {
            u16Checksum -= 0xFFu;
        }
    }

    return (uint8_t)((uint16_t)(~u16Checksum));  /* reversed */
}

/**
 * @brief  Master send break field.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
static en_result_t LIN_SendBreak(stc_lin_hanlde_t *pstcLinHandle)
{
    uint32_t u32Baudrate = 0ul;

    while (!USART_GetFlag(M0P_USART1, USART_FLAG_TC))
    {
        ;
    }

    u32Baudrate = pstcLinHandle->stcLinInit.u32Baudrate * 9ul / 13ul;

    UsartDisableRxTX(pstcLinHandle);
    USART_SetBaudrate(M0P_USART1, u32Baudrate, NULL);
    UsartEnableTX(pstcLinHandle);

    USART_SendData(M0P_USART1, LIN_BREAK_DATA);
    while (!USART_GetFlag(M0P_USART1, USART_FLAG_TC))
    {
        ;
    }

    UsartDisableRxTX(pstcLinHandle);
    USART_SetBaudrate(M0P_USART1, pstcLinHandle->stcLinInit.u32Baudrate, NULL);
    UsartEnableTX(pstcLinHandle);

    return Ok;
}

/**
 * @brief  Lin send character.
 * @param  [in] USARTx                  USART instance
 * @param  [in] u8Char                  Character
 * @retval An en_result_t enumeration value:
 *           - Ok: success
 */
static en_result_t LIN_SendChar(M0P_USART_TypeDef *USARTx, uint8_t u8Char)
{
    USART_SendData(USARTx, (uint16_t)u8Char);

    while (!USART_GetFlag(USARTx, USART_FLAG_TC))
    {
        ;
    }

    return Ok;
}

/**
 * @brief  Calculate PID field parity.
 * @param  [in] u8PID           PID number
 * @retval Parity
 */
static uint8_t LIN_CalcParity(uint8_t u8PID)
{
    uint8_t u8P0;
    uint8_t u8P1;
    uint8_t u8Parity;

    u8Parity = u8PID;
    u8P0 = (  BIT(u8Parity,0) ^ BIT(u8Parity,1) ^ BIT(u8Parity,2) ^ BIT(u8Parity,4))  << 6;
    u8P1 = (!(BIT(u8Parity,1) ^ BIT(u8Parity,3) ^ BIT(u8Parity,4) ^ BIT(u8Parity,5))) << 7;
    u8Parity |= (u8P0 | u8P1);

    return u8Parity;
}

/**
 * @brief  Master send frame header field.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 */
static en_result_t LIN_MASTER_SendFrameHeader(stc_lin_hanlde_t *pstcLinHandle,
                                            stc_lin_frame_t *pstcFrame)
{
    uint8_t u8PID;
    en_result_t enRet = ErrorInvalidParameter;

    if ((pstcFrame != NULL)  &&
        (pstcLinHandle != NULL) &&
        (pstcFrame->u8PID != 0xFFu) &&
        (pstcFrame->u8PID != 0x00u))
    {
        pstcLinHandle->pstcFrame = pstcFrame;

        /* Idle state */
        if (USART_GetFlag(M0P_USART1, USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_ORE))
        {
            enRet = Error;
        }
        else
        {
            UsartEnableTX(pstcLinHandle);

            /* Send break field */
            LIN_SendBreak(pstcLinHandle);
            pstcFrame->u8State = LinFrameStateBreak;
            DDL_Delay1ms(LIN_BREAKFIELD_DELAY);

            /* Send sync field */
            LIN_SendChar(M0P_USART1, LIN_SYNC_DATA);
            pstcFrame->u8State = LinFrameStateSync;
            DDL_Delay1ms(LIN_DATA_BYTE_DELAY);

            /* Calculate PID */
            u8PID = LIN_CalcParity(pstcFrame->u8PID);

            /* Send PID */
            LIN_SendChar(M0P_USART1, u8PID);
            pstcFrame->u8State = LinFrameStatePID;

            enRet = Ok;
        }
    }

    return enRet;
}

/**
 * @brief  LIN slave get frame data length by PID.
 * @param  [in] u8PID                   Frame PID
 * @retval Frame data length
 */
static uint8_t LIN_SLAVE_GetFrameDataLenbyPID(uint8_t u8PID)
{
    uint8_t u8DataLen = 0u;

    switch (u8PID & 0x30u)
    {
        case 0x00u:
        case 0x10u:
            u8DataLen = 2u;
            break;
        case 0x20u:
            u8DataLen = 4u;
            break;
        case 0x30u:
            u8DataLen = 8u;
            break;
        default:
            break;
    }

    return u8DataLen;
}

/**
 * @brief  Diable USART TX/RX
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval None
 */
static void UsartDisableRxTX(stc_lin_hanlde_t *pstcLinHandle)
{
    USART_FuncCmd(M0P_USART1, USART_TX | USART_RX, Disable);
}

/**
 * @brief  USART RX IRQ callback
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval None
 */
static void UsartEnableTX(stc_lin_hanlde_t *pstcLinHandle)
{
    USART_FuncCmd(M0P_USART1, USART_TX, Enable);
}

/**
 * @brief  USART RX IRQ callback
 * @param  None
 * @retval None
 */
static void UartRxIrqCallback(void)
{
    uint8_t u8Data = (uint8_t)USART_RecData(M0P_USART1);

    switch (m_pstcLinHandle->pstcFrame->u8State)
    {
        case LinFrameStateSync:
            if (u8Data == LIN_CalcParity(u8Data & 0x3Fu))
            {
                m_pstcLinHandle->pstcFrame->u8PID = u8Data;
                m_pstcLinHandle->pstcFrame->u8Length = LIN_SLAVE_GetFrameDataLenbyPID(u8Data);
                m_pstcLinHandle->pstcFrame->u8State = LinFrameStatePID;
            }
            else
            {
                m_pstcLinHandle->pstcFrame->u8Error = LinErrPID;
            }
            break;
        case LinFrameStatePID:
            if (m_pstcLinHandle->pstcFrame->u8XferCnt < m_pstcLinHandle->pstcFrame->u8Length)
            {
                m_pstcLinHandle->pstcFrame->au8Data[m_pstcLinHandle->pstcFrame->u8XferCnt++] = u8Data;
            }

            if (m_pstcLinHandle->pstcFrame->u8XferCnt == m_pstcLinHandle->pstcFrame->u8Length)
            {
                m_pstcLinHandle->pstcFrame->u8State = LinFrameStateData;
            }
            break;
        case LinFrameStateData:
            m_pstcLinHandle->pstcFrame->u8Checksum = u8Data;
            m_pstcLinHandle->pstcFrame->u8State = LinFrameStateChecksum;
            break;
        default:
            break;
    }
}

/**
 * @brief  USART error IRQ callback.
 * @param  None
 * @retval None
 */
static void UartErrIrqCallback(void)
{
    USART_ClearFlag(M0P_USART1, (USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_ORE));
}

/**
 * @brief  Get TIMERB clock.
 * @param  [in] TMRBx                   TIMERB instance
 * @retval None
 */
static uint32_t TimerbGetClk(const M0P_TMRB_TypeDef *TMRBx)
{
    uint32_t u16ClkDiv = 0u;

    u16ClkDiv = TIMERB_GetClkDiv(M0P_TMRB4);

    return (SystemCoreClock >> (u16ClkDiv >> TMRB_BCSTR_CKDIV_POS));
}

/**
 * @brief  TIMERB LIN unit compare match IRQ callback.
 * @param  None
 * @retval None
 */
static void TimerbLinUnitCmpIrqCallback(void)
{
    uint16_t u16Bit = 0u;

    static uint16_t u16CmpVal = 0u;
    static __IO uint32_t m_u32Clk = 0ul;
    static __IO uint32_t m_u32Baudrate = 0ul;
    static __IO uint32_t m_u32SyncCaptureCnt = 0ul;
    static uint32_t m_u32SyncCaptureVal = 0ul;

    u16CmpVal = TIMERB_GetCompare(M0P_TMRB4);

    if (LinFrameStateIdle == m_pstcLinHandle->pstcFrame->u8State)
    {
        m_u32SyncCaptureCnt = 0ul;
        m_u32SyncCaptureVal = 0ul;
        m_u32Clk = TimerbGetClk(M0P_TMRB4);
        u16Bit= (uint16_t)((m_pstcLinHandle->stcLinInit.u32Baudrate * u16CmpVal * 10ul) / m_u32Clk);
        if (u16Bit > 110u)
        {
            TIMERB_SetCounter(M0P_TMRB4, 0u);
            TIMERB_SetHwStopCondition(M0P_TMRB4, TIMERB_HWSTOP_INVALID);
            TIMERB_IC_SetCaptureCondition(M0P_TMRB4, TIMERB_IC_FALLING);
            m_pstcLinHandle->pstcFrame->u8State = LinFrameStateBreak;
        }
        else
        {
            m_pstcLinHandle->pstcFrame->u8Error = LinErrBreak;
        }
    }
    else if (LinFrameStateBreak == m_pstcLinHandle->pstcFrame->u8State)
    {
        m_u32SyncCaptureVal += u16CmpVal;

        if (LIN_SYNC_CAPTURE_EDGE_CNT == ++m_u32SyncCaptureCnt)
        {
            TIMERB_Stop(M0P_TMRB4);
            TIMERB_IC_SetCaptureCondition(M0P_TMRB4, TIMERB_IC_INVALID);
            TIMERB_SetHwTriggerCondition(M0P_TMRB4, TIMERB_HWSTART_INVALID | TIMERB_HWSTOP_INVALID | TIMERB_HWCLEAR_INVALID);

            m_u32Baudrate = m_u32Clk / ((m_u32SyncCaptureVal + LIN_SYNC_CAPTURE_BITS) /LIN_SYNC_CAPTURE_BITS);
            USART_SetBaudrate(M0P_USART1, m_u32Baudrate, NULL);
            USART_FuncCmd(M0P_USART1, USART_RX | USART_INT_RX, Enable);

            m_pstcLinHandle->pstcFrame->u8State = LinFrameStateSync;
        }
    }
    else
    {
        /* Do nothing */
    }

    TIMERB_ClearFlag(M0P_TMRB4, TIMERB_FLAG_CMP);
}

/**
 * @brief  External interrupt CH.1 ISR handler
 * @param  None
 * @retval None
 */
void EXINT01_Handler(void)
{
    if (Set == EXINT_GetExIntSrc(EXINT_CH01))
    {
        NVIC_DisableIRQ(EXINT01_IRQn);
        m_pstcLinHandle->enLinState = LinStateWakeup;
    }

    EXINT_ClrExIntSrc(EXINT_CH01);
}

/**
 * @}
 */

#endif /* MW_LIN_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
