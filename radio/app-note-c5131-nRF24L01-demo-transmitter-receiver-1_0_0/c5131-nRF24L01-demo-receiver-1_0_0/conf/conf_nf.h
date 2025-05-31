//! @file conf_nf.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the NAND Flash
//! This file will be given to any external customer
//!
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0 $Id: conf_nf.h,v 1.1 2006/05/04 12:34:16 cgasnier Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_NF_H_
#define _CONF_NF_H_

#define NF_CAPACITY_AUTO_DETECT   TRUE
#define NF_CONFIG                 NF_1X_512B    /* _1X_512B, _2X_512B, _1X_2KB or _2X_2KB */
#define NF_FULL_CHIP_ERASE        FALSE
#define MEM_RESERVED_SIZE         0             /* Reserved disk space in sector */

/* FAT Format Structure */
typedef struct  
{
  Uint16 nb_cylinder;
  Byte   nb_head;
  Byte   nb_sector;
  Byte   nb_hidden_sector;
  Byte   nb_sector_per_cluster;
} s_format;


// Nand Flash Block driver private defines
//
//! Define to 'TRUE' to have a generic driver (512B, 2KB) or to 'FALSE' if the driver must be compiled
//! in 512B or 2KB release (depending NF_SHIFT_PAGE_BYTE).
#define NF_GENERIC_DRIVER        FALSE
#define ERASING_ALL            DISABLE // ENABLE // erase the whole flash
#define NF_CACHE_LUT_LOG_SZ         32 // number of logical blocks cached from the LUT
//#define NF_CACHE_LUT_LOG_SZ          3 // (UT) number of logical blocks cached from the LUT
#define NF_CACHE_FBB_LOG_SZ         32 // number of logical blocks cached from the Free-blocks Block
//#define NF_CACHE_FBB_LOG_SZ          3 // (UT) number of logical blocks cached from the Free-blocks Block
#define NF_MNGT_DEVICE         ((NF_N_DEVICES)-1) // Number of the device which holds the blocks that manages the NFs
#define NF_N_FIRST_BLOCK             1 // Number of the first block that can be used for the mass-storage.
#define NF_PROTECTED_DEVICES         0 // bitfield of the protected devices

#define NF_N_SUBLUT_SIZE          1024 // Unit in physical blocks
#define DFC_EN                    TRUE // Use the DFC for the data transfer

#endif  //! _CONF_NF_H_


