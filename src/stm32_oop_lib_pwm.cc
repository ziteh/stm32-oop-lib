/**
 * @file   stm32_oop_lib_pwm.cc
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  This file is part of the 'STM32 OOP Library (stm32_oop_lib)' project.
 */

#include "stm32_oop_lib_pwm.h"

namespace stm32_oop_lib
{
  PWM::PWM(GPIOPortPin portPin,
           uint32_t timer,
           tim_oc_id channel,
           uint16_t duty_cycle)
  {
    this->port_pin_ = portPin;
    this->timer_ = timer;
    this->channel_ = channel;
    this->duty_cycle_ = duty_cycle;
  }

  void PWM::Init(bool immediately_enable = true)
  {
    GPIO pwm_gpio(this->port_pin_,
                  OutputAltfnPushPull,
                  true,
                  Speed50MHz);

    this->Disable();

    timer_set_mode(this->timer_,
                   TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_disable_preload(this->timer_);
    timer_continuous_mode(this->timer_);

    timer_set_period(this->timer_, this->period);
    timer_set_prescaler(this->timer_,
                        ((rcc_ahb_frequency / this->period) / 1000.0));

    timer_set_oc_mode(this->timer_, this->channel_, TIM_OCM_PWM1);
    timer_set_oc_value(this->timer_, this->channel_, this->GetPulse());

    if (immediately_enable)
    {
      this->Enable();
    }
  }

  void PWM::Enable(void)
  {
    timer_enable_oc_output(this->timer_, this->channel_);
    timer_enable_counter(this->timer_);
  }

  void PWM::Disable(void)
  {
    timer_disable_oc_output(this->timer_, this->channel_);
    timer_disable_counter(this->timer_);
  }

  /**
   * TODO
   */
  void PWM::SetFrequency(uint16_t value)
  {
    /**
   *  TIM_Period = ((System_Frequency / TIM_Prescaler) / PWM_Frequency) - 1
   *
   *  The Maximum of System_Frequency is 72MHz (STM32F103RB)
   */
    // float frequency = value + this->FrequencyOffset;

    // _TIM_TimeBaseStructure.TIM_Prescaler = 100;
    // _TIM_TimeBaseStructure.TIM_Period = ((this->_RCC_Clocks.SYSCLK_Frequency / _TIM_TimeBaseStructure.TIM_Prescaler) / frequency) - 1;

    // this->Init_Timer();
  }

  void PWM::SetDutyCycle(uint16_t duty_cycle)
  {
    this->duty_cycle_ = duty_cycle_;
    timer_set_oc_value(this->timer_, this->channel_, this->GetPulse());
  }

  /**
   * TODO
   */
  uint16_t PWM::GetFrequency(void)
  {
    /**
   * PWM_Frequency = (System_Frequency / TIM_Prescaler) / (TIM_Period + 1)
   *
   * The Maximum of System_Frequency = 72MHz (STM32F103RB)
   */

    // return (this->_RCC_Clocks.SYSCLK_Frequency / _TIM_TimeBaseStructure.TIM_Prescaler) / (_TIM_TimeBaseStructure.TIM_Period + 1);
  }

  /**
   * TODO
   */
  uint16_t PWM::GetDutyCycle(void)
  {
    /**
   *  PWM_Duty Cycle % = (TIM_Pulse * 100%) / TIM_Period
   *
   *  TIM_Pulse = CCRx
   *  TIM_Period = ARR
   */

    // switch (this->channel_)
    // {
    // case Channel1:
    //   return ((timer_->CCR1) * 100.0) / (timer_->ARR);
    //   break;
    // case CH2:
    //   return ((timer_->CCR2) * 100.0) / (timer_->ARR);
    //   break;
    // case CH3:
    //   return ((timer_->CCR3) * 100.0) / (timer_->ARR);
    //   break;
    // case CH4:
    //   return ((timer_->CCR4) * 100.0) / (timer_->ARR);
    //   break;
    // default:
    //   break;
    // }
  }

  uint32_t PWM::GetPulse()
  {
    /* TIM_Pulse = (PWM_Duty Cycle % * TIM_Period) / 100% */
    return (uint32_t)(this->period * (this->duty_cycle_ / 100.0));
  }
}
