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
 * ���ڲ���ϵͳ�ش�����Ϣ���ڶ�֡��һ֡�ֶη��������
 * ������Ҫ�ȶԽ��յ�����Ϣ�������ϣ�������ݽ��ղ���У��ͨ����Ž��н���
 */

// �ش���Ϣ����Buffer
static uint8_t receive_buffer[JS_MSG_MAX_LENGTH + 1];

// Buffer��ǰ����ѹ�����ݵ�index��ѹ����ɺ�index����1
static uint8_t buffer_index = 0;

// ���ݰ�Data�����ݳ��ȣ�JS_MSG_MAX_LENGTH=44ΪRM2017����ϵͳͨ��Э��������ܵ�ȫ�����ݳ���
uint16_t frame_length = JS_MSG_MAX_LENGTH;

// ���ݰ�Data�����ݳ���
uint16_t data_segment_length = 0;

// ���ݰ�֡ͷУ��Flag��У��ͨ��Ϊ1��У�鲻��Ϊ0
uint8_t frame_header_available = 0;

// ���ݰ�ȫ֡У��Flag, У��ͨ��Ϊ1��У�鲻��Ϊ0
uint8_t frame_total_available = 0;

// �ϴ�������ϵͳ�����ݰ����
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
 *  ����ϵͳ ������Ϣ������
 */
void js_download_msg_process(uint8_t msg)
{
    // ����֡ͷJS_MSG_SOF����ʼ��buffer
    if(msg == JS_MSG_SOF)
    {
        buffer_index = 0;
        frame_header_available = 0;
        frame_total_available = 0;
        frame_length = JS_MSG_MAX_LENGTH;
    }
    
    // ������Ϣֱ��һ֡����
    if (buffer_index < JS_MSG_MAX_LENGTH && buffer_index < frame_length)
        receive_buffer[buffer_index++] = msg;
    
    // ������֡ͷ��Ϣ��У��֡ͷ��ֻ��У��ͨ���Ż�ȡ֡����Ϣ
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
    
    // ������ȫ֡��Ϣ��У��ȫ֡��ֻ��֡ͷ��ȫ֡У�鶼ͨ���Ŷ�����֡���н��
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
 *  ����ϵͳ �ϴ���Ϣ������
 */
void js_upload_msg_process(float d1, float d2, float d3, uint8_t mask)
{
    uTransmitDataFrame uframe;
    uFrameHead uframeHead;
    
    // �ۼӰ����
    if (seq++ >= 0xff) seq = 0;
    
    // ��֯֡ͷFrameHead 
    uframeHead.tframeHead.sof = JS_MSG_SOF;
    uframeHead.tframeHead.dataLength = sizeof(uframe.tframe.data);
    uframeHead.tframeHead.seq = seq;
    append_crc8_check_sum(uframeHead.tframeHeadCh, sizeof(uframeHead.tframeHeadCh));
    uframe.tframe.frameHead = uframeHead.tframeHead;    
    
    // ����������CmdIDΪ0x0100
    uframe.tframe.CmdID = JS_SHOW_DATA_CMD_ID;
    
    // Data
    uframe.tframe.data.data1 = d1;
    uframe.tframe.data.data2 = d2;
    uframe.tframe.data.data3 = d3;
    uframe.tframe.data.mask = mask;
    
    // ��װ֡βFrameTail
    append_crc16_check_sum(uframe.tframeCh, sizeof(uframe.tframeCh));
    
    // ͨ��USART2���ͽṹ��
    USART2_PrintBlock(uframe.tframeCh, sizeof(uframe.tframeCh));
}

/**
 *  ����ϵͳ �������
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
