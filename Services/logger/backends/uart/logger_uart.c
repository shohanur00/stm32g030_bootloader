
/**
 * @file logger_uart.c
 * @brief UART logger driver for STM32G030F6P6
 */

#include "logger_uart.h"
#include "board.h"
#include "systemclock/systemclock.h"

#include <stddef.h>
#include <stdint.h>

#define LOG_UART_CLOCK_FREQ_HZ    SYSTEMCLOCK_FREQ_HZ

/* ============================================================
 * UART Instance Selection
 * ============================================================ */

#if (LOG_UART_INSTANCE == 1U) || (LOG_UART_INSTANCE == 2U)

static USART_TypeDef *LOG_UART_GetInstance(void)
{
#if (LOG_UART_INSTANCE == 1U)

    return USART1;

#elif (LOG_UART_INSTANCE == 2U)

    return USART2;

#endif
}

#else

#error "Unsupported LOG_UART_INSTANCE. STM32G030F6P6 supports USART1 and USART2."

#endif

/* ============================================================
 * UART Clock Control
 * ============================================================ */

static void LOG_UART_ClockEnable(void)
{
#if (LOG_UART_INSTANCE == 1U)

    RCC->APBENR2 |= RCC_APBENR2_USART1EN;

#elif (LOG_UART_INSTANCE == 2U)

    RCC->APBENR1 |= RCC_APBENR1_USART2EN;

#endif
}

static void LOG_UART_ClockDisable(void)
{
#if (LOG_UART_INSTANCE == 1U)

    RCC->APBENR2 &= ~RCC_APBENR2_USART1EN;

#elif (LOG_UART_INSTANCE == 2U)

    RCC->APBENR1 &= ~RCC_APBENR1_USART2EN;

#endif
}

/* ============================================================
 * UART Configuration
 * ============================================================ */

static void LOG_UART_Config(void)
{
    USART_TypeDef *uart = LOG_UART_GetInstance();

    /* Disable UART before configuration */
    uart->CR1 = 0U;
    uart->CR2 = 0U;
    uart->CR3 = 0U;

    /*
     * Assumption:
     * USART clock source is PCLK and its frequency equals
     * SYSTEMCLOCK_FREQ_HZ.
     *
     * OVER8 = 0, so BRR = peripheral_clock / baud_rate.
     */
    uart->BRR = LOG_UART_CLOCK_FREQ_HZ / LOG_UART_BAUDRATE;

    /* --------------------------------------------------------
     * Parity Configuration
     * -------------------------------------------------------- */

#if (LOG_UART_PARITY == UART_PARITY_NONE)

    /* No parity */

#elif (LOG_UART_PARITY == UART_PARITY_EVEN)

    uart->CR1 |= USART_CR1_PCE;

#elif (LOG_UART_PARITY == UART_PARITY_ODD)

    uart->CR1 |= USART_CR1_PCE | USART_CR1_PS;

#else

#error "Unsupported LOG_UART_PARITY"

#endif

    /* --------------------------------------------------------
     * Stop Bit Configuration
     * -------------------------------------------------------- */

#if (LOG_UART_STOP_BITS == UART_STOPBITS_1)

    /* One stop bit: STOP = 00 */

#elif (LOG_UART_STOP_BITS == UART_STOPBITS_2)

    uart->CR2 |= USART_CR2_STOP_1;

#else

#error "Unsupported LOG_UART_STOP_BITS"

#endif

    /* Enable transmitter and UART peripheral */
    uart->CR1 |= USART_CR1_TE;
    uart->CR1 |= USART_CR1_UE;
}

/* ============================================================
 * Character Transmission
 * ============================================================ */

static void LOG_UART_WriteChar(char ch)
{
    USART_TypeDef *uart = LOG_UART_GetInstance();

    /* Wait until transmit data register is empty */
    while ((uart->ISR & USART_ISR_TXE_TXFNF) == 0U)
    {
    }

    uart->TDR = (uint8_t)ch;
}

/* ============================================================
 * Public API
 * ============================================================ */

void LOG_UART_Init(void)
{
    LOG_UART_ClockEnable();
    LOG_UART_Config();
}

void LOG_UART_Deinit(void)
{
    USART_TypeDef *uart = LOG_UART_GetInstance();

    /* Disable UART peripheral */
    uart->CR1 &= ~USART_CR1_UE;

    LOG_UART_ClockDisable();
}

void LOG_UART_Write(const char *buffer, size_t length)
{
    if (buffer == NULL)
    {
        return;
    }

    for (size_t i = 0U; i < length; i++)
    {
        LOG_UART_WriteChar(buffer[i]);
    }
}
