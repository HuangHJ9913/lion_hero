#ifndef _LIFT_TASK_H_
#define _LIFT_TASK_H_

#include "main.h"

#define LIFT_MOTOR_TOTAL_NUM          4
#define LIFT_SPEED_FACT_SLOW
#define LIFT_LIMIT_SPEED              2000  // mm/s
#define LIFT_MAX_SPEED              5000  // mm/s

#define LIFT_MOTOR_MAX_RPM            8500
#define LIFT_ANGLE_RATE            150

typedef struct {
    // 
    int16_t set;
    int16_t get;
    int16_t max;
    int16_t limit;
  
    int16_t rpm_set;
    int16_t rpm_get;
} lift_speed_t;

typedef struct {
    //
    int16_t set;
    int16_t get;  
    float distance_set;
    float distance_get;
} lift_position_t;

typedef enum 
{
    lift_mode_1 = 0,
    lift_mode_2,
} lift_mode_e;

typedef struct {
    lift_position_t   position[LIFT_MOTOR_TOTAL_NUM];
    lift_speed_t      speed[LIFT_MOTOR_TOTAL_NUM];
    lift_mode_e       control_mode;
		
		float             current[LIFT_MOTOR_TOTAL_NUM];
		int16_t           limit_speed;
		int16_t           max_speed;
} lift_t;

extern volatile lift_t lift;

void lift_init(void);
void lift_control(void);
#endif
