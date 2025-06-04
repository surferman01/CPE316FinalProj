/** receiver.c
 *
 *
 * Author: Alex Castellar
 */

#define _RECEIVER_C

#include "receiver.h"

#include "radio.h"
#include "radio_conf.h"

#include "timer.h"

static volatile int recved;
static uint8_t recved_data[32];

void Receiver_setup(SPI_HandleTypeDef *hspi) {
	Radio_rx_init(2500, _1Mbps, hspi);
	recved = 0;
}

void Receiver_irq(uint16_t GPIO_Pin) {
	if(GPIO_Pin == NRF24L01P_IRQ_PIN_NUMBER) {
		if (recved == 0) {
			Radio_rx_receive(recved_data);
			recved = 1;
		} else {
			// discard the packet, we aren't ready for it
			Radio_flush_rx_fifo();
			Radio_clear_rx_dr();
		}
	}
}

void Receiver_process(void (*commandCallback)(uint8_t *command)) {
	if (recved) {
		commandCallback(recved_data);
		recved = 0;
	}
}
