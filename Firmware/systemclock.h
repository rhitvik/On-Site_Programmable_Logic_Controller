#ifndef __SYSTEMCLOCK_H__
#define __SYSTEMCLOCK_H__

#include <stdbool.h>

#define ONESEC		16000000 - 1
#define MILLISEC	16000 - 1
#define MICROSEC	16 - 1

void SysTick_Init(void);
bool TimeOut(void);

#endif
