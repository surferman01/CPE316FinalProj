
#define _SHIFTREG_C

#include "shiftreg.h"
#include "timer.h"

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
#define GPIO_RISE_FALL_TIME(gpio_speed)                      		\
    ( (gpio_speed) == GPIO_SPEED_FREQ_VERY_HIGH ?  4 : /* 3.3 ns */ \
      (gpio_speed) == GPIO_SPEED_FREQ_HIGH      ?  6 : /* 5.8 ns */ \
      (gpio_speed) == GPIO_SPEED_FREQ_MEDIUM    ?  9 : /* 9 ns */ 	\
      (gpio_speed) == GPIO_SPEED_FREQ_LOW       ? 25 : /* 25 ns */ 	\
	  	  	  	  	  	  	  	  	  	  	  	  25 ) /* default */



#define GPIO_RISE_FALL_SAFETY_MARGIN 0 // 0 ns
// to be really safe, incase we have a ton of extra capacitance, we are using large safety factor
#define GPIO_RISE_FALL_SAFETY_FACTOR 3 // 3x

#define GPIO_SAFE_RISE_FALL_TIME(gpio_speed) (GPIO_RISE_FALL_TIME(gpio_speed) * GPIO_RISE_FALL_SAFETY_FACTOR + GPIO_RISE_FALL_SAFETY_MARGIN)

// select from speed settings
#define RCK_RISE_FALL_TIME   	GPIO_SAFE_RISE_FALL_TIME(SHIFTREG_RCK_SPEED_SETTING)
#define SRCK_RISE_FALL_TIME   	GPIO_SAFE_RISE_FALL_TIME(SHIFTREG_SRCK_SPEED_SETTING)
#define SIN_RISE_FALL_TIME   	GPIO_SAFE_RISE_FALL_TIME(SHIFTREG_SIN_SPEED_SETTING)
#define NG_RISE_FALL_TIME   	GPIO_SAFE_RISE_FALL_TIME(SHIFTREG_NG_SPEED_SETTING)


/* static function definitions */

static void set_RCK(GPIO_PinState val);
static void set_SRCK(GPIO_PinState val);
static void set_SIN(GPIO_PinState val);
static void set_nG(GPIO_PinState val);

/* exposed functions */

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

	for (regnum = shiftreg_count; regnum >= 0; regnum--) {
		for (bitnum = 7; bitnum >= 0; bitnum--) {
			// get bitnum of regnum's data
			bit = (data[regnum] >> bitnum) & 0x01;

			// clock the bit in
			// shifts previously shifted bits forward

			set_SIN(bit);
			delay_ns(MIN_SIN_SETUP_TIME);

			set_SRCK(HIGH);

			delay_ns(MIN_SIN_HOLD_TIME);
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
	delay_ns(RCK_RISE_FALL_TIME);
}

static void set_SRCK(GPIO_PinState val) {
	HAL_GPIO_WritePin(SHIFTREG_SRCK_GPIO_EXPANDER, SHIFTREG_SRCK_GPIO_PIN, val);
	delay_ns(SRCK_RISE_FALL_TIME);
}

static void set_SIN(GPIO_PinState val) {
	HAL_GPIO_WritePin(SHIFTREG_SIN_GPIO_EXPANDER, SHIFTREG_SIN_GPIO_PIN, val);
	delay_ns(SIN_RISE_FALL_TIME);
}

static void set_nG(GPIO_PinState val) {
	HAL_GPIO_WritePin(SHIFTREG_NG_GPIO_EXPANDER, SHIFTREG_NG_GPIO_PIN, val);
	delay_ns(NG_RISE_FALL_TIME);
}


