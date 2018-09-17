#include "imu_task.h"

volatile IMU_t imu = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// 回传信息接收buffer
static uint8_t receive_buffer[IMU_MSG_MAX_LENGTH + 1];

// buffer当前正在压入数据的index，压入完成后index自增1
static uint8_t buffer_index = 0;


void imu_task_init()
{
    // bsp
    USART6_Configuration();
    
    memset((IMU_t *)&imu, 0, sizeof(IMU_t));
}

/**
 *  MPU6050 下载信息处理函数
 */
void imu_download_msg_process(uint8_t msg)
{
    // 遇到帧头，初始化buffer
    if(msg == IMU_MSG_SOF)
        buffer_index = 0;
    
    if (buffer_index < IMU_MSG_MAX_LENGTH)
        receive_buffer[buffer_index++] = msg;
    
    // 接收完一帧
    if(buffer_index == IMU_MSG_MAX_LENGTH)
        imu_msg_unpack_handler(receive_buffer);
}

/**
 *  MPU6050 解包函数
 */
void imu_msg_unpack_handler(uint8_t * buffer)
{
    switch(buffer[IMU_CMD_ID_SEGMENT_OFFSET])
    {
        case IMU_ACCEL_CMD_ID:
            imu.acc_x = ((short)(buffer[3]<<8 | buffer[2])) / 32768.f * 16.f * 9.8f;    // g
            imu.acc_y = ((short)(buffer[5]<<8 | buffer[4])) / 32768.f * 16.f * 9.8f;    // g
            imu.acc_z = ((short)(buffer[7]<<8 | buffer[6])) / 32768.f * 16.f * 9.8f;    // g
            break;
        case IMU_ANGLE_VELOCITY_CMD_ID:
            imu.angle_velocity_x = ((short)(buffer[3]<<8 | buffer[2])) / 32768.f * 2000.f;        // deg/sec
            imu.angle_velocity_y = ((short)(buffer[5]<<8 | buffer[4])) / 32768.f * 2000.f;        // deg/sec
            imu.angle_velocity_z = ((short)(buffer[7]<<8 | buffer[6])) / 32768.f * 2000.f;        // deg/sec
            break;
        case IMU_ANGLE_CMD_ID:
            imu.angle_x = ((short)(buffer[3]<<8 | buffer[2])) / 32768.f * 180.f;          // deg
            imu.angle_y = ((short)(buffer[5]<<8 | buffer[4])) / 32768.f * 180.f;          // deg
            imu.angle_z = ((short)(buffer[7]<<8 | buffer[6])) / 32768.f * 180.f;          // deg
            break;
    }
}
