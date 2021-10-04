# STM32F103RB Libraries

[![License: MIT](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/ziteh/stm32f103rb-libraries/blob/main/LICENSE)  

Basic libraries specialized for STM32F103RB ([NUCLEO-F103RB](https://www.st.com/en/evaluation-tools/nucleo-f103rb.html) development board).

專門給 STM32F103RB ([NUCLEO-F103RB](https://www.st.com/en/evaluation-tools/nucleo-f103rb.html) development board) 使用的基本函式庫。 

# Usage / 用法

1. Setup path for [lib](/lib) in your IDE.
2. Include header files, select one of the following:
    1. Add `#include "f103rb_xxx.hpp"` into youe code to use specific features. 
    2. Include [f103rb_lib.hpp](/lib/f103rb_lib.hpp) to use all the features in this libraries.

All the code in this libraries are inside namespace `F103RB`.  
The following example may ellipsis `F103RB::` or `using namespace F103RB;`.

<br/>

1. 在你的 IDE 中設定好 [lib](/lib) 的路徑。
2. 使用以下的其中一種方法來 include 標頭檔：
    1. 在你的程式碼中加入 `#include "f103rb_xxx.hpp"` 以使用特定的功能。
    2. Include [f103rb_lib.hpp](/lib/f103rb_lib.hpp) 以使用此函式庫中的所有功能。

此函式庫的所有程式碼都在命名空間 `F103RB` 之中。  
以下的範例可能會省略 `F103RB::` 或 `using namespace F103RB;`。

## GPIO

Files:
- [f103rb_gpio.cpp](/lib/f103rb_gpio.cpp)
- [f103rb_gpio.hpp](/lib/f103rb_gpio.hpp)
- [f103rb_gpio_mapping.hpp](/lib/f103rb_gpio_mapping.hpp)

### Setup / 設定
```cpp
// Setup PA0 to Push-Pull Output mode, and Speed is 2MHz.
GPIO pin0(PA0, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);

// Setup PB0 to Open-Drain Output mode, and Speed is 50MHz.
GPIO pin1(PB0, GPIO_Mode_Out_OD, GPIO_Speed_50MHz);

// Setup and immediately set it to HIGH.
GPIO pin2(PA1, GPIO_Mode_Out_PP, HIGH, GPIO_Speed_10MHz);

// GPIO Speed is optional. Default is 10MHz.
GPIO pin3(PA2, GPIO_Mode_Out_PP);
GPIO pin4(PA3, GPIO_Mode_Out_PP, HIGH);

// You can use Arduino pin too. D12 is equal to PA6 on NUCLEO-F103RB.
GPIO pin5(D12, GPIO_Mode_Out_PP);

// Input pin.
GPIO pin6(A5, GPIO_Mode_AIN);
```

### Set value / 賦值輸出
```cpp
GPIO pin0(PA0, GPIO_Mode_Out_PP);

// Use HIGH or LOW.
pin0.Set(HIGH);
pin0.Set(LOW);

// You can ues uint8_t too. 1 equals HIGH; 0 equals LOW.
pin0.Set(1);
pin0.Set(0);
```

### Get value / 讀值
```cpp
GPIO_ValueTypeDef value0, value1;

// For input pin.
GPIO pin0(PA0, GPIO_Mode_IPD);
value0 = pin0.Get();

// For output pin.
GPIO pin1(PA1, GPIO_Mode_Out_PP, LOW);
value1 = pin1.Get(); // value1 = LOW.
pin1.Set(HIGH);
value1 = pin1.Get(); // value1 = HIGH.

// Use Get_Input() or Get_Output() to specify read as input or output pin.
value0 = pin0.Get_Input();
value1 = pin1.Get_Output();
```

### Concrete instance / 實例
```cpp
// main.cpp

extern "C"
{
#include "stm32f1xx_nucleo.h"
}
#include "f103rb_gpio.hpp"

using namespace F103RB;

int main(void)
{
  // Setup RCC.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  // Setup LD2(PA5) and immediately set to HIGH (LED ON).
  GPIO Led(LD2, GPIO_Mode_Out_PP, HIGH);

  // Setup D12(PA6) to Push-Pull Output mode.
  GPIO OutputPin(D12, GPIO_Mode_Out_PP);

  // Setup D7(PA8) to Pull-Down Input mode.
  GPIO InputPin(D7, GPIO_Mode_IPD);

  while (1)
  {
    GPIO_ValueTypeDef inputValue = InputPin.Get();

    Led.Set(inputValue);
    OutputPin.Set(inputValue);
  }
}
```
## USART

Files:
- [f103rb_usart.cpp](/lib/f103rb_usart.cpp)
- [f103rb_usart.hpp](/lib/f103rb_usart.hpp)

```cpp
// main.cpp

extern "C"
{
#include <stdio.h>
#include "stm32f1xx_nucleo.h"
}
#include "f103rb_usart.hpp"

using namespace F103RB;

GPIO Led(LD2, GPIO_Mode_Out_PP, LOW);
USART MyUSART(9600);

void USART_Handler()
{
  Led.Set(HIGH);
  uint16_t receivData = USART_ReceiveData(USART2);
  
  // Convert int(ASCII) to char.
  char data[1];
  sprintf(data, "%c", (int)receivData);
  
  USB.Send(data);
  Led.Set(LOW);
}

int main(void)
{
  // Setup NVIC.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  // Setup RCC.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  Led.Init();
  MyUSART.Init();

  MyUSART.Send("Ready!");
  while (1)
  { /* null */ }
}
```

```cpp
// stm32f1xx_it.c

... ...

void USART2_IRQHandler(void)
{
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    USART2_Handler(); // In main.cpp

    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}
```

## PWM

Files:
- [f103rb_pwm.cpp](/lib/f103rb_pwm.cpp)
- [f103rb_pwm.hpp](/lib/f103rb_pwm.hpp)

```cpp
// main.cpp

extern "C"
{
#include "stm32f1xx_nucleo.h"
}
#include "f103rb_pwm.hpp"

using namespace F103RB;

RCC_ClocksTypeDef RCC_Clocks;
PWM myPWM(PA7, TIM3, CH2);

int main(void)
{
  // Setup NVIC.
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  // Setup RCC.
  RCC_GetClocksFreq(&RCC_Clocks);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  myPWM.Init(RCC_Clocks);
  myPWM.Set_Frequency(1500); // 1.5k Hz
  myPWM.Set_DutyCycle(33);   // 33.0 % Duty cycle

  myPWM.Enable(); // Enable PWM output.

  while (1)
  { /* null */ }
}
```

## ADC

Files:
- [f103rb_adc.cpp](/lib/f103rb_adc.cpp)
- [f103rb_adc.hpp](/lib/f103rb_adc.hpp)

```cpp
// main.cpp

extern "C"
{
#include <stdio.h>
#include "stm32f1xx_nucleo.h"
}
#include "f103rb_adc.hpp"
#include "f103rb_usart.hpp"

using namespace F103RB;

USART myUSART(9600);
ADC myADC(A1, ADC1, ADC_Channel_1);

void Delay(__IO uint32_t ms)
{ /* some code here ... */ }

int main(void)
{
  // Setup RCC.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_ADC1,
                         ENABLE);

  myUSART.Init();
  myADC.Init();
  myADC.Enable();

  while (1)
  {
    uint16_t value = myADC.Get_Value();

    // Convert int to char.
    static char msg[1];
    sprintf(msg, "%d", value);

    myUSART.Send(msg);
    myUSART.Send("\n");

    Delay(250);
  }
}
```
