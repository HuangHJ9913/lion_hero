#include "led.h"


void LED_Configuration(void)
{
		GPIO_InitTypeDef gpioInitStruct;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		gpioInitStruct.GPIO_Pin = GPIO_Pin_9;
		gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
		gpioInitStruct.GPIO_OType = GPIO_OType_PP;
		gpioInitStruct.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOB, &gpioInitStruct);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void led_turn_on(void)
{
	 GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}
void led_turn_off(void)
{
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void led_blink(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	for(int i=0;i<100;i++)
		for(int ii=0;ii<100;ii++);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	for(int i=0;i<100;i++)
		for(int ii=0;ii<100;ii++);
}
