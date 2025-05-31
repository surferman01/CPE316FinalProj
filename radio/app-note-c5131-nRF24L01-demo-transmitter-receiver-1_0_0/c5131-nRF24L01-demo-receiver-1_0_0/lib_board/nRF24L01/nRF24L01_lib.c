//! @file nRF24L01_lib.c,v
//!
//! Copyright (c) 2006 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the driver for the
//! nRF24l01 RF module.
//!
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0 $Id: nRF24L01_lib.c,v 1.1 2006/05/02 14:26:36 cgasnier Exp $
//!
//! @todo
//! @bug

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "lib_mcu\spi\spi_drv.h"

#include "lib_board/nRF24L01/nRF_API.h"
#include "lib_board/nRF24L01/Protocol_API.h"
#include "lib_board/nRF24L01/LL_API_C51.h"

//_____ M A C R O S ________________________________________________________

//_____ P R I V A T E    D E C L A R A T I O N _____________________________

//_____ D E F I N I T I O N ________________________________________________

//_____ D E C L A R A T I O N ______________________________________________
extern Uchar IRQ_Source;
BYTE SPI_Mode;

#ifdef nRF24L01_TRANSMITTER

/*F**************************************************************************
* NAME: nRF24L01_init
*----------------------------------------------------------------------------
* PARAMS:   
* return: none
*----------------------------------------------------------------------------
* PURPOSE: nRF24L01 init in transmitter mode
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void nRF24L01_init (void)
{
  //select Hardware SPI
  SPI_Init(HW_MODE);

  //reset and unreset nRF24L01
  nRF24L01_reset();

  //ready to send data
  nRF24L01_tx_config();
}
#endif //nRF24L01_TRANSMITTER



#ifdef nRF24L01_RECEIVER

/*F**************************************************************************
* NAME: nRF24L01_init
*----------------------------------------------------------------------------
* PARAMS:   
* return: none
*----------------------------------------------------------------------------
* PURPOSE: nRF24L01 init in receiver mode
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void nRF24L01_init (void)
{
  //select Hardware SPI
  SPI_Init(HW_MODE);

  nRF24L01_reset();

  C5131_enable_it();

  // ready to receive
  nRF24L01_rx_config();

  //wait for IT (see usb_task.c) 
}
#endif //nRF24L01_RECEIVER


/*F**************************************************************************
* NAME: nRF24L01_send_byte
*----------------------------------------------------------------------------
* PARAMS:   
* return: none
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
void nRF24L01_send_byte (Uchar uc_command)
{
  Uchar TX_pload[1];
  
  TX_pload[0] = uc_command;

  // Writes new payload to L01
  L01_Write_TX_Pload(TX_pload, 1);

  //clear IT
  L01_Clear_IRQ(MASK_IRQ_FLAGS);
}

/*F**************************************************************************
* NAME: nRF24L01_receive_byte
*----------------------------------------------------------------------------
* PARAMS:   
* return: none
*----------------------------------------------------------------------------
* PURPOSE: Receive a byte via RF
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
Uchar nRF24L01_receive_byte (void)
{
  Uchar RX_pload[1];

  while(IRQ_Source != RX_DR);

  //Read FIFO with mouse message
  SPI_Read_Buf(RD_RX_PLOAD, RX_pload, 1);

  //clear IT
  L01_Clear_IRQ(MASK_IRQ_FLAGS);

  return(RX_pload[0]);
}



/*F**************************************************************************
* NAME: nRF24L01_interrupt
*----------------------------------------------------------------------------
* PARAMS:   
* return: none
*----------------------------------------------------------------------------
* PURPOSE: nRF24L01 interrupt signal connected on INT0 pin
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void nRF24L01_interrupt(void) interrupt IRQ_INT0
{
 Uchar temp;

  temp = L01_Get_Status();  

  // Read status, get IRQ source & pipe#
  switch(temp & MASK_IRQ_FLAGS)                                   // Get IRQ source
  {
    case RX_DR:                                                   // Data Ready RX FIFO ?
      IRQ_Source = RX_DR;                                         // Flag for RX_DR
	break;

    case TX_DS:                                                   // Data Sent TX FIFO ?
      IRQ_Source = TX_DS;                                         // Flag for TX_DS
    break;

    case MAX_RT:                                                  // Max #of retries ?
      IRQ_Source = MAX_RT;                                        // Flag for MAX_RT
    break;

    default:                                                      // Default, clear IRQ flags
      L01_Clear_IRQ(MASK_IRQ_FLAGS);
    break;
  }
}

