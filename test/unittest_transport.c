/**
 * @file   unittest_transport.c
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  The custom unit test transport source code file for PlatformIO.
 * @remark Reference: https://docs.platformio.org/en/latest/tutorials/ststm32/stm32cube_debugging_unit_testing.html#writing-unit-tests
 */

#include "unittest_transport.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>

#define USART_BAUD_RATE (115200)

void unittest_uart_begin(void)
{
  /* Setup clock. */
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_USART2);

  /* Setup Tx. */
  gpio_set_mode(GPIO_BANK_USART2_TX,
                GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                GPIO_USART2_TX);

  /* Setup Rx. */
  gpio_set_mode(GPIO_BANK_USART2_RX,
                GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_FLOAT,
                GPIO_USART2_RX);

  /* Setup USART. */
  usart_set_baudrate(USART2, USART_BAUD_RATE);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART2, USART_MODE_TX_RX);

  usart_enable(USART2);
}

void unittest_uart_putchar(char c)
{
  usart_send_blocking(USART2, (uint16_t)c);
}

void unittest_uart_flush(void) {}

void unittest_uart_end(void)
{
  usart_disable(USART2);

  rcc_periph_clock_disable(RCC_USART2);
  rcc_periph_clock_disable(RCC_GPIOA);
}
