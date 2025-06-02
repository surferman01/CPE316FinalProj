/** shiftreg.c
 * Implements a driver for the TPIC6595 shift register using Open Drain GPIOs
 *
 * Author: Alex Castellar
*/

#define _SHIFTREG_C

#include "shiftreg.h"
#include "shiftreg_conf.h"

#include "timer.h"

#include <stdint.h>

/* constants */

#define HIGH GPIO_PIN_SET
#define LOW  GPIO_PIN_RESET

// from TPIC6596 datasheet section 5.3 (Recommended Operating Conditions)
#define MIN_SIN_SETUP_TIME 		10 // ns
#define MIN_SIN_HOLD_TIME 		10 // ns
#define MIN_SIN_PULSE_DURATION 	20 // ns

// from TPIC6596 datasheet section 5.5 (Switching Characteristics)
#define TYP_NG_HIGH_TO_LOW_PROPAGATION_DELAY	650 // ns
#define TYP_NG_LOW_TO_HIGH_PROPAGATION_DELAY	150 // ns

// from stm32l476rg reference manual table 72 (I/O AC characteristics)
// took 3.3V and highest capacitance conditions
// 30pF for very high and 50pF for high, medium, and low
#define GPIO_RISE_FALL_TIME(gpio_speed)                      		\
    ( (gpio_speed) == GPIO_SPEED_FREQ_VERY_HIGH ?  4 : /* 3.3 ns */ \
      (gpio_speed) == GPIO_SPEED_FREQ_HIGH      ?  6 : /* 5.8 ns */ \
      (gpio_speed) == GPIO_SPEED_FREQ_MEDIUM    ?  9 : /* 9 ns */ 	\
      (gpio_speed) == GPIO_SPEED_FREQ_LOW       ? 25 : /* 25 ns */ 	\
	  	  	  	  	  	  	  	  	  	  	  	  25 ) /* default */

#define GPIO_SPEC_CAPACITANCE_CONDITIONS(gpio_speed) \
	( (gpio_speed) == GPIO_SPEED_FREQ_VERY_HIGH ? 30 : 50) // pF

// scale safety factor based on capacitance ((capacitance + speced capacitance - 1) / (speced capacitance)
#define CAPACITANCE_SAFETY_FACTOR(gpio_capacitance, gpio_speed) \
		(((gpio_capacitance) + GPIO_SPEC_CAPACITANCE_CONDITIONS(gpio_speed) - 1) / GPIO_SPEC_CAPACITANCE_CONDITIONS(gpio_speed))

// get GPIO safe driven rise and fall time
#define GPIO_SAFE_RISE_FALL_TIME(gpio_speed, gpio_capacitance) \
					(GPIO_RISE_FALL_TIME(gpio_speed) * CAPACITANCE_SAFETY_FACTOR(gpio_capacitance, gpio_speed))

// RC Circuit (how long from 0 to 90%)
//
// V(t) = VDD * (1 – e^(–t/RC))
// -> 0.9 = 1 – e^(–t90/RC)
// -> e^(–t90/RC) = 0.1
// -> t90 = –RC * ln(0.1)
// -> t = RC * ln(10)
// -> t = ~2.302 * RC
// R in ohms, C in pF, t in ns
// -> t * 10^-9 = ~2.302 * R * C * 10^12
// -> t = (~2.302 * R * C) / 1000
// -> t = (~23.02 * R * C) / 10000
//
#define RC_RISE_TIME_90(R, C) ((23 * (R) * (C)) / 10000)

// get driving speed from speed settings
#define RCK_FALL_TIME   GPIO_SAFE_RISE_FALL_TIME(SHIFTREG_RCK_SPEED_SETTING, SHIFTREG_RCK_CAPACITANCE_PICOFARADS)
#define SRCK_FALL_TIME	GPIO_SAFE_RISE_FALL_TIME(SHIFTREG_SRCK_SPEED_SETTING, SHIFTREG_SRCK_CAPACITANCE_PICOFARADS)
#define SIN_FALL_TIME   GPIO_SAFE_RISE_FALL_TIME(SHIFTREG_SIN_SPEED_SETTING, SHIFTREG_SIN_CAPACITANCE_PICOFARADS)
#define NG_FALL_TIME   	GPIO_SAFE_RISE_FALL_TIME(SHIFTREG_NG_SPEED_SETTING, SHIFTREG_NG_CAPACITANCE_PICOFARADS)

// goes open drain and becomes an RC circuit after stopping driving
#define RCK_RISE_TIME   (RCK_FALL_TIME + RC_RISE_TIME_90(SHIFTREG_RCK_EXTERNAL_PULLUP_OHMS, SHIFTREG_RCK_CAPACITANCE_PICOFARADS))
#define SRCK_RISE_TIME	(SRCK_FALL_TIME + RC_RISE_TIME_90(SHIFTREG_SRCK_EXTERNAL_PULLUP_OHMS, SHIFTREG_SRCK_CAPACITANCE_PICOFARADS))
#define SIN_RISE_TIME   (SIN_FALL_TIME + RC_RISE_TIME_90(SHIFTREG_SIN_EXTERNAL_PULLUP_OHMS, SHIFTREG_SIN_CAPACITANCE_PICOFARADS))
#define NG_RISE_TIME   	(NG_FALL_TIME + RC_RISE_TIME_90(SHIFTREG_NG_EXTERNAL_PULLUP_OHMS, SHIFTREG_NG_CAPACITANCE_PICOFARADS))

/* static function definitions */

static void set_RCK(GPIO_PinState val);
static void set_SRCK(GPIO_PinState val);
static void set_SIN(GPIO_PinState val);
static void set_nG(GPIO_PinState val);

/* exposed functions */

void ShiftReg_test() {
	while (1) {
		set_SIN(LOW);
		set_RCK(LOW);
		set_SRCK(LOW);
		set_nG(LOW);
		HAL_Delay(1000);
		set_SIN(HIGH);
		set_RCK(HIGH);
		set_SRCK(HIGH);
		set_nG(HIGH);
		HAL_Delay(1000);
	}
}

void ShiftReg_output_enable() {
	set_nG(LOW);
	delay_ns(TYP_NG_HIGH_TO_LOW_PROPAGATION_DELAY);
}

void ShiftReg_output_disable() {
	set_nG(HIGH);
	delay_ns(TYP_NG_LOW_TO_HIGH_PROPAGATION_DELAY);
}

void ShiftReg_shift_in_data(uint8_t *data, int shiftreg_count) {
	int regnum, bitnum;
	int bit;

	// from TPIC6596 datasheet section 6 (Parameter Measurement Information)
	// "Write data and read data are valid only when RCK is low"
	set_RCK(LOW);

	// reset SRCLK to low
	set_SRCK(LOW);

	for (regnum = shiftreg_count - 1; regnum >= 0; regnum--) {
		for (bitnum = 7; bitnum >= 0; bitnum--) {
			// get bitnum of regnum's data
			bit = (data[regnum] >> bitnum) & 0x01;

			// clock the bit in
			// shifts previously shifted bits forward

			set_SIN(bit);
			delay_ns(MIN_SIN_SETUP_TIME);

			set_SRCK(HIGH);

			delay_ns(MIN_SIN_PULSE_DURATION);
			set_SRCK(LOW);
		}
	}

	// clock RCLK, saving the values that were shifted in
	set_RCK(HIGH);
	set_RCK(LOW);
}

/* static functions */

static void set_RCK(GPIO_PinState val) {
	HAL_GPIO_WritePin(SHIFTREG_RCK_GPIO_EXPANDER, SHIFTREG_RCK_GPIO_PIN, val);
	if (val == GPIO_PIN_SET) {
		delay_ns(RCK_RISE_TIME);
	} else {
		delay_ns(RCK_FALL_TIME);
	}
}

static void set_SRCK(GPIO_PinState val) {
	HAL_GPIO_WritePin(SHIFTREG_SRCK_GPIO_EXPANDER, SHIFTREG_SRCK_GPIO_PIN, val);
	if (val == GPIO_PIN_SET) {
		delay_ns(SRCK_RISE_TIME);
	} else {
		delay_ns(SRCK_FALL_TIME);
	}
}

static void set_SIN(GPIO_PinState val) {
	HAL_GPIO_WritePin(SHIFTREG_SIN_GPIO_EXPANDER, SHIFTREG_SIN_GPIO_PIN, val);
	if (val == GPIO_PIN_SET) {
		delay_ns(SIN_RISE_TIME);
	} else {
		delay_ns(SIN_FALL_TIME);
	}
}

static void set_nG(GPIO_PinState val) {
	HAL_GPIO_WritePin(SHIFTREG_NG_GPIO_EXPANDER, SHIFTREG_NG_GPIO_PIN, val);
	if (val == GPIO_PIN_SET) {
		delay_ns(NG_RISE_TIME);
	} else {
		delay_ns(NG_FALL_TIME);
	}
}


