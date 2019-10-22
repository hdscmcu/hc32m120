/**
 *******************************************************************************
 * @file  rmu/source/main.c
 * @brief Main program of RMU for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-25       Wangmin         First version
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
#include <stdio.h>

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup RMU
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* Test reset mode enum*/
#define RESET_WDT                       0u
#define RESET_XTALERR                   1u
#define RESET_SOFTWARERST               2u
#define RESET_LVD                       3u

/* Modify for test reset mode, can be RESET_WDT,RESET_XTALERR or RESET_SOFTWARERST or RESET_LVD*/
#define TEST_RESET_MODE                 (RESET_LVD)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void PrintResetMode(stc_rmu_rstcause_t stcRst);
static void MakeReset(void);
static void WaitSw2_ShortPress(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of RMU project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_rmu_rstcause_t stcResetFlag;

    /* Configure system clock. */
    SystemClockConfig();

    /* configuration uart for debug information */
    DDL_UartInit();
    DDL_Delay1ms(10u);

    RMU_GetResetCause(&stcResetFlag);
    RMU_ClrResetFlag();

    PrintResetMode(stcResetFlag);

    printf("\nPress SW2 to config reset condition .\n");
    /* Wait short press key SW2 */
    WaitSw2_ShortPress();

    MakeReset();

    /* Reset condition configuration finished */
    while(1u)
    {
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
 * @brief  Print reset information.
 * @param  [in]  stcRst    Reset cause structure
 * @retval None
 */
static void PrintResetMode(stc_rmu_rstcause_t stcRst)
{
    if(Set == stcRst.MultiRst)
    {
        printf("Multiple reset.\r\n");
    }
    if(Set == stcRst.XtalErrRst)
    {
        printf("XTAL error reset.\r\n");
    }
    if(Set == stcRst.CpuLockErrRst)
    {
        printf("M0+ CPU lock reset.\r\n");
    }
    if(Set == stcRst.RamParityErrRst)
    {
        printf("RAM parity error reset.\r\n");
    }
    if(Set == stcRst.SoftwareRst)
    {
        printf("Software reset.\r\n");
    }
    if(Set == stcRst.WdtRst)
    {
        printf("WDT reset.\r\n");
    }
    if(Set == stcRst.LvdRst)
    {
        printf("Low voltage detect reset.\r\n");
    }
    if(Set == stcRst.RstPinRst)
    {
        printf("NRST pin reset.\r\n");
    }
    if(Set == stcRst.PowerOnRst)
    {
        printf("Power on reset.\r\n");
    }
}

/**
 * @brief  Make reset condition for test
 * @param  [in]  None
 * @retval None
 */
static void MakeReset(void)
{
#if (TEST_RESET_MODE == RESET_XTALERR)
    /* Config XTAL status detect error reset on*/
    stc_clk_xtalstd_init_t stcXtalStdCfg;
    stcXtalStdCfg.u8XtalStdState = CLK_XTALSTD_ON;
    stcXtalStdCfg.u8XtalStdMode = CLK_XTALSTD_MODE_RST;
    stcXtalStdCfg.u8XtalStdInt = CLK_XTALSTD_INT_OFF;
    stcXtalStdCfg.u8XtalStdRst = CLK_XTALSTD_RST_ON;
    CLK_XTALStdInit(&stcXtalStdCfg);

    printf("Xtal status detected reset function on.\r\n");
#elif (TEST_RESET_MODE == RESET_SOFTWARERST)
    /* Software reset MCU */
    printf("Software reset soon.\r\n");
    printf(0u);
    NVIC_SystemReset();

#elif (TEST_RESET_MODE == RESET_WDT)
    /* Configuration Watchdog function */
    stc_swdt_init_t stcSwdtCfg;
    stcSwdtCfg.u32CountCycle = SWDT_COUNTER_CYCLE_256;
    stcSwdtCfg.u32ClockDivision = SWDT_CLOCK_DIV1;
    stcSwdtCfg.u32RefreshRange = SWDT_RANGE_100PCT;
    stcSwdtCfg.u32LPModeCountEn = Disable;
    stcSwdtCfg.u32RequestType = SWDT_TRIG_EVENT_RESET;

    SWDT_Init(&stcSwdtCfg);
    printf("Watchdog reset function on.\r\n");
    SWDT_ReloadCounter();
#elif (TEST_RESET_MODE == RESET_LVD)
    stc_pwc_lvd_cfg_t  stcPwcLvdCfg;

    /* Config LVD */
    stcPwcLvdCfg.u16IRDIS = PWC_LVD_IR_ON;
    stcPwcLvdCfg.u16IRSel = PWC_LVD_RST;
    stcPwcLvdCfg.u16NMISel = PWC_LVD_INT_MASK;
    stcPwcLvdCfg.u16Level = PWC_LVD_LEVEL2;
    stcPwcLvdCfg.u16DFDIS = PWC_LVD_DF_OFF;
    stcPwcLvdCfg.u16DFSel = PWC_LVD_DFS_2;

    PWC_LvdConfig(&stcPwcLvdCfg);

    /* Enable LVD */
    PWC_LvdCmd(Enable);
    /* Enable cmp result output */
    PWC_LvdCmpOutputCmd(Enable);

    printf("LVD reset on, please make the power supply voltage lower than 3.0V\r\n");

#endif
}

/**
 * @brief  Wait SW2 key short press.
 * @param  None
 * @retval None
 */
static void WaitSw2_ShortPress(void)
{
    /* Wait key up */
    while(Pin_Set != GPIO_ReadInputPortPin(GPIO_PORT_2, GPIO_PIN_1))
    {
        ;
    }
    DDL_Delay1ms(5u);

    /* Wait key down */
    while(Pin_Reset != GPIO_ReadInputPortPin(GPIO_PORT_2, GPIO_PIN_1))
    {
        ;
    }
    DDL_Delay1ms(5u);

    /* Wait key up */
    while(Pin_Set != GPIO_ReadInputPortPin(GPIO_PORT_2, GPIO_PIN_1))
    {
        ;
    }
    DDL_Delay1ms(5u);
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
