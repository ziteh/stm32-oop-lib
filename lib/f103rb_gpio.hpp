/**
 * @file    f103rb_gpio.hpp 
 * @author  ZiTe <honmonoh@gmail.com>
 */

#ifndef F103RB_GPIO_HPP_
#define F103RB_GPIO_HPP_

extern "C"
{
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
}
#include "f103rb_gpio_mapping.hpp"

#define DEFAULT_GPIO_SPEED (GPIO_Speed_10MHz)

typedef enum
{
  LOW = 0,
  HIGH = !LOW
} GPIO_ValueTypeDef;

namespace F103RB
{
  class GPIO
  {
  private:
    GPIO_PortPinTypeDef _Port_Pin;
    GPIOMode_TypeDef _Mode;
    GPIOSpeed_TypeDef _Speed;

    GPIO_ValueTypeDef Convert_uint8_t_to_GPIO_Value_TypeDef(uint8_t value);

  public:
    GPIO(GPIO_PortPinTypeDef port_Pin,
         GPIOMode_TypeDef mode,
         GPIOSpeed_TypeDef speed = DEFAULT_GPIO_SPEED,
         bool immediatelyInit = true);
    GPIO(GPIO_PortPinTypeDef port_Pin,
         GPIOMode_TypeDef mode,
         GPIO_ValueTypeDef init_Value,
         GPIOSpeed_TypeDef speed = DEFAULT_GPIO_SPEED,
         bool immediatelyInit = true);

    void Init(void);

    void Set(GPIO_ValueTypeDef value);
    void Set(uint8_t value);
    void Toggle(void);

    GPIO_ValueTypeDef Get(void);
    GPIO_ValueTypeDef Get_Input(void);
    GPIO_ValueTypeDef Get_Output(void);

    GPIO_TypeDef *Get_Port(void);
    uint16_t Get_Pin(void);

    bool Is_OutputPin(void);
  };
}

#endif /* F103RB_GPIO_HPP_ */
