#include "bl_jump.h"

#include "stm32g030xx.h"
#include "../config/bl_flash_config.h"


/* ============================================================================
 * Private Definitions
 * ========================================================================== */

#define BL_APP_STACK_POINTER_OFFSET      (0x00UL)
#define BL_APP_RESET_HANDLER_OFFSET      (0x04UL)

#define BL_SRAM_START_ADDRESS            (0x20000000UL)
#define BL_SRAM_SIZE                     (8UL * 1024UL)

#define BL_SRAM_END_ADDRESS_EXCLUSIVE    \
    (BL_SRAM_START_ADDRESS + BL_SRAM_SIZE)


/* ============================================================================
 * Application Validation
 * ========================================================================== */

uint8_t BL_Jump_IsApplicationValid(void)
{
    uint32_t stack_pointer;
    uint32_t reset_handler;
    uint32_t reset_handler_address;

    /*
     * Read application vector table.
     *
     * Offset 0x00: Initial MSP
     * Offset 0x04: Reset_Handler
     */

    stack_pointer =
        *(volatile const uint32_t *)
        (BL_APP_START_ADDRESS + BL_APP_STACK_POINTER_OFFSET);

    reset_handler =
        *(volatile const uint32_t *)
        (BL_APP_START_ADDRESS + BL_APP_RESET_HANDLER_OFFSET);


    /* ------------------------------------------------------------------------
     * Validate initial stack pointer
     * ---------------------------------------------------------------------- */

    /*
     * SRAM range:
     * Start      = 0x20000000
     * End        = 0x20001FFF
     * Upper limit = 0x20002000 (valid initial MSP)
     */

    if ((stack_pointer < BL_SRAM_START_ADDRESS) ||
        (stack_pointer > BL_SRAM_END_ADDRESS_EXCLUSIVE))
    {
        return 0U;
    }

    /*
     * Initial MSP must be 8-byte aligned.
     */

    if ((stack_pointer & 0x7UL) != 0UL)
    {
        return 0U;
    }


    /* ------------------------------------------------------------------------
     * Validate Reset_Handler Thumb bit
     * ---------------------------------------------------------------------- */

    if ((reset_handler & 0x1UL) == 0UL)
    {
        return 0U;
    }


    /* ------------------------------------------------------------------------
     * Validate Reset_Handler Flash address
     * ---------------------------------------------------------------------- */

    /*
     * Remove Thumb bit before checking the address range.
     */

    reset_handler_address = reset_handler & ~0x1UL;

    /*
     * Application region:
     * Start = 0x08001800
     * End   = 0x080077FF
     *
     * BL_APP_END_ADDRESS is inclusive.
     */

    if ((reset_handler_address < BL_APP_START_ADDRESS) ||
        (reset_handler_address > BL_APP_END_ADDRESS))
    {
        return 0U;
    }


    return 1U;
}


/* ============================================================================
 * Jump To Application
 * ========================================================================== */

void BL_Jump_ToApplication(void)
{
    uint32_t app_stack_pointer;
    uint32_t app_reset_handler;

    uint32_t i;
    uint32_t nvic_register_count;


    /* ------------------------------------------------------------------------
     * Validate application
     * ---------------------------------------------------------------------- */

    if (BL_Jump_IsApplicationValid() == 0U)
    {
        return;
    }


    /* ------------------------------------------------------------------------
     * Read application vector table
     * ---------------------------------------------------------------------- */

    app_stack_pointer =
        *(volatile const uint32_t *)
        (BL_APP_START_ADDRESS + BL_APP_STACK_POINTER_OFFSET);

    app_reset_handler =
        *(volatile const uint32_t *)
        (BL_APP_START_ADDRESS + BL_APP_RESET_HANDLER_OFFSET);


    /* ------------------------------------------------------------------------
     * Disable global interrupts
     * ---------------------------------------------------------------------- */

    // __disable_irq();

    __DSB();
    __ISB();


    /* ------------------------------------------------------------------------
     * Stop SysTick
     * ---------------------------------------------------------------------- */

    SysTick->CTRL = 0UL;
    SysTick->LOAD = 0UL;
    SysTick->VAL  = 0UL;


    /* ------------------------------------------------------------------------
     * Disable and clear NVIC interrupts
     * ---------------------------------------------------------------------- */

    nvic_register_count =
        sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]);

    for (i = 0UL; i < nvic_register_count; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFFUL;
        NVIC->ICPR[i] = 0xFFFFFFFFUL;
    }

    __DSB();
    __ISB();


    /* ------------------------------------------------------------------------
     * Bootloader peripheral cleanup
     * ---------------------------------------------------------------------- */

    /*
     * Disable bootloader-specific peripheral interrupt sources.
     *
     * If UART or DMA is used by the bootloader, stop active DMA transfers
     * and disable UART/DMA interrupt generation here.
     *
     * Implement the required cleanup using the actual bootloader drivers.
     *
     * Clearing NVIC alone does not disable peripheral interrupt sources.
     */


    /* ------------------------------------------------------------------------
     * Relocate vector table to application
     * ---------------------------------------------------------------------- */

    SCB->VTOR = BL_APP_START_ADDRESS;

    __DSB();
    __ISB();


    /* ------------------------------------------------------------------------
     * Final handoff to application
     * ---------------------------------------------------------------------- */

    /*
     * Load the application's MSP and branch directly to Reset_Handler.
     *
     * Do not use __set_MSP() followed by a normal C function call.
     *
     * R0 and R1 are used as temporary registers. The assembly branch
     * does not return to the bootloader.
     */

    __asm volatile
    (
        "mov r0, %0       \n"
        "mov r1, %1       \n"
        "msr msp, r0      \n"
        "bx   r1          \n"
        :
        : "r" (app_stack_pointer),
          "r" (app_reset_handler)
        : "r0", "r1", "memory"
    );


    /*
     * Defensive fallback.
     * Normally unreachable.
     */

    while (1)
    {
        __NOP();
    }
}