# Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.20)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# Set target definition variables required by the toolchain file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/target.cmake)

# Set the toolchain file for cross-compilation
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake)

# Project definition
project(stm32g030_bootloader)

# Enable CMake support for ASM and C/CPP languages
enable_language(C ASM)

set(PROJECT_NAME stm32g030_bootloader)
 
# Create an executable object type
add_executable(${PROJECT_NAME}
  
  App/app.c
  Drivers/gpio/gpio.c
  Drivers/gpio/gpio_hal.c
  Drivers/timecore/timecore_hal.c
  Drivers/timecore/timecore.c
  Drivers/led/led.c
  Drivers/systemclock/systemclock.c
  Drivers/uart/uart_driver.c
  HAL/uart/uart_hal.c
  # Services/logger/core/logger.c
  # Services/logger/core/logger_hal.c
  # Services/logger/backends/uart/logger_uart.c
  Libraries/containers/ringbuffer/ring_buffer.c
  Libraries/crc/crc.c
  Bootloader/core/bootloader.c
  Bootloader/protocol/bl_protocol.c
  Bootloader/transport/bl_transport.c
  Bootloader/transport/uart/bootloader_uart.c
  Bootloader/flash/bl_flash.c
  Bootloader/bootloader_app.c
  Bootloader/metadata/bl_metadata.c
  Bootloader/jump/bl_jump.c

 )

# Include additional CMake files for flags, components, and files
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/flags.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/components.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/files.cmake)

# Add sources to executable/library
target_sources(${PROJECT_NAME} PRIVATE
  # User defined sources
)

# =============================================================================
# Include Directories
# =============================================================================

target_include_directories(${PROJECT_NAME} PRIVATE

    # -------------------------------------------------------------------------
    # Application / Drivers
    # -------------------------------------------------------------------------

    ${CMAKE_CURRENT_SOURCE_DIR}/App
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP


    # -------------------------------------------------------------------------
    # Services
    # -------------------------------------------------------------------------

    ${CMAKE_CURRENT_SOURCE_DIR}/Services


    # -------------------------------------------------------------------------
    # Configuration
    # -------------------------------------------------------------------------

    ${CMAKE_CURRENT_SOURCE_DIR}/Config


    # -------------------------------------------------------------------------
    # Libraries
    # -------------------------------------------------------------------------

    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries

 
    # -------------------------------------------------------------------------
    # HAL
    # -------------------------------------------------------------------------

    ${CMAKE_CURRENT_SOURCE_DIR}/HAL

    # -------------------------------------------------------------------------
    # Bootloader
    # -------------------------------------------------------------------------

    ${CMAKE_CURRENT_SOURCE_DIR}/Bootloader

)

# Compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
  # User defined compiler options
  $<$<COMPILE_LANGUAGE:C>:-flto>
)

# Add additional defined symbols
target_compile_definitions(${PROJECT_NAME} PRIVATE
  # Add additional defined symbols here
)

# Linker options
target_link_options(${PROJECT_NAME} PRIVATE 
  # User defined linker options
  -flto
)

# Add additional linker search directories
target_link_directories(${PROJECT_NAME} PRIVATE
  # Add additional linker search directories here
)

# Add additional linker libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  # Add additional linker libraries here
)

 

# =============================================================================
# Linker Script Configuration
#
# One linker template is used for all configurations.
# CMake generates the actual linker script inside the build directory.
# =============================================================================

set(LINKER_SCRIPT
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32g030x6_flash.ld
)

configure_file(
    ${LINKER_SCRIPT}
    ${CMAKE_BINARY_DIR}/stm32g030x6_flash.ld
    @ONLY
)


# =============================================================================
# Linker Options
# =============================================================================

target_link_options(${PROJECT_NAME} PRIVATE

    "-T${CMAKE_BINARY_DIR}/stm32g030x6_flash.ld"

)
