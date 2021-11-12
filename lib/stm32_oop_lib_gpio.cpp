/**
 * @file    stm32_oop_lib_gpio.cpp
 * @author  ZiTe <honmonoh@gmail.com>
 * @brief   This file is part of the 'STM32F1xx OOP Library' project.
 */

#include "stm32_oop_lib_gpio.hpp"

namespace F103RB
{
  GPIO::GPIO(GPIO_PortPinTypeDef port_pin,
             gpio_mode mode,
             bool immediately_init,
             gpio_speed speed)
  {
    this->_mode = mode;
    this->_speed = speed;
    this->_port = this->parse_port(port_pin);
    this->_pin = this->parse_pin(port_pin);

    if (immediately_init)
    {
      this->init();
    }
  }

  GPIO::GPIO(GPIO_PortPinTypeDef port_pin,
             gpio_mode mode,
             gpio_value init_value,
             gpio_speed speed)
  {
    this->_mode = mode;
    this->_speed = speed;
    this->_port = this->parse_port(port_pin);
    this->_pin = this->parse_pin(port_pin);

    if (this->is_output())
    {
      this->init();
      this->set(init_value);
    }
  }

  void GPIO::init(void)
  {
    uint8_t mode;
    uint8_t cnf;

    switch (this->_speed)
    {
    case speed_10mhz:
      mode = GPIO_MODE_OUTPUT_10_MHZ;
      break;

    case speed_50mhz:
      mode = GPIO_MODE_OUTPUT_50_MHZ;
      break;

    case speed_2mhz:
    default:
      mode = GPIO_MODE_OUTPUT_2_MHZ;
      break;
    }

    switch (this->_mode)
    {
    case output_open_drain:
      cnf = GPIO_CNF_OUTPUT_OPENDRAIN;
      break;

    case output_push_pull:
      cnf = GPIO_CNF_OUTPUT_PUSHPULL;
      break;

    case output_altfn_open_drain:
      cnf = GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN;
      break;

    case output_altfn_push_pull:
      cnf = GPIO_CNF_OUTPUT_ALTFN_PUSHPULL;
      break;

    case input_pull_down:
    case input_pull_up:
      cnf = GPIO_CNF_INPUT_PULL_UPDOWN;
      mode = GPIO_MODE_INPUT;
      break;

    case input_analog:
      cnf = GPIO_CNF_INPUT_ANALOG;
      mode = GPIO_MODE_INPUT;
      break;

    case input_float:
    default:
      cnf = GPIO_CNF_INPUT_FLOAT;
      mode = GPIO_MODE_INPUT;
      break;
    }

    gpio_set_mode(this->get_port(),
                  mode,
                  cnf,
                  this->get_pin());
  }

  void GPIO::set(gpio_value value)
  {
    switch (value)
    {
    case low:
      GPIO_BRR(this->get_port()) |= this->get_pin();
      break;

    case high:
      GPIO_BSRR(this->get_port()) |= this->get_pin();
      break;
    }
  }

  void GPIO::set(uint8_t value)
  {
    this->set(this->convert_uint8_t_to_gpio_value(value));
  }

  void GPIO::toggle(void)
  {
    GPIO_ODR(this->get_port()) ^= this->get_pin();
  }

  gpio_value GPIO::get(void)
  {
    gpio_value value;

    if (this->is_output())
    {
      value = this->get_output();
    }
    else
    {
      value = this->get_input();
    }

    return value;
  }

  gpio_value GPIO::get_input(void)
  {
    gpio_value value;
    uint32_t IDR_value = GPIO_IDR(this->get_port()) & this->get_pin();

    if (IDR_value == 0)
    {
      value = low;
    }
    else
    {
      value = high;
    }

    return value;
  }

  gpio_value GPIO::get_output(void)
  {
    gpio_value value;
    uint32_t ODR_value = GPIO_ODR(this->get_port()) & this->get_pin();

    if (ODR_value == 0)
    {
      value = low;
    }
    else
    {
      value = high;
    }

    return value;
  }

  uint32_t GPIO::get_port(void)
  {
    return this->_port;
  }

  uint16_t GPIO::get_pin(void)
  {
    return this->_pin;
  }

  gpio_value GPIO::convert_uint8_t_to_gpio_value(uint8_t value)
  {
    if (value == 0)
    {
      return (gpio_value)low;
    }
    else
    {
      return (gpio_value)high;
    }
  }

  bool GPIO::is_output()
  {
    bool value;

    switch (this->_mode)
    {
    case output_open_drain:
    case output_push_pull:
    case output_altfn_open_drain:
    case output_altfn_push_pull:
      // This GPIO is output.
      value = true;
      break;

    case input_analog:
    case input_float:
    case input_pull_down:
    case input_pull_up:
      // This GPIO is input.
      value = false;
      break;
    }

    return value;
  }

  uint32_t parse_port(GPIO_PortPinTypeDef port_pin)
  {
    uint8_t u8_Port_Pin = (uint8_t)port_pin;

    if (u8_Port_Pin <= (uint8_t)PA15) // Port-A:  0~15
      return GPIOA;
    else if (u8_Port_Pin <= (uint8_t)PB15) // Port-B: 16~31
      return GPIOB;
    else if (u8_Port_Pin <= (uint8_t)PC15) // Port-C: 32~47
      return GPIOC;
    else if (u8_Port_Pin <= (uint8_t)PD15) // Port-D: 48~63
      return GPIOD;
    else if (u8_Port_Pin <= (uint8_t)PE15) // Port-E: 64~79
      return GPIOE;
  }

  uint16_t parse_pin(GPIO_PortPinTypeDef port_pin)
  {
    uint8_t offset = 0;
    uint8_t u8_Port_Pin = (uint8_t)port_pin;

    if (u8_Port_Pin <= (uint8_t)PA15) // Port-A:  0~15
      offset = (uint8_t)PA0;
    else if (u8_Port_Pin <= (uint8_t)PB15) // Port-B: 16~31
      offset = (uint8_t)PB0;
    else if (u8_Port_Pin <= (uint8_t)PC15) // Port-C: 32~47
      offset = (uint8_t)PC0;
    else if (u8_Port_Pin <= (uint8_t)PD15) // Port-D: 48~63
      offset = (uint8_t)PD0;
    else if (u8_Port_Pin <= (uint8_t)PE15) // Port-E: 64~79
      offset = (uint8_t)PE0;

    return (uint16_t)(0x0001 << (u8_Port_Pin - offset));
  }
}