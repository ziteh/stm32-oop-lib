# STM32F103RB Libraries

[![License: MIT](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/ziteh/stm32f103rb-libraries/blob/main/LICENSE)  

Basic libraries specialized for STM32F103RB ([NUCLEO-F103RB](https://www.st.com/en/evaluation-tools/nucleo-f103rb.html) development board).

專門給 STM32F103RB ([NUCLEO-F103RB](https://www.st.com/en/evaluation-tools/nucleo-f103rb.html) development board) 使用的基本函式庫。 

# Usage / 用法

All the code in this libraries are inside namespace `F103RB`.  
The following example may ellipsis `F103RB::` or `using namespace F103RB;`.

此函式庫的所有程式碼都在命名空間 `F103RB` 之中。  
以下的範例可能會省略 `F103RB::` 或 `using namespace F103RB;`。

## GPIO
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
#include "gpio.hpp"

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