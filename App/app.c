/* Application module implementation. */
#include "app.h"
#include "stm32g030xx.h"
#include "bootloader_app.h"

void App_Setup(void)
{
      Bootloader_App_Setup();
}


void App_Loop(void)
{   
      Bootloader_App_Loop();
}