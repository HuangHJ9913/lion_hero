#include "lift_task.h"

volatile lift_t lift;

extern volatile encoder_t lift_encoder[LIFT_MOTOR_TOTAL_NUM];

void lift_init()
{
    memset((chassis_t *)&lift, 0, sizeof(lift_t));
    lift.limit_speed = LIFT_LIMIT_SPEED; 
		lift.max_speed = LIFT_MAX_SPEED; 
	
    for (uint8_t motor = 0; motor < LIFT_MOTOR_TOTAL_NUM; motor++)
        PID_struct_init(&pid_lift_speed[motor], POSITION_PID, CHASSIS_WHEEL_MAX_CURRENT, 10000, 3.5, 0.1, 0);
}

void lift_control()
{
    switch (lift.control_mode)
    {
        case lift_mode_1:  
							lift.speed[RF].set = lift.speed[RF].get + (imu.angle_x - imu.angle_y) * LIFT_ANGLE_RATE;
							lift.speed[LF].set = lift.speed[LF].get + (imu.angle_x + imu.angle_y) * LIFT_ANGLE_RATE;
							lift.speed[LR].set = lift.speed[LR].get - (imu.angle_x - imu.angle_y) * LIFT_ANGLE_RATE;
							lift.speed[RR].set = lift.speed[RR].get - (imu.angle_x + imu.angle_y) * LIFT_ANGLE_RATE;          
        break;
				
        case lift_mode_2:  
							lift.speed[LR].set = lift.position[LR].get ;
							lift.speed[RR].set = lift.position[RR].get ;        
        break;
				
        default: 
					  for (uint8_t motor = 0; motor < LIFT_MOTOR_TOTAL_NUM; motor++)
							lift.speed[motor].set = lift.speed[motor].get;
        break;
    }
		
		for (uint8_t motor = 0; motor < LIFT_MOTOR_TOTAL_NUM; motor++)
		{
				if(get_microswitch_nadir(motor))
					if(lift.speed[motor].set<0)
						lift.speed[motor].set = 0;
		}
    //USART3_Oscilloscope(imu.angle_x*100, 0, imu.angle_y, 0);
		
		
		
		
    for (uint8_t motor = 0; motor < LIFT_MOTOR_TOTAL_NUM; motor++)
        lift.speed[motor].rpm_get = lift_encoder[motor].filter_rpm;
    
    for (uint8_t motor = 0; motor < LIFT_MOTOR_TOTAL_NUM; motor++)
        lift.current[motor] = pid_calc(&pid_lift_speed[motor], lift.speed[motor].rpm_get, lift.speed[motor].set); 
		
		set_lift_current(lift.current[RF], lift.current[LF], lift.current[LR], lift.current[RR]);
}
