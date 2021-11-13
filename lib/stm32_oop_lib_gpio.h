/**
 * @file    stm32_oop_lib_gpio.h 
 * @author  ZiTe <honmonoh@gmail.com>
 * @brief   This file is part of the 'STM32F1xx OOP Library' project.
 */

#ifndef STM32_OOP_LIB_GPIO_H_
#define STM32_OOP_LIB_GPIO_H_

extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}
#include "stm32_oop_lib_gpio_mapping.h"

namespace stm32_oop_lib
{
  typedef enum
  {
    Low = 0,
    High = !Low
  } GPIOValue;

  typedef enum
  {
    Speed2MHz = 2,
    Speed10MHz = 10,
    Speed50MHz = 50
  } GPIOSpeed;

  typedef enum
  {
    InputFloat = 0,
    InputAnalog,
    InputPullUp,
    InputPullDown,

    OutputOpenDrain,
    OutputPushPull,
    OutputAltfnOpenDrain,
    OutputAltfnPushPull
  } GPIOMode;

  class GPIO
  {
  private:
    uint8_t port_;
    uint8_t pin_;
    GPIOMode mode_;
    GPIOSpeed speed_;

    GPIOValue ConvertUint8tToGPIOValue(uint8_t value);
    uint32_t ParsePort(GPIOPortPin port_pin);
    uint16_t ParsePin(GPIOPortPin port_pin);

  public:
    GPIO(GPIOPortPin port_pin,
         GPIOMode mode,
         bool immediately_init = true,
         GPIOSpeed speed = Speed2MHz);

    GPIO(GPIOPortPin port_pin,
         GPIOMode mode,
         GPIOValue init_value,
         GPIOSpeed speed = Speed2MHz);

    void Init(void);

    void Set(GPIOValue value);
    void Set(uint8_t value);
    void Toggle(void);

    GPIOValue Get(void);
    GPIOValue GetInput(void);
    GPIOValue GetOutput(void);

    uint32_t GetPort(void);
    uint16_t GetPin(void);

    bool IsOutput(void);
  };
}

#endif /* STM32_OOP_LIB_GPIO_H_ */
