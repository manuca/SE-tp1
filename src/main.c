/*
 * Plantilla
*/

#include "chip.h"
#include <cr_section_macros.h>

#define TICKBASE_HZ (1000)
#define PERIOD      (1000)

int systick = 0;

void SysTick_Handler(void)
{
	systick = 1;
}

void toggleLed()
{
  Chip_GPIO_SetPinToggle(LPC_GPIO, 0, 22);
}

int main(void) {
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/TICKBASE_HZ);

    Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 22);

    unsigned int time = 0;

    while(1) {
      if(systick) {
        systick = 0;

        if(time >= (PERIOD / 2)) {
          time = 0;
          toggleLed();
        }
        else {
          time++;
        }

    	}
    }

    return 0;
}
