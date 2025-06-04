/** shotclock.c
 *
 *
 * Author: Ben Soto
 */

#define _CONTROLLER_C

#include <controller.h>

#include "main.h"

#include "transmitter.h"
#include "uart.h"
#include "timer.h"

/* Constant defintions */

/* Static variable defintions */

// static uint8_t test_data[32] = { 0 };

/* Static function definition */

/* Entry points */

// None of these are running in an IRQ context, so you can do blocking code here

// called when a UART character is received
void Controller_UART_recv_callback(char c) {

}

// NOT IMPLEMENTED (YET)
void Controller_Keypad_callback(char c) {

}

// called in main while loop
void Controller_process() {
	// Transmitter_send(test_data);
	// HAL_Delay(100);
}

/* Static function defintions */
