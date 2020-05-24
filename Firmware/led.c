#include "TM4C123.h"                    // Device header
#include "led.h"
#include "delay.h"

void Initialize_LED_control (void){ 
	
	if ( ((SYSCTL->RCGCGPIO >> 5) & 1U ) == 0){
		SYSCTL->RCGCGPIO |= (0x20);											// Activate clock on the PORTF as the GPIO for SW1 and SW2 is 0b 0010 0000
	}

	GPIOF->DIR |= (RED | BLUE | GREEN);		// Set gpio pins conneted to LEDs to Output
	GPIOF->DEN |= (RED | BLUE | GREEN);		// Enable GPIO pins conneced to LEDs
	GPIOF->DATA &= ~(RED | BLUE | GREEN);	// Turn-off LEDs
}

void Switch_LED(volatile uint8_t Color, volatile uint8_t LED_state){
	switch (LED_state){
		case OFF:
			GPIOF->DATA &= ~Color;
			break;
		case ON:
			GPIOF->DATA |= Color;
			break;
		case TOGGLE:
			GPIOF->DATA ^= Color;
			break;
	} 
}

void Flash_LED(volatile uint8_t Color, volatile unsigned int NumberOfTimes){
	int i; // Double the value of number of times so that we can see the flash equal to number of times
	for (i = NumberOfTimes << 1; i > 0; i--){
		GPIOF->DATA ^= Color;
		_delay_ms(250);		
	}
}

void Toggle_LED(volatile uint8_t Color){
	GPIOF->DATA ^= Color;
}
void Blink_LED(volatile uint8_t Color){
	while(1){
		GPIOF->DATA |= Color;
		_delay_ms(1000);
		GPIOF->DATA &= ~Color;
		_delay_ms(1000);
	}
}

bool isLED_on(volatile uint8_t Color){
	bool Led_Status;
	if(Color == RED){
		Led_Status = (GPIOF->DATA >> 1) & 1U;
		return Led_Status;
	}
	else if (Color == BLUE){
		Led_Status = (GPIOF->DATA >> 2) & 1U;
		return Led_Status;
	}
	else if (Color == GREEN){
		Led_Status = (GPIOF->DATA >> 3) & 1U;
		return Led_Status;
	}
	return Led_Status;		// just to avoid the warning
}
