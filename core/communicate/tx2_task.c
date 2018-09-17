#include "tx2_task.h"

volatile pc_robot_status_t pc_robot_status;
volatile pc_chassis_upload_t pc_chassis_upload;
volatile pc_gimbal_upload_t pc_gimbal_upload;
volatile pc_custom_upload_t pc_custom_upload;

volatile pc_chassis_download_t pc_chassis_download;
volatile pc_gimbal_download_t pc_gimbal_download;
volatile pc_custom_download_t pc_custom_download;


// 回传信息接收Buffer
uint8_t pc_receive_buffer[PC_MSG_MAX_LENGTH + 1];

// Buffer当前正在压入数据的index，压入完成后index自增1
uint8_t pc_buffer_index = 0;

// 数据包数据长度
uint16_t pc_frame_length = PC_MSG_MAX_LENGTH;

// 数据包Data段数据长度
uint16_t pc_data_segment_length = 0;

// 数据包帧头校验Flag，校验通过为1，校验不过为0
uint8_t pc_frame_header_available = 0;

// 数据包全帧校验Flag, 校验通过为1，校验不过为0
uint8_t pc_frame_total_available = 0;

// 上传到机载电脑的数据包序号
uint8_t pc_seq = 0;

void pc_task_init()
{
    // bsp
    USART3_Configuration();
    
    memset((pc_robot_status_t *)    &pc_robot_status,    0, sizeof(pc_robot_status_t));
    memset((pc_chassis_upload_t *)   &pc_chassis_upload,   0, sizeof(pc_chassis_upload_t));
    memset((pc_gimbal_upload_t *)   &pc_gimbal_upload,   0, sizeof(pc_gimbal_upload_t));
    memset((pc_custom_upload_t *)   &pc_custom_upload,   0, sizeof(pc_custom_upload_t));
    
    memset((pc_chassis_download_t *) &pc_chassis_download, 0, sizeof(pc_chassis_download_t));
    memset((pc_gimbal_download_t *) &pc_gimbal_download, 0, sizeof(pc_gimbal_download_t));
    memset((pc_custom_download_t *) &pc_custom_download, 0, sizeof(pc_custom_download_t));
}

/**
 *  机载电脑 下载信息处理函数
 */
void pc_download_msg_process(uint8_t msg)
{
    // 遇到帧头PC_MSG_SOF，初始化buffer
    if(msg == PC_MSG_SOF)
    {
        pc_buffer_index = 0;
        pc_frame_header_available = 0;
        pc_frame_total_available = 0;
        pc_frame_length = PC_MSG_MAX_LENGTH;
    }
    
    // 接收信息直到一帧结束
    if (pc_buffer_index < PC_MSG_MAX_LENGTH && pc_buffer_index < pc_frame_length)
        pc_receive_buffer[pc_buffer_index++] = msg;
    
    // 接收完帧头信息，校验帧头，只有校验通过才获取帧长信息
    if (pc_buffer_index == PC_CMD_ID_SEGMENT_OFFSET)
    {
        if (verify_crc8_check_sum(pc_receive_buffer, PC_CMD_ID_SEGMENT_OFFSET))
            pc_frame_header_available = 1;
        else
        {
            pc_frame_header_available = 0;
            return;
        }
        
        if (pc_frame_header_available)
        {
            pc_data_segment_length = (pc_receive_buffer[2] << 8) | (pc_receive_buffer[1] & 0xff);
            pc_frame_length = pc_data_segment_length + PC_DATA_SEGMENT_OFFSET + 2;
        }
    }
    
    // 接收完全帧信息，校验全帧，只有帧头和全帧校验都通过才对数据帧进行解包
    if (pc_frame_header_available && pc_buffer_index >= pc_frame_length)
    {
        if (verify_crc16_check_sum(pc_receive_buffer, pc_frame_length))
            pc_frame_total_available = 1;
        else
        {
            pc_frame_total_available = 0;
            return;
        }
        
        if (pc_frame_total_available)
            pc_msg_upack_handler(pc_receive_buffer);
    }
}

/**
 *  机载电脑 上传信息处理函数
 */
void pc_upload_msg_process(float d1, float d2, float d3, float d4)
{
    pc_upload_data_frame_u uframe;
    pc_upload_frame_head_u frame_head_u;
    
    // 累加包序号
    if (pc_seq++ >= 0xff) pc_seq = 0;
    
    // 组织帧头FrameHead 
    frame_head_u.frame_head.sof = PC_MSG_SOF;
    frame_head_u.frame_head.dataLength = sizeof(uframe.tframe.data);
    frame_head_u.frame_head.seq = pc_seq;
    append_crc8_check_sum(frame_head_u.tframeHeadCh, sizeof(frame_head_u.tframeHeadCh));
    uframe.tframe.frame_head_u = frame_head_u;
    
    // 设置命令码CmdID为0x0100
    uframe.tframe.CmdID = PC_CUSTOM_UPLOAD_CMD_ID;
    
    // Data
    uframe.tframe.data.data1 = d1;
    uframe.tframe.data.data2 = d2;
    uframe.tframe.data.data3 = d3;
    uframe.tframe.data.data3 = d4;
    
    // 封装帧尾FrameTail
    append_crc16_check_sum(uframe.tframeCh, sizeof(uframe.tframeCh));
    
    // 通过USART3发送结构体
    USART3_PrintBlock(uframe.tframeCh, sizeof(uframe.tframeCh));
}

/**
 *  机载电脑 解包函数
 */
void pc_msg_upack_handler(uint8_t * buffer)
{
    switch (buffer[PC_CMD_ID_SEGMENT_OFFSET])
    {
        case PC_CHASSIS_DOWNLOAD_CMD_ID:
            memcpy((pc_chassis_download_t *)&pc_chassis_download, 
                    buffer + PC_DATA_SEGMENT_OFFSET, 
                    pc_data_segment_length);
        break;
        
        case PC_GIMBAL_DOWNLOAD_CMD_ID:
                    memcpy((pc_gimbal_download_t *)&pc_gimbal_download, 
                            buffer + PC_DATA_SEGMENT_OFFSET, 
                            pc_data_segment_length);
        break;
        
        case PC_CUSTOM_DOWNLOAD_CMD_ID:
                    memcpy((pc_custom_download_t *)&pc_custom_download, 
                            buffer + PC_DATA_SEGMENT_OFFSET, 
                            pc_data_segment_length);
        break;
        
        default: break;
    }
}
