//! @file ctrl_access.c,v
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
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0 $Id: ctrl_access.c,v 1.1 2005/03/10 08:29:48 lguilhau Exp $
//!
//! @todo
//! @bug

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "ctrl_access.h"


//_____ D E F I N I T I O N S ______________________________________________


//! Computed the maximum of static lun (don't add the lun of Mass Storage in mode USB Host)
// FYC: Memory = Logical Unit



               // CHECK FOR LUN DEFINE
#ifndef  LUN_0
   #  error LUN_0 must be defined with ENABLE or DISABLE in conf_access.h
#endif
#ifndef  LUN_1
   #  error LUN_1 must be defined with ENABLE or DISABLE in conf_access.h
#endif
#ifndef  LUN_2
   #  error LUN_2 must be defined with ENABLE or DISABLE in conf_access.h
#endif
#ifndef  LUN_3
   #  error LUN_3 must be defined with ENABLE or DISABLE in conf_access.h
#endif
#ifndef  LUN_4
   #  error LUN_4 must be defined with ENABLE or DISABLE in conf_access.h
#endif
#ifndef  LUN_5
   #  error LUN_5 must be defined with ENABLE or DISABLE in conf_access.h
#endif
#ifndef  LUN_6
   #  error LUN_6 must be defined with ENABLE or DISABLE in conf_access.h
#endif
#ifndef  LUN_7
   #  error LUN_7 must be defined with ENABLE or DISABLE in conf_access.h
#endif


#if (LUN_0 == ENABLE)
   #define     LUN_0_EN   1
   #else
   #define   LUN_0_EN   0
#endif
#if (LUN_1 == ENABLE)
   #define     LUN_1_EN   1
   #else
   #define     LUN_1_EN   0
#endif
#if (LUN_2 == ENABLE)
   #define     LUN_2_EN   1
   #else
   #define     LUN_2_EN   0
#endif
#if (LUN_3 == ENABLE)
   #define     LUN_3_EN   1
   #else
   #define     LUN_3_EN   0
#endif
#if (LUN_4 == ENABLE)
   #define     LUN_4_EN   1
   #else
   #define     LUN_4_EN   0
#endif
#if (LUN_5 == ENABLE)
   #define     LUN_5_EN   1
   #else
   #define     LUN_5_EN   0
#endif
#if (LUN_6 == ENABLE)
   #define     LUN_6_EN   1
   #else
   #define     LUN_6_EN   0
#endif
#if (LUN_7 == ENABLE)
   #define     LUN_7_EN   1
   #else
   #define     LUN_7_EN   0
#endif


#define  LUN_ID_0        (0)
#define  LUN_ID_1        (LUN_0_EN)
#define  LUN_ID_2        (LUN_0_EN+LUN_1_EN)
#define  LUN_ID_3        (LUN_0_EN+LUN_1_EN+LUN_2_EN)
#define  LUN_ID_4        (LUN_0_EN+LUN_1_EN+LUN_2_EN+LUN_3_EN)
#define  LUN_ID_5        (LUN_0_EN+LUN_1_EN+LUN_2_EN+LUN_3_EN+LUN_4_EN)
#define  LUN_ID_6        (LUN_0_EN+LUN_1_EN+LUN_2_EN+LUN_3_EN+LUN_4_EN+LUN_5_EN)
#define  LUN_ID_7        (LUN_0_EN+LUN_1_EN+LUN_2_EN+LUN_3_EN+LUN_4_EN+LUN_5_EN+LUN_6_EN)
#define  MAX_LUN         (LUN_0_EN+LUN_1_EN+LUN_2_EN+LUN_3_EN+LUN_4_EN+LUN_5_EN+LUN_6_EN+LUN_7_EN)

// Check configuration
#if (MAX_LUN == 0)
   #error No memory is active in conf_access.h
#endif

// Write protect variable
#if (GLOBAL_WR_PROTECT == ENABLED)
   static U8 g_u8_wr_protect;
#endif


//_____ D E F I N I T I O N S __ F O N C T I O N S _________________________


//! This fonction return the number of logical unit
//!
//! @return U8   number of logical unit in the system
//!
U8    get_nb_lun()
{
   return   MAX_LUN;
}


//! This fonction return the current logical unit
//!
//! @return U8   number of logical unit in the system
//!
U8    get_cur_lun()
{
   return   0; //TODO
}


//! This fonction test the state of memory, and start the initialisation of the memory
//!
//! MORE (see SPC-3 §5.2.4) : The TEST UNIT READY command allows an application client 
//! to poll a logical unit until it is ready without the need to allocate space for returned data.
//! The TEST UNIT READY command may be used to check the media status of logical units with removable media.
//!
//! @param lun        Logical unit number
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   Memory unplug  ->    CTRL_NO_PRESENT
//!   Not initialize ->    CTRL_BUSY
//!
Ctrl_status mem_test_unit_ready( U8 lun )
{
   switch( lun )
   {
#     if (LUN_0 == ENABLE)
      case LUN_ID_0:
         return Lun_0_test_unit_ready();
         break;
#     endif
#     if (LUN_1 == ENABLE)
      case LUN_ID_1:
         return Lun_1_test_unit_ready();
         break;
#     endif
#     if (LUN_2 == ENABLE)
      case LUN_ID_2:
         return Lun_2_test_unit_ready();
         break;
#     endif
#     if (LUN_3 == ENABLE)
      case LUN_ID_3:
         return Lun_3_test_unit_ready();
         break;
#     endif
#     if (LUN_4 == ENABLE)
      case LUN_ID_4:
         return Lun_4_test_unit_ready();
         break;
#     endif
#     if (LUN_5 == ENABLE)
      case LUN_ID_5:
         return Lun_5_test_unit_ready();
         break;
#     endif
#     if (LUN_6 == ENABLE)
      case LUN_ID_6:
         return Lun_6_test_unit_ready();
         break;
#     endif
#     if (LUN_7 == ENABLE)
      case LUN_ID_7:
         return Lun_7_test_unit_ready();
         break;
#     endif
   }
   return   CTRL_FAIL;
}

//! This fonction return the capacity of the memory
//!
//! @param lun        Logical unit number
//!
//! @return *u16_nb_sector number of sector (sector = 512B)
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   Memory unplug  ->    CTRL_NO_PRESENT
//!
Ctrl_status mem_read_capacity( U8 lun , U32 _MEM_TYPE_SLOW_ *u32_nb_sector )
{
   switch( lun )
   {
#     if (LUN_0 == ENABLE)
      case LUN_ID_0:
         return Lun_0_read_capacity( u32_nb_sector );
         break;
#     endif
#     if (LUN_1 == ENABLE)
      case LUN_ID_1:
         return Lun_1_read_capacity( u32_nb_sector );
         break;
#     endif
#     if (LUN_2 == ENABLE)
      case LUN_ID_2:
         return Lun_2_read_capacity( u32_nb_sector );
         break;
#     endif
#     if (LUN_3 == ENABLE)
      case LUN_ID_3:
         return Lun_3_read_capacity( u32_nb_sector );
         break;
#     endif
#     if (LUN_4 == ENABLE)
      case LUN_ID_4:
         return Lun_4_read_capacity( u32_nb_sector );
         break;
#     endif
#     if (LUN_5 == ENABLE)
      case LUN_ID_5:
         return Lun_5_read_capacity( u32_nb_sector );
         break;
#     endif
#     if (LUN_6 == ENABLE)
      case LUN_ID_6:
         return Lun_6_read_capacity( u32_nb_sector );
         break;
#     endif
#     if (LUN_7 == ENABLE)
      case LUN_ID_7:
         return Lun_7_read_capacity( u32_nb_sector );
         break;
#     endif
   }
   return   CTRL_FAIL;
}

//! This fonction return is the write protected mode
//!
//! @param lun        Logical unit number
//!
//! Only used by memory removal with a HARDWARE SPECIFIC write protected detection
//! !!! The customer must be unplug the card for change this write protected mode.
//!
//! @return TRUE  -> the memory is protected
//!
Bool  mem_wr_protect( U8 lun )
{
   switch( lun )
   {
#     if (LUN_0 == ENABLE)
      case LUN_ID_0:
         return Lun_0_wr_protect();
         break;
#     endif
#     if (LUN_1 == ENABLE)
      case LUN_ID_1:
         return Lun_1_wr_protect();
         break;
#     endif
#     if (LUN_2 == ENABLE)
      case LUN_ID_2:
         return Lun_2_wr_protect();
         break;
#     endif
#     if (LUN_3 == ENABLE)
      case LUN_ID_3:
         return Lun_3_wr_protect();
         break;
#     endif
#     if (LUN_4 == ENABLE)
      case LUN_ID_4:
         return Lun_4_wr_protect();
         break;
#     endif
#     if (LUN_5 == ENABLE)
      case LUN_ID_5:
         return Lun_5_wr_protect();
         break;
#     endif
#     if (LUN_6 == ENABLE)
      case LUN_ID_6:
         return Lun_6_wr_protect();
         break;
#     endif
#     if (LUN_7 == ENABLE)
      case LUN_ID_7:
         return Lun_7_wr_protect();
         break;
#     endif
   }
   return   CTRL_FAIL;
}


//! This fonction inform about the memory type
//!
//! @param lun        Logical unit number
//!
//! @return TRUE  -> The memory is removal
//!
Bool  mem_removal( U8 lun )
{
   switch( lun )
   {
#     if (LUN_0 == ENABLE)
      case LUN_ID_0:
         return Lun_0_removal();
         break;
#     endif
#     if (LUN_1 == ENABLE)
      case LUN_ID_1:
         return Lun_1_removal();
         break;
#     endif
#     if (LUN_2 == ENABLE)
      case LUN_ID_2:
         return Lun_2_removal();
         break;
#     endif
#     if (LUN_3 == ENABLE)
      case LUN_ID_3:
         return Lun_3_removal();
         break;
#     endif
#     if (LUN_4 == ENABLE)
      case LUN_ID_4:
         return Lun_4_removal();
         break;
#     endif
#     if (LUN_5 == ENABLE)
      case LUN_ID_5:
         return Lun_5_removal();
         break;
#     endif
#     if (LUN_6 == ENABLE)
      case LUN_ID_6:
         return Lun_6_removal();
         break;
#     endif
#     if (LUN_7 == ENABLE)
      case LUN_ID_7:
         return Lun_7_removal();
         break;
#     endif
   }
   return   CTRL_FAIL;
}


//************************************************************************************
//!----------- Listing of READ/WRITE interface with MODE ACCESS REGISTER -------------
//************************************************************************************


//! This fonction tranfer a data from memory to usb
//!
//! @param lun          Logical unit number
//! @param addr         Sector address to start read (sector = 512B)
//! @param nb_sector    Number of sectors to transfer
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   A error occur  ->    CTRL_FAIL
//!   Memory unplug  ->    CTRL_NO_PRESENT
//!
Ctrl_status    memory_2_usb( U8 lun , U32 addr , U16 nb_sector )
{
   Ctrl_status status;

   switch( lun )
   {
#     if (LUN_0 == ENABLE)
      case LUN_ID_0:
         status = Lun_0_read_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_0_usb_read();
         }
         break;
#     endif
#     if (LUN_1 == ENABLE)
      case LUN_ID_1:
         status = Lun_1_read_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_1_usb_read();
         }
         break;
#     endif
#     if (LUN_2 == ENABLE)
      case LUN_ID_2:
         status = Lun_2_read_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_2_usb_read();
         }
         break;
#     endif
#     if (LUN_3 == ENABLE)
      case LUN_ID_3:
         status = Lun_3_read_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_3_usb_read();
         }
         break;
#     endif
#     if (LUN_4 == ENABLE)
      case LUN_ID_4:
         status = Lun_4_read_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_4_usb_read();
         }
         break;
#     endif
#     if (LUN_5 == ENABLE)
      case LUN_ID_5:
         status = Lun_5_read_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_5_usb_read();
         }
         break;
#     endif
#     if (LUN_6 == ENABLE)
      case LUN_ID_6:
         status = Lun_6_read_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_6_usb_read();
         }
         break;
#     endif
#     if (LUN_7 == ENABLE)
      case LUN_ID_7:
         status = Lun_7_read_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_7_usb_read();
         }
         break;
#     endif
   }
   return   status;
}

//! This fonction trabsfer a data from usb to memory
//!
//! @param lun          Logical unit number
//! @param addr         Sector address to start write (sector = 512B)
//! @param nb_sector    Number of sectors to transfer
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   A error occur  ->    CTRL_FAIL
//!   Memory unplug  ->    CTRL_NO_PRESENT
//!
Ctrl_status    usb_2_memory( U8 lun , U32 addr , U16 nb_sector )
{
   Ctrl_status status;

   switch( lun )
   {
#     if (LUN_0 == ENABLE)
      case LUN_ID_0:
         status = Lun_0_write_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_0_usb_write();
         }
         break;
#     endif
#     if (LUN_1 == ENABLE)
      case LUN_ID_1:
         status = Lun_1_write_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_1_usb_write();
         }
         break;
#     endif
#     if (LUN_2 == ENABLE)
      case LUN_ID_2:
         status = Lun_2_write_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_2_usb_write();
         }
         break;
#     endif
#     if (LUN_3 == ENABLE)
      case LUN_ID_3:
         status = Lun_3_write_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_3_usb_write();
         }
         break;
#     endif
#     if (LUN_4 == ENABLE)
      case LUN_ID_4:
         status = Lun_4_write_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_4_usb_write();
         }
         break;
#     endif
#     if (LUN_5 == ENABLE)
      case LUN_ID_5:
         status = Lun_5_write_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_5_usb_write();
         }
         break;
#     endif
#     if (LUN_6 == ENABLE)
      case LUN_ID_6:
         status = Lun_6_write_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_6_usb_write();
         }
         break;
#     endif
#     if (LUN_7 == ENABLE)
      case LUN_ID_7:
         status = Lun_7_write_10(addr , nb_sector);
         if (CTRL_GOOD == status)
         {
            status = Lun_7_usb_write();
         }
         break;
#     endif
   }
   return   status;
}


