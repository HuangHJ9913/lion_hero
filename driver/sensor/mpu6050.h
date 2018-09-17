#ifndef __MPU6050_H__
#define __MPU6050_H__
#include "main.h"

typedef struct __MPU6050_REAL_Data__
{
    float acc_x;  // X方向上的加速度
    float acc_y;  // Y方向上的加速度
    float acc_z;  // Z方向上的加速度
    float gyro_x;   // X方向上的角加速度
    float gyro_y;   // Y方向上的角加速度
    float gyro_z;   // Z方向上的角加速度
	  float mag_x;   	// X方向上的角度
    float mag_y;   	// Y方向上的角度
    float mag_z;   	// Z方向上的角度
}MPU6050_REAL_DATA;

//define the eluer angle
typedef struct AHRS
{
	float pitch;
	float roll;
	float yaw;
}AHRS;

extern AHRS ahrs;

extern volatile MPU6050_REAL_DATA   imu;

void MPU6050_Initialize(void);

void MPU6050_getlastMotion(float* ax, float* ay, float* az,float* wax, float* way,float* waz,float* wx, float* wy, float* wz);
void MPU6050_getMotion(float* ax, float* ay, float* az, float* wax, float* way, float* waz,float* wx, float* wy, float* wz);

void MPU6050_getSpeed(float* Spx,float* Spy,float* Acc_x,float* Acc_y,float* last_Acc_x,float* last_Acc_y);
void MPU6050_getlastDisplacement(float* Disx,float* Disy);
void MPU6050_getDisplacement(float* Disx,float* Disy,float* Spx,float* Spy);
void MPU6050_getlastSpeed(float* Spx,float* Spy);

#endif
