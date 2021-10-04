/**
 * @file    pwm.cpp
 * @author  ZiTe <honmonoh@gmail.com>
 */

#include "f103rb_pwm.hpp"

namespace F103RB
{
  PWM::PWM(GPIO_PortPinTypeDef portPin,
           TIM_TypeDef *Timer,
           PWM_TimerChannelTypeDef channel)
  {
    this->_Port_Pin = portPin;
    this->_Timer = Timer;
    this->_Channel = channel;
  }

  void PWM::Init(RCC_ClocksTypeDef RCC_Clocks)
  {
    this->_RCC_Clocks = RCC_Clocks;
    GPIO gpio(this->_Port_Pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz, true);

    TIM_TimeBaseStructInit(&_TIM_TimeBaseStructure);
    _TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    _TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_OCStructInit(&_TIM_OCInitStructure);
    _TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    _TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    _TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    this->Init_Timer();
  }

  void PWM::Enable(void)
  {
    if (_TIM_OCInitStructure.TIM_OutputState != TIM_OutputState_Enable)
    {
      _TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
      this->Init_Timer();
    }

    // TIMx peripheral Preload register on CCRx
    switch (_Channel)
    {
    case CH1:
      TIM_OC1PreloadConfig(_Timer, TIM_OCPreload_Enable);
      break;
    case CH2:
      TIM_OC2PreloadConfig(_Timer, TIM_OCPreload_Enable);
      break;
    case CH3:
      TIM_OC3PreloadConfig(_Timer, TIM_OCPreload_Enable);
      break;
    case CH4:
      TIM_OC4PreloadConfig(_Timer, TIM_OCPreload_Enable);
      break;
    default:
      break;
    }
    TIM_ARRPreloadConfig(_Timer, ENABLE); // TIMx peripheral Preload register on ARR
    TIM_Cmd(_Timer, ENABLE);              // The specified TIM peripheral
  }

  void PWM::Disable(void)
  {
    if (_TIM_OCInitStructure.TIM_OutputState != TIM_OutputState_Disable)
    {
      _TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
      this->Init_Timer();
    }

    // TIMx peripheral Preload register on CCRx
    switch (_Channel)
    {
    case CH1:
      TIM_OC1PreloadConfig(_Timer, TIM_OCPreload_Disable);
      break;
    case CH2:
      TIM_OC2PreloadConfig(_Timer, TIM_OCPreload_Disable);
      break;
    case CH3:
      TIM_OC3PreloadConfig(_Timer, TIM_OCPreload_Disable);
      break;
    case CH4:
      TIM_OC4PreloadConfig(_Timer, TIM_OCPreload_Disable);
      break;
    default:
      break;
    }
    TIM_ARRPreloadConfig(_Timer, DISABLE); // TIMx peripheral Preload register on ARR
    TIM_Cmd(_Timer, DISABLE);              // The specified TIM peripheral
  }

  void PWM::Set_Frequency(uint16_t frequency)
  {
    /**
   *  TIM_Period = ((System_Frequency / TIM_Prescaler) / PWM_Frequency) - 1
   *
   *  The Maximum of System_Frequency is 72MHz (STM32F103RB)
   */

    _TIM_TimeBaseStructure.TIM_Prescaler = 100;
    _TIM_TimeBaseStructure.TIM_Period = ((this->_RCC_Clocks.SYSCLK_Frequency / _TIM_TimeBaseStructure.TIM_Prescaler) / frequency) - 1;

    this->Init_Timer();
  }

  void PWM::Set_DutyCycle(uint16_t dutyCycle)
  {
    if (_TIM_OCInitStructure.TIM_Pulse != this->ConvertDutyCycleToPulse(dutyCycle))
    {
      _TIM_OCInitStructure.TIM_Pulse = this->ConvertDutyCycleToPulse(dutyCycle);
      this->Init_Timer();
    }
  }

  uint16_t PWM::Get_Frequency(void)
  {
    /**
   * PWM_Frequency = (System_Frequency / TIM_Prescaler) / (TIM_Period + 1)
   *
   * The Maximum of System_Frequency = 72MHz (STM32F103RB)
   */

    return (this->_RCC_Clocks.SYSCLK_Frequency / _TIM_TimeBaseStructure.TIM_Prescaler) / (_TIM_TimeBaseStructure.TIM_Period + 1);
  }

  uint16_t PWM::Get_DutyCycle(void)
  {
    /**
   *  PWM_Duty Cycle % = (TIM_Pulse * 100%) / TIM_Period
   *
   *  TIM_Pulse = CCRx
   *  TIM_Period = ARR
   */

    switch (this->_Channel)
    {
    case CH1:
      return ((_Timer->CCR1) * 100.0) / (_Timer->ARR);
      break;
    case CH2:
      return ((_Timer->CCR2) * 100.0) / (_Timer->ARR);
      break;
    case CH3:
      return ((_Timer->CCR3) * 100.0) / (_Timer->ARR);
      break;
    case CH4:
      return ((_Timer->CCR4) * 100.0) / (_Timer->ARR);
      break;
    default:
      break;
    }
  }

  void PWM::Init_Timer(void)
  {
    TIM_TimeBaseInit(_Timer, &_TIM_TimeBaseStructure);
    switch (_Channel)
    {
    case CH1:
      TIM_OC1Init(_Timer, &_TIM_OCInitStructure);
      break;
    case CH2:
      TIM_OC2Init(_Timer, &_TIM_OCInitStructure);
      break;
    case CH3:
      TIM_OC3Init(_Timer, &_TIM_OCInitStructure);
      break;
    case CH4:
      TIM_OC4Init(_Timer, &_TIM_OCInitStructure);
      break;
    }
  }

  uint16_t PWM::ConvertDutyCycleToPulse(uint16_t dutyCycle)
  {
    /* TIM_Pulse = (PWM_Duty Cycle % * TIM_Period) / 100% */
    return (dutyCycle * (_TIM_TimeBaseStructure.TIM_Period) / 100.0);
  }
}
