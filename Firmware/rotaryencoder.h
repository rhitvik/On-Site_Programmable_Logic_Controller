#ifndef __ROTARYENCODER_H__
#define __ROTARYENCODER_H__

#define IN1				(1U << 6)
#define IN2				(1U << 7)

void init_RotaryEncoder(void);
void PollEncoder(void);
int PollEncoder_selection(int index);
void SW_debounce(void);

#endif
