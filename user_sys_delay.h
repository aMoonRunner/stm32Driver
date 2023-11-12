
#ifndef __USER_DELAY_H__
#define __USER_DELAY_H__
#include "stm32f1xx_hal.h"
#include "stdint.h"
extern void user_delay_us(uint32_t us);
extern void user_delay_ms(uint32_t ms);

#endif
