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
static TIM_HandleTypeDef* htim_ns_upcounter = NULL;

void Timer_set_ns_upcounter(TIM_HandleTypeDef* htim) {
	htim_ns_upcounter = htim;
}

void delay_ns(uint32_t ns) {
    uint32_t start = htim_ns_upcounter->Instance->CNT;

    while ((uint32_t) (htim_ns_upcounter->Instance->CNT - start) < ns)
        ;;
}
