/**
 *******************************************************************************
 * @file  efm/efm_irq/source/main.c
 * @brief Main program of EFM for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-05-7        chengy          First version
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
 * @addtogroup EFM_Irq
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define EFM_IRQn                (Int024_IRQn)

#define EFM_WIN_START_ADDR      (0x000002810u)
#define EFM_WIN_END_ADDR        (0x000002910u)

#define LED_R_PORT              (GPIO_PORT_12)
#define LED_G_PORT              (GPIO_PORT_7)

#define LED_R_PIN               (GPIO_PIN_0)
#define LED_G_PIN               (GPIO_PIN_0)

#define LED_R_ON()              (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_ON()              (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))

#define LED_R_OFF()             (GPIO_SetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_OFF()             (GPIO_SetPins(LED_G_PORT, LED_G_PIN))

#define LED_RG_ON()             {LED_R_ON();LED_G_ON();}
#define LED_RG_OFF()            {LED_R_OFF();LED_G_OFF();}

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
/**
 * @brief  Led Init
 * @param  None
 * @retval None
 */
void EfmPgmEraseErr_IrqHandler(void)
{
    LED_G_OFF();
    LED_R_ON();

    EFM_Unlock();
    EFM_ClearFlag(EFM_FLAG_CLR_PEPRTERRCLR);
    EFM_Lock();
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
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);

    /* "Turn off" LED before set to output */
    LED_RG_OFF();

    /* Output enable */
    GPIO_OE(LED_R_PORT, LED_R_PIN, Enable);
    GPIO_OE(LED_G_PORT, LED_G_PIN, Enable);
}

/**
 * @brief  Main function of EFM project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_efm_win_protect_addr_t  stcWinAddr;
    const uint32_t u32TestData = 0x5A5A5A5Au;
    uint32_t u32Addr;

    /* Led Init */
    Led_Init();
    LED_G_ON();

    /* Unlock EFM. */
    EFM_Unlock();

    /* Enable EFM. */
    EFM_Cmd(Enable);
    /* Wait flash ready. */
    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        ;
    }

    /* Set windows protect address. */
    stcWinAddr.u32StartAddr = EFM_WIN_START_ADDR;
    stcWinAddr.u32EndAddr = EFM_WIN_END_ADDR;
    EFM_SetWinProtectAddr(stcWinAddr);

    /* Enable program & erase err interrupt. */
    EFM_InterruptCmd(EFM_INT_PEERR, Enable);

    /* Erase sector 20. */
    EFM_SectorErase(EFM_SECTOR20_ADRR);

    /* Set EFM_PEERR interrupt. */
    INTC_ShareIrqCmd(INT_EFM_PEERR, Enable);

    /* Enable interrupt. */
    NVIC_ClearPendingIRQ(EFM_IRQn);
    NVIC_SetPriority(EFM_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(EFM_IRQn);

     /* program between windows address. */
    u32Addr = EFM_WIN_START_ADDR + 4ul;
    EFM_ProgramWord(u32Addr,u32TestData);

    /* SW2 */
    while(Pin_Reset != GPIO_ReadInputPortPin(GPIO_PORT_2, GPIO_PIN_1))
    {
        ;
    }

    /* program out of windows address. */
    u32Addr = EFM_WIN_START_ADDR - 4ul;
    EFM_ProgramWord(u32Addr,u32TestData);

    /* Lock EFM. */
    EFM_Lock();

    while(1)
    {
        ;
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
