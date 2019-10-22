/**
 *******************************************************************************
 * @file  timer2/timer2_capture/source/main.c
 * @brief Main program TIMER2 capture for the Device Driver Library.
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
 * @addtogroup Timer2_Capture
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Clock source definition for this example.
   Set HCLK as the clock source for TIMER2 by default. HCLK is XTAL(20MHz on the test board) in this example.
   TIMER2 clock frequency is 20/8 = 2.5MHz, clock cycle is 1/2.5 = 0.4us. */
#define TIMER2_CLK_PRESCALER        (TIMER2_CLK_PRESCALER_8)

/* Capturing function definition. */
#define APP_CAPTURE_EVENT           (0u)
#define APP_CAPTURE_TRIGA           (1u)
#define APP_MEASURE_PULSE_WIDTH     (2u)
#define APP_MEASURE_PERIOD          (3u)
#define APP_FUNCTION                (APP_CAPTURE_EVENT)

/* TRIGA definition. */
#if ((APP_FUNCTION == APP_MEASURE_PERIOD)      || \
     (APP_FUNCTION == APP_MEASURE_PULSE_WIDTH) || \
     (APP_FUNCTION == APP_CAPTURE_TRIGA))
/* Enable the filter of TRIGA if needed. */
#define TIMER2_TRIGA_FILTER_ENABLE  (0u)

/* TRIGA pin definition.
   One of these pins P00, P01, P30, P31, P41 and P50 can be used as TRIGA input pin. */
#define TIMER2_TRIGA_PORT           (GPIO_PORT_3)
#define TIMER2_TRIGA_PIN            (GPIO_PIN_0)
#endif // TRIGA definition end.

/* Hardware condition definitions. */
#if (APP_FUNCTION == APP_CAPTURE_EVENT)
/* Event definition.
   SW1 definition. Press SW1 to generating the event EVT_PORT_EIRQ2. */
#define SW1_PORT                    (GPIO_PORT_2)
#define SW1_PIN                     (GPIO_PIN_2)
#define SW1_EXINT_CH                (EXINT_CH02)
#define SW1_EVENT                   (EVT_PORT_EIRQ2)

#define TIMER2_EVENT_FOR_CAPTURE    (SW1_EVENT)

#define TIMER2_START_CONDITION      (TIMER2_HW_START_EVENT)
#define TIMER2_STOP_CONDITION       (TIMER2_HW_STOP_INVALID)
#define TIMER2_CLR_CONDITION        (TIMER2_HW_CLR_EVENT)
#define TIMER2_CAP_CONDITION        (TIMER2_HW_CAP_EVENT)

#elif (APP_FUNCTION == APP_CAPTURE_TRIGA)
#define TIMER2_START_CONDITION      (TIMER2_HW_START_TRIGA_FALL)
#define TIMER2_STOP_CONDITION       (TIMER2_HW_STOP_INVALID)
#define TIMER2_CLR_CONDITION        (TIMER2_HW_CLR_TRIGA_FALL)
#define TIMER2_CAP_CONDITION        (TIMER2_HW_CAP_TRIGA_FALL)

#elif (APP_FUNCTION == APP_MEASURE_PULSE_WIDTH)
#define TIMER2_START_CONDITION      (TIMER2_HW_START_TRIGA_RISE)
#define TIMER2_STOP_CONDITION       (TIMER2_HW_STOP_TRIGA_FALL)
#define TIMER2_CLR_CONDITION        (TIMER2_HW_CLR_TRIGA_FALL)
#define TIMER2_CAP_CONDITION        (TIMER2_HW_CAP_TRIGA_FALL)

#elif (APP_FUNCTION == APP_MEASURE_PERIOD)
#define TIMER2_START_CONDITION      (TIMER2_HW_START_TRIGA_FALL)
#define TIMER2_STOP_CONDITION       (TIMER2_HW_STOP_INVALID)
#define TIMER2_CLR_CONDITION        (TIMER2_HW_CLR_TRIGA_FALL)
#define TIMER2_CAP_CONDITION        (TIMER2_HW_CAP_TRIGA_FALL)

#else
#error "Undefined function!!!"
#endif // Hardware condition definitions end.

/* Capture count definition. */
#define CAPTURE_COUNT               (16u)

/* Debug printing definition. */
#if (DDL_PRINT_ENABLE == DDL_ON)
#define DBG         printf
#else
#define DBG(...)
#endif

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);

#if (APP_FUNCTION == APP_CAPTURE_EVENT)
static void EventPinInit(void);
#endif

static void Timer2Config(void);
static void Timer2IrqConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t m_u8CaptureCount = 0u;
static uint16_t m_au16CaptureVal[CAPTURE_COUNT];

#if ((APP_FUNCTION == APP_CAPTURE_EVENT) || (APP_FUNCTION == APP_CAPTURE_TRIGA))
static uint32_t m_u32OvfCount = 0u;
static uint32_t m_u32Interval;
#endif

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_capture
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Configures the system clock -- XTAL 20MHz for this example. */
    SystemClockConfig();

#if (APP_FUNCTION == APP_CAPTURE_EVENT)
    /* Initializes the event pin for TIMER2 capturing. */
    EventPinInit();
#endif

#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes USART3 for debug printing. Baud rate is 115200. */
    DDL_UartInit();
#endif

    /* Configures TIMER2. */
    Timer2Config();

    /***************** Configuration end, application start **************/

    while (1u)
    {
#if ((APP_FUNCTION == APP_CAPTURE_EVENT) || (APP_FUNCTION == APP_CAPTURE_TRIGA))
    #if 0
            /* If the time interval between two events or two falling edges is greater than 65536 TIMER2 clock cycles,
            the interval can be calculated by the following method. */
            if (m_u8CaptureCount >= 2u)
            {
                /* The interval(TIMER2 clock cycle) between two occurrences of the event(or edge of TRIGA) is: */
                m_u32Interval = 65536ul * m_u32OvfCount + m_au16CaptureVal[1u];
                DBG("\nInterval: %d.", m_u32Interval);
                m_u32OvfCount    = 0u;
                m_u8CaptureCount = 0u;
                (void)m_u32Interval;
            }
    #else
            if (m_u8CaptureCount >= CAPTURE_COUNT)
            {
                m_u8CaptureCount = 0u;
                // TODO: Use the capture values.
                (void)m_au16CaptureVal;
            }
            (void)m_u32Interval;
    #endif
#else
        if (m_u8CaptureCount >= CAPTURE_COUNT)
        {
            m_u8CaptureCount = 0u;
            // TODO: Use the capture values.
            (void)m_au16CaptureVal;
        }
#endif
    }
}

/**
 * @brief  Configures XTAL as system clock for more accurate results.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_xtal_init_t stcXTALInit;

    /* Configure XTAL. */
    stcXTALInit.u8XtalState  = CLK_XTAL_ON;
    stcXTALInit.u8XtalMode   = CLK_XTALMODE_OSC;
    stcXTALInit.u8XtalDrv    = CLK_XTALDRV_HIGH;
    stcXTALInit.u8XtalSupDrv = CLK_XTAL_SUPDRV_OFF;
    stcXTALInit.u8XtalStb    = CLK_XTALSTB_8;
    CLK_XTALInit(&stcXTALInit);

    /* Switch system clock from HRC(default) to XTAL */
    CLK_SetSysclkSrc(CLK_SYSCLKSOURCE_XTAL);
}

#if (APP_FUNCTION == APP_CAPTURE_EVENT)
/**
 * @brief  Initializes the event pin for TIMER2 capturing.
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
#endif // #if (APP_FUNCTION == APP_CAPTURE_EVENT)

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

    /* 2. Set a default configuration value for stcCfg. */
    TIMER2_StructInit(&stcCfg);

    /* 3. Modify the configuration value depends on the application. */
    stcCfg.u16ClkPrescaler       = TIMER2_CLK_PRESCALER;
    stcCfg.u16HwStartCondition   = TIMER2_START_CONDITION;
    stcCfg.u16HwStopCondition    = TIMER2_STOP_CONDITION;
    stcCfg.u16HwClearCondition   = TIMER2_CLR_CONDITION;
    stcCfg.u16HwCaptureCondition = TIMER2_CAP_CONDITION;
#if TIMER2_TRIGA_FILTER_ENABLE
    /* The clock prescaler of TRIGA filter depends on the application. */
    stcCfg.u16FilterPrescaler    = TIMER2_FILTER_PRESCALER_4;
    stcCfg.enFilterCmd           = Enable;
#endif // #if TIMER2_TRIGA_FILTER_ENABLE
    /* Enable capturing interrupt. */
    stcCfg.enMatchIntCmd = Enable;
#if ((APP_FUNCTION == APP_CAPTURE_EVENT) || (APP_FUNCTION == APP_CAPTURE_TRIGA))
    /* Enable counter overflow interrupt if needed.
       If the time interval between two events or two falling edges is greater than 65536 TIMER2 clock cycles,
       enable counter overflow interrupt.*/
    stcCfg.enOvfIntCmd   = Enable;
#endif

    /* 4. Configures TIMER2 according the configuration value. */
    TIMER2_CaptureConfig(&stcCfg);

#if (APP_FUNCTION == APP_CAPTURE_EVENT)
    /* 5. Set an event as the object for TIMER2 capturing. */
    /* Enable AOS function. */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);
    /* Set the event. */
    TIMER2_SetTrigEvent(TIMER2_EVENT_FOR_CAPTURE);
#else
    /* 5. Set the specified pin as TRIGA input pin. */
    GPIO_SetFunc(TIMER2_TRIGA_PORT, TIMER2_TRIGA_PIN, GPIO_FUNC_CMP);
#endif // #if (APP_FUNCTION == APP_CAPTURE_EVENT)

    /* 6. Configures IRQ for this example. */
    Timer2IrqConfig();
}

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

    /* Configures capturing interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_TMR2_GCMP;
    stcIrqRegiConf.enIRQn      = Int018_IRQn;
    stcIrqRegiConf.pfnCallback = &Timer2GCmp_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

#if ((APP_FUNCTION == APP_CAPTURE_EVENT) || (APP_FUNCTION == APP_CAPTURE_TRIGA))
    /* Configures counter overflow interrupt. */
    stcIrqRegiConf.enIntSrc    = INT_TMR2_GOVF;
    stcIrqRegiConf.enIRQn      = Int019_IRQn;
    stcIrqRegiConf.pfnCallback = &Timer2GOV_IrqHandler;
    INTC_IrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
#endif // #if ((APP_FUNCTION == APP_CAPTURE_EVENT) || (APP_FUNCTION == APP_CAPTURE_TRIGA))
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
        /* Get the capture value. */
        m_au16CaptureVal[m_u8CaptureCount % CAPTURE_COUNT] = TIMER2_GetCompareVal();
        m_u8CaptureCount++;
        TIMER2_ClrFlag(TIMER2_FLAG_CNT_MATCH);
    }
}

#if ((APP_FUNCTION == APP_CAPTURE_EVENT) || (APP_FUNCTION == APP_CAPTURE_TRIGA))
/**
 * @brief  TIMER2 counter overflow interrupt callback function.
 * @param  None
 * @retval None
 */
void Timer2GOV_IrqHandler(void)
{
    if (TIMER2_GetFlag(TIMER2_FLAG_CNT_OVF) == Set)
    {
        if (m_u8CaptureCount != 0u)
        {
            m_u32OvfCount++;
        }
        TIMER2_ClrFlag(TIMER2_FLAG_CNT_OVF);
    }
}
#endif // #if ((APP_FUNCTION == APP_CAPTURE_EVENT) || (APP_FUNCTION == APP_CAPTURE_TRIGA))

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
