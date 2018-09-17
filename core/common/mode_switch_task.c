#include "main.h"
//#define KI_DEFAULT  2.5f

///*--------------------------------------------CTRL Variables----------------------------------------*/
//WorkState_e lastWorkState = PREPARE_STATE;
//WorkState_e workState = PREPARE_STATE;
//	
///*
//*********************************************************************************************************
//*                                            FUNCTIONS 
//*********************************************************************************************************
//*/

//static void SetWorkState(WorkState_e state)
//{
//    workState = state;
//}


//WorkState_e GetWorkState()
//{
//	return workState;
//}


//static uint32_t time_tick_1ms = 0;
////控制任务，放在timer6 1ms定时中断中执行
//void Control_Task(void)
//{
//	static uint8_t a = 0;
//	if(++a == 50)
//	{
//		Motor_Stuck_Spy();
//		a = 0;
//	}
//	time_tick_1ms++;
//	WorkStateFSM();
//	WorkStateSwitchProcess();
//	if(time_tick_1ms > 1000)
//	{
//		GMPitchControlLoop();    //计算pitch轴的控制Pid
//		GMYawControlLoop();     //计算yaw轴的控制Pid
//	}		
//	SetGimbalMotorOutput();  //pitch和yaw的电机统一输出
//    
//	//chassis motor control
//	if(time_tick_1ms % 4 == 0)         //motor control frequency 4ms
//	{
//		CMControlLoop();     //底盘电机的Pid计算连同输出
//		ShooterMControlLoop();  
//	}
//    
//    // 裁判系统发送功率包的速率为50Hz
////    if(time_tick_1ms % 20 == 0)
////    {
////        if (power_heat_data.chassisPowerBuffer >= MAX_POWER_BUFFER)
////        {
////            power_limit = 1.f;
////        }
////        else
////        {
////            power_limit = power_heat_data.chassisPowerBuffer / MAX_POWER_BUFFER;
////        }
////    }
//}


///**********************************************************
//*工作状态切换状态机,与1ms定时中断同频率
//**********************************************************/

//void WorkStateFSM(void)
//{
//	lastWorkState = workState;
//	switch(workState)
//	{
//		case PREPARE_STATE:
//		{
//			if(GetInputMode() == STOP) //|| Is_Serious_Error())
//			{
//				workState = STOP_STATE;
//			}
//			else if(time_tick_1ms > PREPARE_TIME_TICK_MS)
//			{
//				workState = NORMAL_STATE;
//			}			
//		}break;
//		case NORMAL_STATE:     
//		{
//			if(GetInputMode() == STOP) //|| Is_Serious_Error())
//			{
//				workState = STOP_STATE;
//			}
//			
//		}break;
//		case STANDBY_STATE:     
//		{
//			if(GetInputMode() == STOP) //|| Is_Serious_Error())
//			{
//				workState = STOP_STATE;
//			}
//			else if(IsRemoteBeingAction() || (GetShootState()==SHOOTING) || GetFrictionState() == FRICTION_WHEEL_START_TURNNING)
//			{
//				workState = NORMAL_STATE;
//			}				
//		}break;
//		case STOP_STATE:   
//		{
//			if(GetInputMode() != STOP) //&& !Is_Serious_Error())
//			{
//				workState = PREPARE_STATE;   
//			}
//		}break;  
//		default:
//		{
//			
//		}
//	}	
//}

//static void WorkStateSwitchProcess(void)
//{
//	//如果从其他模式切换到prapare模式，要将一系列参数初始化
//	if((lastWorkState != workState) && (workState == PREPARE_STATE))  
//	{
//		ControtLoopTaskInit();
//		rc_task_init();
//	}
//}

////控制任务初始化程序
//void ControtLoopTaskInit(void)
//{
//	//计数初始化
//	time_tick_1ms = 0;   //中断中的计数清零
//	//设置工作模式
//	SetWorkState(PREPARE_STATE);
//    //云台给定角度初始化
//	GimbalRef.pitch_angle_dynamic_ref = 0.0f;
//	GimbalRef.yaw_angle_dynamic_ref = 0.0f;
//	//PID初始化
////	ShootMotorSpeedPID.Reset(&ShootMotorSpeedPID);
//	GMPPositionPID.Reset(&GMPPositionPID);
//	GMPSpeedPID.Reset(&GMPSpeedPID);
//	GMYPositionPID.Reset(&GMYPositionPID);
//	GMYSpeedPID.Reset(&GMYSpeedPID);
//	CMRotatePID.Reset(&CMRotatePID);
//	CM1SpeedPID.Reset(&CM1SpeedPID);
//	CM2SpeedPID.Reset(&CM2SpeedPID);
//	CM3SpeedPID.Reset(&CM3SpeedPID);
//	CM4SpeedPID.Reset(&CM4SpeedPID);
//	//拨弹轮电机配置
//	FeedMotor_Configuration();
//}
