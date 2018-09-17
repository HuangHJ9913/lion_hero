#include "rc_task.h"

volatile rc_msg_t rc_msg;
volatile rc_ctrl_t rc_ctrl;

ramp_t rc_chassis_x_speed_ramp = RAMP_GEN_DAFAULT;
ramp_t rc_chassis_y_speed_ramp = RAMP_GEN_DAFAULT;
ramp_t rc_gimbal_yaw_speed_ramp = RAMP_GEN_DAFAULT;
ramp_t rc_gimbal_pitch_speed_ramp = RAMP_GEN_DAFAULT;

ramp_t rc_lift_motor_speed_ramp[4]={RAMP_GEN_DAFAULT, RAMP_GEN_DAFAULT, RAMP_GEN_DAFAULT, RAMP_GEN_DAFAULT};


//遥控器数据初始化，斜坡函数等的初始化
void rc_task_init()
{
    // bsp
    USART1_Configuration(100000);
    
	// 斜坡初始化
    rc_chassis_x_speed_ramp.init(&rc_chassis_x_speed_ramp, 300);
    rc_chassis_y_speed_ramp.init(&rc_chassis_y_speed_ramp, 300);
		for(int motor =0;motor<LIFT_MOTOR_TOTAL_NUM;motor++)
		{
			rc_lift_motor_speed_ramp[motor].init(&rc_lift_motor_speed_ramp[motor], 300);
		}
  
    chassis.speed.x_max = 10000;
    chassis.speed.y_max = 10000;
    
    memset((rc_msg_t *)&rc_msg, 0, sizeof(rc_msg_t));
    memset((rc_ctrl_t *)&rc_ctrl, 0, sizeof(rc_ctrl_t));
    
	// 摩擦轮运行状态初始化
}

// 遥控器控制模式处理
void rc_remote_controller_msg_handler()
{
    rc_ctrl.x     =  rc_msg.ch1 * chassis.speed.x_max / RC_NORMALIZATION_DENOMINATOR;
    rc_ctrl.y     =  rc_msg.ch2 * chassis.speed.y_max / RC_NORMALIZATION_DENOMINATOR;
    
    // 逆时针为正
    rc_ctrl.yaw   = -rc_msg.ch3 * gimbal.joint[YAW].speed_max   / RC_NORMALIZATION_DENOMINATOR;
    rc_ctrl.pitch =  rc_msg.ch4 * gimbal.joint[PITCH].speed_max / RC_NORMALIZATION_DENOMINATOR;
    
//	RemoteShootControl(&switch1, rc_msg.s1);
}

// 键盘功能键补充
void keyboard_function_handler()
{
    // chassis speed control
    if (rc_msg.keyboard.bit.SHIFT)
    {
        chassis.speed.x_limit = chassis.speed.x_max * CHASSIS_SPEED_FACT_SLOW;
        chassis.speed.y_limit = chassis.speed.y_max * CHASSIS_SPEED_FACT_SLOW;
        chassis.speed.z_limit = chassis.speed.z_max * CHASSIS_SPEED_FACT_SLOW;
    }
    else if (rc_msg.keyboard.bit.CTRL)
    {
        chassis.speed.x_limit = chassis.speed.x_max * CHASSIS_SPEED_FACT_FAST;
        chassis.speed.y_limit = chassis.speed.y_max * CHASSIS_SPEED_FACT_FAST;
        chassis.speed.z_limit = chassis.speed.z_max * CHASSIS_SPEED_FACT_FAST;
    }
    else
    {
        chassis.speed.x_limit = chassis.speed.x_max * CHASSIS_SPEED_FACT_NORMAL;
        chassis.speed.y_limit = chassis.speed.y_max * CHASSIS_SPEED_FACT_NORMAL;
        chassis.speed.z_limit = chassis.speed.z_max * CHASSIS_SPEED_FACT_NORMAL;
    }
}
//
void rc_lift_speed_get(int8_t orientation,int8_t statement)
{
	//GPIO_SetBits(GPIOB, GPIO_Pin_9);
	lift.speed[orientation].get = statement * lift.limit_speed * rc_lift_motor_speed_ramp[orientation].calc(&rc_lift_motor_speed_ramp[orientation]);
}
void rc_lift_speed_reset(int8_t orientation)
{
	//GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	lift.speed[orientation].get = 0;
	rc_lift_motor_speed_ramp[orientation].init(&rc_lift_motor_speed_ramp[orientation], 300);
}
void rc_lift_state_switch()
{
		int8_t statement=1;
		if (rc_msg.keyboard.bit.SHIFT) statement=-1;
	
		if(rc_msg.keyboard.bit.E) rc_lift_speed_get(RF,statement);
		else rc_lift_speed_reset(RF);
	
		if(rc_msg.keyboard.bit.Q) rc_lift_speed_get(LF,statement);
		else rc_lift_speed_reset(LF);
	
		if(rc_msg.keyboard.bit.Z) rc_lift_speed_get(LR,statement);
		else rc_lift_speed_reset(LR);
	
		if(rc_msg.keyboard.bit.C) rc_lift_speed_get(RR,statement);
		else rc_lift_speed_reset(RR);
}

// 键盘鼠标控制模式处理
void rc_keyboard_mouse_msg_handler()
{
    keyboard_function_handler();
    
    // 底盘运动控制 chassis speed control
    if      (rc_msg.keyboard.bit.W) rc_ctrl.y =  chassis.speed.y_limit * rc_chassis_y_speed_ramp.calc(&rc_chassis_y_speed_ramp);
    else if (rc_msg.keyboard.bit.S) rc_ctrl.y = -chassis.speed.y_limit * rc_chassis_y_speed_ramp.calc(&rc_chassis_y_speed_ramp);
    else
    {
        rc_ctrl.y = 0;
        rc_chassis_y_speed_ramp.init(&rc_chassis_y_speed_ramp, 300);
    }
    
    if      (rc_msg.keyboard.bit.A) rc_ctrl.x =  chassis.speed.x_limit * rc_chassis_x_speed_ramp.calc(&rc_chassis_x_speed_ramp);
    else if (rc_msg.keyboard.bit.D) rc_ctrl.x = -chassis.speed.x_limit * rc_chassis_x_speed_ramp.calc(&rc_chassis_x_speed_ramp);
    else
    {
        rc_ctrl.x = 0;
        rc_chassis_x_speed_ramp.init(&rc_chassis_x_speed_ramp, 300);
    }
    
    // 云台运动控制 gimbal position control
    value_limit(rc_msg.mouse.x, -150, 150);
    value_limit(rc_msg.mouse.y, -150, 150);
    
    rc_ctrl.yaw   = rc_msg.mouse.x * RC_MOUSE_TO_YAW_ANGLE_FACT;
    rc_ctrl.pitch = rc_msg.mouse.y * RC_MOUSE_TO_PITCH_ANGLE_FACT;
    
		rc_lift_state_switch();
//	MouseShootControl(mouse);
}


void rc_input_stop_handler()
{
    rc_ctrl.x = 0;
    rc_ctrl.y = 0;
    rc_ctrl.z = rc_ctrl.yaw;
	  for (uint8_t motor = 0; motor < LIFT_MOTOR_TOTAL_NUM; motor++)
		{
				rc_lift_speed_reset(motor);
		}
}

void rc_control_mode_switch()
{
    if      (rc_msg.sw2 == RC_SW_UP)    rc_remote_controller_msg_handler();
    else if (rc_msg.sw2 == RC_SW_DOWN)  rc_keyboard_mouse_msg_handler();
    else                                rc_input_stop_handler();
}

void rc_msg_process(uint8_t *msg)
{
    if(msg == NULL) return;
    
    rc_msg.ch1  = 0x07FF & (msg[0] | msg[1] << 8);
    rc_msg.ch2  = 0x07FF & (msg[1] >> 3 | msg[2] << 5);
    rc_msg.ch3  = 0x07FF & (msg[2] >> 6 | msg[3] << 2 | msg[4] << 10);
    rc_msg.ch4  = 0x07FF & (msg[4] >> 1 | msg[5] << 7);
    
    rc_msg.ch1  -= RC_STICK_OFFSET;
    rc_msg.ch2  -= RC_STICK_OFFSET;
    rc_msg.ch3  -= RC_STICK_OFFSET;
    rc_msg.ch4  -= RC_STICK_OFFSET;
    
    // 消抖和去除超范围的异常数据
    if(abs(rc_msg.ch1)  <= 5 || abs(rc_msg.ch1) > 660) rc_msg.ch1 = 0;
    if(abs(rc_msg.ch2)  <= 5 || abs(rc_msg.ch2) > 660) rc_msg.ch2 = 0;
    if(abs(rc_msg.ch3)  <= 5 || abs(rc_msg.ch3) > 660) rc_msg.ch3 = 0;
    if(abs(rc_msg.ch4)  <= 5 || abs(rc_msg.ch4) > 660) rc_msg.ch4 = 0;
    
    // sw1: 左  sw2: 右
    rc_msg.last_sw1 = rc_msg.sw1;
    rc_msg.last_sw2 = rc_msg.sw2;
    rc_msg.sw1 = ((msg[5] >> 4) & 0x000C) >> 2;
    rc_msg.sw2 = (msg[5] >> 4) & 0x0003;
	
    // 表示速度
    rc_msg.mouse.x = msg[6]  | msg[7] << 8;
    rc_msg.mouse.y = msg[8]  | msg[9] << 8;
    rc_msg.mouse.z = msg[10] | msg[11] << 8;

    // 1: 按下  0: 未按下
    rc_msg.mouse.l = msg[12];
    rc_msg.mouse.r = msg[13];
	
    // 共16个键可选
    rc_msg.keyboard.code = msg[14] | msg[15] << 8;

    rc_control_mode_switch();
}

