#ifndef _GIMBAL_TASK_H_
#define _GIMBAL_TASK_H_

#include "main.h"

#define GIMBAL_JOINT_TOTAL_NUM          2
#define GIMBAL_JOINT_YAW_ID             0
#define GIMBAL_JOINT_PITCH_ID           1
#define YAW                             GIMBAL_JOINT_YAW_ID
#define PITCH                           GIMBAL_JOINT_PITCH_ID

#define GIMBAL_JOINT_YAW_MAX_ANGLE      20
#define GIMBAL_JOINT_YAW_MIN_ANGLE      -20
#define GIMBAL_JOINT_YAW_MAX_SPEED      720
#define GIMBAL_JOINT_YAW_MAX_CURRENT    5000
#define GIMBAL_JOINT_PITCH_MAX_ANGLE    20
#define GIMBAL_JOINT_PITCH_MIN_ANGLE    -20
#define GIMBAL_JOINT_PITCH_MAX_SPEED    400
#define GIMBAL_JOINT_PITCH_MAX_CURRENT  5000

#define GIMBAL_JOINT_YAW_SENSOR         encoder
#define GIMBAL_JOINT_PITCH_SENSOR       encoder

// 调试状态时可注释掉如下define来整定YAW和PITCH的速度环PID参数
//#define GIMBAL_YAW_POSITION_LOOP_CONTROL
//#define GIMBAL_PITCH_POSITION_LOOP_CONTROL

typedef struct {
    uint16_t gimbal_offset_x;      // (mm) 云台中心到底盘正中心的距离在x轴上的分量
    uint16_t gimbal_offset_y;      // (mm) 云台中心到底盘正中心的距离在y轴上的分量
} gimbal_info_t;

typedef enum {
    mpu6050,
    encoder,
} joint_sensor_e;

typedef struct {
    int16_t        angle_set;
    int16_t        angle_get;
    int16_t        angle_max;
    int16_t        angle_min;
    
    int16_t        speed_set;
    int16_t        speed_get;
    int16_t        speed_max;
    int16_t        speed_limit;
    
    float          current;
    
    joint_sensor_e sensor;
} gimbal_joint_t;

typedef enum {
    RC_MODE,
    PC_MODE,
} gimbal_mode_e;

typedef struct {
    gimbal_info_t       info;
    gimbal_joint_t      joint[GIMBAL_JOINT_TOTAL_NUM];
    
    gimbal_mode_e       control_mode;
} gimbal_t;

extern volatile gimbal_t gimbal;

void gimbal_init(void);
void gimbal_control(void);

#endif
