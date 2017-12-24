#include <stm32f10x.h>
#include <stdbool.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>

#include <string.h>

typedef struct {
    int set_point, actual_value;
    float max, min;
    int e, i;
    float kp, ki, kd;
} pid_t;

void tim1_config(void);
void tim3_config (void);
void pid_t_init (pid_t* , float , float, int);
float pid_update (pid_t*);
void set_duty (int );

float duty_cycle = 0;
int last_circle= 0;

float manp;

pid_t pid;

int main(void) {
    tim1_config();
    tim3_config();

    // Set 1ms(1000Hz)
    SysTick_Config (SystemCoreClock / 1000);

    pid_t_init (&pid, 0.1, 1, 160);

    while(1) {
        /*if(direction) {
            TIM1->CCER &= 0xffef;
            TIM1->CCER |= 0x0001;
        }
        else {
            TIM1->CCER &= 0xfffe;
            TIM1->CCER |= 0x0010;
        }

        TIM_SetCompare1(TIM1, duty_cycle * 1000 - 1);
        TIM_SetCompare2(TIM1, duty_cycle * 1000 - 1);*/


    }
}

void tim1_config(void) {
    // PA8/TIM1 CH1/PWM1 | PA9/TIM1 CH2/PWM2 
    
    // GPIO Config 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM Config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // base
    TIM_TimeBaseInitTypeDef TIM_BaseStructure;
    TIM_TimeBaseStructInit(&TIM_BaseStructure);
    TIM_BaseStructure.TIM_Prescaler = 0;
    TIM_BaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseInit(TIM1, &TIM_BaseStructure);
    
    // Channel 1 & 2
    TIM_OCInitTypeDef TIM_OCStructure;
    TIM_OCStructInit(&TIM_OCStructure);
    TIM_OCStructure.TIM_OCMode = TIM_OCMode_PWM1;
    
    TIM_OC1Init(TIM1, &TIM_OCStructure);
    TIM_OC2Init(TIM1, &TIM_OCStructure);

    // Enable TIM
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void tim3_config (void) {
    // PA6/CH1 | PA7/CH2
    
    // GPIO Config
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init (GPIOA, &GPIO_InitStructure);
    
    // TIM Config
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);

    // Base 
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_TimeBaseStructInit (&TIM_BaseInitStructure);
    TIM_BaseInitStructure.TIM_Prescaler = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_Period = 65535;
    TIM_TimeBaseInit (TIM3, &TIM_BaseInitStructure);

    TIM_EncoderInterfaceConfig (TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    TIM_Cmd (TIM3, ENABLE);

}

void pid_t_init (pid_t* ptr, float min, float max, int set_point) {
    memset (ptr, 0, sizeof(pid_t));
    ptr->set_point = set_point;
    ptr->min = min;
    ptr->max = max;
    ptr->kp = 0.0067;
    ptr->ki = 0.0015;
    ptr->kd = 0.0;
}

float pid_update (pid_t* ptr) {
    float last_e, tmpi;

    last_e = ptr->e;
    ptr->e = ptr->set_point - ptr->actual_value;
    tmpi = ptr->i + ptr->e;

    manp = ptr->kp * ptr->e +
           ptr->ki * tmpi +
           ptr->kd * (last_e - ptr->e);

    if (manp >= 0) {
        if ( (manp < ptr->max) && (manp > ptr->min) ) {
            ptr->i = tmpi;
            if (ptr->i > 600)   ptr->i = 600;
            else if (ptr->i < -600) ptr->i = -600;
        }
        else if ( manp > ptr->max) {
            manp = ptr->max;
        }
        else if (manp < ptr->min) {
            manp = 0;
        }
    } else {
        if ( (manp > -ptr->max) && (manp < -ptr->min) ) {
            ptr->i = tmpi;
        }
        else if ( manp < -ptr->max) {
            manp = -ptr->max;
        }
        else if (manp > -ptr->min) {
            manp = 0;
        }
    }

    return manp;
}

// Acquire real-time error and update PID parameter
void SysTick_Handler (void) {
    pid.actual_value= TIM3->CNT - last_circle;
    if (pid.actual_value < -20000) {
        pid.actual_value += 65535;
    }
    else if (pid.actual_value > 20000) {
        pid.actual_value -= 65535;
    }
    last_circle = TIM3->CNT;
    //set_duty(sp);
    set_duty (pid_update (&pid) * 1000 ) ;
}

void set_duty (int duty) {
    if (duty >= 0) {
        TIM_SetCompare1(TIM1, duty);
        TIM1->CCER &= 0xffef;
        TIM1->CCER |= 0x0001;
    }
    else {
        TIM_SetCompare2(TIM1, -duty);
        TIM1->CCER &= 0xfffe;
        TIM1->CCER |= 0x0010;
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    while(1);
}
#endif
