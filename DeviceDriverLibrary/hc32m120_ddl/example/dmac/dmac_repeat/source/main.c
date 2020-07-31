 /*******************************************************************************
 * @file  dmac/dmac_repeat/source/main.c
 * @brief This example demonstrates DMA repeat function.
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
 * @addtogroup DMAC_Repeat
 * @{
 */
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* DMAC */
#define DMA_CH          (DMA_CHANNEL_1)
#define DMA_TRNCNT      (5u)
#define DMA_BLKSIZE     (4u)
#define DMA_RPTSIZE     (11u)

#define LED_R_PORT      (GPIO_PORT_12)
#define LED_G_PORT      (GPIO_PORT_7)

#define LED_R_PIN       (GPIO_PIN_0)
#define LED_G_PIN       (GPIO_PIN_0)

#define LED_R_ON()      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_ON()      (GPIO_ResetPins(LED_G_PORT, LED_G_PIN))

#define LED_R_OFF()     (GPIO_SetPins(LED_R_PORT, LED_R_PIN))
#define LED_G_OFF()     (GPIO_SetPins(LED_G_PORT, LED_G_PIN))

#define LED_RG_ON()     {LED_R_ON();LED_G_ON();}
#define LED_RG_OFF()    {LED_R_OFF();LED_G_OFF();}
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t u8CmpRet = 1u;
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static const uint32_t u32SrcBuf[22] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                       11, 12, 13, 14, 15, 16, 17, 18,
                                       19, 20, 21, 22};
static uint32_t u32DstBuf[22] = {0};
static uint32_t u32ExpectDstBufData[22] = {12, 13, 14, 15, 16, 17, 18, 19, 20, 10,
                                           11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
 * @brief  Main function of DMAC project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_dma_ch_cfg_t stcDmaChCfg;

    Led_Init();

    /* Config DMA */
    stcDmaChCfg.u32DataWidth = DMA_DATAWIDTH_32BIT;
    stcDmaChCfg.u32BlockSize = DMA_BLKSIZE;
    stcDmaChCfg.u32TransferCnt = DMA_TRNCNT;
    stcDmaChCfg.u32SrcAddr = (uint32_t)(&u32SrcBuf[0]);
    stcDmaChCfg.u32DesAddr = (uint32_t)(&u32DstBuf[0]);
    stcDmaChCfg.u32SrcInc = DMA_SRCADDRINC_INC;
    stcDmaChCfg.u32DesInc = DMA_DESADDRINC_INC;

    CLK_FcgPeriphClockCmd(CLK_FCG_DMA, Enable);

    DMA_ChannelCfg(DMA_CH, &stcDmaChCfg);

    /* set destination address repeat & repeat size = 11 */
    DMA_RepeatInit(DMA_CH, DMA_RPTNSSEL_DESRPT, DMA_RPTSIZE);

    /* Enable DMA1. */
    DMA_Cmd(Enable);

    /* Enable DMA1 channel1. */
    DMA_ChannelEnable(DMA_CH);

    /* Enable AOS clock */
    CLK_FcgPeriphClockCmd(CLK_FCG_AOS, Enable);

    /* Set DMA trigger source */
    DMA_SetTriggerSrc(DMA_CH, EVT_AOS_STRG);

    DMA_ClearCplFlag(DMA_CH, DMA_FLAG_TC);

    while(Set != DMA_GetCplFlag(DMA_CH, DMA_FLAG_TC))
    {
         AOS_SW_Trigger();
    }

    u8CmpRet = (uint8_t)memcmp(u32DstBuf, u32ExpectDstBufData, sizeof(u32DstBuf));
    if(0u == u8CmpRet)
    {
        LED_G_ON();    /* Meet the expected */
    }
    else
    {
        LED_R_ON();    /* Don't meet the expected */
    }

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
