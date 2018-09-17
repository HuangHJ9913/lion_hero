#include "stm32f4xx.h"
#include "servo.h"

void Servo_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_TIM9);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM9); 

		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;  
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInitStructure.TIM_Period = 20000; 
		TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);

		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 500;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

		TIM_OC1Init(TIM9,&TIM_OCInitStructure);
		TIM_OC2Init(TIM9,&TIM_OCInitStructure);

		TIM_Cmd(TIM9,ENABLE);
		TIM_CtrlPWMOutputs(TIM9,ENABLE);
	
	
}
void set_servo_angle(uint8_t motor_id,float angle)
{
		if(motor_id==2)TIM9->CCR2=500+(uint16_t)((angle)*(2000/180));
		else if(motor_id==1)TIM9->CCR1=500+(uint16_t)((angle)*(2000/180));
		//if((motor_id==0||motor_id==1)&&(angle>=0&&angle<=180))*((&TIM9->CCR1)+motor_id*4)=500+(angle)*2000/180;
}
void servo_forward()
{
		set_servo_angle(1,10);
		set_servo_angle(2,10);
}

void servo_backward()
{
		set_servo_angle(1,100);
		set_servo_angle(2,100);
}
