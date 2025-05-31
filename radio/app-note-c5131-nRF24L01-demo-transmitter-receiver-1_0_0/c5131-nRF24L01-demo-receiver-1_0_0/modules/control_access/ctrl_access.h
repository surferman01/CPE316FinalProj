//! @file ctrl_access.h,v
//!
//! Copyright (c) 2004-Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the interface :
//! - between USB <-> MEMORY
//! OR
//! - between USB <- Access Memory Ctrl -> Memory
//!
//! This interface may be controled by a "Access Memory Control" for :
//! - include a management of write protect global or specific
//! - include a management of transfer (ex: DFC on Qundau product)
//! - include a management of access password
//! - ...
//!
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0 $Id: ctrl_access.h,v 1.1 2005/03/10 08:29:43 lguilhau Exp $
//!
//! @todo
//! @bug


#ifndef _MEM_CTRL_H_
#define _MEM_CTRL_H_

#include "conf\conf_access.h"
#include "ctrl_status.h"

// FYC: Memory = Logical Unit
#if (LUN_0 == ENABLE)
   #include    LUN_0_INCLUDE
#endif
#if (LUN_1 == ENABLE)
   #include    LUN_1_INCLUDE
#endif
#if (LUN_2 == ENABLE)
   #include    LUN_2_INCLUDE
#endif
#if (LUN_3 == ENABLE)
   #include    LUN_3_INCLUDE
#endif
#if (LUN_4 == ENABLE)
   #include    LUN_4_INCLUDE
#endif
#if (LUN_5 == ENABLE)
   #include    LUN_5_INCLUDE
#endif
#if (LUN_6 == ENABLE)
   #include    LUN_6_INCLUDE
#endif
#if (LUN_7 == ENABLE)
   #include    LUN_7_INCLUDE
#endif


//------- Test the configuration in conf_access.h

// Specific option control access
#ifndef  GLOBAL_WR_PROTECT
#  error GLOBAL_WR_PROTECT must be defined with ENABLED or DISABLED in conf_access.h
#endif
//------- END OF Test the configuration in conf_access.h



//_____ D E F I N I T I O N S ______________________________________________



//!**** Listing of commun interface ****************************************

U8             get_nb_lun();
U8             get_cur_lun();
Ctrl_status    mem_test_unit_ready( U8 lun );
Ctrl_status    mem_read_capacity( U8 lun , U32 _MEM_TYPE_SLOW_ *u32_nb_sector );
Bool           mem_wr_protect( U8 lun );
Bool           mem_removal( U8 lun );


//!**** Listing of READ/WRITE interface ************************************


//---- Interface for USB ---------------------------------------------------
   Ctrl_status memory_2_usb( U8 lun , U32 addr , U16 nb_sector );
   Ctrl_status usb_2_memory( U8 lun , U32 addr , U16 nb_sector );   
#include "lib_mcu\usb\usb_drv.h"
#include "conf\conf_usb.h"
//--------------------------------------------------------------------------




#endif   // _MEM_CTRL_H_

