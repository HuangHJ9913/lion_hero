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
////�������񣬷���timer6 1ms��ʱ�ж���ִ��
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
//		GMPitchControlLoop();    //����pitch��Ŀ���Pid
//		GMYawControlLoop();     //����yaw��Ŀ���Pid
//	}		
//	SetGimbalMotorOutput();  //pitch��yaw�ĵ��ͳһ���
//    
//	//chassis motor control
//	if(time_tick_1ms % 4 == 0)         //motor control frequency 4ms
//	{
//		CMControlLoop();     //���̵����Pid������ͬ���
//		ShooterMControlLoop();  
//	}
//    
//    // ����ϵͳ���͹��ʰ�������Ϊ50Hz
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
//*����״̬�л�״̬��,��1ms��ʱ�ж�ͬƵ��
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
//	//���������ģʽ�л���prapareģʽ��Ҫ��һϵ�в�����ʼ��
//	if((lastWorkState != workState) && (workState == PREPARE_STATE))  
//	{
//		ControtLoopTaskInit();
//		rc_task_init();
//	}
//}

////���������ʼ������
//void ControtLoopTaskInit(void)
//{
//	//������ʼ��
//	time_tick_1ms = 0;   //�ж��еļ�������
//	//���ù���ģʽ
//	SetWorkState(PREPARE_STATE);
//    //��̨�����Ƕȳ�ʼ��
//	GimbalRef.pitch_angle_dynamic_ref = 0.0f;
//	GimbalRef.yaw_angle_dynamic_ref = 0.0f;
//	//PID��ʼ��
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
//	//�����ֵ������
//	FeedMotor_Configuration();
//}
