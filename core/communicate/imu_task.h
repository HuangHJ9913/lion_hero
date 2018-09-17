#ifndef _IMU_TASK_H_
#define _IMU_TASK_H_

#include "main.h"

#define IMU_MSG_MAX_LENGTH          11u
#define IMU_MSG_SOF                 0x55
#define IMU_CMD_ID_SEGMENT_OFFSET   0x01

#define IMU_ACCEL_CMD_ID            0x51
#define IMU_ANGLE_VELOCITY_CMD_ID    0x52
#define IMU_ANGLE_CMD_ID              0x53

typedef struct {
    float acc_x;
    float acc_y;
    float acc_z;
    float angle_velocity_x;
    float angle_velocity_y;
    float angle_velocity_z;
    float angle_x;
    float angle_y;
    float angle_z;
} IMU_t;

extern volatile IMU_t imu;

void imu_task_init(void);
void imu_download_msg_process(uint8_t msg);
void imu_msg_unpack_handler(uint8_t * buffer);

#endif
