#include "js_task.h"

volatile extGameRobotState_t game_robot_state;
volatile extRobotHurt_t robot_hurt;
volatile extShootData_t shoot_data;
volatile extPowerHeatData_t power_heat_data;
volatile extRfidDetect_t rfid_detect;
volatile extGameResult_t game_result;
volatile extGetBuff_t get_buff;
volatile extShowData_t show_data;

/**
 * 由于裁判系统回传的信息存在丢帧、一帧分段发的情况，
 * 所以需要先对接收到的信息进行整合，完成数据接收并且校验通过后才进行解码
 */

// 回传信息接收Buffer
static uint8_t receive_buffer[JS_MSG_MAX_LENGTH + 1];

// Buffer当前正在压入数据的index，压入完成后index自增1
static uint8_t buffer_index = 0;

// 数据包Data段数据长度，JS_MSG_MAX_LENGTH=44为RM2017裁判系统通信协议中最长可能的全包数据长度
uint16_t frame_length = JS_MSG_MAX_LENGTH;

// 数据包Data段数据长度
uint16_t data_segment_length = 0;

// 数据包帧头校验Flag，校验通过为1，校验不过为0
uint8_t frame_header_available = 0;

// 数据包全帧校验Flag, 校验通过为1，校验不过为0
uint8_t frame_total_available = 0;

// 上传到裁判系统的数据包序号
uint8_t seq = 0;

void js_task_init()
{
    // bsp
    USART2_Configuration();
    
    memset((extGameRobotState_t *)  &game_robot_state,  0, sizeof(extGameRobotState_t));
    memset((extRobotHurt_t *)       &robot_hurt,        0, sizeof(extRobotHurt_t));
    memset((extShootData_t *)       &shoot_data,        0, sizeof(extShootData_t));
    memset((extPowerHeatData_t *)   &power_heat_data,   0, sizeof(extPowerHeatData_t));
    memset((extRfidDetect_t *)      &rfid_detect,       0, sizeof(extRfidDetect_t));
    memset((extGameResult_t *)      &game_result,       0, sizeof(extGameResult_t));
    memset((extGetBuff_t *)         &get_buff,          0, sizeof(extGetBuff_t));
    memset((extShowData_t *)        &show_data,         0, sizeof(extShowData_t));
}

/**
 *  裁判系统 下载信息处理函数
 */
void js_download_msg_process(uint8_t msg)
{
    // 遇到帧头JS_MSG_SOF，初始化buffer
    if(msg == JS_MSG_SOF)
    {
        buffer_index = 0;
        frame_header_available = 0;
        frame_total_available = 0;
        frame_length = JS_MSG_MAX_LENGTH;
    }
    
    // 接收信息直到一帧结束
    if (buffer_index < JS_MSG_MAX_LENGTH && buffer_index < frame_length)
        receive_buffer[buffer_index++] = msg;
    
    // 接收完帧头信息，校验帧头，只有校验通过才获取帧长信息
    if (buffer_index == JS_CMD_ID_SEGMENT_OFFSET)
    {
        if (verify_crc8_check_sum(receive_buffer, JS_CMD_ID_SEGMENT_OFFSET))
            frame_header_available = 1;
        else
        {
            frame_header_available = 0;
            return;
        }
        
        if (frame_header_available)
        {
            data_segment_length = (receive_buffer[2] << 8) | (receive_buffer[1] & 0xff);
            frame_length = data_segment_length + JS_DATA_SEGMENT_OFFSET + 2;
        }
    }
    
    // 接收完全帧信息，校验全帧，只有帧头和全帧校验都通过才对数据帧进行解包
    if (frame_header_available && buffer_index >= frame_length)
    {
        if (verify_crc16_check_sum(receive_buffer, frame_length))
            frame_total_available = 1;
        else
        {
            frame_total_available = 0;
            return;
        }
        
        if (frame_total_available)
            js_msg_upack_handler(receive_buffer);
    }
}

/**
 *  裁判系统 上传信息处理函数
 */
void js_upload_msg_process(float d1, float d2, float d3, uint8_t mask)
{
    uTransmitDataFrame uframe;
    uFrameHead uframeHead;
    
    // 累加包序号
    if (seq++ >= 0xff) seq = 0;
    
    // 组织帧头FrameHead 
    uframeHead.tframeHead.sof = JS_MSG_SOF;
    uframeHead.tframeHead.dataLength = sizeof(uframe.tframe.data);
    uframeHead.tframeHead.seq = seq;
    append_crc8_check_sum(uframeHead.tframeHeadCh, sizeof(uframeHead.tframeHeadCh));
    uframe.tframe.frameHead = uframeHead.tframeHead;    
    
    // 设置命令码CmdID为0x0100
    uframe.tframe.CmdID = JS_SHOW_DATA_CMD_ID;
    
    // Data
    uframe.tframe.data.data1 = d1;
    uframe.tframe.data.data2 = d2;
    uframe.tframe.data.data3 = d3;
    uframe.tframe.data.mask = mask;
    
    // 封装帧尾FrameTail
    append_crc16_check_sum(uframe.tframeCh, sizeof(uframe.tframeCh));
    
    // 通过USART2发送结构体
    USART2_PrintBlock(uframe.tframeCh, sizeof(uframe.tframeCh));
}

/**
 *  裁判系统 解包函数
 */
void js_msg_upack_handler(uint8_t * buffer)
{
    switch (buffer[JS_CMD_ID_SEGMENT_OFFSET])
    {
        case JS_GAME_ROBOT_STATE_CMD_ID:
            memcpy((extGameRobotState_t *)&game_robot_state, 
                    buffer + JS_DATA_SEGMENT_OFFSET, 
                    data_segment_length);
        break;
        
        case JS_ROBOT_HURT_CMD_ID:
//                    memcpy((extRobotHurt_t *)&robot_hurt, 
//                            buffer + JS_DATA_SEGMENT_OFFSET, 
//                            data_segment_length);
        break;
        
        case JS_SHOOT_DATA_CMD_ID:
//                    memcpy((extShootData_t *)&shoot_data, 
//                            buffer + JS_DATA_SEGMENT_OFFSET, 
//                            data_segment_length);
        break;
        
        case JS_POWER_HEAT_DATA_CMD_ID:
            memcpy((extPowerHeatData_t *)&power_heat_data, 
                    buffer + JS_DATA_SEGMENT_OFFSET, 
                    data_segment_length);
        break;
        
        case JS_RFID_DETECT_CMD_ID:
//                    memcpy((extRfidDetect_t *)&rfid_detect, 
//                            buffer + JS_DATA_SEGMENT_OFFSET, 
//                            data_segment_length);
        break;
        
        case JS_GAME_RESULT_CMD_ID:
//                    memcpy((extGameResult_t *)&game_result, 
//                            buffer + JS_DATA_SEGMENT_OFFSET, 
//                            data_segment_length);
        break;
        
        case JS_GET_BUFF_CMD_ID:
//                    memcpy((extGetBuff_t *)&get_buff, 
//                            buffer + JS_DATA_SEGMENT_OFFSET, 
//                            data_segment_length);
        break;
        
        default: break;
    }
}
