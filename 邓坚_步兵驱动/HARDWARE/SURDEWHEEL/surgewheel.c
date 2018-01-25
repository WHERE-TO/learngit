#include "surgewheel.h"



void SurWheel_Init(void)
{
	  TIM3_OC_Init();
	
}
void SurWheel_SetP(u32 compare)
{
	  TIM3_OCH3_PLUSE( compare);
    TIM3_OCH4_PLUSE( compare);
}
