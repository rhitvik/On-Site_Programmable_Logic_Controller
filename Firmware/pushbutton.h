#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

#include <stdbool.h>
#include <stdint.h>

#define SW1					(1U<<4)		// Push Button for user selection
#define SW2					(1U<<0)		// NMI pin | Required Special consideration register operation sequence
#define LOCKED 			0
#define UNLOCKED		1
#define SW1_				0x01
#define SW2_				0x10
#define SW12				0x00

void Unlock_SCR(void);
void Initialize_PushButtons(void);

uint32_t SW_read(void);
uint8_t detect_SWITCH_press(void);

// more functions can be added but not because we are testing the project with the...
// ...main functions first than going to manipulate the nuances to proove the concept


#endif
