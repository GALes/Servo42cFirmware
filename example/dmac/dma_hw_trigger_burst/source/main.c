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
**   - 2018-03-09  1.0  Lux First version
**
******************************************************************************/

/******************************************************************************
* Include files
******************************************************************************/
#include "ddl.h"
#include "dmac.h"
#include "adc.h"
#include "gpio.h"
#include "bgr.h"
/******************************************************************************
* Local pre-processor symbols/macros ('#define')                            
******************************************************************************/

/******************************************************************************
* Global variable definitions (declared in header file with 'extern')
******************************************************************************/
uint16_t u16AdcRestult0;
uint16_t u16AdcRestult2;
uint16_t u16AdcRestult5;

//static stc_adc_irq_t stcAdcIrqFlag;
/******************************************************************************
* Local type definitions ('typedef')                                         
******************************************************************************/

/******************************************************************************
* Local function prototypes ('static')
******************************************************************************/

/******************************************************************************
* Local variable definitions ('static')                                      *
******************************************************************************/
static uint32_t ADC_Result_Array[16] = {0};
/******************************************************************************
* Local pre-processor symbols/macros ('#define')                             
******************************************************************************/

/*****************************************************************************
* Function implementation - global ('extern') and local ('static')
******************************************************************************/
void App_PortCfg(void);
void App_AdcInit(void);
void App_AdcSqrCfg(void);
void App_DmaCfg(void);
/**
******************************************************************************
** \brief  Main function of project
**
** \return uint32_t return value, if needed
**
******************************************************************************/
int32_t main(void)
{
    // 端口功能配置
    App_PortCfg();
    
    // ADC模块初始化
    App_AdcInit();
    
    // ADC 顺序扫描功能 配置
    App_AdcSqrCfg();
    
    // DMA通道配置， ADC SQR触发DMA传输
    App_DmaCfg();
    
    // 启动ADC顺序扫描转换，DMA Burst模式下只需启动一次
    Adc_SQR_Start();  

    // 等待传输完成
    while(Dma_GetStat(DmaCh0) != DmaTransferComplete);
    
    while (1)
    {
      ;
    }
}

// 端口功能配置
void App_PortCfg(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);   // 开启GPIO时钟模块
    
    Gpio_SetAnalogMode(GpioPortA, GpioPin0);                 //配置PA00 (AIN0)   
    Gpio_SetAnalogMode(GpioPortA, GpioPin2);                 //配置PA02 (AIN2)
    Gpio_SetAnalogMode(GpioPortA, GpioPin5);                 //配置PA05 (AIN5)  
}

// ADC模块初始化
void App_AdcInit(void)
{
    stc_adc_cfg_t              stcAdcCfg;

    //结构体变量 初始值清零
    DDL_ZERO_STRUCT(stcAdcCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr,TRUE);   // 开启ADC和BGR时钟模块
    
    Bgr_BgrEnable();                                   //使能BGR

    stcAdcCfg.enAdcMode = AdcScanMode;                 //扫描模式
    stcAdcCfg.enAdcClkDiv = AdcMskClkDiv1;             //ADC时钟分频为PCLK
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle8Clk; //采样时钟选择为8个采样时钟
    stcAdcCfg.enAdcRefVolSel = AdcMskRefVolSelAVDD;    //ADC参考电压为AVDD
    stcAdcCfg.enAdcOpBuf = AdcMskBufDisable;           //关闭放大器BUF
    stcAdcCfg.enInRef = AdcMskInRefDisable;            //内部参考电压关闭
    stcAdcCfg.enAdcAlign  = AdcAlignRight;             //ADC运算结果右对齐
  
    Adc_Init(&stcAdcCfg);                              //初始化ADC
}

// ADC 顺序扫描功能 配置
void App_AdcSqrCfg(void)
{
    stc_adc_sqr_cfg_t          stcAdcSqrCfg;

    //结构体变量 初始值清零
    DDL_ZERO_STRUCT(stcAdcSqrCfg);
    
    //配置顺序扫描转换通道,转换通道0 4 7配置为AIN0;转换通道1 5 8配置为AIN2;转换通道2 6 9配置为AIN5
    Adc_CfgSqrChannel(AdcSQRCH0MUX, AdcExInputCH0);
    Adc_CfgSqrChannel(AdcSQRCH1MUX, AdcExInputCH2);
    Adc_CfgSqrChannel(AdcSQRCH2MUX, AdcExInputCH5);
    Adc_CfgSqrChannel(AdcSQRCH4MUX, AdcExInputCH0);
    Adc_CfgSqrChannel(AdcSQRCH5MUX, AdcExInputCH2);
    Adc_CfgSqrChannel(AdcSQRCH6MUX, AdcExInputCH5);
    Adc_CfgSqrChannel(AdcSQRCH7MUX, AdcExInputCH0);
    Adc_CfgSqrChannel(AdcSQRCH8MUX, AdcExInputCH2);
    Adc_CfgSqrChannel(AdcSQRCH9MUX, AdcExInputCH5);
      
    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);     //使能NVIC对应中断位
    
    Adc_EnableIrq();                               //使能ADC中断功能

    stcAdcSqrCfg.enResultAcc = AdcResultAccDisable;//禁用ADC转换结果自动累加功能
    stcAdcSqrCfg.u8SqrCnt = 9;                     //ADC顺序扫描转换次数
    stcAdcSqrCfg.bSqrDmaTrig = TRUE;               //ADC顺序扫描转换完成后，触发DMA数据传输
    Adc_SqrModeCfg(&stcAdcSqrCfg);                 //配置顺序扫描转换模式
}

// DMA通道配置， ADC SQR触发DMA传输
void App_DmaCfg(void)
{
    stc_dma_cfg_t           stcDmaCfg;    
    
    DDL_ZERO_STRUCT(stcDmaCfg);   
    
    // 使能DMA时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralDma,TRUE);   
    
    stcDmaCfg.enMode =  DmaMskBurst;                             //选择突发(Burst)传输    
    stcDmaCfg.u16BlockSize = 0x03u;                              //块传输个数
    stcDmaCfg.u16TransferCnt = 0x03u;                            //burst模式，一次触发传输数据大小为 3*3,
    stcDmaCfg.enTransferWidth = DmaMsk32Bit;                     //传输数据的宽度，此处选择字(32Bit)宽度     
    stcDmaCfg.enSrcAddrMode = DmaMskSrcAddrInc;                  //源地址自增
    stcDmaCfg.enDstAddrMode = DmaMskDstAddrInc;                  //目的地址自增
    stcDmaCfg.enDestAddrReloadCtl = DmaMskDstAddrReloadDisable;  //禁止重新加载传输目的地址
    stcDmaCfg.enSrcAddrReloadCtl = DmaMskSrcAddrReloadDisable;   //禁止重新加载传输源地址
    stcDmaCfg.enSrcBcTcReloadCtl = DmaMskBcTcReloadDisable;      //禁止重新加载BC/TC值
    stcDmaCfg.u32SrcAddress = (uint32_t) &(M0P_ADC->SQRRESULT0); //指定传输源地址
    stcDmaCfg.u32DstAddress = (uint32_t)&ADC_Result_Array[0];    //指定传输目的地址
    stcDmaCfg.enTransferMode = DmaMskOneTransfer;                //DMAC 在传输完成时清除 CONFA:ENS 位。允许Dma传输一次。
    stcDmaCfg.enRequestNum = DmaADCSQRTrig;                      //设置为ADCSQR触发
    
    Dma_InitChannel(DmaCh0,&stcDmaCfg);                          //初始化DMA通道0
    
    //使能DMA，使能DMA0
    Dma_Enable();
    Dma_EnableChannel(DmaCh0);

}

// Adc 中断处理函数
void Adc_IRQHandler(void)
{    
    if(TRUE == Adc_GetIrqStatus(AdcMskIrqSqr))            //adc顺序扫描完成中断
    {
        u16AdcRestult0 = Adc_GetSqrResult(AdcSQRCH0MUX);  //获取采样值
        u16AdcRestult2 = Adc_GetSqrResult(AdcSQRCH1MUX);
        u16AdcRestult5 = Adc_GetSqrResult(AdcSQRCH2MUX);

        Adc_ClrIrqStatus(AdcMskIrqSqr);  //清中断状态位
    }        
    
}

/******************************************************************************
* EOF (not truncated)
******************************************************************************/


