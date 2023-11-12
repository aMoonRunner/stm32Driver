

#include "user_sys_delay.h"

void user_delay_us(uint32_t us)
{
  uint32_t startval, tickn, delays, endvar;
  static uint32_t sysFreq_MHz = 0;
  startval = SysTick->VAL;
  tickn = HAL_GetTick();
  if (sysFreq_MHz == 0)
  { // 只运行一次
    sysFreq_MHz = HAL_RCC_GetSysClockFreq() / 1e6;
  }
  if (us > 2)
  {
    us = us - 1;
  }
  else
  {
    us = 0;
  }
  delays = us * sysFreq_MHz;
  if (delays > startval)
  {
    while (HAL_GetTick() == tickn)
    {
    }
    // endvar = sysFreq_MHz * 1000 / HAL_TICK_FREQ_1KHZ + startval - delays;
    endvar = sysFreq_MHz * 1000 + startval - delays;
    while (endvar < SysTick->VAL)
    {
    }
  }
  else
  {
    endvar = startval - delays;
    while (endvar < SysTick->VAL && HAL_GetTick() == tickn)
    {
    }
  }
}
void user_delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}
