#include "bl_jump.h"

#include "stm32g030xx.h"
#include "../config/bl_flash_config.h"

/* --------------------------------------------------------------------------
 * Private Definitions
 * -------------------------------------------------------------------------- */

#define BL_APP_STACK_POINTER_OFFSET      (0x00U)
#define BL_APP_RESET_HANDLER_OFFSET      (0x04U)

#define BL_SRAM_START_ADDRESS            (0x20000000UL)
#define BL_SRAM_SIZE                     (8UL * 1024UL)
#define BL_SRAM_END_ADDRESS              \
    (BL_SRAM_START_ADDRESS + BL_SRAM_SIZE - 1UL)


/* --------------------------------------------------------------------------
 * Application Validation
 * -------------------------------------------------------------------------- */

uint8_t BL_Jump_IsApplicationValid(void)
{
    uint32_t stack_pointer;
    uint32_t reset_handler;

    /* Read application vector table */
    stack_pointer =
        *(volatile const uint32_t *)(BL_APP_START_ADDRESS +
                                     BL_APP_STACK_POINTER_OFFSET);

    reset_handler =
        *(volatile const uint32_t *)(BL_APP_START_ADDRESS +
                                     BL_APP_RESET_HANDLER_OFFSET);

    /* Initial MSP must be inside SRAM or exactly at its end.
       The stack pointer must be 8-byte aligned. */
    if ((stack_pointer < BL_SRAM_START_ADDRESS) ||
        (stack_pointer > (BL_SRAM_END_ADDRESS + 1UL)) ||
        ((stack_pointer & 0x7UL) != 0U))
    {
        return 0U;
    }

    /* Reset_Handler must be a Thumb address */
    if ((reset_handler & 0x01U) == 0U)
    {
        return 0U;
    }

    /* Reset_Handler must point into the application Flash region */
    if (((reset_handler & ~0x01UL) < BL_APP_START_ADDRESS) ||
        ((reset_handler & ~0x01UL) > BL_APP_END_ADDRESS))
    {
        return 0U;
    }

    return 1U;
}


/* --------------------------------------------------------------------------
 * Jump To Application
 * -------------------------------------------------------------------------- */

void BL_Jump_ToApplication(void)
{
    uint32_t app_stack_pointer;
    uint32_t app_reset_handler;

    void (*app_reset_handler_function)(void);

    if (BL_Jump_IsApplicationValid() == 0U)
    {
        return;
    }

    /* Read application vector table */
    app_stack_pointer =
        *(volatile const uint32_t *)(BL_APP_START_ADDRESS +
                                     BL_APP_STACK_POINTER_OFFSET);

    app_reset_handler =
        *(volatile const uint32_t *)(BL_APP_START_ADDRESS +
                                     BL_APP_RESET_HANDLER_OFFSET);

    /* Disable global interrupts */
    // __disable_irq();

    /* Disable SysTick */
    SysTick->CTRL = 0U;
    SysTick->LOAD = 0U;
    SysTick->VAL  = 0U;

    /* Disable and clear all implemented NVIC interrupts */
    for (uint32_t i = 0U;
         i < (sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]));
         i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFFUL;
        NVIC->ICPR[i] = 0xFFFFFFFFUL;
    }

    /*
     * Deinitialize bootloader peripherals before jumping.
     * For example: disable UART/DMA interrupts and reset
     * peripherals used by the bootloader.
     */

    /* Relocate vector table to application */
    SCB->VTOR = BL_APP_START_ADDRESS;

    __DSB();
    __ISB();

    /* Prepare reset handler function pointer */
    app_reset_handler_function =
        (void (*)(void))app_reset_handler;

    /*
     * Set the application's stack pointer and branch to Reset_Handler.
     * This final handoff is implemented in assembly so the compiler
     * cannot use the old stack after MSP changes.
     */
    __asm volatile
    (
        "msr msp, %0     \n"
        "bx  %1          \n"
        :
        : "r" (app_stack_pointer),
          "r" (app_reset_handler_function)
        : "memory"
    );

    /* Should never return */
    while (1)
    {
    }
}
