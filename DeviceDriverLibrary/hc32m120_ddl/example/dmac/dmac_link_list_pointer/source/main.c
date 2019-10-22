 /*******************************************************************************
 * @file  dmac/dmac_link_list_pointer/source/main.c
 * @brief This example demonstrates TIMERA base count function.
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
 * @addtogroup DMAC_LLP
 * @{
 */
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* DMAC */
#define DMA_CH          (DMA_CHANNEL_0)
#define DMA_TRNCNT      (1ul)
#define DMA_BLKSIZE     ((uint32_t)ARRAY_SZ(u16SrcBuf))
#define DMA_LLP_MODE    (DMA_LLP_RUN)
#define DMA_LLP_POS     (DMA_CH0CTL0_LLP_POS - 2ul)

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
uint8_t u8CmpRet = 0u;
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static const uint8_t u8SrcBuf[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static uint8_t u8DstBuf[10] = {0};

static const uint16_t u16SrcBuf[10] = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
static uint16_t u16DstBuf[10] = {0};

static const uint32_t u32SrcBuf[10] = {31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
static uint32_t u32DstBuf[10] = {0};

static stc_dma_llp_descriptor_t stcLlpDesc[2] = {0};
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

    /* descriptor 0 */
    stcLlpDesc[0].SARx = (uint32_t)(&u16SrcBuf[0]);
    stcLlpDesc[0].DARx = (uint32_t)(&u16DstBuf[0]);
    stcLlpDesc[0].CHxCTL0 = DMA_BLKSIZE | DMA_TRNCNT << DMA_CH0CTL0_CNT_POS |  \
                            (uint32_t)(&stcLlpDesc[1]) << DMA_LLP_POS       |  \
                            DMA_LLP_ENABLE | DMA_LLP_MODE | DMA_DATAWIDTH_16BIT;
    stcLlpDesc[0].CHxCTL1 = DMA_DESADDRINC_INC | DMA_SRCADDRINC_INC;

    /* descriptor 1 */
    stcLlpDesc[1].SARx = (uint32_t)(&u32SrcBuf[0]);
    stcLlpDesc[1].DARx = (uint32_t)(&u32DstBuf[0]);
    stcLlpDesc[1].CHxCTL0 = DMA_BLKSIZE | DMA_TRNCNT << DMA_CH0CTL0_CNT_POS |  \
                            DMA_LLP_DISABLE | DMA_LLP_MODE | DMA_DATAWIDTH_32BIT;

    stcLlpDesc[1].CHxCTL1 = DMA_DESADDRINC_INC | DMA_SRCADDRINC_INC;

    /* Config DMA */
    stcDmaChCfg.u32DataWidth = DMA_DATAWIDTH_8BIT;
    stcDmaChCfg.u32BlockSize = DMA_BLKSIZE;
    stcDmaChCfg.u32TransferCnt = DMA_TRNCNT;
    stcDmaChCfg.u32SrcAddr = (uint32_t)(&u8SrcBuf[0]);
    stcDmaChCfg.u32DesAddr = (uint32_t)(&u8DstBuf[0]);
    stcDmaChCfg.u32SrcInc = DMA_SRCADDRINC_INC;
    stcDmaChCfg.u32DesInc = DMA_DESADDRINC_INC;

    CLK_FcgPeriphClockCmd(CLK_FCG_DMA, Enable);

    DMA_ChannelCfg(DMA_CH, &stcDmaChCfg);

    DMA_LlpInit(DMA_CH, DMA_LLP_RUN, (uint32_t)(&stcLlpDesc[0]));

    /* Enable DMA1. */
    DMA_Cmd(Enable);

    /* Enable DMA1 channel0. */
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

    /* Verify destination buffer data && expeted data */
    if(0 != memcmp(u8DstBuf, u8SrcBuf, sizeof(u8SrcBuf)))
    {
        u8CmpRet += 1u;
    }

    if(0 != memcmp(u16DstBuf, u16SrcBuf, sizeof(u16SrcBuf)))
    {
        u8CmpRet += 1u;
    }

    if(0 != memcmp(u32DstBuf, u32SrcBuf, sizeof(u32SrcBuf)))
    {
        u8CmpRet += 1u;
    }

    if(0u == u8CmpRet)
    {
        LED_G_ON();      /* Meet the expected */
    }
    else
    {
        LED_R_ON();      /* Don't meet the expected */
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
