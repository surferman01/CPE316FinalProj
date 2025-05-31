//! @file virtual_mem.h,v
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
//! @version 1.5 c5131-nRF24L01-demo-receiver-1_0_0 $Id: virtual_mem.h,v 1.5 2005/03/22 11:50:22 lguilhau Exp $
//!
//! @todo
//! @bug
#ifndef _VMEM_H_
#define _VMEM_H_

#include "conf\conf_access.h"
#include "modules\control_access\ctrl_status.h"

//_____ D E F I N I T I O N S ______________________________________________

#define MEM_BASE_ADDRESS       0x4000

//! FAT specification
#define  ENTRY_SIZE           32    // size of entrie in byte

//! Value of virtual PBR
#define  VMEM_SECTOR_SIZE     512   // SECTOR = 512B
#define  VMEM_CLUSTER_SIZE    1     // Number of sector per cluster
#define  VMEM_RESERVED_SIZE   1     // Number of reserved sectors in the Reserved region of the volume starting at the first sector (FAT sector) of the volume 
                                    // It must be 1 for FAT 12 and 16 and 32 for FAT 32
#define  VMEM_NB_FATS         1     // number of FAT, it must be 2 for support all systems
#define  VMEM_NB_ROOT_ENTRY   16    // the count of 32 byte directory entries in the root directory (32*this value = must be a multiple of sector size)
                                    // It must be 512 for FAT 12 and 16 and 0 for FAT 32
#define  VMEM_MEDIA_TYPE      0xF8  // hard disk device (not removable=0xF8 or removable=0xF0), the standard value is 0xF8
#define  VMEM_SIZE_FAT        1     // number of sector per fat
// Physical structure of disk
#define  VMEM_SECT_PER_TRACK  1     // number of sector by track (after 8?)
#define  VMEM_NB_HEAD         1     // number of header

#define  VMEM_NB_HIDDEN_SECT  0     // This field should always be zero on media that not partitioned
#define  VMEN_DRIVE_NUMBER    0x80  // It must be 0x00 for floppy disk and 0x80 for hard disk


// First sector of the disk virtual (this disk isn't partitioned, only one partion)
#define  PBR_SECTOR           (0)
#define  FAT_SECTOR           (PBR_SECTOR  + VMEM_RESERVED_SIZE)
#define  ROOT_SECTOR          (FAT_SECTOR  + (VMEM_SIZE_FAT*VMEM_NB_FATS))
#define  FILE_SECTOR          (ROOT_SECTOR + ((ENTRY_SIZE*VMEM_NB_ROOT_ENTRY) / VMEM_SECTOR_SIZE))                   // 1(sector) = size root dir 

#define  FILE_SIZE            30     // Nb cluster disponible

// Computed the size of virtual mem
#define  VMEM_NB_SECTOR       ((FILE_SIZE * VMEM_CLUSTER_SIZE) + FILE_SECTOR)


#if (VMEM_NB_SECTOR > 0xFF)
#  error the size of the volume > 512B * 0xFF
#endif

#define vmem_write(offset,data_to_write)       (__api_wr_code_byte(offset, data_to_write))

#define _COMMAND_WR_CODE_BYTE		13
#define _COMMAND_WR_CODE_PAGE		13


//---- CONTROL FONCTIONS ----

Ctrl_status    virtual_test_unit_ready(void);
Ctrl_status    virtual_read_capacity( U32 _MEM_TYPE_SLOW_ *u32_nb_sector );
Bool           virtual_wr_protect(void);
Bool           virtual_removal(void);


//---- ACCESS DATA FONCTIONS ----

// Standard fonctions for open in read/write mode the device
Ctrl_status    virtual_read_10( U32 addr , U16 nb_sector );
Ctrl_status    virtual_write_10( U32 addr , U16 nb_sector );


//** If your device transfer have a specific transfer for USB (Particularity of Chejudo product, or bootloader)
   #include "lib_mcu\usb\usb_drv.h"    // In this case the driver must be know the USB access
   Ctrl_status virtual_usb_read( void );
   Ctrl_status virtual_usb_write( void );

Uchar __api_wr_code_byte (Uchar xdata* , Uchar);

Uchar __api_wr_code_page_fix (Uchar xdata* pt_code, Uchar xdata* pt_xram, Uchar nb_data);

#endif   // _VMEM_H_

