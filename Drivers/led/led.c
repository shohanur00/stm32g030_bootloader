#include "led.h"
#include "stm32g030xx.h"

/* LED pin configuration */
#define LED_GPIO_PORT       GPIOA
#define LED_GPIO_PIN        0U

#define LED_GPIO_CLK_EN()   (RCC->IOPENR |= RCC_IOPENR_GPIOAEN)

/* Active-high LED: GPIO HIGH = ON */
void LED_Init(void)
{
    /* Enable GPIOA clock */
    LED_GPIO_CLK_EN();

    /* Configure PA0 as output */
    GPIOA->MODER &= ~(3UL << (LED_GPIO_PIN * 2U));
    GPIOA->MODER |=  (1UL << (LED_GPIO_PIN * 2U));

    /* Push-pull output */
    GPIOA->OTYPER &= ~(1UL << LED_GPIO_PIN);

    /* Low speed */
    GPIOA->OSPEEDR &= ~(3UL << (LED_GPIO_PIN * 2U));

    /* No pull-up / pull-down */
    GPIOA->PUPDR &= ~(3UL << (LED_GPIO_PIN * 2U));

    /* Start with LED OFF */
    LED_Off();
}

void LED_On(void)
{
    LED_GPIO_PORT->BSRR = (1UL << LED_GPIO_PIN);
}

void LED_Off(void)
{
    LED_GPIO_PORT->BSRR = (1UL << (LED_GPIO_PIN + 16U));
}

void LED_Toggle(void)
{
    LED_GPIO_PORT->ODR ^= (1UL << LED_GPIO_PIN);
}

void LED_Set(uint8_t state)
{
    if (state != 0U)
    {
        LED_On();
    }
    else
    {
        LED_Off();
    }
}
