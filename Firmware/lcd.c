#include "lcd.h"
#include "TM4C123.h"                    // Device header
#include "delay.h"


void LCD4Bits_Init(void){
	SYSCTL->RCGCGPIO |= (1U<<1);		// PortB
	LCD->DIR |= 0xFF;
	LCD->DEN |= 0xFF;
	LCD4bits_Cmd(Sel4bitMode); // set mode as 4-bits, 2-lines, 5x7 fonts
	LCD4bits_Cmd(MovCursorRight);	// move cursor right 
	LCD4bits_Cmd(ClearScreen);	// clear screen
	LCD4bits_Cmd(TurnONdisplay);	// turn on the display
	LCD4bits_Cmd(ClearScreen);		// clear screen
	_delay_ms(2);
	LCD4bits_Cmd(CursorLine1);		// set cursor at line 1
	_delay_ms(50);
}

void LCD_String(char *stringOfCharacters){
	while(*stringOfCharacters > 0){
		LCD4bit_Data(*stringOfCharacters++);
		_delay_ms(2);
	}
}

void LCD_Write4bits(unsigned char data, unsigned char control){
	data &= 0xF0;			// clear lower 4 bits to keep upper bits i.e. data bits
	control &= 0x0F;	// clear upper 4 bits to keep lower bits i.e. control bits
	LCD->DATA = data | control;
	LCD->DATA = data | control | EN;
	_delay_us(2);		// was 0 
	LCD->DATA = data;
}

void LCD4bits_Cmd(unsigned char command){
	LCD_Write4bits(command & 0xF0 , 0);		// upper 4-bits
	LCD_Write4bits(command << 4 , 0);			// lower 4-bits
	
	if(command < 4)
		{_delay_ms(2);}
	else
		{_delay_us(40);}
	
}

void LCD4bit_Data(unsigned char data){
	LCD_Write4bits(data & 0xF0, RS);
	LCD_Write4bits(data << 4, RS);
	_delay_us(40);
}
