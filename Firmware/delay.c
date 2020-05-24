#include "delay.h"

void _delay_ms(int n){
	int x,y;
	for(x=0;x<n;x++){
		for(y=0;y<3180;y++)
		{}
	}
}

void _delay_us(int n){
	int x,y;
	for(x=0;x<n;x++){
		for(y=0;y<3;y++)
		{}
	}
}
