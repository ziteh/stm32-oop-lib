/**
 * @file    f103rb_adc.cpp 
 * @author  ZiTe <honmonoh@gmail.com>
 */

#include "f103rb_adc.hpp"

namespace stm32_oop_lib
{
  ADC::ADC(GPIO_PortPinTypeDef port_pin,
           ADC_TypeDef *ADC,
           uint8_t ADC_Channel)
  {
    // The clock of ADC con't over than 14MHz.
    RCC_ADCCLKConfig(DEFAULT_ADC_CLK_CONFIG);

    this->_PortPin = port_pin;
    this->_ADCx = ADC;
    this->_ADC_Channel = ADC_Channel;
  }

  void ADC::Init(void)
  {
    GPIO adc_pin(this->_PortPin, GPIO_Mode_AIN);
    adc_pin.Init();

    ADC_DeInit(this->_ADCx);

    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_Init(this->_ADCx, &ADC_InitStruct);

    this->Disable();
  }

  void ADC::Enable(void)
  {
    ADC_Cmd(this->_ADCx, ENABLE);

    /* ADC Calibration */
    // Reset calibration
    ADC_ResetCalibration(this->_ADCx);

    // Wait until reset calibration complete
    while (ADC_GetResetCalibrationStatus(this->_ADCx) == 1)
    {
      /* Null */
    }

    // Start calibration
    ADC_StartCalibration(this->_ADCx);

    // Wait until calibration complete
    while (ADC_GetCalibrationStatus(this->_ADCx) == 1)
    {
      /* Null */
    }
  }

  void ADC::Disable(void)
  {
    ADC_Cmd(_ADCx, DISABLE);
  }

  uint16_t ADC::Get_Value(uint8_t Rank, uint8_t SampleTime)
  {
    ADC_RegularChannelConfig(this->_ADCx, this->_ADC_Channel, Rank, SampleTime);
    ADC_SoftwareStartConvCmd(this->_ADCx, ENABLE);

    // Wait for convert complete
    while (ADC_GetFlagStatus(this->_ADCx, ADC_FLAG_EOC) == 0)
    {
      /* Null */
    }

    return ADC_GetConversionValue(this->_ADCx);
  }
}