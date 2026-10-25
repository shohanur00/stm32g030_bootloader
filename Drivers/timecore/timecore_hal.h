
/**
 * @file timecore_hal.h
 * @brief Hardware Abstraction Layer for TimeCore module
 * @author Md. Shohanur Rahman
 * @version 1.0.0
 * @date 25/10/2026
 * @details
 * Hardware abstraction for TimeCore on STM32G030F6P6.
 * Provides timer selection, peripheral mapping and HAL interfaces.
 */

#ifndef TIMECORE_HAL_H
#define TIMECORE_HAL_H

#include <stdint.h>
#include "stm32g030xx.h"

/* ============================================================
 * TimeCore Timer Selection
 * ============================================================ */

#define TIMECORE_HAL_TIMER_TIM1       1U
#define TIMECORE_HAL_TIMER_TIM3       3U
#define TIMECORE_HAL_TIMER_TIM14     14U
#define TIMECORE_HAL_TIMER_TIM16     16U
#define TIMECORE_HAL_TIMER_TIM17     17U

/* ============================================================
 * Selected Timer
 * ============================================================ */

/* Change this value to select another supported timer. */
#define TIMECORE_HAL_TIMER            TIMECORE_HAL_TIMER_TIM14

/* ============================================================
 * Timer Hardware Mapping
 * ============================================================ */

#if (TIMECORE_HAL_TIMER == TIMECORE_HAL_TIMER_TIM1)

    #define TIMECORE_HAL_TIMER_INSTANCE    TIM1
    #define TIMECORE_HAL_TIMER_IRQn        TIM1_BRK_UP_TRG_COM_IRQn

    #define TIMECORE_HAL_TIMER_ENABLE()    \
        (RCC->APBENR2 |= RCC_APBENR2_TIM1EN)

    #define TIMECORE_HAL_TIMER_DISABLE()   \
        (RCC->APBENR2 &= ~RCC_APBENR2_TIM1EN)


#elif (TIMECORE_HAL_TIMER == TIMECORE_HAL_TIMER_TIM3)

    #define TIMECORE_HAL_TIMER_INSTANCE    TIM3
    #define TIMECORE_HAL_TIMER_IRQn        TIM3_IRQn

    #define TIMECORE_HAL_TIMER_ENABLE()    \
        (RCC->APBENR1 |= RCC_APBENR1_TIM3EN)

    #define TIMECORE_HAL_TIMER_DISABLE()   \
        (RCC->APBENR1 &= ~RCC_APBENR1_TIM3EN)


#elif (TIMECORE_HAL_TIMER == TIMECORE_HAL_TIMER_TIM14)

    #define TIMECORE_HAL_TIMER_INSTANCE    TIM14
    #define TIMECORE_HAL_TIMER_IRQn        TIM14_IRQn

    #define TIMECORE_HAL_TIMER_ENABLE()    \
        (RCC->APBENR2 |= RCC_APBENR2_TIM14EN)

    #define TIMECORE_HAL_TIMER_DISABLE()   \
        (RCC->APBENR2 &= ~RCC_APBENR2_TIM14EN)


#elif (TIMECORE_HAL_TIMER == TIMECORE_HAL_TIMER_TIM16)

    #define TIMECORE_HAL_TIMER_INSTANCE    TIM16
    #define TIMECORE_HAL_TIMER_IRQn        TIM16_IRQn

    #define TIMECORE_HAL_TIMER_ENABLE()    \
        (RCC->APBENR2 |= RCC_APBENR2_TIM16EN)

    #define TIMECORE_HAL_TIMER_DISABLE()   \
        (RCC->APBENR2 &= ~RCC_APBENR2_TIM16EN)


#elif (TIMECORE_HAL_TIMER == TIMECORE_HAL_TIMER_TIM17)

    #define TIMECORE_HAL_TIMER_INSTANCE    TIM17
    #define TIMECORE_HAL_TIMER_IRQn        TIM17_IRQn

    #define TIMECORE_HAL_TIMER_ENABLE()    \
        (RCC->APBENR2 |= RCC_APBENR2_TIM17EN)

    #define TIMECORE_HAL_TIMER_DISABLE()   \
        (RCC->APBENR2 &= ~RCC_APBENR2_TIM17EN)

#else

    #error "Unsupported timer selected for STM32G030F6P6"

#endif

/* ============================================================
 * HAL Interface
 * ============================================================ */

/**
 * @brief Initialize the TimeCore hardware abstraction layer.
 */
void TimeCore_HAL_Init(void);

/**
 * @brief Deinitialize the TimeCore hardware abstraction layer.
 */
void TimeCore_HAL_Deinit(void);

/**
 * @brief Retrieve elapsed system time in milliseconds.
 * @return Elapsed time in milliseconds.
 */
uint32_t TimeCore_HAL_GetTick(void);

/**
 * @brief Reset the system time counter.
 */
void TimeCore_HAL_ResetTick(void);

/**
 * @brief Clear the timer update event flag.
 */
void TimeCore_HAL_ClearUpdateFlag(void);

/**
 * @brief Retrieve the timer update event flag status.
 * @return Non-zero if the update flag is set.
 */
uint8_t TimeCore_HAL_GetUpdateFlag(void);

/**
 * @brief Handle the timer update interrupt.
 */
void TimeCore_HAL_IRQHandler(void);

/**
 * @brief Execute the TimeCore HAL diagnostic test.
 */
void TimeCore_HAL_Test(void);

#endif /* TIMECORE_HAL_H */
