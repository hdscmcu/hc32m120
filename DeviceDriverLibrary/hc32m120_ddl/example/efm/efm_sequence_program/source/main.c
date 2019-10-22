/**
 *******************************************************************************
 * @file  efm/efm_sequence_program/source/main.c
 * @brief Main program of EFM for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2019-06-28       chengy          First version
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
#include "string.h"

/**
 * @addtogroup HC32M120_DDL_Examples
 * @{
 */

/**
 * @addtogroup EFM_Sequence_Program
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define LED_R_PORT      (GPIO_PORT_12)
#define LED_G_PORT      (GPIO_PORT_7)

#define LED_R_PIN       (GPIO_PIN_0)
#define LED_G_PIN       (GPIO_PIN_0)

#define LED_R_ON()      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_ON()      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))

#define LED_R_OFF()     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_OFF()     (GPIO_SetPins(LED_G_PORT, LED_G_PIN))

#define LED_RG_OFF()   {LED_R_OFF();LED_G_OFF();}

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t u8CmpRet1 = 1u;
uint8_t u8CmpRet2 = 1u;
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint32_t u32ExpectBuf[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu};
uint32_t u32ExpectBuf1[] = {0x04030201u, 0x08070605u, 0x0C0B0A09u, 0x100F0E0Du, 0xFFFFFF11u};
uint32_t u32ExpectBuf2[] = {0x04030201u, 0x08070605u, 0x0C0B0A09u, 0x100F0E0Du, 0xFF131211u};
uint32_t u32ReadBuf1[5];
uint32_t u32ReadBuf2[5];
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

    /* "Turn off" LED before set to output */
    LED_RG_OFF();

    /* Output enable */
    GPIO_OE(LED_R_PORT, LED_R_PIN, Enable);
    GPIO_OE(LED_G_PORT, LED_G_PIN, Enable);
}

/**
 * @brief  Main function of EFM project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint8_t u8TestBuf[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    uint32_t u32Len = 17u;
    uint32_t u32Addr = 0ul;

    /* Led Init */
    Led_Init();

    /* Unlock EFM. */
    EFM_Unlock();

    /* Enable flash. */
    EFM_Cmd(Enable);
    /* Wait flash ready. */
    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        ;
    }
    /* Erase sector 10 11. */
    EFM_SectorErase(EFM_SECTOR10_ADRR);
    EFM_SectorErase(EFM_SECTOR11_ADRR);

    u32Addr = EFM_SECTOR10_ADRR;
    for(uint8_t i = 0u; i < 5; i++)
    {
         u32ReadBuf1[i] = *((volatile unsigned int*)(u32Addr));
         u32Addr += 4ul;
    }

    u32Addr = EFM_SECTOR11_ADRR;
    for(uint8_t i = 0u; i < 5u; i++)
    {
         u32ReadBuf2[i] = *((volatile unsigned int*)(u32Addr));
         u32Addr += 4ul;
    }

    u8CmpRet1 = (uint8_t)memcmp(u32ReadBuf1, u32ExpectBuf, sizeof(u32ReadBuf1));
    u8CmpRet2 = (uint8_t)memcmp(u32ReadBuf2, u32ExpectBuf, sizeof(u32ReadBuf2));
    if(1u == (u8CmpRet1 || u8CmpRet2))
    {
         LED_R_ON();        /* Don't meet the expected */
    }
    else
    {
         LED_G_ON();        /* Meet the expected */
    }

    /* Sequence program. */
    EFM_SequenceProgram(EFM_SECTOR10_ADRR, u32Len, u8TestBuf);

    /* Sequence program. */
    EFM_SequenceProgram(EFM_SECTOR11_ADRR, u32Len + 2u, u8TestBuf);

    u32Addr = EFM_SECTOR10_ADRR;
    for(uint8_t i = 0u; i < 5u; i++)
    {
         u32ReadBuf1[i] = *((volatile unsigned int*)(u32Addr));
         u32Addr += 4ul;
    }

    u32Addr = EFM_SECTOR11_ADRR;
    for(uint8_t i = 0u; i < 5u; i++)
    {
         u32ReadBuf2[i] = *((volatile unsigned int*)(u32Addr));
         u32Addr += 4ul;
    }

    LED_RG_OFF();

    u8CmpRet1 = (uint8_t)memcmp(u32ReadBuf1, u32ExpectBuf1, sizeof(u32ReadBuf1));
    u8CmpRet2 = (uint8_t)memcmp(u32ReadBuf2, u32ExpectBuf2, sizeof(u32ReadBuf2));
    if(1u == (u8CmpRet1 || u8CmpRet2))
    {
         LED_R_ON();        /* Don't meet the expected */
    }
    else
    {
         LED_G_ON();        /* Meet the expected */
    }

    EFM_ChipErase();

    u32Addr = EFM_SECTOR10_ADRR;
    for(uint8_t i = 0u; i < 5u; i++)
    {
         u32ReadBuf1[i] = *((volatile unsigned int*)(u32Addr));
         u32Addr += 4ul;
    }

    u32Addr = EFM_SECTOR11_ADRR;
    for(uint8_t i = 0u; i < 5u; i++)
    {
         u32ReadBuf2[i] = *((volatile unsigned int*)(u32Addr));
         u32Addr += 4ul;
    }

    LED_RG_OFF();

    u8CmpRet1 = (uint8_t)memcmp(u32ReadBuf1, u32ExpectBuf, sizeof(u32ReadBuf1));
    u8CmpRet2 = (uint8_t)memcmp(u32ReadBuf2, u32ExpectBuf, sizeof(u32ReadBuf2));
    if(1u == (u8CmpRet1 || u8CmpRet2))
    {
         LED_R_ON();        /* Don't meet the expected */
    }
    else
    {
         LED_G_ON();        /* Meet the expected */
    }

    /* Lock EFM. */
    EFM_Lock();

    while(1)
    {
        ;
    }
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
