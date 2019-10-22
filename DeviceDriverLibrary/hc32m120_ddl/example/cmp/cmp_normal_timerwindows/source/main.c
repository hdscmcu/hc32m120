/**
 *******************************************************************************
 * @file  cmp/cmp_normal_timerwindows/source/main.c
 * @brief Main program of CMP for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-09       Wangmin         First version
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
 * @addtogroup CMP_Normal_TimerWindows
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CMP_TEST_UNIT                   (M0P_CMP1)

/* Define port and pin for CMP */
/* VCOUT1_B*/
#define CMP_VCOUT1_PORT                 (GPIO_PORT_7)
#define CMP_VCOUT1_PIN                  (GPIO_PIN_0)
/* VCOUT2_C*/
#define CMP_VCOUT2_PORT                 (GPIO_PORT_1)
#define CMP_VCOUT2_PIN                  (GPIO_PIN_1)
/* IVCMP1_0 */
#define VCMP1_0_PORT                    (GPIO_PORT_1)
#define VCMP1_0_PIN                     (GPIO_PIN_3)
/* IVCMP1_1 */
#define VCMP1_1_PORT                    (GPIO_PORT_2)
#define VCMP1_1_PIN                     (GPIO_PIN_0)
/* IREF1 */
#define IREF1_PORT                      (GPIO_PORT_1)
#define IREF1_PIN                       (GPIO_PIN_2)

/* TIMERB unit & interrupt number & counter period/compare value definition */
#define TIMERB_ODD_UNIT                 (M0P_TMRB1)
#define TIMERB_ODD_UNIT_CMP_INT         (INT_TMRB_1_CMP)
#define TIMERB_ODD_UNIT_CMP_IRQn        (Int022_IRQn)
#define TIMERB_ODD_UNIT_PERIOD_VALUE    ((SystemCoreClock/512ul))
#define TIMERB_ODD_UNIT_COMPARE_VALUE   ((TIMERB_ODD_UNIT_PERIOD_VALUE/2ul))
/* TIMERB TIMB_t_PWM1 Port/Pin definition */
#define TIMERB_ODD_UNIT_PWM1_PORT       (GPIO_PORT_1)     /* P15: TIMB_1_PWM1_D */
#define TIMERB_ODD_UNIT_PWM1_PIN        (GPIO_PIN_5)
/* Function clock gate definition  */
#define FUNCTION_CLK_GATE               (CLK_FCG_TIMB1 | CLK_FCG_TIMB2)

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
static stc_timerb_init_t m_stcTimerbInit = {
    .u16CntDir = TIMERB_CNT_UP,
    .u16ClkDiv = TIMERB_CLKDIV_DIV512,
    .u16CntMode = TIMERB_SAWTOOTH_WAVE,
    .u16OverflowAction = TIMERB_OVERFLOW_COUNT,
    .u16SynStartState = TIMERB_SYNC_START_ENABLE,
};

static stc_timerb_oc_init_t m_stcTimerbOddUnitOcInit = {
    .u16PortOutputState = TIMERB_OC_PORT_OUTPUT_ENABLE,
    .u16StartCntOutput = TIMERB_OC_STARTCNT_OUTPUT_HIGH,
    .u16StopCntOutput = TIMERB_OC_STOPCNT_OUTPUT_HIGH,
    .u16CompareMatchOutput = TIMERB_OC_CMPMATCH_OUTPUT_INVERTED,
    .u16PeriodMatchOutput = TIMERB_OC_PERIODMATCH_OUTPUT_HOLD,
};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function of cmp_normal_timerwindows project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_cmp_init_t stcCmpCfg;
    stc_pwc_pwrmon_init_t stcPwcIni;
    stc_cmp_timerwindows_t stcTimerWindowsIni;

    /* Configure system clock. */
    SystemClockConfig();

    /* Port function configuration for CMP*/
    GPIO_SetFunc(CMP_VCOUT1_PORT, CMP_VCOUT1_PIN, GPIO_FUNC_3_CMP);
    GPIO_SetFunc(VCMP1_0_PORT, VCMP1_0_PIN, GPIO_FUNC_1_IVCMP);
    //GPIO_SetFunc(IREF1_PORT, IREF1_PIN, GPIO_FUNC_1_IVCMP);

    /* Port function configuration for Timerb PWM */
    GPIO_SetFunc(TIMERB_ODD_UNIT_PWM1_PORT, TIMERB_ODD_UNIT_PWM1_PIN, GPIO_FUNC_2_TIMB);

    /* Enable peripheral clock for Timerb*/
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Initialize TIMERB odd unit. */
    m_stcTimerbInit.u16PeriodVal = (uint16_t)TIMERB_ODD_UNIT_PERIOD_VALUE;
    TIMERB_Init(TIMERB_ODD_UNIT, &m_stcTimerbInit);

    /* Initialize TIMERB even unit output compare function . */
    m_stcTimerbOddUnitOcInit.u16CompareVal = (uint16_t)TIMERB_ODD_UNIT_COMPARE_VALUE;
    TIMERB_OC_Init(TIMERB_ODD_UNIT, &m_stcTimerbOddUnitOcInit);
    /* Start TIMERB counter. */
    TIMERB_Start(TIMERB_ODD_UNIT);

    /* Enable internal Vref*/
    PWC_PwrMonStructInit(&stcPwcIni);
    PWC_PwrMonInit(&stcPwcIni);

    /* Configuration Timerb pwm function */
    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(FUNCTION_CLK_GATE, Enable);

    /* Enable peripheral clock for CMP*/
    CLK_FcgPeriphClockCmd(CLK_FCG_CMP, Enable);

    /* Clear structure */
    CMP_StructInit(&stcCmpCfg);
    /* De-initialize CMP unit */
    CMP_DeInit(CMP_TEST_UNIT);

    /* Configuration for normal compare function */
    stcCmpCfg.u8CmpVol = CMP1_CVSL_VCMP1_0;
    stcCmpCfg.u8RefVol = CMP1_RVSL_VREF;
    stcCmpCfg.u8OutDetectEdges = CMP_DETECT_EDGS_BOTH;
    stcCmpCfg.u8OutFilter = CMP_OUT_FILTER_PCLKDIV8;
    stcCmpCfg.u8OutPolarity = CMP_OUT_REVERSE_ON;
    CMP_NormalModeInit(CMP_TEST_UNIT, &stcCmpCfg);

    /* Enable interrupt if need */

    /* Enable VCOUT if need */
    CMP_VCOUTCmd(CMP_TEST_UNIT, Enable);

    /* Timer windows function configuration if need */
    stcTimerWindowsIni.u8TWMode = CMP_TIMERWIN_ON;
    stcTimerWindowsIni.u8TWSelect = CMP1_TIMERWIN_TIMB_1_PWM;
    stcTimerWindowsIni.u8TWOutLevel = CMP_TIMERWIN_OUT_LEVEL_HIGH;
    stcTimerWindowsIni.u8TWInvalidLevel = CMP_TIMERWIN_INVALID_LEVEL_HIGH;
    CMP_TimerWindowsCfg(CMP_TEST_UNIT, &stcTimerWindowsIni);

    /* Enable CMP output */
    CMP_OutputCmd(CMP_TEST_UNIT, Enable);

    /* Configuration finished */
    while(1)
    {
        ;
    }
}

/**
 * @brief  Configure system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_xtal_init_t stcXTALInit;

    /* Configure XTAL */
    stcXTALInit.u8XtalState = CLK_XTAL_ON;
    stcXTALInit.u8XtalMode = CLK_XTALMODE_OSC;
    stcXTALInit.u8XtalDrv = CLK_XTALDRV_HIGH;
    stcXTALInit.u8XtalSupDrv = CLK_XTAL_SUPDRV_OFF;
    stcXTALInit.u8XtalStb = CLK_XTALSTB_8;

    /* Initialize XTAL clock */
    CLK_XTALInit(&stcXTALInit);

    /* Switch system clock from HRC(default) to XTAL */
    CLK_SetSysclkSrc(CLK_SYSCLKSOURCE_XTAL);
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
