#include "TM4C123.h"                    // Device header
#include "timers.h"
//#include "led.h"

/*
Test Routine - Passed | Corner Cases - passed

int main(){
	SYSCTL->RCGCGPIO 	|= 0x20;
	GPIOF->DIR				|= BLUE;	// set pin as output
	GPIOF->DEN				|= BLUE;	// enable pin
	
	while(1){
		GPIOF->DATA ^= BLUE;
		periodic_timer(200);
	}	
}

*/

void periodic_timer(int time){
	SYSCTL->RCGCTIMER |= 0x01;						// Clock to Timer-1
	for(int i = 0; i==3;i++){} 						// Wait few clock cycles as a fail safe guarantee 		
	TIMER0->CTL				|= 0x00;						// Disable TIMER0
	TIMER0->CFG				|= 0x04;						// 16-Bit Mode
	TIMER0->TAMR			|= 0x02;						// 0x02 for periodic mode as per the data sheet
	TIMER0->TAILR			|= 16000 - 1;				// Because we start to count from 0
	TIMER0->ICR				|= 0x01;
	TIMER0->CTL				|= 0x01;
	
	for(int j = 0; j < time; j++){
		while ((TIMER0->RIS & 0x01) == 0);
		TIMER0->ICR = 0x01;	// clear ICR to reenable the timer counter
	}	
}

void Wide_Timer_init(void){				
	SYSCTL->RCGCWTIMER |= 0x01;			// Clock to Wide Timer-1
	for(int i = 0; i==3;i++){} 			// Wait few clock cycles as a fail safe guarantee 
	WTIMER0->CTL = 0;								// Disable WTIMER0
	WTIMER0->CFG = 0x04;						// 32-Bit option
	WTIMER0->TAMR = 0x02;						// Periodic mode and down-counter mode
	WTIMER0->TAILR = 16000000; 			// Timer A Interval Load Register (16000000Hz / 16000000 = 1 sec)
	WTIMER0->CTL |= 0x20;						// select timer trigger ADC
	WTIMER0->CTL |= 0x01; 					// enable timer A
}
