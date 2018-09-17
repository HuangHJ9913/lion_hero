#ifndef _COMMON_H_
#define _COMMON_H_
#include "stm32f4xx.h"
#define SW_MODE RELEASE
#define RELEASE 0
#define DEBUG 1

typedef void(*VoidFun_t)(void);
typedef void(*U32pVoidFun_t) (uint32_t, void *p_arg);
typedef void(*U8pU8Fun_t) (uint8_t, uint8_t *p_arg);

#endif

