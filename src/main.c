/*
 * Plantilla
*/

#include "chip.h"
#include <cr_section_macros.h>

#define TICKBASE_HZ      (1000)
#define TICKS_PER_SECOND (1000)

// Target 10Hz
#define LED_MIN_PERIOD   (100)
// Period decreased in LED_PERIOD_DELTA ticks every time
#define LED_PERIOD_DELTA (100)

struct seconds_clock {
  unsigned int seconds;
  unsigned int ticks;
};

struct led_cycle_state {
  unsigned int ticks;
  bool toggle;
  unsigned int ticks_per_period;
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

void ledCycleUpdate(struct led_cycle_state * state)
{
  if(state->ticks >= (state->ticks_per_period / 2)) {
    state->ticks  = 0;
    state->toggle = TRUE;
  }
  else {
    state->ticks++;
  }
}

void ledCycleUpdateLedState(struct led_cycle_state * state)
{
  if(state->toggle) {
    toggleLed();
    state->toggle = FALSE;
  }
}

void secondsClockReset(struct seconds_clock * clock)
{
  clock->seconds = 0;
  clock->ticks   = 0;
}

void secondsClockTickUpdate(struct seconds_clock * clock)
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

  struct seconds_clock clock       = {.seconds = 0, .ticks = 0};
  struct led_cycle_state led_state = {.ticks = 0, .toggle = FALSE, .ticks_per_period = TICKS_PER_SECOND};

  secondsClockReset(&clock);

  while(1) {
    if(systick) {
      systick = 0;

      ledCycleUpdate(&led_state);
      secondsClockTickUpdate(&clock);

      // Every 5s
      if((clock.seconds != 0) && (clock.seconds % 5 == 0)) {
        secondsClockReset(&clock);
        // Increase led toggle frequency
        led_state.ticks_per_period -= LED_PERIOD_DELTA; 
        
        if(led_state.ticks_per_period < LED_MIN_PERIOD) {
          led_state.ticks_per_period = LED_MIN_PERIOD;
        }
      }

      ledCycleUpdateLedState(&led_state);
    }
  }

  return 0;
}
