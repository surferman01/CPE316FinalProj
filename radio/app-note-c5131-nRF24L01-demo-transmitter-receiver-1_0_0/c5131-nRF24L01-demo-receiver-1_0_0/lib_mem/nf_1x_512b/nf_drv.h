/*H**************************************************************************
* NAME:         nf_drv.h         
*----------------------------------------------------------------------------
* Copyright (c) 2004 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      snd1c-demo-nf-mmc-4_6_0      
* REVISION:     1.16     
*----------------------------------------------------------------------------
* PURPOSE:
* This file contains the NF driver definitions
*****************************************************************************/

#ifndef _NF_DRV_H_
#define _NF_DRV_H_


/*_____ I N C L U D E S ____________________________________________________*/

#include "board.h"                             /* board definition */
#include "conf\conf_nf.h"

/*_____ M A C R O S ________________________________________________________*/

/************************** Read Command ************************************/
#define NF_READ_A_AREA_CMD           0x00
#define NF_READ_B_AREA_CMD           0x01
#define NF_READ_C_AREA_CMD           0x50
/************************** Read ID Command *********************************/
#define NF_READ_ID_CMD               0x90
#define NF_RESET_CMD                 0xff
/************************** Program command *********************************/
#define NF_SEQUENTIAL_DATA_INPUT_CMD 0x80
#define NF_PAGE_PROGRAM_CMD          0x10
/************************** Erase command ***********************************/
#define NF_BLOCK_ERASE_CMD           0x60
#define NF_BLOCK_ERASE_CONFIRM_CMD   0xD0
/************************** Read Status command *****************************/
#define NF_READ_STATUS_CMD           0x70

#define NF_BUFFER_SIZE              (Byte)(133)

#define Nf_wait_busy()              {while (!NF_RD_BUSY);}

/* Nand Flash Driver */
#define NF_1X_512B            0
#define NF_2X_512B            1
#define NF_1X_2KB             2
#define NF_2X_2KB             3


/*_____ D E F I N I T I O N ________________________________________________*/

/* NAND FLASH / SMARTMEDIA CARD DEFINITION */
/* This value are only for 16Mbytes and upper SMARTMEDIA Card capacities   */
#define NF_PAGE_PER_BLOCK (32)    /* pages_block value: pages in a block   */ 
#define NF_DATA_SIZE      (512)   /* data_size   value: data_size in bytes */
#define NF_SPARE_SIZE     (16)    /* spare_size  value: spare_size in bytes*/
#define NF_BLOCK_PER_ZONE (1024)  /* 1024 block per size                   */
#define NF_DATA_SIZE_H    (NF_DATA_SIZE >> 8)
#define NF_DATA_SIZE_L    (NF_DATA_SIZE & 0xFF)
#define NF_BLOCK_MASK     (NF_PAGE_PER_BLOCK - 1)

#if (NF_CAPACITY_AUTO_DETECT == TRUE)
  #define NF_ZONE_MAX             (Byte)(8)
  #define NF_4_CYCLE_ADDRESS_BIT  nf_4_cycle_address  
  #define NF_TYPE                 nf_device_type
  #define NF_ZONE_MAX_CPT         nf_zone_max
  #define NF_SECTOR_SIZE          (Uint32)(nf_mem_size)

  #define NF_SIZE_16MB            (Byte)(0)
  #define NF_SIZE_32MB            (Byte)(1)
  #define NF_SIZE_64MB            (Byte)(2)
  #define NF_SIZE_128MB           (Byte)(3)

  #define NF_SECTOR_SIZE_16MB     (Uint32)(31999)
  #define NF_SECTOR_SIZE_32MB     (Uint32)(63999)
  #define NF_SECTOR_SIZE_64MB     (Uint32)(127999)
  #define NF_SECTOR_SIZE_128MB    (Uint32)(255999)
#else
  #ifdef NF_16
  #define NF_TYPE               (Byte)(0)
  #define NF_SIZE               (Byte)(16)
  #define NF_ZONE_MAX           (Byte)(1)
  #define NF_SECTOR_SIZE        (Uint32)(31999)
  #define NF_4_CYCLE_ADDRESS    (0)
  #endif
  
  #ifdef NF_32
  #define NF_TYPE               (Byte)(1)
  #define NF_SIZE               (Byte)(32)
  #define NF_ZONE_MAX           (Byte)(2)
  #define NF_SECTOR_SIZE        (Uint32)(63999)
  #define NF_4_CYCLE_ADDRESS    (0)
  #endif
  
  #ifdef NF_64
  #define NF_TYPE               (Byte)(2)
  #define NF_SIZE               (Byte)(64)
  #define NF_ZONE_MAX           (Byte)(4)
  #define NF_SECTOR_SIZE        (Uint32)(127999)
  #define NF_4_CYCLE_ADDRESS    (1)
  #endif
  
  #ifdef NF_128
  #define NF_TYPE               (Byte)(3)
  #define NF_SIZE               (Byte)(128)
  #define NF_ZONE_MAX           (Byte)(8)
  #define NF_SECTOR_SIZE        (Uint32)(255999)
  #define NF_4_CYCLE_ADDRESS    (1)
  #endif
  
  #ifndef NF_TYPE
    #error NF capacity must be defined in board.h
  #endif

  #define NF_ZONE_MAX_CPT         NF_ZONE_MAX
  #define NF_4_CYCLE_ADDRESS_BIT  NF_4_CYCLE_ADDRESS  
#endif

/* SCSI DEFINITION FOR NF/SMARTMEDIA */
#define NF_BLOCK_SIZE     ((Uint32)(NF_DATA_SIZE))
#define NF_DISK_SIZE      ((Uint32)(NF_SECTOR_SIZE))

/* Low Level routine */
#define Nf_rd_byte()                (nf_data)
#define Nf_wr_byte(b)               (nf_data = b)
#define Nf_send_command(command)    (nf_send_cmd = command)
#define Nf_send_address(address)    (nf_send_add = address)

#if NF_CAPACITY_AUTO_DETECT
  #define Nf_write_open_B_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_B_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        if (NF_4_CYCLE_ADDRESS_BIT)                                           \
          Nf_send_address ( ((Byte*)&address_sector)[1] )
  
  
  #define Nf_write_open_A_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_A_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        if (NF_4_CYCLE_ADDRESS_BIT)                                           \
          Nf_send_address ( ((Byte*)&address_sector)[1] )
  
  #define Nf_write_open_C_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_C_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        if (NF_4_CYCLE_ADDRESS_BIT)                                           \
          Nf_send_address ( ((Byte*)&address_sector)[1] )
  
  #define Nf_read_open_A_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_A_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        if (NF_4_CYCLE_ADDRESS_BIT)                                           \
          Nf_send_address ( ((Byte*)&address_sector)[1] );                    \
        Nf_wait_busy()
  
  #define Nf_read_open_B_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_B_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        if (NF_4_CYCLE_ADDRESS_BIT)                                           \
          Nf_send_address ( ((Byte*)&address_sector)[1] );                    \
        Nf_wait_busy()
  
  #define Nf_read_open_C_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_C_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        if (NF_4_CYCLE_ADDRESS_BIT)                                           \
          Nf_send_address ( ((Byte*)&address_sector)[1] );                    \
        Nf_wait_busy()
#else
  #if (NF_4_CYCLE_ADDRESS == 1)
  
  #define Nf_write_open_B_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_B_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[1] )
  
  
  #define Nf_write_open_A_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_A_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[1] )
  
  #define Nf_write_open_C_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_C_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[1] )
  
  #define Nf_read_open_A_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_A_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[1] );                      \
        Nf_wait_busy()
  
  #define Nf_read_open_B_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_B_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[1] );                      \
        Nf_wait_busy()
  
  #define Nf_read_open_C_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_C_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[1] );                      \
        Nf_wait_busy()
  
  #endif
  
  #if (NF_4_CYCLE_ADDRESS == 0)
  
  #define Nf_write_open_B_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_B_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] )
  
  
  #define Nf_write_open_A_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_A_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] )
  
  #define Nf_write_open_C_area(address_sector, nb_byte)                       \
        Nf_send_command (NF_READ_C_AREA_CMD);                                 \
        Nf_send_command (NF_SEQUENTIAL_DATA_INPUT_CMD);                       \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] )
  
  #define Nf_read_open_B_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_B_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_wait_busy()
  
  #define Nf_read_open_C_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_C_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_wait_busy()
  
  #define Nf_read_open_A_area(address_sector, nb_byte)                        \
        Nf_send_command (NF_READ_A_AREA_CMD);                                 \
        Nf_send_address ( nb_byte );                                          \        
        Nf_send_address ( ((Byte*)&address_sector)[3] );                      \
        Nf_send_address ( ((Byte*)&address_sector)[2] );                      \
        Nf_wait_busy()
  
  #endif
#endif


/*_____ D E C L A R A T I O N ______________________________________________*/

extern  xdata Uint32 nf_disk_size;

extern  void  nf_download_buffer(void);     /* copy the content of gl_buffer on the card */
extern  void  nf_upload_buffer(void);       /* load the content of the card on gl_buffer */
extern  void  nf_init_buffer(void);         /* initialize the buffer gl_buffer with 0xFF */

void  nf_copy_block_head(void);             /* copy the first sectors of a block */
void  nf_copy_block_tail(void);             /* copy the last sectors of a block */
void  nf_reassign_block(void);              /* reassign the physical block on the look up table */
bit   nf_block_erase (Uint32);              /* Erase one block */
bit   nf_erase_all_block(void);             /* Erase all block */
void  nf_init_spare(void);                  /* Init the first redundant data of a block */
void  nf_calc_logical_block();              /* Calulate the value of logical block in spare area */
void  nf_update_spare_data(void);           /* Write only the redundant information */
bit   nf_read_spare_byte(void);             /* LUT generation */
#endif /* _NF_DRV_H_ */
