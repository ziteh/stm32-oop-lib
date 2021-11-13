# STM32 OOP Library

The basic OOP library of STM32.  
Published on PlatformIO ([stm32_oop_lib](https://platformio.org/lib/show/13134/stm32_oop_lib)).

# Features / 功能
- GPIO

> Only tested on STM32F103RB (NUCLEO-F103RB board).  
> 僅在 STM32F103RB (NUCLEO-F103RB 開發板) 上進行過測試。

# Usage / 用法

All the code in this library are inside namespace `stm32_oop_lib`.  
The following example may ellipsis `stm32_oop_lib::` or `using namespace stm32_oop_lib;`.

<br/>

此函式庫的所有程式碼都在命名空間 `stm32_oop_lib` 之中。  
以下的範例可能會省略 `stm32_oop_lib::` 或 `using namespace stm32_oop_lib;`。

## GPIO

### LED Blink Example/ LED 閃爍範例
```cpp
// main.cc

#include <libopencm3/stm32/rcc.h>
#include <stm32_oop_lib_gpio.h>

void delay(int value);

int main(void)
{
  rcc_periph_clock_enable(RCC_GPIOA);

  stm32_oop_lib::GPIO led(stm32_oop_lib::UserLED,
                          stm32_oop_lib::OutputPushPull);

  while (1)
  {
    led.Toggle();
    delay(200000);
  }

  return 0;
}

void delay(int value)
{
  while (value--)
  {
    __asm__("nop");
  }
}
```

## Example Repo / 範例 Repo

- [ziteh/stm32-oop-lib-examples](https://github.com/ziteh/stm32-oop-lib-examples)

# Dependency / 依賴

- [libopencm3/libopencm3: Open source ARM Cortex-M microcontroller library](https://github.com/libopencm3/libopencm3)
