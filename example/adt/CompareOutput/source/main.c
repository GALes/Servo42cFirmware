/******************************************************************************
* Copyright (C) 2017, Xiaohua Semiconductor Co.,Ltd All rights reserved.
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
#include "flash.h"

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


/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
///< 时钟初始化
void App_ClockInit(void)
{
    en_flash_waitcycle_t      enFlashWait;
    stc_sysctrl_pll_cfg_t     stcPLLCfg;
    
    DDL_ZERO_STRUCT(stcPLLCfg);
    
    enFlashWait = FlashWaitCycle1;                      //读等待周期设置为1（当HCLK大于24MHz时必须至少为1）
    Flash_WaitCycle(enFlashWait);                       // Flash 等待1个周期
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq4_6MHz;     //RCH 4MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  //PLL 输出48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllRch;              //输入时钟源选择RCH
    stcPLLCfg.enPllMul    = SysctrlPllMul12;            //4MHz x 12 = 48MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE);
    
    Sysctrl_SysClkSwitch(SysctrlClkPLL);   ///< 时钟切换
}

///< AdvTimer端口初始化
void App_AdvTimerPortInit(void)
{
    stc_gpio_cfg_t         stcTIM4Port;
    
    DDL_ZERO_STRUCT(stcTIM4Port);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //端口外设时钟使能
    
    stcTIM4Port.enDir  = GpioDirOut;
    //PA08设置为TIM4_CHA
    Gpio_Init(GpioPortA, GpioPin8, &stcTIM4Port);
    Gpio_SetAfMode(GpioPortA,GpioPin8,GpioAf6);
    
    //PA11设置为TIM4_CHB
    Gpio_Init(GpioPortA, GpioPin11, &stcTIM4Port);
    Gpio_SetAfMode(GpioPortA,GpioPin11,GpioAf7);
}


///< AdvTimer初始化
void App_AdvTimerInit(uint16_t u16Period, uint16_t u16CHA_PWMDuty, uint16_t u16CHB_PWMDuty)
{
    en_adt_compare_t          enAdtCompareA;
    en_adt_compare_t          enAdtCompareB;

    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM4ACfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM4BCfg;
    
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM4ACfg);
    DDL_ZERO_STRUCT(stcAdtTIM4BCfg);
    

    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);    //ADT外设时钟使能
    
    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;                 //锯齿波模式
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0;
    
    Adt_Init(M0P_ADTIM4, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置
    
    Adt_SetPeriod(M0P_ADTIM4, u16Period);                         //周期设置
    
    enAdtCompareA = AdtCompareA;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompareA, u16CHA_PWMDuty);  //通用比较基准值寄存器A设置
    
    enAdtCompareB = AdtCompareB;
    Adt_SetCompareValue(M0P_ADTIM4, enAdtCompareB, u16CHB_PWMDuty);  //通用比较基准值寄存器B设置
    
    stcAdtTIM4ACfg.enCap = AdtCHxCompareOutput;            //比较输出
    stcAdtTIM4ACfg.bOutEn = TRUE;                          //CHA输出使能
    stcAdtTIM4ACfg.enPerc = AdtCHxPeriodLow;               //计数值与周期匹配时CHA电平保持不变
    stcAdtTIM4ACfg.enCmpc = AdtCHxCompareHigh;             //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM4ACfg.enStaStp = AdtCHxStateSelSS;            //CHA起始结束电平由STACA与STPCA控制
    stcAdtTIM4ACfg.enStaOut = AdtCHxPortOutLow;            //CHA起始电平为低
    stcAdtTIM4ACfg.enStpOut = AdtCHxPortOutLow;            //CHA结束电平为低
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxA, &stcAdtTIM4ACfg);   //端口CHA配置
    
    stcAdtTIM4BCfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM4BCfg.bOutEn = TRUE;
    stcAdtTIM4BCfg.enPerc = AdtCHxPeriodInv;
    stcAdtTIM4BCfg.enCmpc = AdtCHxCompareInv;
    stcAdtTIM4BCfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIM4BCfg.enStaOut = AdtCHxPortOutLow;
    stcAdtTIM4BCfg.enStpOut = AdtCHxPortOutLow;
    Adt_CHxXPortCfg(M0P_ADTIM4, AdtCHxB, &stcAdtTIM4BCfg);    //端口CHB配置
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
    App_ClockInit();     //时钟初始化
    
    App_AdvTimerInit(0xC000, 0x4000, 0x8000);  //AdvTimer4初始化
    //配置为锯齿波模式: 周期0xC000, CHA占空比设置0x4000，CHB占空比设置0x8000
    
    App_AdvTimerPortInit();    //AdvTimer端口初始化
    
    Adt_StartCount(M0P_ADTIM4); //AdvTimer4运行
    
    while(1);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


