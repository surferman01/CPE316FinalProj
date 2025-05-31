//! @file nf_mem.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief  This file contains the interface routines of Nand Flash memory.
//!
//! @version 1.2 c5131-nRF24L01-demo-receiver-1_0_0 $Id: nf_mem.c,v 1.2 2005/03/24 12:10:42 lguilhau Exp $
//!
//! @todo
//! @bug

//_____  I N C L U D E S ___________________________________________________

#include "config.h"                         // system configuration
#include "conf\conf_nf.h"                   // Nand Flash configuration
#include "nf_mem.h"
#include "nf.h"



//_____ M A C R O S ________________________________________________________
//_____ P R I V A T E    D E C L A R A T I O N _____________________________


//_____ D E F I N I T I O N ________________________________________________


#define MEM_BASE_ADDRESS       0x0000
#define GL_BUF_SIZE           (256)

data    U32  gl_ptr_mem;             /* memory data pointer */
pdata   Byte    gl_buffer[GL_BUF_SIZE];

/* Disk management  */
bit     reserved_disk_space = FALSE;    /* reserved space for application on disk */
bit     flag = FALSE;


extern xdata U32 nf_mem_size;


void  nf_check_init( void );


//_____ D E C L A R A T I O N ______________________________________________


//! This fonction initialise the memroy
//!
void nf_check_init(void)
{
   if( !flag )
   {
      nf_init();
      nf_read_spare_byte();
      flag = TRUE;
   }
}


//! This fonction test the state of memory, and start the initialisation of the memory
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!
Ctrl_status nf_test_unit_ready(void)
{
   nf_check_init();
   return CTRL_GOOD;
}


//! This fonction returns the address of the last valid sector.
//!
//! @return *u16_nb_sector number of sector (sector = 512B)
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!
Ctrl_status nf_read_capacity( U32 _MEM_TYPE_SLOW_ *u32_nb_sector )
{
   nf_check_init();
   *u32_nb_sector = nf_mem_size;   //provisional
   return CTRL_GOOD;
}


//! This fonction return is the write protected mode
//!
//! Only used by memory removal with a HARDWARE SPECIFIC write protected detection
//! !!! The customer must be unplug the card for change this write protected mode.
//!
//! @return TRUE  -> the memory is protected
//!
Bool  nf_wr_protect(void)
{
   return FALSE;
}


//! This fonction inform about the memory type
//!
//! @return FASLE  -> The memory isn't removal
//!
Bool  nf_removal(void)
{
   return FALSE;
}



//------------ STANDARD FONCTION for open in read/write mode the device ---------------

//! This fonction initialise the memory for a read operation
//!
//! (sector = 512B)
//! @param addr         Sector address to start read
//! @param nb_sector    Number of sectors to transfer
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   A error occur  ->    CTRL_FAIL
//!
Ctrl_status nf_read_10( U32 addr , U16 nb_sector )
{

   nf_read_open(addr);
   nf_read_sector(nb_sector);
   nf_read_close();
   return CTRL_GOOD;
}


//! This fonction initialise the memory for a write operation
//!
//! (sector = 512B)
//! @param addr         Sector address to start write
//! @param nb_sector    Number of sectors to transfer
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   A error occur  ->    CTRL_FAIL
//!
Ctrl_status nf_write_10( U32 addr , U16 nb_sector )
{

   nf_write_open(addr);
   nf_write_sector(nb_sector);
   nf_write_close();
    return CTRL_GOOD;
}


//------------ SPECIFIC FONCTION USB TRANSFER -----------------------------------------
#if (USB_BOOST == ENABLED)

//** If your device transfer have a specific transfer for USB (Particularity of Chejudo product, or bootloader)
// !!! In this case the driver must be know the USB access

//! This fonction transfer the memory data (programed in scsi_read_10) directly to the usb interface
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!
Ctrl_status nf_usb_read()
{

   return CTRL_GOOD;
}


//! This fonction transfer the usb data (programed in scsi_write_10) directly to the memory data
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!
Ctrl_status nf_usb_write( void )
{

   return CTRL_GOOD;
}

#endif   // USB_BOOST == ENABLED


