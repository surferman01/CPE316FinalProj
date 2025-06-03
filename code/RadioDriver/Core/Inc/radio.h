/** radio.h
 * Implements a driver for the nRF24L01 transceiver
 *
 * Modified by Alex Castellar
 * Original Code from mokhwasomssi
*/


#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#ifdef _RADIO_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"
#include <stdbool.h>

/* nRF24L01+ typedefs */
typedef uint8_t count;
typedef uint8_t widths;
typedef uint8_t length;
typedef uint16_t delay;
typedef uint16_t channel;

typedef enum
{
    _250kbps = 2,
    _1Mbps   = 0,
    _2Mbps   = 1
} air_data_rate;

typedef enum
{
    _0dBm  = 3,
    _6dBm  = 2,
    _12dBm = 1,
    _18dBm = 0
} output_power;


/* Main Functions */
void Radio_rx_init(channel MHz, air_data_rate bps, SPI_HandleTypeDef *hspi);
void Radio_tx_init(channel MHz, air_data_rate bps, SPI_HandleTypeDef *hspi);

void Radio_rx_receive(uint8_t* rx_payload);
void Radio_tx_transmit(uint8_t* tx_payload);

// Check tx_ds or max_rt
void Radio_tx_irq();


/* Sub Functions */
void Radio_reset();

void Radio_prx_mode();
void Radio_ptx_mode();

void Radio_power_up();
void Radio_power_down();

uint8_t Radio_get_status();
uint8_t Radio_get_fifo_status();

// Static payload lengths
void Radio_rx_set_payload_widths(widths bytes);

uint8_t Radio_read_rx_fifo(uint8_t* rx_payload);
uint8_t Radio_write_tx_fifo(uint8_t* tx_payload);

void Radio_flush_rx_fifo();
void Radio_flush_tx_fifo();

// Clear IRQ pin. Change LOW to HIGH
void Radio_clear_rx_dr();
void Radio_clear_tx_ds();
void Radio_clear_max_rt();

void Radio_set_rf_channel(channel MHz);
void Radio_set_rf_tx_output_power(output_power dBm);
void Radio_set_rf_air_data_rate(air_data_rate bps);

void Radio_set_crc_length(length bytes);
void Radio_set_address_widths(widths bytes);
void Radio_auto_retransmit_count(count cnt);
void Radio_auto_retransmit_delay(delay us);


/* nRF24L01+ Commands */
#define NRF24L01P_CMD_R_REGISTER                  0b00000000
#define NRF24L01P_CMD_W_REGISTER                  0b00100000
#define NRF24L01P_CMD_R_RX_PAYLOAD                0b01100001
#define NRF24L01P_CMD_W_TX_PAYLOAD                0b10100000
#define NRF24L01P_CMD_FLUSH_TX                    0b11100001
#define NRF24L01P_CMD_FLUSH_RX                    0b11100010
#define NRF24L01P_CMD_REUSE_TX_PL                 0b11100011
#define NRF24L01P_CMD_R_RX_PL_WID                 0b01100000
#define NRF24L01P_CMD_W_ACK_PAYLOAD               0b10101000
#define NRF24L01P_CMD_W_TX_PAYLOAD_NOACK          0b10110000
#define NRF24L01P_CMD_NOP                         0b11111111

/* nRF24L01+ Registers */
#define NRF24L01P_REG_CONFIG            0x00
#define NRF24L01P_REG_EN_AA             0x01
#define NRF24L01P_REG_EN_RXADDR         0x02
#define NRF24L01P_REG_SETUP_AW          0x03
#define NRF24L01P_REG_SETUP_RETR        0x04
#define NRF24L01P_REG_RF_CH             0x05
#define NRF24L01P_REG_RF_SETUP          0x06
#define NRF24L01P_REG_STATUS            0x07
#define NRF24L01P_REG_OBSERVE_TX        0x08    // Read-Only
#define NRF24L01P_REG_RPD               0x09    // Read-Only
#define NRF24L01P_REG_RX_ADDR_P0        0x0A
#define NRF24L01P_REG_RX_ADDR_P1        0x0B
#define NRF24L01P_REG_RX_ADDR_P2        0x0C
#define NRF24L01P_REG_RX_ADDR_P3        0x0D
#define NRF24L01P_REG_RX_ADDR_P4        0x0E
#define NRF24L01P_REG_RX_ADDR_P5        0x0F
#define NRF24L01P_REG_TX_ADDR           0x10
#define NRF24L01P_REG_RX_PW_P0          0x11
#define NRF24L01P_REG_RX_PW_P1          0x12
#define NRF24L01P_REG_RX_PW_P2          0x13
#define NRF24L01P_REG_RX_PW_P3          0x14
#define NRF24L01P_REG_RX_PW_P4          0x15
#define NRF24L01P_REG_RX_PW_P5          0x16
#define NRF24L01P_REG_FIFO_STATUS       0x17
#define NRF24L01P_REG_DYNPD             0x1C
#define NRF24L01P_REG_FEATURE           0x1D


#undef SCOPE
#endif /* INC_RADIO_H_ */
