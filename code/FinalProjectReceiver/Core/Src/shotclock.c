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

#define HORN_STR "horn"
#define HORN_ON 0xFF
#define HORN_OFF 0x00
#define SSEG_SRT "sseg"
#define SHIFT_REG_COUNT 7

/* Static variable defintions */

/* Static function definition */

/* Entry points */

static uint8_t test_data[SHIFT_REG_COUNT] = {
    0x55, // 1
    0xAA, // 2
    0xCC, // 3
    0x33, // 4
    0xFF, // 5
    0x00, // 6
    0x00, // 7
};

uint8_t SSEG_Data[SHIFT_REG_COUNT];


// None of these are running in an IRQ context, so you can do blocking code here

static char *Incoming_Command;
static volatile int incoming_flg = 0;

// called when a UART character is received
// this would be if we are directly hooked up
// mostly gonna be unused unless sending direct commands
// (dangerous if plugged into STM while connected to shotclock)
void Shotclock_UART_recv_callback(char c) {

}

// called when radio received and has processed a command
// command is 32 bytes
void Shotclock_Radio_recv_callback(uint8_t* command) {
    Incoming_Command = (char*)command;

    if (strncmp(HORN_STR, Incoming_Command, strlen(HORN_STR)) == 0) {
        SSEG_Data[6] = 0xFF;
        int i;
        int cur = 0;

        // for (i = 0; i < SHIFT_REG_COUNT; i++) {
        //   ShiftReg_display_digit(&SSEG_Data, i, i);
        // }
    }

}

// called in main while loop
void Shotclock_process() {
    ShiftReg_shift_in_data(test_data, SHIFT_REG_COUNT);
}

/* Static function defintions */
