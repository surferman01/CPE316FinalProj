/** timer.h
 * Implements timers using timer interrupts and ns delay with an upcounting timer
 * 
 * Modified by Alex Castellar
 * Original Code by Professor Han
*/

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#ifdef _TIMER_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"


#include <stdint.h>

#define NUMBER_OF_TIMERS			0

//#define SHIFT_TEST_TIMER			0
//#define SHIFT_TEST_TIME			1000

SCOPE unsigned short sTimer[NUMBER_OF_TIMERS];

SCOPE void Timer_handle(void);

SCOPE void Timer_set_ns_upcounter(TIM_HandleTypeDef* htim);
SCOPE void delay_ns(uint32_t ns);

#undef SCOPE
#endif /* INC_TIMER_H_ */
