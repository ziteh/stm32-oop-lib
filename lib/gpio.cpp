/**
 * @file    gpio.cpp
 * @author  ZiTe <honmonoh@gmail.com>
 */

#include "gpio.hpp"

namespace F103RB
{
  GPIO::GPIO(GPIO_PortPinTypeDef port_Pin,
             GPIOMode_TypeDef mode,
             GPIOSpeed_TypeDef speed,
             bool immediatelyInit)
  {
    this->_Port_Pin = port_Pin;
    this->_Mode = mode;
    this->_Speed = speed;

    if (immediatelyInit)
      this->Init();
  }

  GPIO::GPIO(GPIO_PortPinTypeDef port_Pin,
             GPIOMode_TypeDef mode,
             GPIO_ValueTypeDef init_Value,
             GPIOSpeed_TypeDef speed,
             bool immediatelyInit)
  {
    this->_Port_Pin = port_Pin;
    this->_Mode = mode;
    this->_Speed = speed;

    if (immediatelyInit)
      this->Init();

    if (this->Is_OutputPin())
      this->Set(init_Value);
  }

  void GPIO::Init(void)
  {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = this->_Mode;
    GPIO_InitStructure.GPIO_Speed = this->_Speed;
    GPIO_InitStructure.GPIO_Pin = this->Get_Pin();
    GPIO_Init(this->Get_Port(), &GPIO_InitStructure);
  }

  void GPIO::Set(GPIO_ValueTypeDef value)
  {
    switch (value)
    {
    case LOW:
      (this->Get_Port())->BRR |= this->Get_Pin(); // Set value LOW
      break;

    case HIGH:
      (this->Get_Port())->BSRR |= this->Get_Pin(); // Set value HIGH
      break;
    }
  }

  void GPIO::Set(uint8_t value)
  {
    this->Set(this->Convert_uint8_t_to_GPIO_Value_TypeDef(value));
  }

  void GPIO::Toggle(void)
  {
    (this->Get_Port())->ODR ^= this->Get_Pin();
  }

  GPIO_ValueTypeDef GPIO::Get(void)
  {
    GPIO_ValueTypeDef value;

    if (this->Is_OutputPin())
    {
      value = this->Get_Output();
      // value = GPIO_ReadOutputDataBit(this->Get_Port(), this->Get_Pin());
    }
    else
    {
      value = this->Get_Input();
      // value = GPIO_ReadInputDataBit(this->Get_Port(), this->Get_Pin());
    }

    return value;
  }

  GPIO_ValueTypeDef GPIO::Get_Input(void)
  {
    GPIO_ValueTypeDef value;
    uint32_t IDR_value = (this->Get_Port())->IDR & this->Get_Pin();

    if (IDR_value == (uint32_t)Bit_RESET)
      value = LOW;
    else
      value = HIGH;

    return value;
  }

  GPIO_ValueTypeDef GPIO::Get_Output(void)
  {
    GPIO_ValueTypeDef value;
    uint32_t ODR_value = (this->Get_Port())->ODR & this->Get_Pin();

    if (ODR_value == (uint32_t)Bit_RESET)
      value = LOW;
    else
      value = HIGH;

    return value;
  }

  GPIO_TypeDef *GPIO::Get_Port(void)
  {
    uint8_t u8_Port_Pin = (uint8_t)this->_Port_Pin;

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

  uint16_t GPIO::Get_Pin(void)
  {
    uint8_t offset = 0;
    uint8_t u8_Port_Pin = (uint8_t)this->_Port_Pin;

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

  /**
 * @brief   Convert uint8_t to GPIO_Value_TypeDef.
 * @param   value: The value in uint8_t. This parameter should be 0 or 1.
 * @return  The converted GPIO_Value_TypeDef value.
 */
  GPIO_ValueTypeDef GPIO::Convert_uint8_t_to_GPIO_Value_TypeDef(uint8_t value)
  {
    GPIO_ValueTypeDef gpioValue;
    if (value == 0)
    {
      gpioValue = (GPIO_ValueTypeDef)LOW;
    }
    else
    {
      gpioValue = (GPIO_ValueTypeDef)HIGH;
    }
    return gpioValue;
  }

  bool GPIO::Is_OutputPin()
  {
    bool value;

    switch (this->_Mode)
    {
    case GPIO_Mode_Out_OD:
    case GPIO_Mode_Out_PP:
    case GPIO_Mode_AF_OD:
    case GPIO_Mode_AF_PP:
      // This GPIO is output.
      value = true;
      break;

    case GPIO_Mode_AIN:
    case GPIO_Mode_IN_FLOATING:
    case GPIO_Mode_IPD:
    case GPIO_Mode_IPU:
      // This GPIO is input.
      value = false;
      break;
    }

    return value;
  }
}