/**
 * @file   stm32_oop_lib_usart.h
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  This file is part of the 'STM32 OOP Library (stm32_oop_lib)' project.
 */

#ifndef STM32_OOP_LIB_USART_H_
#define STM32_OOP_LIB_USART_H_

#include <string>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>
#include "stm32_oop_lib_gpio.h"

namespace stm32_oop_lib
{
  class USART
  {
  private:
    uint32_t baud_rate_;
    uint32_t usart_;
    GPIOPortPin tx_;
    GPIOPortPin rx_;

  public:
    USART(uint32_t usart,
          GPIOPortPin tx,
          GPIOPortPin rx,
          uint32_t baud_rate = 9600);
    void Init();
    void Send(uint8_t *msg);
    void Send(std::string msg);
  };
}

#endif /* STM32_OOP_LIB_USART_H_ */
