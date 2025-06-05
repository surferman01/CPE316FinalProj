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

#define SEG_TOP         (1 << 7)
#define SEG_TOP_RIGHT   (1 << 6)
#define SEG_BOTTOM_RIGHT (1 << 5)
#define SEG_BOTTOM      (1 << 4)
#define SEG_BOTTOM_LEFT (1 << 3)
#define SEG_TOP_LEFT    (1 << 2)
#define SEG_CENTER      (1 << 1)
#define SEG_UNUSED      (1 << 0)

static const uint8_t SSEG_TT[10] = {
   // 0
   SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT | SEG_BOTTOM | SEG_BOTTOM_LEFT | SEG_TOP_LEFT,
   // 1
   SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT,
   // 2
   SEG_TOP | SEG_TOP_RIGHT | SEG_CENTER | SEG_BOTTOM_LEFT | SEG_BOTTOM,
   // 3
   SEG_TOP | SEG_TOP_RIGHT | SEG_CENTER | SEG_BOTTOM_RIGHT | SEG_BOTTOM,
   // 4
   SEG_TOP_LEFT | SEG_CENTER | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT,
   // 5
   SEG_TOP | SEG_TOP_LEFT | SEG_CENTER | SEG_BOTTOM_RIGHT | SEG_BOTTOM,
   // 6
   SEG_TOP | SEG_TOP_LEFT | SEG_CENTER | SEG_BOTTOM_RIGHT | SEG_BOTTOM | SEG_BOTTOM_LEFT,
   // 7
   SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT,
   // 8
   SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT | SEG_BOTTOM | SEG_BOTTOM_LEFT | SEG_TOP_LEFT | SEG_CENTER,
   // 9
   SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT | SEG_BOTTOM | SEG_TOP_LEFT | SEG_CENTER,
};

SCOPE void Shotclock_process();
SCOPE void Shotclock_Radio_recv_callback(uint8_t *command);
SCOPE void Shotclock_UART_recv_callback(char c);

#undef SCOPE
#endif /* INC_SHOTCLOCK_H_ */
