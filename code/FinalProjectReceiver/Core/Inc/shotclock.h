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
#include <string.h>

#define SEG_TOP         	0x80
#define SEG_TOP_RIGHT   	0x40
#define SEG_BOTTOM_RIGHT 	0x20
#define SEG_BOTTOM      	0x10
#define SEG_BOTTOM_LEFT 	0x08
#define SEG_TOP_LEFT    	0x04
#define SEG_CENTER      	0x02
#define SEG_UNUSED      	0x01

#define HORN_STR 		"horn"
#define HORN_ON 		0xFF
#define HORN_OFF 		0x00
#define SSEG_STR 		"sseg"
#define SHIFT_REG_COUNT 7
#define SSEG_COUNT		6

#define COMMAND_MAX_LENGTH 31
#define COMMAND_BUFFER_SIZE (COMMAND_MAX_LENGTH + 1)

#define NONE			0
#define HORN_TYPE		1
#define SSEG_TYPE		2

#define SSEG0			0
#define SSEG1			1
#define SSEG2			2
#define SSEG3			3
#define SSEG4			4
#define SSEG5			5
#define HORN			7

#define MIN_CMD_LEN		4
#define HORN_CMD_LEN	4
#define SSEG_CMD_LEN	10

//static int Command_Type = NONE;

static const uint8_t SSEG_TT[10] = {
   // 0 (Decimal 0: 0xFC)
   SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT | SEG_BOTTOM | SEG_BOTTOM_LEFT | SEG_TOP_LEFT,
   // 1 (Decimal 1: 0x60)
   SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT,
   // 2 (Decimal 2: 0xDA)
   SEG_TOP | SEG_TOP_RIGHT | SEG_CENTER | SEG_BOTTOM_LEFT | SEG_BOTTOM,
   // 3 (Decimal 3: 0xF2)
   SEG_TOP | SEG_TOP_RIGHT | SEG_CENTER | SEG_BOTTOM_RIGHT | SEG_BOTTOM,
   // 4 (Decimal 4: 0x66)
   SEG_TOP_LEFT | SEG_CENTER | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT,
   // 5 (Decimal 5: 0xB6)
   SEG_TOP | SEG_TOP_LEFT | SEG_CENTER | SEG_BOTTOM_RIGHT | SEG_BOTTOM,
   // 6 (Decimal 6: 0xBE)
   SEG_TOP | SEG_TOP_LEFT | SEG_CENTER | SEG_BOTTOM_RIGHT | SEG_BOTTOM | SEG_BOTTOM_LEFT,
   // 7 (Decimal 7: 0xE0)
   SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT,
   // 8 (Decimal 8: 0xFE)
   SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT | SEG_BOTTOM | SEG_BOTTOM_LEFT | SEG_TOP_LEFT | SEG_CENTER,
   // 9 (Decimal 9: 0xF6)
   SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT | SEG_BOTTOM | SEG_TOP_LEFT | SEG_CENTER,
};

// this is just random garbage data
static const uint8_t test_data[SHIFT_REG_COUNT] = {
    0x55, // 1
    0xAA, // 2
    0xCC, // 3
    0x33, // 4
    0xFF, // 5
    0x00, // 6
    0x00, // 7
};

// this is the global shift reg data that will be preserved once set
SCOPE uint8_t SR_Data[SHIFT_REG_COUNT];

// this is for the UART to differentiate commands
SCOPE uint8_t Command_Type;


SCOPE void Shotclock_process();
SCOPE void Shotclock_Radio_recv_callback(uint8_t *command);
SCOPE void Shotclock_UART_recv_callback(char c);
SCOPE void horn();
SCOPE void sseg();
SCOPE void do_shift();


#undef SCOPE
#endif /* INC_SHOTCLOCK_H_ */
