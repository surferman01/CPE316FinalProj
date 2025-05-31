//! @file usb_user_enum.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief user call-back functions
//!
//! This file contains the user call-back functions corresponding to the
//! application:
//! MASS STORAGE DEVICE
//!
//! @version 1.5 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_user_enum.c,v 1.5 2006/05/04 09:17:17 cgasnier Exp $
//!
//! @todo
//! @bug


//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_usb.h"
#include "lib_mcu\usb\usb_drv.h"
#include "modules\usb\usb_enum.h"
#include "usb_user_configuration.h"
#include "usb_user_enum.h"
#include "modules\control_access\ctrl_access.h"

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________

//_____ P R I V A T E   D E C L A R A T I O N ______________________________

extern _MEM_TYPE_SLOW_ U8   *pbuffer;
extern _MEM_TYPE_SLOW_ U8   data_to_transfer;
extern                 bit  zlp;
extern U8 xdata hid_idle_duration;
       U8 xdata hid_protocol;

       bit  ms_multiple_drive;

//_____ D E C L A R A T I O N ______________________________________________

//! usb_user_read_request(type, request);
//!
//! This function is called by the standard usb read request function when
//! the Usb request is not supported. This function returns TRUE when the
//! request is processed. This function returns FALSE if the request is not
//! supported. In this case, a STALL handshake will be automatically 
//! sent by the standard usb read request function.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
Bool usb_user_read_request(U8 type, U8 request)
{
U16  wLength         ;
U8   c;

  switch (request)
  {
    case MASS_STORAGE_RESET:
         Usb_ack_receive_setup();
         Usb_send_control_in();
         return TRUE;
         break;

    case GET_MAX_LUN:
         Usb_ack_receive_setup();
         Usb_set_DIR();                            /* set out on EP0 */
         Usb_write_byte( (get_nb_lun()-1) );
         Usb_send_control_in();
         while(!Is_usb_read_control_enabled());
         Usb_ack_in_ready();
         ms_multiple_drive = 1;
         return TRUE;
         break;

    case GET_DESCRIPTOR:
         if(type == 0x81)
         {
            ACC = Usb_read_byte();                     //!< don't care of wIndex field
            type = Usb_read_byte();
            if (type == 0x22)
            {
               data_to_transfer = sizeof (usb_hid_report_descriptor);
               pbuffer          = &(usb_hid_report_descriptor.report[0]);
            }
            else
            {
               data_to_transfer = sizeof (usb_conf_desc_ms_fs.hid);
               pbuffer          = &(usb_conf_desc_ms_fs.hid.bLength);
            }

            ACC = Usb_read_byte();
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
               return TRUE;
            }
            if(zlp == TRUE)
            {
               Usb_send_control_in();
               while((!Is_usb_read_control_enabled()) && (!Is_usb_receive_out()));
               Usb_ack_in_ready();
            }

            while(!Is_usb_receive_out());
            Usb_ack_receive_out();
            return TRUE;
         }
         break;
    case HID_SET_IDLE:
         ACC = Usb_read_byte();
         hid_idle_duration = Usb_read_byte();      /* wValue contains the duration */
         Usb_ack_receive_setup();

         Usb_send_control_in();
         return TRUE;
         break;
    case HID_GET_IDLE:
         Usb_ack_receive_setup();
         Usb_set_DIR();
         Usb_write_byte(hid_idle_duration);
         Usb_send_control_in();
         while(!Is_usb_read_control_enabled());
         Usb_ack_in_ready();
         return TRUE;
         break;
    case HID_SET_PROTOCOL:
         hid_protocol = Usb_read_byte();      /* wValue contains the duration */
         Usb_ack_receive_setup();

         Usb_send_control_in();
         return TRUE;
         break;
    case HID_GET_PROTOCOL:
         Usb_ack_receive_setup();
         Usb_set_DIR();
         Usb_write_byte(hid_protocol);
         Usb_send_control_in();
         while(!Is_usb_read_control_enabled());
         Usb_ack_in_ready();
         return TRUE;
         break;

    default:
         return FALSE;
         break;
  }
  return FALSE;
}


//! usb_user_get_descriptor.
//!
//! This function returns the size and the pointer on a user information
//! structure
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
Bool usb_user_get_descriptor(U8 type, U8 string)
{
  switch (type)
  {
    case STRING_DESCRIPTOR:
      if (string == 0)
      {
        data_to_transfer = sizeof (usb_user_language_id);
        pbuffer          = &usb_user_language_id.bLength;
        return TRUE;
      }
      else
      if (string == 1)
      {
        data_to_transfer = sizeof (usb_user_manufacturer_string_descriptor);
        pbuffer          = &usb_user_manufacturer_string_descriptor.bLength;
        return TRUE;
      }
      else
      if (string == 2)
      {
        data_to_transfer = sizeof (usb_user_product_string_descriptor);
        pbuffer          = &usb_user_product_string_descriptor.bLength;
        return TRUE;
      }
      else
      if (string == 3)
      {
        data_to_transfer = sizeof (usb_user_serial_number);
        pbuffer          = &usb_user_serial_number.bLength;
        return TRUE;
      }
      else
      {
        return FALSE;
      }
      break;
    default:
      return FALSE;
      break;
  }
}


//! usb_user_endpoint_init.
//!
//! This function configures the endpoints.
//!
//! @warning Code:xx bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
void usb_user_endpoint_init(U8 conf_nb)
{
  U8 ep_size;

  conf_nb = conf_nb;                    // FIXME (added for warnings)

  if(Is_usb_high_speed()) { ep_size = SIZE_512; } else { ep_size = SIZE_64; }

 /* usb_configure_endpoint(EP_MS_IN,      \
                         TYPE_BULK,     \
                         DIRECTION_IN,  \
                         ep_size,       \
                         TWO_BANKS,     \
                         NYET_ENABLED);
  usb_configure_endpoint(EP_MS_OUT,     \
                         TYPE_BULK,     \
                         DIRECTION_OUT, \
                         ep_size,       \
                         TWO_BANKS,     \
                         NYET_ENABLED);No MS*/

  usb_configure_endpoint(EP_MOUSE_IN,   \
                         TYPE_INTERRUPT,\
                         DIRECTION_IN,  \
                         4,             \
                         ONE_BANK,      \
                         NYET_DISABLED);
}
