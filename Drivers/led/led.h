#ifndef LED_H
#define LED_H

#include <stdint.h>

void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void LED_Set(uint8_t state);

#endif /* LED_DRIVER_H */