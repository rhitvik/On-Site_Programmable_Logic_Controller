#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "TM4C123.h"                    // Device header
#include "rotaryencoder.h"
#include "pushbutton.h"
#include "lcd.h"
#include "delay.h"
#include "key.h"
/*
	int counter = 0;
  bool aState = 0;
  bool bState = 0;
  bool aLastState = 1;
	char convertedCount[12];	
	unsigned long tempCount = 0;
*/	

int Global_Peripheral_Defination[8] = {4,2,2,1,1,1,1,1};

void init_RotaryEncoder(void){
	SYSCTL->RCGCGPIO |= (0x08);			// Activate clock on the PORTD as the GPIO for IN1 and IN2 is 0b 1100 0000
	GPIOD->LOCK = KEY;							// Unlock special consideration key SW2 which is also a NMI
	GPIOD->CR |= IN2;								// Set CR to 1 to allow changes
	
	GPIOD->DIR &= ~(IN1);						// GPIO Direction SW1 set to input
  GPIOD->AFSEL &= ~(IN1);					// Analog functionality Disabled
  GPIOD->DR2R |= (IN1);						// Set Slew Rate
  GPIOD->ODR &= ~(IN1);						// Open Drain Disabled
  GPIOD->PUR |= (IN1);						// Pull-up Enabled
  GPIOD->DEN |= (IN1);						// Digital Pins Enabled
  
  GPIOD->DIR &= ~(IN2);						// GPIO Direction SW2 set to input
  GPIOD->AFSEL &= ~(IN2);					// Analog functionality Disabled
  GPIOD->DR2R |= (IN2);						// Set Slew Rate
  GPIOD->ODR &= ~(IN2);						// Open Drain Disabled
  GPIOD->PUR |= (IN2);						// Pull-up Enabled
  GPIOD->DEN |= (IN2);						// Digital Pins Enabled
}
/*
void PollEncoder(void){
	while(2){		
		aState = ((GPIOD->DATA & 0x40) >> 6);
		bState = ((GPIOD->DATA & 0x80) >> 7);		
		if(aState!=aLastState){
			if(bState!=aState){
				counter++;
				while(!(((GPIOD->DATA & 0x40) >> 6) == 1));
			}
			else 
			{
				counter--;
				while(!(((GPIOD->DATA & 0x80) >> 7) == 1));
			}
		}
		aLastState = aState;
			
		tempCount++;
		if (tempCount == 30000){
			tempCount = 0;
			snprintf(convertedCount, 12,"%d",counter>>1);		
			// counter increments/decrements in the following fashion 2,4,6,8,10... so divide it by 2 right shift operator
			LCD4bits_Cmd(ClearScreen);		// clear screen
			_delay_ms(2);
			LCD_String(convertedCount);		
		}		
	}
}
*/

void SW_debounce(void){
	_delay_ms(350);		//enter value for software debouncing 
	// can be reduced to 20
}

int PollEncoder_selection(int index){
	uint8_t button_status;
	int actual_value = 0;
	int counter = 0;
  bool aState = 0;
  bool bState = 0;
  bool aLastState = 1;
	char convertedCount[12];	
	unsigned long tempCount = 0;
	
	while(2){		
		button_status = SW_read(); // read the status of SW1 and SW2
		aState = ((GPIOD->DATA & 0x40) >> 6);
		bState = ((GPIOD->DATA & 0x80) >> 7);
		
		if(button_status == SW1_){		// condition to monitor if the button is pressed by the user
			SW_debounce();							// debounce sw pin
			button_status = 0;					// reset button_status. not really necessary since we tend to break out of this function
			counter = 0;								// reset the counter so that it starts counting again from 0
			while(SW_read() != 0x11){}	// wait till the button is relased
			return actual_value;				// Return the value of thte actual count so that 
		}
		else if(aState!=aLastState){
			if(bState!=aState){
				counter++;
				while(!(((GPIOD->DATA & 0x40) >> 6) == 1)){}
			}
			else 
			{
				counter--;
				while(!(((GPIOD->DATA & 0x80) >> 7) == 1)){}
			}
		}
		aLastState = aState;
			
		tempCount++;
		if (tempCount == 20000){
			tempCount = 0;
			actual_value = counter >> 1;										// divide the counter val by 2 to obtain actual value	
			
			// condition to check the state ad the task we are trying to read the values to 
			// some conditions are as follows...
			// 1. Read the values for index for asking the peripherals we want to use
			// 2. Read the values for selecting the programming the valies in the Instruction node
			// 3. Generic count methods for duty setting or the counter setting	-> count up to 100 
			if(actual_value > Global_Peripheral_Defination[index] || actual_value < -2){
				//actual_value = Global_Peripheral_Defination[index];
				counter = (actual_value>Global_Peripheral_Defination[index])? -4:(Global_Peripheral_Defination[index]<<1);
			}
			snprintf(convertedCount, 12,"%d",actual_value);	// counter increments like 2,4,6,8,10... so divide it by 2 right shift operator!!
			LCD4bits_Cmd(CursorLine2);											// go to second line
			_delay_ms(1);
			LCD_String("                ");									// clear second line
			LCD4bits_Cmd(CursorLine2);											// go to second line
			_delay_ms(1);	
			if(actual_value >= 0){
				LCD_String(convertedCount);										// pass the generated output
				_delay_ms(1);	
			}
			else if(actual_value == -1){
				LCD_String("Back");														// pass the generated output
				_delay_ms(1);	
			}
			else if(actual_value == -2){
				LCD_String("Skip");														// pass the generated output
				_delay_ms(1);	
			}
		}		
	}
}
