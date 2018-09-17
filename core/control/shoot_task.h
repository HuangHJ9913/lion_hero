#ifndef _SHOOT_TASK_H_
#define _SHOOT_TASK_H_

#include "main.h"

#define PID_SHOOT_MOTOR_SPEED_INIT  (7)
#define PID_SHOOT_MOTOR_SPEED       (10)
#define PID_SHOOT_MOTOR_CHECK_SPEED (3)

typedef enum 
{
    UNSHOOTING = 0,
    SHOOTING ,
} shoot_mode_e;

extern volatile shoot_mode_e shoot_mode;
void shoot_task_init(void);  
void shoot_control(void);

#endif
