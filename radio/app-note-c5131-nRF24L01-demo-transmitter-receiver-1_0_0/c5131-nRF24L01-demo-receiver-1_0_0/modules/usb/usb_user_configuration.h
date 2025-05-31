//! @file usb_user_configuration.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Mass Storage USB Identifers.
//!
//! This file contains the usb parameters that uniquely identify the
//! Mass Storage application through descriptor tables.
//! 
//! @version 1.8 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_user_configuration.h,v 1.8 2006/05/04 13:50:41 cgasnier Exp $
//!
//! @todo
//! @bug

#ifndef _USB_USERCONFIG_H_
#define _USB_USERCONFIG_H_

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_usb.h"
#include "usb_enum.h"

//_____ M A C R O S ________________________________________________________

#define Usb_get_dev_desc_pointer()        (&(usb_dev_desc_ms.bLength))
#define Usb_get_dev_desc_length()         (sizeof (usb_dev_desc_ms))
#define Usb_get_conf_desc_fs_pointer()    (&(usb_conf_desc_ms_fs.cfg.bLength))
#define Usb_get_conf_desc_fs_length()     (sizeof (usb_conf_desc_ms_fs))



#define Usb_get_conf_desc_pointer()       Usb_get_conf_desc_fs_pointer()
#define Usb_get_conf_desc_length()        Usb_get_conf_desc_fs_length()


//_____ U S B    D E F I N E _______________________________________________

#define MASS_STORAGE_RESET    0xFF
#define GET_MAX_LUN           0xFE

#define HID_SET_IDLE          0x0A
#define HID_GET_IDLE          0x02
#define HID_SET_REPORT        0x09
#define SIZE_OF_REPORT        HID_DESCRIPTOR_LENGTH
#define HID_SET_PROTOCOL      0x0B
#define HID_GET_PROTOCOL      0x03



//_____ D E F I N I T I O N ________________________________________________



#endif

