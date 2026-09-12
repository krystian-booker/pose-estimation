#include <Arduino.h>

// Override the generic F405 variant's internal RC oscillator. The MicoAir
// board has an 8 MHz crystal; camera/IMU timestamps and USB must use it.
extern "C" void SystemClock_Config(void) {
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitTypeDef oscillator{};
    oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    oscillator.HSEState = RCC_HSE_ON;
    oscillator.PLL.PLLState = RCC_PLL_ON;
    oscillator.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    oscillator.PLL.PLLM = 8;
    oscillator.PLL.PLLN = 336;
    oscillator.PLL.PLLP = RCC_PLLP_DIV2;  // 168 MHz system clock
    oscillator.PLL.PLLQ = 7;            // 48 MHz USB clock
    if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) Error_Handler();

    RCC_ClkInitTypeDef clocks{};
    clocks.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                      RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clocks.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clocks.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clocks.APB1CLKDivider = RCC_HCLK_DIV4;
    clocks.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&clocks, FLASH_LATENCY_5) != HAL_OK) Error_Handler();
}
