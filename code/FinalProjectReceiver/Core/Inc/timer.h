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

#define NUMBER_OF_TIMERS			3

#define KEY_SCAN_TIMER           	0
#define KEY_REPEAT_TIMER			1
#define KEY_WAIT_REPEAT_TIMER		2


#define KEY_SCAN_TIME            10
#define KEY_REPEAT_TIME          333
#define KEY_WAIT_REPEAT_TIME     1000

//#define SHIFT_TEST_TIMER			0
//#define SHIFT_TEST_TIME			1000

SCOPE unsigned short sTimer[NUMBER_OF_TIMERS];

SCOPE void Timer_handle(void);

SCOPE void Timer_set_us_upcounter(TIM_HandleTypeDef* htim);
#define DELAY_NS(ns) delay_us(ns/1000)
SCOPE void delay_us(uint32_t us);

#undef SCOPE
#endif /* INC_TIMER_H_ */
