//! @file usb_enum.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is a template for writing C software programs.
//!
//! This file contains the USB endpoint 0 management routines corresponding to
//! the standard enumeration process (refer to chapter 9 of the USB
//! specification.
//! This file calls routines of the usb_user_enum.c file for non-standard
//! request management.
//! The enumeration parameters (descriptor tables) are contained in the
//! usb_user_configuration.c file.
//!
//! @version 1.7 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_enum.c,v 1.7 2006/05/04 13:57:50 cgasnier Exp $
//!
//! @todo
//! @bug

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_usb.h"
#include "lib_mcu\usb\usb_drv.h"
#include "usb_enum.h"
#include "usb_user_configuration.h"
#include "usb_user_enum.h"
#include "modules\control_access\ctrl_access.h"

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________

//_____ P R I V A T E   D E C L A R A T I O N ______________________________
static  void    usb_get_descriptor(   void);
static  void    usb_read_request(     void);
static  void    usb_set_address(      void);
static  void    usb_set_configuration(void);
static  void    usb_clear_feature(    void);
static  void    usb_set_feature(      void);
static  void    usb_get_status(       void);
static  void    usb_get_configuration(void);

//_____ D E C L A R A T I O N ______________________________________________
#ifndef NO_SUPPORT_USB_PING_PONG
   bdata bit   rx_bank;
#endif
        bit  zlp;
static  _MEM_TYPE_SLOW_ U8   endpoint_status[7];
        _MEM_TYPE_SLOW_ U8   *pbuffer;
        _MEM_TYPE_SLOW_ U8   data_to_transfer;


static  U8   bmRequestType;

_MEM_TYPE_SLOW_ U8   usb_configuration_nb;
extern  bit     usb_connected;
extern  code    S_usb_device_descriptor             usb_user_device_descriptor;
extern  code    S_usb_user_configuration_descriptor usb_user_configuration_descriptor;


//! usb_enum_variable_init.
//!
//! This function initializes the main usb variables:
//! - endpoint status
//! - connection status
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_enum_var_init (void)
{
U8 ep_num ;

   for( ep_num=0 ; ep_num<NB_ENDPOINTS ; ep_num++ )
   {
      endpoint_status[ep_num] = 0;
   }
   usb_connected        = FALSE;         //!< USB is not connected
   usb_configuration_nb = 0    ;         //!< Default configuration number is 0
#ifndef NO_SUPPORT_USB_PING_PONG
   rx_bank = 0;
#endif

}


//! usb_process_request.
//!
//! This function reads the SETUP request sent to the default control endpoint
//! and calls the appropriate function. When exiting of the usb_read_request
//! function, the device is ready to manage the next request.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//! NOTE: list of supported requests:
//!               GET_DESCRIPTOR
//!               GET_CONFIGURATION
//!               SET_ADDRESS
//!               SET_CONFIGURATION
//!               CLEAR_FEATURE
//!               SET_FEATURE
//!               GET_STATUS
//!
void usb_process_request(void)
{ 
U8 bmRequest;

   Usb_select_endpoint(EP_CONTROL); 

   bmRequestType = Usb_read_byte();
   bmRequest     = Usb_read_byte();

   switch (bmRequest)
   {
    case GET_DESCRIPTOR:
         if (0x80 == bmRequestType) { usb_get_descriptor(); }
         else                       { usb_user_read_request(bmRequestType, bmRequest); }
         break;

    case GET_CONFIGURATION: 
         if (0x80 == bmRequestType) { usb_get_configuration(); }
         else                       { usb_user_read_request(bmRequestType, bmRequest); }
         break;

    case SET_ADDRESS:
         if (0x00 == bmRequestType) { usb_set_address(); }
         else                       { usb_user_read_request(bmRequestType, bmRequest); }
         break;

    case SET_CONFIGURATION:
         if (0x00 == bmRequestType) { usb_set_configuration(); }
         else                       { usb_user_read_request(bmRequestType, bmRequest); }
         break;

    case CLEAR_FEATURE:
         if (0x02 >= bmRequestType) { usb_clear_feature(); }
         else                       { usb_user_read_request(bmRequestType, bmRequest); }
         break;

    case SET_FEATURE:
         if (0x02 >= bmRequestType) { usb_set_feature(); }
         else                       { usb_user_read_request(bmRequestType, bmRequest); }
         break;

    case GET_STATUS:
         if ((0x7F < bmRequestType) & (0x82 >= bmRequestType))
                                    { usb_get_status(); }
         else                       { usb_user_read_request(bmRequestType, bmRequest); }
         break;

    case GET_INTERFACE:
    case SET_DESCRIPTOR:
    case SET_INTERFACE:
    case SYNCH_FRAME:
    default: //!< un-supported request => call to user read request
         if(usb_user_read_request(bmRequestType, bmRequest) == FALSE)
         {
            Usb_enable_stall_handshake();
            Usb_ack_receive_setup();
            return;
         }
         break;
  }
}


//! usb_set_address.
//!
//! This function manages the SET ADDRESS request. When complete, the device
//! will filter the requests using the new address.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_set_address(void)
{
U8 addr;

   addr = Usb_read_byte();
   Usb_ack_receive_setup();

   Usb_send_control_in();                    //!< send a ZLP for STATUS phase
   while(!Is_usb_in_ready());                //!< waits for status phase done
                                             //!< before using the new address
   Usb_ack_in_ready();              // for Tiger compatibility
   Usb_configure_address(addr);
   Usb_enable_address();
}


//! usb_set_configuration.
//!
//! This function manages the SET CONFIGURATION request. If the selected
//! configuration is valid, this function call the usb_user_endpoint_init()
//! function that will configure the endpoints following the configuration
//! number.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_set_configuration( void )
{
U8 configuration_number;

   configuration_number = Usb_read_byte();

   if (configuration_number <= NB_CONFIGURATIONS)
   {
      Usb_ack_receive_setup();
      usb_configuration_nb = configuration_number;
   }
   else
   {
      //!< keep that order (set StallRq/clear RxSetup) or a
      //!< OUT request following the SETUP may be acknowledged
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
      return;
   }

   Usb_send_control_in();                    //!< send a ZLP for STATUS phase
   while(!Is_usb_in_ready());
   Usb_ack_in_ready();

   usb_user_endpoint_init(usb_configuration_nb);  //!< endpoint configuration
   Usb_set_configuration_action();
}


//! usb_get_descriptor.
//!
//! This function manages the GET DESCRIPTOR request. The device descriptor,
//! the configuration descriptor and the device qualifier are supported. All
//! other descriptors must be supported by the usb_user_get_descriptor
//! function.
//! Only 1 configuration is supported.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_get_descriptor(void)
{
U16  wLength         ;
U8   descriptor_type ;
U8   string_type     ;
U8   c;                    // New

   zlp             = FALSE;                  /* no zero length packet */
   string_type     = Usb_read_byte();        /* read LSB of wValue    */
   descriptor_type = Usb_read_byte();        /* read MSB of wValue    */
  
   switch (descriptor_type)
   {
    case DEVICE_DESCRIPTOR:
      data_to_transfer = Usb_get_dev_desc_length(); //!< sizeof (usb_user_device_descriptor);
      pbuffer          = Usb_get_dev_desc_pointer();
      break;
    case CONFIGURATION_DESCRIPTOR:
      data_to_transfer = Usb_get_conf_desc_length(); //!< sizeof (usb_user_configuration_descriptor);
      pbuffer          = Usb_get_conf_desc_pointer();
      break;
    default:
      if( usb_user_get_descriptor(descriptor_type, string_type)==FALSE )
      {
         Usb_enable_stall_handshake();
         Usb_ack_receive_setup();
         return;
      }
      break;
   }

   ACC = Usb_read_byte();                     //!< don't care of wIndex field
   ACC = Usb_read_byte();
   ((U8*)&wLength)[1] = Usb_read_byte();      //!< read wLength
   ((U8*)&wLength)[0] = Usb_read_byte();
   Usb_ack_receive_setup() ;                  //!< clear the receive setup flag
   Usb_set_DIR();                            /* set out on EP0 */

   if (wLength > data_to_transfer)
   {
      if ((data_to_transfer % EP_CONTROL_LENGTH) == 0) { zlp = TRUE; }
      else { zlp = FALSE;}                   //!< no need of zero length packet
   }
   else
   {
      data_to_transfer = (U8)wLength;         //!< send only requested number of data
   }

   while((data_to_transfer != 0) && (!Is_usb_receive_out()))
   {
      if (data_to_transfer > EP_CONTROL_LENGTH) { c = EP_CONTROL_LENGTH; }
                                           else { c = data_to_transfer;  }
      data_to_transfer -= c;
      while ( c!= 0 )
      {
         Usb_write_byte(*pbuffer);
         c--;
         pbuffer ++;
      }
      Usb_send_control_in();
      while((!Is_usb_read_control_enabled()) && (!Is_usb_receive_out()));
      Usb_ack_in_ready();
   }
  
   if(Is_usb_receive_out())   //!< abort from Host
   {
      Usb_ack_receive_out();
      return;
   }
   if(zlp == TRUE)
   {
      Usb_send_control_in();
      while((!Is_usb_read_control_enabled()) && (!Is_usb_receive_out()));
      Usb_ack_in_ready();
   }

   while(!Is_usb_receive_out());
   Usb_ack_receive_out();
}


//! usb_get_configuration.
//!
//! This function manages the GET CONFIGURATION request. The current
//! configuration number is returned.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_get_configuration(void)
{
   Usb_ack_receive_setup();
   Usb_set_DIR();                            /* set out on EP0 */

   Usb_write_byte(usb_configuration_nb);
   Usb_send_control_in();
   while(!Is_usb_in_ready());
   Usb_ack_in_ready();
   while( !Is_usb_receive_out() );
   Usb_ack_receive_out();
}

//! usb_get_status.
//!
//! This function manages the GET STATUS request. The device, interface or
//! endpoint status is returned.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_get_status(void)
{
U8 wIndex;

   ACC    = Usb_read_byte();                 //!< dummy read
   ACC    = Usb_read_byte();                 //!< dummy read
   wIndex = Usb_read_byte();

   switch(bmRequestType)
   {
    case REQUEST_DEVICE_STATUS:    Usb_ack_receive_setup();
                                   Usb_set_DIR();
                                   Usb_write_byte(DEVICE_STATUS);
                                   break;

    case REQUEST_INTERFACE_STATUS: Usb_ack_receive_setup();
                                   Usb_set_DIR();
                                   Usb_write_byte(INTERFACE_STATUS);
                                   break;

    case REQUEST_ENDPOINT_STATUS:  Usb_ack_receive_setup();
                                   Usb_set_DIR();
                                   wIndex = wIndex & MSK_EP_DIR;
                                   Usb_write_byte(endpoint_status[wIndex]);
                                   break;
    default:
                                   Usb_enable_stall_handshake();
                                   Usb_ack_receive_setup();
                                   return;
   }

   Usb_write_byte(0x00);
   Usb_send_control_in();
   while(!Is_usb_in_ready());
   Usb_ack_in_ready();
   while( !Is_usb_receive_out() );
   Usb_ack_receive_out();
}


//! usb_set_feature.
//!
//! This function manages the SET FEATURE request. The USB test modes are
//! supported by this function.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_set_feature(void)
{
U8 wValue;
U8 wIndex;

   if (bmRequestType == INTERFACE_TYPE)
   {
      //!< keep that order (set StallRq/clear RxSetup) or a
      //!< OUT request following the SETUP may be acknowledged
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
      return;
   }
   else if (bmRequestType == ENDPOINT_TYPE)
   {
      wValue = Usb_read_byte();
      ACC    = Usb_read_byte();                //!< dummy read

      if (wValue == FEATURE_ENDPOINT_HALT)
      {
         wIndex = (Usb_read_byte() & MSK_EP_DIR);
      
         if (wIndex == EP_CONTROL)
         {
            Usb_enable_stall_handshake();
            Usb_ack_receive_setup();
            return;
         }
      
         Usb_select_endpoint(wIndex);
         if(Is_usb_endpoint_enabled())
         {
            Usb_enable_stall_handshake();
            Usb_select_endpoint(EP_CONTROL);
            endpoint_status[wIndex] = 0x01;
            Usb_ack_receive_setup();
            Usb_send_control_in();
         }
         else
         {
            Usb_enable_stall_handshake();
            Usb_ack_receive_setup();
            return;
         }
      }
      else
      {
         Usb_enable_stall_handshake();
         Usb_ack_receive_setup();
         return;
      }
   }
}


//! usb_clear_feature.
//!
//! This function manages the SET FEATURE request.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_clear_feature(void)
{
U8 wValue;
U8 wIndex;

   if (bmRequestType == ZERO_TYPE)
   {
      //!< keep that order (set StallRq/clear RxSetup) or a
      //!< OUT request following the SETUP may be acknowledged
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
      return;
   }
   else if (bmRequestType == INTERFACE_TYPE)
   {
      //!< keep that order (set StallRq/clear RxSetup) or a
      //!< OUT request following the SETUP may be acknowledged
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
      return;
   }
   else if (bmRequestType == ENDPOINT_TYPE)
   {
      wValue = Usb_read_byte();
      ACC    = Usb_read_byte();                //!< dummy read

      if (wValue == FEATURE_ENDPOINT_HALT)
      {
         wIndex = (Usb_read_byte() & MSK_EP_DIR);

         Usb_select_endpoint(wIndex);
         if(Is_usb_endpoint_enabled())
         {
            if(wIndex != EP_CONTROL)
            {
               Usb_disable_stall_handshake();
               Usb_reset_endpoint(wIndex);
               Usb_reset_data_toggle();
            }
            Usb_select_endpoint(EP_CONTROL);
            endpoint_status[wIndex] = 0x00;
            Usb_ack_receive_setup();
            Usb_send_control_in();
         }
         else
         {
            Usb_enable_stall_handshake();
            Usb_ack_receive_setup();
            return;
         }
      }
      else
      {
         Usb_enable_stall_handshake();
         Usb_ack_receive_setup();
         return;
      }
   }
}
