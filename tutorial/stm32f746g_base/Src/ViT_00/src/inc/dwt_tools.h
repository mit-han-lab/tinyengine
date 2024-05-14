#pragma once
#include "stm32f746xx.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tp_dwt_time {
    uint32_t fcpu;
    int s;
    int ms;
    int us;
};

extern uint32_t SystemCoreClock;

void dwt_init()
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

#ifdef STM32F7
    DWT->LAR = 0xC5ACCE55;
#endif

    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk | DWT_CTRL_CPIEVTENA_Msk;
}

void dwt_reset(void)
{
    DWT->CYCCNT = 0; /* Clear DWT cycle counter */
}

uint32_t dwt_get_cycles(void)
{
    return DWT->CYCCNT;
}

int dwt_cycles_to_time(uint64_t clks, struct tp_dwt_time *t)
{
    if (!t)
        return -1;
    uint32_t fcpu = SystemCoreClock;
    uint64_t s  = clks / fcpu;
    uint64_t ms = (clks * 1000) / fcpu;
    uint64_t us = (clks * 1000 * 1000) / fcpu;
    ms -= (s * 1000);
    us -= (ms * 1000 + s * 1000000);
    t->fcpu = fcpu;
    t->s = s;
    t->ms = ms;
    t->us = us;
    return 0;
}


float dwt_cycles_to_float_ms(uint64_t clks)
{
    float res;
    float fcpu = (float) SystemCoreClock;
    res = ((float)clks * (float)1000.0) / fcpu;
    return res;
}

#ifdef __cplusplus
}
#endif