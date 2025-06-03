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

void Transmitter_setup();
void Transmitter_send(uint8_t *command);

#undef SCOPE
#endif /* INC_RADIO_H_ */
