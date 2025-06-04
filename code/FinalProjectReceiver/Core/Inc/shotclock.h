/** shotclock.h
 *
 *
 * Author: Ben Soto
*/


#ifndef INC_SHOTCLOCK_H_
#define INC_SHOTCLOCK_H_

#ifdef _SHOTCLOCK_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"

SCOPE void Shotclock_process();
SCOPE void Shotclock_Radio_recv_callback(uint8_t *command);
SCOPE void Shotclock_UART_recv_callback(char c);

#undef SCOPE
#endif /* INC_SHOTCLOCK_H_ */
