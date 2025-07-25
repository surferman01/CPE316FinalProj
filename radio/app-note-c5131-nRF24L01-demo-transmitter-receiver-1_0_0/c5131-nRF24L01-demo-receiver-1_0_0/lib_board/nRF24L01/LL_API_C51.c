//! @file LL_API_C51.c,v
//!
//! Copyright (c) 2006 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level macros and definition for the
//! nRF24l01 RF module.
//!
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0 $Id: LL_API_C51.c,v 1.1 2006/05/02 14:26:37 cgasnier Exp $
//!
//! @todo
//! @bug

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "lib_mcu\spi\spi_drv.h"
#include "lib_board/nRF24L01/nRF_API.h"
#include "lib_board/nRF24L01/LL_API_C51.h"

//_____ M A C R O S ________________________________________________________

//_____ P R I V A T E    D E C L A R A T I O N _____________________________

//_____ D E F I N I T I O N ________________________________________________
extern BYTE SPI_Mode;

//_____ D E C L A R A T I O N ______________________________________________

/*F**************************************************************************
* NAME: nRF24L01_tx_config
*----------------------------------------------------------------------------
* PARAMS:   
* return:
*----------------------------------------------------------------------------
* PURPOSE: Configure nRF24L01 in Tx mode
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void nRF24L01_tx_config(void)
{
  Uchar test_wr[1];

  //Send TX config
  test_wr[0] = 0x0A;
  SPI_Write_Buf(0x20, test_wr, 1);
}

/*F**************************************************************************
* NAME: nRF24L01_rx_config
*----------------------------------------------------------------------------
* PARAMS:   
* return:
*----------------------------------------------------------------------------
* PURPOSE: Configure nRF24L01 in Rx mode
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void nRF24L01_rx_config(void)
{
  Uchar test_wr[1];

  //Send RX config
  test_wr[0] = 0x0B;
  SPI_Write_Buf(0x20, test_wr, 1);

  //rx length is 1 byte
  test_wr[0] = 0x01;
  SPI_Write_Buf(0x20 + RX_PW_P0, test_wr, 1);
}

/*F**************************************************************************
* NAME: nRF24L01_reset
*----------------------------------------------------------------------------
* PARAMS:   
* return:
*----------------------------------------------------------------------------
* PURPOSE: Reset/Unreset nRF24L01
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void nRF24L01_reset()
{
  //Reset/unreset nRF24L01, reset CE=1
  CE = 0;
  CE = 1;
}

/*F**************************************************************************
* NAME: C5131_enable_it
*----------------------------------------------------------------------------
* PARAMS:   
* return:
*----------------------------------------------------------------------------
* PURPOSE: Enable C5131 IT on INT0 pin
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void  C5131_enable_it(void)
{
  //Enable ITs
  Enable_interrupt();

  // enable INT0 interrupt
  IE0 |= 0x01;

  // interrupt on falling edge
  TCON |= 0x01;
}

/*F**************************************************************************
* NAME: SPI_HW
*----------------------------------------------------------------------------
* PARAMS: Data to send   
* return: 
*----------------------------------------------------------------------------
* PURPOSE: Send a byte via RF
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
BYTE SPI_HW(BYTE byte)             // Write one byte 
{
  Spi_write_data(byte);            // Writes 'byte' to nRF24L01
  Spi_wait_spif();     
 
  return(SPDAT);                   // return received byte
}

/*F**************************************************************************
* NAME: SPI_Init
*----------------------------------------------------------------------------
* PARAMS: SPI Mode (hardware)  
* return: 
*----------------------------------------------------------------------------
* PURPOSE: Init C5131 hardware SPI
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void SPI_Init(BYTE Mode)
{
  SPI_Mode = Mode; 

  // configure AT89C5131 Hardware SPI
  Spi_disable_ss();
  Spi_set_mode(SPI_MASTER_MODE_0); // CPOL = 0 ; CPHA = 0
  Spi_set_rate(SPI_RATE_1);
  Spi_enable();
}

/*F**************************************************************************
* NAME: CSN_Pin
*----------------------------------------------------------------------------
* PARAMS: state (O or 1)
* return:
*----------------------------------------------------------------------------
* PURPOSE: Control of CSN pin of nRF24L01
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void CSN_Pin(BYTE state)                                
{
  if(state)
    CSN = 1;
  else
    CSN = 0;
}

/*F**************************************************************************
* NAME: CE_Pin
*----------------------------------------------------------------------------
* PARAMS: state (O or 1)  
* return: 
*----------------------------------------------------------------------------
* PURPOSE: Control of CE pin of nRF24L01
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void CE_Pin(BYTE action)                                
{
  switch(action)
  {
    case CE_LOW:                                        // action == 0, CE low
      CE = 0;
    break;

    case CE_HIGH:                                       // action == 1, CE high
      CE = 1;
    break;
  }
}


