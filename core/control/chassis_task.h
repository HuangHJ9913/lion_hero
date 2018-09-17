#ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_

#include "main.h"

#define CHASSIS_WHEEL_TOTAL_NUM          4
//#define CHASSIS_WHEEL_RIGHT_FORWARD_ID   0
//#define CHASSIS_WHEEL_LEFT_FORWARD_ID    1
//#define CHASSIS_WHEEL_LEFT_REAR_ID       2
//#define CHASSIS_WHEEL_RIGHT_REAR_ID      3
#define RF                              0
#define LF                              1
#define LR                              2
#define RR                              3

// chassis structure
#define CHASSIS_WHEEL_BASE               285 // mm  车轮轴中心到底盘中心的距离
#define CHASSIS_WHEEL_PERIMETER          478 // mm  车轮周长
#define CHASSIS_ROTATE_RATIO             (CHASSIS_WHEEL_BASE  / RAD_TO_DEG)
#define CHASSIS_WHEEL_RPM_RARIO          (SEC_PER_MIN / (chassis.info.wheel_perimeter * CHASSIS_DECELE_RATIO))

#define CHASSIS_SPEED_FACT_FAST          0.9
#define CHASSIS_SPEED_FACT_NORMAL        0.5
#define CHASSIS_SPEED_FACT_SLOW          0.3

#define CHASSIS_WHEEL_MAX_RPM            8500
#define CHASSIS_DECELE_RATIO             (1.f/19.f)
#define CHASSIS_MAX_SPEED_X              3000  // mm/s
#define CHASSIS_MAX_SPEED_Y              3000  // mm/s
#define CHASSIS_MAX_SPEED_Z              720   // deg/s

#define CHASSIS_ROTATE_MAX_CURRENT       10000 // (mA)
#define CHASSIS_WHEEL_MAX_CURRENT        10000

typedef struct {
    uint16_t wheel_base;                // (mm) 轮子轴心到底盘正中心的距离
    uint16_t wheel_perimeter;           // (mm) 轮子周长
} chassis_info_t;

typedef struct {
    // 对于底盘整体而言
    int16_t x_set;
    int16_t x_get;
    int16_t x_max;
    int16_t x_limit;
    
    int16_t y_set;
    int16_t y_get;
    int16_t y_max;
    int16_t y_limit;
    
    int16_t z_set;
    int16_t z_get;
    int16_t z_max;
    int16_t z_limit;
    
    // 对于每个轮子而言
    int16_t rpm_set[CHASSIS_WHEEL_TOTAL_NUM];
    int16_t rpm_get[CHASSIS_WHEEL_TOTAL_NUM];
} chassis_speed_t;

typedef struct {
    // 对于底盘整体而言
    int16_t x_set;
    int16_t x_get;
    
    int16_t y_set;
    int16_t y_get;
    
    int16_t z_set;
    int16_t z_get;
    
    // 对于每个轮子而言
    int16_t angel_set[CHASSIS_WHEEL_TOTAL_NUM];
    int16_t angel_get[CHASSIS_WHEEL_TOTAL_NUM];
    
    float distance_set[CHASSIS_WHEEL_TOTAL_NUM];
    float distance_get[CHASSIS_WHEEL_TOTAL_NUM];
} chassis_position_t;

typedef enum 
{
    MANUAL_SEPARATE_GIMBAL = 0,
    TWIST_MODE,
} chassis_mode_e;

typedef struct {
    chassis_info_t       info;
    chassis_position_t   position;
    chassis_speed_t      speed;
    float               current[CHASSIS_WHEEL_TOTAL_NUM];
    
    chassis_mode_e       control_mode;
} chassis_t;

extern volatile chassis_t chassis;

void chassis_init(void);
void chassis_control(void);
void chassis_mecanum_calc(float vx, float vy, float vw, int16_t speed[]);
void power_limit_handler(void);
#endif
