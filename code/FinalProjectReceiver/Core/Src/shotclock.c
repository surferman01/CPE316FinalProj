/** shotclock.c
 *
 *
 * Author: Ben Soto
 */

#define _SHOTCLOCK_C
#include <stdbool.h>
//#include "Common.h"
#include "shotclock.h"

#include "main.h"

#include "uart.h"
#include "shiftreg.h"
#include "timer.h"
//#include "HAL.h"
//#include "Key.h"
#include "keypad.h"

/* Constant defintions */

/* Static variable defintions */

/* Static function definition */

/* Entry points */

// None of these are running in an IRQ context, so you can do blocking code here

//static char *Incoming_Command;
static volatile int incoming_flg = 0;

static uint8_t Command_Buffer[32] = {0};

static int command_length = 0;
static volatile int return_flg = 0;

// called when a UART character is received
// this would be if we are directly hooked up
// mostly gonna be unused unless sending direct commands
// (dangerous if plugged into STM while connected to shotclock)
void Shotclock_UART_recv_callback(char c) {
	// i want it to fill out a buffer up to 32 characters long,
		// then when the return button is pressed,
		// i want it to send the message

	if (c != '\r' && c != '\n' && c != '\b') {
	        UART_send_char(c); // Echo regular characters
	    }

		// Handle return (\r or \n)
	    if (c == '\r' || c == '\n') {
			if (command_length < MIN_CMD_LEN) {
				UART_send("\n\r");
				UART_send("Command too short, restart");
				UART_send("\n\r");
				command_length = 0;
				memset(Command_Buffer, 0, COMMAND_BUFFER_SIZE);
				return;
			}
			if (strncmp(HORN_STR, (char*)Command_Buffer, strlen(HORN_STR)) == 0) {
				return_flg = 1;
				UART_send("\n\r");
				UART_send("Sending Command: HORN...");
				UART_send("\n\r");
				return;
			}
			if (strncmp(SSEG_STR, (char*)Command_Buffer, strlen(SSEG_STR)) == 0) {
				if (command_length != SSEG_CMD_LEN) {
					UART_send("\n\r");
					UART_send("sseg command format (X = decimal value to display): ssegXXXXXX");
					UART_send("\n\r");
					command_length = 0;
					memset(Command_Buffer, 0, COMMAND_BUFFER_SIZE);
					return;
				}
				return_flg = 1;
				UART_send("\n\r");
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

void keyPressedCallback(char key) {
	if (SHIFT_PROCESSING) {
		UART_send("COMMAND PROCESSING... NOT READING INPUT...\n\r");
		return;
	}
	if (key == '1') {
		UART_send("KEY 1 PRESS\n\r");
		key1();
//		UART_send("KEY 1 PRESS, OUTPUT ENABLE\n\r");
//		ShiftReg_output_enable();
//		UART_send("DONE OUTPUT ENABLE\n\r");
	}
	if (key == '2') {
		UART_send("KEY 2 PRESS\n\r");
		key2();
//		UART_send("KEY 2 PRESS, OUTPUT DISABLE\n\r");
//		ShiftReg_output_disable();
//		UART_send("DONE OUTPUT DISABLE\n\r");
	}
	if (key == '3') {
		UART_send("KEY 3 PRESS\n\r");
		key3();
	}
	if (key == '4') {
		UART_send("KEY 4 PRESS\n\r");
		key4();
	}
	if (key == '5') {
		UART_send("KEY 5 PRESS\n\r");
		key5();
	}
	if (key == '6') {
		UART_send("KEY 6 PRESS\n\r");
		key6();
	}
	if (key == '7') {
		UART_send("KEY 7 PRESS\n\r");
		key7();
	}
	if (key == '8') {
		UART_send("KEY 8 PRESS\n\r");
		key8();
	}
	if (key == '9') {
		UART_send("KEY 9 PRESS\n\r");
		key9();
	}
	if (key == '0') {
		UART_send("KEY 0 PRESS\n\r");
		key0();
	}
	if (key == '*') {
		UART_send("KEY * PRESS\n\r");
		keyS();
	}
	if (key == '#') {
		UART_send("KEY # PRESS\n\r");
		keyP();
	}
}

// called when radio received and has processed a command
// command is 32 bytes
void Shotclock_Radio_recv_callback(uint8_t* command) {
//    Incoming_Command = (char*)command;
//
//    if (strncmp(HORN_STR, Incoming_Command, strlen(HORN_STR)) == 0) {
//        SR_Data[6] = 0xFF;
//        int i;
//        int cur = 0;

        // for (i = 0; i < SHIFT_REG_COUNT; i++) {
        //   ShiftReg_display_digit(&SR_Data, i, i);
        // }
//    }

}

// called in main while loop
void Shotclock_process() {

	// here is the following keypad funcitonality:
	// if you press any number 1, 2, 3, 4, 5, 6,
	// then you will be prompted to press another number
	// the second number you press will set the data in the backend
	// for the index you first pressed to that decimal value
	// now, the next time you perform a shift operation
	// the value at that segment will be set to the second number
	// you pressed on the keypad
	if (sTimer[KEY_SCAN_TIMER] == 0)
		{
			KeypadScan();
			KeyProcess(&keyPressedCallback, NULL, NULL);
			sTimer[KEY_SCAN_TIMER] = KEY_SCAN_TIME;
		}

	// return flag is for UART
	// here if the following functionality for uart:
	// when you type "horn":
	// the horn will be toggled and shifted into the registers to enable it
	// when you type "sseg" followed by 6 integers:
	// the integers you pass will be the values displayed on the SSEG and the index
	// of the integer will determine which SSEG it will display on
	// example:
	// sseg123456
	// then SSEG[0] will show '1'
	// SSEG[1] will show '2'
	// ...
	if (return_flg == 1) {
		return_flg = 0;

		if (strncmp(HORN_STR, (char*)Command_Buffer, strlen(HORN_STR)) == 0) {
			Command_Type = HORN_TYPE;
		}
		if (strncmp(SSEG_STR, (char*)Command_Buffer, strlen(SSEG_STR)) == 0) {
			Command_Type = SSEG_TYPE;
		}

		switch (Command_Type) {
		case HORN_TYPE:
//			UART_send("SENT HORN ACTION IN SWITCH\n\r");
			horn();
			break;
		case SSEG_TYPE:
//			UART_send("SEND SSEG ACTION IN SWITCH\n\r");
			sseg();
			break;
		default:
			break;
		}
		command_length = 0;
		memset(Command_Buffer, 0, COMMAND_BUFFER_SIZE);
//		return_flg = 0;
	}

//	if (Key_isPushed(&hal_p->key1))
//		  {
//		      UART_send("1 is pushed.\n\r");
//		      UART_send_newline();
//			  AllKeyRestart();
//
//		  }
    // ShiftReg_shift_in_data(test_data, SHIFT_REG_COUNT);
}

void horn() {
	UART_send("TOGGLING HORN\n\r");
	// toggle the horn
	SR_Data[HORN] ^= 0xFF;
	do_shift();
	return;
}

void sseg() {
	int loc;
	// next 6 bytes are the values (in decimal) that each sseg will display
	// the one it displays on is the index that its at
	// SHIFT_REG_COUNT -
	for (loc = 0; loc < SSEG_COUNT; loc++) {
		SR_Data[loc] = SSEG_TT[Command_Buffer[loc + strlen(SSEG_STR)] - '0'];
	}
	do_shift();
	return;
}

void do_shift() {
	UART_send("\n\r");
	UART_send("SHIFTING IN DATA");
	UART_send("\n\r");
	UART_send("WAIT 2 SECONDS...");
	UART_send("\n\r");

	ShiftReg_shift_in_data(SR_Data, 7);
	UART_send("DONE SHIFTING DATA\n\r");
}

/* Static function defintions */
