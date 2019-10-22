/**
 *******************************************************************************
 * @file  event_port/source/main.c
 * @brief Main program of event port for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-26       Zhangxl         First version
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
 * @addtogroup EVENT_PORT
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define EP_PORT         (GPIO_PORT_2)
#define EP1_PIN         (GPIO_PIN_0)
#define EP2_PIN         (GPIO_PIN_1)
#define EP3_PIN         (GPIO_PIN_2)
#define EP4_PIN         (GPIO_PIN_3)

#define EP1_TRG_PORT    (GPIO_PORT_3)
#define EP1_TRG_PIN     (GPIO_PIN_0)
#define EP2_TRG_PORT    (GPIO_PORT_1)
#define EP2_TRG_PIN     (GPIO_PIN_0)
#define EP3_TRG_PORT    (GPIO_PORT_1)
#define EP3_TRG_PIN     (GPIO_PIN_5)

#define LED_R_PORT      (GPIO_PORT_12)
#define LED_G_PORT      (GPIO_PORT_7)
#define LED_B_PORT      (GPIO_PORT_3)

#define LED_R_PIN       (GPIO_PIN_0)
#define LED_G_PIN       (GPIO_PIN_0)
#define LED_B_PIN       (GPIO_PIN_1)

#define LED_R_ON()      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_ON()      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))
#define LED_B_ON()      (GPIO_ResetPins(LED_B_PORT, LED_B_PIN))

#define LED_R_OFF()     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_OFF()     (GPIO_SetPins(LED_G_PORT, LED_G_PIN))
#define LED_B_OFF()     (GPIO_SetPins(LED_B_PORT, LED_B_PIN))

#define LED_R_TOGGLE()  (GPIO_TogglePins(LED_R_PORT, LED_R_PIN))
#define LED_G_TOGGLE()  (GPIO_TogglePins(LED_G_PORT, LED_G_PIN))
#define LED_B_TOGGLE()  (GPIO_TogglePins(LED_B_PORT, LED_B_PIN))
#define LED_RGB_TOGGLE()                                                        \
                        {LED_R_TOGGLE();LED_G_TOGGLE();LED_B_TOGGLE();}

#define LED_RGB_ON()    {LED_R_ON();LED_G_ON();LED_B_ON();}
#define LED_RGB_OFF()   {LED_R_OFF();LED_G_OFF();LED_B_OFF();}

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @brief  Generate a rising edge as the EVENT_PORT1 trigger signal
 * @param  None
 * @retval None
 */
static void EP1_Rising(void)
{
    GPIO_ResetPins(EP1_TRG_PORT, EP1_TRG_PIN);
    DDL_Delay1ms(1ul);
    GPIO_SetPins(EP1_TRG_PORT, EP1_TRG_PIN);
}

/**
 * @brief  Generate a rising edge as the EVENT_PORT2 trigger signal
 * @param  None
 * @retval None
 */
static void EP2_Rising(void)
{
    GPIO_ResetPins(EP2_TRG_PORT, EP2_TRG_PIN);
    DDL_Delay1ms(1ul);
    GPIO_SetPins(EP2_TRG_PORT, EP2_TRG_PIN);
}

/**
 * @brief  Generate a rising edge as the EVENT_PORT3 trigger signal
 * @param  None
 * @retval None
 */
static void EP3_Rising(void)
{
    GPIO_ResetPins(EP3_TRG_PORT, EP3_TRG_PIN);
    DDL_Delay1ms(1ul);
    GPIO_SetPins(EP3_TRG_PORT, EP3_TRG_PIN);
}

/**
 * @brief  EVENT_PORT ISR callback function
 * @param  None
 * @retval None
 */
static void EVENT_PORT_IrqCallback(void)
{
    LED_G_TOGGLE();
}

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of event port project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint8_t u8Ret;
    stc_gpio_init_t stcGpioInit;
    stc_event_port_init_t stcEventPortInit;
    stc_irq_regi_config_t stcIrqRegister;

    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinState = Pin_Set;
    stcGpioInit.u16PinMode = PIN_MODE_OUT;
    /* Trigger input for EVENT_PORT initialize */
    GPIO_Init(EP1_TRG_PORT, EP1_TRG_PIN, &stcGpioInit);
    GPIO_Init(EP2_TRG_PORT, EP2_TRG_PIN, &stcGpioInit);
    GPIO_Init(EP3_TRG_PORT, EP3_TRG_PIN, &stcGpioInit);

    /* "Turn off" LED before set to output */
    LED_RGB_OFF();

    /* RGB LED initialize */
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
    GPIO_Init(LED_B_PORT, LED_B_PIN, &stcGpioInit);

    /* Enable EVENT_PORT(AOS) clock */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);

    /* EVENT_PORT pin function set */
    GPIO_SetFunc(EP_PORT, (EP1_PIN | EP2_PIN | EP3_PIN), GPIO_FUNC_4_EVENTPT);

    /* Set EVENT_PORT1,2,3 */
    EVENT_PORT_StructInit(&stcEventPortInit);
    stcEventPortInit.u32EPFE = EVENT_PORT_FILTER_ON;
    stcEventPortInit.u32EPFClk = EVENT_PORT_FCLK_HCLK_DIV16;
    stcEventPortInit.u32EPTrigger = EP1_TRIGGER_RISING | EP2_TRIGGER_RISING | EP3_TRIGGER_RISING;
    EVENT_PORT_Init(&stcEventPortInit);

    /* Set IRQ handler 20 as the EVENT_PORT ISR entry */
    stcIrqRegister.enIRQn       = Int020_IRQn;
    stcIrqRegister.enIntSrc     = INT_EVENT_PORT;
    stcIrqRegister.pfnCallback  = &EVENT_PORT_IrqCallback;
    u8Ret = INTC_IrqRegistration(&stcIrqRegister);
    if (Ok != u8Ret)
    {
        // check parameter
        while(1)
        {
            ;
        }
    }

    /* IRQ020 for EVENT_PORT interrupt */
    NVIC_ClearPendingIRQ(Int020_IRQn);
    NVIC_SetPriority(Int020_IRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(Int020_IRQn);


    while (1)
    {
        /* triggered by EP1 rising edge */
        GPIO_SetPins(EP3_TRG_PORT, EP3_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP2_TRG_PORT, EP2_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP1_TRG_PORT, EP1_TRG_PIN);        /* 1 */

        EP1_Rising();
        while((EP3_STATE_SET | EP2_STATE_SET | EP1_STATE_SET) !=                \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }

        GPIO_SetPins(EP3_TRG_PORT, EP3_TRG_PIN);        /* 1 */
        GPIO_ResetPins(EP2_TRG_PORT, EP2_TRG_PIN);      /* 0 */
        GPIO_SetPins(EP1_TRG_PORT, EP1_TRG_PIN);        /* 1 */

        EP1_Rising();
        while((EP3_STATE_SET | EP2_STATE_RESET | EP1_STATE_SET) !=              \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }

        GPIO_ResetPins(EP3_TRG_PORT, EP3_TRG_PIN);      /* 0 */
        GPIO_SetPins(EP2_TRG_PORT, EP2_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP1_TRG_PORT, EP1_TRG_PIN);        /* 1 */

        EP1_Rising();
        while((EP3_STATE_RESET | EP2_STATE_SET | EP1_STATE_SET) !=              \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }

        DDL_Delay1ms(100ul);

        /* triggered by EP2 rising edge */
        GPIO_SetPins(EP3_TRG_PORT, EP3_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP2_TRG_PORT, EP2_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP1_TRG_PORT, EP1_TRG_PIN);        /* 1 */

        EP2_Rising();
        while((EP3_STATE_SET | EP2_STATE_SET | EP1_STATE_SET) !=                \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }

        GPIO_ResetPins(EP3_TRG_PORT, EP3_TRG_PIN);      /* 0 */
        GPIO_SetPins(EP2_TRG_PORT, EP2_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP1_TRG_PORT, EP1_TRG_PIN);        /* 1 */

        EP2_Rising();
        while((EP3_STATE_RESET | EP2_STATE_SET | EP1_STATE_SET) !=              \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }

        GPIO_SetPins(EP3_TRG_PORT, EP3_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP2_TRG_PORT, EP2_TRG_PIN);        /* 1 */
        GPIO_ResetPins(EP1_TRG_PORT, EP1_TRG_PIN);      /* 0 */

        EP2_Rising();
        while((EP3_STATE_SET | EP2_STATE_SET | EP1_STATE_RESET) !=              \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }

        DDL_Delay1ms(100ul);

        /* triggered by EP3 rising edge */
        GPIO_SetPins(EP3_TRG_PORT, EP3_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP2_TRG_PORT, EP2_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP1_TRG_PORT, EP1_TRG_PIN);        /* 1 */

        EP3_Rising();
        while((EP3_STATE_SET | EP2_STATE_SET | EP1_STATE_SET) !=                \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }

        GPIO_SetPins(EP3_TRG_PORT, EP3_TRG_PIN);        /* 1 */
        GPIO_SetPins(EP2_TRG_PORT, EP2_TRG_PIN);        /* 1 */
        GPIO_ResetPins(EP1_TRG_PORT, EP1_TRG_PIN);      /* 0 */

        EP3_Rising();
        while((EP3_STATE_SET | EP2_STATE_SET | EP1_STATE_RESET) !=              \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }

        GPIO_SetPins(EP3_TRG_PORT, EP3_TRG_PIN);        /* 1 */
        GPIO_ResetPins(EP2_TRG_PORT, EP2_TRG_PIN);      /* 0 */
        GPIO_SetPins(EP1_TRG_PORT, EP1_TRG_PIN);        /* 1 */

        EP3_Rising();
        while((EP3_STATE_SET | EP2_STATE_RESET | EP1_STATE_SET) !=              \
                EVENT_PORT_GetStatus(EP_STATE_CURRENT))
        {
            ;
        }
        DDL_Delay1ms(100ul);
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
