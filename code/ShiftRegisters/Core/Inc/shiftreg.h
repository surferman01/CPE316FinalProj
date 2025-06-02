/** shiftreg.h
 * Implements a driver for the TPIC6595 shift register using Open Drain GPIOs
 * See shiftreg_conf.h for configuration and setup of circuit and IOC
 *
 * Author: Alex Castellar
*/

#ifndef INC_SHIFTREG_H_
#define INC_SHIFTREG_H_

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
#endif /* INC_SHIFTREG_H_ */
