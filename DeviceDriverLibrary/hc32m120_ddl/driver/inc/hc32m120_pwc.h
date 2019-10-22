/**
 *******************************************************************************
 * @file  hc32m120_pwc.h
 * @brief This file contains all the functions prototypes of the PWC driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-09       Chengy          First version
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
#ifndef __HC32M120_PWC_H__
#define __HC32M120_PWC_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_PWC
 * @{
 */

#if (DDL_PWC_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup PWC_Global_Types PWC Global Types
 * @{
 */

/**
 * @brief PWC stop mode config
 */
typedef struct
{
    uint8_t             u8HPorSel;      /*!< Specifies the high POR(power on reset) on or off while stop mode.      */
    uint8_t             u8HrcSel;       /*!< Specifies the system clock while awake from stop mode.                 */
} stc_pwc_stop_cfg_t;

/**
 * @brief PWC power monitor init
 */
typedef struct
{
    uint8_t             u8PwrMonEn;     /*!< Specifies the power monitor on or off.             */
    uint8_t             u8PwrMonSel;    /*!< Specifies the power monitor reference voltage.     */
} stc_pwc_pwrmon_init_t;

/**
 * @brief PWC LVD config
 */
typedef struct
{
    uint16_t            u16IRDIS;       /*!< Specifies interrupt/reset on or off.           */
    uint16_t            u16IRSel;       /*!< Specifies interrupt or reset.                  */
    uint16_t            u16NMISel;      /*!< Specifies the NMI maskable or non_maskable.    */
    uint16_t            u16Level;       /*!< Specifies the LVD level.                       */
    uint16_t            u16DFDIS;       /*!< Specifies the digital filter on or off.        */
    uint16_t            u16DFSel;       /*!< Specifies the digital filter sample ability.   */
} stc_pwc_lvd_cfg_t;

/**
 * @brief PWC Ram Init
 */
typedef struct
{
    uint8_t             u8ParityRstEn;  /*!< Specifies the parity check reset on or off.    */
    uint8_t             u8ProtectArea;  /*!< Specifies the ram protect area.                */
} stc_pwc_ram_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup PWC_Global_Macros PWC Global Macros
 * @{
 */

/**
 * @defgroup PWC_HPOR_config PWC High POR(power on reset) Config
 * @{
 */
#define PWC_HPOR_ON                 (0x00U)                   /*!< High POR is valid while in stop mode   */
#define PWC_HPOR_OFF                (PWC_STPMCR_HAPORDIS)     /*!< High POR is invalid while in stop mode */
/**
 * @}
 */

/**
 * @defgroup PWC_CKSHRC_config PWC Clock Config
 * @{
 */
#define PWC_SYSCLK_FIX              (0x00U)               /*!< System clock is fixed after stop mode awake              */
#define PWC_SYSCLK_HRCDIVX          (PWC_STPMCR_CKSHRC)   /*!< System clock is n divided of HRC after stop mode awake */
/**
 * @}
 */

/**
 * @defgroup PWC_PWMON_config PWC Power Monitor Config
 * @{
 */
#define PWC_PWRMON_ON                (PWC_PWRC_PWMONE)
#define PWC_PWRMON_OFF               (0x00U)
/**
 * @}
 */

/**
 * @defgroup PWC_PWMON_Selection PWC Power Monitor Selection
 * @{
 */
#define PWC_PWRMON_VINREF           (0x00U)                /*!< Internal reference voltage */
#define PWC_PWRMON_VOTS             (PWC_PWRC_PWMONSEL)   /*!< temperature sensor voltage */
/**
 * @}
 */

/**
 * @defgroup PWC_DRV_Selection PWC Driver ability selection
 * @{
 */
#define PWC_DRV_LOW                 (0x00U)       /*!< system clock = 32kHz */
#define PWC_DRV_HIGH                (0x07U)       /*!< system clock < 48Mhz */
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_config PWC LVD Config
 * @{
 */
#define PWC_LVD_ON                  (0x0000U)
#define PWC_LVD_OFF                 (EFM_LVDICGCR_LVDDIS)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_IR_Config PWC LVD Interrupt Config
 * @{
 */
#define PWC_LVD_IR_ON               (0x0000U)
#define PWC_LVD_IR_OFF              (EFM_LVDICGCR_LVDDIS)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_Mode_selection PWC_LVD_Mode_selection interrupt or reset
 * @{
 */
#define PWC_LVD_INT                 (0x0000U)
#define PWC_LVD_RST                 (EFM_LVDICGCR_IRS)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_Int_Mode_selection PWC_LVD_nt_IMode_selection maskable or non_maskable
 * @{
 */
#define PWC_LVD_INT_MASK            (0x0000U)
#define PWC_LVD_INT_NONMASK         (EFM_LVDICGCR_NMIS)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_detection_level PWC LVD Detection Level
 * @{
 */
#define PWC_LVD_LEVEL0              (0x0000U)    /*!< Specifies the voltage range 3.98V~4.06V.    */
#define PWC_LVD_LEVEL1              (0x0100U)    /*!< Specifies the voltage range 3.67V~3.75V.    */
#define PWC_LVD_LEVEL2              (0x0200U)    /*!< Specifies the voltage range 3.06V~3.13V.    */
#define PWC_LVD_LEVEL3              (0x0300U)    /*!< Specifies the voltage range 2.96V~3.02V.    */
#define PWC_LVD_LEVEL4              (0x0400U)    /*!< Specifies the voltage range 2.86V~2.92V.    */
#define PWC_LVD_LEVEL5              (0x0500U)    /*!< Specifies the voltage range 2.75V~2.81V.    */
#define PWC_LVD_LEVEL6              (0x0600U)    /*!< Specifies the voltage range 2.65V~2.71V.    */
#define PWC_LVD_LEVEL7              (0x0700U)    /*!< Specifies the voltage range 2.55V~2.61V.    */
#define PWC_LVD_LEVEL8              (0x0800U)    /*!< Specifies the voltage range 2.45V~2.50V.    */
#define PWC_LVD_LEVEL9              (0x0900U)    /*!< Specifies the voltage range 2.04V~2.09V.    */
#define PWC_LVD_LEVEL10             (0x0A00U)    /*!< Specifies the voltage range 1.94V~1.98V.    */
#define PWC_LVD_LEVEL11             (0x0B00U)    /*!< Specifies the voltage range 1.84V~1.88V.    */
#define PWC_LVD_INP                 (0x0E00U)    /*!< Specifies the external reference voltage.   */
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_DF_config PWC_LVD_DF_config LVD digital filter on or off
 * @{
 */
#define PWC_LVD_DF_ON               (0x0000U)
#define PWC_LVD_DF_OFF              (EFM_LVDICGCR_DFDIS)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_DFS_selection PWC_LVD_DFS_selection LVD digital filter sample ability
 * @note     modified this value must when PWC_LVD_DF_OFF
 * @{
 */
#define PWC_LVD_DFS_2               (0x0000U)     /*!< 2 LRC cycle    */
#define PWC_LVD_DFS_4               (0x0001U)     /*!< 4 LRC cycle    */
#define PWC_LVD_DFS_8               (0x0002U)     /*!< 8 LRC cycle    */
#define PWC_LVD_DFS_16              (0x0003U)     /*!< 16 LRC cycle   */
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_CMP_config PWC LVD Compare Config
 * @note while PWC_LVD_OFF(LVDICGCR.LVDDIS = 1), this value is fixed as PWC_LVD_CMP_OFF.
 * @{
 */
#define PWC_LVD_CMP_ON              (PWC_LVDCSR_CMPOE)
#define PWC_LVD_CMP_OFF             (0x00U)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_FLAG PWC LVD Flag
 * @{
 */
#define PWC_LVD_FLAG_DET            (PWC_LVDCSR_DETF)     /*!< VDD = VLVD or LVDINP = VInref  */
#define PWC_LVD_FLAG_LVI            (PWC_LVDCSR_LVIF)     /*!< VCC < VLVD                     */
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_EXVCC_config PWC LVD External VCC Config
 * @{
 */
#define PWC_LVD_EXVCC_ON            (PWC_LVDCSR_EXVCCINEN)
#define PWC_LVD_EXVCC_OFF           (0x00U)
/**
 * @}
 */

/**
 * @defgroup PWC_DBGC_config PWC Debug Config
 * @{
 */
#define PWC_DBGC_ON                 (PWC_DBGC_DBGEN)
#define PWC_DBGC_OFF                (0x00U)
/**
 * @}
 */

/**
 * @defgroup PWC_Ram_Parity_Rst_En PWC ram parity check reset on or off
 * @{
 */
#define PWC_RAM_PARITY_RST_ON       (0x00U)
#define PWC_RAM_PARITY_RST_OFF      (PWC_RAMCR_RPEF)
/**
 * @}
 */

/**
 * @defgroup PWC_Ram_PRTA_Sel PWC ram protect area selection
 * @{
 */
#define PWC_RAM_PRTA_NONE           (0x00U)
#define PWC_RAM_PRTA_128            (0x01U)
#define PWC_RAM_PRTA_256            (0x02U)
#define PWC_RAM_PRTA_512            (0x03U)
/**
 * @}
 */

/**
 * @}
 */


/**
 * @defgroup PWC_REG_Write_Configuration PWC register write Configuration
 * @{
 */
#define PWC_REG_WRITE_ENABLE()          (M0P_PWC->FPRC = 0xa502u)
#define PWC_REG_WRITE_DISABLE()         (M0P_PWC->FPRC = 0xa500u)

#define PWC_LVD_REG_WRITE_ENABLE()      (M0P_PWC->FPRC = 0xa508u)
#define PWC_LVD_REG_WRITE_DISABLE()     (M0P_PWC->FPRC = 0xa500u)

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup PWC_Global_Functions
 * @{
 */
void PWC_EnterStopMode(void);
void PWC_EnterSleepMode(void);

en_result_t PWC_StopStructInit(stc_pwc_stop_cfg_t* pstcStopCfg);
en_result_t PWC_StopMdConfig(const stc_pwc_stop_cfg_t* pstcStopCfg);

void PWC_HighSpeedToLowSpeed(void);
void PWC_LowSpeedToHighSpeed(void);

en_result_t PWC_PwrMonStructInit(stc_pwc_pwrmon_init_t* pstcPwrMonInit);
en_result_t PWC_PwrMonInit(const stc_pwc_pwrmon_init_t* pstcPwrMonInit);

en_result_t PWC_LvdStructInit(stc_pwc_lvd_cfg_t* pstcLvdCfg);
en_result_t PWC_LvdConfig(const stc_pwc_lvd_cfg_t* pstcLvdCfg);
void PWC_LvdCmd(en_functional_state_t enNewState);
void PWC_LvdCmpOutputCmd(en_functional_state_t enNewState);
void PWC_LvdExRefVolCmd(en_functional_state_t enNewState);
en_flag_status_t PWC_GetLvdFlag(uint8_t u8Flag);
void PWC_ClearLvdDetFlag(void);

en_result_t PWC_RamStructInit(stc_pwc_ram_init_t *pstcRamInit);
en_result_t PWC_RamInit(const stc_pwc_ram_init_t *pstcRamInit);
en_flag_status_t PWC_GetRamFlag(void);;

void PWC_DebugCmd(en_functional_state_t enNewState);
/**
 * @}
 */

#endif /* DDL_PWC_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_PWC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
