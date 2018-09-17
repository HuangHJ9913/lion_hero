#ifndef __MPU6050_H__
#define __MPU6050_H__
#include "main.h"

typedef struct __MPU6050_REAL_Data__
{
    float acc_x;  // X�����ϵļ��ٶ�
    float acc_y;  // Y�����ϵļ��ٶ�
    float acc_z;  // Z�����ϵļ��ٶ�
    float gyro_x;   // X�����ϵĽǼ��ٶ�
    float gyro_y;   // Y�����ϵĽǼ��ٶ�
    float gyro_z;   // Z�����ϵĽǼ��ٶ�
	  float mag_x;   	// X�����ϵĽǶ�
    float mag_y;   	// Y�����ϵĽǶ�
    float mag_z;   	// Z�����ϵĽǶ�
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
