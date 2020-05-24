#include "TM4C123.h"                    // Device header
#include "timers.h"
#include "adc.h"

volatile int temperature;

void TemperatureSensePeripherals_init(void){
	TemperatureSense_ADC_init();	
	Wide_Timer_init(); 
}
	
int SenseTemperature(void){
	while( (ADC0->RIS & 8) == 0 ){}
	temperature = 147 - (247 * ADC0->SSFIFO3)/4096;
	ADC0->ISC = 8;	// clear conversion complete flag		
}
