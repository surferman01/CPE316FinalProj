/** transmitter.h
 *
 *
 * Author: Alex Castellar
*/


#ifndef INC_TRANSMITTER_H_
#define INC_TRANSMITTER_H_

#ifdef _TRANSMITTER_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"
#include <stdbool.h>

SCOPE void Transmitter_setup(SPI_HandleTypeDef *hspi);
SCOPE void Transmitter_send(uint8_t *command);
SCOPE void Transmitter_irq(uint16_t GPIO_Pin);

#undef SCOPE
#endif /* INC_RADIO_H_ */
