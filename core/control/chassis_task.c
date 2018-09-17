#include "chassis_task.h"

volatile chassis_t chassis;

extern volatile encoder_t chassis_encoder[CHASSIS_WHEEL_TOTAL_NUM];

void chassis_init()
{
    memset((chassis_t *)&chassis, 0, sizeof(chassis_t));
    
    chassis.info.wheel_base      =   CHASSIS_WHEEL_BASE;
    chassis.info.wheel_perimeter =   CHASSIS_WHEEL_PERIMETER;
    chassis.speed.x_max          =   CHASSIS_MAX_SPEED_X;
    chassis.speed.y_max          =   CHASSIS_MAX_SPEED_Y;
    chassis.speed.z_max          =   CHASSIS_MAX_SPEED_X;
    
    for (uint8_t wheel = 0; wheel < CHASSIS_WHEEL_TOTAL_NUM; wheel++)
        PID_struct_init(&pid_wheel_speed[wheel], POSITION_PID, CHASSIS_WHEEL_MAX_CURRENT, 10000, 4.5, 0.1, 0);
    
    PID_struct_init(&pid_chassis_angle, POSITION_PID, CHASSIS_ROTATE_MAX_CURRENT, 1000, 1, 0, 0);
}

void chassis_control()
{
    switch (chassis.control_mode)
    {
        case TWIST_MODE:
            
        break;
        
        default: 
            chassis.speed.x_set = rc_ctrl.x;
            chassis.speed.y_set = rc_ctrl.y;
            chassis.speed.z_set = rc_ctrl.yaw;
        break;
    }
    
    
    for (uint8_t wheel = 0; wheel < CHASSIS_WHEEL_TOTAL_NUM; wheel++)
        chassis.speed.rpm_get[wheel] = chassis_encoder[wheel].filter_rpm;
    
    chassis_mecanum_calc(chassis.speed.x_set, chassis.speed.y_set, chassis.speed.z_set, (int16_t *)&chassis.speed.rpm_set);
    
    for (uint8_t wheel = 0; wheel < CHASSIS_WHEEL_TOTAL_NUM; wheel++)
        chassis.current[wheel] = pid_calc(&pid_wheel_speed[wheel], chassis.speed.rpm_get[wheel], chassis.speed.rpm_set[wheel]);
    
    //power_limit_handler();
    
    set_chassis_current(chassis.current[RF], chassis.current[LF], chassis.current[LR], chassis.current[RR]);
}


/**
  * @brief mecanum chassis velocity decomposition
  * @param input : ?=+vx(mm/s)  ?=+vy(mm/s)  ccw=+vw(deg/s)
  *        output: every wheel speed(rpm)
  * @note  1=RF 2=LF 3=LR 4=RR
  */
void chassis_mecanum_calc(float vx, float vy, float vw, int16_t speed[])
{
    int16_t      wheel_rpm[CHASSIS_WHEEL_TOTAL_NUM];
    float        max = 0;

    value_limit(vx, -chassis.speed.x_max, chassis.speed.x_max);  // mm/s
    value_limit(vy, -chassis.speed.y_max, chassis.speed.y_max);  // mm/s
    value_limit(vw, -chassis.speed.z_max, chassis.speed.z_max);  // deg/s

    // 麦克纳姆轮底盘运动学模型（O-长方形安装方式）
    // 分别以前进、右平移、逆时针为正
    wheel_rpm[RF] = (-vx + vy + vw * CHASSIS_ROTATE_RATIO) * CHASSIS_WHEEL_RPM_RARIO;
    wheel_rpm[LF] = ( vx + vy - vw * CHASSIS_ROTATE_RATIO) * CHASSIS_WHEEL_RPM_RARIO;
    wheel_rpm[LR] = (-vx + vy - vw * CHASSIS_ROTATE_RATIO) * CHASSIS_WHEEL_RPM_RARIO;
    wheel_rpm[RR] = ( vx + vy + vw * CHASSIS_ROTATE_RATIO) * CHASSIS_WHEEL_RPM_RARIO;

    // 轮子逆时针转为正 左轮顺时针为前 所以左轮负为前 计算结果取相反数
    wheel_rpm[LF] *= -1;
    wheel_rpm[LR] *= -1;
    
    // 输出转速限幅
    for (uint8_t i = 0; i < CHASSIS_WHEEL_TOTAL_NUM; i++)
        if (abs(wheel_rpm[i]) > max)
            max = abs(wheel_rpm[i]);
    
    if (max > CHASSIS_WHEEL_MAX_RPM)
    {
        float rate = CHASSIS_WHEEL_MAX_RPM / max;
        for (uint8_t i = 0; i < CHASSIS_WHEEL_TOTAL_NUM; i++)
          wheel_rpm[i] *= rate;
    }
    
    memcpy(speed, wheel_rpm, 4*sizeof(int16_t));
}


float total_cur_limit;
float total_cur;
#define WARNING_ENERGY 40.f

void power_limit_handler(void)
{
//  if (g_err.list[JUDGE_SYS_OFFLINE].err_exist)
//  {
//    //judge system offline, mandatory limit current
//    total_cur_limit = 8000;
//  }
//  else
//  {
    if (power_heat_data.chassisPowerBuffer < WARNING_ENERGY)
      total_cur_limit = 40000.f
                      * ((power_heat_data.chassisPowerBuffer * power_heat_data.chassisPowerBuffer) 
                      /  (WARNING_ENERGY * WARNING_ENERGY));
    else
      total_cur_limit = 40000.f;
//  }
  
  total_cur = fabs(chassis.current[RF]) 
            + fabs(chassis.current[LF]) 
            + fabs(chassis.current[LR]) 
            + fabs(chassis.current[RR]);
  
  if (total_cur > total_cur_limit)
  {
    chassis.current[RF] = chassis.current[RF] / total_cur * total_cur_limit;
    chassis.current[LF] = chassis.current[LF] / total_cur * total_cur_limit;
    chassis.current[LR] = chassis.current[LR] / total_cur * total_cur_limit;
    chassis.current[RR] = chassis.current[RR] / total_cur * total_cur_limit;
  }

}
