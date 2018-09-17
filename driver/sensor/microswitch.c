#include "main.h"

void Microswitch_Configuration()
{
    GPIO_InitTypeDef gpio;  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	  gpio.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;
	  gpio.GPIO_Mode = GPIO_Mode_IN;
	  gpio.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOD,&gpio);
}
uint8_t get_microswitch_nadir(uint8_t microswitch)
{
	switch (microswitch)
	{
		case RF:
			 	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15))
					return 1;		
				else
					return 0;
		case LF:
			 	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14))
					return 1;		
				else
					return 0;
		case LR:
			 	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13))
					return 1;		
				else
					return 0;
		case RR:
			 	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12))
					return 1;		
				else
					return 0;
	}
	return 0;
}

uint8_t get_microswitch_peak(uint8_t microswitch)
{
	switch (microswitch)
	{
		case RF:
			 	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15))
					return 1;		
				else
					return 0;
		case LF:
			 	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14))
					return 1;		
				else
					return 0;
		case LR:
			 	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13))
					return 1;		
				else
					return 0;
		case RR:
			 	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12))
					return 1;		
				else
					return 0;
	}
	return 0;
}
