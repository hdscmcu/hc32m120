/**
 *******************************************************************************
 * @file  pwc/pwc_lvd/source/main.c
 * @brief Main program of PWC for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-12      chengy          First version
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
 * @addtogroup PWC_Lvd
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define LVD_IRQn        (Int031_IRQn)

#define LED_R_PORT      (GPIO_PORT_12)
#define LED_G_PORT      (GPIO_PORT_7)

#define LED_R_PIN       (GPIO_PIN_0)
#define LED_G_PIN       (GPIO_PIN_0)

#define LED_R_ON()      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_ON()      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))

#define LED_R_OFF()     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_OFF()     (GPIO_SetPins(LED_G_PORT, LED_G_PIN))

#define LED_RG_ON()     {LED_R_ON();LED_G_ON();}
#define LED_RG_OFF()    {LED_R_OFF();LED_G_OFF();}

#define DELAY_TIME      (5000u)
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

void Lvd_IrqHandler(void)
{
    LED_G_OFF();
    while(1)
    {
        LED_R_ON();
    }
}
/**
 * @brief  Led Init
 * @param  None
 * @retval None
 */
static void Led_Init(void)
{
    stc_gpio_init_t stcGpioInit;

    /* LED initialize */
    GPIO_StructInit(&stcGpioInit);
    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);

    /* "Turn off" LED before set to output */
    LED_RG_OFF();

    /* Output enable */
    GPIO_OE(LED_R_PORT, LED_R_PIN, Enable);
    GPIO_OE(LED_G_PORT, LED_G_PIN, Enable);
}

/**
 * @brief  Main function of PWC project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_pwc_lvd_cfg_t  stcPwcLvdCfg;

    Led_Init();

    /* Config LVD */
    stcPwcLvdCfg.u16IRDIS = PWC_LVD_IR_ON;
    stcPwcLvdCfg.u16IRSel = PWC_LVD_INT;
    stcPwcLvdCfg.u16NMISel = PWC_LVD_INT_MASK;
    stcPwcLvdCfg.u16Level = PWC_LVD_LEVEL0;
    stcPwcLvdCfg.u16DFDIS = PWC_LVD_DF_OFF;
    stcPwcLvdCfg.u16DFSel = PWC_LVD_DFS_2;

    PWC_LvdConfig(&stcPwcLvdCfg);

    /* Set LVD interrupt. */
    INTC_ShareIrqCmd(INT_PVD_DET, Enable);

    /* Enable interrupt. */
    NVIC_ClearPendingIRQ(LVD_IRQn);
    NVIC_SetPriority(LVD_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(LVD_IRQn);

    /* Enable LVD */
    PWC_LvdCmd(Enable);
    /* Enable cmp result output */
    PWC_LvdCmpOutputCmd(Enable);

    while(1)
    {
        LED_G_ON();
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
