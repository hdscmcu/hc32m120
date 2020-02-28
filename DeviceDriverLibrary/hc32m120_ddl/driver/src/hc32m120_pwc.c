/**
 *******************************************************************************
 * @file  hc32m120_pwc.c
 * @brief This file provides firmware functions to manage the Power Contorl(PWC).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-09       Chengy          First version
   2020-02-28       Chengy          Modified comment error & spelling error
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
#include "hc32m120_pwc.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_PWC PWC
 * @brief Power Control Driver Library
 * @{
 */

#if (DDL_PWC_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup PWC_Local_Macros PWC Local Macros
 * @{
 */
#define PWC_LVD_MSK     (EFM_LVDICGCR_DFDIS | EFM_LVDICGCR_LVDLVL | EFM_LVDICGCR_NMIS | \
                         EFM_LVDICGCR_IRS | EFM_LVDICGCR_IRDIS)
/**
 * @defgroup PWC_Check_Parameters_Validity PWC Check Parameters Validity
 * @{
 */
/*  Parameter validity check for stop mode high POR sel. */
#define IS_VALID_PWC_STOP_HPOR_SEL(sel)                                        \
(       ((sel) == PWC_HPOR_ON)                  ||                             \
        ((sel) == PWC_HPOR_OFF))

/*  Parameter validity check for system clock sel while awake stop mode. */
#define IS_VALID_PWC_STOP_SYSCLK_SEL(sel)                                      \
(       ((sel) == PWC_SYSCLK_FIX)               ||                             \
        ((sel) == PWC_SYSCLK_HRCDIVX))

/*  Parameter validity check for power monitor. */
#define IS_VALID_PWC_PWRMON_EN(x)                                              \
(       ((x) == PWC_PWRMON_ON)                  ||                             \
        ((x) == PWC_PWRMON_OFF))

/*  Parameter validity check for power monitor sel. */
#define IS_VALID_PWC_PWRMON_SEL(sel)                                           \
(       ((sel) == PWC_PWRMON_VINREF)            ||                             \
        ((sel) == PWC_PWRMON_VOTS))

/*  Parameter validity check for LVD Level. */
#define IS_VALID_PWC_LVD_LEVEL(level)                                          \
(       ((level) == PWC_LVD_LEVEL0)             ||                             \
        ((level) == PWC_LVD_LEVEL1)             ||                             \
        ((level) == PWC_LVD_LEVEL2)             ||                             \
        ((level) == PWC_LVD_LEVEL3)             ||                             \
        ((level) == PWC_LVD_LEVEL4)             ||                             \
        ((level) == PWC_LVD_LEVEL5)             ||                             \
        ((level) == PWC_LVD_LEVEL6)             ||                             \
        ((level) == PWC_LVD_LEVEL7)             ||                             \
        ((level) == PWC_LVD_LEVEL8)             ||                             \
        ((level) == PWC_LVD_LEVEL9)             ||                             \
        ((level) == PWC_LVD_LEVEL10)            ||                             \
        ((level) == PWC_LVD_LEVEL11)            ||                             \
        ((level) == PWC_LVD_INP))

/*  Parameter validity check for LVD digital filter cycle selection. */
#define IS_VALID_PWC_LVD_DFS_SEL(sel)                                          \
(       ((sel) == PWC_LVD_DFS_2)                ||                             \
        ((sel) == PWC_LVD_DFS_4)                ||                             \
        ((sel) == PWC_LVD_DFS_8)                ||                             \
        ((sel) == PWC_LVD_DFS_16))

/*  Parameter validity check for LVD flag. */
#define IS_VALID_PWC_LVD_FLAG(flag)                                            \
(       ((flag) == PWC_LVD_FLAG_DET)            ||                             \
        ((flag) == PWC_LVD_FLAG_LVI))

/*  Parameter validity check for Ram protect area selection. */
#define IS_VALID_PWC_RAM_PRAT_SEL(sel)                                         \
(       ((sel) == PWC_RAM_PRTA_NONE)            ||                             \
        ((sel) == PWC_RAM_PRTA_128)             ||                             \
        ((sel) == PWC_RAM_PRTA_256)             ||                             \
        ((sel) == PWC_RAM_PRTA_512))

/**
 * @}
 */

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
 * @defgroup PWC_Global_Functions PWC Global Functions
 * @{
 */

/**
 * @brief  Enter stop mode.
 * @param  None
 * @retval None
 */
void PWC_EnterStopMode(void)
{
    PWC_REG_WRITE_ENABLE();

    SET_REG8_BIT(M0P_PWC->STPMCR, PWC_STPMCR_STOP);

    PWC_REG_WRITE_DISABLE();

     __WFI();
}

/**
 * @brief  Enter sleep mode.
 * @param  None
 * @retval None
 */
void PWC_EnterSleepMode(void)
{
    PWC_REG_WRITE_ENABLE();

    bM0P_PWC->STPMCR_b.STOP = 0u;

    PWC_REG_WRITE_DISABLE();

     __WFI();
}

/**
 * @brief  Init the stop config structure with default value.
 * @param  [in] pstcStopCfg            The pointer of stop config structure.
 *   @arg u8HPorSel  Specifies the high POR on or off while stop mode.
 *   @arg u8HrcSel   Specifies the system clock while awake from stop mode.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t PWC_StopStructInit(stc_pwc_stop_cfg_t* pstcStopCfg)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcStopCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcStopCfg->u8HPorSel = PWC_HPOR_ON;
        pstcStopCfg->u8HrcSel = PWC_SYSCLK_FIX;
    }

    return enRet;
}

/**
 * @brief  Config the stop mode.
 * @param  [in] pstcStopCfg      The pointer of stop config.
 *   @arg u8HPorSel  Specifies the high POR on or off while stop mode.
 *   @arg u8HrcSel   Specifies the system clock while awake from stop mode.
 * @retval None
 */
en_result_t PWC_StopMdConfig(const stc_pwc_stop_cfg_t* pstcStopCfg)
{
    en_result_t enRet = Ok;

    if(pstcStopCfg == NULL)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Parameter valid check */
        DDL_ASSERT(IS_VALID_PWC_STOP_HPOR_SEL(pstcStopCfg->u8HPorSel));
        DDL_ASSERT(IS_VALID_PWC_STOP_SYSCLK_SEL(pstcStopCfg->u8HrcSel));

        PWC_REG_WRITE_ENABLE();

        /* Config the stop mode */
        MODIFY_REG8(M0P_PWC->STPMCR, PWC_STPMCR_HAPORDIS , pstcStopCfg->u8HPorSel);
        MODIFY_REG8(M0P_PWC->STPMCR, PWC_STPMCR_CKSHRC , pstcStopCfg->u8HrcSel);

        PWC_REG_WRITE_DISABLE();
    }

    return enRet;
}

/**
 * @brief  Init power monitor structure with default value.
 * @param  [in] pstcPwrMonInit         The pointer of power monitor structure.
 *   @arg u8PwrMonEn    Specifies the power monitor on or off.
 *   @arg u8PwrMonSel   Specifies the power monitor reference voltage.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t PWC_PwrMonStructInit(stc_pwc_pwrmon_init_t* pstcPwrMonInit)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcPwrMonInit)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcPwrMonInit->u8PwrMonEn = PWC_PWRMON_ON;
        pstcPwrMonInit->u8PwrMonSel = PWC_PWRMON_VINREF;
    }

    return enRet;
}

/**
 * @brief  Init power monitor.
 * @param  [in] pstcPwrMonInit      The pointer of power monitor init.
 *   @arg u8PwrMonEn    Specifies the power monitor on or off.
 *   @arg u8PwrMonSel   Specifies the power monitor reference voltage.
 * @retval None
 */
en_result_t PWC_PwrMonInit(const stc_pwc_pwrmon_init_t* pstcPwrMonInit)
{
    en_result_t enRet = Ok;

    if(pstcPwrMonInit == NULL)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Parameter valid check */
        DDL_ASSERT(IS_VALID_PWC_PWRMON_EN(pstcPwrMonInit->u8PwrMonEn));
        DDL_ASSERT(IS_VALID_PWC_PWRMON_SEL(pstcPwrMonInit->u8PwrMonSel));

        PWC_REG_WRITE_ENABLE();

        if(PWC_PWRMON_OFF == pstcPwrMonInit->u8PwrMonEn)
        {
            bM0P_PWC->PWRC_b.PWMONE = 0u;
        }
        else
        {
            MODIFY_REG8(M0P_PWC->PWRC, PWC_PWRC_PWMONSEL , pstcPwrMonInit->u8PwrMonSel);
            SET_REG8_BIT(M0P_PWC->PWRC, PWC_PWRC_PWMONE);
        }

        PWC_REG_WRITE_DISABLE();
    }

    return enRet;
}

/**
 * @brief  Switch high speed to low speed, set the LDO driver ability.
 * @param  None
 * @retval None
 */
void PWC_HighSpeedToLowSpeed(void)
{
    PWC_REG_WRITE_ENABLE();

    MODIFY_REG8(M0P_PWC->PWRC, PWC_PWRC_PWDRV , PWC_DRV_LOW);

    DDL_Delay1ms(1u);

    PWC_REG_WRITE_DISABLE();
}

/**
 * @brief  Switch low speed to high speed, set the LDO driver ability.
 * @param  None
 * @retval None
 */
void PWC_LowSpeedToHighSpeed(void)
{
    PWC_REG_WRITE_ENABLE();

    MODIFY_REG8(M0P_PWC->PWRC, PWC_PWRC_PWDRV , PWC_DRV_HIGH);

    DDL_Delay1ms(1u);

    PWC_REG_WRITE_DISABLE();
}

/**
 * @brief  Init lvd config structure with default value.
 * @param  [in] pstcLvdCfg         The pointer of lvd config structure.
 *   @arg u16IRDIS  Specifies interrupt/reset on or off.
 *   @arg u16IRSel  Specifies interrupt or reset.
 *   @arg u16NMISel Specifies the NMI maskable or non_maskable.
 *   @arg u16Level  Specifies the LVD level.
 *   @arg u16DFDIS  Specifies the digital filter on or off.
 *   @arg u16DFSel  Specifies the digital filter sample ability.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t PWC_LvdStructInit(stc_pwc_lvd_cfg_t* pstcLvdCfg)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcLvdCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcLvdCfg->u16IRDIS = PWC_LVD_IR_OFF;
        pstcLvdCfg->u16IRSel = PWC_LVD_RST;
        pstcLvdCfg->u16NMISel = PWC_LVD_INT_MASK;
        pstcLvdCfg->u16Level = PWC_LVD_LEVEL0;
        pstcLvdCfg->u16DFDIS = PWC_LVD_DF_OFF;
        pstcLvdCfg->u16DFSel = PWC_LVD_DFS_16;
    }

    return enRet;
}

/**
 * @brief  Config the Low Voltage Detector(LVD).
 * @param  [in] pstcLvdCfg      The pointer of LVD config.
 *   @arg u16IRDIS  Specifies interrupt/reset on or off.
 *   @arg u16IRSel  Specifies interrupt or reset.
 *   @arg u16NMISel Specifies the NMI maskable or non_maskable.
 *   @arg u16Level  Specifies the LVD level.
 *   @arg u16DFDIS  Specifies the digital filter on or off.
 *   @arg u16DFSel  Specifies the digital filter sample ability.
 * @retval None
 */
en_result_t PWC_LvdConfig(const stc_pwc_lvd_cfg_t* pstcLvdCfg)
{
    en_result_t enRet = Ok;

    if(pstcLvdCfg == NULL)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Parameter valid check */
        DDL_ASSERT(IS_VALID_PWC_LVD_LEVEL(pstcLvdCfg->u16Level));
        DDL_ASSERT(IS_VALID_PWC_LVD_DFS_SEL(pstcLvdCfg->u16DFSel));

        PWC_LVD_REG_WRITE_ENABLE();

        /* When modified pstcLvdCfg->u16DFSel, the pstcLvdCfg->u16DFDIS shoule be PWC_LVD_DF_OFF
        LVDICGCR.DFS modified should when LVDICGCR.DFDIS = 1. */
        SET_REG16_BIT(M0P_EFM->LVDICGCR, EFM_LVDICGCR_DFDIS);
        /* Set the LVD digital filter sample ability. */
        MODIFY_REG16(M0P_EFM->LVDICGCR, EFM_LVDICGCR_DFS, pstcLvdCfg->u16DFSel);

        /* Config LVD. */
        MODIFY_REG16(M0P_EFM->LVDICGCR, PWC_LVD_MSK,
                pstcLvdCfg->u16DFDIS | pstcLvdCfg->u16Level | pstcLvdCfg->u16NMISel |
                pstcLvdCfg->u16IRSel | pstcLvdCfg->u16IRDIS);

        PWC_LVD_REG_WRITE_DISABLE();
    }

    return enRet;
}

/**
 * @brief  Enable or disable the Low Voltage Detector(LVD).
 * @param  [in] enNewState      The new state of the LVD.
 *   @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void PWC_LvdCmd(en_functional_state_t enNewState)
{
    /* Parameter valid check */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    PWC_LVD_REG_WRITE_ENABLE();

    if(Enable == enNewState)
    {
        bM0P_EFM->LVDICGCR_b.LVDDIS = 0u;
    }
    else
    {
        bM0P_EFM->LVDICGCR_b.LVDDIS = 1u;
    }

    PWC_LVD_REG_WRITE_DISABLE();
}

/**
 * @brief  Enable or disable the LVD compare result output.
 * @param  [in] enNewState      The new state of the LVD CmpOutput.
 *   @arg  This parameter can be: Enable or Disable.
 * @note   This function should be call after PWC_LVDCmd(Enable), witch LVDICGCR.LVDDIS = 0.
 * @retval None
 */
void PWC_LvdCmpOutputCmd(en_functional_state_t enNewState)
{
    /* Parameter valid check */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    PWC_LVD_REG_WRITE_ENABLE();

    bM0P_PWC->LVDCSR_b.CMPOE = enNewState;

    PWC_LVD_REG_WRITE_DISABLE();
}

/**
 * @brief  Enable or disable the external reference voltage.
 * @param  [in] enNewState      The new state of the LVD external reference voltage.
 *   @arg  This parameter can be: Enable or Disable.
 * @note   When enable, LVDICGCR.LVDLVL = b1110.
 * @retval None
 */
void PWC_LvdExRefVolCmd(en_functional_state_t enNewState)
{
    /* Parameter valid check */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    PWC_LVD_REG_WRITE_ENABLE();

    bM0P_PWC->LVDCSR_b.EXVCCINEN = enNewState;

    PWC_LVD_REG_WRITE_DISABLE();
}

/**
 * @brief  Get LVD flag.
 * @param  [in] u8Flag      The flag of LVD.
 *  This parameter can be one of the following values:
 *   @arg    PWC_LVD_FLAG_DET       VDD = VLVD or LVDINP = VInref
 *   @arg    PWC_LVD_FLAG_LVI       VCC < VLVD
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Interrupt flag is set
 *           - Reset: Interrupt flag is reset
 */
en_flag_status_t PWC_GetLvdFlag(uint8_t u8Flag)
{
    /* Parameter valid check */
    DDL_ASSERT(IS_VALID_PWC_LVD_FLAG(u8Flag));

    return ((0u == (M0P_PWC->LVDCSR & u8Flag)) ? Reset :Set);
}

/**
 * @brief  Clear LVD flag.
 * @param  None
 * @retval None
 */
void PWC_ClearLvdDetFlag(void)
{
    PWC_LVD_REG_WRITE_ENABLE();

    bM0P_PWC->LVDCSR_b.DETF = 0u;

    PWC_LVD_REG_WRITE_DISABLE();
}

/**
 * @brief  Init ram init structure with default value.
 * @param  [in] pstcRamInit         The pointer of ram init structure.
 *   @arg u8ParityRstEn  Specifies the ram parity check reset on or off.
 *   @arg u8ProtectArea  Specifies the ram protect area.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t PWC_RamStructInit(stc_pwc_ram_init_t *pstcRamInit)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcRamInit)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcRamInit->u8ParityRstEn = PWC_RAM_PARITY_RST_OFF;
        pstcRamInit->u8ProtectArea = PWC_PWRMON_VINREF;
    }

    return enRet;
}

/**
 * @brief  Init the Ram.
 * @param  [out] pstcRamInit         The pointer of ram init structure.
 *   @arg u8ParityRstEn  Specifies the ram parity check reset on or off.
 *   @arg u8ProtectArea  Specifies the ram protect area.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t PWC_RamInit(const stc_pwc_ram_init_t *pstcRamInit)
{
    en_result_t enRet = Ok;

    if(pstcRamInit == NULL)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Parameter valid check */
        DDL_ASSERT(IS_VALID_PWC_RAM_PRAT_SEL(pstcRamInit->u8ProtectArea));

        PWC_LVD_REG_WRITE_ENABLE();

        MODIFY_REG8(M0P_PWC->RAMCR, PWC_RAMCR_RPRTA | PWC_RAMCR_RPEF,
                pstcRamInit->u8ParityRstEn | pstcRamInit->u8ProtectArea);

        PWC_LVD_REG_WRITE_DISABLE();
    }

    return enRet;
}

/**
 * @brief  Get the ram parity check flag.
 * @param  None
 * @retval An en_flag_status_t enumeration value:
 *           - Set: Interrupt flag is set
 *           - Reset: Interrupt flag is reset
 */
en_flag_status_t PWC_GetRamFlag(void)
{
    return ((0u == (M0P_PWC->RAMCR & PWC_RAMCR_RPEF)) ? Reset :Set);
}

/**
 * @brief  Enable or disable the Debug.
 * @param  [in] enNewState      The new state of the LVD.
 *   @arg  This parameter can be: Enable or Disable.
 * @retval None
 */
void PWC_DebugCmd(en_functional_state_t enNewState)
{
    /* Parameter valid check */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    PWC_REG_WRITE_ENABLE();

    bM0P_PWC->DBGC_b.DBGEN = enNewState;

    PWC_REG_WRITE_DISABLE();
}

/**
 * @}
 */

#endif  /* DDL_PWC_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
