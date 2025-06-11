/******************************************************************************
* Copyright (C) 2018, Xiaohua Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Xiaohua Semiconductor Co.,Ltd ("XHSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with XHSC
* components. This software is licensed by XHSC to be adapted only
* for use in systems utilizing XHSC components. XHSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. XHSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* XHSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* XHSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
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
*/
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2018-05-08  1.0  Lux First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "flash.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//#define RUN_IN_RAM 1    //need to cfg *.icf

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint32_t           u32TestBuffer[128] = {0};
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/

int32_t main(void)
{
    uint32_t          u32Addr         = 0xFE00;

    ///< STEP-1: 确保初始化正确执行后方能进行FLASH编程操作，FLASH初始化（编程时间,休眠模式配置）
    while(Ok != Flash_Init(1, TRUE))
    {
        while(1);
    }

    ///< STEP-2 FLASH 操作区解锁(Sector124~127)
    Flash_LockSet(0x80000000);
    
//< ==> 8位编程     
    ///< STEP-3 FLASH目标扇区擦除
    while(Ok != Flash_OpModeConfig(FlashSectorEraseMode));
    if(Ok != Flash_SectorErase(u32Addr))
    {
        while(1);
    }
    ///< STEP-4 FLASH 字节写、校验    
    while(Ok != Flash_OpModeConfig(FlashWriteMode));
    if (Ok != Flash_Write8(u32Addr, (uint8_t *)u32TestBuffer, 512))
    {
        while(1);
    }

    
    
//< ==> 16位编程    
    ///< FLASH目标扇区擦除
    while(Ok != Flash_OpModeConfig(FlashSectorEraseMode));
    if(Ok != Flash_SectorErase(u32Addr))
    {
        while(1);
    }
    ///< FLASH 字节写、校验
    while(Ok != Flash_OpModeConfig(FlashWriteMode));
    if (Ok != Flash_Write16(u32Addr, (uint16_t *)u32TestBuffer, 256))
    {
        while(1);
    }


    
//< ==> 32位编程    
    ///< FLASH目标扇区擦除
    while(Ok != Flash_OpModeConfig(FlashSectorEraseMode));
    while(Ok != Flash_SectorErase(u32Addr))
    {
        while(1);
    }
    
    ///< FLASH 字节写、校验
    while(Ok != Flash_OpModeConfig(FlashWriteMode));
    if (Ok != Flash_Write32(u32Addr, u32TestBuffer, 128))
    {
        while(1);
    }


    ///< FLASH 读模式
    ///< 注意！！！当不对FLASH进行操作时，应当将FLASH操作状态设置为【读模式】
    while(Ok != Flash_OpModeConfig(FlashReadMode));
    ///< FLASH 全部加锁
    while(Ok != Flash_LockAll());
    
    while (1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
