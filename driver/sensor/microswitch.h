#ifndef __MICROSWITCH_H__
#define __MICROSWITCH_H__

#include "main.h"
#define MICROSWITCH_TOTAL_NUM       4


void Microswitch_Configuration(void);
uint8_t get_microswitch_nadir(uint8_t);
uint8_t get_microswitch_peak(uint8_t);
#endif
