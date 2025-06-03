/** radio_conf.h
 *
 * Author: Alex Castellar
 */

#ifndef INC_RADIO_CONF_H_
#define INC_RADIO_CONF_H_

/* User Configurations */

// PB10
#define NRF24L01P_SPI_CS_PIN_PORT         GPIOB
#define NRF24L01P_SPI_CS_PIN_NUMBER       GPIO_PIN_10

// PC7
#define NRF24L01P_CE_PIN_PORT             GPIOC
#define NRF24L01P_CE_PIN_NUMBER           GPIO_PIN_7

// PB6
#define NRF24L01P_IRQ_PIN_PORT            GPIOB
#define NRF24L01P_IRQ_PIN_NUMBER          GPIO_PIN_6

#define NRF24L01P_PAYLOAD_LENGTH          8     // 1 - 32bytes

#endif /* INC_RADIO_CONF_H_ */
