#include "main.h"
   
void BSP_Init(void)
{
//	Led_Configuration();
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     
//	PWM_Configuration(); 
//	TIM1_Configuration();
//	Quad_Encoder_Configuration();
	USART2_Configuration();
	USART6_Configuration();
//	TIM6_Start();
//	Encoder_Start();
	delay_ms(800);   //Ħ���ֵ��Ҫ��
}

