/**
 * @file    f103rb_gpio.hpp 
 * @author  ZiTe <honmonoh@gmail.com>
 * @brief   This file is part of the 'STM32F1xx OOP Library' project.
 */

#ifndef F103RB_GPIO_HPP_
#define F103RB_GPIO_HPP_

extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}
#include "f103rb_gpio_mapping.hpp"

#define DEFAULT_GPIO_SPEED (speed_2mhz)

namespace F103RB
{
  typedef enum
  {
    low = 0,
    high = !low
  } gpio_value;

  typedef enum
  {
    speed_2mhz = 2,
    speed_10mhz = 10,
    speed_50mhz = 50
  } gpio_speed;

  typedef enum
  {
    input_float = 0,
    input_analog,
    input_pull_up,
    input_pull_down,
    output_open_drain,
    output_push_pull,
    output_altfn_open_drain,
    output_altfn_push_pull
  } gpio_mode;

  class GPIO
  {
  private:
    uint8_t _port;
    uint8_t _pin;
    gpio_mode _mode;
    gpio_speed _speed;

    gpio_value convert_uint8_t_to_gpio_value(uint8_t value);
    uint32_t parse_port(GPIO_PortPinTypeDef port_pin);
    uint16_t parse_pin(GPIO_PortPinTypeDef port_pin);

  public:
    GPIO(GPIO_PortPinTypeDef port_pin,
         gpio_mode mode,
         bool immediately_init = true,
         gpio_speed speed = DEFAULT_GPIO_SPEED);
    GPIO(GPIO_PortPinTypeDef port_pin,
         gpio_mode mode,
         gpio_value init_value,
         gpio_speed speed = DEFAULT_GPIO_SPEED);

    void init(void);

    void set(gpio_value value);
    void set(uint8_t value);
    void toggle(void);

    gpio_value get(void);
    gpio_value get_input(void);
    gpio_value get_output(void);

    uint32_t get_port(void);
    uint16_t get_pin(void);

    bool is_output(void);
  };
}

#undef DEFAULT_GPIO_SPEED

#endif /* F103RB_GPIO_HPP_ */
