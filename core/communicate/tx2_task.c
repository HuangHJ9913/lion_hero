#include "tx2_task.h"

volatile pc_robot_status_t pc_robot_status;
volatile pc_chassis_upload_t pc_chassis_upload;
volatile pc_gimbal_upload_t pc_gimbal_upload;
volatile pc_custom_upload_t pc_custom_upload;

volatile pc_chassis_download_t pc_chassis_download;
volatile pc_gimbal_download_t pc_gimbal_download;
volatile pc_custom_download_t pc_custom_download;


// �ش���Ϣ����Buffer
uint8_t pc_receive_buffer[PC_MSG_MAX_LENGTH + 1];

// Buffer��ǰ����ѹ�����ݵ�index��ѹ����ɺ�index����1
uint8_t pc_buffer_index = 0;

// ���ݰ����ݳ���
uint16_t pc_frame_length = PC_MSG_MAX_LENGTH;

// ���ݰ�Data�����ݳ���
uint16_t pc_data_segment_length = 0;

// ���ݰ�֡ͷУ��Flag��У��ͨ��Ϊ1��У�鲻��Ϊ0
uint8_t pc_frame_header_available = 0;

// ���ݰ�ȫ֡У��Flag, У��ͨ��Ϊ1��У�鲻��Ϊ0
uint8_t pc_frame_total_available = 0;

// �ϴ������ص��Ե����ݰ����
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
 *  ���ص��� ������Ϣ������
 */
void pc_download_msg_process(uint8_t msg)
{
    // ����֡ͷPC_MSG_SOF����ʼ��buffer
    if(msg == PC_MSG_SOF)
    {
        pc_buffer_index = 0;
        pc_frame_header_available = 0;
        pc_frame_total_available = 0;
        pc_frame_length = PC_MSG_MAX_LENGTH;
    }
    
    // ������Ϣֱ��һ֡����
    if (pc_buffer_index < PC_MSG_MAX_LENGTH && pc_buffer_index < pc_frame_length)
        pc_receive_buffer[pc_buffer_index++] = msg;
    
    // ������֡ͷ��Ϣ��У��֡ͷ��ֻ��У��ͨ���Ż�ȡ֡����Ϣ
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
    
    // ������ȫ֡��Ϣ��У��ȫ֡��ֻ��֡ͷ��ȫ֡У�鶼ͨ���Ŷ�����֡���н��
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
 *  ���ص��� �ϴ���Ϣ������
 */
void pc_upload_msg_process(float d1, float d2, float d3, float d4)
{
    pc_upload_data_frame_u uframe;
    pc_upload_frame_head_u frame_head_u;
    
    // �ۼӰ����
    if (pc_seq++ >= 0xff) pc_seq = 0;
    
    // ��֯֡ͷFrameHead 
    frame_head_u.frame_head.sof = PC_MSG_SOF;
    frame_head_u.frame_head.dataLength = sizeof(uframe.tframe.data);
    frame_head_u.frame_head.seq = pc_seq;
    append_crc8_check_sum(frame_head_u.tframeHeadCh, sizeof(frame_head_u.tframeHeadCh));
    uframe.tframe.frame_head_u = frame_head_u;
    
    // ����������CmdIDΪ0x0100
    uframe.tframe.CmdID = PC_CUSTOM_UPLOAD_CMD_ID;
    
    // Data
    uframe.tframe.data.data1 = d1;
    uframe.tframe.data.data2 = d2;
    uframe.tframe.data.data3 = d3;
    uframe.tframe.data.data3 = d4;
    
    // ��װ֡βFrameTail
    append_crc16_check_sum(uframe.tframeCh, sizeof(uframe.tframeCh));
    
    // ͨ��USART3���ͽṹ��
    USART3_PrintBlock(uframe.tframeCh, sizeof(uframe.tframeCh));
}

/**
 *  ���ص��� �������
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
