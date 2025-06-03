/** transmitter.c
 *
 *
 * Author: Alex Castellar
*/

#define _TRANSMITTER_C

#include "transmitter.h"

#include "radio.h"
#include "timer.h"


/* static function definitions */

void Transmitter_setup() {
	Radio_tx_init(2500, _1Mbps, &hspi1);
}


void Transmitter_send(uint8_t *command) {
	Radio_tx_transmit(command);
}


/* static variable definitions */


/* exposed functions */


/* static functions */


