/*H**************************************************************************
* NAME:         usb_user_enum.h
*----------------------------------------------------------------------------
* Copyright (c) 2004 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5131-nRF24L01-demo-receiver-1_0_0      
* REVISION:     1.4     
*----------------------------------------------------------------------------
* PURPOSE:
* This file contains the user call-back functions corresponding to the
* application:
* MASS STORAGE DEVICE
*****************************************************************************/

#ifndef _USB_USER_ENUM_H_
#define _USB_USER_ENUM_H_

/*_____ I N C L U D E S ____________________________________________________*/

#include "config.h"

/*_____ M A C R O S ________________________________________________________*/

extern  code S_usb_device_descriptor usb_dev_desc_ms;
extern  code S_usb_user_configuration_descriptor usb_conf_desc_ms_hs;
extern  code S_usb_user_configuration_descriptor usb_conf_desc_ms_fs;
extern  code S_usb_user_configuration_descriptor usb_other_conf_desc_ms;
extern  code S_usb_device_qualifier_descriptor usb_qual_desc_ms;
extern  code S_usb_manufacturer_string_descriptor usb_user_manufacturer_string_descriptor;
extern  code S_usb_product_string_descriptor usb_user_product_string_descriptor;
extern  code S_usb_serial_number usb_user_serial_number;
extern  code S_usb_language_id usb_user_language_id;
extern  code S_usb_hid_report_descriptor usb_hid_report_descriptor;


/*_____ D E F I N I T I O N ________________________________________________*/
Bool  usb_user_read_request(U8, U8);
Bool  usb_user_get_descriptor(U8 , U8);
void  usb_user_endpoint_init(U8);

#endif

