//! @file virtual_mem.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief  This file contains the interface routines of virtual memory.
//!
//! @version 1.7 c5131-nRF24L01-demo-receiver-1_0_0 $Id: virtual_mem.c,v 1.7 2005/06/20 09:28:48 lguilhau Exp $
//!
//! @todo
//! @bug

//_____  I N C L U D E S ___________________________________________________

#include "config.h"                         // system configuration
#include "virtual_mem.h"


//_____ M A C R O S ________________________________________________________
//_____ P R I V A T E    D E C L A R A T I O N _____________________________
#if (VIRTUAL_MEM == ENABLE)

Uchar  data api_command     _at_ 0x1C;
Uchar  data api_value       _at_ 0x1D;
Uchar  data api_dpl         _at_ 0x1F;
Uchar  data api_dph         _at_ 0x1E;

 
#define MSK_AUXR1_ENBOOT    0x20
#define MAP_BOOT            AUXR1 |= MSK_AUXR1_ENBOOT;
#define UNMAP_BOOT          AUXR1 &= ~MSK_AUXR1_ENBOOT;

#define __API_FLASH_ENTRY_POINT    (*((const void(code*)(void)) 0xFFC0 ))
#define __API_FLASH_ENTRY_POINT_PE (*((const void(code*)(void)) 0xFFF3 ))


//_____ D E F I N I T I O N ________________________________________________

static code U8 vmem_data[ VMEM_SECTOR_SIZE * VMEM_NB_SECTOR ] At (MEM_BASE_ADDRESS);


static   _MEM_TYPE_SLOW_   U8   virtual_init=0;
static   _MEM_TYPE_SLOW_   U16  s_u16_addr;
static   _MEM_TYPE_SLOW_   U8   s_u8_nb_sector;
static                     Bool virtual_write_protection = FALSE;

U8 xdata    page_mask[128];
void  virtual_check_init( void );

#define   Is_virtual_write_protected()      (0 == P1_0)
#define   Is_virtual_not_write_protected()  (1 == P1_0)

//_____ D E C L A R A T I O N ______________________________________________
//! This fonction check the virtual memroy presence on P2_0
//!
Bool virtual_is_present(void)
{
//   return (1 == P1_0);
   return (TRUE);
}


//! This fonction check the virtual memroy change
//!
Bool virtual_check_change_state(void)
{
   // check 1, if the write protection have changed
   if( ((TRUE  == virtual_write_protection) && (Is_virtual_not_write_protected()))
   ||  ((FALSE == virtual_write_protection) && (Is_virtual_write_protected())) )
   {
      virtual_write_protection = Is_virtual_write_protected();
      return TRUE;
   }
   return FALSE;
}


//! This fonction initialise the memroy
//!
void virtual_check_init(void)
{
   U16 i;
   U8 xdata *addr;
   U8 xdata *addr_code;

   if ( virtual_init )
      return;
   virtual_init = 1;

if ((P3_2 == 0)||(0xEB != vmem_data[0]))
{
   // PBR sector init
   // Offset 0
       // init page_mask
   for (i = 0; i < 128 ; i++)  { page_mask[i] = 0; }

   for (i = MEM_BASE_ADDRESS; i < 0x7FFF; i += 128)
   {
      addr_code = i;
      __api_wr_code_page_fix (addr_code, page_mask, 128);
   }

   page_mask[ 0] = (U8)(0xEB);                     // JMP inst to PBR boot code
   page_mask[ 1] = (U8)(0x3C);
   page_mask[ 2] = (U8)(0x90);

   page_mask[ 3] = (U8)('A');                      // OEM name
   page_mask[ 4] = (U8)('T');
   page_mask[ 5] = (U8)('M');
   page_mask[ 6] = (U8)('E');
   page_mask[ 7] = (U8)('L');
   page_mask[ 8] = (U8)(' ');
   page_mask[ 9] = (U8)(' ');
   page_mask[10] = (U8)(' ');

   page_mask[11] = (U8)(VMEM_SECTOR_SIZE);         // number of bytes per sector
   page_mask[12] = (U8)(VMEM_SECTOR_SIZE >> 8);
   page_mask[13] = (U8)(VMEM_CLUSTER_SIZE);        // number of sector per cluster
   page_mask[14] = (U8)(VMEM_RESERVED_SIZE);       // number of reserved sector
   page_mask[15] = (U8)(VMEM_RESERVED_SIZE >> 8);
   page_mask[16] = (U8)(VMEM_NB_FATS);             // number of FAT
   page_mask[17] = (U8)(VMEM_NB_ROOT_ENTRY);       // number of root directory entries
   page_mask[18] = (U8)(VMEM_NB_ROOT_ENTRY >> 8);
   page_mask[19] = (U8)(VMEM_NB_SECTOR);
   page_mask[20] = (U8)(VMEM_NB_SECTOR >> 8);
   page_mask[21] = (U8)(VMEM_MEDIA_TYPE);          // media type
   page_mask[22] = (U8)(VMEM_SIZE_FAT);            // FAT size
   page_mask[23] = (U8)(VMEM_SIZE_FAT >> 8);
   page_mask[24] = (U8)(VMEM_SECT_PER_TRACK);      // Number of sectors on a track
   page_mask[25] = (U8)(VMEM_SECT_PER_TRACK >> 8);
   page_mask[26] = (U8)(VMEM_NB_HEAD);             // Number of heads
   page_mask[27] = (U8)(VMEM_NB_HEAD >> 8);
   page_mask[28] = (U8)(VMEM_NB_HIDDEN_SECT);      // Number of hidden sectors
   page_mask[29] = (U8)(VMEM_NB_HIDDEN_SECT >> 8);
   page_mask[30] = (U8)(VMEM_NB_HIDDEN_SECT >> 16);
   page_mask[31] = (U8)(VMEM_NB_HIDDEN_SECT >> 24);
   page_mask[32] = (U8)(0x00);                     // Number of sectors for FAT 32 only
   page_mask[33] = (U8)(0x00);
   page_mask[34] = (U8)(0x00);
   page_mask[35] = (U8)(0x00);
   page_mask[36] = (U8)(VMEN_DRIVE_NUMBER);        // Driver number 
   page_mask[37] = (U8)(0x00);                     // reserved (it must be 0x00, set to 0x01 by Windows after error)
   page_mask[38] = (U8)(0x29);                     // extended boot signature
   page_mask[39] = (U8)(0x00);                     // volume ID
   page_mask[40] = (U8)(0x00);
   page_mask[41] = (U8)(0x00);
   page_mask[42] = (U8)(0x00);

   page_mask[43] = (U8)('V');                // Volume Label
   page_mask[44] = (U8)('I');
   page_mask[45] = (U8)('R');
   page_mask[46] = (U8)('T');
   page_mask[47] = (U8)('U');
   page_mask[48] = (U8)('A');
   page_mask[49] = (U8)('L');
   page_mask[50] = (U8)(' ');
   page_mask[51] = (U8)('M');
   page_mask[52] = (U8)('E');
   page_mask[53] = (U8)('M');

   page_mask[54] = (U8)('F');                // File System Type in ASCII
   page_mask[55] = (U8)('A');
   page_mask[56] = (U8)('T');
   page_mask[57] = (U8)('1');
   page_mask[58] = (U8)('2');
   page_mask[59] = (U8)(' ');
   page_mask[60] = (U8)(' ');
   page_mask[61] = (U8)(' ');
   __api_wr_code_page_fix (MEM_BASE_ADDRESS, page_mask, 128);

   for (i = 0; i < 128 ; i++)  { page_mask[i] = 0; }

   page_mask[126] = (0x55);
   page_mask[127] = (0xAA);
   __api_wr_code_page_fix (MEM_BASE_ADDRESS + (512-128), page_mask, 128);

   // end of PBR

 
   for( i=0 ; i<VMEM_NB_FATS ; i++)
   {
      // FAT sector init
      addr  = (MEM_BASE_ADDRESS + VMEM_SECTOR_SIZE * (FAT_SECTOR+(i*VMEM_SIZE_FAT)))+0;
      vmem_write(addr , 0xF8);
      addr ++;
      vmem_write(addr , 0xFF);
      addr ++;
      vmem_write(addr , 0xFF);                  // other sectors set to 00
   }
}
}


//! This fonction test the state of memory, and start the initialisation of the memory
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur            ->    CTRL_FAIL
//!
Ctrl_status virtual_test_unit_ready(void)
{
   if ( !virtual_is_present() )
      return CTRL_NO_PRESENT;
   virtual_check_init();
   if( virtual_check_change_state() )
      return CTRL_BUSY;

   return CTRL_GOOD;
}


//! This fonction returns the address of the last valid sector.
//!
//! @return *u16_nb_sector number of sector (sector = 512B)
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur            ->    CTRL_FAIL
//!
Ctrl_status virtual_read_capacity( U32 _MEM_TYPE_SLOW_ *u32_nb_sector )
{
   if ( !virtual_is_present() )
      return CTRL_NO_PRESENT;
   virtual_check_init();
   if( virtual_check_change_state() )
      return CTRL_BUSY;

#if (VMEM_NB_SECTOR < 8)
   *u32_nb_sector = 8-1;
#else
   *u32_nb_sector = VMEM_NB_SECTOR-1;
#endif
   virtual_check_init();
   return CTRL_GOOD;
}


//! This fonction return is the write protected mode
//!
//! Only used by memory removal with a HARDWARE SPECIFIC write protected detection
//! !!! The customer must be unplug the card for change this write protected mode.
//!
//! @return TRUE  -> the memory is protected
//!
Bool  virtual_wr_protect(void)
{
   return virtual_write_protection;
}


//! This fonction inform about the memory type
//!
//! @return FASLE  -> The memory isn't removal
//!
Bool  virtual_removal(void)
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
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur  ->    CTRL_FAIL
//!
Ctrl_status virtual_read_10( U32 addr , U16 nb_sector )
{
   if ( !virtual_is_present() )
      return CTRL_NO_PRESENT;
   virtual_check_init();
   if ( virtual_check_change_state() )
      return CTRL_BUSY;

#if (VMEM_NB_SECTOR < 8)
   if ( 8              < (addr + nb_sector))
#else
   if ( VMEM_NB_SECTOR < (addr + nb_sector))
#endif
   {
      return CTRL_FAIL;
   }

   s_u16_addr = addr;
   s_u8_nb_sector = LSB(nb_sector);

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
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur  ->    CTRL_FAIL
//!
Ctrl_status virtual_write_10( U32 addr , U16 nb_sector )
{
   if (!virtual_is_present())         return CTRL_NO_PRESENT;

   virtual_check_init();

   if (virtual_check_change_state())  return CTRL_BUSY;

   if ( VMEM_NB_SECTOR < (addr + nb_sector))
   {
      return CTRL_FAIL;
   }
   
   s_u16_addr = addr;
   s_u8_nb_sector = LSB(nb_sector);

   return CTRL_GOOD;
}


//------------ SPECIFIC FONCTION USB TRANSFER -----------------------------------------

//! This fonction transfer the memory data (programed in scsi_read_10) directly to the usb interface
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur            ->    CTRL_FAIL
//!
Ctrl_status virtual_usb_read()
{
   U8    code *ptr_cram;
   U8    nb_64;
   U8    nb_read;

   while (s_u8_nb_sector != 0)
   {
      if ( !virtual_is_present() )        return CTRL_NO_PRESENT;

      if (s_u16_addr > (VMEM_NB_SECTOR-1))   // If overflow (possible with size virtual mem < 8 sectors)
         s_u16_addr = VMEM_NB_SECTOR-1;   // then read the last sector

      ptr_cram = &vmem_data[VMEM_SECTOR_SIZE * s_u16_addr];

      nb_64 = (VMEM_SECTOR_SIZE/64);
      while ( nb_64 )
      {
         // read 8x64B = 512B
         for(nb_read=0;nb_read<64;nb_read++)
         {
            Usb_write_byte(*ptr_cram++);
         }
         nb_64--;  
         Usb_send_in();
      }
      s_u16_addr++;         // new page
      s_u8_nb_sector--;
   }

   return CTRL_GOOD;
}


//! This fonction transfer the usb data (programed in scsi_write_10) directly to the memory data
//!
//! @return                Ctrl_status
//!   It is ready    ->    CTRL_GOOD
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur            ->    CTRL_FAIL
//!
Ctrl_status virtual_usb_write( void )
{
   U8    xdata *ptr_cram;
   U8    nb_64;
   U8    nb_read;

   while (s_u8_nb_sector != 0)
   {
      if ( !virtual_is_present() )
         return CTRL_NO_PRESENT;
      if ( virtual_check_change_state() )
         return CTRL_BUSY;

      ptr_cram = &vmem_data[VMEM_SECTOR_SIZE * s_u16_addr];

      nb_64 = (VMEM_SECTOR_SIZE/64);

      while ( nb_64 )
      {
         while(!Is_usb_receive_out());
         for(nb_read=0;nb_read<64;nb_read++)
         {
            page_mask[nb_read] = Usb_read_byte();
         }
         Usb_ack_receive_out_ms();
         while(!Is_usb_receive_out());
         for(nb_read=64;nb_read<128;nb_read++)
         {
            page_mask[nb_read] = Usb_read_byte();
         }
         nb_64 -= 2;
         Usb_ack_receive_out_ms();
         __api_wr_code_page_fix (ptr_cram, page_mask, 128);
         ptr_cram += 128;
      }
      s_u16_addr++;         // new page
      s_u8_nb_sector--;
   }

   return CTRL_GOOD;
}


/*F**************************************************************************
* NAME: __api_wr_code_byte 
*----------------------------------------------------------------------------
* PARAMS:
* Uint16 address : address to program
* Uchar value : data to write   
* return: 
* Uchar return : 
*       return  = 0x00 -> pass                            
*       return != 0x00 -> fail
*----------------------------------------------------------------------------
* PURPOSE: 
* This function allows to program data byte in Flash memory.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
* To use this function the constante __API_WR_CODE_BYTE must be define in 
* C header file flash_api.h.
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
Uchar __api_wr_code_byte (Uchar xdata * pt_address, Uchar value)
{
  bit ea_save;

  ea_save = EA;
  EA = 0;
  api_command = _COMMAND_WR_CODE_BYTE;
  FCON = 0x08;

  *pt_address = value;

  MAP_BOOT;
  __API_FLASH_ENTRY_POINT_PE();
  UNMAP_BOOT;
  EA = ea_save;        // restore interrupt state

  return(api_value);
}



/*F**************************************************************************
* NAME: __api_wr_code_page_fix 
*----------------------------------------------------------------------------
* PARAMS:
* Uint16 add_flash : address of the first byte to program in the Flash
* Uint16 add_xram  : address in XRAM of the first data to program
* Uchar nb_data : number of bytes to program
* return: 
* Uchar return : 
*       return = 0x00 -> pass                            
*       return != 0x00 -> fail
*----------------------------------------------------------------------------
* PURPOSE: 
* This function allows to program until 128 Datas in Flash memory.
* Number of bytes to program is limited such as the Flash write remains in a
* single 128 bytes page. 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
* To use this function the constante __API_WR_CODE_PAGE must be define in 
* C header file flash_api.h.
* This function used Dual Data Pointer DPTR0&1. At the end of this function 
* DPTR = DPTR0.
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
Uchar __api_wr_code_page_fix (Uchar xdata * pt_code, Uchar xdata * pt_xram, Uchar nb_data)
{
  bit ea_save;
  Uint16 add_xram; 
  data Uint16 add_code;


  add_xram = DPTR; // save the DPTR
  add_code = pt_code;
  ea_save = EA;
  EA = 0;
  add_xram = pt_xram;
  api_command =0x01;
  api_value   = nb_data;
  api_dpl = LOW(add_xram);
  api_dph = ((Uchar)((add_xram)>>8));
  AUXR1++;
//  add_xram = DPTR; // save the DPTR
//  DPTR = pt_code;
  DPTR = add_code;
  AUXR1++;
  MAP_BOOT;
  __API_FLASH_ENTRY_POINT();
  UNMAP_BOOT;

  AUXR1++;
  DPTR = add_xram; // restore the DPTR
  AUXR1++;
  EA = ea_save;    	// restore interrupt state
 
  return(api_value);
}
#endif // (VIRTUAL_MEM == ENABLE)

