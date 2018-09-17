#include "main.h"

void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    tim.TIM_Prescaler                       =   84 - 1;
    tim.TIM_Period                          =   1000;
    tim.TIM_CounterMode                     =   TIM_CounterMode_Up;
    tim.TIM_ClockDivision                   =   TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM6, &tim);
    
    nvic.NVIC_IRQChannel                    =   TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   0;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(TIM6, ENABLE);
    
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
}

uint32_t cnt_500ms = 0;
int8_t flag = 1;
uint16_t temp = 500;
extern volatile encoder_t chassis_encoder[CHASSIS_WHEEL_TOTAL_NUM];

void print_rc()
{
    printf("ch1:%d, ch2:%d, ch3:%d, ch4:%d\n", rc_msg.ch1, rc_msg.ch2, rc_msg.ch3, rc_msg.ch4);
}

void print_chassis_rpm()
{
//    printf("%d, %d, %d, %d, %d, %d, %d\n", 
    printf("x_set:%d, y_set:%d, z_set:%d, rf_s:%d, lf_s:%d, lr_s:%d, rr_s%d, rf_g:%d, lf_g:%d, lr_g:%d, rr_g%d\n", 
            chassis.speed.x_set,
            chassis.speed.y_set,
            chassis.speed.z_set,
            chassis.speed.rpm_set[RF],
            chassis.speed.rpm_set[LF],
            chassis.speed.rpm_set[LR], 
            chassis.speed.rpm_set[RR],
            chassis.speed.rpm_get[RF],
            chassis.speed.rpm_get[LF],
            chassis.speed.rpm_get[LR], 
            chassis.speed.rpm_get[RR]);
}

void TIM6_DAC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        TIM_ClearFlag(TIM6, TIM_FLAG_Update);
        
        cnt_500ms++;
        
        if (cnt_500ms % 10 == 0)
        {
//            gimbal_control();
            chassis_control();
					  lift_control();
						shoot_control();
        }
        
        if (cnt_500ms % 100 == 0)
        {
            print_chassis_rpm();
//            print_rc();
            
            cnt_500ms = 0;
        }
        
    }
}
