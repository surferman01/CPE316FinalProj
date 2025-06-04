/** transmitter.c
 *
 *
 * Author: Alex Castellar
*/

#define _TRANSMITTER_C

#include "transmitter.h"

#include "radio.h"
#include "radio_conf.h"
#include "timer.h"

void Transmitter_setup(SPI_HandleTypeDef *hspi) {
	Radio_tx_init(2500, _1Mbps, hspi);
}

void Transmitter_send(uint8_t *command) {
	Radio_tx_transmit(command);
}

void Transmitter_irq(uint16_t GPIO_Pin) {
	if (GPIO_Pin == NRF24L01P_IRQ_PIN_NUMBER) {
		Radio_tx_irq();
	}
}

