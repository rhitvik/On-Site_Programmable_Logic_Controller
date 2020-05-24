#include <stdbool.h>
#include <stdint.h>
#include "TM4C123.h"                    					// Device header
#include "pushbutton.h"
#include "key.h"
#include "led.h"
#include "delay.h"

volatile bool PushButton_SCR_Status = LOCKED; 		 

void Unlock_SCR(void){														// Unlocks the special consideration Registers for one of the switches
	if ( ((SYSCTL->RCGCGPIO >> 5) & 1U ) == 0){
		SYSCTL->RCGCGPIO |= (0x20);										// Activate clock on the PORTF
	}
	GPIOF->LOCK = KEY;
	GPIOF->CR = 0x01;
	GPIOF->LOCK = 0;
	PushButton_SCR_Status = UNLOCKED;
}

void Initialize_PushButtons(void){
	if ( ((SYSCTL->RCGCGPIO >> 5) & 1U ) == 0){
		SYSCTL->RCGCGPIO |= (0x20);										// Activate clock on the PORTF
	}
	if(PushButton_SCR_Status == LOCKED){
		Unlock_SCR();
	}
	GPIOF->DIR &=	 ~0x11;														// make pf4 and pf0 input pins
	GPIOF->DEN |=		0x11;														// enable digital gpio functionality	 
	GPIOF->PUR |=		0x11;														// enable pullup in gpios	as a default parameter
}

uint32_t SW_read(void){
	return GPIOF->DATA & 0x11; 											// read PinF4 or PinF0
}

uint8_t detect_SWITCH_press(void){
	volatile uint32_t SW_state; 										// reads the gpio data register and stores the value
	volatile uint8_t SW_retVal;
	
	while(SW_read() == 0x11){}											// wait till either SW1 or SW2 are pressed
		
	_delay_ms(100);																	// debounce and let us read both switch pressed simultaneously
	SW_state = SW_read();	
	
		switch(SW_state){
		case SW1_:
			SW_retVal = 1;
			break;
		case SW2_:
			SW_retVal = 2;
			break;
		case SW12:
			SW_retVal = 3;
			break;
	}		
	return SW_retVal;
}
