#include  <stm32f4xx.h>	 
#include <stm32f4xx_conf.h>

int main()
{
	GPIO_InitTypeDef gpio;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&gpio);

	GPIO_ResetBits(GPIOB,GPIO_Pin_14|GPIO_Pin_15);
	
	while(1)
	{
		
	}
}
