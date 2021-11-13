/**
 * @file    stm32_oop_lib_gpio.cc
 * @author  ZiTe <honmonoh@gmail.com>
 * @brief   This file is part of the 'STM32F1xx OOP Library' project.
 */

#include "stm32_oop_lib_gpio.h"

namespace stm32_oop_lib
{
  GPIO::GPIO(GPIOPortPin port_pin,
             GPIOMode mode,
             bool immediately_init,
             GPIOSpeed speed)
  {
    this->mode_ = mode;
    this->speed_ = speed;
    this->port_ = this->ParsePort(port_pin);
    this->pin_ = this->ParsePin(port_pin);

    if (immediately_init)
    {
      this->Init();
    }
  }

  GPIO::GPIO(GPIOPortPin port_pin,
             GPIOMode mode,
             GPIOValue init_value,
             GPIOSpeed speed)
  {
    this->mode_ = mode;
    this->speed_ = speed;
    this->port_ = this->ParsePort(port_pin);
    this->pin_ = this->ParsePin(port_pin);

    if (this->IsOutput())
    {
      this->Init();
      this->Set(init_value);
    }
  }

  void GPIO::Init(void)
  {
    uint8_t mode;
    uint8_t cnf;

    switch (this->speed_)
    {
    case Speed10MHz:
      mode = GPIO_MODE_OUTPUT_10_MHZ;
      break;

    case Speed50MHz:
      mode = GPIO_MODE_OUTPUT_50_MHZ;
      break;

    case Speed2MHz:
    default:
      mode = GPIO_MODE_OUTPUT_2_MHZ;
      break;
    }

    switch (this->mode_)
    {
    case OutputOpenDrain:
      cnf = GPIO_CNF_OUTPUT_OPENDRAIN;
      break;

    case OutputPushPull:
      cnf = GPIO_CNF_OUTPUT_PUSHPULL;
      break;

    case OutputAltfnOpenDrain:
      cnf = GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN;
      break;

    case OutputAltfnPushPull:
      cnf = GPIO_CNF_OUTPUT_ALTFN_PUSHPULL;
      break;

    case InputPullDown:
    case InputPullUp:
      cnf = GPIO_CNF_INPUT_PULL_UPDOWN;
      mode = GPIO_MODE_INPUT;
      break;

    case InputAnalog:
      cnf = GPIO_CNF_INPUT_ANALOG;
      mode = GPIO_MODE_INPUT;
      break;

    case InputFloat:
    default:
      cnf = GPIO_CNF_INPUT_FLOAT;
      mode = GPIO_MODE_INPUT;
      break;
    }

    gpio_set_mode(this->GetPort(),
                  mode,
                  cnf,
                  this->GetPin());
  }

  void GPIO::Set(GPIOValue value)
  {
    switch (value)
    {
    case Low:
      GPIO_BRR(this->GetPort()) |= this->GetPin();
      break;

    case High:
      GPIO_BSRR(this->GetPort()) |= this->GetPin();
      break;
    }
  }

  void GPIO::Set(uint8_t value)
  {
    this->Set(this->ConvertUint8tToGPIOValue(value));
  }

  void GPIO::Toggle(void)
  {
    GPIO_ODR(this->GetPort()) ^= this->GetPin();
  }

  GPIOValue GPIO::Get(void)
  {
    GPIOValue value;

    if (this->IsOutput())
    {
      value = this->GetOutput();
    }
    else
    {
      value = this->GetInput();
    }

    return value;
  }

  GPIOValue GPIO::GetInput(void)
  {
    GPIOValue value;
    uint32_t idr_value = GPIO_IDR(this->GetPort()) & this->GetPin();

    if (idr_value == 0)
    {
      value = Low;
    }
    else
    {
      value = High;
    }

    return value;
  }

  GPIOValue GPIO::GetOutput(void)
  {
    GPIOValue value;
    uint32_t odr_value = GPIO_ODR(this->GetPort()) & this->GetPin();

    if (odr_value == 0)
    {
      value = Low;
    }
    else
    {
      value = High;
    }

    return value;
  }

  uint32_t GPIO::GetPort(void)
  {
    return this->port_;
  }

  uint16_t GPIO::GetPin(void)
  {
    return this->pin_;
  }

  GPIOValue GPIO::ConvertUint8tToGPIOValue(uint8_t value)
  {
    if (value == 0)
    {
      return (GPIOValue)Low;
    }
    else
    {
      return (GPIOValue)High;
    }
  }

  bool GPIO::IsOutput()
  {
    bool is_output;

    switch (this->mode_)
    {
    case OutputOpenDrain:
    case OutputPushPull:
    case OutputAltfnOpenDrain:
    case OutputAltfnPushPull:
      // This GPIO is output.
      is_output = true;
      break;

    case InputAnalog:
    case InputFloat:
    case InputPullDown:
    case InputPullUp:
      // This GPIO is input.
      is_output = false;
      break;
    }

    return is_output;
  }

  uint32_t ParsePort(GPIOPortPin port_pin)
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

  uint16_t ParsePin(GPIOPortPin port_pin)
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