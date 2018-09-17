#include "main.h"

//extern WorkState_e workState;
//FeedMotor_t FeedMotor = {0,0,20,0,CLOCKWISE,STOP_SHOOT,STUCK_NO};
//void FeedMotor_Configuration(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5|GPIO_Pin_3; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOB, &GPIO_InitStructure); 
//}

//void Update_Motor_Speed()
//{
//	FeedMotor.encoder_last = FeedMotor.encoder_now;	
//	if(TIM1->CNT > 40000)TIM1->CNT = 2*One_Shoot_Encoder-(65536-TIM1->CNT);
//	FeedMotor.encoder_now = TIM1->CNT;
//	FeedMotor.motor_speed = FeedMotor.encoder_now-FeedMotor.encoder_last;
//	if(abs(FeedMotor.motor_speed) < FeedMotor.threshold_Speed)FeedMotor.stuck = STUCK_YES;
//	else FeedMotor.stuck = STUCK_NO;
//}
//void Toggle_Motor_Direction()
//{
//	if(FeedMotor.feedmotor_direction == CLOCKWISE)
//	{
//		Set_Motor_AntiClockwise();
//		FeedMotor.feedmotor_direction = ANTICLOCKWISE;
//	}
//	else 
//	{
//		Set_Motor_Clockwise();
//		FeedMotor.feedmotor_direction = CLOCKWISE;
//	}	
//}
//void Set_Motor_Clockwise()
//{
//	GPIO_SetBits(GPIOB,GPIO_Pin_5);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_3);	
//	FeedMotor.feedmotor_direction = CLOCKWISE;
//}
//void Set_Motor_AntiClockwise()
//{
//	GPIO_SetBits(GPIOB,GPIO_Pin_3);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//	FeedMotor.feedmotor_direction = ANTICLOCKWISE;
//}
//void Stop_Shooting()
//{
//	GPIO_SetBits(GPIOB,GPIO_Pin_5 | GPIO_Pin_3);
//	FeedMotor.feedmotor_state = STOP_SHOOT;
//}
//void One_Shoot()
//{
//	if(workState != NORMAL_STATE)
//	{
//		Stop_Shooting();
//	}
//	else 
//	{
//		if(FeedMotor.feedmotor_direction == CLOCKWISE)
//		{
//			Set_Motor_Clockwise();
//		}
//		else 
//		{
//			Set_Motor_AntiClockwise();
//		}
//		FeedMotor.feedmotor_state = ONE_SHOOT;
//	}
//}
//void Continuous_Shoot()
//{
//	if(workState != NORMAL_STATE)
//	{
//		Stop_Shooting();
//	}
//	else 
//	{
//		if(FeedMotor.feedmotor_direction == CLOCKWISE)
//		{
//			Set_Motor_Clockwise();
//		}
//		else 
//		{
//			Set_Motor_AntiClockwise();
//		}
//		FeedMotor.feedmotor_state = CONTINUOUS_SHOOT;
//  }
//}
//void Motor_Stuck_Spy()
//{
//	static uint8_t count = 0;
//	FeedMotor.encoder_last = FeedMotor.encoder_now;	//Update_Motor_Speed()
//	if(TIM1->CNT>40000)TIM1->CNT = 2*One_Shoot_Encoder-(65536-TIM1->CNT);
//	FeedMotor.encoder_now = TIM1->CNT;
//	FeedMotor.motor_speed = FeedMotor.encoder_now - FeedMotor.encoder_last;
//	if(abs(FeedMotor.motor_speed) < FeedMotor.threshold_Speed)FeedMotor.stuck = STUCK_YES;
//	else FeedMotor.stuck = STUCK_NO;

//	if(FeedMotor.feedmotor_state != STOP&&FeedMotor.stuck == STUCK_YES)
//	{
//		if(++count == 3)
//		
//		{
//			if(FeedMotor.feedmotor_direction == CLOCKWISE)//Toggle_Motor_Direction()
//			{
//				Set_Motor_AntiClockwise();
//			}
//			else 
//			{
//				Set_Motor_Clockwise();
//			}		
//			count = 0;
//		}
//	}
//	else count = 0;	
//}
