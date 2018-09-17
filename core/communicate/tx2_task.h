#ifndef _TX2_TASK_H_
#define _TX2_TASK_H_

#include "main.h"

#define PC_MSG_MAX_LENGTH               44u
#define PC_MSG_SOF                      0xFF
#define PC_CMD_ID_SEGMENT_OFFSET        0x05
#define PC_DATA_SEGMENT_OFFSET          0x07

#define PC_ROBOT_STATE_CMD_ID           0x0001
#define PC_CHASSIS_UPLOAD_CMD_ID         0x0002
#define PC_CHASSIS_DOWNLOAD_CMD_ID       0x0003
#define PC_GIMBAL_UPLOAD_CMD_ID         0x0004
#define PC_GIMBAL_DOWNLOAD_CMD_ID       0x0005
#define PC_CUSTOM_UPLOAD_CMD_ID         0x0020
#define PC_CUSTOM_DOWNLOAD_CMD_ID       0x0021

typedef __packed struct
{
    // error_list
    int16_t error_list;
} pc_robot_status_t;

typedef __packed struct
{
    __packed struct {
        int16_t x_get;
        int16_t x_max;
        int16_t x_limit;
        
        int16_t y_get;
        int16_t y_max;
        int16_t y_limit;
        
        int16_t z_get;
        int16_t z_max;
        int16_t z_limit;
    } speed;
} pc_chassis_upload_t;

typedef __packed struct
{
    __packed struct {
        int16_t x_set;
        int16_t y_set;
        int16_t z_set;
    } speed;
} pc_chassis_download_t;

typedef __packed struct
{
    __packed struct {
        int16_t angle_get;
        int16_t speed_get;
    } yaw;
    
    __packed struct {
        int16_t angle_get;
        int16_t speed_get;
    } pitch;
} pc_gimbal_upload_t;

typedef __packed struct
{
    __packed struct {
        int16_t angle_set;
        int16_t speed_set;
    } yaw;
    
    __packed struct {
        int16_t angle_set;
        int16_t speed_set;
    } pitch;
} pc_gimbal_download_t;

typedef __packed struct
{
    float data1;
    float data2;
    float data3;
    float data4;
} pc_custom_upload_t;

typedef __packed struct
{
    float data1;
    float data2;
    float data3;
    float data4;
} pc_custom_download_t;

typedef __packed struct
{
    uint8_t sof;
    uint16_t dataLength;
    uint8_t seq;
    uint8_t crc8;
}pc_upload_frame_head_t;

typedef __packed union
{
    pc_upload_frame_head_t frame_head;
    uint8_t tframeHeadCh[sizeof(pc_upload_frame_head_t)];
}pc_upload_frame_head_u;

typedef __packed struct
{
    pc_upload_frame_head_u frame_head_u;
    uint16_t CmdID;
    pc_custom_upload_t data;
    uint16_t frameTail;
}pc_upload_data_frame_t;

typedef __packed union
{
    pc_upload_data_frame_t tframe;
    uint8_t tframeCh[sizeof(pc_upload_data_frame_t)];
} pc_upload_data_frame_u;

extern volatile pc_chassis_download_t pc_chassis_download;
extern volatile pc_gimbal_download_t pc_gimbal_download;
extern volatile pc_custom_download_t pc_custom_download;

void pc_task_init(void);
void pc_download_msg_process(uint8_t);
void pc_upload_msg_process(float data1, float data2, float data3, float d4);
void pc_msg_upack_handler(uint8_t * buffer);

#endif
