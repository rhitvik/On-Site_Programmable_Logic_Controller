#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>
#include <stdbool.h>

#define TOGGLE		2U
#define ON				1U
#define OFF				0

#define RED			(1U << 1)
#define BLUE		(1U << 2)
#define GREEN		(1U << 3)
#define PINK		(3U << 1)
#define YELLOW	(5U << 1)
#define CYAN		(3U << 2)
#define WHITE	(7U << 1)



void Initialize_LED_control (void);
void Switch_LED(volatile uint8_t Color, volatile uint8_t LED_state);
void Flash_LED(volatile uint8_t Color, volatile unsigned int NumberOfTimes);
void Toggle_LED(volatile uint8_t Color);
void Blink_LED(volatile uint8_t Color);

bool isLED_on(volatile uint8_t Color);

#endif
