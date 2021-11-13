/**
 * @file    f103rb_usart.hpp
 * @author  ZiTe <honmonoh@gmail.com>
 */

#ifndef F103RB_USART_HPP_
#define F103RB_USART_HPP_

extern "C"
{
#include "stm32f10x.h"
}
#include <string>
#include "f103rb_gpio.hpp"

namespace F103RB
{
  class USART
  {
  private:
    uint32_t _BaudRate;

  public:
    USART(uint32_t baud_Rate = 9600);
    void Init();
    void Send(uint8_t *data);
    void Send(std::string data);
  };
}

#endif /* F103RB_USART_HPP_ */
