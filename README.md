![Platform](https://img.shields.io/badge/Platform-STM32G0-blue)
![MCU](https://img.shields.io/badge/MCU-STM32G030F6P6-03234B)
![Architecture](https://img.shields.io/badge/Architecture-ARM%20Cortex--M0%2B-green)
![Interface](https://img.shields.io/badge/Interface-UART-orange)
![Language](https://img.shields.io/badge/Language-C-00599C)
![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-03234B)
![Status](https://img.shields.io/badge/Status-Under%20Development-yellow)

# STM32G030F6P6 UART Bootloader

A custom UART bootloader for the STM32G030F6P6 microcontroller, designed to provide firmware programming, application management, and serial firmware update capabilities for embedded systems.

## Overview

This project focuses on implementing a UART-based bootloader for the STM32G030F6P6, an ARM Cortex-M0+ microcontroller from the STM32G0 series.

The bootloader provides a mechanism for communicating with an external host over UART, receiving firmware data, and transferring control to the main application.

## Features

- **Target MCU:** STM32G030F6P6
- **MCU Family:** STM32G0
- **CPU Core:** ARM Cortex-M0+
- **Communication Interface:** UART
- **Firmware Transfer:** Serial communication
- **Flash Memory Management:** Bootloader and application separation
- **Application Handover:** Transfer execution to the main application
- **Firmware Update:** UART-based firmware update capability

*Features listed above are subject to the current implementation.*

## Hardware Requirements

- STM32G030F6P6 microcontroller
- ST-Link debugger/programmer
- USB-to-UART adapter or compatible serial interface
- Host computer for firmware transfer

## Software Requirements

- STM32CubeIDE
- STM32CubeProgrammer
- Serial terminal or host-side firmware transfer utility
- Compatible ARM embedded toolchain

## How It Works

1. The microcontroller starts execution from the bootloader.
2. The bootloader initializes the required peripherals.
3. It communicates with a host device through UART.
4. Firmware data can be received according to the implemented protocol.
5. The bootloader can transfer execution to the application when the appropriate conditions are met.

## Memory Layout

The bootloader and application must occupy separate, properly configured flash memory regions.

| Memory Region | Purpose |
|---|---|
| Bootloader | Bootloader code and routines |
| Application | Main application firmware |
| Reserved Area | Optional firmware metadata or configuration |

Exact memory addresses and sizes depend on the project configuration.

## Build and Flash

1. Clone this repository.
2. Open the project in STM32CubeIDE.
3. Verify the MCU configuration and flash memory layout.
4. Build the bootloader firmware.
5. Flash the firmware using ST-Link or a compatible programmer.
6. Connect the UART interface to a host computer.
7. Test communication and firmware update operations using the supported protocol.

## Development Status

**Status:** Under Development

UART communication, firmware transfer, application validation, and startup behavior should be verified against the current source code.

## License

Specify the license applicable to this project before redistribution.

---

Developed for embedded applications using the STM32G030F6P6 microcontroller.
