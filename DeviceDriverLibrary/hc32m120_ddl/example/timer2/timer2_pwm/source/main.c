/**
 *******************************************************************************
 * @file  timer2/timer2_pwm/source/main.c
 * @brief Main program TIMER2 pwm for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-20       Wuze            First version
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
 * @addtogroup Timer2_Pwm
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Compare value definition depends on PWM frequency(Hz).
   CompareVal = TIMER2ClockFrequency(Hz) / PwmFrequency(Hz) / 2.
   In this example:
   TIMER2ClockFrequency = HCLK(32MHz) / Timer2ClockPrescaler(16) = 2000000Hz;
   PwmFrequency = 2000Hz;
   CompareVal = 2000000 / 2000 / 2 = 500. */
#define TIMER2_CLK_PRESCALER        (TIMER2_CLK_PRESCALER_16)
#define TIMER2_COMPARE_VALUE        (500u)

/* PWM pin definition.
   P00, P01, P30, P31, P41 and P50 can be used as PWM output pin. */
#define TIMER2_PWM_PORT             (GPIO_PORT_3)
#define TIMER2_PWM_PIN              (GPIO_PIN_0)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void Timer2Config(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_pwm project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Configures the system clock to HRC32MHz. */
    SystemClockConfig();

    /* Configures TIMER2. */
    Timer2Config();

    /* Call TIMER2_Start to start the PWM output. */
    TIMER2_Start();

    /***************** Configuration end, application start **************/

    while (1u)
    {
        /* Call TIMER2_Stop to stop the PWM output. */
        // TIMER2_Stop();
    }
}

/**
 * @brief  Configures a new system clock -- HRC32MHz.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    /* Set EFM read latency when system clock greater than 24MHz. */
    EFM_SetLatency(EFM_LATENCY_1);

    /* Configure the system clock to HRC32MHz. */
    CLK_HRCInit(CLK_HRC_ON, CLK_HRCFREQ_32);
}

/**
 * @brief  TIMER2 configuration.
 * @param  None
 * @retval None
 */
static void Timer2Config(void)
{
    stc_timer2_config_t stcCfg;

    /* 1. Enable TIMER2 peripheral clock. */
    CLK_FcgPeriphClockCmd(CLK_FCG_TIM2, Enable);

    /* 2. Set a default configuration value for stcCfg.
          Default configuration values for basic timer is:
            Clock sourc(u16ClkSource): TIMER2_SYNC_CS_HCLK, default clock is HCLK.
            Clock prescaler(u16ClkPrescaler): TIMER2_CLK_PRESCALER_1, prescaler of clock source is 1.
            Start condition(u16HwStartCondition): TIMER2_HW_START_INVALID, do not use the start condition.
            Stop condition(u16HwStopCondition): TIMER2_HW_STOP_INVALID, do not use the stop condition.
            Start polarity(u16PwmStartPolarity): TIMER2_PWM_START_LOW, PWM start polarity is low.
            Stop polarity(u16PwmStopPolarity): TIMER2_PWM_START_LOW, PWM stop polarity is low.
          All of the above configuration values can be modified based on the application.
          NOTE: Start condition and stop condition CAN NOT be the edge of TRIGA. */
    TIMER2_StructInit(&stcCfg);

    /* 3. Modify the configuration value depends on the application. */
    stcCfg.u16ClkPrescaler = TIMER2_CLK_PRESCALER;
    stcCfg.u16CompareVal   = TIMER2_COMPARE_VALUE;

    /* 4. Configures TIMER2 according the configuration value. */
    TIMER2_PwmConfig(&stcCfg);

    /* 5. Set the GPIO pin as PWM output pin. */
    GPIO_SetFunc(TIMER2_PWM_PORT, TIMER2_PWM_PIN, GPIO_FUNC_3_TIM2);
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
