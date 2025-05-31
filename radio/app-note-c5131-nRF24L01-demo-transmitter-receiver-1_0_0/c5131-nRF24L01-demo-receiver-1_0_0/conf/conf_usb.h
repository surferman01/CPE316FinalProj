//! @file conf_usb.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the USB
//! This file will be given to any external customer
//!
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0 $Id: conf_usb.h,v 1.1 2006/05/04 12:34:16 cgasnier Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#define USB_HS                DISABLE
#include "modules/usb/usb_enum.h"

#define USB_NORMAL_SPEED      
#define USB_FULL_SPEED        Usb_force_full_speed()
#define USB_HIGH_SPEED        Usb_force_high_spedd()
#define USB_FULL_SPEED_UTMI   Usb_force_full_speed_utmi()

#define Usb_set_speed()       USB_FULL_SPEED




/*_____ U S B    M A S S   S T O R A G E    D E F I N E _____________________*/

//#define NO_SUPPORT_USB_PING_PONG        // un-comment if MS is not used on Ping-Pong Endpoints

#define SBC_VENDOR_ID         {'A','T','M','E','L',' ',' ',' '}      // 8 Bytes only
#define SBC_PRODUCT_ID        {'A','T','8','9','C','5','1','3','1',' ','M','.','S','.',' ',' '}  // 16 Bytes only
#define SBC_REVISION_ID       {'0','.','0','0'}  // 4 Bytes only

#define Scsi_read_start_action()     // action to perform on new SCSI READ command
#define Scsi_read_stop_action()
#define Scsi_write_start_action()    // action to perform on new SCSI WRITE command
#define Scsi_write_stop_action()    
#define Scsi_start_action()          // action to perform on new SCSI command
                                                // except READ or WRITE


#define Usb_suspend_action()        Set_power_down_mode();    // go to powerdown
                                 // Set_idle_mode();    // go to idle


//_____ U S B    D E S C R I P T O R    T A B L E S ________________________

                                                   // USB Device descriptor 
#define USB_SPECIFICATION     0x0002
#define DEVICE_CLASS          0      // each configuration has its own class
#define DEVICE_SUB_CLASS      0      // each configuration has its own sub-class
#define DEVICE_PROTOCOL       0      // each configuration has its own protocol
#define EP_CONTROL_LENGTH     32
#define VENDOR_ID             0xEB03 // Atmel vendor ID = 03EBh 
#define PRODUCT_ID            0x1420 // Product ID: 2014h = MS + Mouse
#define RELEASE_NUMBER        0x0001
#define MAN_INDEX             0x01
#define PROD_INDEX            0x02
#define SN_INDEX              0x03
#define NB_CONFIGURATION      1

            // FULL SPEED MASS STORAGE CONFIGURATION 
                                         // USB Configuration descriptor FS 
#define NB_INTERFACE_FS       1//NO MS 2
#define CONF_NB_FS            1
#define CONF_INDEX_FS         0
#define CONF_ATTRIBUTES_FS    USB_CONFIG_BUSPOWERED
#define MAX_POWER_FS          50          // 100 mA 

                                             // USB Interface descriptor FS 
#define INTERFACE_NB_FS        0
#define ALTERNATE_FS           0
#define NB_ENDPOINT_FS         2
#define INTERFACE_CLASS_FS     0x08    // Mass Storage 
#define INTERFACE_SUB_CLASS_FS 0x06    // SCSI transparent Cammand Set
#define INTERFACE_PROTOCOL_FS  0x50    // Bulk-Only Transport
#define INTERFACE_INDEX_FS     0

                                            // USB Endpoint 1 descriptor FS 
#define ENDPOINT_NB_1_FS       (EP_MS_IN | 0x80)
#define EP_ATTRIBUTES_1_FS     0x02          // BULK = 0x02 
#define EP_IN_LENGTH_FS        64
#define EP_SIZE_1_FS           (wSWAP(EP_IN_LENGTH_FS))
#define EP_INTERVAL_1_FS       0x00

                                            // USB Endpoint 2 descriptor FS 
#define ENDPOINT_NB_2_FS       (EP_MS_OUT)
#define EP_ATTRIBUTES_2_FS     0x02          // BULK = 0x02 
#define EP_OUT_LENGTH_FS       64
#define EP_SIZE_2_FS           (wSWAP(EP_OUT_LENGTH_FS))
#define EP_INTERVAL_2_FS       0x00


            // FULL SPEED MOUSE CONFIGURATION 

#define INTERFACE_NB_MOUSE_FS        0//No MS 1
#define ALTERNATE_MOUSE_FS           0
#define NB_ENDPOINT_MOUSE_FS         1
#define INTERFACE_CLASS_MOUSE_FS     0x03    // HID 
#define INTERFACE_SUB_CLASS_MOUSE_FS 0x01    // Mouse
#define INTERFACE_PROTOCOL_MOUSE_FS  0x02    // 
#define INTERFACE_INDEX_MOUSE_FS     0

                                            // USB Endpoint 1 descriptor FS 
#define ENDPOINT_NB_3_FS             (EP_MOUSE_IN | 0x80)
#define EP_ATTRIBUTES_3_FS           0x03          // INTERRUPT = 0x03 
#define EP_IN_MOUSE_LENGTH_FS        4
#define EP_SIZE_3_FS                 (wSWAP(EP_IN_MOUSE_LENGTH_FS))
#define EP_INTERVAL_3_FS             0x0A



typedef struct
{
   S_usb_configuration_descriptor cfg;
  /* S_usb_interface_descriptor     ifc;
   S_usb_endpoint_descriptor      ep1;
   S_usb_endpoint_descriptor      ep2;No MS*/
   S_usb_interface_descriptor     ifc2;
   S_usb_hid_descriptor           hid;
   S_usb_endpoint_descriptor      ep3;
} S_usb_user_configuration_descriptor;

#define NB_ENDPOINTS          2//No MS4  // number of endpoints in the application 
#define EP_MS_IN              4
#define EP_MS_OUT             5
#define EP_MOUSE_IN           1


#define ENDPOINT_0            0x00 // OUT EP 
#define ENDPOINT_1            0x84 //  IN EP

#define NB_CONFIGURATIONS     0x02
#define DEVICE_STATUS         0x00 // TBD
#define INTERFACE_STATUS      0x00 // TBD

#define LANG_ID               0x00

#define USB_MANUFACTURER_NAME \
{ 'A'<<8 \
, 'T'<<8 \
, 'M'<<8 \
, 'E'<<8 \
, 'L'<<8 \
}

#define USB_PRODUCT_NAME                       \
   Usb_str_28('A', 'T', '8', '9', 'C', '5',      \
              '1', '3', '1', ' ',                \
              '-', ' ', 'U', 'S', 'B', ' ', 'M', \
              'a', 's', 's', ' ', 'S', 't',      \
              'o', 'r', 'a', 'g', 'e'            \
              )

                            // Serial Number should be at least 12 characters long
#define USB_SERIAL_NUMBER \
{ '1'<<8 \
, '.'<<8 \
, '3'<<8 \
, '.'<<8 \
, '5'<<8 \
, '.'<<8 \
, '7'<<8 \
, '.'<<8 \
, '9'<<8 \
, '.'<<8 \
, 'B'<<8 \
, '.'<<8 \
, 'D'<<8 \
}

#define HID_DESCRIPTOR        0x21
#define HID_BDC               0x1001
#define HID_COUNTRY_CODE      0x00
#define HID_CLASS_DESC_NB     0x01
#define HID_DESCRIPTOR_TYPE   0x22
#define HID_DESCRIPTOR_LENGTH (wSWAP((U16)0x34))

#define LANGUAGE_ID           0x0904
//_____ U S B   M A N U F A C T U R E R   D E S C R I P T O R _______________

#define USB_MN_LENGTH         5


//struct usb_st_manufacturer
typedef struct {
   U8  bLength;               // size of this descriptor in bytes 
   U8  bDescriptorType;       // STRING descriptor type           
   U16 wstring[USB_MN_LENGTH];// unicode characters               
} S_usb_manufacturer_string_descriptor;


//_____ U S B   P R O D U C T   D E S C R I P T O R _________________________

#define USB_PN_LENGTH         28

//struct usb_st_product
typedef struct {
   U8  bLength;               // size of this descriptor in bytes 
   U8  bDescriptorType;       // STRING descriptor type           
   U16 wstring[USB_PN_LENGTH];// unicode characters               
} S_usb_product_string_descriptor;


//_____ U S B   S E R I A L   N U M B E R   D E S C R I P T O R _____________

#define USB_SN_LENGTH         13

//struct usb_st_serial_number
typedef struct {
   U8  bLength;               // size of this descriptor in bytes 
   U8  bDescriptorType;       // STRING descriptor type           
   U16 wstring[USB_SN_LENGTH];// unicode characters               
} S_usb_serial_number;



    // write here the action to associate to each USB event
    // be carefull not to waste time in order not disturbing the functions

#define Usb_sof_action()
#define Usb_wake_up_action()
#define Usb_resume_action()
//#define Usb_suspend_action()
#define Usb_reset_action()
#define Usb_vbus_on_action()
#define Usb_vbus_off_action()
#define Usb_set_configuration_action()

    // write here the action to associate to each SCSI event
    // be carefull not to waste time in order not disturbing the functions


                                                             /* SCSI module */

                          /* write here the actions to execute
                             when a SCSI read or write occurs */
#define Scsi_start_read_action()
#define Scsi_stop_read_action()     //mail_push( MAILBOX_USE_EVT , EVT_MASS_STORAGE_READ  | (TASK_MMI << 8) , NULL);  
#define Scsi_start_write_action()
#define Scsi_stop_write_action()    //mail_push( MAILBOX_USE_EVT , EVT_MASS_STORAGE_WRITE | (TASK_MMI << 8) , NULL);



#define STANDARD_PORT           0
#define LED_PORT_2MA            1
#define LED_PORT_4MA            2
#define LED_PORT_10MA           3


#define Wdt_immediate_reset()   ((WDTRST = 0x1E), \
                                 (WDTRST = 0xE1), \
                                 (WDTRST = 0x3C), \
                                 (WDTRST = 0xC3))


#define Wdt_refresh()            ((WDTRST = 0x1E), \
                                  (WDTRST = 0xE1))

#define Configure_led(num,type) {LEDCON &= (0xFF & ~(0x03 << (num*2))); \
                                 LEDCON |= (type << (num*2));}

#define Led_0_on()              (P3_3 = 0)
#define Led_0_off()             (P3_3 = 1)
#define Led_0_toggle()          (P3_3 = ~P3_3)
#define Led_0_status()          (P3_3)
#define Led_0_write(x)          (P3_3 = x) 
#define Led_1_on()              (P3_5 = 0)
#define Led_1_off()             (P3_5 = 1)
#define Led_1_toggle()          (P3_5 = ~P3_5)
#define Led_1_status()          (P3_5)
#define Led_1_write(x)          (P3_5 = x) 
#define Led_2_on()              (P3_6 = 0)
#define Led_2_off()             (P3_6 = 1)
#define Led_2_toggle()          (P3_6 = ~P3_6)
#define Led_2_status()          (P3_6)
#define Led_2_write(x)          (P3_6 = x) 
#define Led_3_on()              (P3_7 = 0)
#define Led_3_off()             (P3_7 = 1)
#define Led_3_toggle()          (P3_7 = ~P3_7)
#define Led_3_status()          (P3_7)
#define Led_3_write(x)          (P3_7 = x) 


#define Led_all_on()            (P3 &= 0x17)
#define Led_all_off()           (P3 |= ~0x17)




#endif  //! _CONF_USB_H_


