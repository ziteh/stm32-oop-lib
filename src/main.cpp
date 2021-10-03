/**
  * @file     main.cpp
  * @author   ZiTe <honmonoh@gmail.com>
  * @brief    Main program body
  */

extern "C"
{
#include <stdio.h>
#include "main.h"
}
//#include <sstream>
#include "gpio.hpp"
#include "usart.hpp"
#include "pwm.hpp"

static __IO uint32_t TimingDelay;
uint8_t BlinkSpeed = 0;

using namespace F103RB;

RCC_ClocksTypeDef RCC_Clocks;

USART USB;
GPIO led(LD2, GPIO_Mode_Out_PP, HIGH);
PWM myPWM(PA7, TIM3, CH2);

int main(void)
{
  Init_NVIC();
  Init_RCC();

  led.Init();
  USB.Init();
  myPWM.Init(RCC_Clocks);
  myPWM.Set_Frequency(1500);
  myPWM.Set_DutyCycle(33);
  myPWM.Enable();

  USB.Send("OK");

  while (1)
  {
    led.Toggle();
    //    USB.Send("a");
    Delay(500);
  }
}

void Init_NVIC()
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

void Init_RCC()
{
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 |
                             RCC_APB1Periph_TIM3,
                         ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,
                         ENABLE);
}

void USART2_Handler()
{
  uint16_t receivData = USART_ReceiveData(USART2);

  // led.Toggle();
  if (receivData == 0x00)
  {
    USB.Send("Hello");
  }
  else
  {
    char d[1];
    sprintf(d, "%c", (int)receivData);
    USB.Send(d);
  }
}

/* Modified by Atollic to build with c++ */
extern "C"
{
  /**
* @brief  Inserts a delay time.
* @param  nTime: specifies the delay time length, in 1 ms.
* @retval None
*/
  void Delay(__IO uint32_t nTime)
  {
    TimingDelay = nTime;

    while (TimingDelay != 0)
      ;
  }

  /**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
  void TimingDelay_Decrement(void)
  {
    if (TimingDelay != 0x00)
    {
      TimingDelay--;
    }
  }
}

#ifdef USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
