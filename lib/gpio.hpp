/**
 * @file    gpio.hpp 
 * @author	ZiTe <honmonoh@gmail.com>
 */

#ifndef GPIO_HPP_
#define GPIO_HPP_

extern "C"
{
#include "stm32f10x.h"
#include "stm32f103rb_gpio_mapping.h"
}

#define DEFAULT_GPIO_SPEED GPIO_Speed_10MHz

typedef enum
{
  LOW = 0,
  HIGH = !LOW
} GPIO_ValueTypeDef;

class GPIO
{
public:
  GPIO(GPIO_PortPinTypeDef port_Pin,
       GPIOMode_TypeDef mode,
       GPIOSpeed_TypeDef speed = DEFAULT_GPIO_SPEED);

  GPIO(GPIO_PortPinTypeDef port_Pin,
       GPIOMode_TypeDef mode,
       GPIO_ValueTypeDef init_Value,
       GPIOSpeed_TypeDef speed = DEFAULT_GPIO_SPEED);

  void Set(GPIO_ValueTypeDef value);
  void Set(uint8_t value);
  void Toggle(void);

  GPIO_ValueTypeDef Get(void);
  GPIO_ValueTypeDef Get_Input(void);
  GPIO_ValueTypeDef Get_Output(void);

  GPIO_TypeDef *Get_Port(void);
  uint16_t Get_Pin(void);

  bool Is_OutputPin(void);

private:
  GPIO_PortPinTypeDef _Port_Pin;
  GPIOMode_TypeDef _Mode;
  GPIOSpeed_TypeDef _Speed;

  void Init(void);
  GPIO_ValueTypeDef Convert_uint8_t_to_GPIO_Value_TypeDef(uint8_t value);
};

#endif /* GPIO_HPP_ */
