/**
 * @file    f103rb_adc.hpp 
 * @author  ZiTe <honmonoh@gmail.com>
 */

#ifndef F103RB_ADC_HPP_
#define F103RB_ADC_HPP_

extern "C"
{
#include "stm32f10x.h"
}
#include "f103rb_gpio.hpp"

#define DEFAULT_ADC_RANK (1)
#define DEFAULT_ADC_SAMPLE_TIME (ADC_SampleTime_55Cycles5)
#define DEFAULT_ADC_CLK_CONFIG (RCC_PCLK2_Div6)

namespace F103RB
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

#endif /* F103RB_ADC_HPP_ */
