/**
 *******************************************************************************
 * @file  hc32m120_clk.c
 * @brief This file provides firmware functions to manage the Clock(CLK).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-24       Chengy          First version
   2020-02-14       Zhangxl         Use CLEAR_REG8_BIT for CLK_ClearXtalStdFlag()
                                    instead of CLEAR_REG8
   2020-02-27       Zhangxl         Use new macro in hc32_common.h
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
#include "hc32m120_clk.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_CLK CLK
 * @brief Clock Driver Library
 * @{
 */

#if (DDL_CLK_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CLK_Local_Macros CLK Local Macros
 * @{
 */

#define CLK_TIMEOUT             (0x1000U)
#define CLK_XTAL_TIMEOUT        (0x20000U)


/**
 * @defgroup RTC_Check_Parameters_Validity RTC Check Parameters Validity
 * @{
 */
/* Paramer valid check for XTAL state */
#define IS_VALID_CLK_XTAL_STATE(STA)                                           \
(       ((STA) == CLK_XTAL_OFF)                     ||                         \
        ((STA) == CLK_XTAL_ON))

/* Paramer valid check for XTAL mode */
#define IS_VALID_CLK_XTAL_MODE(MODE)                                           \
(       ((MODE) == CLK_XTALMODE_OSC)                ||                         \
        ((MODE) == CLK_XTALMODE_EXCLK))

/* Paramer valid check for XTAL super driver state */
#define IS_VALID_CLK_XTAL_SUPDRV_STATE(STA)                                    \
(       ((STA) == CLK_XTAL_SUPDRV_ON)               ||                         \
        ((STA) == CLK_XTAL_SUPDRV_OFF))

/* Paramer valid check for XTAL driver ability mode */
#define IS_VALID_CLK_XTALDRV_MODE(MODE)                                        \
(       ((MODE) == CLK_XTALDRV_HIGH)                ||                         \
        ((MODE) == CLK_XTALDRV_MID)                 ||                         \
        ((MODE) == CLK_XTALDRV_LOW)                 ||                         \
        ((MODE) == CLK_XTALDRV_ULOW))

/* Paramer valid check for XTAL stable time selection */
#define IS_VALID_CLK_XTALSTB_SEL(SEL)                                          \
(       ((SEL) == CLK_XTALSTB_8)                    ||                         \
        ((SEL) == CLK_XTALSTB_9 )                   ||                         \
        ((SEL) == CLK_XTALSTB_10)                   ||                         \
        ((SEL) == CLK_XTALSTB_11)                   ||                         \
        ((SEL) == CLK_XTALSTB_13)                   ||                         \
        ((SEL) == CLK_XTALSTB_15)                   ||                         \
        ((SEL) == CLK_XTALSTB_16)                   ||                         \
        ((SEL) == CLK_XTALSTB_17))

/* Paramer valid check for XTALSTD state */
#define IS_VALID_CLK_XTALSTD_STATE(STA)                                        \
(       ((STA) == CLK_XTALSTD_OFF)                  ||                         \
        ((STA) == CLK_XTALSTD_ON))

/* Paramer valid check for XTALSTD mode */
#define IS_VALID_CLK_XTALSTD_MODE(MD)                                          \
(       ((MD) == CLK_XTALSTD_MODE_RST)            ||                           \
        ((MD) == CLK_XTALSTD_MODE_INT))

/* Paramer valid check for XTALSTD interrupt state */
#define IS_VALID_CLK_XTALSTD_INT_STATE(STA)                                    \
(       ((STA) == CLK_XTALSTD_INT_OFF)              ||                         \
        ((STA) == CLK_XTALSTD_INT_ON))

/* Paramer valid check for XTALSTD reset state */
#define IS_VALID_CLK_XTALSTD_RST_STATE(STA)                                    \
(       ((STA) == CLK_XTALSTD_RST_OFF)              ||                         \
        ((STA) == CLK_XTALSTD_RST_ON))

/* Paramer valid check for HRC state */
#define IS_VALID_CLK_HRC_STATE(STA)                                            \
(       ((STA) == CLK_HRC_OFF)                      ||                         \
        ((STA) == CLK_HRC_ON))

/* Paramer valid check for HRC state */
#define IS_VALID_CLK_HRC_FREQ_SEL(SEL)                                         \
 (      ((SEL) == CLK_HRCFREQ_32)                   ||                         \
        ((SEL) == CLK_HRCFREQ_16)                   ||                         \
        ((SEL) == CLK_HRCFREQ_8)                    ||                         \
        ((SEL) == CLK_HRCFREQ_4)                    ||                         \
        ((SEL) == CLK_HRCFREQ_2)                    ||                         \
        ((SEL) == CLK_HRCFREQ_1))

/* Paramer valid check for LRC state */
#define IS_VALID_CLK_LRC_STATE(STA)                                            \
(       ((STA) == CLK_LRC_OFF)                      ||                         \
        ((STA) == CLK_LRC_ON))

/* Paramer valid check for HRC frequency selection */
#define IS_VALID_CLK_HRCFREQ_SEL(SEL)                                          \
(       ((SEL) == CLK_HRCFREQ_32)                   ||                         \
        ((SEL) == CLK_HRCFREQ_16)                   ||                         \
        ((SEL) == CLK_HRCFREQ_8)                    ||                         \
        ((SEL) == CLK_HRCFREQ_4)                    ||                         \
        ((SEL) == CLK_HRCFREQ_2)                    ||                         \
        ((SEL) == CLK_HRCFREQ_1)

/* Paramer valid check for system clock source */
#define IS_VALID_CLK_SYSCLK_SRC(SRC)                                           \
(       ((SRC) == CLK_SYSCLKSOURCE_HRC)             ||                         \
        ((SRC) == CLK_SYSCLKSOURCE_XTAL)            ||                         \
        ((SRC) == CLK_SYSCLKSOURCE_LRC))

/* Paramer valid check for system clock divider */
#define IS_VALID_CLK_SYSCLK_DIV(DIV)                                           \
(       ((DIV) == CLK_SYSCLK_DIV1)                  ||                         \
        ((DIV) == CLK_SYSCLK_DIV2)                  ||                         \
        ((DIV) == CLK_SYSCLK_DIV4)                  ||                         \
        ((DIV) == CLK_SYSCLK_DIV8)                  ||                         \
        ((DIV) == CLK_SYSCLK_DIV16)                 ||                         \
        ((DIV) == CLK_SYSCLK_DIV32)                 ||                         \
        ((DIV) == CLK_SYSCLK_DIV64))

/* Paramer valid check for hclk divider */
#define IS_VALID_HCLK_DIV(DIV)                                                 \
(       ((DIV) == CLK_HCLK_DIV1)                    ||                         \
        ((DIV) == CLK_HCLK_DIV2)                    ||                         \
        ((DIV) == CLK_HCLK_DIV4)                    ||                         \
        ((DIV) == CLK_HCLK_DIV8)                    ||                         \
        ((DIV) == CLK_HCLK_DIV16)                   ||                         \
        ((DIV) == CLK_HCLK_DIV32))

/* Parameter valid check for CLK MCO clock source. */
#define IS_VALID_CLK_MCOSOURCE(SOURCE)                                         \
(       ((SOURCE) == CLK_MCOSOURCCE_HRC)            ||                         \
        ((SOURCE) == CLK_MCOSOURCCE_LRC)            ||                         \
        ((SOURCE) == CLK_MCOSOURCCE_XTAL)           ||                         \
        ((SOURCE) == CLK_MCOSOURCCE_SYSCLK))

/* Parameter valid check for CLK MCO clock Prescaler. */
#define IS_VALID_CLK_MCODIV(DIV)                                               \
(       ((DIV) == CLK_MCODIV_1)                     ||                         \
        ((DIV) == CLK_MCODIV_2)                     ||                         \
        ((DIV) == CLK_MCODIV_4)                     ||                         \
        ((DIV) == CLK_MCODIV_8)                     ||                         \
        ((DIV) == CLK_MCODIV_16)                    ||                         \
        ((DIV) == CLK_MCODIV_32)                    ||                         \
        ((DIV) == CLK_MCODIV_64)                    ||                         \
        ((DIV) == CLK_MCODIV_128))

/* Parameter valid check for CLK stable flag. */
#define IS_VALID_CLK_STB_FLAG(flag)                                            \
(       ((flag) == CLK_FLAG_HRCSTB)                 ||                         \
        ((flag) == CLK_FLAG_XTALSTB))
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
 * @defgroup CLK_Global_Functions CLK Global Functions
 * @{
 */
/**
  * @brief  Resets the CLK clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - LRC HRC ON and HRC used as system clock source
  *            - XTAL OFF
  *            - SysClk divider set to 1.
  *            - MCO OFF
  *            - All interrupts disabled
  * @param  None
  * @retval None
  */
void CLK_DeInit(void)
{
    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    /* Reset peripheral clock */
    M0P_CMU->FCG = 0xFFFFFFFFu;

    /* Reset system clock / HCLK divider */
    M0P_CMU->SCKDIVR = 0x00u;
    M0P_CMU->PERICKSEL = 0x00u;

    /* Reset MCO */
    M0P_CMU->MCO1CFGR = 0x00u;

    /* Reset the system source */
    M0P_CMU->CKSWR = 0x00u;

    /* Reset HRC LRC XTAL */
    M0P_CMU->HRCCR = 0x00u;
    M0P_CMU->LRCCR = 0x00u;
    M0P_CMU->XTALCR = 0x01u;

    /* Reset XTALSTD */
    M0P_CMU->XTALSTDCR = 0x00u;

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();
}

/**
 * @brief  Init Xtal initial structure with default value.
 * @param  pstcXtal specifies the paramer of XTAL.
 *            @arg    u8XtalState  : The new state of the XTAL.
 *            @arg    u8XtalMode   : The XTAL mode selection osc or exclk.
 *            @arg    u8XtalDrv    : The XTAL drive ability.
 *            @arg    u8XtalSupDrv : The XTAL super drive on or off.
 *            @arg    u8XtalStb    : The XTAL stable time selection.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t CLK_XtalStrucInit(stc_clk_xtal_init_t* pstcXtal)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcXtal)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcXtal->u8XtalState   = CLK_XTAL_OFF;
        pstcXtal->u8XtalMode    = CLK_XTALMODE_OSC;
        pstcXtal->u8XtalDrv     = CLK_XTALDRV_HIGH;
        pstcXtal->u8XtalSupDrv  = CLK_XTAL_SUPDRV_ON;
        pstcXtal->u8XtalStb     = CLK_XTALSTB_8;
    }

    return enRet;
}

/**
  * @brief  Initialise the XTAL.
  * @param  pstcXtal specifies the paramer of XTAL.
  *            @arg    u8XtalState  : The new state of the XTAL.
  *            @arg    u8XtalMode   : The XTAL mode selection osc or exclk.
  *            @arg    u8XtalDrv    : The XTAL drive ability.
  *            @arg    u8XtalSupDrv : The XTAL super drive on or off.
  *            @arg    u8XtalStb    : The XTAL stable time selection.
  * @retval en_result_t Status
  */
en_result_t CLK_XTALInit(const stc_clk_xtal_init_t* pstcXtal)
{
    uint32_t u32timeout = 0ul;
    en_result_t enRet = Ok;

    if (pstcXtal == NULL)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Paramers check */
        DDL_ASSERT(IS_VALID_CLK_XTAL_STATE(pstcXtal->u8XtalState));

        /* Enable register write. */
        CLK_REG_WRITE_ENABLE();

        if(CLK_XTAL_OFF == pstcXtal->u8XtalState)
        {
            /* When the XTAL is used as system clock in these case XTAL will not disabled */
            if(CLK_SYSCLKSOURCE_XTAL == READ_REG8_BIT(M0P_CMU->CKSWR, CMU_CKSWR_CKSW))
            {
                enRet = Error;
            }
            else
            {
                /* Disable XTAL */
                M0P_CMU->XTALCR = CLK_XTAL_OFF;
                /* Wait XTAL stop */
                while(CLK_FLAG_XTALSTB == READ_REG8_BIT(M0P_CMU->OSCSTBSR, CMU_OSCSTBSR_XTALSTBF))
                {
                    u32timeout++;
                    if(u32timeout > CLK_XTAL_TIMEOUT)
                    {
                        enRet = ErrorTimeout;
                    }
                    else
                    {
                        /* code */
                    }
                }
            }
        }
        else
        {
            /* Paramers check */
            DDL_ASSERT(IS_VALID_CLK_XTAL_MODE(pstcXtal->u8XtalMode));
            DDL_ASSERT(IS_VALID_CLK_XTALDRV_MODE(pstcXtal->u8XtalDrv));
            DDL_ASSERT(IS_VALID_CLK_XTAL_SUPDRV_STATE(pstcXtal->u8XtalSupDrv));
            DDL_ASSERT(IS_VALID_CLK_XTALSTB_SEL(pstcXtal->u8XtalStb));

            /* Configure XTAL mode, driver ability, super_driver */
            M0P_CMU->XTALCFGR = pstcXtal->u8XtalDrv | pstcXtal->u8XtalMode | pstcXtal->u8XtalSupDrv;
            /* Configure XTAL stable time */
            M0P_CMU->XTALSTBCR = pstcXtal->u8XtalStb;
            /* Enable XTAL */
            M0P_CMU->XTALCR = CLK_XTAL_ON;
            /* Wait XTAL stable */
            while(CLK_FLAG_XTALSTB != READ_REG8_BIT(M0P_CMU->OSCSTBSR, CMU_OSCSTBSR_XTALSTBF))
            {
                u32timeout++;
                if(u32timeout > CLK_XTAL_TIMEOUT)
                {
                    enRet = ErrorTimeout;
                }
                else
                {
                    /* code */
                }
            }
        }

        /* Disbale register write. */
        CLK_REG_WRITE_DISABLE();
    }

    return enRet;
}

/**
 * @brief  Initialise the HRC.
 * @param  HRCState specifies the new state of HRC.
 *           This parameter can be one of the following values:
 *            @arg    CLK_HRC_OFF  : Disable HRC.
 *            @arg    CLK_HRC_ON   : Enable HRC.
 * @param  HRCFreq  specifies the frequency of HRC.
 *           This parameter can be one of the following values:
 *            @arg    CLK_HRCFREQ_32 : HRC frequency is 32MHz while ICG.HRCFREQS[3] = 0.
 *            @arg    CLK_HRCFREQ_16 : HRC frequency is 16MHz while ICG.HRCFREQS[3] = 0.
 *            @arg    CLK_HRCFREQ_8  : HRC frequency is 8MHz while ICG.HRCFREQS[3] = 0.
 *            @arg    CLK_HRCFREQ_4  : HRC frequency is 4MHz while ICG.HRCFREQS[3] = 0.
 *            @arg    CLK_HRCFREQ_2  : HRC frequency is 2MHz while ICG.HRCFREQS[3] = 0.
 *            @arg    CLK_HRCFREQ_1  : HRC frequency is 1MHz while ICG.HRCFREQS[3] = 0.
 *            @arg    CLK_HRCFREQ_48 : HRC frequency is 48MHz while ICG.HRCFREQS[3] = 1.
 *            @arg    CLK_HRCFREQ_24 : HRC frequency is 24MHz while ICG.HRCFREQS[3] = 1.
 *            @arg    CLK_HRCFREQ_12 : HRC frequency is 12MHz while ICG.HRCFREQS[3] = 1.
 *            @arg    CLK_HRCFREQ_6  : HRC frequency is 6MHz while ICG.HRCFREQS[3] = 1.
 *            @arg    CLK_HRCFREQ_3  : HRC frequency is 3MHz while ICG.HRCFREQS[3] = 1.
 *            @arg    CLK_HRCFREQ_1P5: HRC frequency is 1.5MHz while ICG.HRCFREQS[3] = 1.
 * @retval en_result_t Status
 */
en_result_t CLK_HRCInit(uint8_t HRCState, uint8_t HRCFreq)
{
    uint32_t u32timeout = 0ul;
    en_result_t enRet = Ok;

    /* Paramers check */
    DDL_ASSERT(IS_VALID_CLK_HRC_STATE(HRCState));

    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    if(CLK_HRC_OFF == HRCState)
    {
        /* When the HRC is used as system clock in these case HRC will not disabled */
        if(CLK_SYSCLKSOURCE_HRC == READ_REG8_BIT(M0P_CMU->CKSWR, CMU_CKSWR_CKSW))
        {
            enRet = Error;
        }
        else
        {
            /* Disable HRC */
            M0P_CMU->HRCCR = CLK_HRC_OFF;
            /* Wait HRC stop */
            while(CLK_FLAG_HRCSTB == READ_REG8_BIT(M0P_CMU->OSCSTBSR, CMU_OSCSTBSR_HRCSTBF))
            {
                u32timeout++;
                if(u32timeout > CLK_TIMEOUT)
                {
                    enRet = ErrorTimeout;
                }
                else
                {
                    /* code */
                }
            }
        }
    }
    else
    {
        /* Paramers check */
        DDL_ASSERT(IS_VALID_CLK_HRC_FREQ_SEL(HRCFreq));

        /* Enable CMU_HRCCFGR register write */
        CLK_HRCCFGR_WRITE_ENBALE();

        /* Configure HRC frequency */
        M0P_EFM->HRCCFGR = HRCFreq;

        /* Disbale CMU_HRCCFGR register write */
        CLK_HRCCFGR_WRITE_DISABLE();

        /* Enable HRC */
        M0P_CMU->HRCCR = CLK_HRC_ON;
        /* Wait HRC stable */
        while(CLK_FLAG_HRCSTB != READ_REG8_BIT(M0P_CMU->OSCSTBSR, CMU_OSCSTBSR_HRCSTBF))
        {
            u32timeout++;
            if(u32timeout > CLK_TIMEOUT)
            {
                enRet = ErrorTimeout;
            }
            else
            {
                /* code */
            }
        }
    }

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();

    /* Update system clock */
    SystemCoreClockUpdate();

    return enRet;
}

/**
 * @brief  Initialise the LRC.
 * @param  LRCState specifies the new state of LRC.
 *           This parameter can be one of the following values:
 *            @arg    CLK_LRC_OFF  : Disable LRC.
 *            @arg    CLK_LRC_ON   : Enable LRC.
 * @retval en_result_t Status
 */
en_result_t CLK_LRCInit(uint8_t LRCState)
{
    en_result_t enRet = Ok;

    /* Paramers check */
    DDL_ASSERT(IS_VALID_CLK_LRC_STATE(LRCState));

    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    if(CLK_LRC_OFF == LRCState)
    {
        /* When the LRC is used as system clock in these case LRC will not disabled */
        if(CLK_SYSCLKSOURCE_LRC == READ_REG8_BIT(M0P_CMU->CKSWR, CMU_CKSWR_CKSW))
        {
            enRet = Error;
        }
        else
        {
            /* Disable LRC */
            M0P_CMU->LRCCR = CLK_LRC_OFF;
        }
    }
    else
    {
        /* Enable LRC */
        M0P_CMU->LRCCR = CLK_LRC_ON;
    }

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();

    return enRet;
}

/**
 * @brief  Init XtalStd initial structure with default value.
 * @param  pstcXtalStd specifies the paramer of XTALSTD.
 *            @arg    u8XtalStdState:   The new state of the XTALSTD.
 *            @arg    u8XtalStdMode:    The XTAL status detection occur interrupt or reset.
 *            @arg    u8XtalStdInt:     The XTAL status detection interrupt on or off.
 *            @arg    u8XtalStdRst:     The XTAL status detection reset on or off.
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: Invalid parameter
 */
en_result_t CLK_XtalStdStrucInit(stc_clk_xtalstd_init_t* pstcXtalStd)
{
    en_result_t enRet = Ok;

    /* Check if pointer is NULL */
    if (NULL == pstcXtalStd)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Configure to default value */
        pstcXtalStd->u8XtalStdState = CLK_XTALSTD_OFF;
        pstcXtalStd->u8XtalStdMode  = CLK_XTALSTD_MODE_INT;
        pstcXtalStd->u8XtalStdInt   = CLK_XTALSTD_RST_OFF;
        pstcXtalStd->u8XtalStdRst   = CLK_XTALSTD_INT_OFF;
    }

    return enRet;
}

/**
 * @brief  Initialise the XTAL status detection.
 * @param  pstcXtalStd specifies the paramer of XTALSTD.
 *            @arg    u8XtalStdState:   The new state of the XTALSTD.
 *            @arg    u8XtalStdMode:    The XTAL status detection occur interrupt or reset.
 *            @arg    u8XtalStdInt:     The XTAL status detection interrupt on or off.
 *            @arg    u8XtalStdRst:     The XTAL status detection reset on or off.
 * @retval None
 */
en_result_t CLK_XTALStdInit(const stc_clk_xtalstd_init_t* pstcXtalStd)
{
    en_result_t enRet = Ok;

    if (pstcXtalStd == NULL)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Paramer valid check */
        DDL_ASSERT(IS_VALID_CLK_XTALSTD_STATE(pstcXtalStd->u8XtalStdState));

        /* Enable register write. */
        CLK_REG_WRITE_ENABLE();

        if(CLK_XTALSTD_OFF == pstcXtalStd->u8XtalStdState)
        {
            /* Disbale XTAL status detection */
            CLEAR_REG8_BIT(M0P_CMU->XTALSTDCR, CMU_XTALSTDCR_XTALSTDE);
        }
        else
        {
            /* Paramer valid check */
            DDL_ASSERT(IS_VALID_CLK_XTALSTD_MODE(pstcXtalStd->u8XtalStdMode));
            DDL_ASSERT(IS_VALID_CLK_XTALSTD_INT_STATE(pstcXtalStd->u8XtalStdInt));
            DDL_ASSERT(IS_VALID_CLK_XTALSTD_RST_STATE(pstcXtalStd->u8XtalStdRst));

            /* Confiure XTALSTD and enable XTALSTD */
            M0P_CMU->XTALSTDCR = pstcXtalStd->u8XtalStdState | pstcXtalStd->u8XtalStdMode | \
                                pstcXtalStd->u8XtalStdInt   | pstcXtalStd->u8XtalStdRst;
        }
        /* Disbale register write. */
        CLK_REG_WRITE_DISABLE();
    }

    return enRet;
}

/**
 * @brief  Set the system clock source.
 * @param  u8Src specifies the source of system clock.
 *          This parameter can be one of the following values:
 *            @arg    CLK_SYSCLKSOURCE_HRC    : select HRC as system clock source
 *            @arg    CLK_SYSCLKSOURCE_XTAL   : select XTAL as system clock source
 *            @arg    CLK_SYSCLKSOURCE_LRC    : select LRC as system clock source
 * @retval None
 */
void CLK_SetSysclkSrc(uint8_t u8Src)
{
    DDL_ASSERT(IS_VALID_CLK_SYSCLK_SRC(u8Src));

    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    /* Set system clock divider */
    MODIFY_REG8(M0P_CMU->CKSWR, CMU_CKSWR_CKSW, u8Src);

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();

    /* Update system clock */
    SystemCoreClockUpdate();
}

/**
 * @brief  Set the system clock divider.
 * @param  u8Div specifies the divider of system clock.
 *          This parameter can be one of the following values:
 *            @arg    CLK_SYSCLK_DIV1 : system clock 1 divider
 *            @arg    CLK_SYSCLK_DIV2 : system clock 2 divider
 *            @arg    CLK_SYSCLK_DIV4 : system clock 4 divider
 *            @arg    CLK_SYSCLK_DIV8 : system clock 8 divider
 *            @arg    CLK_SYSCLK_DIV16: system clock 16 divider
 *            @arg    CLK_SYSCLK_DIV32: system clock 32 divider
 * @retval None
 */
void CLK_SetSysclkDiv(uint8_t u8Div)
{
    DDL_ASSERT(IS_VALID_CLK_SYSCLK_DIV(u8Div));

    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    /* Set system clock divider */
    MODIFY_REG8(M0P_CMU->SCKDIVR, CMU_SCKDIVR_SCKDIV, u8Div);

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();

    /* Update system clock */
    SystemCoreClockUpdate();
}

/**
 * @brief  Set the HCLK divider witch used as ADC clock.
 * @param  u8Div specifies the divider of HCLK.
 *          This parameter can be one of the following values:
 *            @arg    CLK_HCLK_DIV1 : HCLK 1 divider
 *            @arg    CLK_HCLK_DIV2 : HCLK 2 divider
 *            @arg    CLK_HCLK_DIV4 : HCLK 4 divider
 *            @arg    CLK_HCLK_DIV8 : HCLK 8 divider
 *            @arg    CLK_HCLK_DIV16: HCLK 16 divider
 *            @arg    CLK_HCLK_DIV32: HCLK 32 divider
 * @retval None
 */
void CLK_SetADClkDiv(uint8_t u8Div)
{
    DDL_ASSERT(IS_VALID_HCLK_DIV(u8Div));

    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    /* Set system clock divider */
    MODIFY_REG8(M0P_CMU->PERICKSEL, CMU_PERICKSEL_PERICKSEL, u8Div);

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();
}

/**
 * @brief  Enable or disable the FCG peripheral clock.
 * @note   After reset,the peripheral clock is disabled and the application
 *         software has to enable this clock before using it.
 * @param  u32FcgPeriph specifies the peripheral in FCG.
 *          This parameter can be one of the following values:
 *            @arg CLK_FCG_AD:       ADC clock
 *            @arg CLK_FCG_CT:       CTC clock
 *            @arg CLK_FCG_CM:       CMP clock
 *            @arg CLK_FCG_AO:       AOS clock
 *            @arg CLK_FCG_DMA:      DMA clock
 *            @arg CLK_FCG_CR:       CRC clock
 *            @arg CLK_FCG_TIMB1:    TIMB1 clock
 *            @arg CLK_FCG_TIMB2:    TIMB2 clock
 *            @arg CLK_FCG_TIMB3:    TIMB3 clock
 *            @arg CLK_FCG_TIMB4:    TIMB4 clock
 *            @arg CLK_FCG_TIM0:     TIM0 clock
 *            @arg CLK_FCG_TIM2:     TIM2 clock
 *            @arg CLK_FCG_TIM4:     TIM4 clock
 *            @arg CLK_FCG_TIMA:     TIMA clock
 *            @arg CLK_FCG_EMB:      EMB clock
 *            @arg CLK_FCG_UART1:    UART1 clock
 *            @arg CLK_FCG_UART2:    UART2 clock
 *            @arg CLK_FCG_UART3:    UART3 clock
 *            @arg CLK_FCG_I2C:      I2C clock
 *            @arg CLK_FCG_SPI:      SPI clock
 *
 * @param  enNewState specifies the new state of the clock output.
 *          This parameter can be one of the following values:
 *            @arg Enable:   Enable clock output.
 *            @arg Disable:  Disable clock output.
 * @retval None
 */
void CLK_FcgPeriphClockCmd(uint32_t u32FcgPeriph, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    if(Enable == enNewState)
    {
        M0P_CMU->FCG &= ~u32FcgPeriph;
    }
    else
    {
        M0P_CMU->FCG |= u32FcgPeriph;
    }

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();
}

/**
 * @brief  Selects the clock source to output on MCO pin.
 * @note   P15 should be configured in alternate function mode.
 * @param  CLK_MCOSource specifies the clock source to output.
 *          This parameter can be one of the following values:
 *            @arg CLK_MCOSOURCCE_HRC:       HRC clock selected as MCOx source
 *            @arg CLK_MCOSOURCCE_LRC:       LRC clock selected as MCOx source
 *            @arg CLK_MCOSOURCCE_XTAL:      XTAL clock selected as MCOx source
 *            @arg CLK_MCOSOURCCE_SYSCLK:    System clock(SYSCLK) selected as MCOx source
 * @param  CLK_MCODiv specifies the MCOx prescaler.
 *          This parameter can be one of the following values:
 *            @arg CLK_MCODIV_1:   no division applied to MCOx clock
 *            @arg CLK_MCODIV_2:   division by 2 applied to MCOx clock
 *            @arg CLK_MCODIV_4:   division by 4 applied to MCOx clock
 *            @arg CLK_MCODIV_8:   division by 8 applied to MCOx clock
 *            @arg CLK_MCODIV_16:  division by 16 applied to MCOx clock
 *            @arg CLK_MCODIV_32:  division by 32 applied to MCOx clock
 *            @arg CLK_MCODIV_64:  division by 64 applied to MCOx clock
 *            @arg CLK_MCODIV_128: division by 128 applied to MCOx clock
 * @retval None
 */
void CLK_MCOConfig(uint8_t CLK_MCOSource, uint8_t CLK_MCODiv)
{
    /* Check the parameters. */
    DDL_ASSERT(IS_VALID_CLK_MCOSOURCE(CLK_MCOSource));
    DDL_ASSERT(IS_VALID_CLK_MCODIV(CLK_MCODiv));

    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    /* Configure the MCO and Enable MCO */
    M0P_CMU->MCO1CFGR = (CMU_MCO1CFGR_MCO1EN | CLK_MCODiv | CLK_MCOSource);

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();
}

/**
 * @brief  Enable or Disable MCO.
 * @param  enNewState specifies the new state of the clock output.
 *          This parameter can be one of the following values:
 *            @arg Enable:   Enable clock output.
 *            @arg Disable:  Disable clock output.
 * @retval None
 */
void CLK_MCOCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    /* Enable or disable clock output. */
    bM0P_CMU->MCO1CFGR_b.MCO1EN = enNewState;

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();
}

/**
 * @brief  Clear the XTAL error flag.
 * @param  None
 * @retval None
 * @note   The system clock should not be XTAL before call this function.
 */
void CLK_ClearXtalStdFlag(void)
{
    /* Enable register write. */
    CLK_REG_WRITE_ENABLE();

    if(CMU_XTALSTDSR_XTALSTDF == M0P_CMU->XTALSTDSR)
    {
        /* Clear the XTAL STD flag */
        CLEAR_REG8_BIT(M0P_CMU->XTALSTDSR, CMU_XTALSTDSR_XTALSTDF);
    }

    /* Disbale register write. */
    CLK_REG_WRITE_DISABLE();
}
/**
 * @brief  Get the clock stable flag.
 * @param  u8Flag  specifies the clock stable flag.
 *          This parameter can be one of the following values:
 *            @arg CLK_FLAG_HRCSTB:       HRC clock stable flag
 *            @arg CLK_FLAG_XTALSTB:      XTAL clock stable flag
 * @retval en_flag_status_t
 */
en_flag_status_t CLK_GetStableFlag(uint8_t u8Flag)
{
    en_flag_status_t enFlagStatus = Reset;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_CLK_STB_FLAG(u8Flag));

    if (Reset != (READ_REG8_BIT(M0P_CMU->OSCSTBSR, u8Flag)))
    {
        enFlagStatus = Set;
    }

    return enFlagStatus;
}

/**
 * @}
 */

#endif /* DDL_CLK_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
