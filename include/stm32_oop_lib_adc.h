/**
 * @file   stm32_oop_lib_adc.h
 * @author ZiTe (honmonoh@gmail.com)
 * @brief  This file is part of the 'STM32 OOP Library (stm32_oop_lib)' project.
 */

#ifndef STM32_OOP_LIB_ADC_H_
#define STM32_OOP_LIB_ADC_H_

#include "stm32_oop_lib_gpio.h"

#define DEFAULT_ADC_RANK (1)
#define DEFAULT_ADC_SAMPLE_TIME (ADC_SampleTime_55Cycles5)
#define DEFAULT_ADC_CLK_CONFIG (RCC_PCLK2_Div6)

namespace stm32_oop_lib
{
  class ADC
  {
  private:
    GPIO_PortPinTypeDef _PortPin;
    ADC_TypeDef *_ADCx;
    uint8_t _ADC_Channel;

  public:
    ADC(GPIO_PortPinTypeDef port_pin,
        ADC_TypeDef *ADC,
        uint8_t ADC_Channel);

    void Init(void);
    void Enable(void);
    void Disable(void);

    /**
     * @brief Get the converted value of ADC.
     * @param Rank: The rank in the regular group sequencer. This parameter must be between 1 to 16.
     * @param SampleTime: The sample time value to be set for the selected channel.
     * @return Converted value.
     */
    uint16_t Get_Value(uint8_t Rank = DEFAULT_ADC_RANK,
                       uint8_t SampleTime = DEFAULT_ADC_SAMPLE_TIME);
  };
}

#endif /* STM32_OOP_LIB_ADC_H_ */
