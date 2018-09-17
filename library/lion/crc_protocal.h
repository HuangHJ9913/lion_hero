#ifndef _CRC_PROTOCAL_H_
#define _CRC_PROTOCAL_H_

#include "main.h"

unsigned char get_crc8_check_sum(unsigned char *,unsigned int,unsigned char);
unsigned int verify_crc8_check_sum(unsigned char *, unsigned int);
void append_crc8_check_sum(unsigned char *, unsigned int);

uint16_t get_crc16_check_sum(uint8_t *,uint32_t,uint16_t);
uint32_t verify_crc16_check_sum(uint8_t *, uint32_t );
void append_crc16_check_sum(uint8_t *,uint32_t);

#endif
