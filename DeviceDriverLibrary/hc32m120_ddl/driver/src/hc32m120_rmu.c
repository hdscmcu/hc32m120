/**
 *******************************************************************************
 * @file  hc32m120_rmu.c
 * @brief This file provides firmware functions to manage the Reset Manage Unit
 *        (RMU).
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
#include "hc32m120_rmu.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_RMU RMU
 * @brief RMU Driver Library
 * @{
 */

#if (DDL_RMU_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup RMU_Local_Macros RMU Local Macros
 * @{
 */

#define RMU_FLAG_TIM                    ((uint16_t)0x1000)


/**
 * @}
 */

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
 * @defgroup RMU_Global_Functions RMU Global Functions
 * @{
 */

/**
 * @brief  Get the reset cause.
 * @param  [in] pstcData: Pointer to return reset cause structure.
 *   @arg  See the definition for stc_rmu_rstcause_t
 *
 * @retval Ok                    Get successfully.
 */
en_result_t RMU_GetResetCause(stc_rmu_rstcause_t *pstcData)
{
    en_result_t enRet = Ok;
    uint16_t u16RstReg = 0u;

    if(NULL == pstcData)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        u16RstReg = M0P_RMU->RSTF0;

        pstcData->MultiRst = u16RstReg & RMU_RSTF0_MULTIRF ? Set : Reset;
        pstcData->XtalErrRst = u16RstReg & RMU_RSTF0_XTALERF ? Set : Reset;
        pstcData->CpuLockErrRst = u16RstReg & RMU_RSTF0_CPULKUPRF ? Set : Reset;
        pstcData->RamParityErrRst = u16RstReg & RMU_RSTF0_RAMPERF ? Set : Reset;
        pstcData->SoftwareRst = u16RstReg & RMU_RSTF0_SWRF ? Set : Reset;
        pstcData->WdtRst = u16RstReg & RMU_RSTF0_WDRF ? Set : Reset;
        pstcData->LvdRst = u16RstReg & RMU_RSTF0_LVRF ? Set : Reset;
        pstcData->RstPinRst = u16RstReg & RMU_RSTF0_PINRF ? Set : Reset;
        pstcData->PowerOnRst = u16RstReg & RMU_RSTF0_PORF ? Set : Reset;

    }
    return enRet;
}

/**
 * @brief  Clear reset flag.
 * @param  None
 * @retval en_result_t Indication clear operation result.
 * @note   clear reset flag should be done after read RMU_RSTF0 register.
 */
en_result_t RMU_ClrResetFlag(void)
{
    en_result_t enRet = Ok;
    uint16_t u16status = 0u;
    uint32_t u32timeout = 0u;

    /* Enable RMU register write */
    RMU_REG_WRITE_ENABLE();

    do
    {
        u32timeout++;
        bM0P_RMU->RSTF0_b.CLRF = 1u;
        /* Wait for register clear */
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        u16status = M0P_RMU->RSTF0;
    }while((u32timeout != RMU_FLAG_TIM) && u16status);

    /* Disable RMU register write */
    RMU_REG_WRITE_DISABLE();

    if(u32timeout >= RMU_FLAG_TIM)
    {
        enRet = ErrorTimeout;
    }

    return enRet;
}

/**
 * @}
 */

#endif /* DDL_RMU_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
