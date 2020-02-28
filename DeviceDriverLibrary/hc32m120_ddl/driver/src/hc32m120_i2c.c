/**
 *******************************************************************************
 * @file  hc32m120_i2c.c
 * @brief This file provides firmware functions to manage the Inter-Integrated
 *        Circuit(I2C).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-09       Wangmin         First version
   2020-02-28       Wangmin         Modify after hc32_common.h changed.
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
#include "hc32m120_i2c.h"
#include "hc32m120_utility.h"

/**
 * @addtogroup HC32M120_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_I2C I2C
 * @brief I2C Driver Library
 * @{
 */

#if (DDL_I2C_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup I2C_Local_Macros I2C Local Macros
 * @{
 */

#define I2C_BAUDRATE_MAX                400000ul

#define I2C_CLR_MASK                   ((uint32_t)0x00F012DF)

/**
 * @defgroup I2C_Check_Parameters_Validity I2C Check Parameters Validity
 * @{
 */
#define IS_VALID_CLEARBIT(x)            (0u == ((x) & (~I2C_CLR_MASK)))

#define IS_VALID_SPEED(speed)           ((speed) <= (I2C_BAUDRATE_MAX))

#define IS_VALID_DIGITAL_FILTER(x)             ((x) <= I2C_DIG_FILTMODE_4CYCLE)

#define IS_VALID_RD_STATUS_BIT(x)                                              \
(   ((x) == I2C_SR_STARTF)                         ||                          \
    ((x) == I2C_SR_SLADDR0F)                       ||                          \
    ((x) == I2C_SR_SLADDR1F)                       ||                          \
    ((x) == I2C_SR_TENDF)                          ||                          \
    ((x) == I2C_SR_STOPF)                          ||                          \
    ((x) == I2C_SR_RFULLF)                         ||                          \
    ((x) == I2C_SR_TEMPTYF)                        ||                          \
    ((x) == I2C_SR_ARLOF)                          ||                          \
    ((x) == I2C_SR_ACKRF)                          ||                          \
    ((x) == I2C_SR_NACKF)                          ||                          \
    ((x) == I2C_SR_MSL)                            ||                          \
    ((x) == I2C_SR_BUSY)                           ||                          \
    ((x) == I2C_SR_TRA)                            ||                          \
    ((x) == I2C_SR_GENCALLF)                       ||                          \
    ((x) == I2C_SR_SMBDEFAULTF)                    ||                          \
    ((x) == I2C_SR_SMBHOSTF)                       ||                          \
    ((x) == I2C_SR_SMBALRTF))

#define IS_VALID_WR_STATUS_BIT(x)                                              \
(   ((x) == I2C_SR_MSL)                            ||                          \
    ((x) == I2C_SR_TRA))

#define IS_VALID_SMBUS_CONFIG(x)       ( 0u == ((x) & I2C_SMBUS_CONFIG_CLEARMASK))

#define IS_VALID_ADRCONFIG(x)                                                  \
    (0u == ((x) & ~(I2C_SLR0_ADDRMOD0 | I2C_SLR0_SLADDR0EN)))

#define IS_VALID_7BIT_ADR(x)           ((x) <= 0x7Fu)
#define IS_VALIDE_10BIT_ADR(x)          ((x) <= 0x3FFu)

#define IS_VALID_ADR_NUM(x)                                                    \
(   ((x) == I2C_ADR_0)                             ||                          \
    ((x) == I2C_ADR_1))

#define IS_VALID_CLK_DIV(x)                                                    \
(   ((x) == I2C_CLK_DIV1)                          ||                          \
    ((x) == I2C_CLK_DIV2)                          ||                          \
    ((x) == I2C_CLK_DIV4)                          ||                          \
    ((x) == I2C_CLK_DIV8)                          ||                          \
    ((x) == I2C_CLK_DIV16)                         ||                          \
    ((x) == I2C_CLK_DIV32)                         ||                          \
    ((x) == I2C_CLK_DIV64)                         ||                          \
    ((x) == I2C_CLK_DIV128))

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
 * @defgroup I2C_Global_Functions I2C Global Functions
 * @{
 */

/**
 * @brief  Set the baudrate for I2C peripheral.
 * @param  [in] pstcI2C_InitStruct   Pointer to I2C configuration structure
 *                                   @ref stc_i2c_init_t
 *         @arg pstcI2C_InitStruct->u32I2cClkDiv: Division of Hclk, reference as:
 *              step1: calculate div = (Hclk/Baudrate/(68+2*dnfsum+SclTime)
 *                     Hclk -- system clock
 *                     Baudrate -- baudrate of i2c
 *                     SclTime -- =(SCL rising time + SCL falling time)/period of i2cclok
 *                                according to i2c bus hardware parameter.
 *                     dnfsum -- 0 if digital filter off;
 *                               Filter capacity if digital filter on(1 ~ 4)
 *              step2: chose a division item which is similar and bigger than div
 *                     from @ref I2C_Clock_division.
 *         @arg pstcI2C_InitStruct->u32Baudrate : Baudrate configuration
 *         @arg pstcI2C_InitStruct->u32SclTime : Indicate SCL pin rising and
 *              falling time, should be number of T(i2c clock period time)
 * @param  [in] pf32Err               Baudrate error
 * @retval en_result_t
 */
en_result_t I2C_BaudrateConfig(const stc_i2c_init_t* pstcI2C_InitStruct, float32_t *pf32Err)
{
    en_result_t enRet = Ok;
    uint32_t Hclk, I2cDivClk, SclCnt, Baudrate;
    uint32_t dnfsum = 0ul, divsum = 0ul;
    float32_t WidthTotal, SumTotal;
    float32_t WidthHL;
    float32_t fErr = 0.0f;

    if ((NULL == pstcI2C_InitStruct)||(NULL == pf32Err))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_SPEED(pstcI2C_InitStruct->u32Baudrate));
        DDL_ASSERT(IS_VALID_CLK_DIV(pstcI2C_InitStruct->u32I2cClkDiv));

        /* Get configuration for i2c */
        Hclk = SystemCoreClock;
        I2cDivClk = 1ul << pstcI2C_InitStruct->u32I2cClkDiv;
        SclCnt = pstcI2C_InitStruct->u32SclTime;
        Baudrate = pstcI2C_InitStruct->u32Baudrate;

        /* Judge digital filter status*/
        if(0u != READ_REG32_BIT(M0P_I2C->FLTR, I2C_FLTR_DNFEN))
        {
            dnfsum = ((M0P_I2C->FLTR & I2C_FLTR_DNF) >> I2C_FLTR_DNF_POS) + 1u;
        }
        else
        {
            dnfsum = 0ul;
        }

        /* Judge if clock divider on*/
        if(I2C_CLK_DIV1 == I2cDivClk)
        {
            divsum = 3ul;
        }
        else
        {
            divsum = 2ul;
        }

        WidthTotal = (float32_t)Hclk/(float32_t)Baudrate/(float32_t)I2cDivClk;
        SumTotal = 2.0f*(float32_t)divsum + 2.0f*(float32_t)dnfsum + (float32_t)SclCnt;
        WidthHL = WidthTotal - SumTotal;

        if(WidthTotal <= SumTotal)
        {
            /* Err, Should set a smaller division value for pstcI2C_InitStruct->u32I2cClkDiv */
            //DDL_ASSERT(NULL);
            enRet = ErrorInvalidParameter;
        }
        else if(WidthHL > ((float32_t)0x1F*(float32_t)2))
        {
            /* Err, Should set a bigger division value for pstcI2C_InitStruct->u32I2cClkDiv */
            //DDL_ASSERT(NULL);
            enRet = ErrorInvalidParameter;
        }
        else
        {
            fErr =(WidthHL - (float32_t)((uint32_t)WidthHL)) / WidthHL;

            M0P_I2C->CCR = (pstcI2C_InitStruct->u32I2cClkDiv << I2C_CCR_CKDIV_POS)     \
                           | (((uint32_t)WidthHL/2u) << I2C_CCR_SLOWW_POS)              \
                           | (((uint32_t)WidthHL - (uint32_t)WidthHL/2u) << I2C_CCR_SHIGHW_POS);
        }
    }
    if((NULL != pf32Err)&&(Ok == enRet))
    {
        *pf32Err = fErr;
    }
    return enRet;
}

/**
 * @brief  De-initialize I2C unit
 *
 * @retval None
 */
void I2C_DeInit(void)
{
    /* Reset peripheral register and internal status*/
    bM0P_I2C->CR1_b.PE = 0u;
    bM0P_I2C->CR1_b.SWRST = 1u;
}

/**
 * @brief  Initialize I2C peripheral according to the structure
 * @param  [in] pstcI2C_InitStruct   Pointer to I2C configuration structure
 *                                   @ref stc_i2c_init_t
 *         @arg pstcI2C_InitStruct->u32I2cClkDiv: Division of Hclk, reference as:
 *              step1: calculate div = (Hclk/Baudrate/(68+2*dnfsum+SclTime)
 *                     Hclk -- system clock
 *                     Baudrate -- baudrate of i2c
 *                     SclTime -- =(SCL rising time + SCL falling time)/period of i2cclok
 *                                according to i2c bus hardware parameter.
 *                     dnfsum -- 0 if ditital filter off;
 *                               Filter capacity if ditital filter on(1 ~ 4)
 *              step2: chose a division item which is similar and bigger than div
 *                     from @ref I2C_Clock_division.
 *         @arg pstcI2C_InitStruct->u32Baudrate : Baudrate configuration
 *         @arg pstcI2C_InitStruct->u32SclTime : Indicate SCL pin rising and
 *              falling time, should be number of T(i2cclok period time)
 * @param  [in] pf32Err               Baudrate error
 * @retval en_result_t
 */
en_result_t I2C_Init(const stc_i2c_init_t* pstcI2C_InitStruct, float32_t *pf32Err)
{
    en_result_t enRet = Ok;
    if (NULL == pstcI2C_InitStruct )
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        DDL_ASSERT(IS_VALID_SPEED(pstcI2C_InitStruct->u32Baudrate));
        DDL_ASSERT(IS_VALID_CLK_DIV(pstcI2C_InitStruct->u32I2cClkDiv));

        /* Register and internal status reset */
        bM0P_I2C->CR1_b.PE = 0u;
        bM0P_I2C->CR1_b.SWRST = 1u;
        bM0P_I2C->CR1_b.PE = 1u;

        /* I2C baudrate config */
        I2C_BaudrateConfig(pstcI2C_InitStruct, pf32Err);

        /* Disable global broadcast address function */
        bM0P_I2C->CR1_b.GCEN = 1u;

        /* Release software reset */
        bM0P_I2C->CR1_b.SWRST = 0u;
        /* Disable I2C peripheral */
        bM0P_I2C->CR1_b.PE = 0u;
    }
    return enRet;
}

/**
 * @brief  I2C function command
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t

 * @retval None
 */
void I2C_Cmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_I2C->CR1_b.PE = enNewState;
}

/**
 * @brief  I2C SMBUS function configuration
 * @param  [in] SmbusConfig       Indicate the SMBUS function configuration
 *         This parameter can be one or any combination of the configure items,
 *         @ref I2C_Smbus_config
 *         @arg  I2C_SMBUS_ALRTEN    : Smbus alarm address enable
 *         @arg  I2C_SMBUS_DEFAULTEN : Smbus default address enable
 *         @arg  I2C_SMBUS_HOSTEN    : Smbus host address address enable
 *         @arg  I2C_SMBUS_DISEN     : Smbus mode disable
 * @retval None
 */
void I2C_SmbusConfig(uint32_t SmbusConfig)
{
    DDL_ASSERT(IS_VALID_SMBUS_CONFIG(SmbusConfig));

    MODIFY_REG32(M0P_I2C->CR1, I2C_SMBUS_CONFIG_CLEARMASK, SmbusConfig);
}

/**
 * @brief  I2C SMBUS function command
 * @param  [in] enNewState           New state of the I2Cx SMBUS function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_SmBusCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_I2C->CR1_b.SMBUS = enNewState;
}

/**
 * @brief  I2C software reset function command
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_SoftwareResetCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_I2C->CR1_b.SWRST = enNewState;
}

/**
 * @brief  I2C digital filter function configuration
 * @param  [in] DigFilterMode        Chose the digital filter mode,
 *                                   @ref I2C_Digital_Filter_mode
 * @retval None
 */
void I2C_DigitalFilterConfig(uint32_t DigFilterMode)
{
    DDL_ASSERT(IS_VALID_DIGITAL_FILTER(DigFilterMode));

    MODIFY_REG32(M0P_I2C->FLTR, I2C_FLTR_DNF, DigFilterMode);
}

/**
 * @brief  I2C digital filter function command
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_DigitalFilterCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_I2C->FLTR_b.DNFEN = enNewState;
}

/**
 * @brief  I2C analog filter function command
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_AnalogFilterCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_I2C->FLTR_b.ANFEN = enNewState;
}

/**
 * @brief  I2C general call function command
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_GeneralCallCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_I2C->CR1_b.GCEN = enNewState;
}

/**
 * @brief  I2C slave address config
 * @param  [in] u32AdrNum            I2C_ADR_0 or I2C_ADR_1 @ref I2C_Adr_Num
 * @param  [in] u32AdrConfig         Address mode configuration,
 *                                   @ref I2C_Adr_Config
 * @param  [in] u32Adr               The slave address
 * @retval None
 */
void I2C_SlaveAdrConfig(uint32_t u32AdrNum, uint32_t u32AdrConfig, uint32_t u32Adr)
{
    DDL_ASSERT(IS_VALID_ADRCONFIG(u32AdrConfig));
    DDL_ASSERT(IS_VALID_ADR_NUM(u32AdrNum));

    if(I2C_ADR_CONFIG_10BIT == u32AdrConfig)
    {
        /* if 10 bit address mode */
        DDL_ASSERT(IS_VALIDE_10BIT_ADR(u32Adr));
        *(__IO uint32_t*)(&M0P_I2C->SLR0 + u32AdrNum) = u32AdrConfig + u32Adr;
    }
    //else if(I2C_ADR_CONFIG_7BIT == u32AdrConfig)
    else
    {
        /* if 7 bit address mode */
        DDL_ASSERT(IS_VALID_7BIT_ADR(u32Adr));
        *(__IO uint32_t*)(&M0P_I2C->SLR0 + u32AdrNum) = u32AdrConfig + (u32Adr << 1);
    }
}

/**
 * @brief  I2C interrupt function command
 * @param  [in] u32IntEn   Specifies the I2C interrupts sources to be configuration
 *         This parameter can be any combination of the following values:
 *         @arg    I2C_CR2_STARTIE      : Start flag interrupt
 *         @arg    I2C_CR2_SLADDR0IE    : Address 0 detected interrupt
 *         @arg    I2C_CR2_SLADDR1IE    : Address 1 detected interrupt
 *         @arg    I2C_CR2_TENDIE       : Transfer end interrupt
 *         @arg    I2C_CR2_STOPIE       : Stop flag interrupt
 *         @arg    I2C_CR2_RFULLIE      : Receive buffer full interrupt
 *         @arg    I2C_CR2_TEMPTYIE     : Transfer buffer empty interrupt
 *         @arg    I2C_CR2_ARLOIE       : Arbitration fails interrupt
 *         @arg    I2C_CR2_NACKIE       : NACK flag detected interrupt
 *         @arg    I2C_CR2_GENCALLIE    : General call address detected interrupt
 *         @arg    I2C_CR2_SMBDEFAULTIE : Smbus default address detected interrupt
 *         @arg    I2C_CR2_SMBHOSTIE    : Smbus host address detected interrupt
 *         @arg    I2C_CR2_SMBALRTIE    : Smbus alarm address detected interrupt
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_IntCmd(uint32_t u32IntEn, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        M0P_I2C->CR2 |= u32IntEn;
    }
    else
    {
        M0P_I2C->CR2 &= (uint32_t)~u32IntEn;
    }
}

/**
 * @brief  I2C send data or address
 * @param  [in] u8Data               The data to be send
 *
 * @retval None
 */
void I2C_SendData( uint8_t u8Data)
{
    M0P_I2C->DTR = u8Data;
}

/**
 * @brief  I2C read data from register
 * @retval The value of the received data
 */
uint8_t I2C_ReadData(void)
{
    return (uint8_t)M0P_I2C->DRR;
}

/**
 * @brief  I2C ACK status configuration
 * @param  [in] enNewState           New state of the I2Cx function, can be
 *                                   Disable or Enable the function
 * @retval None
 */
void I2C_NackConfig(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_I2C->CR1_b.ACK = enNewState;
}

/**
 * @brief  I2C status bit get
 * @param  [in] u32StatusBit         specifies the flag to check,
 *         This parameter can be one of the following values:
 *         @arg   I2C_SR_STARTF     : Start condition detected flag
 *         @arg   I2C_SR_SLADDR0F   : Address 0 detected flag
 *         @arg   I2C_SR_SLADDR1F   : Address 1 detected flag
 *         @arg   I2C_SR_TENDF      : Transfer end flag
 *         @arg   I2C_SR_STOPF      : Stop condition detected flag
 *         @arg   I2C_SR_RFULLF     : Receive buffer full flag
 *         @arg   I2C_SR_TEMPTYF    : Transfer buffer empty flag
 *         @arg   I2C_SR_ARLOF      : Arbitration fails flag
 *         @arg   I2C_SR_ACKRF      : NACK detected flag
 *         @arg   I2C_SR_NACKF      : NACK detected flag
 *         @arg   I2C_SR_MSL        : Master mode flag
 *         @arg   I2C_SR_BUSY       : Bus busy status flag
 *         @arg   I2C_SR_TRA        : Transfer mode flag
 *         @arg   I2C_SR_GENCALLF   : General call detected flag
 *         @arg   I2C_SR_SMBDEFAULTF: Smbus default address detected flag
 *         @arg   I2C_SR_SMBHOSTF   : Smbus host address detected flag
 *         @arg   I2C_SR_SMBALRTF   : Smbus alarm address detected flag
 * @retval The status of the I2C status flag, may be Set or Reset.
 */
en_flag_status_t I2C_GetStatus(uint32_t u32StatusBit)
{
    DDL_ASSERT(IS_VALID_RD_STATUS_BIT(u32StatusBit));

    return ((M0P_I2C->SR & u32StatusBit) ? Set : Reset);
}

/**
 * @brief  I2C status bit get
 * @param  [in] u32StatusBit         Specifies the flag to be write,
 *         This parameter can be one of the following values:
 *         @arg  I2C_SR_MSL
 *         @arg  I2C_SR_TRA
 * @param  [in] enStatus             New state of the I2Cx function, can be
 *                                   Disable or Enable the function
 * @retval None
 */
void  I2C_WriteStatus(uint32_t u32StatusBit, en_flag_status_t enStatus)
{
    DDL_ASSERT(IS_VALID_WR_STATUS_BIT(u32StatusBit));

    if(Set == enStatus)
    {
        M0P_I2C->SR |= u32StatusBit;
    }
    else
    {
        M0P_I2C->SR &= (~u32StatusBit);
    }
}

/**
 * @brief  Clear I2C status flag
 * @param  [in] u32StatusBit         Specifies the flag to clear,
 *         This parameter can be any combination of the following values:
 *         @arg  I2C_CLR_STARTFCLR     : Start flag clear
 *         @arg  I2C_CLR_SLADDR0FCLR   : Address 0 detected flag clear
 *         @arg  I2C_CLR_SLADDR1FCLR   : Address 1 detected flag clear
 *         @arg  I2C_CLR_TENDFCLR      : Transfer end flag clear
 *         @arg  I2C_CLR_STOPFCLR      : Stop flag clear
 *         @arg  I2C_CLR_RFULLFCLR     : Receive buffer full flag clear
 *         @arg  I2C_CLR_TEMPTYFCLR    : Transfer buffer empty flag clear
 *         @arg  I2C_CLR_ARLOFCLR      : Arbitration fails flag clear
 *         @arg  I2C_CLR_NACKFCLR      : Nack detected flag clear
 *         @arg  I2C_CLR_GENCALLFCLR   : General call address detected flag clear
 *         @arg  I2C_CLR_SMBDEFAULTFCLR: Smbus default address detected flag clear
 *         @arg  I2C_CLR_SMBHOSTFCLR   : Smbus host address detected flag clear
 *         @arg  I2C_CLR_SMBALRTFCLR   : Smbus alarm address detected flag clear
 * @retval None
 */
void I2C_ClearStatus(uint32_t u32StatusBit)
{
    DDL_ASSERT(IS_VALID_CLEARBIT(u32StatusBit));

    M0P_I2C->CLR |= u32StatusBit;
}

/**
 * @brief  I2C fast ACK config
 * @param  [in] enNewState    New state of the fast ACK function, can be Disable
 *                            or Enable the function
 *
 * @retval None
 */
void I2C_FastAckConfig(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    bM0P_I2C->CR1_b.FACKEN = enNewState;
}

/**
 * @brief  Initialize structure stc_i2c_init_t variable with default value.
 * @param  [out] pstcI2C_InitStruct     Pointer to a stc_i2c_init_t structure variable
 *                                      which will be initialized.
 *   @arg  See the struct @ref stc_i2c_init_t.
 * @retval en_result_t
 */
en_result_t I2C_StructInit(stc_i2c_init_t* pstcI2C_InitStruct)
{
    en_result_t enRet = Ok;
    if (pstcI2C_InitStruct == NULL)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        pstcI2C_InitStruct->u32Baudrate = 50000ul;
        pstcI2C_InitStruct->u32SclTime = 0ul;
    }

    return enRet;
}

/**
 * @}
 */

#endif /* DDL_I2C_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
