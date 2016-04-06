/*
 * Plantilla
*/

#include "chip.h"
#include <cr_section_macros.h>

#define TICKBASE_HZ (1000)
#define PERIOD      (1000)
#define TICKS_PER_SECOND (1000)

struct seconds_clock_state {
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

void secondsClockUpdate(struct seconds_clock_state * state)
{
  if(state->ticks >= TICKS_PER_SECOND) {
    state->seconds++;
    state->ticks = 0;
  }
  else {
    state->ticks++;
  }
}

int main(void) {
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/TICKBASE_HZ);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 22);

  unsigned int led_time = 0;
  struct seconds_clock_state seconds_clock = {.seconds = 0, .ticks = 0};

  while(1) {
    if(systick) {
      systick = 0;

      handleLedCycle(&led_time);
      secondsClockUpdate(&seconds_clock);

      // Every 5s
      if(seconds_clock.seconds == 5) {
        // Increase led toggle frequency
        seconds_clock.seconds++;
      }
    }
  }

  return 0;
}
