/**
 * @file    f103rb_pwm.hpp
 * @author  ZiTe <honmonoh@gmail.com>
 */

#ifndef F103RB_PWM_HPP_
#define F103RB_PWM_HPP_

extern "C"
{
#include "stm32f10x.h"
}
#include "f103rb_gpio.hpp"

namespace F103RB
{
  typedef enum
  {
    CH1 = 1,
    CH2,
    CH3,
    CH4
  } PWM_TimerChannelTypeDef;

  class PWM
  {
  private:
    GPIO_PortPinTypeDef _Port_Pin;
    TIM_TypeDef *_Timer;
    PWM_TimerChannelTypeDef _Channel;
    TIM_TimeBaseInitTypeDef _TIM_TimeBaseStructure;
    TIM_OCInitTypeDef _TIM_OCInitStructure;
    RCC_ClocksTypeDef _RCC_Clocks;

    void Init_Timer(void);
    uint16_t ConvertDutyCycleToPulse(uint16_t dutyCycle);

  public:
    PWM(GPIO_PortPinTypeDef portPin,
        TIM_TypeDef *Timer,
        PWM_TimerChannelTypeDef channel);
    void Init(RCC_ClocksTypeDef RCC_Clocks);

    void Enable(void);
    void Disable(void);

    void Set_Frequency(uint16_t frequency);
    void Set_DutyCycle(uint16_t dutyCycle);

    uint16_t Get_Frequency(void);
    uint16_t Get_DutyCycle(void);
  };
}

#endif /* F103RB_PWM_HPP_ */
