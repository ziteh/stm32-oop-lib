
#include <libopencm3/stm32/rcc.h>
#include <stm32_oop_lib_gpio.h>

void delay(int value);

int main(void)
{
  rcc_periph_clock_enable(RCC_GPIOA);

  stm32_oop_lib::GPIO led(stm32_oop_lib::UserLED,
                          stm32_oop_lib::OutputPushPull);

  while (1)
  {
    led.Toggle();
    delay(200000);
  }

  return 0;
}

void delay(int value)
{
  while (value--)
  {
    __asm__("nop");
  }
}
