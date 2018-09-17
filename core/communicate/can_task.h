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
	int32_t raw_value;                // 编码器不经处理的原始值
	int32_t last_raw_value;           // 上一次的编码器原始值
	int32_t ecd_value;                // 经过处理后连续的编码器值
	int32_t diff;                     // 两次编码器之间的差值
	int32_t temp_count;               // 计数用
	uint8_t buf_count;                // 滤波更新buf用
	int32_t ecd_bias;                 // 初始编码器值	
	int32_t ecd_raw_rate;             // 通过编码器计算得到的速度原始值
	int32_t rate_buf[RATE_BUF_SIZE];  // buf，for filter
	int32_t round_cnt;                // 圈数
	float filter_rpm;                 // 速度
	float local_angle;                // 角度
	float total_angle;                // 角度
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

