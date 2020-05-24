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
// function to ask what peripherals the user wants to use
// this function will ask bunch of questions on the lcd interfaced and we will have to select yes/no...
// ...for every option provided
// the options are 4 GPIOs, 2-timers, 2-PWM, 1-UART, 1-I2C, 1-SPI, 1-NMI
// interrupts assioated with the above peripherals are TBD
*/
/*
program flow...

-->	If the user selects the option for the peripheral, put 1 in the 
		assioated index (default value 0) of the array. 

--> A linked list with a common defination of the instruction and processing codes
		32-bit sized and used for understanding conditional statement defination 
		(setup plc)
		
		void loop(){ 
			while(1){
				some work;
				is condition met (like a beq instruction)
				update the user of what is happening
				break if the condition is met()
			}
		}


*/
/*
 work assioated with eaery peripheral
 
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
struct Instruction_Node *First_Instruction = NULL, *Last_Instruction = NULL, *Y_Node = NULL;
int peripheralIndex = 0;
// GPIO pins-4, Timer-2, PWM-2, UART-1, I2C-1, SPI/SSI-1, NMI-1, CAN-1

uint8_t Users_Peripheral_Defination[8] = {0};		
// Global array to store the definations for the peripherals employed


// global functions

// 1st one for selecting the function on which the the operands will operate on.
void ALU_Global(struct Instruction_Node *IN){
	uint32_t result;
	int temp;
	switch (IN->OpCode){
		case 0:																			//	No operation for input 0
			break;
		case 1:																			//	if (op1 > op2)
			result = (IN->Op1 > IN->Op2)? 1:0;
			break;
		case 2:																			//	if (op1 < op2)
			result = (IN->Op1 < IN->Op2)? 1:0;
			break;				
		case 3:																			//	if (op1 == op2)
			result = (IN->Op1 == IN->Op2)? 1:0;
			break;
		case 4:																			//  if (op1 >= op2)			
			result = (IN->Op1 >= IN->Op2)? 1:0;
			break;
		case 5:																			//	if (op1 <= op2)			
			result = (IN->Op1 <= IN->Op2)? 1:0;
			break;
		case 6:																			//  if (op1 || op2)
			result = (IN->Op1 || IN->Op2) > 0? 1:0;
			break;
		case 7:																			//  if (op1 && op2)
			result = (IN->Op1 && IN->Op2) > 0? 1:0;
			break;
		case 8:																			//  op1 << op2
			result = (IN->Op1 << IN->Op2);
			break;
		case 9:																			//  op1 >> op2
			result = (IN->Op1 >> IN->Op2);
			break;
		case 10:																		//	op1 + op2  
			result = (IN->Op1 + IN->Op2);
			break;
		case 11:																		//  op1 - op2  
			temp = (int) (IN->Op1) - (int) (IN->Op2);
			break;
		case 12:																		//  op1 * op2
			result = (IN->Op1 * IN->Op2);
			break;
		case 13:																		//  op1 / op2 --> integer result only
			result = (IN->Op1 / IN->Op2);
			break;
		case 14:																		//  op1 | op2
			result = (IN->Op1 | IN->Op2);
			break;
		case 15:																		//  op1 & op2
			result = (IN->Op1 & IN->Op2);
			break;
		case 16:																		//  op1 ^ op2
			result = (IN->Op1 ^ IN->Op2);
			break;
		case 17:																		//  invert op1
			result = ~(IN->Op1);
			break;
		case 18:																		//  invert op2
			result = ~(IN->Op2);
			break;
		case 19:																		//  toggle op1
			result = IN->Op1^IN->Op1;
			break;
		case 21:																		//  toggle op2
			result = IN->Op2^IN->Op2;
			break;
/*	case 22:																		//  MORE CASES FOR FUTURE USE
			break;
		case 23:																		//  MORE CASES FOR FUTURE USE
			break;				*/	
	}
	IN->Result = (IN->OpCode) == 11? temp:(int) result;
}

// 2nd the function in which all services will be added and input parameters will be taken from the structure itself.


// 3rd one will be a function that will walk the user through the setup each instruction node.
void Generate_Instruction_Node(struct Instruction_Node *Node){		// This function helps enter the values of the params in Instruction Node
	struct Instruction_Node *temp = NULL;
	temp = (struct Instruction_Node*) malloc(sizeof(struct Instruction_Node));
	if(First_Instruction == NULL){
		First_Instruction = Last_Instruction = temp;				// make the global pointers point at the first allocated instruction structure
		Last_Instruction->Next_Instruction = NULL;					// make the self referential node of the last node to point at null
	}
	else if(temp){
		// this means that that the memory space was successfully allocated (memory not full)
		// also this means that the First instruction node has been defined 
		if (Last_Instruction == First_Instruction){
			First_Instruction->Next_Instruction = temp;				// make the first instruction to point at the next instruction
			Last_Instruction->Next_Instruction = temp;
			Last_Instruction = temp;	
			Last_Instruction->Next_Instruction = NULL;				// make the self referential node of the last node to point at null
		}
		else if(Last_Instruction != First_Instruction){			// this means that the first instruction has been already defined
			Last_Instruction->Next_Instruction = temp;
			Last_Instruction = temp;
			Last_Instruction->Next_Instruction = NULL;				// make the self referential node of the last node to point at null		
		}
	}
	else {
		// show error message on lcd that the memory is full
		// press reset and try again 
	}
}

// fourth function will help to establish the structure if the program flow
// this will ask if the setup is complete

void Initialize_PLC(void){	
	// this is going to act as the void setup() function of arduino

	// show welcome message
	_delay_ms(50);
	LCD_String("  Project  PLC  ");
	LCD4bits_Cmd(CursorLine2);		// set cursor at line 1
	_delay_ms(50);
	LCD_String("Initializing PLC");
	
	
	for(int i = 0; i<6;i++){
		_delay_ms(1000);	// wait for some time to read the message
	}
	
	_delay_ms(10);		
	LCD4bits_Cmd(ClearScreen);		// clear screen
	_delay_ms(2);
	LCD_String("How Many GPIOs");

	
	/*
	// this will ask you the following questions
	Select Number by Rotary Encoder
	press sw[1] to select yes/no
	SW2 is the NMI for usage!
	
	0. Display all the information i.e., apart from the gpios tuned as HMI and the LEDs, provide options for
			GPIO pins-4, Timer-2, PWM-2, UART-1, I2C-1, SPI/SSI-1, NMI-1, CAN-1

	1. 	LCD line 1: How many GPIOs you want to use?
			LCD line 2: selection number -2(means skip function),-1(means go back),0,1,2,3,4
	
	2. 	LCD Line 1: How many Timers you want to use?
			LCD Line 2:	selection nunber -2,-1(means go back),0,1,2
	
	3.  LCD Line 1: How many PWM ports you want to use?
			LCD Line 2: Selection number -2,-1,0,1,2
	
	4.  LCD Line 1: How many UART ports you want to use?
			LCD Line 2: Selection number -2,-1,0,1
	
	5.  LCD Line 1: How many I2C ports you want to use?
			LCD Line 2: Selection number -2,-1,0,1
	
	6.  LCD Line 1: How many SPI/SSI ports you want to use?
			LCD Line 2: Selection number -2,-1,0,1

	7.  LCD Line 1: How many NMI ports you want to use?
			LCD Line 2: Selection number -2,-1,0,1

	8.  LCD Line 1: How many CAN ports you want to use?
			LCD Line 2: Selection number -2,-1,0,1
	
	*/
}


void Display_Peripheral_Info_prepolled(void){
	_delay_ms(3);
	LCD4bits_Cmd(ClearScreen);
	_delay_ms(3);
	
	switch (peripheralIndex){
		case (0):		
			_delay_ms(3);
			LCD_String("Set # of GPIOs");
			_delay_ms(3);		
			break;
		
		case (1):		
			_delay_ms(3);
			LCD_String("Set # of Timers");
			_delay_ms(3);		
			break;
		
		case (2):
			_delay_ms(3);
			LCD_String("Set # of PWM ch");
			_delay_ms(3);
			break;			
		
		case (3):
			_delay_ms(3);
			LCD_String("Set UART Port");
			_delay_ms(3);
			break;		
	
		case (4):
			_delay_ms(3);
			LCD_String("Set I2C/TWI Port");
			_delay_ms(3);
			break;					
		
		case (5):		
			_delay_ms(3);
			LCD_String("Set SPI/SSI Port");
			_delay_ms(3);
			break;				
		
		case (6):
			_delay_ms(3);
			LCD_String("Set NMI Pin");
			_delay_ms(3);
			break;				
		
		case (7):
			_delay_ms(3);
			LCD_String("Set CAN Port");
			_delay_ms(3);
			break;
	}
}

void Display_Peripheral_Info(int polledVal){
	char convertedInteger[12];		
	// for converting integer to string for lcd display
	snprintf(convertedInteger, 12,"%d",polledVal);	// counter increments like 2,4,6,8,10... so divide it by 2 right shift operator!!
	LCD4bits_Cmd(ClearScreen);
	_delay_ms(2);
	switch (polledVal){
		case (-2):
			_delay_ms(3);
			LCD_String("Index is Skipped");
			_delay_ms(3);
		
		  _delay_ms(DisplayDelay);
			break;
			
		case (-1):
			_delay_ms(3);
			LCD_String("Previous index");
			_delay_ms(3);
		
		  _delay_ms(DisplayDelay);
			break;
	}
	
	switch(peripheralIndex){		
		case (0):
			_delay_ms(3);
			LCD_String("# of GPIO = ");
			_delay_ms(3);
			LCD_String(convertedInteger);
			_delay_ms(DisplayDelay);		// dalay on purpose to notice

			break;
		
		case (1):
			_delay_ms(3);
			LCD_String("# of Timers = ");
			_delay_ms(3);
			LCD_String(convertedInteger);
			_delay_ms(DisplayDelay);		// dalay on purpose to notice
		
			break;			
		
		case (2):
			_delay_ms(3);
			LCD_String("# of PWM ch = ");
			_delay_ms(3);
			LCD_String(convertedInteger);
			_delay_ms(DisplayDelay);		// dalay on purpose to notice

			break;		
	
		case (3):
			_delay_ms(3);
			LCD_String("# of UART ch = ");
			_delay_ms(3);
			LCD_String(convertedInteger);
			_delay_ms(DisplayDelay);		// dalay on purpose to notice
		
			break;					
		
		case (4):
			_delay_ms(3);
			LCD_String("# I2C/TWI ch = ");
			_delay_ms(3);
			LCD_String(convertedInteger);
			_delay_ms(DisplayDelay);		// dalay on purpose to notice

			break;				
		
		case (5):
			_delay_ms(3);
			LCD_String("# of SPI/SSI = ");
			_delay_ms(3);
			LCD_String(convertedInteger);
			_delay_ms(DisplayDelay);		// dalay on purpose to notice

			break;				
		
		case (6):
			_delay_ms(3);
			LCD_String("# NMI Pin = ");
			_delay_ms(3);
			LCD_String(convertedInteger);
			_delay_ms(DisplayDelay);		// dalay on purpose to notice

			break;				
		
		case (7):
			_delay_ms(3);
			LCD_String("# of CAN ch = ");
			_delay_ms(3);
			LCD_String(convertedInteger);
			_delay_ms(DisplayDelay);		// dalay on purpose to notice
		
			LCD_String("Peripherals Done");
			_delay_ms(3);	
			_delay_ms(DisplayDelay);
		
			LCD4bits_Cmd(ClearScreen);
			_delay_ms(3);
			LCD_String("Setup Ins_Node");
			_delay_ms(3);	
			_delay_ms(DisplayDelay);
			break;							
	}
}


void Define_Peripherals(void){
	int polledVal;
	_delay_ms(50);	
	LCD4bits_Cmd(ClearScreen);
	_delay_ms(3);
	LCD_String("Initializing PLC");
	LCD4bits_Cmd(CursorLine2);		// set cursor at line 1
	_delay_ms(50);
	LCD_String("Lets Begin: ");
	_delay_ms(10);
	_delay_ms(DisplayDelay);
	
	while(32){
		Display_Peripheral_Info_prepolled();
		polledVal = PollEncoder_selection(peripheralIndex);
		Display_Peripheral_Info(polledVal);
		switch(polledVal){
			case (-2):
				Flash_LED(RED,1);  
				//Display_Peripheral_Info(polledVal);
				
				// this means that we can skip the current index leaving it set to default
				peripheralIndex++;
				break;
			case (-1):
				Flash_LED(BLUE,1); 
				// this means that we have to go back to the previous index
				// show error message if we try to go back beyond 0
				peripheralIndex = (peripheralIndex > 0)? (peripheralIndex - 1):(peripheralIndex);
				
				break;
			
			case (0):
				Flash_LED(GREEN,1); 
				Users_Peripheral_Defination[peripheralIndex] = polledVal;
				peripheralIndex++; 
				break;
			case (1):
				Flash_LED(PINK,1); 
				Users_Peripheral_Defination[peripheralIndex] = polledVal;
				peripheralIndex++;
				break;
			case (2):
				Flash_LED(YELLOW,1);
				Users_Peripheral_Defination[peripheralIndex] = polledVal;
				peripheralIndex++; 
				break;
			case (3):
				Flash_LED(CYAN,1); 
				Users_Peripheral_Defination[peripheralIndex] = polledVal;
				peripheralIndex++;
				break;
			case (4):
				Flash_LED(WHITE,1); 
				Users_Peripheral_Defination[peripheralIndex] = polledVal;
				peripheralIndex++;
				break;
		}
		if(peripheralIndex > 7){
			// the last peripheral defination array has been programmed
			Blink_LED(RED);	// for debugging
			break;	// break out of the function "define peripherals"
		}
	}
}

//uint8_t temp;
