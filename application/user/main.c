#include "main.h"

int main(void)
{
		can_task_init();
		rc_task_init();
		js_task_init(); 
		imu_task_init();
	  shoot_task_init();
	
		gimbal_init();
		chassis_init();
		lift_init();
		
		TIM6_Configuration();
		USART3_Configuration();
		LED_Configuration();
		Microswitch_Configuration();
	  Servo_Configuration();
		
		while (1)
		{
			if(get_microswitch_nadir(RR)) //°´ÏÂ
			{
				shoot_mode = SHOOTING;
				SetFrictionWheelSpeed(1300);
				set_servo_angle(1,10);
				set_servo_angle(2,10);
				led_turn_on();
			}
			else
			{
				//servo_forward();
				shoot_mode = UNSHOOTING;
				SetFrictionWheelSpeed(1000);
				set_servo_angle(1,100);
				set_servo_angle(2,180);
				led_turn_off();
			}
		}
}
