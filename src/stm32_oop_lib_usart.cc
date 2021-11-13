/**
 * @file   stm32_oop_lib_usart.cc
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  This file is part of the 'STM32 OOP Library (stm32_oop_lib)' project.
 */

#include "stm32_oop_lib_usart.h"

namespace stm32_oop_lib
{
  USART::USART(uint32_t usart,
               GPIOPortPin tx,
               GPIOPortPin rx,
               uint32_t baud_rate)
  {
    this->usart_ = usart_;
    this->tx_ = tx;
    this->rx_ = rx;
    this->baud_rate_ = baud_rate;
  }

  void USART::Init()
  {
    /* Setup GPIO. */
    GPIO tx(this->tx_, OutputAltfnPushPull, true, Speed50MHz);
    GPIO rx(this->rx_, InputFloat);

    /* Setup USART. */
    usart_set_baudrate(this->usart_, this->baud_rate_);
    usart_set_databits(this->usart_, 8);
    usart_set_stopbits(this->usart_, USART_STOPBITS_1);
    usart_set_parity(this->usart_, USART_PARITY_NONE);
    ;
    usart_set_flow_control(this->usart_, USART_FLOWCONTROL_NONE);
    usart_set_mode(this->usart_, USART_MODE_TX_RX);

    /* Enable. */
    usart_enable_rx_interrupt(this->usart_);
    usart_enable(this->usart_);
  }

  void USART::Send(uint8_t *msg)
  {
    for (int i = 0; msg[i] != '\0'; i++)
    {
      usart_send_blocking(this->usart_, (uint16_t)msg[i]);
    }
  }

  void USART::Send(std::string msg)
  {
    for (int i = 0; msg[i] != '\0'; i++)
    {
      usart_send_blocking(this->usart_, (uint16_t)msg[i]);
    }
  }
}