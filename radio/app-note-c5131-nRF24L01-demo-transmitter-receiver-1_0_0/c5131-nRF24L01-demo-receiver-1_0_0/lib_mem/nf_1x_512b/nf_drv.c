//! @file nf_drv.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief  This file contains the NF driver routines
//!
//! @version 1.2 c5131-nRF24L01-demo-receiver-1_0_0 $Id: nf_drv.c,v 1.2 2005/03/24 12:10:49 lguilhau Exp $
//!
//! @todo
//! @bug

/*_____ I N C L U D E S ____________________________________________________*/

#include "config.h"                         /* system configuration */
//#include FILE_BOARD_H                     /* board definition */
#include "board.h"                          /* board definition */
#include "nf_drv.h"                         /* nf driver definition  */


/*_____ M A C R O S ________________________________________________________*/

#ifndef NF_CAPACITY_AUTO_DETECT
  #error  NF_CAPACITY_AUTO_DETECT must be defined in board.h
#endif

#if NF_CONFIG != NF_1X_512B
  #error NF_CONFIG must be defined with NF_1X_512B in config.h
#endif

#ifndef NF_FULL_CHIP_ERASE
  #error NF_FULL_CHIP_ERASE  must be set to TRUE or FALSE in config.h
#endif

#ifndef MEM_RESERVED_SIZE
  #error MEM_RESERVED_SIZE must be defined in config.h
#endif

/*_____ D E F I N I T I O N ________________________________________________*/


/*_____ D E C L A R A T I O N ______________________________________________*/

extern  pdata Byte gl_buffer[];                       /* global buffer of 256 bytes */
extern  xdata Byte nf_send_cmd;                       /* Command */
extern  xdata Byte nf_send_add;                       /* Address */
extern  xdata Byte volatile nf_data;                  /* Data */

extern  data  Uint32  gl_ptr_mem;                     /* memory data pointer */
extern  data  Uint32 gl_address;                      /* general address variable */
extern  data  Uint16 nf_look_up_table_block;          /* Look up table block address */
extern  data  Byte gl_buf_free_idx ;
extern  data  Byte nf_gl_buf_idx;
extern  data  Uint32 nf_current_physical_sector_addr; /* give the address of the current sector */ 
extern  data  Byte nf_zone;


        idata Uint16 nf_redundant_logical_block_value;/* Logical block value in the redundant area  */
extern  idata Byte nf_gl_buf_idx_max;
extern  idata Uint16 nf_logical_block;

extern  xdata Byte nf_spare_block_number[NF_ZONE_MAX];/* Number of free spare block in each zone */
extern  xdata Byte    nf_lut_index[NF_ZONE_MAX];
extern  xdata Uint16  nf_buf_free[24];
extern  xdata Uint16  nf_block_to_be_deleted;
extern  xdata Uint16  nf_block_min;
extern  xdata Uint16  nf_lut_block[NF_ZONE_MAX];       /* LUT address in block value */
extern  xdata Union16 nf_buf[NF_BUFFER_SIZE];         /* Buffer for write operation: contains physical block address */
extern  xdata Byte    nf_spare_block;
extern  xdata Uint32  nf_reserved_space_start;

#if (NF_CAPACITY_AUTO_DETECT == TRUE)                         /* If autodetect capacity nand flash is active */
  extern  xdata Byte    nf_zone_max;                          /* nf_zone_max definition   */
  extern  xdata Byte    nf_device_type;                       /* nf_device_type definition */
  extern  bdata bit     nf_4_cycle_address;                   /* nf_4_cycle_address definition */
#endif


/*F**************************************************************************
* NAME: nf_check_status
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*   NF status
*----------------------------------------------------------------------------
* PURPOSE:
*   Check the status of the device after a program or an erase operation
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:

*****************************************************************************/
bit nf_check_status (void)
{
  Nf_wait_busy();
  /* Status Type Command */
  Nf_send_command(NF_READ_STATUS_CMD);

  if ( (Nf_rd_byte() & 0x01) == 0x00)
  {  
    return OK;
  }
  else
  {
    return KO;
  }
}  

/*F**************************************************************************
* NAME: nf_calc_logical_block
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Calculate the logical block value (used in spare data area)
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
void nf_calc_logical_block (void)
{
volatile Byte dummy;
bdata bit parity_bit;

  nf_redundant_logical_block_value = (nf_logical_block << 1) + 0x1000;
  /* Parity bit calculation */
  dummy = nf_redundant_logical_block_value;
  parity_bit = P;
  dummy = ((Byte*)&nf_redundant_logical_block_value)[0];
  if (P) parity_bit = ~parity_bit;

  if (parity_bit)
    nf_redundant_logical_block_value++;
}


/*F**************************************************************************
* NAME: nf_update_spare_data
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Update the value of the logical block on the spare data area
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
void nf_update_spare_data (void)
{
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
  Nf_wr_byte(nf_redundant_logical_block_value >> 8);
  Nf_wr_byte(nf_redundant_logical_block_value );
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
  Nf_wr_byte(nf_redundant_logical_block_value >> 8 );
  Nf_wr_byte(nf_redundant_logical_block_value );
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
  Nf_wr_byte(0xFF);
}


/*F**************************************************************************
* NAME: nf_init_spare
*----------------------------------------------------------------------------
* PARAMS:   
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Initialize logical value in redundant data when a block is opened for 
*   the first time and the sector inside the block is not 0.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
void nf_init_spare (void)
{
Byte j;

  for (j = (((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK); j != 0 ; j--)
  {              
    Nf_wait_busy();
    /* Write open on spare data area */
    Nf_write_open_C_area(nf_current_physical_sector_addr, 0x00);
    /* Update spare data */
    nf_update_spare_data();

    /* Send program command */ 
    Nf_send_command (NF_PAGE_PROGRAM_CMD);
    ((Byte*)&nf_current_physical_sector_addr)[3]++;
  }
}


/*F**************************************************************************
* NAME: nf_copy_block_head
*----------------------------------------------------------------------------
* PARAMS:
*   block : physical block number
*   nb_sector : number of sector to be copied
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Copy the first part of a block that is not modified during a write
*   operation
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
void nf_copy_block_head (void)
{
Byte j;

  /* address of the source physical block */
  gl_address = (Uint32)(nf_block_to_be_deleted) << 5;  

  for (j = (((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK); j != 0; j--)
  { 
    /* 1st half page */
    Nf_wait_busy();/* Read Open */
    Nf_read_open_A_area(gl_address, 0x00);    
    nf_upload_buffer();
    Nf_write_open_A_area(nf_current_physical_sector_addr, 0x00);    /* Write open */
    nf_download_buffer();
    Nf_send_command (NF_PAGE_PROGRAM_CMD);
    Nf_wait_busy();

    /* 2nd half page */
    Nf_read_open_B_area(gl_address, 0x00);    /* Read Open */
    nf_upload_buffer();    
    Nf_write_open_B_area(nf_current_physical_sector_addr, 0x00);    /* Write open */
    nf_download_buffer();
    /* Update spare data */
    nf_update_spare_data();  

    Nf_send_command (NF_PAGE_PROGRAM_CMD);

    /* increase counter */
    ((Byte*)&nf_current_physical_sector_addr)[3]++;
    ((Byte*)&gl_address)[3]++;
  }
}


/*F**************************************************************************
* NAME: nf_copy_block_tail
*----------------------------------------------------------------------------
* PARAMS:
*   block : physical block number
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Copy the last part of a block that is not modifiedd uring a write
*   operation
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
void nf_copy_block_tail (void)
{
Byte j;

  /* Address of the source block */
  gl_address = ((Uint32)(nf_block_to_be_deleted) << 5) +  (((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK);

  for (j = (((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK); j < NF_PAGE_PER_BLOCK; j++)
  {
    /* 1st half page */
    Nf_wait_busy();
    Nf_read_open_A_area(gl_address, 0x00);    /* Read Open  */
    nf_upload_buffer();
    Nf_write_open_A_area(nf_current_physical_sector_addr, 0x00);    /* Write open  */
    nf_download_buffer();

    Nf_send_command (NF_PAGE_PROGRAM_CMD);
    Nf_wait_busy();

    /* 2nd half page */
    Nf_read_open_B_area(gl_address, 0x00);    /* Read Open  */
    nf_upload_buffer();
    Nf_write_open_B_area(nf_current_physical_sector_addr, 0x00);    /* Write open  */
    nf_download_buffer();

    nf_update_spare_data();    /* Update spare data */
    
    Nf_send_command (NF_PAGE_PROGRAM_CMD);

    ((Byte*)&nf_current_physical_sector_addr)[3]++;
    ((Byte*)&gl_address)[3]++;
  }
}


/*F**************************************************************************
* NAME: nf_reassign_block
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Reassign the block value in LUT (copy of LUT)
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
void nf_reassign_block (void)
{
Byte j, k;
Uint16 start;
Uint16 block_start;                     
xdata Uint32 address2;                            /* LUT Source address */

  if (nf_lut_index[nf_zone] == 7)
  {
    /* Source address */
    address2 = ((Uint32)(nf_look_up_table_block)<<5) + 28;

    nf_look_up_table_block =  nf_buf_free[gl_buf_free_idx] & 0x7FFF;
    nf_buf_free[gl_buf_free_idx++] = nf_lut_block[nf_zone] | 0x8000;

    if (gl_buf_free_idx >= nf_spare_block)
    {
      gl_buf_free_idx = 0;
    }      

    /* Target address : recopy the last 2Kb */
    gl_address = ((Uint32)(nf_look_up_table_block) << 5);

    nf_lut_index[nf_zone] = 0;

  }
  else
  {
    /* Source address */
    address2 = ((Uint32)(nf_look_up_table_block)<<5) + ((Uint32)(nf_lut_index[nf_zone])<<2);

    nf_lut_index[nf_zone]++;

    /* Target address */
    gl_address = address2 + 4;
  }

  j = 0;
  k = 0;
  start = 0;
  block_start = nf_block_min;

  do
  {
    Nf_wait_busy();
    Nf_read_open_A_area(address2, 0x00);      /* Open look-up table in read mode */
    nf_upload_buffer();

    while ((k <= nf_gl_buf_idx_max) && (start <= block_start) && ((start + 128) > block_start))
    {
      gl_buffer[(2 * (block_start & 0x7F))] = nf_buf[k].b[0];
      gl_buffer[(2 * (block_start & 0x7F))+1] = nf_buf[k].b[1];
      k++;
      block_start++;
    }

    Nf_write_open_A_area(gl_address, 0x00);
    nf_download_buffer();                     /* Write 256 bytes from the buffer */

    Nf_send_command(NF_PAGE_PROGRAM_CMD);     /* Valid the page programmation */

    start += 128;

    Nf_wait_busy();                           /* Wait for R/B signal */
    Nf_read_open_B_area(address2, 0x00);      /* Open look-up table in read mode */
    nf_upload_buffer();

    while ((k <= nf_gl_buf_idx_max) && (start <= block_start) && ((start + 128) > block_start))
    {
      gl_buffer[(2 * (block_start & 0x7F))] = nf_buf[k].b[0];
      gl_buffer[(2 * (block_start & 0x7F))+1] = nf_buf[k].b[1];
      k++;
      block_start++;
    }   
 
    if (start >= 896)                       /* This part update the free physical block table */
    {
      k = 208;
      for (j = 0; j <= 23; j++)
      {           
        gl_buffer[k] = nf_buf_free[j]>>8;
        gl_buffer[k+1] = nf_buf_free[j];  
        k += 2;
      }
    }

    Nf_write_open_B_area(gl_address, 0x00);
    nf_download_buffer();                   /* Write 256 bytes from the buffer */

    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xE8);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xE8);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);

    /* Valid the page programmation */
    Nf_send_command(NF_PAGE_PROGRAM_CMD);

    start += 128;
    ((Byte*)&address2)[3]++;
    ((Byte*)&gl_address)[3]++;
  }
  while (start < 1024);

  nf_gl_buf_idx_max = 0;

  if (nf_lut_index[nf_zone] == 0)
  {
    address2 = (Uint32)(nf_lut_block[nf_zone])<<5;
    nf_lut_block[nf_zone] = nf_look_up_table_block;           /* Update address of look up table */
    Nf_wait_busy();
    Nf_send_command (NF_READ_A_AREA_CMD);
    Nf_send_command (NF_BLOCK_ERASE_CMD);                     /* Auto Block Erase Setup */
    Nf_send_address ( ((Byte*)&address2)[3] );                /* 2nd address cycle*/
    Nf_send_address ( ((Byte*)&address2)[2] );                /* 3rd address cycle*/
    if (NF_4_CYCLE_ADDRESS_BIT)                               /* Size of card >= 64Mbytes ?*/
      Nf_send_address ( ((Byte*)&address2)[1] );              /* 4th address cycle*/
    Nf_send_command(NF_BLOCK_ERASE_CONFIRM_CMD);              /* Erase command */
  }
}


/*F**************************************************************************
* NAME: nf_mark_bad_block
*----------------------------------------------------------------------------
* PARAMS:   
*           
* return:   
*----------------------------------------------------------------------------
* PURPOSE:
*   Write 0x00 on block status byte (Byte 5 of spare data)
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*   This function use the global variable Uint32 address
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
void nf_mark_bad_block (void)
{
  Nf_wait_busy();
  Nf_write_open_C_area(gl_address, 0x03);

  Nf_wr_byte(0x00);
  Nf_wr_byte(0x00);
  Nf_wr_byte(0x00);

  Nf_send_command (NF_PAGE_PROGRAM_CMD);  /* Send program command */
}


/*F**************************************************************************
* NAME: nf_block_erase
*----------------------------------------------------------------------------
* PARAMS:   
*
* return:
*   OK : erase done
*   KO : erase not done
*----------------------------------------------------------------------------
* PURPOSE: Erase a block on Nand Flash Media
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*   This function use the global variable Uint32 address
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
bit nf_block_erase (Uint32 pos)
{
  Nf_wait_busy();
  Nf_send_command (NF_READ_A_AREA_CMD);
  Nf_send_command (NF_BLOCK_ERASE_CMD);       /* Auto Block Erase Setup     */

  Nf_send_address ( ((Byte*)&pos)[3] );       /* 2nd address cycle          */
  Nf_send_address ( ((Byte*)&pos)[2] );       /* 3rd address cycle          */
  if (NF_4_CYCLE_ADDRESS_BIT)                 /* Size of card >= 64Mbytes ? */
    Nf_send_address ( ((Byte*)&pos)[1] );     /* 4th address cycle          */

  Nf_send_command(NF_BLOCK_ERASE_CONFIRM_CMD);/* Erase command              */
  return OK;
}


/*F**************************************************************************
* NAME: nf_erase_all_block
*----------------------------------------------------------------------------
* PARAMS:   
*           
* return:   
*   OK : erase done
*   KO : erase not done
*----------------------------------------------------------------------------
* PURPOSE:
*   This function erase all blocks on a NF card and write CIS information
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*   This function use the global variable Uint32 address
*----------------------------------------------------------------------------
* REQUIREMENTS: 
* ram/xram:
* cycle:
* stack: 
* code:
*****************************************************************************/
bit nf_erase_all_block (void)
{
Byte i;
Uint16 j;
Uint16 block;
Uint16 block_reserved_space;
bit erase;
 
  Nf_CS_ON();
  /* Erase all block */
  block_reserved_space = (Uint16) (((nf_reserved_space_start >> 5) % 1000));
  gl_address = 0;
  for (nf_zone = 0; nf_zone < NF_ZONE_MAX_CPT; nf_zone++)
  {
    for (j = 1024; j != 0; j--)
    {
      Nf_wait_busy();
      Nf_read_open_C_area(gl_address, 0x05); /* Read block status byte */
      erase = TRUE;
      if (Nf_rd_byte() != 0xFF) /* if bad block */
      {
        erase = FALSE;          /* don't erase block */
      }
      else
      {
        ((Byte*)&block)[0] = Nf_rd_byte(); /* Read logical block address */
        ((Byte*)&block)[1] = Nf_rd_byte();
        if ( (MEM_RESERVED_SIZE != 0) && (nf_zone == (NF_ZONE_MAX_CPT - 1)))
        {
          if ((((Byte*)&block)[0] & 0xF8) == 0x10)
          {
            block = (block & 0x0FFF) >> 1;
            if (block >= block_reserved_space)      /* If it is a reserved block */
            {
              erase = FALSE;                        /* Don't erase block */
            }
          }
        }
      }

      if (NF_FULL_CHIP_ERASE == FALSE)
      {
        if (((Byte*)&block)[0] != 0xE8)
        {
          erase = FALSE;
        }
      }
      else
      {
        if (((Byte*)&block)[0] == 0x00)
        {
          erase = FALSE;
        }
      }

      if (erase)
      {
        nf_block_erase(gl_address);
        if (nf_check_status() == KO)
        { 
          nf_mark_bad_block(); /* Failure on erase operation */     
        }
        else
        { /* Fill redundant area with 0x00 */
          Nf_write_open_C_area(gl_address, 0x00);
          for (i = 16; i != 0; i--)
            Nf_wr_byte(0x00);
          Nf_send_command(NF_PAGE_PROGRAM_CMD);        /* Valid the page programmation */
          if ( nf_check_status() == KO)
          { 
            nf_mark_bad_block();                        /* Failure on program operation */
          }
          else
          {
            Nf_read_open_C_area(gl_address, 0x00);        /* Read 16 bytes */
            i = 16;
            while ((i != 0) && (Nf_rd_byte() == 0x00)) i--;

            if (i)
            {
              nf_mark_bad_block();
            }
            else
            {
              nf_block_erase(gl_address);          /* Finally, erase the block */      
              if ( nf_check_status() == KO)
              { /* Failure on erase operation */
                nf_mark_bad_block();      
              }
            }
          }
        }
      }
      gl_address += 32;
    }
  }
  return (nf_read_spare_byte());
}




