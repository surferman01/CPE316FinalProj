/** shiftreg.h
 * Implements a driver for the TPIC6595 shift register using Open Drain GPIOs
 *
 * Created by Alex Castellar
*/

/** Circuit and IOC Setup
 * In the circuit
 * 	The TPIC6595 requires 5V but STM32 GPIOs can only drive 3.3V
 * 	Thus, the GPIOs must be Open Drain with an external 5V pull-up
 * 	DO NOT USE a 150 ohm pullup, 33 mA is too much for the GPIO drivers
 *
 * In the GPIO configuration:
 * 	set GPIO mode to Output Open Drain
 * 	set Maximum Output Speed to Very High (if possible)
 *
 * Ensure you set SHIFTREG_*:
 *  SHIFTREG_*_GPIO_EXPANDER to the expander being used (GPIOX)
 *  SHIFTREG_*_GPIO_PIN to the pin number used (GPIO_PIN_X)
 * 	SHIFTREG_*_SPEED_SETTING to the set Maximum Output Speed
 * 	SHIFTREG_*_PULLUP_OHMS to the external 5V pullup ohm value
 *
 * This assumes that the TPIC6595s are daisy chained and properly connected
 *
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

/* Configuration */


#undef SCOPE
#endif /* INC_SHIFTREG_H_ */
