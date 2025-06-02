/** radio.h
 * Implements a driver for the nRF24L01 transceiver
 *
 * Created by Alex Castellar
*/


#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#ifdef _SHIFTREG_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"

SCOPE void ShiftReg_output_enable();
SCOPE void ShiftReg_output_disable();

SCOPE void ShiftReg_shift_in_data(uint8_t *data, int shiftreg_count);

#undef SCOPE
#endif /* INC_RADIO_H_ */
