#ifndef _COMMON_H_
#define _COMMON_H_

#include "main.h"

// ȡֵ����
#define value_limit(val, min, max)\
if(val <= min)\
{\
	val = min;\
}\
else if(val >= max)\
{\
	val = max;\
}\

// ������ƽ����
float invSqrt(float x);

// ����byte�����float֮���ת��
typedef __packed union
{
    float f;
    uint8_t a[sizeof(float)];
}A2F_u;

#define PI              3.142f
#define DEG_TO_RAD      0.017f     // = 2 * PI / 360.f
#define RAD_TO_DEG      57.29f     // = 360.f / (2 * PI)
#define PERIMETER_COEF  6.284f     // = 360 * RAD_TO_DEG
#define SEC_PER_MIN     60.f

#endif
