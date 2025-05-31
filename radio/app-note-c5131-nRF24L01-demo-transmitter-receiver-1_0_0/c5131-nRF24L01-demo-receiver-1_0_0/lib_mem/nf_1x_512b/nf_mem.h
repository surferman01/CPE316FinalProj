//! @file nf_mem.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is a template for writing C software programs.
//!
//! This file contains the C51 RAM called CRAM management routines which are
//! used for Mass storage when doing ISP 
//! This file calls routines of the cram_mem.c file 
//!
//! @version 1.2 c5131-nRF24L01-demo-receiver-1_0_0 $Id: nf_mem.h,v 1.2 2005/03/24 12:10:42 lguilhau Exp $
//!
//! @todo
//! @bug
#ifndef _NFMEM_H_
#define _NFMEM_H_

#include "conf\conf_access.h"
#include "modules\control_access\ctrl_status.h"

#ifndef  USB_BOOST
#  error USB_BOOST must be defined with ENABLED or DISABLED in conf_access.h
#endif
#ifndef  DFC
#  error DFC must be defined with ENABLED or DISABLED in conf_access.h
#endif


//_____ D E F I N I T I O N S ______________________________________________

//! FAT specification
#define  ENTRY_SIZE           32    // size of entrie in byte



// First sector of the disk virtual (this disk isn't partitioned, only one partion)
#define  PBR_SECTOR           (0)
#define  FAT_SECTOR           (PBR_SECTOR  + VMEM_RESERVED_SIZE)
#define  ROOT_SECTOR          (FAT_SECTOR  + (VMEM_SIZE_FAT*VMEM_NB_FATS))
#define  FILE_SECTOR          (ROOT_SECTOR + ((ENTRY_SIZE*VMEM_NB_ROOT_ENTRY) / VMEM_SECTOR_SIZE))                   // 1(sector) = size root dir 

#define  FILE_SIZE            30     // Nb cluster disponible



//---- CONTROL FONCTIONS ----

// those fonctions are declared in nf_mem.h
Ctrl_status    nf_test_unit_ready(void);
Ctrl_status    nf_read_capacity( U32 _MEM_TYPE_SLOW_ *u32_nb_sector );
Bool           nf_wr_protect(void);
Bool           nf_removal(void);


//---- ACCESS DATA FONCTIONS ----

// Standard fonctions for open in read/write mode the device
Ctrl_status    nf_read_10( U32 addr , U16 nb_sector );
Ctrl_status    nf_write_10( U32 addr , U16 nb_sector );


//** If your device transfer have a specific transfer for USB (Particularity of Chejudo product, or bootloader)
#if (USB_BOOST == ENABLED)
   #include "lib_mcu\usb\usb_drv.h"    // In this case the driver must be know the USB access
   Ctrl_status nf_usb_read( void );
   Ctrl_status nf_usb_write( void );
#endif


//** If your device transfer the datas by DFC (For Qundau product)
#if (DFC == ENABLED)
   // Virtual memory have a specific DFC fonction for init the RAM
   #include "modules\control_access\dfc_mngt.h" // In this case the driver must be know the USB management
#endif   // DFC == ENABLED



#endif   // _NFMEM_H_

