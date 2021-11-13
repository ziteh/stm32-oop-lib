/**
 * @file   stm32_oop_lib_adc.cc
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  This file is part of the 'STM32 OOP Library (stm32_oop_lib)' project.
 */

#include "stm32_oop_lib_adc.h"

namespace stm32_oop_lib
{
  ADC::ADC(uint32_t adc,
           uint8_t adc_channel,
           GPIOPortPin port_pin)
  {
    this->adc_ = adc;
    this->adc_channel_ = adc_channel;
    this->port_pin_ = port_pin;
  }

  void ADC::Init(void)
  {
    GPIO adc_gpio(this->port_pin_, InputAnalog, false);
    adc_gpio.Init();

    this->Disable();

    adc_disable_scan_mode(this->adc_);
    adc_disable_external_trigger_regular(this->adc_);

    adc_set_single_conversion_mode(this->adc_);
    adc_set_right_aligned(this->adc_);
    adc_set_sample_time(this->adc_,
                        this->adc_channel_, DEFAULT_ADC_SAMPLE_TIME);
  }

  void ADC::Enable(void)
  {
    adc_power_on(this->adc_);

    /* Wait a bit. */
    for (int i = 0; i < 800000; i++)
    {
      __asm__("nop");
    }

    adc_reset_calibration(this->adc_);
    adc_calibrate(this->adc_);

    uint8_t ch[16];
    ch[0] = this->adc_channel_;
    adc_set_regular_sequence(this->adc_, 1, ch);
  }

  void ADC::Disable(void)
  {
    adc_power_on(this->adc_);
  }

  void ADC::StartConversiion(void)
  {
    adc_start_conversion_direct(this->adc_);
  }

  uint16_t ADC::GetValue(void)
  {
    /* Wati for ADC convert complete. */
    while (!adc_get_flag(this->adc_, ADC_SR_EOC))
    {
      __asm__("nop");
    }

    return (uint16_t)ADC_DR(this->adc_);
  }
}