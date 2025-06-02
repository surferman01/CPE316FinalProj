
#define _SHIFTREG_C

#include "shiftreg.h"

void shiftDataIn(uint8_t data, int dataLen) {

}

void SRCLK_Set(GPIO_TypeDef* expander, uint16_t pin) {
	HAL_GPIO_WritePin(expander, pin, GPIO_PIN_);
}


