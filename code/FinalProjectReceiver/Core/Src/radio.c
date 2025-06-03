/** radio.c
 * Implements a driver for the nRF24L01 transceiver
 *
 * Modified by Alex Castellar
 * Original Code from mokhwasomssi
*/

#define _RADIO_C

#include "radio.h"
#include "radio_conf.h"

#include "timer.h"

#include <stdint.h>

/* static function definitions */

static SPI_HandleTypeDef *spi_handle;

/* static variable definitions */

static void cs_high();
static void cs_low();
static void ce_high();
static void ce_low();

static uint8_t read_register(uint8_t reg);
static uint8_t write_register(uint8_t reg, uint8_t value);

static void reuse_last_tx_payload(void);

/* exposed functions */

void Radio_rx_init(channel MHz, air_data_rate bps, SPI_HandleTypeDef *hspi)
{
	spi_handle = hspi;

    Radio_reset();

    Radio_prx_mode();
    Radio_power_up();
    HAL_Delay(2);


    Radio_rx_set_payload_widths(NRF24L01P_PAYLOAD_LENGTH);

    Radio_set_rf_channel(MHz);
    Radio_set_rf_air_data_rate(bps);
    Radio_set_rf_tx_output_power(_0dBm);

    Radio_set_crc_length(1);
    Radio_set_address_widths(5);

    Radio_auto_retransmit_count(15);
    Radio_auto_retransmit_delay(NRF24L01P_RETRANSMIT_DELAY_US);

    ce_high();
}

void Radio_tx_init(channel MHz, air_data_rate bps, SPI_HandleTypeDef *hspi)
{
	spi_handle = hspi;

    Radio_reset();

    Radio_ptx_mode();
    Radio_power_up();
    HAL_Delay(2);

    Radio_set_rf_channel(MHz);
    Radio_set_rf_air_data_rate(bps);
    Radio_set_rf_tx_output_power(_0dBm);

    Radio_set_crc_length(1);
    Radio_set_address_widths(5);

    Radio_auto_retransmit_count(15);
    Radio_auto_retransmit_delay(NRF24L01P_RETRANSMIT_DELAY_US);

    ce_high();
}

void Radio_rx_receive(uint8_t* rx_payload)
{
    Radio_read_rx_fifo(rx_payload);
    Radio_clear_rx_dr();
}

void Radio_tx_transmit(uint8_t* tx_payload)
{
    Radio_write_tx_fifo(tx_payload);
}

void Radio_tx_irq()
{
    uint8_t tx_ds = Radio_get_status();
    tx_ds &= 0x20;

    if(tx_ds)
    {
        // TX_DS (success)
        Radio_clear_tx_ds();
    }

    else
    {
    	// MAX_RT
    	Radio_clear_max_rt();

    	// if we want to never give up
    	reuse_last_tx_payload();

    	// if we want to give up after 15 tries use this
    	//Radio_flush_tx_fifo();


    	// leave standby‑II
    	ce_low();
    	// arm the radio again
    	ce_high();
    }
}

/* nRF24L01+ Sub Functions */
void Radio_reset()
{
    // Reset pins
    cs_high();
    ce_low();

    // Reset registers
    write_register(NRF24L01P_REG_CONFIG, 0x08);
    write_register(NRF24L01P_REG_EN_AA, 0x3F);
    write_register(NRF24L01P_REG_EN_RXADDR, 0x03);
    write_register(NRF24L01P_REG_SETUP_AW, 0x03);
    write_register(NRF24L01P_REG_SETUP_RETR, 0x03);
    write_register(NRF24L01P_REG_RF_CH, 0x02);
    write_register(NRF24L01P_REG_RF_SETUP, 0x07);
    write_register(NRF24L01P_REG_STATUS, 0x7E);
    write_register(NRF24L01P_REG_RX_PW_P0, 0x00);
    write_register(NRF24L01P_REG_RX_PW_P0, 0x00);
    write_register(NRF24L01P_REG_RX_PW_P1, 0x00);
    write_register(NRF24L01P_REG_RX_PW_P2, 0x00);
    write_register(NRF24L01P_REG_RX_PW_P3, 0x00);
    write_register(NRF24L01P_REG_RX_PW_P4, 0x00);
    write_register(NRF24L01P_REG_RX_PW_P5, 0x00);
    write_register(NRF24L01P_REG_FIFO_STATUS, 0x11);
    write_register(NRF24L01P_REG_DYNPD, 0x00);
    write_register(NRF24L01P_REG_FEATURE, 0x00);

    // Reset FIFO
    Radio_flush_rx_fifo();
    Radio_flush_tx_fifo();
}

void Radio_prx_mode()
{
    uint8_t new_config = read_register(NRF24L01P_REG_CONFIG);
    new_config |= 1 << 0;

    write_register(NRF24L01P_REG_CONFIG, new_config);
}

void Radio_ptx_mode()
{
    uint8_t new_config = read_register(NRF24L01P_REG_CONFIG);
    new_config &= 0xFE;

    write_register(NRF24L01P_REG_CONFIG, new_config);
}

uint8_t Radio_read_rx_fifo(uint8_t* rx_payload)
{
    uint8_t command = NRF24L01P_CMD_R_RX_PAYLOAD;
    uint8_t status;

    cs_low();
    HAL_SPI_TransmitReceive(spi_handle, &command, &status, 1, 2000);
    HAL_SPI_Receive(spi_handle, rx_payload, NRF24L01P_PAYLOAD_LENGTH, 2000);
    cs_high();

    return status;
}

uint8_t Radio_write_tx_fifo(uint8_t* tx_payload)
{
    uint8_t command = NRF24L01P_CMD_W_TX_PAYLOAD;
    uint8_t status;

    cs_low();
    HAL_SPI_TransmitReceive(spi_handle, &command, &status, 1, 2000);
    HAL_SPI_Transmit(spi_handle, tx_payload, NRF24L01P_PAYLOAD_LENGTH, 2000);
    cs_high();

    return status;
}

void Radio_flush_rx_fifo()
{
    uint8_t command = NRF24L01P_CMD_FLUSH_RX;
    uint8_t status;

    cs_low();
    HAL_SPI_TransmitReceive(spi_handle, &command, &status, 1, 2000);
    cs_high();
}

void Radio_flush_tx_fifo()
{
    uint8_t command = NRF24L01P_CMD_FLUSH_TX;
    uint8_t status;

    cs_low();
    HAL_SPI_TransmitReceive(spi_handle, &command, &status, 1, 2000);
    cs_high();
}

uint8_t Radio_get_status()
{
    uint8_t command = NRF24L01P_CMD_NOP;
    uint8_t status;

    cs_low();
    HAL_SPI_TransmitReceive(spi_handle, &command, &status, 1, 2000);
    cs_high();

    return status;
}

uint8_t Radio_get_fifo_status()
{
    return read_register(NRF24L01P_REG_FIFO_STATUS);
}

void Radio_rx_set_payload_widths(widths bytes)
{
    write_register(NRF24L01P_REG_RX_PW_P0, bytes);
}

void Radio_clear_rx_dr()
{
    uint8_t new_status = Radio_get_status();
    new_status |= 0x40;

    write_register(NRF24L01P_REG_STATUS, new_status);
}

void Radio_clear_tx_ds()
{
    uint8_t new_status = Radio_get_status();
    new_status |= 0x20;

    write_register(NRF24L01P_REG_STATUS, new_status);
}

void Radio_clear_max_rt()
{
    uint8_t new_status = Radio_get_status();
    new_status |= 0x10;

    write_register(NRF24L01P_REG_STATUS, new_status);
}

void Radio_power_up()
{
    uint8_t new_config = read_register(NRF24L01P_REG_CONFIG);
    new_config |= 1 << 1;

    write_register(NRF24L01P_REG_CONFIG, new_config);
}

void Radio_power_down()
{
    uint8_t new_config = read_register(NRF24L01P_REG_CONFIG);
    new_config &= 0xFD;

    write_register(NRF24L01P_REG_CONFIG, new_config);
}

void Radio_set_crc_length(length bytes)
{
    uint8_t new_config = read_register(NRF24L01P_REG_CONFIG);

    switch(bytes)
    {
        // CRCO bit in CONFIG resiger set 0
        case 1:
            new_config &= 0xFB;
            break;
        // CRCO bit in CONFIG resiger set 1
        case 2:
            new_config |= 1 << 2;
            break;
    }

    write_register(NRF24L01P_REG_CONFIG, new_config);
}

void Radio_set_address_widths(widths bytes)
{
    write_register(NRF24L01P_REG_SETUP_AW, bytes - 2);
}

void Radio_auto_retransmit_count(count cnt)
{
    uint8_t v = read_register(NRF24L01P_REG_SETUP_RETR);
    v &= 0xF0;                 // keep ARD, clear ARC
    v |= (cnt & 0x0F);
    write_register(NRF24L01P_REG_SETUP_RETR, v);
}

void Radio_auto_retransmit_delay(delay us)
{
    uint8_t v = read_register(NRF24L01P_REG_SETUP_RETR);
    v &= 0x0F;                 // keep ARC, clear ARD
    v |= (((us/250) - 1) & 0x0F) << 4;
    write_register(NRF24L01P_REG_SETUP_RETR, v);
}

void Radio_set_rf_channel(channel MHz)
{
	uint16_t new_rf_ch = MHz - 2400;
    write_register(NRF24L01P_REG_RF_CH, new_rf_ch);
}

void Radio_set_rf_tx_output_power(output_power dBm)
{
    uint8_t new_rf_setup = read_register(NRF24L01P_REG_RF_SETUP) & 0xF9;
    new_rf_setup |= (dBm << 1);

    write_register(NRF24L01P_REG_RF_SETUP, new_rf_setup);
}

void Radio_set_rf_air_data_rate(air_data_rate bps)
{
    // Set value to 0
    uint8_t new_rf_setup = read_register(NRF24L01P_REG_RF_SETUP) & 0xD7;

    switch(bps)
    {
        case _1Mbps:
            break;
        case _2Mbps:
            new_rf_setup |= 1 << 3;
            break;
        case _250kbps:
            new_rf_setup |= 1 << 5;
            break;
    }
    write_register(NRF24L01P_REG_RF_SETUP, new_rf_setup);
}


/* static functions */

static void reuse_last_tx_payload(void)
{
    uint8_t cmd = NRF24L01P_CMD_REUSE_TX_PL;   /* 0xE3 */
    cs_low();
    HAL_SPI_Transmit(spi_handle, &cmd, 1, 2000);
    cs_high();
}

static void cs_high()
{
    HAL_GPIO_WritePin(NRF24L01P_SPI_CS_PIN_PORT, NRF24L01P_SPI_CS_PIN_NUMBER, GPIO_PIN_SET);
}

static void cs_low()
{
    HAL_GPIO_WritePin(NRF24L01P_SPI_CS_PIN_PORT, NRF24L01P_SPI_CS_PIN_NUMBER, GPIO_PIN_RESET);
}

static void ce_high()
{
    HAL_GPIO_WritePin(NRF24L01P_CE_PIN_PORT, NRF24L01P_CE_PIN_NUMBER, GPIO_PIN_SET);
}

static void ce_low()
{
    HAL_GPIO_WritePin(NRF24L01P_CE_PIN_PORT, NRF24L01P_CE_PIN_NUMBER, GPIO_PIN_RESET);
}

static uint8_t read_register(uint8_t reg)
{
    uint8_t command = NRF24L01P_CMD_R_REGISTER | reg;
    uint8_t status;
    uint8_t read_val;

    cs_low();
    HAL_SPI_TransmitReceive(spi_handle, &command, &status, 1, 2000);
    HAL_SPI_Receive(spi_handle, &read_val, 1, 2000);
    cs_high();

    return read_val;
}

static uint8_t write_register(uint8_t reg, uint8_t value)
{
    uint8_t command = NRF24L01P_CMD_W_REGISTER | reg;
    uint8_t status;
    uint8_t write_val = value;

    cs_low();
    HAL_SPI_TransmitReceive(spi_handle, &command, &status, 1, 2000);
    HAL_SPI_Transmit(spi_handle, &write_val, 1, 2000);
    cs_high();

    return write_val;
}















