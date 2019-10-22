/**
 *******************************************************************************
 * @file  cmp/cmp_normal_int/source/main.c
 * @brief Main program of CMP for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-08       Wangmin         First version
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
 * @addtogroup CMP_Normal_Int
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
//#define CMP_VCOUT1_PORT                 GPIO_PORT_7
//#define CMP_VCOUT1_PIN                  GPIO_PIN_0
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

/* Define for RGB LED */
#define LED_R_PORT                      (GPIO_PORT_12)
#define LED_G_PORT                      (GPIO_PORT_7)
#define LED_B_PORT                      (GPIO_PORT_3)
#define LED_R_PIN                       (GPIO_PIN_0)
#define LED_G_PIN                       (GPIO_PIN_0)
#define LED_B_PIN                       (GPIO_PIN_1)

#define LED_B_Set()                     (GPIO_SetPins(LED_B_PORT, LED_B_PIN))
#define LED_B_Reset()                   (GPIO_ResetPins(LED_B_PORT, LED_B_PIN))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void LedConfig(void);
static void CMP_Irq_Callback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of cmp_normal_int project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_cmp_init_t stcCmpCfg;
    stc_pwc_pwrmon_init_t stcPwcIni;
    stc_irq_regi_config_t stcIrqRegiCfg;

    /* Configure system clock. */
    SystemClockConfig();

    /* RGB LED configuration */
    LedConfig();

    /* Port function configuration */
    GPIO_SetFunc(VCMP1_0_PORT, VCMP1_0_PIN, GPIO_FUNC_1_IVCMP);
    //GPIO_SetFunc(IREF1_PORT, IREF1_PIN, GPIO_FUNC_1_IVCMP);

    /* Enable internal Vref*/
    PWC_PwrMonStructInit(&stcPwcIni);
    PWC_PwrMonInit(&stcPwcIni);

    /* Enable peripheral clock */
    CLK_FcgPeriphClockCmd(CLK_FCG_CMP, Enable);

    /* Clear structure */
    CMP_StructInit(&stcCmpCfg);
    /* De-initialize CMP unit */
    CMP_DeInit(CMP_TEST_UNIT);

    /* Configuration for normal compare function */
    stcCmpCfg.u8CmpVol = CMP1_CVSL_VCMP1_0;
    stcCmpCfg.u8RefVol = CMP1_RVSL_VREF;
    stcCmpCfg.u8OutDetectEdges = CMP_DETECT_EDGS_BOTH;
    stcCmpCfg.u8OutFilter = CMP_OUT_FILTER_PCLKDIV32;
    stcCmpCfg.u8OutPolarity = CMP_OUT_REVERSE_ON;
    CMP_NormalModeInit(CMP_TEST_UNIT, &stcCmpCfg);

    /* Enable interrupt if need */
    CMP_IntCmd(CMP_TEST_UNIT, Enable);

    /* Enable VCOUT if need */
    //CMP_VCOUTCmd(CMP_TEST_UNIT, Enable);

    /* Timer windows function configuration if need */

    /* Enable CMP output */
    CMP_OutputCmd(CMP_TEST_UNIT, Enable);

    /*NVIC configuration for interrupt */
    stcIrqRegiCfg.enIRQn = Int020_IRQn;
    stcIrqRegiCfg.enIntSrc = INT_CMP_1_IRQ;
    stcIrqRegiCfg.pfnCallback = &CMP_Irq_Callback;
    INTC_IrqRegistration(&stcIrqRegiCfg);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /* Configuration finished */
    while(1)
    {
        ;
    }
}

/**
 * @brief  CMP interrupt call back
 * @param  None
 * @retval None
 */
static void CMP_Irq_Callback(void)
{
    en_flag_status_t stdflag;
    CMP_ResultGet(CMP_TEST_UNIT, &stdflag);

    (Set == stdflag) ? LED_B_Set() : LED_B_Reset();
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
 * @brief  Configure RGB LED.
 * @param  None
 * @retval None
 */
static void LedConfig(void)
{
    stc_gpio_init_t stcGpioInit = {0};

    stcGpioInit.u16PinMode = PIN_MODE_OUT;
    stcGpioInit.u16PinState = PIN_STATE_SET;
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
    GPIO_Init(LED_B_PORT, LED_B_PIN, &stcGpioInit);
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
