/**
 * @file   test_main.cc
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  The unit test case for PlatformIO.
 */

#include <libopencm3/stm32/rcc.h>
#include <stm32_oop_lib_gpio.h>
#include <unity.h>

stm32_oop_lib::GPIO led(stm32_oop_lib::UserLED,
                        stm32_oop_lib::OutputPushPull,
                        false,
                        stm32_oop_lib::Speed2MHz);

void delay(int value);
void test_setup(void);
void testcase_led_output_low(void);
void testcase_led_output_high(void);

int main(void)
{
  test_setup();
  delay(100000);

  UNITY_BEGIN();

  RUN_TEST(testcase_led_output_low);
  delay(100000);
  RUN_TEST(testcase_led_output_high);
  delay(100000);

  UNITY_END();

  /* Halt. */
  while (1)
  {
    __asm__("nop");
  }

  return 0;
}

void test_setup(void)
{
  rcc_clock_setup_in_hsi_out_48mhz();
  rcc_periph_clock_enable(RCC_GPIOA);

  led.Init();
}

void testcase_led_output_low(void)
{
  led.Set(stm32_oop_lib::Low);
  TEST_ASSERT_EQUAL(stm32_oop_lib::Low, led.Get());
}

void testcase_led_output_high(void)
{
  led.Set(stm32_oop_lib::High);
  TEST_ASSERT_EQUAL(stm32_oop_lib::High, led.Get());
}

void delay(int value)
{
  while (value--)
  {
    __asm__("nop");
  }
}
