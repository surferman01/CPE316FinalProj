//! @file usb_drv.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the USB driver routines.
//!
//! This file contains the USB driver routines.
//!
//! @version 1.19 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_drv.c,v 1.19 2006/04/06 12:58:12 cgasnier Exp $
//!
//! @todo
//! @bug

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "usb_drv.h"

//_____ M A C R O S ________________________________________________________

//_____ D E C L A R A T I O N ______________________________________________

//! usb_configure_endpoint.
//!
//!  This function configures an endpoint with the selected type.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param uc_data_receive   Number of the endpoint to configure.
//! @param i_length          Type to configure.
//!
//! @return Is_endpoint_configured.
//!
//!           The possible types are:
//!                CONTROL
//!                BULK_IN
//!                BULK_OUT
//!                INTERRUPT_IN
//!                INTERRUPT_OUT
//!                ISOCHRONOUS_IN
//!                ISOCHRONOUS_OUT
//!
//! EXAMPLE:
//! usb_configure_endpoint(EP_CONTROL,TYPE_CONTROL, DIRECTION_OUT, SIZE_64, ONE_BANK, NYET_DISABLED);
//!
//! NOTE:
//! The endpoint 0 shall always be configured in CONTROL type.
//!
U8 usb_config_ep(U8 config0, U8 config1)
{
    Usb_enable_endpoint();
    Usb_set_cfg0(config0); //    UECFG0X = config0;
    Usb_set_cfg1(config1);//    UECFG1X = (UECFG1X & MSK_ALLOC) | config1;
    UEPCONX = MSK_EPEN | (config0>>6) | ((config0 & 0x01)<<2);
    Usb_allocate_memory();
    return (TRUE);
}

//! usb_select_endpoint_interrupt.
//!
//! This function select the endpoint where an event occurs and returns the
//! number of this endpoint. If no event occurs on the endpoints, this
//! function returns 0.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return endpoint number.
//!
U8 usb_select_enpoint_interrupt(void)
{
U8 interrupt_flags;
U8 ep_num;

   ep_num = 0;
   interrupt_flags = Usb_interrupt_flags();

   while(ep_num < 9)
   {
      if (interrupt_flags & 1)
      {
         return (ep_num);
      }
      else
      {
         ep_num++;
         interrupt_flags = interrupt_flags >> 1;
      }
   }
   return 0;
}

//! usb_send_packet.
//!
//! This function moves the data pointed by tbuf to the selected endpoint fifo
//! and sends it through the USB.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param ep_num       number of the addressed endpoint
//! @param *tbuf        address of the first data to send
//! @param data_length  number of bytes to send
//!
//! @return address of the next U8 to send.
//!
//! Example:
//! usb_send_packet(3,&first_data,0x20);    // send packet on the endpoint #3
//! while(!(Usb_tx_complete));              // wait packet ACK'ed by the Host
//! Usb_clear_tx_complete();                     // acknowledge the transmit
//!
//! Note:
//! tbuf is incremented of 'data_length'.
//!
U8 usb_send_packet(U8 ep_num, U8* tbuf, U8 data_length)
{
U8 remaining_length;

   remaining_length = data_length;
   Usb_select_endpoint(ep_num);
   while(Is_usb_write_enabled() && (0 != remaining_length))
   {
      Usb_write_byte(*tbuf);
      remaining_length--;
      tbuf++;
   }
   Usb_send_control_in();
   return remaining_length;
}

//! usb_read_packet.
//!
//! This function moves the data stored in the selected endpoint fifo to 
//! the address specified by *rbuf.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param ep_num       number of the addressed endpoint
//! @param *rbuf        aaddress of the first data to write with the USB data
//! @param data_length  number of bytes to read
//!
//! @return address of the next U8 to send.
//!
//! Example:
//! while(!(Usb_rx_complete));                      // wait new packet received
//! usb_read_packet(4,&first_data,usb_get_nb_byte); // read packet from ep 4
//! Usb_clear_rx();                                 // acknowledge the transmit
//!
//! Note:
//! rbuf is incremented of 'data_length'.
//!
U8 usb_read_packet(U8 ep_num, U8* rbuf, U8  data_length)
{
U8 remaining_length;

   remaining_length = data_length;
   Usb_select_endpoint(ep_num);

   while(Is_usb_read_enabled() && (0 != remaining_length))
   {
      *rbuf = Usb_read_byte();
      remaining_length--;
      rbuf++;
   }
   return remaining_length;
}

//! usb_halt_endpoint.
//!
//! This function sends a STALL handshake for the next Host request. A STALL
//! handshake will be send for each next request untill a SETUP or a Clear Halt
//! Feature occurs for this endpoint.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param number of the addressed endpoint
//!
//! @return none
//!
/*void usb_halt_endpoint (Uchar ep_num)
{
   Usb_select_endpoint(ep_num);
   Usb_enable_stall_handshake();
}*/

//! usb_init_device.
//!
//! This function initializes the USB device controller and
//! configures the Default Control Endpoint.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
U8 usb_init_device (void)
{
   usb_configure_endpoint(EP_CONTROL,    \
                          TYPE_CONTROL,  \
                          DIRECTION_OUT, \
                          SIZE_64,       \
                          ONE_BANK,      \
                          NYET_DISABLED);
   return TRUE;
}

//! Usb_configure_clock
//!
//! This function configure the PLL to generate the 48MHz clock required by
//! the USB controller, considering the FOSC defined in the "config.h" file.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//! The possible value for FOSC are :
//!              3000   ( 3MHz)
//!              6000   ( 6MHz)
//!              8000   ( 8MHz)
//!             12000   (12MHz)
//!             16000   (16MHz)
//!             18000   (18MHz)
//!             20000   (20MHz)
//!             24000   (24MHz)
//!             32000   (32MHz)
//!             40000   (40MHz)
//!             48000   (48MHz)
//!              0000   (frequency auto-dectection)
//!
//! @return none
//!
void usb_generate_clock(void)
{
#if FOSC == 3000
  Pll_set_div(PLL_3MHz);
  Pll_enable();
#endif

#if FOSC == 4000
  Pll_set_div(PLL_4MHz);
  Pll_enable();
#endif

#if FOSC == 6000
  Pll_set_div(PLL_6MHz);
  Pll_enable();
#endif

#if FOSC == 8000
  Pll_set_div(PLL_8MHz);
  Pll_enable();
#endif

#if FOSC == 12000
  Pll_set_div(PLL_12MHz);
  Pll_enable();
#endif

#if FOSC == 13714
  Pll_set_div(PLL_13714MHz);
  Pll_enable();
#endif

#if FOSC == 16000
  Pll_set_div(PLL_16MHz);
  Pll_enable();
#endif

#if FOSC == 18000
  Pll_set_div(PLL_18MHz);
  Pll_enable();
#endif

#if FOSC == 20000
  Pll_set_div(PLL_20MHz);
  Pll_enable();
#endif

#if FOSC == 24000
  Pll_set_div(PLL_24MHz);
  Pll_enable();
#endif

#if FOSC == 32000
  Pll_set_div(PLL_32MHz);
  Pll_enable();
#endif

#if FOSC == 40000
  Pll_set_div(PLL_40MHz);
  Pll_enable();
#endif

#if FOSC == 48000
  Usb_set_EXT48();
#endif

#if FOSC == 0000            /* frequency Auto-detection */

Uchar i;
Uchar reload;
  
  PLLCON = 0x00;
  
  if (CKCON0 & MSK_X2)
  {
    CKCON0 |= MSK_T0X2;
    reload = 6; 
  }
  else
  {
    CKCON0 &= ~MSK_T0X2;
    reload = 9;
  }
  
  TMOD = 0x01;                            /* put Timer 0 in mode 1 */
  USBINT = 0x00;
  i=reload;
  
  while (!(USBINT & MSK_SOFINT))          /* Do until Start Of Frame detection */
  {
    if (i==9)
    {
      Pll_stop();                         /* external 48 MHz supposed */
      Usb_set_EXT48();
    }
    else
    {
      Usb_clear_EXT48();                  /* PLL output supposed */
      Pll_set_div(pll_value[i]);          /* configure PLL */
      Pll_enable();
    }
    TH0 = TH0_value[i];                   /* Run Timer 0 */
    TL0 = TL0_value[i];
    TCON |= 0x10;
    while(((TCON & 0x20) != 0x20));       /* Wait Timer 0 Overflow */
    TCON &= ~(0x30);
    if (i==0)
    {
      i=reload;
    }
    else
    {
      i--;
    }
  }

  TH0  = 0;                               /* Reset Timer 0 Registers */
  TL0  = 0;
  TCON = 0;
  TMOD = 0;
#endif
}



