/** shiftreg_conf.h
 * Configuration options for TPIC6595 shift register driver
 *
 * Author: Alex Castellar
*/

/** Circuit, IOC and Configuration Setup
 * In the circuit
 * 	The TPIC6595 requires 5V but STM32 GPIOs can only drive 3.3V
 * 	Thus, the GPIOs must be Open Drain with an external 5V pull-up
 * 	A 1k pull-up is probably a good number
 *
 * It is assumed that the TPIC6595s are daisy chained and properly connected
 *
 * In the GPIO configuration:
 * 	set GPIO mode to Output Open Drain
 * 	set Maximum Output Speed to Very High (if possible)
 *
 * Ensure you set SHIFTREG_* in shiftreg_conf.h:
 *  _GPIO_EXPANDER to the expander being used (GPIOX)
 *  _GPIO_PIN to the pin number used (GPIO_PIN_X)
 *  _SPEED_SETTING to the set Maximum Output Speed
 * 	_PULLUP_OHMS to the external 5V pullup ohm value
 * 	_CAPACITANCE_PICOFARADS to estimate of capacitance in the circuit
 *
 */

/* ****** CAUTION ******
 *
 * DO NOT USE a 150 ohm pullup, 33 mA is too much for the GPIO drivers
 * The pins used MUST BE 5V TOLERANT (FT)
 *  in the stm32l476rg reference manual, Table 16 (STM32L476xx pin definitions):
 * 	NOT 5V TOLERANCE: PA3, PA4, PA5, PB0
 * 	All other GPIOs on the LQFP64 package are FT
 *
 */

#ifndef INC_SHIFTREG_CONF_H_
#define INC_SHIFTREG_CONF_H_

// PA2 -> RCLK
#define SHIFTREG_RCK_GPIO_EXPANDER 				GPIOA
#define SHIFTREG_RCK_GPIO_PIN 					GPIO_PIN_2
#define SHIFTREG_RCK_SPEED_SETTING 				GPIO_SPEED_FREQ_VERY_HIGH
#define SHIFTREG_RCK_EXTERNAL_PULLUP_OHMS 		1000	// ohms
#define SHIFTREG_RCK_CAPACITANCE_PICOFARADS 	50		// pF

// PA3 -> SRCK
#define SHIFTREG_SRCK_GPIO_EXPANDER 			GPIOA
#define SHIFTREG_SRCK_GPIO_PIN 					GPIO_PIN_3
#define SHIFTREG_SRCK_SPEED_SETTING 			GPIO_SPEED_FREQ_VERY_HIGH
#define SHIFTREG_SRCK_EXTERNAL_PULLUP_OHMS 		1000	// ohms
#define SHIFTREG_SRCK_CAPACITANCE_PICOFARADS 	50		// pF

// PB9 -> SIN
#define SHIFTREG_SIN_GPIO_EXPANDER 				GPIOB
#define SHIFTREG_SIN_GPIO_PIN 					GPIO_PIN_9
#define SHIFTREG_SIN_SPEED_SETTING 				GPIO_SPEED_FREQ_VERY_HIGH
#define SHIFTREG_SIN_EXTERNAL_PULLUP_OHMS 		1000	// ohms
#define SHIFTREG_SIN_CAPACITANCE_PICOFARADS 	50		// pF

// PB8 -> nG
#define SHIFTREG_NG_GPIO_EXPANDER 				GPIOB
#define SHIFTREG_NG_GPIO_PIN 					GPIO_PIN_8
#define SHIFTREG_NG_SPEED_SETTING 				GPIO_SPEED_FREQ_VERY_HIGH
#define SHIFTREG_NG_EXTERNAL_PULLUP_OHMS 		1000	// ohms
#define SHIFTREG_NG_CAPACITANCE_PICOFARADS 		50 		// pF

#endif /* INC_SHIFTREG_CONF_H_ */
