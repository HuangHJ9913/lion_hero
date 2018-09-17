#ifndef _SERVO_H_
#define _SERVO_H_

#include "main.h"

void Servo_Configuration(void);
void set_servo_angle(uint8_t ,float);
void servo_forward(void);
void servo_backward(void);
#endif
