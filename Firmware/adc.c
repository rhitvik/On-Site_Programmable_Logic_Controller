// 2 adc modules 12 bit resolutions in tm4c1230h6pm

// ss0 8 sample sqeuencer FIFO Basis 3
// ss1 4
// ss2 4
// ss3 1
// ADCACTSS 
// ADCMUX
// ADCPSSI
// ADCRIS

// AIN0 		ADC input 0		PE3
// AIN1 		ADC input 1		PE2
// AIN2 		ADC input 2 	PE1
// AIN3 		ADC input 3		PE0
// AIN4 		ADC input 4		PD3
// AIN5 		ADC input 5		PD2
// AIN6 		ADC input 6		PD1
// AIN7 		ADC input 7		PD0
// AIN8 		ADC input 8		PE5
// AIN9 		ADC input 9		PE4
// AIN10 		ADC input 10	PB4
// AIN11 		ADC input 11	PB5

#include "TM4C123.h"                    // Device header

volatile int result;

void ADC_12Bit_init(void){
	SYSCTL->RCGCGPIO  |= 0x10;						// Enable clock for port E
	SYSCTL->RCGCADC		|= 0x01;						// Enable clock for ADC module 0
	
	// PE3 init
	GPIOE->AFSEL |= 8;										// Enable alternate function
	GPIOE->DEN	 &= ~8;										// Disable Digital Function
	GPIOE->AMSEL |= 8;										// Ebable analog function
	
	// ADC init
	ADC0->ACTSS &= ~8;										// Disable Sample Sequencer 3 (SS3)
	ADC0->EMUX  &= ~0xF000;								// software trigger conversion
	ADC0->SSMUX3 = 0;											// Get in/out from channel 0
	ADC0->SSCTL3 |= 6;										// Take one sample at a time, set flat at 1st sample
	ADC0->ACTSS |= 8;											// Enable SS3
}

void TemperatureSense_ADC_init(void){
	// ADC Init for CPU core temperature sense module
	SYSCTL->RCGCADC |= 0x01;
	ADC0->ACTSS &= ~8;
	ADC0->EMUX  &= ~0xF000;
	ADC0->EMUX 	|= 0x5000;			// Timer Trigger conversion sequence 0
	ADC0->SSMUX3 = 0;
	ADC0->SSCTL3 = 0x0E;				// Take MCU tempereture, set flat at first sample
	ADC0->ACTSS |= 8;
}

int Calculate_12Bit_ADC_Val(void){
	ADC0->PSSI |= 8;										// Start conversion at SS3
	while ( (ADC0->RIS & 8) == 0 ){}		// wait for conversion complete
	result = ADC0->SSFIFO3;							// stores the value in a volatile global variable
	ADC0->ISC = 8;											// clear flag to re-enable reconversion
	return result;
}
