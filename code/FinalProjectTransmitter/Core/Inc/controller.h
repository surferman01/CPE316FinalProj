/** controller.h
 *
 *
 * Author: Ben Soto
*/

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#ifdef _CONTROLLER_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"

SCOPE void Controller_process();
SCOPE void Controller_UART_recv_callback(char c);

// NOT IMPLEMENTED (YET)
SCOPE void Controller_Keypad_callback(char c);

#undef SCOPE
#endif /* INC_CONTROLLER_H_ */
