/**
 *******************************************************************************
 * @file  emb/emb_osc_brake_timer4/source/main.c
 * @brief This example demonstrates how to use OSC failure brake function of 
 *        EMB function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-02       Hongjh          First version
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
 * @addtogroup EMB_OSC_Brake_Timer4
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @brief Key state definition
 */
typedef enum
{
    KeyIdle,
    KeyRelease,
} en_key_state_t;

/**
 * @brief Key instance structure definition
 */
typedef struct
{
    uint8_t u8Port;                     /*!< GPIO_PORT_x: x can be (0~7, 12~14) to select the GPIO peripheral */

    uint8_t u8Pin;                      /*!< GPIO_PIN_x: x can be (0~7) to select the PIN index */

    en_pin_state_t enPressPinState;     /*!< Pin level state when key is pressed */
} stc_key_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* Function clock gate definition */
#define FUNCTION_CLK_GATE               (CLK_FCG_EMB | CLK_FCG_TIM4)

/* Timer4 Counter period value && interrupt number definition */
#define TIMER4_CNT_CYCLE_VAL            (SystemCoreClock/512ul/2ul)    /* 500 ms */

/* Key Port/Pin definition */
#define KEY_PORT                        (GPIO_PORT_2)
#define KEY_PIN                         (GPIO_PIN_1)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static en_key_state_t KeyGetState(const stc_key_t *pstcKey);
static void Timer4PwmConfig(void);
static void EmbIrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Configure system clock
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_xtal_init_t    stcXtalInit;
    stc_clk_xtalstd_init_t stcXtalstdInit;

    /* Configure the system clock to HRC32MHz. */
    CLK_HRCInit(CLK_HRC_ON, CLK_HRCFREQ_32);

    /* Config XTAL and Enable */
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalMode = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalDrv = CLK_XTALDRV_HIGH;
    stcXtalInit.u8XtalSupDrv = CLK_XTAL_SUPDRV_OFF;
    stcXtalInit.u8XtalStb = CLK_XTALSTB_8;
    CLK_XTALInit(&stcXtalInit);

    /* Enable xtal fault dectect and occur reset. */
    stcXtalstdInit.u8XtalStdState = CLK_XTALSTD_ON;
    stcXtalstdInit.u8XtalStdMode = CLK_XTALSTD_MODE_INT;
    stcXtalstdInit.u8XtalStdInt = CLK_XTALSTD_INT_ON;
    stcXtalstdInit.u8XtalStdRst = CLK_XTALSTD_RST_OFF;
    CLK_XTALStdInit(&stcXtalstdInit);
}

/**
 * @brief  Get key state
 * @param  [in] pstcKey    Pointer to stc_key_t structure
 * @retval An en_result_t enumeration value:
 *           - KeyIdle: Key isn't pressed
 *           - KeyRelease: Released after key is pressed
 */
static en_key_state_t KeyGetState(const stc_key_t *pstcKey)
{
    en_key_state_t enKeyState = KeyIdle;

    DDL_ASSERT(NULL != pstcKey);

    if (pstcKey->enPressPinState == GPIO_ReadInputPortPin(pstcKey->u8Port, pstcKey->u8Pin))
    {
        DDL_Delay1ms(20ul);

        if (pstcKey->enPressPinState == GPIO_ReadInputPortPin(pstcKey->u8Port, pstcKey->u8Pin))
        {
            while (pstcKey->enPressPinState == GPIO_ReadInputPortPin(pstcKey->u8Port, pstcKey->u8Pin))
            {
                ;
            }
            enKeyState = KeyRelease;
        }
    }

    return enKeyState;
}

/**
 * @brief  Configure Timer4 PWM
 * @param  None
 * @retval None
 */
static void Timer4PwmConfig(void)
{
    stc_timer4_cnt_init_t stcTimer4CntInit;
    stc_timer4_oco_init_t stcTimer4OcoInit;
    stc_timer4_pwm_init_t stcTimer4PwmInit;
    stc_oco_high_ch_compare_mode_t stcHighChCmpMode;
    stc_oco_low_ch_compare_mode_t stcLowChCmpMode;

    /* Initialize TIMER4 Counter */
    TIMER4_CNT_StructInit(&stcTimer4CntInit);
    stcTimer4CntInit.u16ClkDiv = TIMER4_CNT_CLK_DIV512;
    /* Period_Value(500ms) = SystemClock(SystemCoreClock) / TIMER4_CNT_Clock_Division(512) / Frequency(2) */
    stcTimer4CntInit.u16CycleVal = (uint16_t)TIMER4_CNT_CYCLE_VAL;
    TIMER4_CNT_Init(&stcTimer4CntInit);

    /* Initialize TIMER4 OCO high&&low channel */
    TIMER4_OCO_StructInit(&stcTimer4OcoInit);
    stcTimer4OcoInit.enOcoCmd = Enable;
    stcTimer4OcoInit.enOcoIntCmd = Enable;
    stcTimer4OcoInit.u16OcoInvalidOp = TIMER4_OCO_INVAILD_OP_LOW;
    stcTimer4OcoInit.u16OccrVal = (uint16_t)(TIMER4_CNT_CYCLE_VAL/2ul);
    TIMER4_OCO_Init(TIMER4_OCO_UH, &stcTimer4OcoInit);
    TIMER4_OCO_Init(TIMER4_OCO_UL, &stcTimer4OcoInit);

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
    TIMER4_OCO_SetHighChCompareMode(TIMER4_OCO_UH, &stcHighChCmpMode);  /* Set OCO high channel compare mode */

    /* OCMR[31:0] 0x0FF0 0FFF = b 0000 1111 1111 0000   0000 1111 1111 1111 */
    stcLowChCmpMode.OCMRx_f.OCFDCL = TIMER4_OCO_OCF_SET;    /* bit[0] 1 */
    stcLowChCmpMode.OCMRx_f.OCFPKL = TIMER4_OCO_OCF_SET;    /* bit[1] 1 */
    stcLowChCmpMode.OCMRx_f.OCFUCL = TIMER4_OCO_OCF_SET;    /* bit[2] 1 */
    stcLowChCmpMode.OCMRx_f.OCFZRL = TIMER4_OCO_OCF_SET;    /* bit[3] 1 */
    stcLowChCmpMode.OCMRx_f.OPDCL = TIMER4_OCO_OP_INVERT;   /* bit[5:4]    11 */
    stcLowChCmpMode.OCMRx_f.OPPKL = TIMER4_OCO_OP_INVERT;   /* bit[7:6]    11 */
    stcLowChCmpMode.OCMRx_f.OPUCL = TIMER4_OCO_OP_INVERT;   /* bit[9:8]    11 */
    stcLowChCmpMode.OCMRx_f.OPZRL = TIMER4_OCO_OP_INVERT;   /* bit[11:10]  11 */
    stcLowChCmpMode.OCMRx_f.OPNPKL = TIMER4_OCO_OP_HOLD;    /* bit[13:12]  00 */
    stcLowChCmpMode.OCMRx_f.OPNZRL = TIMER4_OCO_OP_HOLD;    /* bit[15:14]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNDCL = TIMER4_OCO_OP_HOLD;   /* bit[17:16]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNUCL = TIMER4_OCO_OP_HOLD;   /* bit[19:18]  00 */
    stcLowChCmpMode.OCMRx_f.EOPDCL = TIMER4_OCO_OP_INVERT;  /* bit[21:20]  11 */
    stcLowChCmpMode.OCMRx_f.EOPPKL = TIMER4_OCO_OP_INVERT;  /* bit[23:22]  11 */
    stcLowChCmpMode.OCMRx_f.EOPUCL = TIMER4_OCO_OP_INVERT;  /* bit[25:24]  11 */
    stcLowChCmpMode.OCMRx_f.EOPZRL = TIMER4_OCO_OP_INVERT;  /* bit[27:26]  11 */
    stcLowChCmpMode.OCMRx_f.EOPNPKL = TIMER4_OCO_OP_HOLD;   /* bit[29:28]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNZRL = TIMER4_OCO_OP_HOLD;   /* bit[31:30]  00 */
    stcLowChCmpMode.enExtendMatchCondCmd = Disable;
    TIMER4_OCO_SetLowChCompareMode(TIMER4_OCO_UL, &stcLowChCmpMode);  /* Set OCO low channel compare mode */

    /* Initialize PWM I/O */
    GPIO_SetFunc(GPIO_PORT_6, GPIO_PIN_1, GPIO_FUNC_4_TIM4);
    GPIO_SetFunc(GPIO_PORT_6, GPIO_PIN_0, GPIO_FUNC_2_TIM4);

    /* Initialize Timer4 PWM */
    TIMER4_PWM_StructInit(&stcTimer4PwmInit);
    stcTimer4PwmInit.enRtIntMaskCmd = Enable;
    stcTimer4PwmInit.u16PwmOutputPolarity = TIMER4_PWM_OP_OXH_HOLD_OXL_HOLD;
    TIMER4_PWM_Init(TIMER4_PWM_U, &stcTimer4PwmInit);
}

/**
 * @brief  EMB IRQ callback function.
 * @param  None
 * @retval None
 */
static void EmbIrqCallback(void)
{
    stc_key_t stcKeySw2 = {
        .u8Port = KEY_PORT,
        .u8Pin = KEY_PIN,
        .enPressPinState = Pin_Reset,
    };

    if(Set == EMB_GetStatus(EMB_FLAG_OSC))
    {
        while (KeyRelease != KeyGetState(&stcKeySw2))
        {
            ;
        }

        CLK_ClearXtalStdFlag();
        EMB_ClearStatus(EMB_FLAG_OSC);  /* Clear OSC Brake */
    }
}

/**
 * @brief  Main function of EMB OSC failure brake
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;
    stc_emb_ctrl_timer4_init_t stcEmbInit;

    /* Configure system clock. */
    SystemClockConfig();

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Configure Timer4 PWM. */
    Timer4PwmConfig();

    /* Configure Timer4 EMB. */
    TIMER4_EMB_SetPwmPortPolarity(TIMER4_EMB_TRIG_PWM_OP_LOW);

    /* Configure EMB. */
    EMB_StructInit(&stcEmbInit);
    stcEmbInit.enOscCmd = Enable;
    EMB_Init(&stcEmbInit);

    EMB_IntCmd(EMB_INT_OSC, Enable);

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegiConf.enIRQn = Int014_IRQn;
    stcIrqRegiConf.enIntSrc = INT_EMB_GR;
    stcIrqRegiConf.pfnCallback = &EmbIrqCallback;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Start TIMER4 counter. */
    TIMER4_CNT_Start();

    while (1u)
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
