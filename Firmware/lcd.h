#ifndef __LCD_H__
#define __LCD_H__

#define LCD GPIOB
#define RS	1			// 0x01 :	0B 0000 0001 : PB0
#define RW	2			// 0x02 : 0B 0000 0010 : PB1
#define EN	4			// 0x04 : 0B 0000 0100 : PB2

#define	Sel4bitMode					0x28  	// set mode as 4-bits, 2-lines, 5x7 fonts
#define Sel8bitMode					0x38		// set mode as 8-bits, 2 lines,5x7 fonts
#define	MovCursorRight			0x06		// move cursor right 
#define	ClearScreen					0x01		// clear screen
#define	TurnONdisplay				0x0F		// turn on the display
#define	CursorLine1					0x80		// set cursor at line 1
#define	CursorLine2					0xC0		// set cursor at line 2
#define RetCursorHome				0x02		//return cursor home

void LCD4Bits_Init(void);
void LCD_Write4bits(unsigned char data, unsigned char control);
void LCD4bits_Cmd(unsigned char command);
void LCD4bit_Data(unsigned char data);
void LCD_String(char *stringOfCharacters);

#endif
