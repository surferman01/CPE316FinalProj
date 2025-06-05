/** shotclock.c
 *
 *
 * Author: Ben Soto
 */

#define _CONTROLLER_C
#define COMMAND_MAX_LENGTH 31
#define COMMAND_BUFFER_SIZE (COMMAND_MAX_LENGTH + 1)
#define HORN_STR "horn"
#define SSEG_STR "sseg"

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
static uint8_t Command_Buffer[32] = {0};
// static uint8_t Processed_Buffer[32] = {0};


static int command_length = 0;
static volatile int return_flg = 0;
// maybe we need a flag to tell whether we are processing cmd or not
// static volatile int processing_flg = 0
// called when a UART character is received
void Controller_UART_recv_callback(char c) {
	// i want it to fill out a buffer up to 32 characters long,
	// then when the return button is pressed,
	// i want it to send the message

	if (c != '\r' && c != '\n' && c != '\b') {
        UART_send_char(c); // Echo regular characters
    }

	// Handle return (\r or \n)
    if (c == '\r' || c == '\n') {
		if (command_length < 4) {
			UART_send("\n\r");
			UART_send("Command too short, restart");
			UART_send("\n\r");
			command_length = 0;
			memset(Command_Buffer, 0, COMMAND_BUFFER_SIZE);
			return;
		}
		if (strncmp(HORN_STR, (char*)Command_Buffer, strlen(HORN_STR)) == 0) {
			return_flg = 1; 
			UART_send("Sending Command: HORN...");
			UART_send("\n\r");
			return;
		}
		if (strncmp(SSEG_STR, (char*)Command_Buffer, strlen(SSEG_STR)) == 0) {
			return_flg = 1; 
			UART_send("Sending Command: SSEG...");
			UART_send("\n\r");
			return;
		}
		UART_send("\n\r");
		UART_send("Invalid command. Try again.");
		UART_send("\n\r");
		command_length = 0;
		memset(Command_Buffer, 0, COMMAND_BUFFER_SIZE);
        return;
    }

	// for backspace
	if (c == '\b') {
        if (command_length > 0) {
            command_length--;
			// move the cursor back, put a space, then move back again
            UART_send_char('\b');
            UART_send_char(' ');
            UART_send_char('\b');
        }
        return;
    }

	// if the buffer is full, then say so
	if (command_length >= COMMAND_MAX_LENGTH) {
        UART_send("\r\nMax Length Command! Press Enter to send current command.\r\n>");
		return;
	}
	Command_Buffer[command_length] = (uint8_t)c;
	command_length++;
}

// NOT IMPLEMENTED (YET)
void Controller_Keypad_callback(char c) {


}

// void Process_Input() {
// 	int i;
// 	for (i = 0; i < command_length; i++) {

// 	}
// }

// called in main while loop
void Controller_process() {
	if (return_flg == 1) {
		return_flg = 0;
		// Process_Input();
		Command_Buffer[command_length] = '\0';
		// here i want to send the command buffer
		Transmitter_send(Command_Buffer);
		// Transmitter_send(test_data);
		command_length = 0;
		memset(Command_Buffer, 0, COMMAND_BUFFER_SIZE);
		// memset(Processed_Buffer, 0, COMMAND_BUFFER_SIZE);
		UART_send("> ");
	}
	// HAL_Delay(100);
}

/* Static function defintions */
