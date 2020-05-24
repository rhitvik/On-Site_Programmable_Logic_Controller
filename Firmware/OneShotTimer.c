#include "TM4C123.h"                    		// Device header

// GPTM Configuration GPTMCFG								16-32 / 32/64 Bit configuration
// GPTM Control (GPTMCTL)										Timer-stall, output trigger
// GPTM Interrupt Mask (GPTMIMR)						Enable/Disable Interrupt
// GPTM Raw interrupt Status (GPTMRIS)			Shows Interrupt State
// GPTM Interrupt Clear (GPTMICR)						clear inerrupt status bit
// GPTM Timer A Match (GPTMTAMATCHR)				loaded with interrupt match value for timer A
// GPTM Timer A interval Load (GPTMTAILR)		loaded with timeout/start value for timer A
// GPTM Timer A (GPTMTAR)										shows the current value / no. of edges for timer A
// GPTM Timer A mode (GPTMTAMR)							up vs down | periodic vs one shot | etc.

void timer1_oneShot(int delay_milli){
	SYSCTL->RCGCTIMER |= 0x01;								// 0b 0000 0001 	enable clock in the timer block 0
	TIMER0->CTL = 0;
	TIMER0->CFG = 0x04;												// For a 16/32-bit timer, this value selects the 16-bit timer configuration
	TIMER0->TAMR = 0x01;											// Write a value of 0x1 for One-Shot mode
	TIMER0->TAILR = 16000 * delay_milli - 1;	// Because we are running the clock at 16MHz
																						// And we count down/up to/from 0 not 1
	TIMER0->ICR = 0x01;												// set the interrupt clear register to 1 just as a safety precaution
	TIMER0->CTL |= 0x01;											// Set the TnEN bit in the GPTMCTL register to enable the timer and start counting 
	while((TIMER0->RIS & 0x01) == 0); 				// wait for timeout flat to set	
}
