/*C**************************************************************************
* NAME:         uart_usb_lib.c
*----------------------------------------------------------------------------
* Copyright (c) 2004 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5131-nRF24L01-demo-receiver-1_0_0      
* REVISION:     1.7     
*----------------------------------------------------------------------------
* PURPOSE: 
* This file controls the UART USB functions.
*****************************************************************************/
/*_____ I N C L U D E S ____________________________________________________*/

#include "config.h"
#include "usb_drv.h"
#include "modules\usb_cdc_enum\usb_cdc_enum.h"
#include "uart_usb_lib.h"

/*_____ M A C R O S ________________________________________________________*/

/*_____ D E F I N I T I O N ________________________________________________*/

Uchar tx_counter;
Uchar rx_counter;

extern         Uchar  usb_configuration_nb;

/*_____ D E C L A R A T I O N ______________________________________________*/

/*F**************************************************************************
* NAME: uart_usb_init
*----------------------------------------------------------------------------
* PARAMS:
* delay:    none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE: 
* This function initializes the UART USB library.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/

void uart_usb_init(void)
{
  tx_counter = 0;
  rx_counter = 0;
}

/*F**************************************************************************
* NAME: uart_usb_test_hit
*----------------------------------------------------------------------------
* PARAMS:
* delay:    none
* return:   boolean TRUE if a byte is ready to be read, FALSE otherwise
*----------------------------------------------------------------------------
* PURPOSE: 
* This function checks if a character has been received on the USB bus.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
bit uart_usb_test_hit(void)
{
  if (!rx_counter)
  { 
    Usb_select_ep(RX_EP);
    if (Usb_rx_complete()) { rx_counter = usb_get_nb_byte(); if (!rx_counter) Usb_clear_rx_cdc(); }
  }
  return (rx_counter!=0);
}

/*F**************************************************************************
* NAME: uart_usb_getchar
*----------------------------------------------------------------------------
* PARAMS:
* delay:    none
* return:   Uchar, read data from USB
*----------------------------------------------------------------------------
* PURPOSE: 
* This function reads one byte from the USB bus
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
* If one byte is present in the USB fifo, this byte is returned. If no data
* is present in the USB fifo, this function waits for USB data.
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
char uart_usb_getchar(void)
{
  register Uchar data_rx;

  Usb_select_ep(RX_EP);
  if (!rx_counter) while (!uart_usb_test_hit());
  data_rx=Usb_read_byte();
  rx_counter--;
  if (!rx_counter) Usb_clear_rx_cdc();
  return data_rx;
}


/*F**************************************************************************
* NAME: uart_usb_tx_ready
*----------------------------------------------------------------------------
* PARAMS:
* delay:    none
* return:   Boolean. TRUE if the firmware can write a new byte to transmit.
* FALSE oterwise.
*----------------------------------------------------------------------------
* PURPOSE: 
* This function checks if the USB emission buffer is ready to accept at
* at least 1 byte
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
bit uart_usb_tx_ready(void)
{
  Usb_select_ep(TX_EP);
  if (Usb_tx_ready())    { return FALSE; }
  if (Usb_tx_complete()) { Usb_clear_tx_complete(); return TRUE; }
  return TRUE;
}

/*F**************************************************************************
* NAME: uart_usb_putchar
*----------------------------------------------------------------------------
* PARAMS:
* Uchar:    data to send through USB
* delay:    none
* return:   data to send
*----------------------------------------------------------------------------
* PURPOSE: 
* This function fills the USB transmit buffer with the new data. This buffer
* is sent if complete. To flush this buffer before waiting full, launch
* the uart_usb_flush() function.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
char uart_usb_putchar(Uchar data_to_send)
{ 
  Usb_select_ep(TX_EP);
  if ((Usb_tx_ready())||(Usb_tx_complete())) 
  {
    	while(!Usb_tx_complete())
      {
          Usb_select_ep(EP_CONTROL);
          if (Usb_setup_received()) { usb_enumeration_process(); }
        
          Usb_select_ep(3);
          if (Usb_tx_complete())    { Usb_clear_tx_complete();   }
      
        	 Usb_select_ep(TX_EP);
      }
    	Usb_select_ep(TX_EP);
      Usb_clear_tx_complete();
      tx_counter = 0;
	}
  Usb_write_byte(data_to_send);
  tx_counter++;
  if(tx_counter >= TX_EP_SIZE-1) 
  {
	uart_usb_flush();
  } 
  return data_to_send;
}


/*F**************************************************************************
* NAME: uart_usb_flush
*----------------------------------------------------------------------------
* PARAMS:
* delay:    none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE: 
* This function sends the data stored in the USB transmit buffer.
* This function does nothing if there is no data in the buffer.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void uart_usb_flush (void)
{
  if (usb_configuration_nb != 0)
  {
      if (tx_counter)
      {
    	   Usb_select_ep(TX_EP);
    	   Usb_set_tx_ready();
         tx_counter = 0;
      }
  }
}


