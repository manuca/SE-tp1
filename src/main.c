/*
 * Plantilla
*/

#include "chip.h"
#include <cr_section_macros.h>

#define TICKBASE_HZ      (1000)
#define PERIOD           (1000)
#define TICKS_PER_SECOND (1000)

struct seconds_clock {
  unsigned int seconds;
  unsigned int ticks;
};

int systick = 0;

void SysTick_Handler(void)
{
	systick = 1;
}

void toggleLed()
{
  Chip_GPIO_SetPinToggle(LPC_GPIO, 0, 22);
}

unsigned int handleLedCycle(unsigned int * time)
{
  if(*time >= (PERIOD / 2)) {
    *time = 0;
    toggleLed();
  }
  else {
    *time++;
  }

  return time;
}

void secondsClockUpdate(struct seconds_clock * clock)
{
  if(clock->ticks >= TICKS_PER_SECOND) {
    clock->seconds++;
    clock->ticks = 0;
  }
  else {
    clock->ticks++;
  }
}

int main(void) {
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/TICKBASE_HZ);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 22);

  unsigned int led_time = 0;
  struct seconds_clock clock = {.seconds = 0, .ticks = 0};

  while(1) {
    if(systick) {
      systick = 0;

      handleLedCycle(&led_time);
      secondsClockUpdate(&clock);

      // Every 5s
      if((clock.seconds != 0) && (clock.seconds % 5 == 0)) {
        // Increase led toggle frequency
      }
    }
  }

  return 0;
}
