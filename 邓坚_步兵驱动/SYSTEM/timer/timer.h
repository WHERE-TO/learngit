#ifndef _TIMER_H
#define _TIMER_H

#include "main.h"

void TIM3_OC_Init(void);
void TIM3_OCH3_PLUSE(u32 pluse);
void TIM3_OCH4_PLUSE(u32 pluse);
void TIM7_PIDControl_Init(void);
void TIM6_DELAY_Init(void);
void TIM6_delay(u16 ms);

#endif
