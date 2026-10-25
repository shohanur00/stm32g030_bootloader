#include "systemclock.h"
#include "stm32g030xx.h"


#define SYSTEMCLOCK_FREQ_HZ    16000000UL


void SystemClock_Init(void)
{

    
}


uint32_t SystemClock_GetFrequency(void)
{
    return SYSTEMCLOCK_FREQ_HZ;
}