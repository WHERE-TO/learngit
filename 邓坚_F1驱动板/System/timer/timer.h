#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"




//////////////////////////////TIM1_OC_Init()参数设置///////////////////////////////////////////
////////////////////////////TIM1时钟//////////////////////////////////////////////////////////
#define TIM1_OC_CLK     RCC_APB2Periph_TIM1
////////////////////////TIM1输出比较通1///////////////////////////
// PA8   通道1
#define TIM1_CH1_GPIO_CLK   RCC_APB2Periph_GPIOA
#define TIM1_OCH1_GPIOx   GPIOA
#define TIM1_OCH1_GPIOx_PIN  GPIO_Pin_8
// PA9   通道2
#define TIM1_CH2_GPIO_CLK   RCC_APB2Periph_GPIOA
#define TIM1_OCH2_GPIOx   GPIOA
#define TIM1_OCH2_GPIOx_PIN  GPIO_Pin_9
/////////////////////////TIM1时基结构体配置(周期设置)//////////////////////////////////////////
#define  TIM1_PER   1000        //重装载值
#define  TIM1_PSC   0      //预分频
/////////////////////////TIM1输出比较(占空比设置)///////////////////////////////////////////////////
#define   TIM1_PULSE  0
///////////////////////////刹车输入通道(失能)//////////////////////////////////////////////////////////////
//#define  TIM1_BRKIN_GPIO_CLK    RCC_APB2Periph_GPIOB
//#define  TIM1_BRKIN_GPIOx       GPIOB
//#define  TIM1_BRKIN_GPIOX_PIN   GPIO_Pin_12
////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************************/
void TIM1_OC_Init(void);

void TIM1_OCH1_setForward(u16 plus);
void TIM1_OCH2_setBack(u16 plus);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TIM3_ECODE_Init(void);
void TIM4_GetSpeed_Init(void);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TIM2_PIDcontrl_Init(void);
	
#endif
