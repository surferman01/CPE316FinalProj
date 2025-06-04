/*
 * Display.h
 *
 *  Created on: Mar 9, 2025
 *      Author: peterh17
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#ifdef _DISPLAY_C
   #define SCOPE
#else
   #define SCOPE extern
#endif



SCOPE void DisplayClear(UART_HandleTypeDef*);
SCOPE void DisplayHome(UART_HandleTypeDef*);


SCOPE const char clrscr[]
#ifdef _DISPLAY_C
= {
		0x1B,
		'[',
		'2',
		'J',
		0x00
}
#endif
; // Clear the screen

SCOPE const char cursorHome[]
#ifdef _DISPLAY_C
= {
		0x1B,
		'[',
		'H',
		0x00
}
#endif
; // Cursor home




#undef SCOPE
#endif /* INC_DISPLAY_H_ */
