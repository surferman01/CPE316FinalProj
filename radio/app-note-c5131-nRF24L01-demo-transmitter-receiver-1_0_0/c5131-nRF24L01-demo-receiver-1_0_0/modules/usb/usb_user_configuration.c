//! @file usb_user_configuration.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Mass Storage USB parameters.
//!
//! This file contains the usb parameters that uniquely identify the
//! application through descriptor tables.
//! MASS STORAGE DEVICE
//!
//! @version 1.7 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_user_configuration.c,v 1.7 2006/05/04 09:17:27 cgasnier Exp $
//!
//! @todo
//! @bug


//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_usb.h"
#include "lib_mcu\usb\usb_drv.h"
#include "usb_enum.h"
#include "usb_user_configuration.h"


//_____ M A C R O S ________________________________________________________




//_____ D E F I N I T I O N ________________________________________________
                                              // usb_user_device_descriptor

code S_usb_device_descriptor usb_dev_desc_ms =
{
  sizeof(usb_dev_desc_ms)
, DEVICE_DESCRIPTOR
, USB_SPECIFICATION
, DEVICE_CLASS
, DEVICE_SUB_CLASS
, DEVICE_PROTOCOL
, EP_CONTROL_LENGTH
, VENDOR_ID
, PRODUCT_ID
, RELEASE_NUMBER
, MAN_INDEX
, PROD_INDEX
, SN_INDEX
, NB_CONFIGURATION
};

                                              // usb_device_qualifier_descriptor
code S_usb_device_qualifier_descriptor usb_qual_desc_ms =
{
  sizeof(S_usb_device_qualifier_descriptor)
, DEVICE_QUALIFIER_DESCRIPTOR
, USB_SPECIFICATION
, DEVICE_CLASS
, DEVICE_SUB_CLASS
, DEVICE_PROTOCOL
, EP_CONTROL_LENGTH
, NB_CONFIGURATION
, 0x00
};

                                    // usb_user_configuration_descriptor FS
code S_usb_user_configuration_descriptor usb_conf_desc_ms_fs = {
 { sizeof(S_usb_configuration_descriptor)
 , CONFIGURATION_DESCRIPTOR
 , wSWAP(sizeof(usb_conf_desc_ms_fs))
 , NB_INTERFACE_FS
 , CONF_NB_FS
 , CONF_INDEX_FS
 , CONF_ATTRIBUTES_FS
 , MAX_POWER_FS
 }
/* ,
 { sizeof(S_usb_interface_descriptor)    
 , INTERFACE_DESCRIPTOR
 , INTERFACE_NB_FS
 , ALTERNATE_FS
 , NB_ENDPOINT_FS
 , INTERFACE_CLASS_FS
 , INTERFACE_SUB_CLASS_FS
 , INTERFACE_PROTOCOL_FS
 , INTERFACE_INDEX_FS
 }
 ,
 { sizeof(S_usb_endpoint_descriptor)     
 , ENDPOINT_DESCRIPTOR
 , ENDPOINT_NB_1_FS
 , EP_ATTRIBUTES_1_FS
 , EP_SIZE_1_FS
 , EP_INTERVAL_1_FS
 }
 ,
 { sizeof(S_usb_endpoint_descriptor)     
 , ENDPOINT_DESCRIPTOR
 , ENDPOINT_NB_2_FS
 , EP_ATTRIBUTES_2_FS
 , EP_SIZE_2_FS
 , EP_INTERVAL_2_FS
 }No MS*/
 ,
 { sizeof(S_usb_interface_descriptor)    
 , INTERFACE_DESCRIPTOR
 , INTERFACE_NB_MOUSE_FS
 , ALTERNATE_MOUSE_FS
 , NB_ENDPOINT_MOUSE_FS
 , INTERFACE_CLASS_MOUSE_FS
 , INTERFACE_SUB_CLASS_MOUSE_FS
 , INTERFACE_PROTOCOL_MOUSE_FS
 , INTERFACE_INDEX_MOUSE_FS
 }
 ,
 { sizeof(S_usb_hid_descriptor)
 , HID_DESCRIPTOR
 , HID_BDC
 , HID_COUNTRY_CODE
 , HID_CLASS_DESC_NB
 , HID_DESCRIPTOR_TYPE
 , HID_DESCRIPTOR_LENGTH
 }
 ,
 { sizeof(S_usb_endpoint_descriptor)     
 , ENDPOINT_DESCRIPTOR
 , ENDPOINT_NB_3_FS
 , EP_ATTRIBUTES_3_FS
 , EP_SIZE_3_FS
 , EP_INTERVAL_3_FS
 }
};

                                      // usb_user_manufacturer_string_descriptor
code S_usb_manufacturer_string_descriptor usb_user_manufacturer_string_descriptor = {
  sizeof(usb_user_manufacturer_string_descriptor)
, STRING_DESCRIPTOR
, USB_MANUFACTURER_NAME
};


                                      // usb_user_product_string_descriptor

code S_usb_product_string_descriptor usb_user_product_string_descriptor = {
  sizeof(usb_user_product_string_descriptor)
, STRING_DESCRIPTOR
, USB_PRODUCT_NAME
};


                                      // usb_user_serial_number

code S_usb_serial_number usb_user_serial_number = {
  sizeof(usb_user_serial_number)
, STRING_DESCRIPTOR
, USB_SERIAL_NUMBER
};


                                      // usb_user_language_id

code S_usb_language_id usb_user_language_id = {
  sizeof(usb_user_language_id)
, STRING_DESCRIPTOR
, LANGUAGE_ID
};

code S_usb_hid_report_descriptor usb_hid_report_descriptor = {
      0x05,0x01,          /* Usage Page (Generic Desktop)      */        
      0x09,0x02,          /* Usage (Mouse)                     */
      0xA1,0x01,          /* Collection (Application)          */
      0x09,0x01,          /* Usage (Pointer)                   */
      0xA1,0x00,          /* Collection (Application)          */
      0x05,0x09,          /* Usage Page (Button)               */
      0x19,0x01,          /* Usage Minimum (1)                 */
      0x29,0x03,          /* Usage Maximum (3)                 */
      0x15,0x00,          /* Logical Minimum (0)               */
      0x25,0x01,          /* Logical Maximum (1)               */
      0x75,0x01,          /* Report Size (1)                   */
      0x95,0x03,          /* Report Count (3)                  */
      0x81,0x02,          /* Input (Data, Variable, Absolute)  */
      0x75,0x05,          /* Report Size (5)                   */
      0x95,0x01,          /* Report Count (1)                  */
      0x81,0x01,          /* Input (Constant)                  */
      0x05,0x01,          /* Usage Page (Generic Desktop Control)*/
      0x09,0x30,          /* Usage X                           */
      0x09,0x31,          /* Usage Y                           */
      0x09,0x38,          /* Usage Wheel                       */
      0x15,0x81,          /* Logical Minimum (-127)            */
      0x25,0x7F,          /* Logical Maximum (127)             */
      0x75,0x08,          /* Report Size (8)                   */
      0x95,0x03,          /* Report Count (3)                  */
      0x81,0x06,          /* Input (Data, Variable, Absolute)  */
      0xC0,               /* End Collection                    */
      0xC0                /* End Collection                    */ 
};
