/*
 * Servo42C Firmware – Version v0.1 (Blinky)
 * =========================================
 *
 * Target MCU  : HC32L130F8UA (Cortex-M0+ @ 48 MHz)
 * Board       : MKS SERVO42C
 * Function    : Blink on-board LED D3 wired to PC14
 *
 * Toolchain   : Keil Studio / VS Code + Arm-Clang or GCC
 * Drivers     : Use the HC32L130 DDL drivers from:
 *               ├── RTE/Device/HC32L130F8UA        (CMSIS + startup)
 *               └── driver/{inc,src}/              (DDL peripheral drivers)
 */

#include "RTE_Components.h"
#include CMSIS_device_header             /* Device definitions */
#include "ddl.h"                         /* Delay and common macros */
#include "gpio.h"                        /* GPIO driver */
#include "sysctrl.h"                     /* System control for peripheral clocks */

/* -------------------------------------------------------------------------- */
#define FW_VERSION             "v0.1"

#define LED_PORT               GpioPortC
#define LED_PIN                GpioPin14
#define BLINK_PERIOD_MS        (250U)

/* -------------------------------------------------------------------------- */
static void LedGpioInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    /* Clear structure */
    DDL_ZERO_STRUCT(stcGpioCfg);

    /* 1) Enable GPIO clock */
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    /* 2) Configure PC14 as output */
    stcGpioCfg.enDir  = GpioDirOut;
    Gpio_Init(LED_PORT, LED_PIN, &stcGpioCfg);

    /* 3) Turn off LED initially */
    Gpio_ClrIO(LED_PORT, LED_PIN);
}

/* -------------------------------------------------------------------------- */
int32_t main(void)
{
    /* System clock is already configured in startup */

    LedGpioInit();

    while (1)
    {
        Gpio_SetIO(LED_PORT, LED_PIN);
        delay1ms(BLINK_PERIOD_MS);
        Gpio_ClrIO(LED_PORT, LED_PIN);
        delay1ms(BLINK_PERIOD_MS);
    }
}
