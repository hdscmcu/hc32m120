/**
 *******************************************************************************
 * @file  timer2/timer2_base_counter/source/main.c
 * @brief Main program TIMER2 base counter for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-20       Wuze            First version
   2019-10-21       Wuze            Modified the pin of SW1 from P17 to P22.
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
 * @addtogroup Timer2_Base_Counter
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* If use interrupt. */
#define TIMER2_USE_INTERRUPT        (1u)

/* Synchronous clock source definition. */
#define SYNC_CLK_EVENT              (0u)
#define SYNC_CLK_TRIGA              (1u)
#define TIMER2_SYNC_CLK             (SYNC_CLK_EVENT)

#if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)
#define TIMER2_SYNC_CLK_SRC         (TIMER2_SYNC_CS_EVENT)
/* SW1 definition. Press SW1 to generating the event EVT_PORT_EIRQ2. */
#define SW1_PORT                    (GPIO_PORT_2)
#define SW1_PIN                     (GPIO_PIN_2)
#define SW1_EXINT_CH                (EXINT_CH02)
#define SW1_EVENT                   (EVT_PORT_EIRQ2)

#define TIMER2_SYNC_CLK_EVENT       (SW1_EVENT)

#elif (TIMER2_SYNC_CLK == SYNC_CLK_TRIGA)
#define TIMER2_SYNC_CLK_SRC         (TIMER2_SYNC_CS_TRIGA_FALL) /* TIMER2_SYNC_CS_TRIGA_RISE is also can be used. */

#else
#error "Function not defined!!!"
#endif // #if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)

/* TRIGA pin definition.
   One of these pins P00, P01, P30, P31, P41 and P50 can be used as TRIGA input pin. */
#define TIMER2_TRIGA_PORT           (GPIO_PORT_3)
#define TIMER2_TRIGA_PIN            (GPIO_PIN_0)

/* Count times definition. */
#define TIMER2_COUNT_TIMES          (5u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);

#if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)
static void EventPinInit(void);
#endif

static void Timer2Config(void);

#if TIMER2_USE_INTERRUPT
static void Timer2IrqConfig(void);
#endif

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
#if TIMER2_USE_INTERRUPT
static uint8_t m_u8Timer2IrqFlag = 0u;
#endif

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_base_counter project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Configure the system clock to HRC32MHz. */
    SystemClockConfig();

#if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)
    EventPinInit();
#endif

    /* Configures TIMER2. */
    Timer2Config();

    /* Starts TIMER2. */
    TIMER2_Start();

    /***************** Configuration end, application start **************/

    while (1u)
    {
#if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)
        /* Press SW1 to generate event EVT_PORT_EIRQ2, the counter will count the number of presses. */
#else
        /* Make falling edge on P30. */
#endif

#if TIMER2_USE_INTERRUPT
        /* Check the IRQ flag. */
        if (m_u8Timer2IrqFlag != 0u)
        {
            /* The counter has reached the set number of counts. */
            m_u8Timer2IrqFlag = 0u;
        }
#else
        /* Call TIMER2_GetCounterVal to get the value of counter directly, like this:
           CounterValue = TIMER2_GetCounterVal(); */
#endif
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

#if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)
/**
 * @brief  Initializes the event pin for TIMER2.
 * @param  None
 * @retval None
 */
static void EventPinInit(void)
{
    stc_exint_config_t stcExintCfg;
    stc_gpio_init_t    stcGpioIni;

    GPIO_StructInit(&stcGpioIni);
    stcGpioIni.u16ExInt = PIN_EXINT_ON;
    GPIO_Init(SW1_PORT, SW1_PIN, &stcGpioIni);
    EXINT_StructInit(&stcExintCfg);
    stcExintCfg.u16ExIntCh = SW1_EXINT_CH;
    EXINT_Init(&stcExintCfg);
}
#endif // #if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)

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
          Default configuration values for basic counter is:
            Clock sourc(u16ClkSource): TIMER2_SYNC_CS_HCLK, default clock is HCLK.
            Clock prescaler(u16ClkPrescaler): TIMER2_CLK_PRESCALER_1, prescaler of clock source is 1.
            Start condition(u16HwStartCondition): TIMER2_HW_START_INVALID, do not use the start condition.
            Stop condition(u16HwStopCondition): TIMER2_HW_STOP_INVALID, do not use the stop condition.
            Clear condition(u16HwClearCondition): TIMER2_HW_CLR_INVALID, do not use the clear condition.
            TRIGA filter clock prescaler(u16FilterPrescaler): TIMER2_FILTER_PRESCALER_1, valid while enFilterCmd == Enable.
            TRIGA filter function(enFilterCmd): Disable.
            Counter match interrupt(enMatchIntCmd): Disable.
            Counter overflow interrupt(enOvfIntCmd): Disable.
          All of the above configuration values can be modified based on the application. */
    TIMER2_StructInit(&stcCfg);

    /* 3. Modify the configuration values depends on the application. */
    stcCfg.u16ClkSource    = TIMER2_SYNC_CLK_SRC;
    stcCfg.u16CompareVal   = TIMER2_COUNT_TIMES - 1u;
#if TIMER2_USE_INTERRUPT
    /* Enable counter match interrupt. */
    stcCfg.enMatchIntCmd   = Enable;
    /* Counter overflow interrupt can also be enabled if needed. */
    stcCfg.enOvfIntCmd     = Enable;
#endif

    /* 4. Configures TIMER2 according the configuration value. */
    TIMER2_TimerConfig(&stcCfg);

#if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)
    /* 5. Configures the event for TIMER2. */
    /* Enable AOS function. */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);
    /* Use event TIMER2_SYNC_CLK_EVENT(depends on the application) as synchronous clock.  */
    TIMER2_SetTrigEvent(TIMER2_SYNC_CLK_EVENT);
#else
    /* 5. Set the specified pin as TRIGA input pin. */
    GPIO_SetFunc(TIMER2_TRIGA_PORT, TIMER2_TRIGA_PIN, GPIO_FUNC_3_TIM2);
#endif // #if (TIMER2_SYNC_CLK == SYNC_CLK_EVENT)

#if TIMER2_USE_INTERRUPT
    /* 6. Configures IRQ if needed. */
    Timer2IrqConfig();
#endif
}

#if TIMER2_USE_INTERRUPT
/**
 * @brief  TIMER2 interrupt configuration.
 * @param  None
 * @retval None
 * @note   All TIMER2 interrupts can be configured as independent interrupt or shared interrupt.
 *         INT_TMR2_GCMP: Counter comparison match(CNTAR == CMPAR) or a capturing occurred.
 *                        Independent vec[Int018_IRQn, Int019_IRQn]
 *                        Share vec [Int029_IRQn]
 *         INT_TMR2_GOVF: Counter overflow(CNTAR == 0xFFFF).
 *                        Independent vec[Int018_IRQn, Int019_IRQn]
 *                        Share vec[Int029_IRQn]
 */
static void Timer2IrqConfig(void)
{
    stc_irq_regi_config_t stcIrqRegiConf;

    /* Configures counter match interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_TMR2_GCMP;
    stcIrqRegiConf.enIRQn      = Int018_IRQn;
    stcIrqRegiConf.pfnCallback = &Timer2GCmp_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Counter overflow interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_TMR2_GOVF;
    stcIrqRegiConf.enIRQn      = Int019_IRQn;
    stcIrqRegiConf.pfnCallback = &Timer2GOV_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

/**
 * @brief  TIMER2 counter comparison match interrupt callback function.
 * @param  None
 * @retval None
 */
void Timer2GCmp_IrqHandler(void)
{
    if (TIMER2_GetFlag(TIMER2_FLAG_CNT_MATCH) == Set)
    {
        TIMER2_ClrFlag(TIMER2_FLAG_CNT_MATCH);
        m_u8Timer2IrqFlag = 1u;
    }
}

/**
 * @brief  TIMER2 counter overflow interrupt callback function.
 * @param  None
 * @retval None
 */
void Timer2GOV_IrqHandler(void)
{
    if (TIMER2_GetFlag(TIMER2_FLAG_CNT_OVF) == Set)
    {
        TIMER2_ClrFlag(TIMER2_FLAG_CNT_OVF);
    }
}
#endif // #if TIMER2_USE_INTERRUPT

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
