/**
 *******************************************************************************
 * @file  hc32m120_gpio.h
 * @brief This file contains all the functions prototypes of the GPIO driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-21       Zhangxl         First version
   2019-06-21       Zhangxl         Revise debug port definition
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
#ifndef __HC32M120_GPIO_H__
#define __HC32M120_GPIO_H__

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
 * @addtogroup DDL_GPIO
 * @{
 */

#if (DDL_GPIO_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup GPIO_Global_Types GPIO Global Types
 * @{
 */

/**
 * @brief  GPIO Pin Set and Reset enumeration
 */
typedef enum
{
    Pin_Reset = 0u,           /*!< Pin reset    */
    Pin_Set                   /*!< Pin set      */
} en_pin_state_t;

/**
 * @brief  GPIO Init structure definition
 */
typedef struct
{
    uint16_t u16PinState;     /*!< Set pin state to High or Low, @ref GPIO_PinState_Sel for details     */
    uint16_t u16PinMode;      /*!< Pin mode setting, @ref GPIO_PinMode_Sel for details                  */
    uint16_t u16PinOType;     /*!< Output type setting, @ref GPIO_PinOutType_Sel for details            */
    uint16_t u16PinDrv;       /*!< Pin drive capacity setting, @ref GPIO_PinDrv_Sel for details         */
    uint16_t u16Latch;        /*!< Pin latch setting, @ref GPIO_PinLatch_Sel for details                */
    uint16_t u16PullUp;       /*!< Internal pull-up resistor setting, @ref GPIO_PinPU_Sel for details   */
    uint16_t u16Invert;       /*!< Pin input/output invert setting, @ref GPIO_PinInvert_Sel             */
    uint16_t u16PinIType;     /*!< Input type setting, @ref GPIO_PinInType_Sel                          */
    uint16_t u16ExInt;        /*!< External interrupt pin setting, @ref GPIO_PinExInt_Sel for details   */
} stc_gpio_init_t;
/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup GPIO_Global_Macros GPIO Global Macros
 * @{
 */

/** @defgroup GPIO_pins_define GPIO pin source
 * @{
 */
#define GPIO_PIN_0                  ((uint8_t)0x01u)  /*!< Pin 0 selected    */
#define GPIO_PIN_1                  ((uint8_t)0x02u)  /*!< Pin 1 selected    */
#define GPIO_PIN_2                  ((uint8_t)0x04u)  /*!< Pin 2 selected    */
#define GPIO_PIN_3                  ((uint8_t)0x08u)  /*!< Pin 3 selected    */
#define GPIO_PIN_4                  ((uint8_t)0x10u)  /*!< Pin 4 selected    */
#define GPIO_PIN_5                  ((uint8_t)0x20u)  /*!< Pin 5 selected    */
#define GPIO_PIN_6                  ((uint8_t)0x40u)  /*!< Pin 6 selected    */
#define GPIO_PIN_7                  ((uint8_t)0x80u)  /*!< Pin 7 selected    */
#define GPIO_PIN_ALL                ((uint8_t)0xFFu)  /*!< All pins selected */

#define GPIO_PIN_MASK               ((uint8_t)0xFFu)  /*!< PIN mask for assert test */

/**
 * @}
 */

/** @defgroup GPIO_Port_source GPIO port source
  * @{
  */
#define GPIO_PORT_0                 ((uint8_t)0x00u)  /*!< Port 0 selected    */
#define GPIO_PORT_1                 ((uint8_t)0x01u)  /*!< Port 1  selected   */
#define GPIO_PORT_2                 ((uint8_t)0x02u)  /*!< Port 2  selected   */
#define GPIO_PORT_3                 ((uint8_t)0x03u)  /*!< Port 3  selected   */
#define GPIO_PORT_4                 ((uint8_t)0x04u)  /*!< Port 4  selected   */
#define GPIO_PORT_5                 ((uint8_t)0x05u)  /*!< Port 5  selected   */
#define GPIO_PORT_6                 ((uint8_t)0x06u)  /*!< Port 6  selected   */
#define GPIO_PORT_7                 ((uint8_t)0x07u)  /*!< Port 7  selected   */
#define GPIO_PORT_12                ((uint8_t)0x0Cu)  /*!< Port 12 selected   */
#define GPIO_PORT_13                ((uint8_t)0x0Du)  /*!< Port 13 selected   */
#define GPIO_PORT_14                ((uint8_t)0x0Eu)  /*!< Port 14 selected   */
/**
 * @}
 */

/** @defgroup GPIO_Port_index GPIO port index
 * @{
 */
#define GPIO_PORT_IDX0              ((uint16_t)0x0001u)
#define GPIO_PORT_IDX1              ((uint16_t)0x0002u)
#define GPIO_PORT_IDX2              ((uint16_t)0x0004u)
#define GPIO_PORT_IDX3              ((uint16_t)0x0008u)
#define GPIO_PORT_IDX4              ((uint16_t)0x0010u)
#define GPIO_PORT_IDX5              ((uint16_t)0x0020u)
#define GPIO_PORT_IDX6              ((uint16_t)0x0040u)
#define GPIO_PORT_IDX7              ((uint16_t)0x0080u)
#define GPIO_PORT_IDX12             ((uint16_t)0x1000u)
#define GPIO_PORT_IDX13             ((uint16_t)0x2000u)
#define GPIO_PORT_IDX14             ((uint16_t)0x4000u)
#define GPIO_PORT_ALL               ((uint16_t)0x70FFu)

#define GPIO_PORT_MASK              ((uint16_t)0x70FFu)
/**
 * @}
 */

/**
 * @}
 */

/** @defgroup GPIO_function_selection_define GPIO function delection
 * @{
 */
/** @defgroup GPIO_PinFunction0 GPIO Function 0 selection
 * @{
 */
#define GPIO_FUNC_0         ((uint8_t)0x00u)
#define GPIO_FUNC_0_GPO     (GPIO_FUNC_0)     /* GPIO function */
/**
 * @}
 */

/** @defgroup GPIO_PinFunction1 GPIO Function 1 selection
 * @{
 */
#define GPIO_FUNC_1         ((uint8_t)0x01u)
#define GPIO_FUNC_1_LVDINP  (GPIO_FUNC_1)     /*!< Low Voltage Detection input pin  */
#define GPIO_FUNC_1_ADTRG   (GPIO_FUNC_1)     /*!< ADC external trigger             */
#define GPIO_FUNC_1_CTCREF  (GPIO_FUNC_1)     /*!< Reference input pin for internal \
                                               clock trimming controller */
#define GPIO_FUNC_1_PULBUZ  (GPIO_FUNC_1)     /*!< MCO output                       */
#define GPIO_FUNC_1_ANIN    (GPIO_FUNC_1)     /*!< Analog input for ADC             */
#define GPIO_FUNC_1_IVCMP   (GPIO_FUNC_1)     /*!< Compare voltage input for CMP    */
#define GPIO_FUNC_1_IVREF   (GPIO_FUNC_1)     /*!< Reference voltage input for CMP  */
#define GPIO_FUNC_1_OPA     (GPIO_FUNC_1)     /*!< Analog input for OPA             */
#define GPIO_FUNC_1_ANI_OPA (GPIO_FUNC_1)     /*!< Analog input for ADC and OPA     */
#define GPIO_FUNC_1_ANI_OPA_CMP (GPIO_FUNC_1) /*!< Analog output for OPA, input for ADC and CMP     */
/**
 * @}
 */

/** @defgroup GPIO_PinFunction2 GPIO Function 2 selection
 * @{
 */
#define GPIO_FUNC_2         ((uint8_t)0x02u)
#define GPIO_FUNC_2_TIMB    (GPIO_FUNC_2)     /*!< Timer B PWM */
#define GPIO_FUNC_2_TIM4    (GPIO_FUNC_2)     /*!< Timer 4 OUL/OVH */
/**
 * @}
 */

/** @defgroup GPIO_PinFunction3 GPIO Function 3 selection
 * @{
 */
#define GPIO_FUNC_3         ((uint8_t)0x03u)
#define GPIO_FUNC_3_TIM2    (GPIO_FUNC_3)   /*!< Timer 2 PWM/CLK/TRIG */
#define GPIO_FUNC_3_CMP     (GPIO_FUNC_3)   /*!< CMP VCOUT */
#define GPIO_FUNC_3_TIMB    (GPIO_FUNC_3)   /*!< Timer B PWM */
#define GPIO_FUNC_3_USART3  (GPIO_FUNC_3)   /*!< USART3 TX */
/**
 * @}
 */

/** @defgroup GPIO_PinFunction4 GPIO Function 4 selection
 * @{
 */
#define GPIO_FUNC_4         ((uint8_t)0x04u)
#define GPIO_FUNC_4_TIM4    (GPIO_FUNC_4)   /*!< Timer 4 OUH/OUL/OVH/OVL/OWH/OWL/CLK */
#define GPIO_FUNC_4_EMB     (GPIO_FUNC_4)   /*!< EMB_IN */
#define GPIO_FUNC_4_EVENTPT (GPIO_FUNC_4)   /*!< EVENT PORT EVENTP1/2/3/4 */
/**
 * @}
 */

/** @defgroup GPIO_PinFunction5 GPIO Function 5 selection
 * @{
 */
#define GPIO_FUNC_5         ((uint8_t)0x05u)
#define GPIO_FUNC_5_USART   (GPIO_FUNC_5)     /*!< USART  */
#define GPIO_FUNC_5_USART1  (GPIO_FUNC_5)     /*!< USART1 */
#define GPIO_FUNC_5_USART2  (GPIO_FUNC_5)     /*!< USART2 */
#define GPIO_FUNC_5_USART3  (GPIO_FUNC_5)     /*!< USART3 */
#define GPIO_FUNC_5_USART4  (GPIO_FUNC_5)     /*!< USART4 */
/**
 * @}
 */

/** @defgroup GPIO_PinFunction6 GPIO Function 6 selection
 * @{
 */
#define GPIO_FUNC_6         ((uint8_t)0x06u)
#define GPIO_FUNC_6_I2C     (GPIO_FUNC_6)     /*!< I2C  */
#define GPIO_FUNC_6_KR      (GPIO_FUNC_6)     /*!< External KEY */
/**
 * @}
 */

/** @defgroup GPIO_PinFunction7 GPIO Function 7 selection
 * @{
 */
#define GPIO_FUNC_7         ((uint8_t)0x07u)
#define GPIO_FUNC_7_SPI     (GPIO_FUNC_7)     /*!< SPI SCK/NSS/MISO/MOSI */
#define GPIO_FUNC_7_TIMA    (GPIO_FUNC_7)     /*!< Timer A PWM/CLK/TRIG */
/**
 * @}
 */

/** @defgroup GPIO_DebugPin_Sel GPIO Debug pin selection
 * @{
 */
#define GPIO_PIN_SWCLK      ((uint8_t)0x01u)
#define GPIO_PIN_SWDIO      ((uint8_t)0x02u)
#define GPIO_PIN_DEBUG      ((uint8_t)0x03u)
/**
 * @}
 */

/** @defgroup GPIO_ReadCycle_Sel GPIO pin read wait cycle selection
 * @{
 */
#define GPIO_READ_WAIT_0    ((uint16_t)(0x00ul << PORT_PCCR_RDWT_POS))
#define GPIO_READ_WAIT_1    ((uint16_t)(0x01ul << PORT_PCCR_RDWT_POS))
#define GPIO_READ_WAIT_2    ((uint16_t)(0x02ul << PORT_PCCR_RDWT_POS))
#define GPIO_READ_WAIT_3    ((uint16_t)(0x03ul << PORT_PCCR_RDWT_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinState_Sel GPIO pin output state selection
 * @{
 */
#define PIN_STATE_RESET     ((uint16_t)(0ul << PORT_PCR_POUT_POS))
#define PIN_STATE_SET       ((uint16_t)(1ul << PORT_PCR_POUT_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinMode_Sel GPIO pin input/output mode selection
 * @{
 */
#define PIN_MODE_IN         ((uint16_t)(0ul << PORT_PCR_POUTE_POS))
#define PIN_MODE_OUT        ((uint16_t)(1ul << PORT_PCR_POUTE_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinOutType_Sel GPIO pin output type selection
 * @{
 */
#define PIN_OTYPE_CMOS      ((uint16_t)(0ul << PORT_PCR_NOD_POS))
#define PIN_OTYPE_NMOS      ((uint16_t)(1ul << PORT_PCR_NOD_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinDrv_Sel GPIO Pin drive capacity selection
 * @{
 */
#define PIN_NORMAL_DRV      ((uint16_t)(0ul << PORT_PCR_DRV_POS))
#define PIN_HIGH_DRV        ((uint16_t)(1ul << PORT_PCR_DRV_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinLatch_Sel GPIO Pin output latch selection
 * @{
 */
#define PIN_LATCH_OFF       ((uint16_t)(0ul << PORT_PCR_LTE_POS))
#define PIN_LATCH_ON        ((uint16_t)(1ul << PORT_PCR_LTE_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinPU_Sel GPIO Pin internal pull-up resistor selection
 * @{
 */
#define PIN_PU_OFF          ((uint16_t)(0ul << PORT_PCR_PUU_POS))
#define PIN_PU_ON           ((uint16_t)(1ul << PORT_PCR_PUU_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinInvert_Sel GPIO Pin I/O invert selection
 * @{
 */
#define PIN_INVERT_OFF      ((uint16_t)(0ul << PORT_PCR_INVE_POS))
#define PIN_INVERT_ON       ((uint16_t)(1ul << PORT_PCR_INVE_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinInType_Sel GPIO Pin input type selection
 * @{
 */
#define PIN_ITYPE_SMT       ((uint16_t)(0ul << PORT_PCR_CINSEL_POS))
#define PIN_ITYPE_CMOS      ((uint16_t)(1ul << PORT_PCR_CINSEL_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_PinExInt_Sel GPIO Pin external interrupt selection
 * @{
 */
#define PIN_EXINT_OFF       ((uint16_t)(0ul << PORT_PCR_INTE_POS))
#define PIN_EXINT_ON        ((uint16_t)(1ul << PORT_PCR_INTE_POS))
/**
 * @}
 */

/**
 * @defgroup GPIO_Register_Protect GPIO Registers Protect Code
 * @{
 */
#define GPIO_REG_UNPROTECT  ((uint16_t)0xA501u)
#define GPIO_REG_PROTECT    ((uint16_t)0xA500u)
/**
 * @}
 */

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
 * @addtogroup GPIO_Global_Functions
 * @{
 */
/**
 * @brief  GPIO lock. PSPCR, PCCR, PINAER, PCRxy write disable
 * @param  None
 *
 * @retval None
 */
__STATIC_INLINE void GPIO_Lock(void)
{
    M0P_PORT->PWPR = GPIO_REG_PROTECT;
}

/**
 * @brief  GPIO unlock. PSPCR, PCCR, PINAER, PCRxy write enable
 * @param  None
 *
 * @retval None
 */
__STATIC_INLINE void GPIO_Unlock(void)
{
    M0P_PORT->PWPR = GPIO_REG_UNPROTECT;
}

en_result_t GPIO_Init(uint8_t u8Port, uint8_t u8Pin, const stc_gpio_init_t *pstcGpioInit);
void GPIO_DeInit(void);
void GPIO_Unlock(void);
void GPIO_Lock(void);
en_result_t GPIO_StructInit(stc_gpio_init_t *pstcGpioInit);
void GPIO_DebugPortSetting(uint8_t u8DebugPort, en_functional_state_t enNewState);
void GPIO_SetFunc(uint8_t u8Port, uint8_t u8Pin, uint8_t u8Func);
void GPIO_PortReadWait(uint16_t u16ReadWait);
void GPIO_AlwaysOn(uint16_t u16PortIdx, en_functional_state_t enNewState);
void GPIO_OE(uint8_t u8Port, uint8_t u8Pin, en_functional_state_t enNewState);
//port pin
en_pin_state_t GPIO_ReadInputPortPin(uint8_t u8Port, uint8_t u8Pin);
uint8_t GPIO_ReadInputPort(uint8_t u8Port);
en_pin_state_t GPIO_ReadOutputPortPin(uint8_t u8Port, uint8_t u8Pin);
uint8_t GPIO_ReadOutputPort(uint8_t u8Port);
void GPIO_SetPins(uint8_t u8Port, uint8_t u8Pin);
void GPIO_ResetPins(uint8_t u8Port, uint8_t u8Pin);
void GPIO_WritePort(uint8_t u8Port, uint8_t u8PortVal);
void GPIO_TogglePins(uint8_t u8Port, uint8_t u8Pin);

/**
 * @}
 */

#endif /* DDL_GPIO_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32M120_GPIO_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
