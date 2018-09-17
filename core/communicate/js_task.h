#ifndef _JS_TASK_H_
#define _JS_TASK_H_

#include "main.h"

#define JS_MSG_MAX_LENGTH               44u
#define JS_MSG_SOF                      0xA5  
#define JS_CMD_ID_SEGMENT_OFFSET        0x05
#define JS_DATA_SEGMENT_OFFSET          0x07 

#define JS_GAME_ROBOT_STATE_CMD_ID      0x0001
#define JS_ROBOT_HURT_CMD_ID            0x0002
#define JS_SHOOT_DATA_CMD_ID            0x0003
#define JS_POWER_HEAT_DATA_CMD_ID       0x0004
#define JS_RFID_DETECT_CMD_ID           0x0005
#define JS_GAME_RESULT_CMD_ID           0x0006
#define JS_GET_BUFF_CMD_ID              0x0007
#define JS_SHOW_DATA_CMD_ID             0x0100

typedef __packed struct
{
    uint8_t validFlag;
    float x;
    float y;
    float z;
    float yaw;
}position_t;

typedef __packed struct
{
    uint16_t stageRemianTime;
    uint8_t gameProgress;
    uint8_t robotLevel;
    uint16_t remainHP;
    uint16_t maxHP;
    position_t position;
}extGameRobotState_t;

typedef __packed struct
{
    uint8_t armorType :4;
    uint8_t hurtType :4;
}extRobotHurt_t;

typedef __packed struct
{
    uint8_t bulletType;
    uint8_t bulletFreq;
    float bulletSpeed;
    float reserved;
}extShootData_t;

typedef __packed struct
{
    float chassisVolt;
    float chassisCurrent;
    float chassisPower;
    float chassisPowerBuffer;
    uint16_t shooterHeat0;
    uint16_t shooterHeat1;
}extPowerHeatData_t;

typedef __packed struct
{
    uint8_t cardType;
    uint8_t cardIdx;
}extRfidDetect_t;

typedef __packed struct
{
    uint8_t winner;
}extGameResult_t;

typedef __packed struct
{
    uint8_t buffType;
    uint8_t buffAddition;
}extGetBuff_t;

typedef __packed struct
{
    float data1;
    float data2;
    float data3;
    uint8_t mask;
}extShowData_t;

typedef __packed struct
{
    uint8_t sof;
    uint16_t dataLength;
    uint8_t seq;
    uint8_t crc8;
}tFrameHead;

typedef __packed union
{
    tFrameHead tframeHead;
    uint8_t tframeHeadCh[sizeof(tFrameHead)];
}uFrameHead;

typedef __packed struct
{
    tFrameHead frameHead;
    uint16_t CmdID;
    extShowData_t data;
    uint16_t frameTail;
}tTransmitDataFrame;

typedef __packed union
{
    tTransmitDataFrame tframe;
    uint8_t tframeCh[sizeof(tTransmitDataFrame)];
}uTransmitDataFrame;

/**
 * 暂时测试用途, 适用版本为RM2017的裁判系统(2.0.3.33)
 */
typedef __packed struct
{
    uint8_t flag; //0 无效,1 有效
    float x;
    float y;
    float z;
    float compass;
}tLocData;

typedef __packed struct
{
    uint32_t remainTime;
    uint16_t remainLifeValue;
    float realChassisOutV;
    float realChassisOutA;
    tLocData locData;
    float remainPower;
}tGameInfo;

typedef __packed struct
{
    uint8_t weakId:4;
    uint8_t way:4;
    uint16_t value;
}tRealBloodChangedData;

typedef __packed struct
{
    float data1;
    float data2;
    float data3;
}tCustomData;

void js_task_init(void);
void js_download_msg_process(uint8_t);
void js_upload_msg_process(float data1, float data2, float data3, uint8_t mask);
void js_msg_upack_handler(uint8_t * buffer);

extern volatile extGameRobotState_t game_robot_state;
extern volatile extRobotHurt_t robot_hurt;
extern volatile extShootData_t shoot_data;
extern volatile extPowerHeatData_t power_heat_data;
extern volatile extRfidDetect_t rfid_detect;
extern volatile extGameResult_t game_result;
extern volatile extGetBuff_t get_buff;
extern volatile extShowData_t show_data;

#endif

