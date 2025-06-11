#include "RTE_Components.h"
#include CMSIS_device_header

// #include "base_types.h"

// int main() {
//     for (;;) {
//     }
// }


#include "gpio.h"

// int main() {
//     for (;;) {
//     }
// }


/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void App_LedInit(void);

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
    ///< LED端口初始化
    App_LedInit();

    while(1)
    {
        // ///< LED点亮
        // Gpio_SetIO(STK_LED_PORT, STK_LED_PIN);
        // delay1ms(1000);

        // ///< LED关闭
        // Gpio_ClrIO(STK_LED_PORT, STK_LED_PIN);
        // delay1ms(1000);
    }
}

static void App_LedInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
    
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LED关闭
    Gpio_ClrIO(STK_LED_PORT, STK_LED_PIN);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcGpioCfg);
    

}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

