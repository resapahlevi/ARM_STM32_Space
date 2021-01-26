/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

// ----- LED definitions ------------------------------------------------------

// Adjust these definitions for your own board.

#if defined(BOARD_OLIMEX_STM32_E407)

// STM32-E407 definitions (the GREEN led, C13, active low)

// Port numbers: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, ...
#define BLINK_PORT_NUMBER               (2)
#define BLINK_PIN_NUMBER                (13)
#define BLINK_ACTIVE_LOW                (1)

#else

// STM32F4DISCOVERY definitions (the GREEN led, D12, active high)
// (SEGGER J-Link device name: STM32F407VG).

#define BLINK_PORT_NUMBER               (1)
#define BLINK_PIN_NUMBER                (6)
#define BLINK_ACTIVE_LOW                (0)

#endif

#define BLINK_GPIOx(_N)                 ((GPIO_TypeDef *)(GPIOB_BASE + (GPIOB_BASE-GPIOB_BASE)*(_N)))
#define BLINK_PIN_MASK(_N)              (1 << (_N))
#define BLINK_RCC_MASKx(_N)             (RCC_AHB1ENR_GPIOBEN << (_N))

// ----------------------------------------------------------------------------

extern
void
blink_led_init(void);

// ----------------------------------------------------------------------------

inline void
blink_led_on(void);

inline void
blink_led_off(void);

// ----------------------------------------------------------------------------

inline void
__attribute__((always_inline))
blink_led_on(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	}
}

inline void
__attribute__((always_inline))
blink_led_off(void)
{
#if (BLINK_ACTIVE_LOW)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
#else
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
#endif
}

// ----------------------------------------------------------------------------

#endif // BLINKLED_H_
