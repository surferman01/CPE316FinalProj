/** shotclock.c
 *
 *
 * Author: Ben Soto
 */

#define _SHOTCLOCK_C

#include "shotclock.h"

#include "main.h"

#include "uart.h"
#include "shiftreg.h"
#include "timer.h"

/* Constant defintions */

/* Static variable defintions */

/* Static function definition */

/* Entry points */

// None of these are running in an IRQ context, so you can do blocking code here

// called when a UART character is received
void Shotclock_UART_recv_callback(char c) {

}

// called when radio received and has processed a command
// command is 32 bytes
void Shotclock_Radio_recv_callback(uint8_t* command) {

}

// called in main while loop
void Shotclock_process() {

}

/* Static function defintions */
