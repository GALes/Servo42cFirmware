# Servo42C Firmware

Firmware for the [MKS SERVO42C](https://github.com/makerbase-mks/MKS-SERVO42C) board, based on the [HC32L130F8UA](https://www.xhsc.com.cn/product/292.html) microcontroller from XHSC. This project is developed in incremental stages, with each version enabling specific hardware functionality.

## Project Objective

Develop firmware from scratch for the MKS SERVO42C board, enabling its stepper motor control, absolute encoder reading, user interface via buttons and OLED display, and UART communication, following a modular, maintainable, and well-documented architecture.

## Development Tools

- Keil Studio Cloud / VS Code
- Arm-Clang or GCC toolchain
- CMSIS and DDL drivers provided by the manufacturer

## Hardware Overview

### MKS SERVO42C Board

- Manufacturer repository: [https://github.com/makerbase-mks/MKS-SERVO42C](https://github.com/makerbase-mks/MKS-SERVO42C)
- Schematic used: [MKS SERVO42C V1.00](https://github.com/makerbase-mks/MKS-SERVO42C/blob/MKS-SERVO42C-V1.1/Hardware/MKS%20SERVO42C%20V1.00/MKS%20SERVO42C-schematic.pdf)

### HC32L130F8UA Microcontroller

- Manufacturer: XHSC (Huada Semiconductor)
- Product page: [https://www.xhsc.com.cn/product/292.html](https://www.xhsc.com.cn/product/292.html)
- Core: ARM Cortex-M0+
- Max Frequency: 48 MHz
- Flash: 64 KB
- RAM: 8 KB

### Onboard Peripherals

- Absolute Encoder: MT6816
- Motor Driver: 4 discrete H-bridges (8 MOSFETs)
- Control Buttons: Next (PA09), Enter (PA10), Menu (PB04)
- OLED Display: I2C interface
- Serial Communication: UART

## Repository Structure

```
Servo42cFirmware/
├── Servo42cFirmware/           # Main source code
│   ├── RTE/Device/             # Startup and CMSIS for HC32L130F8UA
│   ├── driver/inc/             # Peripheral headers
│   ├── driver/src/             # Peripheral implementations
│   └── main.c                  # Main entry point
├── example/                    # Peripheral examples
├── .cproject.yml               # Keil Studio project configuration
└── README.md                   # This file
```

## Roadmap (Incremental Versions)

| Version | Description                                                 | Branch                    |
| ------- | ----------------------------------------------------------- | ------------------------- |
| v0.1    | Blink LED D3 connected to PC14                              | `v0.1_blinky`             |
| v0.2    | Read buttons Next (PA09), Enter (PA10), Menu (PB04)         | `v0.2_buttons`            |
| v0.3    | Communicate with OLED display via I2C                       | `v0.3_oled-display`       |
| v0.4    | Read MT6816 encoder via SPI                                 | `v0.4_encoder`            |
| v0.5    | Motor control using H-bridges and PWM generation            | `v0.5_motor-control`      |
| v0.6    | Basic UART debug communication                              | `v0.6_uart-debug`         |
| v0.x    | Full closed-loop control (encoder + motor feedback)         | `v0.x_main-control-loop`  |

## License

This project is released under an open-source hardware-compatible license. Attribution to the original MKS repository and the HC32 documentation is required.

---
Developed by [GALes](https://github.com/GALes) · 2025

