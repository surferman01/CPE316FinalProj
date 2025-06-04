/** uart.h
 * Implements higher level UART send and receive with callbacks
 * 
 * Author: Alex Castellar
*/

#ifndef INC_UART_H_
#define INC_UART_H_

#ifdef _UART_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"

SCOPE void UART_setup(UART_HandleTypeDef *huartRef);

SCOPE void UART_irq();
SCOPE void UART_process(void (*uartCallback)(char c));

SCOPE void UART_send(char *str);
SCOPE void UART_send_char(char c);
SCOPE void UART_send_newline();
SCOPE void UART_send_clear_screen();
SCOPE void UART_send_cursor_pos(int row, int col);

#undef SCOPE
#endif /* INC_TIMER_H_ */
