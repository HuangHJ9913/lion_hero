#ifndef _RC_TASK_H_
#define _RC_TASK_H_
#include "main.h"

// 整理后
#define RC_FRAME_LENGTH                     18u

#define RC_STICK_OFFSET                     1024u   
#define RC_STICK_TO_CHASSIS_SPEED_FACT      0.45f
#define RC_STICK_TO_PITCH_SPEED_FACT        0.004f
#define RC_STICK_TO_YAW_SPEED_FACT          0.0008f
#define RC_NORMALIZATION_DENOMINATOR        660.f

#define RC_MOUSE_TO_PITCH_ANGLE_FACT 		0.1625f
#define RC_MOUSE_TO_YAW_ANGLE_FACT 		    0.1625f

#define RC_SW_UP                            1
#define RC_SW_MID                           2
#define RC_SW_DOWN                          3
//#define RC_INPUT_REMOTE_CONTROLLER          (rc_msg.sw2 == RC_SW_UP)
//#define RC_INPUT_STOP                       (rc_msg.sw2 == RC_SW_MID)
//#define RC_INPUT_KEYBOARD_MOUSE             (rc_msg.sw2 == RC_SW_DOWN)

// 键盘映射
//#define KEYMAP_STICK_FORWARD_BACKWARD       rc_msg.ch2 
//#define KEYMAP_STICK_LEFT_RIGHT             rc_msg.ch1
//#define KEYMAP_STICK_YAW                    rc_msg.ch3
//#define KEYMAP_STICK_PITCH                  rc_msg.ch4
//#define KEYMAP_FORWARD                      rc_msg.keyboard.bit.W
//#define KEYMAP_BACKWARD                     rc_msg.keyboard.bit.S
//#define KEYMAP_LEFT                         rc_msg.keyboard.bit.A
//#define KEYMAP_RIGHT                        rc_msg.keyboard.bit.D
//#define KEYMAP_MOVE_SLOW                    rc_msg.keyboard.bit.SHIFT
//#define KEYMAP_MOVE_FAST                    rc_msg.keyboard.bit.CTRL
//#define KEYMAP_RESERVE0                     rc_msg.keyboard.bit.Q
//#define KEYMAP_RESERVE1                     rc_msg.keyboard.bit.E
//#define KEYMAP_RESERVE2                     rc_msg.keyboard.bit.R
//#define KEYMAP_RESERVE3                     rc_msg.keyboard.bit.F
//#define KEYMAP_RESERVE4                     rc_msg.keyboard.bit.G
//#define KEYMAP_RESERVE5                     rc_msg.keyboard.bit.Z
//#define KEYMAP_RESERVE6                     rc_msg.keyboard.bit.X
//#define KEYMAP_RESERVE7                     rc_msg.keyboard.bit.C
//#define KEYMAP_RESERVE8                     rc_msg.keyboard.bit.V
//#define KEYMAP_RESERVE9                     rc_msg.keyboard.bit.B


// 整理前
//0.004
#define FRICTION_WHEEL_MAX_DUTY             1175

//mouse control parameters
#define FRICTION_RAMP_TICK_COUNT			100
#define MOUSE_LR_RAMP_TICK_COUNT			50
#define MOUSR_FB_RAMP_TICK_COUNT			60

#define REMOTE_SWITCH_CHANGE_1TO3      (uint8_t)((REMOTE_SWITCH_VALUE_UP << 2) | REMOTE_SWITCH_VALUE_CENTRAL)   
#define REMOTE_SWITCH_CHANGE_2TO3      (uint8_t)((REMOTE_SWITCH_VALUE_DOWN << 2) | REMOTE_SWITCH_VALUE_CENTRAL)  
#define REMOTE_SWITCH_CHANGE_3TO1      (uint8_t)((REMOTE_SWITCH_VALUE_CENTRAL << 2) | REMOTE_SWITCH_VALUE_UP)
#define REMOTE_SWITCH_CHANGE_3TO2      (uint8_t)((REMOTE_SWITCH_VALUE_CENTRAL << 2) | REMOTE_SWITCH_VALUE_DOWN)

#define REMOTE_SWITCH_CHANGE_1TO3TO2   (uint8_t)((REMOTE_SWITCH_VALUE_UP << 4) |\
                                                 (REMOTE_SWITCH_VALUE_CENTRAL << 2) |\
                                                 (REMOTE_SWITCH_VALUE_DOWN))   

#define REMOTE_SWITCH_CHANGE_2TO3TO1   (uint8_t)((REMOTE_SWITCH_VALUE_DOWN << 4) |\
                                                 (REMOTE_SWITCH_VALUE_CENTRAL << 2) |\
                                                 (REMOTE_SWITCH_VALUE_UP)) 


typedef __packed struct
{
    /* rocker channel information */
    int16_t ch1;
    int16_t ch2;
    int16_t ch3;
    int16_t ch4;

    /* left and right lever information */
    uint8_t last_sw1;
    uint8_t last_sw2;
    uint8_t sw1;
    uint8_t sw2;

    /* mouse movement and button information */
    __packed struct
    {
        int16_t x;
        int16_t y;
        int16_t z;

        uint8_t l;
        uint8_t r;
    } mouse;

    /* keyboard key information */
    __packed union
    {
        uint16_t code;
        __packed struct 
        {
            uint16_t W:1;
            uint16_t S:1;
            uint16_t A:1;
            uint16_t D:1;
            uint16_t SHIFT:1;
            uint16_t CTRL:1;
            uint16_t Q:1;
            uint16_t E:1;
            uint16_t R:1;
            uint16_t F:1;
            uint16_t G:1;
            uint16_t Z:1;
            uint16_t X:1;
            uint16_t C:1;
            uint16_t V:1;
            uint16_t B:1;
        } bit;
    } keyboard;
} rc_msg_t;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
    
    int16_t yaw;
    int16_t pitch;  
	
    uint8_t last_sw1;
    uint8_t last_sw2;
    uint8_t sw1;
    uint8_t sw2;
} rc_ctrl_t;


extern volatile rc_msg_t rc_msg;
extern volatile rc_ctrl_t rc_ctrl;

void rc_task_init(void);
void rc_msg_process(uint8_t *msg);
#endif

