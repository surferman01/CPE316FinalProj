/** shiftreg.h
 * Implements a driver for the TPIC6595 shift register using Open Drain GPIOs
 *
 * Created by Alex Castellar
*/

/** Circuit and IOC Setup
 * The TPIC6595 requires 5V but STM32 GPIOs can only drive 3.3V
 * Thus, the GPIOs must be Open Drain with an external 5V pull-up
 *
 * In the GPIO configuration:
 * 	set GPIO mode to Output Open Drain
 * 	set Maximum Output Speed to Very High (if possible)
 *
 * Ensure SHIFTREG_*_SPEED_SETTING matches Maximum Output Speed
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

// PA2 -> RCLK
#define SHIFTREG_RCK_GPIO_EXPANDER GPIOA
#define SHIFTREG_RCK_GPIO_PIN GPIO_PIN_2
#define SHIFTREG_RCK_SPEED_SETTING GPIO_SPEED_FREQ_VERY_HIGH

// PA3 -> SRCK
#define SHIFTREG_SRCK_GPIO_EXPANDER GPIOA
#define SHIFTREG_SRCK_GPIO_PIN GPIO_PIN_3
#define SHIFTREG_SRCK_SPEED_SETTING GPIO_SPEED_FREQ_VERY_HIGH

// PB9 -> SIN
#define SHIFTREG_SIN_GPIO_EXPANDER GPIOB
#define SHIFTREG_SIN_GPIO_PIN GPIO_PIN_9
#define SHIFTREG_SIN_SPEED_SETTING GPIO_SPEED_FREQ_VERY_HIGH

// PB8 -> nG
#define SHIFTREG_NG_GPIO_EXPANDER GPIOB
#define SHIFTREG_NG_GPIO_PIN GPIO_PIN_8
#define SHIFTREG_NG_SPEED_SETTING GPIO_SPEED_FREQ_VERY_HIGH

#undef SCOPE
#endif /* INC_SHIFTREG_H_ */
