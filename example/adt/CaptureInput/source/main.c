/******************************************************************************
* Copyright (C) 2019, Xiaohua Semiconductor Co.,Ltd All rights reserved.
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
 **   - 2019-04-19 Husj    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adt.h"
#include "gpio.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint16_t u16CaptureA;
static uint16_t u16CaptureB;

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

void Tim4_IRQHandler(void)
{
    //捕获中断A
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM4, AdtCMAIrq))
    {        
        Adt_GetCaptureValue(M0P_ADTIM4, AdtCHxA, &u16CaptureA);  //脉冲低电平计数值
        
        Adt_ClearIrqFlag(M0P_ADTIM4, AdtCMAIrq);
    }
    //捕获中断B
    if(TRUE == Adt_GetIrqFlag(M0P_ADTIM4, AdtCMBIrq))
    {
        Adt_GetCaptureValue(M0P_ADTIM4, AdtCHxB, &u16CaptureB);   //脉冲高电平计数值
        
        Adt_ClearIrqFlag(M0P_ADTIM4, AdtCMBIrq);
    }
}


///< AdvTimer端口初始化
void App_AdtPortInit(void)
{
    stc_gpio_cfg_t           stcTIM4Port;
    
    DDL_ZERO_STRUCT(stcTIM4Port);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);  //端口外设时钟使能
    
    stcTIM4Port.enDir  = GpioDirIn;
    //PA08设置为TIM4_CHA
    Gpio_Init(GpioPortA, GpioPin8, &stcTIM4Port);
    Gpio_SetAfMode(GpioPortA,GpioPin8,GpioAf6);

    //PA07设置为TIM4_CHB
    //Gpio_Init(GpioPortA, GpioPin7, &stcTIM4Port);
    //Gpio_SetAfMode(GpioPortA,GpioPin7,GpioAf7);
  
}


///< AdvTimer初始化
void App_AdvTimerInit(void)
{
    uint16_t                 u16Period;
    stc_adt_basecnt_cfg_t    stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t  stcAdtTIM4ACfg;
    stc_adt_CHxX_port_cfg_t  stcAdtTIM4BCfg;

    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM4ACfg);
    DDL_ZERO_STRUCT(stcAdtTIM4BCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE); //ADT外设时钟使能
    
    
    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;              //Sawtooth Mode
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;                      // Cnt up
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div4;            // PCLK0/4
    Adt_Init(M0P_ADTIM4, &stcAdtBaseCntCfg);                   //ADT载波、计数模式、时钟配置
    
    u16Period = 0xFFFF;
    Adt_SetPeriod(M0P_ADTIM4, u16Period);                      //周期设置
    
    stcAdtTIM4ACfg.enCap = AdtCHxCompareInput;                 //Channel A 作为捕获输入(used as capture input)
    stcAdtTIM4ACfg.bFltEn = TRUE;
    stcAdtTIM4ACfg.enFltClk = AdtFltClkPclk0Div16;
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxA, &stcAdtTIM4ACfg);     //Channel A配置 & GPIO CHA 输入滤波使能
    
    stcAdtTIM4BCfg.enCap = AdtCHxCompareInput;                 //Channel B 作为捕获输入(used as capture input)
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxB, &stcAdtTIM4BCfg);
    
    Adt_CfgHwCaptureA(M0P_ADTIM4, AdtHwTrigCHxARise);       //硬件捕获A条件配置:CHA端口上采样到上升沿
    Adt_CfgHwCaptureB(M0P_ADTIM4, AdtHwTrigCHxAFall);       //硬件捕获B条件配置:CHA端口上采样到下降沿
    
    Adt_CfgHwClear(M0P_ADTIM4, AdtHwTrigCHxARise);          //硬件清零条件：CHA 端口采样到上升沿
    Adt_CfgHwClear(M0P_ADTIM4, AdtHwTrigCHxAFall);          //硬件清零条件：CHA 端口采样到下降沿
    Adt_EnableHwClear(M0P_ADTIM4);
    
    Adt_ClearAllIrqFlag(M0P_ADTIM4);
    Adt_CfgIrq(M0P_ADTIM4, AdtCMAIrq, TRUE);    //捕获中断A配置
    Adt_CfgIrq(M0P_ADTIM4, AdtCMBIrq, TRUE);    //捕获中断B配置
    EnableNvic(ADTIM4_IRQn, IrqLevel3, TRUE);   //AdvTimer4中断使能
}

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
    App_AdtPortInit();         //AdvTimer4 端口初始化
    
    App_AdvTimerInit();        //AdvTimer4 初始化
    
    Adt_StartCount(M0P_ADTIM4);   //AdvTimer4 运行
    
    while(1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


