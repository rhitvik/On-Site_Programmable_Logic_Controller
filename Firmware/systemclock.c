#include "TM4C123.h"                    // Device header
#include "systemclock.h"
#include <stdbool.h>

void SysTick_Init(void){
	SysTick->LOAD = ONESEC;
	SysTick->CTRL = 5;
}

bool TimeOut(void){
	return (SysTick->CTRL & 0x10000);
}
