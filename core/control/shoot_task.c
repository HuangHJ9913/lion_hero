#include "shoot_task.h"

extern volatile encoder_t feed_encoder;
volatile shoot_mode_e shoot_mode;

void shoot_task_init()
{
		Gun_Configuration();
		shoot_mode = UNSHOOTING;
	  PID_struct_init(&pid_feed_speed, POSITION_PID, CHASSIS_WHEEL_MAX_CURRENT, 10000, 3.5, 0.1, 0);
}

void shoot_control()	
{
		if(shoot_mode	== SHOOTING)
		{
			  //SetFrictionWheelSpeed(1300);
				set_gun_current(-500);
		}
		else
				set_gun_current(500); 
}
