/**
 *******************************************************************************
 * @file  timer4/timer4_pwm_through_mode_occr_buf/source/main.c
 * @brief This example demonstrates how to use the through mode function of 
 *        Timer4 PWM function and enable OCCR buffer function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-25       Hongjh          First version
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
#include "hc32_ddl.h"

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup TIMER4_PWM_Through_Mode_With_OCCR_Buffer
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* TIM4 PWM Port/Pin definition */
#define TIM4_1_OXH_PORT                 (GPIO_PORT_6)
#define TIM4_1_OXH_PIN                  (GPIO_PIN_1)
#define TIM4_1_OXH_GPIO_FUNC            (GPIO_FUNC_4_TIM4)

/* Function clock gate definition */
#define FUNCTION_CLK_GATE               (CLK_FCG_TIM4)

/* Timer4 Counter period value && interrupt number definition */
#define TIMER4_CNT_CYCLE_VAL            ((uint16_t)(SystemCoreClock/512ul))    /* 1000 ms */
#define TIMERB_CNT_UDF_INT              (INT_TMR4_GUDF)
#define TIMERB_CNT_UDF_IRQn             (Int017_IRQn)

/* Timer4 OCO Channel definition */
#define TIMER4_OCO_HIGH_CH              (TIMER4_OCO_UH)    /* only TIMER4_OCO_UH  TIMER4_OCO_VH  TIMER4_OCO_WH */

#define TIMER4_OCO_OCCR_BUF_SIZE        (3u)

/* Timer4 PWM get channel by OCO high channel */
#define TIMER4_PWM_CH(x)                (((x) == TIMER4_OCO_UH) ? TIMER4_PWM_U : \
                                         (((x) == TIMER4_OCO_VH) ? TIMER4_PWM_V : TIMER4_PWM_W))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_u16OccrValIdx = 0u;
static uint16_t m_au16OccrVal[TIMER4_OCO_OCCR_BUF_SIZE];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Configure system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    /* Configure the system clock to HRC32MHz. */
    CLK_HRCInit(CLK_HRC_ON, CLK_HRCFREQ_32);
}

/**
 * @brief  TIMER4 Counter zero match interrupt handler callback.
 * @param  None
 * @retval None
 */
static void Timer4ZeroMatchIrqCb(void)
{
    TIMER4_CNT_ClearFlag(TIMER4_CNT_FLAG_ZERO);
    TIMER4_OCO_SetOccrVal(TIMER4_OCO_HIGH_CH, m_au16OccrVal[m_u16OccrValIdx]);

    if (++m_u16OccrValIdx >= TIMER4_OCO_OCCR_BUF_SIZE)
    {
        m_u16OccrValIdx = 0u;
    }
    else
    {
    }
}

/**
 * @brief  Main function of TIMER4 PWM through mode with OCCR buffer
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t u32PwmCh;
    stc_irq_regi_config_t stcIrqRegiConf;
    stc_timer4_cnt_init_t stcTimer4CntInit;
    stc_timer4_oco_init_t stcTimer4OcoInit;
    stc_timer4_pwm_init_t stcTimer4PwmInit;
    stc_oco_high_ch_compare_mode_t stcHighChCmpMode;

    /* Configure system clock. */
    SystemClockConfig();

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Initialize TIMER4 Counter */
    TIMER4_CNT_StructInit(&stcTimer4CntInit);
    stcTimer4CntInit.enZeroIntCmd = Enable;
    stcTimer4CntInit.u16ClkDiv = TIMER4_CNT_CLK_DIV512;
    /* Period_Value(1000ms) = SystemClock(SystemCoreClock) / TIMER4_CNT_Clock_Division(512) / Frequency(1) */
    stcTimer4CntInit.u16CycleVal = TIMER4_CNT_CYCLE_VAL;
    TIMER4_CNT_Init(&stcTimer4CntInit);

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = TIMERB_CNT_UDF_IRQn;
    stcIrqRegiConf.enIntSrc = TIMERB_CNT_UDF_INT;
    stcIrqRegiConf.pfnCallback = &Timer4ZeroMatchIrqCb;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Initialize TIMER4 OCO high channel */
    m_au16OccrVal[0] = 1u * (TIMER4_CNT_CYCLE_VAL / 4u);
    m_au16OccrVal[1] = 2u * (TIMER4_CNT_CYCLE_VAL / 4u);
    m_au16OccrVal[2] = 3u * (TIMER4_CNT_CYCLE_VAL / 4u);
    TIMER4_OCO_StructInit(&stcTimer4OcoInit);
    stcTimer4OcoInit.enOcoCmd = Enable;
    stcTimer4OcoInit.enOcoIntCmd = Disable;
    stcTimer4OcoInit.u16OcoInvalidOp = TIMER4_OCO_INVAILD_OP_LOW;
    stcTimer4OcoInit.u16OcmrBufMode = TIMER4_OCO_OCCR_BUF_CNT_PEAK;
    stcTimer4OcoInit.u16OccrVal = m_au16OccrVal[m_u16OccrValIdx++];
    TIMER4_OCO_Init(TIMER4_OCO_HIGH_CH, &stcTimer4OcoInit);

    if (!(TIMER4_OCO_HIGH_CH % 2ul))
    {
        /* OCMR[15:0] = 0x0FFF = b 0000 1111 1111 1111 */
        stcHighChCmpMode.OCMRx_f.OCFDCH = TIMER4_OCO_OCF_SET;   /* bit[0] 1 */
        stcHighChCmpMode.OCMRx_f.OCFPKH = TIMER4_OCO_OCF_SET;   /* bit[1] 1 */
        stcHighChCmpMode.OCMRx_f.OCFUCH = TIMER4_OCO_OCF_SET;   /* bit[2] 1 */
        stcHighChCmpMode.OCMRx_f.OCFZRH = TIMER4_OCO_OCF_SET;   /* bit[3] 1 */

        stcHighChCmpMode.OCMRx_f.OPDCH = TIMER4_OCO_OP_INVERT;  /* Bit[5:4]    11 */
        stcHighChCmpMode.OCMRx_f.OPPKH = TIMER4_OCO_OP_INVERT;  /* Bit[7:6]    11 */
        stcHighChCmpMode.OCMRx_f.OPUCH = TIMER4_OCO_OP_INVERT;  /* Bit[9:8]    11 */
        stcHighChCmpMode.OCMRx_f.OPZRH = TIMER4_OCO_OP_INVERT;  /* Bit[11:10]  11 */
        stcHighChCmpMode.OCMRx_f.OPNPKH = TIMER4_OCO_OP_HOLD;   /* Bit[13:12]  00 */
        stcHighChCmpMode.OCMRx_f.OPNZRH = TIMER4_OCO_OP_HOLD;   /* Bit[15:14]  00 */

        stcHighChCmpMode.enExtendMatchCondCmd = Disable;

        TIMER4_OCO_SetHighChCompareMode(TIMER4_OCO_HIGH_CH, &stcHighChCmpMode);  /* Set OCO high channel compare mode */
    }

    /* Initialize PWM I/O */
    GPIO_SetFunc(TIM4_1_OXH_PORT, TIM4_1_OXH_PIN, TIM4_1_OXH_GPIO_FUNC);

    /* Timer4 PWM: Get pwm couple channel */
    u32PwmCh = TIMER4_PWM_CH(TIMER4_OCO_HIGH_CH);

    /* Initialize Timer4 PWM */
    TIMER4_PWM_StructInit(&stcTimer4PwmInit);
    stcTimer4PwmInit.enRtIntMaskCmd = Enable;
    TIMER4_PWM_Init(u32PwmCh, &stcTimer4PwmInit);

    /* Start TIMER4 counter. */
    TIMER4_CNT_Start();

    /* Set OCO compare value */
    TIMER4_OCO_SetOccrVal(TIMER4_OCO_HIGH_CH, m_au16OccrVal[m_u16OccrValIdx++]);

    while (1)
    {
    }
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
