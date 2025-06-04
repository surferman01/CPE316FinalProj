/** receiver.h
 *
 *
 * Author: Alex Castellar
*/


#ifndef INC_RECEIVER_H_
#define INC_RECEIVER_H_

#ifdef _RECEIVER_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"
#include <stdbool.h>

SCOPE void Receiver_setup(SPI_HandleTypeDef *hspi);
SCOPE void Receiver_process(void (*commandCallback)(uint8_t *command));
SCOPE void Receiver_irq(uint16_t GPIO_Pin);

#undef SCOPE
#endif /* INC_RECEIVER_H_ */
