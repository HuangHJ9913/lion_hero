#include "cylinder.h"

void Cylinder_GPIO_Configuration(void){
		GPIO_InitTypeDef gpio;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);// A4-7 C4-5
	
		gpio.GPIO_Pin = ENB_IN3|ENB_IN4;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
		gpio.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOE, &gpio);
}
void Cylinder_Open(void){
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);
}
void Cylinder_Close(void){
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
		GPIO_SetBits(GPIOE,GPIO_Pin_4);
}
