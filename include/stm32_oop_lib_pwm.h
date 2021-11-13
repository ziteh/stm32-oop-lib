/**
 * @file   stm32_oop_lib_pwm.h
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  This file is part of the 'STM32 OOP Library (stm32_oop_lib)' project.
 */

#ifndef STM32_OOP_LIB_PWM_H_
#define STM32_OOP_LIB_PWM_H_

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include "stm32_oop_lib_gpio.h"

namespace stm32_oop_lib
{
  class PWM
  {
  private:
    GPIOPortPin port_pin_;
    uint32_t timer_;
    tim_oc_id channel_;
    uint16_t duty_cycle_;

    uint32_t GetPulse(void);

  public:
    // float FrequencyOffset = 0.0;
    // float DutyCycleOffset = 0.0;
    uint32_t period = 480000;

    PWM(GPIOPortPin portPin,
        uint32_t timer,
        tim_oc_id channel,
        uint16_t duty_cycle = 50.0);
    void Init(bool immediately_enable = true);

    void Enable(void);
    void Disable(void);

    void SetFrequency(uint16_t frequency);
    void SetDutyCycle(uint16_t duty_cycle);

    uint16_t GetFrequency(void);
    uint16_t GetDutyCycle(void);
  };
}

#endif /* STM32_OOP_LIB_PWM_H_ */
