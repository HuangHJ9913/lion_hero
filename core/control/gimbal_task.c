#include "gimbal_task.h"

volatile gimbal_t gimbal;

extern volatile encoder_t gimbal_encoder[GIMBAL_JOINT_TOTAL_NUM];

void gimbal_init()
{
    memset((gimbal_t *)&gimbal, 0, sizeof(gimbal_t));
    
    gimbal.joint[YAW].angle_max   = GIMBAL_JOINT_YAW_MAX_ANGLE;
    gimbal.joint[YAW].angle_min   = GIMBAL_JOINT_YAW_MIN_ANGLE;
    gimbal.joint[YAW].speed_max   = GIMBAL_JOINT_YAW_MAX_SPEED;
    gimbal.joint[YAW].sensor      = GIMBAL_JOINT_YAW_SENSOR;
    
    gimbal.joint[PITCH].angle_max = GIMBAL_JOINT_PITCH_MAX_ANGLE;
    gimbal.joint[PITCH].angle_min = GIMBAL_JOINT_PITCH_MIN_ANGLE;
    gimbal.joint[PITCH].speed_max = GIMBAL_JOINT_PITCH_MAX_SPEED;
    gimbal.joint[PITCH].sensor    = GIMBAL_JOINT_PITCH_SENSOR;
    
    PID_struct_init(&pid_yaw_position,   POSITION_PID, GIMBAL_JOINT_YAW_MAX_SPEED,     2500, 1, 0, 0);
    PID_struct_init(&pid_yaw_speed,      POSITION_PID, GIMBAL_JOINT_YAW_MAX_CURRENT,   2500, 1, 0, 0);
    PID_struct_init(&pid_pitch_position, POSITION_PID, GIMBAL_JOINT_PITCH_MAX_SPEED,   2500, 1, 0, 0);
    PID_struct_init(&pid_pitch_speed,    POSITION_PID, GIMBAL_JOINT_PITCH_MAX_CURRENT, 2500, 1, 0, 0);
}

void gimbal_get_sensor_msg_handler()
{
    for (uint8_t id = 0; id < GIMBAL_JOINT_TOTAL_NUM; id++)
    {
        switch (gimbal.joint[id].sensor)
        {
            case mpu6050:
                gimbal.joint[id].angle_get = imu.angle_z;
                gimbal.joint[id].speed_get = imu.angle_velocity_z;
                break;
            default:
                gimbal.joint[id].angle_get = gimbal_encoder[id].total_angle;
                gimbal.joint[id].speed_get = gimbal_encoder[id].filter_rpm;
            break;
        }
    }
}

void gimbal_control()
{
    switch (gimbal.control_mode)
    {
        case PC_MODE:
            gimbal.joint[YAW].speed_set   = pc_gimbal_download.yaw.speed_set;
            gimbal.joint[PITCH].speed_set = pc_gimbal_download.pitch.speed_set;
            break;
        default: 
            gimbal.joint[YAW].speed_set   = rc_ctrl.yaw;
            gimbal.joint[PITCH].speed_set = rc_ctrl.pitch;
        break;
    }
    
    gimbal_get_sensor_msg_handler();
    
#ifdef GIMBAL_YAW_POSITION_LOOP_CONTROL
    gimbal.joint[YAW].speed_get   = pid_calc(&pid_yaw_position,   gimbal.joint[YAW].angle_get,   gimbal.joint[YAW].angle_set);
#endif
    
#ifdef GIMBAL_PITCH_POSITION_LOOP_CONTROL
    gimbal.joint[PITCH].speed_get = pid_calc(&pid_pitch_position, gimbal.joint[PITCH].angle_get, gimbal.joint[PITCH].angle_set);
#endif
    
    gimbal.joint[YAW].current   = pid_calc(&pid_yaw_speed,   gimbal.joint[YAW].speed_get,   gimbal.joint[YAW].speed_set);
    gimbal.joint[PITCH].current = pid_calc(&pid_pitch_speed, gimbal.joint[PITCH].speed_get, gimbal.joint[PITCH].speed_set);
    
    set_gimbal_current(gimbal.joint[YAW].current, gimbal.joint[PITCH].current);
}
