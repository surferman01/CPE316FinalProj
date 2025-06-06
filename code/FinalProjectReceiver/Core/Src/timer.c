#define _TIMER_C

#include "timer.h"

// timer
void Timer_handle(void)
{
	unsigned short sIndex;
	//__disable_irq();

	for (sIndex=0; sIndex<NUMBER_OF_TIMERS; sIndex++)
	{
		if (sTimer[sIndex] != 0)
			sTimer[sIndex]--;
	}
	//__enable_irq();
}

// delay
static TIM_HandleTypeDef* htim_us_upcounter = NULL;

void Timer_set_us_upcounter(TIM_HandleTypeDef* htim) {
	htim_us_upcounter = htim;
}

void delay_us(uint32_t us) {
    uint32_t start = htim_us_upcounter->Instance->CNT;

    while ((uint32_t) (htim_us_upcounter->Instance->CNT - start) < us) {

    }
}
