//! @file nf.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief  This file contains the high level NF routines
//!
//! @version 1.2 c5131-nRF24L01-demo-receiver-1_0_0 $Id: nf.c,v 1.2 2005/03/24 12:10:49 lguilhau Exp $
//!
//! @todo
//! @bug


/*_____ I N C L U D E S ____________________________________________________*/


#include "config.h"                           /* system configuration */
#include "board.h"                            /* board definition  */
#include "lib_mcu\usb\usb_drv.h"              /* usb driver definition */
#include "nf.h"                               /* NF definition */


/*_____ M A C R O S ________________________________________________________*/

#ifndef NF_CAPACITY_AUTO_DETECT
  #error  NF_CAPACITY_AUTO_DETECT must be defined in conf_nf.h
#endif

#if NF_CONFIG != NF_1X_512B
  #error NF_CONFIG must be defined with NF_1X_512B in conf_nf.h
#endif

#ifndef NF_FULL_CHIP_ERASE
  #error NF_FULL_CHIP_ERASE  must be set to TRUE or FALSE in conf_nf.h
#endif

#ifndef MEM_RESERVED_SIZE
  #error MEM_RESERVED_SIZE must be defined in conf_nf.h
#endif

/*_____ D E F I N I T I O N ________________________________________________*/

xdata Byte nf_send_cmd At(NF_CMD_LATCH_ENABLE_ADD); /* Command */
xdata Byte nf_send_add At(NF_ADD_LATCH_ENABLE_ADD); /* Address */
xdata Byte volatile nf_data At(NF_ADDRESS_CMD_DATA);/* Data    */


/*_____ D E C L A R A T I O N ______________________________________________*/

extern  data    Uint32  gl_ptr_mem;         /* memory data pointer */
extern  pdata   Byte gl_buffer[];
extern          bit reserved_disk_space;

data  Uint32  nf_current_physical_sector_addr; /* address of current phys. sect. */  
data  Uint32  gl_address;                      /* general address variable */
data  Uint16  nf_look_up_table_block;          /* lut address (block) */
data  Byte    nf_zone;                         /* current zone number */
data  Byte    nf_gl_buf_idx;                   /* idx for cache */
data  Byte    gl_buf_free_idx ;                /* idx for cache for free blocks */
data  Uint16  gl_cpt_page;                     /* internal page counter */

bdata bit     nf_block_used;                    /* set when a block was used */
bdata bit     nf_busy;                          /* nand flash busy flag */
bdata bit     nf_lut_modified;                  /* LUT modified flag */
bdata bit     nf_reserved_space = FALSE;

idata Uint16  nf_logical_block;                /* current logical block */
idata Byte    nf_gl_buf_idx_max;               /* max index for updating lut */

xdata Byte    nf_spare_block;                  /* number of free block */
xdata Uint16  nf_lut_block[NF_ZONE_MAX];       /* lut address / zone (block) */
xdata Uint16  nf_buf_free[24];                 /* contain free physical block address */
xdata Uint16  nf_block_to_be_deleted;          /* give the address of the next deleted block */
xdata Uint16  nf_block_min;                    /* first address block in the buffer */
xdata Uint16  nf_block_max;                    /* last address block in the buffer */
xdata Union16 nf_buf[NF_BUFFER_SIZE];          /* cache for write operation */
xdata Byte    nf_old_zone;                     /* for zone change */  
xdata Byte    nf_lut_index[NF_ZONE_MAX];       /* give the index for updating LUT */
xdata Byte nf_spare_block_number[NF_ZONE_MAX]; /* number of free spare block in each zone */

extern idata Uint16 nf_redundant_logical_block_value;/* Logical block value in the redundant area  */

#if (NF_CAPACITY_AUTO_DETECT == TRUE)           /* If autodetect capacity nand flash is active */
  xdata Byte    nf_zone_max;                    /* nf_zone_max definition   */
  xdata Byte    nf_device_type;                 /* nf_device_type definition */
  bdata bit     nf_4_cycle_address;             /* nf_4_cycle_address definition */
#endif


xdata Uint32 nf_mem_size;
xdata Uint32 nf_reserved_space_start;
/*F**************************************************************************
* NAME: nf_read_spare_byte
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*   OK : LUT construction complete
*   KO : pb for LUT
*----------------------------------------------------------------------------
* PURPOSE:
*   read spare data byte and construct the look up table.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
bit nf_read_spare_byte(void)
{
Byte byte_5;
Byte byte_6;
Byte lut_is_present;
Uint16 block;
Uint16 j;                       /* counter */
Union16 block2;
bit   block_valid;
xdata Uint16 last_physical_used_block[NF_ZONE_MAX]; /* Last physical block used */

#define half_page block_valid
#define temp_address gl_ptr_mem
#define i nf_zone
#define start gl_cpt_page
#define end nf_look_up_table_block
#define free_bloc_pos nf_logical_block

  Nf_CS_ON();
  Nf_wait_busy();
  /* Global media initialization */
  nf_block_min = 0xFFFF;                    /* Starting buffer value */
  nf_gl_buf_idx_max = 0;                    /* Max index in the buffer */
  nf_lut_modified = FALSE;                  /* Buffer change flag */
  nf_gl_buf_idx = 0;                        /* Main buffer index */
  gl_buf_free_idx = 0;                      /* Free physical buffer idx */
  nf_old_zone = 0xFF;                       /* Previous zone number */
  for (i = 0; i < NF_ZONE_MAX_CPT; i++)
  {
    nf_lut_index[i] = 0;                    /* LUT index */
  }
  lut_is_present = 0x00;

  /***************************************************/
  /* For each zone, this following lines :           */
  /*   Search bad block                              */
  /*   Determine if a LUT is present                 */
  /*   Determine the last physical block             */ 
  /***************************************************/
  gl_address = 0;                           /* Start from physical sector 0 */
  for (i = 0; i < NF_ZONE_MAX_CPT; i++)         /* for each zone */

  {
    nf_spare_block_number[i] = 23;          /* Initialize the spare block table */
    last_physical_used_block[i] = (Uint16)(i) << 10;/* Initialize the last used physical block value */

    for (j = NF_BLOCK_PER_ZONE; j != 0; j--)/* for each block */ 
    {
      Nf_read_open_C_area(gl_address, 0x05);
      if ( Nf_rd_byte() != 0xFF )           /* block status data : valid/invalid block */
      {
        nf_spare_block_number[i]--;         /* Defect block */
      }
      else                                  /* Block is valid */
      {
                                            /* Determine if the block is a specific block */
        byte_6 = Nf_rd_byte();
        if ((byte_6 == 0x00) ||                                /* 0x00 = specific block */
            ((byte_6 != 0xFF) && ((byte_6 & 0xF8) != 0x10) && /* Value in redundant spare area not correct */
            (byte_6 != 0xE8))                /* Don't care about LUT block */
           )
        { 
          nf_spare_block_number[i]--;        /* specific or invalid block */
          if (nf_spare_block_number[i] < 4)
            return KO;
        }
        else
        {
          /* Determine if the block is the look up table */ 
          if (byte_6 == 0xE8)                 /* look up table ? */
          {
              lut_is_present |= (0x01) << i;
              nf_lut_block[i] = gl_address >> 5;
 
          }
          /* Determine the last physical used block */
          if ((byte_6 & 0xF8) == 0x10)         /* Used block */
          {
            last_physical_used_block[i] = gl_address >> 5;
          }
        }
      }
      gl_address += 32;
    }
  }


  for (i = 0; i < NF_ZONE_MAX_CPT; i++)
  { /* Determine the index for each correct lut */
    if ((lut_is_present & (0x01 << i)))
    {
      gl_address = (Uint32)(nf_lut_block[i]) << 5;
      do
      {
        Nf_wait_busy();
        Nf_read_open_C_area(gl_address, 0x06);
        nf_lut_index[i]++;
        gl_address+=4;
      }
      while (Nf_rd_byte() == 0xE8);
      nf_lut_index[i]-=2;
      gl_address = gl_address - 8;
      Nf_read_open_C_area(gl_address, 0x00);
      if (Nf_rd_byte() != 0xFF)             /* LUT has been modified */
      {
        nf_block_erase(gl_address);
        lut_is_present &= ~((0x01) << i);   /* Reset bit */
        nf_lut_index[i] = 0;
      }
    }
  }
  /*****************************************************************/
  /*          Find free physical block for LUT for each zone       */
  /*****************************************************************/
  for (i = 0; i < NF_ZONE_MAX_CPT; i++)
  {
    if (!(lut_is_present & (0x01 << i)))
    {
      block = last_physical_used_block[i];
      start = (Uint16)(i) << 10;            /* starting value for each zone */
      j = 0;
      block_valid = FALSE;                  /* init flag block valid */
      Nf_wait_busy();
      do
      {
        gl_address = (Uint32)(block) << 5;
        Nf_read_open_C_area(gl_address, 0x05);
        byte_5 = Nf_rd_byte();
        byte_6 = Nf_rd_byte();
  
        if ((byte_5 == 0xFF) && (byte_6 == 0xFF)) /* not assigned and valid block */
        {
          block_valid = TRUE;               /* find a correct block */
        }
        else                                /* else decrease block number */
        {
          if (block == start)                       
          {
            block = start + 1023;
          }
          else
          {
            block--;
          }
        }
        j++;
      }
      while ((!block_valid ) && (j < 1024));
      
      // if (j == 1024) and (block_valid=TRUE), then the block valid is the last of listing
      // if (j == 1024) and (block_valid=FALSE), then used the last block of listing

      // Deleted this block
      gl_address = (Uint32)(block) << 5;
      nf_block_erase(gl_address);
  
      nf_lut_block[i] = block;              /* Update look up table address */
    }
  }

  /**********************************************************/
  /* Create the look-up table                               */
  /* Process is done zone per zone and by step of 256 blocks*/
  /* ********************************************************/
  for (nf_zone = 0; nf_zone < NF_ZONE_MAX_CPT; nf_zone++)   /* for each zone */
  {
    if (!(lut_is_present & (0x01 << nf_zone)))  /* If not LUT */
    {
      half_page = 0;
      start = 0x00;
      end   = 0x80;     /* 256 bytes for gl_buffer <-> 128 blocks */                            
      free_bloc_pos = (Uint16)(last_physical_used_block[nf_zone]);
      gl_address    = (Uint32)(nf_lut_block[nf_zone]) << 5;        
      do
      {
        nf_init_buffer();                         /* Reinitialize the buffer */
        temp_address = (Uint32)(nf_zone) << 15;   /* We start at the beginning */
        block        = (Uint16)(nf_zone) << 10;
        Nf_wait_busy();
        for (j = NF_BLOCK_PER_ZONE; j != 0 ; j--) /* for each block */
        { 
          Nf_read_open_C_area(temp_address, 0x05);
          byte_5 = Nf_rd_byte();            /* Block status byte */
          if (byte_5 == 0xFF)               /* If not a bad block */
          {
            block2.b[0] = Nf_rd_byte();     /* Read logical block address */
            block2.b[1] = Nf_rd_byte();
            if ((block2.b[0] & 0xF8) == 0x10)
            {
              
              block2.w = (block2.w & 0x0FFF) >> 1;          
              if ((block2.w < end) && (block2.w >= start)) 
              { /* Save logical block value in the buffer */
                if ( (0xFF != gl_buffer[2 * block2.b[1]])
                ||   (0xFF != gl_buffer[2 * block2.b[1] + 1]) )
                {
                  // Logical block duplicated, it isn't a normal state
                  // Read last page of this block
                  temp_address += 31;
                  Nf_wait_busy();
                  Nf_read_open_C_area(temp_address, 0x06);

                  // If this block is full (complete and correct)
                  if ((Nf_rd_byte() & 0xF8) == 0x10)
                  {
                     // Delete the previous block
                     temp_address = (((Uint32)gl_buffer[2 * block2.b[1]]<<8) + (Uint32)gl_buffer[2 * block2.b[1] + 1])<<5;
                  } else {
                     // Delete this block
                     temp_address-=31;
                  }
                  nf_block_erase(temp_address);
                  // Restart the creation of LUT
                  break;
                }
                gl_buffer[2 * block2.b[1]] = block >> 8;
                gl_buffer[2 * block2.b[1] + 1] = block;
              }
            }
          }
          temp_address += 32; 
          block++;
        }

        // If the creation LUT is stopped, then restart
        if (0 != j)
        {   
          half_page = 0;
          start = 0x00;
          end   = 0x80;     /* 256 bytes for gl_buffer <-> 128 blocks */                            
          free_bloc_pos = (Uint16)(last_physical_used_block[nf_zone]);
          // Delete the LUT block
          nf_block_erase( gl_address );
          gl_address    =  (Uint32)(nf_lut_block[nf_zone]) << 5;        
          continue;
        }
          
        /* affect to the free physical block a fictive logical block */
        /* free physical block => gl_buffer[x] = 0xFF */
        temp_address = (Uint32)(free_bloc_pos) << 5;
        for (j = 0; j <= 0xFE; j += 2)
        {
          if (gl_buffer[j] == 0xFF)
          {
            Nf_wait_busy();                 /* in case of erase block before */
            do                              /* Search free physical block */
            {
              temp_address += 32;
              if (temp_address >= ((Uint32)(nf_zone + 1) << 15))
                  temp_address = (Uint32)(nf_zone) << 15;
    
              Nf_read_open_C_area(temp_address, 0x05);
              
              byte_5 = Nf_rd_byte();        /* Invalid/Valid block */
              byte_6 = Nf_rd_byte();        /* Used/Unused block */
            }
            while ((byte_6 != 0xFF) || (byte_5 != 0xFF));
 
            nf_block_erase(temp_address);   /* It may be not empty block (unplug during the first page writing) */

            free_bloc_pos = temp_address >> 5;
            gl_buffer[j] = (free_bloc_pos >> 8) | 0x80;
            gl_buffer[j + 1] = free_bloc_pos;
          }
        }
        
        Nf_wait_busy();
        if (half_page == 0)
        {
          Nf_write_open_A_area(gl_address, 0x00); /* Write first part of LUT */
          nf_download_buffer();                   /* Write buffer */
          Nf_send_command(NF_PAGE_PROGRAM_CMD);   /* Launch programmation */
        }
        else
        {
          Nf_write_open_B_area(gl_address, 0x00); /* Write second part of LUT */ 
          nf_download_buffer();                   /* Write buffer */
          Nf_wr_byte(0xFF); Nf_wr_byte(0xFF); Nf_wr_byte(0xFF);/* Write redundant data */
          Nf_wr_byte(0xFF); Nf_wr_byte(0xFF);
          Nf_wr_byte(0xFF);
          Nf_wr_byte(0xE8); Nf_wr_byte(0xFF);                   /* Logical block value */
          Nf_wr_byte(0xFF); Nf_wr_byte(0xFF); Nf_wr_byte(0xFF); /* ECC area 2 */
          Nf_wr_byte(0xE8); Nf_wr_byte(0xFF);                   /* Logical block value */
          Nf_wr_byte(0xFF); Nf_wr_byte(0xFF); Nf_wr_byte(0xFF); /* ECC area 1 */
          Nf_send_command(NF_PAGE_PROGRAM_CMD);
          gl_address++;
        }
        half_page = ~half_page;
        start    += 0x80; /* process next 128 logical block */
        end      += 0x80;
      }
      while (start < NF_BLOCK_PER_ZONE);
    }
  }
#undef half_page
#undef temp_address
#undef i
#undef start
#undef end 
#undef free_bloc_pos

  return OK;
}


/*F**************************************************************************
* NAME: nf_init
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*   OK : init complete
*   KO : - NF not supported or not recognise
*----------------------------------------------------------------------------
* PURPOSE:
*   NF initialisation
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
bit nf_init (void)
{

  P2 = NF_IDLE_STATE_ADD;
  Nf_CS_ON();

  Nf_send_command(NF_RESET_CMD);
  Nf_wait_busy();
  #if (NF_CAPACITY_AUTO_DETECT == TRUE)               /* Auto Detect the type of nand-flash  */
    Nf_send_command(NF_READ_ID_CMD);
    Nf_send_address(0x00);
    Nf_rd_byte();                                     /* Maker  code */
    switch (Nf_rd_byte())                             /* Device code */
    {
      /***************************************************************************/
      case 0x73 :                                     /*------- 16 Mbyte --------*/
        nf_device_type = NF_SIZE_16MB;
        nf_zone_max = 1;                              /* 1 zone : 1024 blocks    */
        nf_mem_size =  NF_SECTOR_SIZE_16MB - MEM_RESERVED_SIZE;
        nf_4_cycle_address = 0;                       /* 3 address cycles        */
      break;
      /***************************************************************************/
      case 0x75 :                                     /*------- 32 Mbyte --------*/
        nf_device_type = NF_SIZE_32MB;
        nf_zone_max = 2;                              /* 2 zones : 2048 blocks   */
        nf_mem_size =  NF_SECTOR_SIZE_32MB - MEM_RESERVED_SIZE;
        nf_4_cycle_address = 0;                       /* 3 address cycles        */
      break;
      /***************************************************************************/
      case 0x76 :                                     /*------- 64 Mbyte --------*/
        nf_device_type = NF_SIZE_64MB;
        nf_zone_max = 4;                              /* 4 zones : 4096 blocks   */
        nf_mem_size =  NF_SECTOR_SIZE_64MB - MEM_RESERVED_SIZE;
        nf_4_cycle_address = 1;                       /* 4 address cycles        */
        break;
      /***************************************************************************/
      case 0x79 :                                     /*------ 128 Mbyte --------*/
        nf_device_type = NF_SIZE_128MB;
        nf_zone_max = 8;                              /* 8 zones : 8192 blocks   */
        nf_mem_size =  NF_SECTOR_SIZE_128MB - MEM_RESERVED_SIZE;
        nf_4_cycle_address = 1;                       /* 4 address cycles        */
        break;
      default:
        return KO;
    }
  #else
    nf_mem_size = NF_DISK_SIZE - MEM_RESERVED_SIZE;                     
  #endif

  nf_reserved_space_start = nf_mem_size + 1;

  return OK;
  
}


/*F**************************************************************************
* NAME: nf_read_open
*----------------------------------------------------------------------------
* PARAMS:
*   pos: address of the logic sector to read (size 512 bytes)
*
* return:
*   Update memory for reading
*----------------------------------------------------------------------------
* PURPOSE:
*   Low level memory read update
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
bit nf_read_open (Uint32 pos)
{
Uint16 physical_block;

  Nf_CS_ON();

  if (nf_lut_modified)
  {
    nf_reassign_block();
    nf_lut_modified = FALSE;
  }

  gl_ptr_mem = pos;
  gl_cpt_page = 0;

  /* Determine the logical block value */
  nf_logical_block = (gl_ptr_mem >> 5);

  /* Determinate zone */
  nf_zone = nf_logical_block / 1000;

  /* Each zone have 1000 data blocks */
  nf_logical_block = nf_logical_block - (1000 * (Uint16)(nf_zone));

  /* Calculate the address where are the physical block value */
  gl_address  = ((Uint32)(nf_lut_block[nf_zone])<<5) + ((Uint32)(nf_logical_block) >> 8);
  gl_address += ((Uint32)(nf_lut_index[nf_zone])<<2);
  /* Open the look-up table */
  Nf_wait_busy();
  if (nf_logical_block & 0x80)
  {
    Nf_send_command(NF_READ_B_AREA_CMD);            /* 2nd half page */
    Nf_send_address((nf_logical_block << 1) - 256);
  }
  else
  {
    Nf_send_command(NF_READ_A_AREA_CMD);            /* first half page */
    Nf_send_address(nf_logical_block << 1);
  }

  Nf_send_address (((Byte*)&gl_address)[3]);   /* 2nd address cycle         */
  Nf_send_address (((Byte*)&gl_address)[2]);   /* 3rd address cycle         */
  if (NF_4_CYCLE_ADDRESS_BIT)                  /* Size of card >= 64Mbytes ?*/
    Nf_send_address (((Byte*)&gl_address)[1]); /* 4th address cycle         */
  
  Nf_wait_busy();

  /* Read the physical block number */
  ((Byte*)&physical_block)[0] = Nf_rd_byte();
  ((Byte*)&physical_block)[1] = Nf_rd_byte();

  /* Calculate the physical sector address */
  nf_current_physical_sector_addr = ((Uint32)(physical_block) << 5) + (gl_ptr_mem & NF_BLOCK_MASK); 
  Nf_CS_OFF();
  return OK;
}


/*F**************************************************************************
* NAME: nf_read_close
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Low level memory read close
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void nf_read_close (void)
{
  Nf_CS_OFF();
}


/*F*************************************************************************
* NAME: nf_read_byte
*---------------------------------------------------------------------------
* PARAMS:
*
* return:
*   Data read from memory
*---------------------------------------------------------------------------
* PURPOSE:
*   Low level memory read function
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
****************************************************************************/
Byte nf_read_byte (void)
{
Byte b;
  
  if (!gl_cpt_page)
  {
    Nf_CS_ON();
    Nf_wait_busy();
    Nf_read_open_A_area(nf_current_physical_sector_addr, 0x00);
    b = Nf_rd_byte();
    Nf_CS_OFF();
  }
  else
  {
    b = Nf_rd_byte();
  }
  
  gl_cpt_page++;
  
  /* Detection of the end of data page */
  if (((Byte*)&gl_cpt_page)[0] == NF_DATA_SIZE_H)                 
  {
    /* read spare data bytes */
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();
    Nf_rd_byte();

    gl_ptr_mem++;                           /* new page */
    gl_cpt_page=0;                          /* start at column 0 */

    if (!(((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK))               /* New block ? */
    {
      nf_read_open(gl_ptr_mem);
    }
    else
    {
      nf_current_physical_sector_addr++;
    }
  }
  return b;
}


/*F**************************************************************************
* NAME: nf_read_sector
*----------------------------------------------------------------------------
* PARAMS:
*   nb_sector:  number of contiguous sector to read 
*   global:     gl_ptr_mem
*
* return: OK read done
*         KO read failure
*----------------------------------------------------------------------------
* PURPOSE: 
*   This function is an optimized function that writes nb-sector * 512 bytes
*   from NF card to USB controller 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*   nb_sector always >= 1, can not be zero
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
bit nf_read_sector(Uint16 nb_sector)
{
Byte i;

#ifndef NO_SUPPORT_USB_PING_PONG
  bit   begin_ping_pong;
    begin_ping_pong = TRUE;
#endif

  do
  {
    Nf_CS_ON();
    Nf_wait_busy();
    Nf_read_open_A_area(nf_current_physical_sector_addr, 0x00);
  
    for (i = 8; i != 0; i--)
    {
      Usb_write_byte(Nf_rd_byte());         /* read 64 bytes from card */
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());
      Usb_write_byte(Nf_rd_byte());

      #ifdef NO_SUPPORT_USB_PING_PONG
          Usb_set_TXRDY();                      /* start usb transfer */
          while (!Usb_tx_complete());           /* wait end of transfer */
          Usb_clear_TXCMPL();                   /* ack transfer */
      #else
          if (begin_ping_pong)
          {
            begin_ping_pong = FALSE;
          }
          else
          {
            while (!Usb_tx_complete());         /* wait end of transfer */
            Usb_clear_TXCMPL();                 /* ack transfer */
          }
          Usb_set_TXRDY();                      /* start usb transfer */
      #endif
    }
  
    gl_ptr_mem++;                           /* new page */
    Nf_CS_OFF();
    if (! ( ((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK ) )               /* New block ? */
    {
      nf_read_open(gl_ptr_mem);
    }
    else
    {
      nf_current_physical_sector_addr++;
    }
    nb_sector--;
  }
  while (nb_sector != 0);

  #ifndef NO_SUPPORT_USB_PING_PONG
    while (!Usb_tx_complete());             /* wait end of last transfer */
    Usb_clear_TXCMPL();                     /* ack transfer */
  #endif

  return OK;
}


/*F**************************************************************************
* NAME: nf_write_open
*----------------------------------------------------------------------------
*----------------------------------------------------------------------------
* PARAMS:
*   pos: address of the the next write data
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Low level memory write update
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
bit nf_write_open (Uint32 pos)
{
Byte i;
Byte j;
bit change_zone = FALSE;

  gl_ptr_mem = pos;
  gl_cpt_page = 0;

  /* Determine the logical block value */
  nf_logical_block = (gl_ptr_mem >> 5);
  nf_zone = 0;

  while (nf_logical_block > 999)
  {
    nf_logical_block -= 1000;
    nf_zone++;
  }
  Nf_CS_ON();

  if (nf_zone != nf_old_zone)
  {
    change_zone = TRUE;
    if (nf_lut_modified)
    { /* save old lut */
      i = nf_old_zone;
      nf_old_zone = nf_zone;
      nf_zone = i;
      nf_reassign_block();
      nf_lut_modified = FALSE;
      nf_zone = nf_old_zone;
    }
    else
    { /* update old zone value */
      nf_old_zone = nf_zone;
    }
    nf_block_min = 0xFFFF;
    gl_buf_free_idx = 0;
  }

  if ((nf_logical_block >= nf_block_min) && (nf_logical_block < (nf_block_min + NF_BUFFER_SIZE - 5)))
  { /* we don't have to update the buffer */
    nf_calc_logical_block();
    nf_gl_buf_idx = nf_logical_block - nf_block_min;
  }
  else 
  { /* we have to update the buffer */
    /* Store the look up table block address */
    nf_look_up_table_block =  nf_lut_block[nf_zone];
    /* If LUT have been modified */
    if (nf_lut_modified)           
    {
      nf_reassign_block();                  /* update the LUT */
    }
    /* Calculate the address for LUT access */
    gl_address = ((Uint32)(nf_look_up_table_block)<<5) + ((Uint32)(nf_logical_block) >> 8);
    gl_address += (Uint32)(nf_lut_index[nf_zone])<<2;
    /* Calculate the redundant block address value */
    nf_calc_logical_block();
    i = nf_logical_block;
    /* For the current zone, initialize the number of spare block */
    nf_spare_block = nf_spare_block_number[nf_zone];
    /* Calculate the low and the high block stored in the buffer */
    nf_block_min = nf_logical_block;
    nf_block_max = ((nf_logical_block + NF_BUFFER_SIZE) > 999) ? (1000 - nf_logical_block) : NF_BUFFER_SIZE;

    /* Open the look-up table */
    Nf_wait_busy();     
    if (((Byte*)&nf_logical_block)[1] & 0x80)
    {
      Nf_send_command(NF_READ_B_AREA_CMD);  /* 2nd half page */
      Nf_send_address((nf_logical_block << 1) - 256);
    }
    else
    {
      Nf_send_command(NF_READ_A_AREA_CMD);  /* first half page */
      Nf_send_address(nf_logical_block << 1);
    }

    Nf_send_address (((Byte*)&gl_address)[3]);     /* 2nd address cycle         */
    Nf_send_address (((Byte*)&gl_address)[2]);     /* 3rd address cycle         */
    if (NF_4_CYCLE_ADDRESS_BIT)                    /* Size of card >= 64Mbytes ?*/
      Nf_send_address ( ((Byte*)&gl_address)[1] ); /* 4th address cycle         */
  
    Nf_wait_busy();

    for (j = 0; j < nf_block_max ; j++)
    {
      /* Read conversion table : 64 datas */
      nf_buf[j].b[0] = Nf_rd_byte();
      nf_buf[j].b[1] = Nf_rd_byte();
      i++;
      if (i == 0)
      {
        gl_address++;
        Nf_read_open_A_area(gl_address, 0x00);
      }
    }
  
    if (change_zone)        /* If it is a new zone, then load the spare buffer */
    {
      /* Check for used block : first, read in the LUT the free physical block */
      gl_address = ((Uint32)(nf_look_up_table_block)<<5) + 3;
      gl_address += (Uint32)(nf_lut_index[nf_zone])<<2;
      Nf_read_open_B_area(gl_address, 208);
      for (i = 0; i < nf_spare_block; i++)
      {
        nf_buf_free[i] = (Uint16)(Nf_rd_byte()<<8);
        nf_buf_free[i] +=  Nf_rd_byte();
      }
    }
    nf_gl_buf_idx = 0;           /* initialize index for main buffer */
    nf_gl_buf_idx_max = 0;       /* initialize the max index for the buffer */
  }
  
  /* if block is already assigned, then invert with a spare block */
  nf_block_used = (!(nf_buf[nf_gl_buf_idx].w & 0x8000)) ? TRUE : FALSE;
  if (nf_block_used)
  {
    /* assign block to be deleted */
    nf_block_to_be_deleted = nf_buf[nf_gl_buf_idx].w;
    nf_buf[nf_gl_buf_idx].w = nf_buf_free[gl_buf_free_idx];
    nf_buf_free[gl_buf_free_idx++] = nf_block_to_be_deleted | 0x8000;
    /* increase the index for spare block buffer */
    if (gl_buf_free_idx >= nf_spare_block)
    {
      gl_buf_free_idx = 0;
    }
  }

  /* Mark block as assigned */
  nf_buf[nf_gl_buf_idx].w &= 0x7FFF;       

  if (nf_gl_buf_idx > nf_gl_buf_idx_max)
    nf_gl_buf_idx_max = nf_gl_buf_idx;

  /* Update the current physical sector address */
  nf_current_physical_sector_addr = ((Uint32)(nf_buf[nf_gl_buf_idx].w) << 5);

  if (nf_block_used)                        /* Fisrt block already used block ?  */
  { 
    nf_copy_block_head();                   /* Copy the first part of the block  */
  }
  else                                 
  { 
    nf_init_spare();                        /* else init spare data for new logical block  */
  }

  nf_busy = FALSE;                          /* Clear flag busy */
  nf_lut_modified = TRUE;
  /* Set LUT as modified */
  gl_address = ((Uint32)(nf_look_up_table_block)<<5);
  gl_address += (Uint32)(nf_lut_index[nf_zone])<<2;
  Nf_wait_busy();
  Nf_write_open_C_area(gl_address, 0x00);
  Nf_wr_byte(0x00);                         /* Reset first byte */
  Nf_send_command(NF_PAGE_PROGRAM_CMD);     /* Send program command to the device */
  
  return OK; 
}


/*F**************************************************************************
* NAME: nf_write_close
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*----------------------------------------------------------------------------
* PURPOSE:
*   Low level memory write close: release NF 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
bit nf_write_close (void)
{ 
  Nf_CS_ON();
  if (gl_cpt_page)
  {
    while (((Byte*)&gl_cpt_page)[0] != NF_DATA_SIZE_H)
    {
      gl_cpt_page++;
      Nf_wr_byte(0x00);
    }
    nf_update_spare_data();
    Nf_send_command(NF_PAGE_PROGRAM_CMD);
    ((Byte*)&gl_cpt_page)[0] = 0;
    gl_ptr_mem++;
    ((Byte*)&nf_current_physical_sector_addr)[3]++;
  }

  /* Copy last part of a block if the block was assigned */
  if (nf_block_used)
  {
    if (((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK) 
      nf_copy_block_tail();
    nf_block_erase((Uint32)(nf_block_to_be_deleted) << 5);    /* Erase old block */
  }
  Nf_CS_OFF();
  return OK;
}


/*F**************************************************************************
* NAME: nf_write_byte
*----------------------------------------------------------------------------
* PARAMS:
*   b: data to write
*
* RETURN:
*   write status: OK: write done
*                 KO: write not done
*
*----------------------------------------------------------------------------
* PURPOSE:
*   Low level memory write function
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
bit nf_write_byte (Byte b)
{
  if (nf_busy)
  {
    nf_busy = FALSE;
    /* If previous block have to de deleted */
    if (nf_block_used)       
    {
      nf_block_erase((Uint32)(nf_block_to_be_deleted) << 5);
    }
    /* increase the main buffer index */
    nf_gl_buf_idx++;
    /* if zone change */
    if (nf_gl_buf_idx >= nf_block_max)
    {
      nf_write_open(gl_ptr_mem);
    }
    else
    {
      /* if the block in the buffer is already assign, then invert it with a spare block */
      if (!(nf_buf[nf_gl_buf_idx].w & 0x8000))                 
      {                                                 
        /* invert the block and assign the next block to be deleted */
        nf_block_to_be_deleted       = nf_buf[nf_gl_buf_idx].w;
        nf_buf[nf_gl_buf_idx].w      = nf_buf_free[gl_buf_free_idx] & 0x7FFF;
        nf_buf_free[gl_buf_free_idx++] = nf_block_to_be_deleted | 0x8000;
        /* block have to be deleted */
        nf_block_used = TRUE;
        /* increase the spare buffer index */
        if (gl_buf_free_idx >= nf_spare_block)
        {
          gl_buf_free_idx = 0;
        }

      }
      else /* The block is not assigned. Nothing to do */
      {
        nf_buf[nf_gl_buf_idx].w &= 0x7FFF;
        nf_block_used = FALSE;
      }
      /* update the max index buffer */
      if (nf_gl_buf_idx > nf_gl_buf_idx_max)
        nf_gl_buf_idx_max = nf_gl_buf_idx;

      /* Update current physical sector */
      nf_current_physical_sector_addr = (Uint32)(nf_buf[nf_gl_buf_idx].w) << 5;
      /* increase the logical block */
      nf_logical_block++;
      /* calculate the redundant block address */
      nf_calc_logical_block();
    }
  }

  Nf_CS_ON();
  if (!gl_cpt_page)
  {
    Nf_wait_busy();
    Nf_write_open_A_area(nf_current_physical_sector_addr, 0x00);
  }
  Nf_wr_byte(b);
  /* Increase internal page counter */
  gl_cpt_page++;                          
  if (((Byte*)&gl_cpt_page)[0] == NF_DATA_SIZE_H) /* check if end of data page */
  {
    nf_update_spare_data();
    Nf_send_command(NF_PAGE_PROGRAM_CMD);
    ((Byte*)&gl_cpt_page)[0] = 0;
    gl_ptr_mem++;
    if (!(((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK)) 
    {
      nf_busy = TRUE;
    }
    else
    {
      ((Byte*)&nf_current_physical_sector_addr)[3]++;
    }
  }

  return OK;
}


/*F**************************************************************************
* NAME: nf_write_sector
*----------------------------------------------------------------------------
* PARAMS:
*   global: gl_ptr_mem
*
* return:
*   write status: OK: write done
*                 KO: write not done
*----------------------------------------------------------------------------
* PURPOSE: 
*   This function is an optimized function that writes nb-sector * 512 bytes
*   from USB controller to NF card
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*   nb_sector always >= 1, can not be zero
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
bit nf_write_sector (Uint16 nb_sector)
{
Byte j;

  do
  {   
    if (nf_busy)
    {
      nf_busy = FALSE;
      /* If previous block have to de deleted */
      if (nf_block_used)       
      {
        nf_block_erase((Uint32)(nf_block_to_be_deleted) << 5);
      }
      /* increase the main buffer index */
      nf_gl_buf_idx++;
      /* if zone change */
      if (nf_gl_buf_idx >= nf_block_max)
      {
        nf_write_open(gl_ptr_mem);
      }
      else
      {
        /* if the block in the buffer is already assign, then invert it with a spare block */
        if (!(nf_buf[nf_gl_buf_idx].w & 0x8000))                 
        {                                                 
          /* invert the block and assign the next block to be deleted */
          nf_block_to_be_deleted       = nf_buf[nf_gl_buf_idx].w;
          nf_buf[nf_gl_buf_idx].w      = nf_buf_free[gl_buf_free_idx] & 0x7FFF;
          nf_buf_free[gl_buf_free_idx++] = nf_block_to_be_deleted | 0x8000;
          /* block have to be deleted */
          nf_block_used = TRUE;
          /* increase the spare buffer index */
          if (gl_buf_free_idx >= nf_spare_block)
          {
            gl_buf_free_idx = 0;
          }
        }
        else /* The block is not assigned. Nothing to do */
        {
          nf_buf[nf_gl_buf_idx].w &= 0x7FFF;
          nf_block_used = FALSE;
        }
        /* update the max index buffer */
        if (nf_gl_buf_idx > nf_gl_buf_idx_max)
          nf_gl_buf_idx_max = nf_gl_buf_idx;
  
        /* Update current physical sector */
        nf_current_physical_sector_addr = (Uint32)(nf_buf[nf_gl_buf_idx].w) << 5;
        /* increase the logical block */
        nf_logical_block++;
        /* calculate the redundant block address */
        nf_calc_logical_block();
      }
    }

    Nf_CS_ON();
  
    Nf_wait_busy();
    Nf_write_open_A_area(nf_current_physical_sector_addr, 0x00);

    for (j = 8; j != 0; j--)
    {
      while (!Usb_rx_complete());             /* wait end of reception */
      Nf_wr_byte(Usb_read_byte());            /* write 64 bytes to card */
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Nf_wr_byte(Usb_read_byte());
      Usb_clear_RXOUT_PP();                      /* usb read acknowledgement */
    }
  
    /* Update spare data */
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(((Byte*)&nf_redundant_logical_block_value)[0]);
    Nf_wr_byte(((Byte*)&nf_redundant_logical_block_value)[1]);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(0xFF);
    Nf_wr_byte(((Byte*)&nf_redundant_logical_block_value)[0]);
    Nf_wr_byte(((Byte*)&nf_redundant_logical_block_value)[1]);
  
    Nf_send_command (NF_PAGE_PROGRAM_CMD);
    /* increase gl_ptr_mem : next page */
    gl_ptr_mem++;              
    /* set flag busy */
    if (!(((Byte*)&gl_ptr_mem)[3] & NF_BLOCK_MASK)) 
    {
      nf_busy = TRUE;
    }
    else
    {
      ((Byte*)&nf_current_physical_sector_addr)[3]++;
    }
    nb_sector--;
  }
  while (nb_sector != 0);
  return OK;
}


/*F**************************************************************************
* NAME: nf_format
*----------------------------------------------------------------------------
* PARAMS:
*
* return:
*   Address of the format parameter structure in code
*----------------------------------------------------------------------------
* PURPOSE: 
*   This function is called by the fat_format function and returns a pointer
*   to a table containing the format parameters after erasing the NF.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE:
*----------------------------------------------------------------------------
* REQUIREMENTS:
*****************************************************************************/
s_format * nf_format (void)
{

#define CYL_16  (Uint16)((Uint16)(500) - (MEM_RESERVED_SIZE / (4 * 16)))
#define CYL_32  (Uint16)((Uint16)(500) - (MEM_RESERVED_SIZE / (8 * 16)))
#define CYL_64  (Uint16)((Uint16)(500) - (MEM_RESERVED_SIZE / (8 * 32)))
#define CYL_128 (Uint16)((Uint16)(500) - (MEM_RESERVED_SIZE / (16 * 32)))

code  s_format  nf_tab_format[]=
  {
   /* nb_cylinder, nb_head,  nb_sector, nb_hidden, nb_sector_per_cluster */
    { CYL_16, (Byte)4,  (Byte)16,  (Byte)57,  (Byte)32 }, /* 16MB */
    { CYL_32, (Byte)8,  (Byte)16,  (Byte)51,  (Byte)32 }, /* 32MB */
    { CYL_64, (Byte)8,  (Byte)32,  (Byte)39,  (Byte)32 }, /* 64MB */
    { CYL_128, (Byte)16, (Byte)32,  (Byte)63,  (Byte)32 }, /* 128MB */
  };

code  s_format  nf_tab_format_reserved[]=
  {
   /* nb_cylinder, nb_head,  nb_sector, nb_hidden, nb_sector_per_cluster */
    { (Uint16)500, (Byte)4,  (Byte)16,  (Byte)57,  (Byte)32 }, /* 16MB */
    { (Uint16)500, (Byte)8,  (Byte)16,  (Byte)51,  (Byte)32 }, /* 32MB */
    { (Uint16)500, (Byte)8,  (Byte)32,  (Byte)39,  (Byte)32 }, /* 64MB */
    { (Uint16)500, (Byte)16, (Byte)32,  (Byte)63,  (Byte)32 }, /* 128MB */
  };

  /* Erase all block */
  nf_erase_all_block();

  /* -- NF Type Selection -- */
  if (reserved_disk_space == FALSE)
  {
    if (nf_reserved_space == TRUE)
    {
      nf_mem_size -= MEM_RESERVED_SIZE;
      nf_reserved_space = FALSE;
    }
    return &nf_tab_format[NF_TYPE];
  }
  else
  {
    if (nf_reserved_space == FALSE)
    {
      nf_reserved_space = TRUE;
      nf_mem_size += MEM_RESERVED_SIZE;
    }
    return &nf_tab_format_reserved[NF_TYPE];
  }
}

