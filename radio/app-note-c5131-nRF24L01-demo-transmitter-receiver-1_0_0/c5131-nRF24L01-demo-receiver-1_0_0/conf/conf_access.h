//! @file conf_access.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the control access
//! This file will be given to any external customer
//!
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0 $Id: conf_access.h,v 1.1 2006/05/04 12:34:16 cgasnier Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_ACCESS_H_
#define _CONF_ACCESS_H_


// Active the Logical Unit
#define  LUN_0                DISABLE
#define  LUN_1                ENABLE
#define  LUN_2                DISABLE
#define  LUN_3                DISABLE
#define  LUN_4                DISABLE
#define  LUN_5                DISABLE
#define  LUN_6                DISABLE
#define  LUN_7                DISABLE

// LUN 0 DEFINE
#define  LUN_0_INCLUDE                           "lib_mem\virtual_mem\virtual_mem.h"
#define  Lun_0_test_unit_ready()                 virtual_test_unit_ready()
#define  Lun_0_read_capacity(nb_sect)            virtual_read_capacity(nb_sect)
#define  Lun_0_wr_protect()                      virtual_wr_protect()
#define  Lun_0_removal()                         virtual_removal()
#define  Lun_0_read_10(ad, sec)                  virtual_read_10(ad, sec)
#define  Lun_0_usb_read()                        virtual_usb_read()
#define  Lun_0_write_10(ad, sec)                 virtual_write_10(ad, sec)
#define  Lun_0_usb_write()                       virtual_usb_write()

// LUN 1 DEFINE
#define  LUN_1_INCLUDE                           "lib_mem\nf_1x_512b\nf_mem.h"
#define  Lun_1_test_unit_ready()                 nf_test_unit_ready()
#define  Lun_1_read_capacity(nb_sect)            nf_read_capacity(nb_sect)
#define  Lun_1_wr_protect()                      nf_wr_protect()
#define  Lun_1_removal()                         nf_removal()
#define  Lun_1_read_10(ad, sec)                  nf_read_10(ad, sec)
#define  Lun_1_usb_read()                        nf_usb_read()
#define  Lun_1_write_10(ad, sec)                 nf_write_10(ad, sec)
#define  Lun_1_usb_write()                       nf_usb_write()

// ATMEL DEFINE - DO NOT MODIFY

// Define for access mode (Don't change)
#define  ACCESS_BY_REGISTER   0       
#define  ACCESS_BY_DFC        1

// Define the transfer access mode
#define  MODE_ACCESS          ACCESS_BY_REGISTER  // Transfer by register

// Active interface fonction
#define  ACCESS_USB           ENABLED  // USB I/O in control access
#define  ACCESS_STREAM        DISABLED  // Stream I/O in control access
#define  ACCESS_STREAM_RECORD DISABLED // Stream I/O in control access AND RECORD MODE
#define  ACCESS_MEM_TO_RAM    DISABLED  // RAM to Mem I/O in control access
#define  ACCESS_MEM_TO_MEM    DISABLED // Mem to Mem I/O in control access
#define  ACCESS_CODEC         DISABLED // Codec I/O in control access



// Specific option control access
#define  GLOBAL_WR_PROTECT    DISABLED // To manage a global write protection

#define USB_BOOST             ENABLED
#define DFC                   DISABLED

#endif  //! _CONF_ACCESS_H_

