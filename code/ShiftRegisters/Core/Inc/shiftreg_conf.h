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
 * DO NOT USE: less than 625ohm pullup, max 8mA for 5V the GPIO drivers
 *  see stm32l476xx reference manual, figure 30 (I/O input characteristics)
 *  "The GPIOs (general purpose input/outputs) can sink or source up to ±8 mA,
 *   and sink or source up to ± 20 mA **(with a relaxed VOL/VOH)**"
 *
 *
 * DO NOT USE: PA2, PA3, PA4, PA5, PA13, PA14, PB0, PB3
 * 	The pins used MUST BE 5V TOLERANT (FT)
 *   see stm32l476xx reference manual, table 16 (STM32L476xx pin definitions):
 * 	 NOT 5V TOLERANCE: PA3, PA4, PA5, PB0
 * 	 All other GPIOs on the LQFP64 package are FT
 *  The pins used MUST NOT be already used by ST-LINK at 3.3V
 * 	 see mb1136 user manual, bolded entries in table 10 (solder bridges):
 * 	 USED: PA13, PA14, PB3, PA2, PA3
 *
 *
 */

#ifndef INC_SHIFTREG_CONF_H_
#define INC_SHIFTREG_CONF_H_

// PB4 -> RCLK
#define SHIFTREG_RCK_GPIO_EXPANDER 				GPIOB
#define SHIFTREG_RCK_GPIO_PIN 					GPIO_PIN_4
#define SHIFTREG_RCK_SPEED_SETTING 				GPIO_SPEED_FREQ_VERY_HIGH
#define SHIFTREG_RCK_EXTERNAL_PULLUP_OHMS 		1000	// ohms
#define SHIFTREG_RCK_CAPACITANCE_PICOFARADS 	50		// pF

// PB8 -> SRCK
#define SHIFTREG_SRCK_GPIO_EXPANDER 			GPIOB
#define SHIFTREG_SRCK_GPIO_PIN 					GPIO_PIN_8
#define SHIFTREG_SRCK_SPEED_SETTING 			GPIO_SPEED_FREQ_VERY_HIGH
#define SHIFTREG_SRCK_EXTERNAL_PULLUP_OHMS 		1000	// ohms
#define SHIFTREG_SRCK_CAPACITANCE_PICOFARADS 	50		// pF

// PB9 -> SIN
#define SHIFTREG_SIN_GPIO_EXPANDER 				GPIOB
#define SHIFTREG_SIN_GPIO_PIN 					GPIO_PIN_9
#define SHIFTREG_SIN_SPEED_SETTING 				GPIO_SPEED_FREQ_VERY_HIGH
#define SHIFTREG_SIN_EXTERNAL_PULLUP_OHMS 		1000	// ohms
#define SHIFTREG_SIN_CAPACITANCE_PICOFARADS 	50		// pF

// PB5 -> nG
#define SHIFTREG_NG_GPIO_EXPANDER 				GPIOB
#define SHIFTREG_NG_GPIO_PIN 					GPIO_PIN_5
#define SHIFTREG_NG_SPEED_SETTING 				GPIO_SPEED_FREQ_VERY_HIGH
#define SHIFTREG_NG_EXTERNAL_PULLUP_OHMS 		1000	// ohms
#define SHIFTREG_NG_CAPACITANCE_PICOFARADS 		50 		// pF

#endif /* INC_SHIFTREG_CONF_H_ */
