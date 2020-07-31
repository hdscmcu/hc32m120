/**
 *******************************************************************************
 * @file  system_hc32m120.c
 * @brief This file provides two functions and one global variable to be called
 *        from  user application
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-26       chengy          First version
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
#include "hc32_common.h"

/**
 * @addtogroup CMSIS
 * @{
 */

/**
 * @addtogroup HC32m120_System
 * @{
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/**
 * @addtogroup HC32m120_System_Global_Variable
 * @{
 */

uint32_t SystemCoreClock = HRC_VALUE;  /*!< System clock frequency (Core clock) */

/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @addtogroup HC32m120_System_Global_Functions
 * @{
 */

/**
 * @brief  Setup the microcontroller system. Initialize the System and update
 *         the SystemCoreClock variable.
 * @param  None
 * @retval None
 */
void SystemInit(void)
{
    SystemCoreClockUpdate();
}

/**
 * @brief  Update HRC frequency according to Clock Register Values.
 * @param  None
 * @retval HRC frequency
 */
uint32_t HrcUpdate(void)
{
    uint32_t Hrc_value = 0ul;
    uint8_t tmp = M0P_EFM->HRCCFGR & 0x07u;

    if(EFM_HRCCFGR_HRCFREQS_3 == (M0P_EFM->HRCCFGR & EFM_HRCCFGR_HRCFREQS_3))
    {
        switch(tmp)
        {
            case 0x00:
                Hrc_value = (uint32_t)32000000ul * 3ul / 2ul;
                break;
            case 0x01:
                Hrc_value = (uint32_t)16000000ul * 3ul / 2ul;
                break;
            case 0x02:
                Hrc_value = (uint32_t)8000000ul * 3ul / 2ul;
                break;
            case 0x03:
                Hrc_value = (uint32_t)4000000ul * 3ul / 2ul;
                break;
            case 0x04:
                Hrc_value = (uint32_t)2000000ul * 3ul / 2ul;
                break;
            default:
                Hrc_value = (uint32_t)1000000ul * 3ul / 2ul;
                break;
        }
    }
    else
    {
        switch(tmp)
        {
            case 0x00:
                Hrc_value = (uint32_t)32000000ul;
                break;
            case 0x01:
                Hrc_value = (uint32_t)16000000ul;
                break;
            case 0x02:
                Hrc_value = (uint32_t)8000000ul;
                break;
            case 0x03:
                Hrc_value = (uint32_t)4000000ul;
                break;
            case 0x04:
                Hrc_value = (uint32_t)2000000ul;
                break;
            default:
                Hrc_value = (uint32_t)1000000ul;
                break;
        }
    }

    return Hrc_value;
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 * @param  None
 * @retval None
 */
void SystemCoreClockUpdate(void)
{
    uint8_t tmp;
    uint8_t div;
    tmp = M0P_CMU->CKSWR & CMU_CKSWR_CKSW;
    div = M0P_CMU->SCKDIVR & CMU_SCKDIVR_SCKDIV;
    switch(tmp)
    {
        case 0x00:  /* use internal high speed RC */
            SystemCoreClock = HrcUpdate();
            break;
        case 0x01:  /* use external high speed OSC */
            SystemCoreClock = XTAL_VALUE;
            break;
        case 0x02:  /* use internal low speed RC */
            SystemCoreClock = LRC_VALUE;
            break;
    }
    switch(div)
    {
        case 0x00:
            SystemCoreClock = SystemCoreClock;
            break;
        case 0x01:
            SystemCoreClock = SystemCoreClock / 2ul;
            break;
        case 0x02:
            SystemCoreClock = SystemCoreClock / 4ul;
            break;
        case 0x03:
            SystemCoreClock = SystemCoreClock / 8ul;
            break;
        case 0x04:
            SystemCoreClock = SystemCoreClock / 16ul;
            break;
        case 0x05:
            SystemCoreClock = SystemCoreClock / 32ul;
            break;
        case 0x06:
            SystemCoreClock = SystemCoreClock / 64ul;
            break;
    }
}

#if defined (__CC_ARM)
extern int32_t $Super$$main(void);
/* re-define main function */
int $Sub$$main(void)
{
    SystemCoreClockUpdate();
    $Super$$main();
    return 0;
}
#elif defined(__ICCARM__)
extern int32_t main(void);
/* __low_level_init will auto called by IAR cstartup */
extern void __iar_data_init3(void);
int __low_level_init(void)
{
    // call IAR table copy function.
    __iar_data_init3();
    SystemCoreClockUpdate();
    main();
    return 0;
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
