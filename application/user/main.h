#ifndef __MAIN_H__
#define __MAIN_H__

// ST Libraries
#include "stm32f4xx.h"

// Standard Libraries
#include <math.h>
#include "stdio.h"
#include "string.h"

// RM Libraries
#include "fifo.h"
#include "ramp.h"
#include "common.h"

// PID
#include "pid.h"
#include "pid_regulator.h"

// driver/can
#include "can1.h"
#include "can2.h"
// driver/usart
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"
// driver/timer
#include "timer.h"
// driver/sensor
#include "led.h"
#include "encoder.h"
#include "microswitch.h"
// driver/engine
#include "servo.h"
#include "gun.h"
#include "cylinder.h"
// driver/others
#include "bsp.h"
#include "delay.h"


// FeedMotor
#include "FeedMotorDriver.h"

// application/configureation
#include "configureation.h"

// core/common
#include "mode_switch_task.h"
#include "robot_status.h"
#include "supervisor_task.h"
// core/communicate
#include "can_task.h"
#include "rc_task.h"
#include "js_task.h"
#include "tx2_task.h"
#include "imu_task.h"
// core/control
#include "chassis_task.h"
#include "gimbal_task.h"
#include "shoot_task.h"
#include "lift_task.h"
// library/lion
#include "pid.h"
#include "common.h"
#include "crc_protocal.h"

#endif 
