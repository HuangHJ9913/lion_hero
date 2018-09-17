#ifndef _CAN_TASK_H_
#define _CAN_TASK_H_

#include "main.h"

#define CAN_CHASSIS_RIGHT_FORWARD_FEEDBACK_MSG_ID    0x201
#define CAN_CHASSIS_LEFT_FORWARD_FEEDBACK_MSG_ID     0x202 
#define CAN_CHASSIS_LEFT_REAR_FEEDBACK_MSG_ID        0x203
#define CAN_CHASSIS_RIGHT_REAR_FEEDBACK_MSG_ID       0x204
#define CAN_GIMBAL_YAW_FEEDBACK_MSG_ID              0x205
#define CAN_GIMBAL_PITCH_FEEDBACK_MSG_ID            0x206

#define CAN_PORT_LIFT                                CAN2
#define CAN_PORT_GUN                                 CAN2
#define CAN_PORT_CHASSIS                             CAN1
#define CAN_PORT_GIMBAL                              CAN1

#define ENCODER_RPM_COEFFICIENT                     7.32f // = 1000.f * 60.f / 8192.f

#define RATE_BUF_SIZE 6

typedef struct{
	int32_t raw_value;                // ���������������ԭʼֵ
	int32_t last_raw_value;           // ��һ�εı�����ԭʼֵ
	int32_t ecd_value;                // ��������������ı�����ֵ
	int32_t diff;                     // ���α�����֮��Ĳ�ֵ
	int32_t temp_count;               // ������
	uint8_t buf_count;                // �˲�����buf��
	int32_t ecd_bias;                 // ��ʼ������ֵ	
	int32_t ecd_raw_rate;             // ͨ������������õ����ٶ�ԭʼֵ
	int32_t rate_buf[RATE_BUF_SIZE];  // buf��for filter
	int32_t round_cnt;                // Ȧ��
	float filter_rpm;                 // �ٶ�
	float local_angle;                // �Ƕ�
	float total_angle;                // �Ƕ�
}encoder_t;

void can_task_init(void);
void get_encoder_bias(volatile encoder_t *v, CanRxMsg * msg);
void encoder_process_handler(volatile encoder_t *v, CanRxMsg * msg);
void can1_receive_msg_process(CanRxMsg * msg);
void can2_receive_msg_process(CanRxMsg * msg);

void set_gimbal_current(int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq);
void set_chassis_current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);
void set_lift_current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);
void set_gun_current(int16_t gun_feed_iq);
#endif

