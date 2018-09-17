#include "stm32f4xx.h"
#include "gun.h"


void TIM5_Configuration(void)
{
    GPIO_InitTypeDef            gpio;
    TIM_TimeBaseInitTypeDef     tim;
    TIM_OCInitTypeDef           oc;

    // 时钟总线
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    
    // GPIO配置
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

    gpio.GPIO_Pin           =   GPIO_Pin_0 | GPIO_Pin_1 ;
    gpio.GPIO_Mode          =   GPIO_Mode_AF;
    gpio.GPIO_Speed         =   GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &gpio);
    
    
    // 时钟基本配置
    tim.TIM_Period          =   2500;   //2.5ms
    tim.TIM_Prescaler       =   84 - 1; // 1MHz (1us)
    tim.TIM_CounterMode     =   TIM_CounterMode_Up;
    tim.TIM_ClockDivision   =   TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM5, &tim); 
    
    // PWM配置
    oc.TIM_Pulse            =   2500;
    oc.TIM_OCMode           =   TIM_OCMode_PWM2;
    oc.TIM_OutputState      =   TIM_OutputState_Enable;
    oc.TIM_OutputNState     =   TIM_OutputState_Disable;
    oc.TIM_OCPolarity       =   TIM_OCPolarity_Low;
    oc.TIM_OCNPolarity      =   TIM_OCPolarity_High;
    oc.TIM_OCIdleState      =   TIM_OCIdleState_Reset;
    oc.TIM_OCNIdleState     =   TIM_OCIdleState_Set;
    TIM_OC1Init(TIM5, &oc);
    TIM_OC2Init(TIM5, &oc);

    TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
		
    TIM_ARRPreloadConfig(TIM5, ENABLE);
    // 开启时钟
    TIM_Cmd(TIM5, ENABLE);   
    TIM_CtrlPWMOutputs(TIM5, ENABLE);

}

void Gun_Configuration(void)
{
		TIM5_Configuration();
		InitFrictionWheel();
}
