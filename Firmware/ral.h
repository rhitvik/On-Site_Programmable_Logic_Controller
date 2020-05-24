#ifndef __RAL_H__
#define __RAL_H__

#include <stdlib.h>
#include <stdint.h>

// structure defination for dynamic Instruction allocation
struct Instruction_Node{
	volatile uint32_t OpCode;		// select between if | if/else | while | do while 
	volatile uint32_t Op1;
	volatile uint32_t Op2;
	volatile int Result;
	volatile uint8_t Global_Function_Call_Index[16];			// upto 16 functions can be called in a single instruction defination
	volatile int Global_Function_param_int[16];			// can store upto 16 different input parameters
	volatile char Global_Function_param_char[16];		// can store upto 16 different input characters
	
	struct Instruction_Node *Next_Instruction;		// Self referential pointer that stores the address of the next node
};


void Initialize_PLC(void);
void ALU_Global(struct Instruction_Node *IN);
void Generate_Instruction_Node(struct Instruction_Node *Node);

void Display_Peripheral_Info(int polledVal);
void Define_Peripherals(void);
void Display_Peripheral_Info_prepolled(void);

#endif
