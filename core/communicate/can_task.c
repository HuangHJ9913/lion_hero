#include "can_task.h"

static uint32_t can1_count = 0;
static uint32_t can2_count = 0;

volatile encoder_t chassis_encoder[CHASSIS_WHEEL_TOTAL_NUM];
volatile encoder_t gimbal_encoder[GIMBAL_JOINT_TOTAL_NUM];
volatile encoder_t lift_encoder[LIFT_MOTOR_TOTAL_NUM];
volatile encoder_t feed_encoder;

void can_task_init()
{
    // bsp
    CAN1_Configuration();
    CAN2_Configuration();
    
    // Initial encoders
    memset((encoder_t *)&chassis_encoder, 0, CHASSIS_WHEEL_TOTAL_NUM * sizeof(chassis_encoder));
    memset((encoder_t *)&gimbal_encoder, 0, GIMBAL_JOINT_TOTAL_NUM * sizeof(gimbal_encoder));
		memset((encoder_t *)&lift_encoder, 0, GIMBAL_JOINT_TOTAL_NUM * sizeof(lift_encoder));
}

/********************************************************************************
                                    get_encoder_bias

�����������ʼ������ֵ��Ϊƫ������ڵ��̵���ϣ���Ϊ������̨�����ʼ��ʱ������
*********************************************************************************/
void get_encoder_bias(volatile encoder_t *v, CanRxMsg * msg)
{
    // �����ʼ������ֵ��Ϊƫ��
    v->ecd_bias = (msg->Data[0] << 8) | msg->Data[1];  
    v->ecd_value = v->ecd_bias;
    v->last_raw_value = v->ecd_bias;
    v->temp_count++;
}

/********************************************************************************
                                  encoder_process_handler

������������յ��ĵ������Ļش���Ϣ����Բ�ͬ�����ȡ��ͬ����Ϣ
*********************************************************************************/
void encoder_process_handler(volatile encoder_t *v, CanRxMsg * msg)
{
    int32_t temp_sum = 0;
    
    v->last_raw_value   =   v->raw_value;
    v->raw_value        =   (msg->Data[0] << 8) | msg->Data[1];
    v->diff             =   v->raw_value - v->last_raw_value;

    // ���α������ķ���ֵ���̫�󣬱�ʾȦ�������˸ı�
    if(v->diff < -4096)    
    {
        v->round_cnt++;
        v->ecd_raw_rate = v->diff + 8192;
    }
    else if(v->diff > 4096)
    {
        v->round_cnt--;
        v->ecd_raw_rate = v->diff - 8192;
    }
    else
    {
        v->ecd_raw_rate = v->diff;
    }

    // ����õ������ı��������ֵ
    v->ecd_value = v->raw_value + v->round_cnt * 8192;

    // ����õ��Ƕ�ֵ
    v->local_angle = (float)((v->raw_value - v->ecd_bias) * 360 / 8192);
    v->total_angle = (float)(v->local_angle + v->round_cnt * 360.f);

    v->rate_buf[v->buf_count++] = v->ecd_raw_rate;

    if(v->buf_count == RATE_BUF_SIZE)
    {
        v->buf_count = 0;
    }

    // �����ٶ�ƽ��ֵ
    for(uint8_t i = 0;i < RATE_BUF_SIZE; i++)
    {
        temp_sum += v->rate_buf[i];
    }

    v->filter_rpm = (temp_sum == 0) ? 0 : (int32_t)(temp_sum * ENCODER_RPM_COEFFICIENT / RATE_BUF_SIZE);
}

/********************************************************************************
														can_receive_msg_process

����������can�Ļش���Ϣ�����ݱ��ĵı�ʶ����ȡ��ͬ�Ĵ���취
*********************************************************************************/
void can1_receive_msg_process(CanRxMsg * msg)
{
    can1_count++;

    switch(msg->StdId)
    {
        case CAN_CHASSIS_RIGHT_FORWARD_FEEDBACK_MSG_ID:
        {
            (can1_count <= 50) 
            ? get_encoder_bias(&chassis_encoder[RF], msg)
            : encoder_process_handler(&chassis_encoder[RF], msg);        
        }break;

        case CAN_CHASSIS_LEFT_FORWARD_FEEDBACK_MSG_ID:
        {
            (can1_count <= 50) 
            ? get_encoder_bias(&chassis_encoder[LF], msg)
            : encoder_process_handler(&chassis_encoder[LF], msg);
        }break;

        case CAN_CHASSIS_LEFT_REAR_FEEDBACK_MSG_ID:
        {
            (can1_count <= 50) 
            ? get_encoder_bias(&chassis_encoder[LR], msg)
            : encoder_process_handler(&chassis_encoder[LR], msg);   
        }break;

        case CAN_CHASSIS_RIGHT_REAR_FEEDBACK_MSG_ID:
        {
            (can1_count <= 50) 
            ? get_encoder_bias(&chassis_encoder[RR], msg)
            : encoder_process_handler(&chassis_encoder[RR], msg);
        }break;

        case CAN_GIMBAL_YAW_FEEDBACK_MSG_ID:
        {
            encoder_process_handler(&gimbal_encoder[YAW], msg);
        }break;

        case CAN_GIMBAL_PITCH_FEEDBACK_MSG_ID:
        {
            encoder_process_handler(&gimbal_encoder[PITCH], msg);
        }break;

        default: { }
    }
}

void can2_receive_msg_process(CanRxMsg * msg)
{
    can2_count++;
    switch(msg->StdId)
    {
        case CAN_CHASSIS_RIGHT_FORWARD_FEEDBACK_MSG_ID:
        {
            (can2_count <= 50) 
            ? get_encoder_bias(&lift_encoder[RF], msg)
            : encoder_process_handler(&lift_encoder[RF], msg);        
        }break;

        case CAN_CHASSIS_LEFT_FORWARD_FEEDBACK_MSG_ID:
        {
            (can2_count <= 50) 
            ? get_encoder_bias(&lift_encoder[LF], msg)
            : encoder_process_handler(&lift_encoder[LF], msg);
        }break;

        case CAN_CHASSIS_LEFT_REAR_FEEDBACK_MSG_ID:
        {
            (can2_count <= 50) 
            ? get_encoder_bias(&lift_encoder[LR], msg)
            : encoder_process_handler(&lift_encoder[LR], msg);   
        }break;

        case CAN_CHASSIS_RIGHT_REAR_FEEDBACK_MSG_ID:
        {
            (can2_count <= 50) 
            ? get_encoder_bias(&lift_encoder[RR], msg)
            : encoder_process_handler(&lift_encoder[RR], msg);
        }break;

        case CAN_GIMBAL_YAW_FEEDBACK_MSG_ID:
        {
            encoder_process_handler(&feed_encoder, msg);
        }break;

        default: { }
    }
}

/********************************************************************************
                                    set_chassis_current

����������Ԥ��Э������̵���巢�͵���ָ�������Ϊ1Mbps
      ��һ���ֲ��ġ�RM_820R���ʹ��˵���顷�жԵ�����ձ��ĸ�ʽ��˵��
*********************************************************************************/
void set_lift_current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq)
{
    CanTxMsg tx_message;

    // ��ʶ��ID	0x200
    tx_message.StdId = 0x200;

    // ֡����		��׼֡
    tx_message.IDE = CAN_Id_Standard;

    // ֡��ʽ		DATA
    tx_message.RTR = CAN_RTR_Data;

    // DLC			8�ֽ�
    tx_message.DLC = 0x08;

    // �������ID 0x201
    // ����16λ����(-32768~+32767)
    tx_message.Data[0] = (uint8_t)(cm1_iq >> 8);
    tx_message.Data[1] = (uint8_t)cm1_iq;

    // �������ID 0x202
    // ����16λ����(-32768~+32767)
    tx_message.Data[2] = (uint8_t)(cm2_iq >> 8);
    tx_message.Data[3] = (uint8_t)cm2_iq;

    // �������ID 0x203
    // ����16λ����(-32768~+32767)
    tx_message.Data[4] = (uint8_t)(cm3_iq >> 8);
    tx_message.Data[5] = (uint8_t)cm3_iq;

    // �������ID 0x204
    // ����16λ����(-32768~+32767)
    tx_message.Data[6] = (uint8_t)(cm4_iq >> 8);
    tx_message.Data[7] = (uint8_t)cm4_iq;

    CAN_Transmit(CAN_PORT_LIFT, &tx_message);
}

/********************************************************************************
                                    set_chassis_current

����������Ԥ��Э������̵���巢�͵���ָ�������Ϊ1Mbps
      ��һ���ֲ��ġ�RM_820R���ʹ��˵���顷�жԵ�����ձ��ĸ�ʽ��˵��
*********************************************************************************/
void set_chassis_current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq)
{
    CanTxMsg tx_message;

    // ��ʶ��ID	0x200
    tx_message.StdId = 0x200;

    // ֡����		��׼֡
    tx_message.IDE = CAN_Id_Standard;

    // ֡��ʽ		DATA
    tx_message.RTR = CAN_RTR_Data;

    // DLC			8�ֽ�
    tx_message.DLC = 0x08;

    // �������ID 0x201
    // ����16λ����(-32768~+32767)
    tx_message.Data[0] = (uint8_t)(cm1_iq >> 8);
    tx_message.Data[1] = (uint8_t)cm1_iq;

    // �������ID 0x202
    // ����16λ����(-32768~+32767)
    tx_message.Data[2] = (uint8_t)(cm2_iq >> 8);
    tx_message.Data[3] = (uint8_t)cm2_iq;

    // �������ID 0x203
    // ����16λ����(-32768~+32767)
    tx_message.Data[4] = (uint8_t)(cm3_iq >> 8);
    tx_message.Data[5] = (uint8_t)cm3_iq;

    // �������ID 0x204
    // ����16λ����(-32768~+32767)
    tx_message.Data[6] = (uint8_t)(cm4_iq >> 8);
    tx_message.Data[7] = (uint8_t)cm4_iq;

    CAN_Transmit(CAN_PORT_CHASSIS, &tx_message);
}

/********************************************************************************
                                    set_gimbal_current

����������Ԥ��Э�����̨����巢�͵���ָ��
      ��һ���ֲ��ġ�RM2017_6623��̨������ʹ��˵���顷�жԵ�����ձ��ĸ�ʽ��˵��
*********************************************************************************/
void set_gimbal_current(int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq)
{
    CanTxMsg tx_message;

    // ��ʶ��ID	0x1FF
    tx_message.StdId = 0x1FF;

    // ֡����		��׼֡
    tx_message.IDE = CAN_Id_Standard;

    // ֡��ʽ		DATA
    tx_message.RTR = CAN_RTR_Data;

    // DLC			8�ֽ�
    tx_message.DLC = 0x08;

    // Yaw��
    // ����16λ����(-5000~+5000)
    tx_message.Data[0] = (uint8_t)(gimbal_yaw_iq >> 8);
    tx_message.Data[1] = (uint8_t)gimbal_yaw_iq;

    // Pitch��
    // ����16λ����(-5000~+5000)
    tx_message.Data[2] = (uint8_t)(gimbal_pitch_iq >> 8);
    tx_message.Data[3] = (uint8_t)gimbal_pitch_iq;

    // Roll��
    // ����16λ����(-5000~+5000)
    tx_message.Data[4] = 0x00;
    tx_message.Data[5] = 0x00;

    // Resv Ԥ������
    // ����16λ����(-5000~+5000)
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;

    CAN_Transmit(CAN_PORT_GIMBAL, &tx_message);
}

void set_gun_current(int16_t gun_feed_iq)
{
    CanTxMsg tx_message;
    // ��ʶ��ID	0x1FF
    tx_message.StdId = 0x1FF;
    // ֡����		��׼֡
    tx_message.IDE = CAN_Id_Standard;
    // ֡��ʽ		DATA
    tx_message.RTR = CAN_RTR_Data;
    // DLC			8�ֽ�
    tx_message.DLC = 0x08;

    // ������
    // ����16λ����(-5000~+5000)
    tx_message.Data[0] = (uint8_t)(gun_feed_iq >> 8);
    tx_message.Data[1] = (uint8_t)gun_feed_iq;

    tx_message.Data[2] = 0x00;
    tx_message.Data[3] = 0x00;

    tx_message.Data[4] = 0x00;
    tx_message.Data[5] = 0x00;

    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;

    CAN_Transmit(CAN_PORT_GUN, &tx_message);
}
