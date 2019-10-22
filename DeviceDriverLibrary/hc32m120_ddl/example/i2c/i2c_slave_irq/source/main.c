/**
 *******************************************************************************
 * @file  i2c/i2c_slave_irq/source/main.c
 * @brief Main program of I2C for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-09       Wangmin         First version
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
 * @addtogroup I2C_Slave_Irq
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Define slave device address for example */
#define DEVICE_ADDRESS                  0x06u

/* Define port and pin for SDA and SCL */
#define I2C_SCL_PORT                    (GPIO_PORT_6)
#define I2C_SCL_PIN                     (GPIO_PIN_0)
#define I2C_SDA_PORT                    (GPIO_PORT_6)
#define I2C_SDA_PIN                     (GPIO_PIN_1)

#define TIMEOUT                         ((uint32_t)0x10000)

#define I2C_RET_OK                      0u
#define I2C_RET_ERROR                   1u

#define GENERATE_START                  0x00u
#define GENERATE_RESTART                0x01u

#define ADDRESS_W                       0x00u
#define ADDRESS_R                       0x01u

/* Define Write and read data length for the example */
#define TEST_DATA_LEN                   128u
/* Define i2c baudrate */
#define I2C_BAUDRATE                    400000ul

/* I2C interrupt source and number define */
#define I2C_EEI_IRQn                    (Int008_IRQn)
#define I2C_EEI_SOURCE                  (INT_IIC_EE1)
#define I2C_TXI_IRQn                    (Int020_IRQn)
#define I2C_TXI_SOURCE                  (INT_IIC_TXI)
#define I2C_RXI_IRQn                    (Int018_IRQn)
#define I2C_RXI_SOURCE                  (INT_IIC_RXI)
#define I2C_TEI_IRQn                    (Int016_IRQn)
#define I2C_TEI_SOURCE                  (INT_IIC_TEI)

/* Define for RGB LED */
#define LED_R_PORT                      (GPIO_PORT_12)
#define LED_G_PORT                      (GPIO_PORT_7)
#define LED_B_PORT                      (GPIO_PORT_3)
#define LED_R_PIN                       (GPIO_PIN_0)
#define LED_G_PIN                       (GPIO_PIN_0)
#define LED_B_PIN                       (GPIO_PIN_1)
#define LED_G_TOGGLE()                  (GPIO_TogglePins(LED_G_PORT, LED_G_PIN))
#define LED_R_TOGGLE()                  (GPIO_TogglePins(LED_R_PORT, LED_R_PIN))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void LedConfig(void);
static uint8_t Slave_Initialize(void);
static void I2C_EEI_Callback(void);
static void I2C_TXI_Callback(void);
static void I2C_RXI_Callback(void);
static void I2C_TEI_Callback(void);
static void BufWrite(uint8_t u8Data);
static uint8_t BufRead(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint8_t u8TxBuf[TEST_DATA_LEN];
uint8_t u8RxBuf[TEST_DATA_LEN];
uint32_t u32DataInOffset = 0u;
uint32_t u32DataOutOffset = 0u;
__IO uint8_t u8FinishFlag = 0u;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function of i2c_at24c02 project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t i;

    /* Configure system clock. */
    SystemClockConfig();

    /* RGB LED configuration */
    LedConfig();

    /* Test buffer initialize */
    for(i=0u; i<TEST_DATA_LEN; i++)
    {
        u8RxBuf[i] = 0u;
    }

    /* Initialize I2C port*/
    GPIO_SetFunc(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_FUNC_6_I2C);
    GPIO_SetFunc(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_FUNC_6_I2C);

    /* Enable I2C Peripheral*/
    CLK_FcgPeriphClockCmd(CLK_FCG_I2C, Enable);

    /* Initialize I2C peripheral and enable function*/
    Slave_Initialize();

    /* Wait communicaiton finished*/
    while(0u == u8FinishFlag)
    {
        ;
    }

    /* Communication finished */
    while(1)
    {
        LED_G_TOGGLE();
        DDL_Delay1ms(500u);
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
 * @brief   Initialize the I2C peripheral for slave
 * @param   None
 * @retval  Process result
 *          - I2C_RET_ERROR  Initialize failed
 *          - I2C_RET_OK     Initialize success
 */
static uint8_t Slave_Initialize(void)
{
    stc_i2c_init_t stcI2cInit;
    stc_irq_regi_config_t stcIrqRegiCfg;
    float32_t fErr;

    I2C_DeInit();

    I2C_StructInit(&stcI2cInit);
    stcI2cInit.u32Baudrate = I2C_BAUDRATE;
    stcI2cInit.u32SclTime = 5u;
    stcI2cInit.u32I2cClkDiv = I2C_CLK_DIV1;
    I2C_Init(&stcI2cInit, &fErr);

    /* Set slave address*/
#ifdef I2C_10BITS_ADDRESS
    I2C_SlaveAdrConfig(I2C_ADR_0, I2C_ADR_CONFIG_10BIT, DEVICE_ADDRESS);
#else
    I2C_SlaveAdrConfig(I2C_ADR_0, I2C_ADR_CONFIG_7BIT, DEVICE_ADDRESS);
#endif

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegiCfg.enIRQn = I2C_EEI_IRQn;
    stcIrqRegiCfg.enIntSrc = I2C_EEI_SOURCE;
    stcIrqRegiCfg.pfnCallback = &I2C_EEI_Callback;
    INTC_IrqRegistration(&stcIrqRegiCfg);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    stcIrqRegiCfg.enIRQn = I2C_RXI_IRQn;
    stcIrqRegiCfg.enIntSrc = I2C_RXI_SOURCE;
    stcIrqRegiCfg.pfnCallback = &I2C_RXI_Callback;
    INTC_IrqRegistration(&stcIrqRegiCfg);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    stcIrqRegiCfg.enIRQn = I2C_TXI_IRQn;
    stcIrqRegiCfg.enIntSrc = I2C_TXI_SOURCE;
    stcIrqRegiCfg.pfnCallback = &I2C_TXI_Callback;
    INTC_IrqRegistration(&stcIrqRegiCfg);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    stcIrqRegiCfg.enIRQn = I2C_TEI_IRQn;
    stcIrqRegiCfg.enIntSrc = I2C_TEI_SOURCE;
    stcIrqRegiCfg.pfnCallback = &I2C_TEI_Callback;
    INTC_IrqRegistration(&stcIrqRegiCfg);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /* I2C function command */
    I2C_Cmd(Enable);

    /* Config slave address match interrupt function*/
    I2C_IntCmd(I2C_CR2_SLADDR0IE, Enable);

    return I2C_RET_OK;
}

/**
 * @brief  Configure RGB LED.
 * @param  None
 * @retval None
 */
static void LedConfig(void)
{
    stc_gpio_init_t stcGpioInit = {0u};

    stcGpioInit.u16PinMode = PIN_MODE_OUT;
    stcGpioInit.u16PinState = PIN_STATE_SET;
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
}

/**
 * @brief   I2C EEI(communication error or event) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_EEI_Callback(void)
{
    /* If address flag valid */
    if(Set == I2C_GetStatus(I2C_SR_SLADDR0F))
    {
        I2C_ClearStatus(I2C_CLR_SLADDR0FCLR);
        /* Enable Tx or Rx function*/
        if(Set == I2C_GetStatus(I2C_SR_TRA))
        {
            /* Config tx buffer empty interrupt function*/
            I2C_IntCmd(I2C_CR2_TEMPTYIE, Enable);
            /* Write the first data to DTR immediately */
            I2C_SendData(BufRead());
        }
        else
        {
            /* Config rx buffer full interrupt function*/
            I2C_IntCmd(I2C_CR2_RFULLIE, Enable);
        }

        /* Enable stop and NACK interrupt */
        I2C_IntCmd(I2C_CR2_STOPIE | I2C_CR2_NACKIE, Enable);
    }

    /* If NACK interrupt occurred */
    if(Set == I2C_GetStatus(I2C_SR_ACKRF))
    {
        /* clear NACK flag*/
        I2C_ClearStatus(I2C_CLR_NACKFCLR);
        /* Stop tx or rx process*/
        if(Set == I2C_GetStatus(I2C_SR_TRA))
        {
            /* Config tx buffer empty interrupt function disable*/
            I2C_IntCmd(I2C_CR2_TEMPTYIE, Disable);

            /* Read DRR register to release SCL*/
            I2C_ReadData();
        }
        else
        {
            /* Config rx buffer full interrupt function disable */
            I2C_IntCmd(I2C_CR2_RFULLIE, Disable);
        }
    }

    /* If stop interrupt occurred */
    if(Set == I2C_GetStatus(I2C_SR_STOPF))
    {
        /* Disable all interrupt enable flag except SLADDR0IE*/
        I2C_IntCmd(I2C_CR2_TEMPTYIE |  I2C_CR2_RFULLIE |                       \
                   I2C_CR2_STOPIE | I2C_CR2_NACKIE,                            \
                   Disable);
        /* Clear STOPF flag */
        I2C_ClearStatus(I2C_CLR_STOPFCLR);
        /* Communication finished */
        u8FinishFlag = 1u;
    }
}

/**
 * @brief   I2C TXI(transfer buffer empty) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_TXI_Callback(void)
{
    if(Set == I2C_GetStatus(I2C_SR_TEMPTYF))
    {
        if(u32DataOutOffset < TEST_DATA_LEN)
        {
            I2C_SendData(BufRead());
        }
        else
        {
            /* Disable TXI interrupt */
            I2C_IntCmd(I2C_CR2_TEMPTYIE, Disable);
            /* Enable TEI interrupt */
            I2C_IntCmd(I2C_CR2_TENDIE, Enable);
        }
    }
}

/**
 * @brief   I2C RXI(receive buffer full) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_RXI_Callback(void)
{
    if(Set == I2C_GetStatus(I2C_SR_RFULLF))
    {
        BufWrite(I2C_ReadData());
    }
}

/**
 * @brief   I2C TEI(transfer end) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_TEI_Callback(void)
{
    __IO uint8_t tmp;
    if(Set == I2C_GetStatus(I2C_SR_TENDF))
    {
        /* Dummy read for release SCL */
        tmp = I2C_ReadData();
    }
}


/**
 * @brief   static function for buffer write.
 * @param   [in]   u8Data the data to be write.
 * @retval  None
 */
static void BufWrite(uint8_t u8Data)
{
    if(u32DataInOffset >= TEST_DATA_LEN)
    {
        //error
        while(1)
        {
            ;
        }
    }
    u8RxBuf[u32DataInOffset] = u8Data;
    u32DataInOffset++;
}

/**
 * @brief   Static function for buffer read.
 * @param   None
 * @retval  uint8_t  The data read out from buffer.
 */
static uint8_t BufRead(void)
{
    uint8_t temp;
    if(u32DataOutOffset >= TEST_DATA_LEN)
    {
        //error
        while(1)
        {
            ;
        }
    }
    temp = u8RxBuf[u32DataOutOffset];
    u32DataOutOffset++;

    return temp;
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
