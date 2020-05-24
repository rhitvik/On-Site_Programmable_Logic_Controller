#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#include "TM4C123.h"                    // Device header for ARM CMSIS Core and peripherals
#include "ral.h"
#include "key.h"
#include "lcd.h"
#include "delay.h"
#include "systemclock.h"
#include "rotaryencoder.h"
#include "led.h"
#include "pushbutton.h"
#include "timers.h"
#include "adc.h"
#include "coretemp.h"
/*
			// peripheralIndex Globally defined
			// GPIO pins-4, Timer-2, PWM-2, UART-1, I2C-1, SPI/SSI-1, NMI-1, CAN-1
			switch(peripheralIndex){
				case (0):		// next is timer selection
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Set # of Timers");
					_delay_ms(1);
					break;
				
				case (1):		// next is PWM Selection
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Set # of PWM ch");
					_delay_ms(1);
					break;
				
				
				case (2):		// next is UART Selection
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Set UART Port");
					_delay_ms(1);
					break;
				
				
				case (3):		// next is I2C Selection
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Set I2C/TWI Port");
					_delay_ms(1);
					break;
				
				
				case (4):		// next is SPI/SSI Selection
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Set SPI/SSI Port");
					_delay_ms(1);
					break;
				
				
				case (5):		// next is NMI-1 Selection
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Set NMI Routine");
					_delay_ms(1);
					break;
				
				
				case (6):		// next is CAN Selection
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Set CAN Port");
					_delay_ms(1);
					break;
				
				
				case (7):		// the index is entered by the user at this point
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Peripherals Done");
					_delay_ms(1);	
					_delay_ms(1000);
				
					LCD4bits_Cmd(ClearScreen);
					_delay_ms(1);
					LCD_String("Setup Ins_Node");
					_delay_ms(1);	
					_delay_ms(1000);
					break;				
			} 
*/

int main(){
	Initialize_LED_control();
	Initialize_PushButtons();
	
	//Flash_LED(GREEN,10);
	//temp = detect_SWITCH_press()==3?GREEN:RED;
	//Switch_LED(temp,TOGGLE);
	
	// Blink_LED(BLUE);
	
 _delay_ms(150);
	
	LCD4Bits_Init();
	_delay_ms(50);
	LCD_String("RHITVIK@NYU.EDU");
	LCD4bits_Cmd(CursorLine2);		// set cursor at line 1
	_delay_ms(50);
	LCD_String("MakerSpace ");
	
  _delay_ms(50);

	init_RotaryEncoder();
	Define_Peripherals();	
}

/*

1. GPIO
		*	setup pin
		* Read PIN
		* change pin state

 2. Timer
		* setup timer 
		* Read timing register
		* Invoke interrupt if requested
		* Wait for some time

3.	PWM
		* setup PWM
		* Increase Duty
		* Decrease Duty
		* Increase Frequency
		* Decrease Frequency
		
4.	UART
		* Setup UART | baud, endianess, parity etc.
		* Transmit Data
		* Receive Data
		* Flush Buffer
		
5. 	I2C
		* Setup I2C | 
		* Transmit Data
		* Receive Data
		* Write to a specific address
		* Read from a specific Address
		
6.  SPI
		* Setup SPI | endianess, clock rate, MSB first / LSB first
		* Transmit Data
		* Receive Data
		
7.	NMI
		* Setup NMI PIN->provide options for rising edge/falling edge only
		* Set NMI bit when invoked
		* if and a counter
		
*/
