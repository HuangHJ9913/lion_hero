#ifndef _MODE_SWITCH_TASK_H_
#define _MODE_SWITCH_TASK_H_

#include "main.h"

////initiate status: 
//typedef enum
//{
//    PREPARE_STATE,     		//�ϵ���ʼ��״̬ 4s������
//    STANDBY_STATE,			//��ֹ̨ͣ��ת״̬
//    NORMAL_STATE,			//������״̬
//    STOP_STATE,        	//ֹͣ�˶�״̬
//    CALI_STATE,    			//У׼״̬
//}WorkState_e;

//typedef __packed struct
//{
//	int8_t pid_type;		// position PID
//	int8_t motor_type;   //motor type ie: pitch yaw 201 202 203 204	
//	int16_t kp_offset;
//	int16_t ki_offset;
//	int16_t kd_offset;
//}PIDParamStruct_t;

//#define INIT_ANGLE_REF   4.3f
//#define PREPARE_TIME_TICK_MS 4000      //prapare time in ms
//#define STATE_SWITCH_DELAY_TICK 100000   //mode change delay count in ms


//WorkState_e GetWorkState(void);
//void ControtLoopTaskInit(void);
//void WorkStateFSM(void);
//static void WorkStateSwitchProcess(void);
//void Control_Task(void);

#endif
