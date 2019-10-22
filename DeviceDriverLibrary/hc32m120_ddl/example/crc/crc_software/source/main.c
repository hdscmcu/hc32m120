/**
 *******************************************************************************
 * @file  crc/crc_software/source/main.c
 * @brief Main program of CRC software for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-07-16       Heqb            First version
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
 * @addtogroup CRC_Software
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Enable CRC. */
#define ENABLE_CRC()                (CLK_FcgPeriphClockCmd(CLK_FCG_CRC, Enable))

/* Disable CRC. */
#define DISABLE_CRC()               (CLK_FcgPeriphClockCmd(CLK_FCG_CRC, Disable))

#define LED_R_PORT      (GPIO_PORT_12)
#define LED_G_PORT      (GPIO_PORT_7)

#define LED_R_PIN       (GPIO_PIN_0)
#define LED_G_PIN       (GPIO_PIN_0)

#define LED_R_ON()      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_ON()      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))

#define LED_R_OFF()     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_OFF()     (GPIO_SetPins(LED_G_PORT, LED_G_PIN))

#define ByteCnt1        (1u)
#define ByteCnt2        (2u)
#define ByteCnt4        (4u)

#define CRC16_InitVal                 (0xFFFFu)
#define CRC32_InitVal                 (0xFFFFFFFFul)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void CrcConfig(void);
static uint16_t CalaCRC16(const uint8_t *pu8Data, uint32_t u32InitVal, uint8_t ByteWidth, uint32_t Length);
static uint32_t CalaCRC32(const uint8_t *pu8Data, uint32_t u32InitVal, uint8_t ByteWidth, uint32_t Length);
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
static void Led_Init(void)
{
    stc_gpio_init_t stcGpioInit;

    /* LED initialize */
    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);

    /* Output enable */
    GPIO_OE(LED_R_PORT, LED_R_PIN, Enable);
    GPIO_OE(LED_G_PORT, LED_G_PIN, Enable);
}

/**
 * @brief  Main function of CRC project
 * @param  None
 * @retval None
 */
int32_t main(void)
{
    uint8_t  au8SrcData [3u] = {0x12u,0x21u,0u};
    uint16_t au16SrcData[3u] = {0x1234u,0x4321u,0u};
    uint32_t au32SrcData[3u] = {0x12345678u,0x87654321u,0u}; 
    uint32_t u16Checksum;
    uint32_t u32Checksum;
    uint32_t u32CRC16Data;
    uint32_t u32CRC32Data;

    /* Configures CRC. */
    CrcConfig();
    
    Led_Init();
    LED_R_OFF();
    LED_G_ON();    
    
    /***************** Configuration end, application start **************/
    while (1)
    {
        /* Calculates byte data's CRC16 checksum and CRC32 checksum. */
        u32CRC16Data    = CalaCRC16((uint8_t *)&au8SrcData, CRC16_InitVal, ByteCnt1, 3u);
        u16Checksum     = (uint16_t)CRC_Calculate(CRC_CRC16, (uint8_t *)&au8SrcData, CRC16_InitVal, 3u, CRC_BW_8);
        if(u32CRC16Data != u16Checksum)
        {
            LED_R_ON();
            LED_G_OFF();   
        }
        u32CRC32Data    = CalaCRC32((uint8_t *)&au8SrcData, CRC32_InitVal, ByteCnt1, 3u);
        u32Checksum     = CRC_Calculate(CRC_CRC32, (uint8_t *)&au8SrcData, CRC32_InitVal, 3u, CRC_BW_8);
        if(u32CRC32Data != u32Checksum)
        {
            LED_R_ON();
            LED_G_OFF();
        }

        /* Calculates half word data's CRC16 checksum and CRC32 checksum. */
        u32CRC16Data    = CalaCRC16((uint8_t *)&au16SrcData, CRC16_InitVal, ByteCnt2, 3u);
        u16Checksum     = (uint16_t)CRC_Calculate(CRC_CRC16, (uint16_t *)&au16SrcData, CRC16_InitVal, 3u, CRC_BW_16);
        if(u32CRC16Data != u16Checksum)
        {
            LED_R_ON();
            LED_G_OFF();
        }
        u32CRC32Data    = CalaCRC32((uint8_t *)&au16SrcData, CRC32_InitVal, ByteCnt2, 3u);
        u32Checksum     = CRC_Calculate(CRC_CRC32, (uint16_t *)&au16SrcData, CRC32_InitVal, 3u, CRC_BW_16);
        if(u32CRC32Data != u32Checksum)
        {
            LED_R_ON();
            LED_G_OFF();
        }
              
        /* Calculates word data's CRC16 checksum and CRC32 checksum. */
        u32CRC16Data    = CalaCRC16((uint8_t *)&au32SrcData, CRC16_InitVal, ByteCnt4, 3u);        
        u16Checksum     = (uint16_t)CRC_Calculate(CRC_CRC16, (uint32_t *)&au32SrcData, CRC16_InitVal, 3u, CRC_BW_32);
        if(u32CRC16Data != u16Checksum)
        {
            LED_R_ON();
            LED_G_OFF();
        }
        u32CRC32Data    = CalaCRC32((uint8_t *)&au32SrcData, CRC32_InitVal,ByteCnt4, 3u);
        u32Checksum     = CRC_Calculate(CRC_CRC32, (uint32_t *)&au32SrcData, CRC32_InitVal, 3u, CRC_BW_32);
        if(u32CRC32Data != u32Checksum)
        {
            LED_R_ON();
            LED_G_OFF();
        }
    }
}

/**
 * @brief  CRC configuration.
 * @param  None
 * @retval None
 */
static void CrcConfig(void)
{
    ENABLE_CRC();

}

/**
 * @brief CRC-16 calculation.
 * @param  [in]   pu8Data               Pointer to the buffer containing the data to be computed.
 * @param  [in]   u32InitVal            Initialize the CRC calculation.
 * @param  [in]   u8ByteWidth           Byte width of the data.
 * @param  [in]   u32Length             The length of the data to be computed.
 * @retval crc
 */

uint16_t CalaCRC16(const uint8_t *pu8Data, uint32_t u32InitVal, uint8_t u8ByteWidth, uint32_t u32Length)
{ 
    uint32_t i = 0u,j = 0u;
    uint16_t crc = (uint16_t)u32InitVal;       /*  Initial value */ 

    while(u32Length--) 
    {   
        i = u8ByteWidth;
        while(i--)
        {
            crc ^= (*pu8Data++);   
            for (j = 0u; j < 8u; j++)  
            {     
                if (crc & 0x1u)    
                {   
                    crc >>= 1u;
                    crc ^= 0x8408u;  /* 0x8408 = reverse 0x1021 */ 
                }   
                else
                {
                    crc >>= 1u;
                }
            } 
       }  
        

    }

    crc =~crc;

    return crc;
}


/**
 * @brief CRC-32 calculation.
 * @param  [in]   pu8Data               Pointer to the buffer containing the data to be computed.
 * @param  [in]   u32InitVal            Initialize the CRC calculation.
 * @param  [in]   u8ByteWidth           Byte width of the data.
 * @param  [in]   u32Length             The length of the data to be computed.
 * @retval crc
 */
uint32_t CalaCRC32(const uint8_t *pu8Data, uint32_t u32InitVal, uint8_t u8ByteWidth, uint32_t u32Length)  
{  
    uint32_t i = 0u, j = 0u;
    uint32_t crc = u32InitVal;     /*  Initial value */ 

    while(u32Length--) 
    {  
        i = u8ByteWidth;
        while(i--)
        {
            crc ^= (*pu8Data++);         
            for (j = 0u; j < 8u; j++)  
            {  
                if (crc & 0x1u) 
                {
                    crc = (crc >> 1u) ^ 0xEDB88320ul; /*0xEDB88320= reverse 0x04C11DB7*/ 
                }
                else
                {
                crc = (crc >> 1u);  
                }
            } 
        }
    }  
    crc =~crc;

    return crc; 
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
