/**
 * @file   stm32_oop_lib_adc.h
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  This file is part of the 'STM32 OOP Library (stm32_oop_lib)' project.
 */

#ifndef STM32_OOP_LIB_ADC_H_
#define STM32_OOP_LIB_ADC_H_

#include <libopencm3/stm32/adc.h>
#include "stm32_oop_lib_gpio.h"

#define DEFAULT_ADC_SAMPLE_TIME (ADC_SMPR_SMP_55DOT5CYC)

namespace stm32_oop_lib
{
  class ADC
  {
  private:
    GPIOPortPin port_pin_;
    uint32_t adc_;
    uint8_t adc_channel_;

  public:
    ADC(uint32_t adc,
        uint8_t adc_channel,
        GPIOPortPin port_pin);

    void Init(void);
    void Enable(void);
    void Disable(void);
    void StartConversiion(void);

    uint16_t GetValue(void);
  };
}

#endif /* STM32_OOP_LIB_ADC_H_ */
