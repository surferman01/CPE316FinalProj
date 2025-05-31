/*H**************************************************************************
* NAME:         board.h         
*----------------------------------------------------------------------------
* Copyright (c) 2004 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      snd1c-demo-nf-mmc-4_6_0      
* REVISION:     1.25     
*----------------------------------------------------------------------------
* PURPOSE:
* This file contains the demonstration board definition
*****************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_


/*_____ I N C L U D E S ____________________________________________________*/

//#include "lib_mcu\kbd\kbd_drv.h"           /* keyboard driver definition */

/*_____ M A C R O S ________________________________________________________*/


/*----- Board definition -----*/

/* BOARD VERSION */
#define VERSION         22                  /* V2.1 or V2.2 */

/* LCD */
#define LCD_CMD_ADD     0xA8FE              /* Command register address */
#define LCD_DAT_ADD     0xE8FF              /* Data register address */

/* DAC */
#define DAC_MUTE        P4_3                /* DAC mute port */
#define DAC_DEEM        P3_7                /* DAC de-emphasis port */
#define DAC_CLK         P3_1                /* DAC CSEL port */
#define DAC_NB_BIT      20                  /* 20-bit DAC */

/* ADC */
#define AD_CLOCK        (FPER / F_CONV_MAX / 2) //* ADC clock divider 
#define BATTERY_CHANNEL ((Byte)0x01)        //* ADC battery channel 1 
#define VOICE_CHANNEL   ((Byte)0x00)        //* ADC voice channel 0 

/* ECP Interface */
#define ECP_CTRL        P3                  /* Control port */
#define ECP_DATA        P5                  /* Data port */
#define ECP_PCLK        P3_0                /* PeriphClk signal */
#define ECP_PACK        P3_1                /* PeriphAck signal */
#define ECP_1284        P3_2                /* 1284_Active signal */
#define ECP_HCLK        P3_3                /* HostClk signal */
#define ECP_HACK        P3_4                /* HostAck signal */
#define ECP_NRRV        P3_5                /* nReverseRequest signal */
#define ECP_NARV        P3_7                /* nAckReverse signal */
#define ECP_NEGO        ((Byte)0x10)        /* negociation ECP byte */

/* DataFlash Memories */
#if VERSION == 21
  #define DF_4_MB                           /* AT45DB321 Memories */
#else
  #define DF_8_MB                           /* AT45DB642 Memories */
#endif
#define DF_CS           P4                  /* dataflash CS# port */
#define DF_CS0          P4_4
#define DF_CS1          P4_5
#define DF_CS2          P4_6
#define DF_CS3          P4_7
#define DF_DESEL_ALL    ((Byte)0xF0)        /* set CS# dataflash memories */
#define DF_NB_MEM       4                   /* 4 on-board memories */


/* MultiMediaCard */
#define MM_MAX_CARDS    2                   /* number of maximum on-board cards */

/* NAND Flash (SMC extended version) */
/* 
   For one nand flash or smc
   ALE = A9
   CLE = A8
   WP# = A10
   CS# = P2.3/A11
   R/B#= P2.4/A12
   nand flash Even will be selected when two nand flash card is used with as a single nand flash

   For two nand flash
   CLE      = P2.0/A8
   ALE      = P2.1/A9
   R/B# ODD = P2.2/A10
   CS# EVEN = P2.3/A11
   CS# ODD  = P2.5/A13
   R/B# EVEN= P2.4/A12
   WP# is connected to VDD

*/



#define SMC_IDLE_STATE_ADD        0x1C      /* Smart Media Card not selected */
#define SMC_ADD_LATCH_ENABLE_ADD  0x2600    /* Address Latch Enable Address */
#define SMC_CMD_LATCH_ENABLE_ADD  0x2500    /* Command Latch Enable Address */
#define SMC_ADDRESS_CMD_DATA      0x2400    /* Command / Data Address register */
#define SMC_RD_BUSY               P2_4
#define Smc_CS_ON()               (P2_3 = 0)
#define Smc_CS_OFF()              (P2_3 = 1)
#define SMC_PRESENT               (P3_2 )  /* Define here your pin out */

#define NF_WP                      P1_7
#define Nf_WP_ON()                 (P1_7 = 0)
#define Nf_WP_OFF()                (P1_7 = 1)

#define NF_CE0                 0
#define NF_CE1                 1
#define NF_CE                 NF_CE0

#if(NF_CE == NF_CE1)
/* even */
#define NF_IDLE_STATE_ADD         0x3C      /* Smart Media Card not selected */
#define NF_ADD_LATCH_ENABLE_ADD   0x2600    /* Address Latch Enable Address */
#define NF_CMD_LATCH_ENABLE_ADD   0x2500    /* Command Latch Enable Address */
#define NF_ADDRESS_CMD_DATA       0x2400    /* Command / Data Address register */
#define NF_RD_BUSY                P2_4
#define Nf_CS_ON()                (P2_3 = 0)
#define Nf_CS_OFF()               (P2_3 = 1)
#endif

#if(NF_CE == NF_CE0)
/* odd */
#define NF_IDLE_STATE_ADD         0x3C      /* Smart Media Card not selected */
#define NF_ADD_LATCH_ENABLE_ADD   0x0A00    /* Address Latch Enable Address */
#define NF_CMD_LATCH_ENABLE_ADD   0x0900    /* Command Latch Enable Address */
#define NF_ADDRESS_CMD_DATA       0x0800    /* Command / Data Address register */
#define NF_RD_BUSY                P2_2
#define Nf_CS_ON()                (P2_5 = 0)
#define Nf_CS_OFF()               (P2_5 = 1)
#endif

#define NF_IDLE_STATE_ADD_DOUBLE      0x3C      /* Idle state for 2 Nand Flash */
#define NF_ADD_LATCH_ENABLE_ADD_EVEN  0x6200    /* Address Latch Enable Address */
#define NF_CMD_LATCH_ENABLE_ADD_EVEN  0x6100    /* Command Latch Enable Address */
#define NF_ADDRESS_CMD_DATA_EVEN      0x6000    /* Command / Data Address register */
#define NF_ADD_LATCH_ENABLE_ADD_ODD   0x4A00    /* Address Latch Enable Address */
#define NF_CMD_LATCH_ENABLE_ADD_ODD   0x4900    /* Command Latch Enable Address */
#define NF_ADDRESS_CMD_DATA_ODD       0x4800    /* Command / Data Address register */

#define NF_BSY_ODD                    P2_2
#define NF_BSY_EVEN                   P2_4


#define NF_CS_ODD                     P2_5
#define NF_CS_EVEN                    P2_3

#define NF_EVEN                         0
#define NF_ODD                          1


#define Nf_CS_ODD()     nf_parity_bit = NF_ODD;  NF_CS_EVEN = 1; NF_CS_ODD  = 0
#define Nf_CS_EVEN()    nf_parity_bit = NF_EVEN; NF_CS_ODD  = 1; NF_CS_EVEN = 0

#define Nf_active_ODD()     NF_CS_EVEN = 1; NF_CS_ODD = 0
#define Nf_active_EVEN()    NF_CS_ODD = 1; NF_CS_EVEN = 0


#define NF_128   /* Nand Flash Capacity : NF_16 NF_32 NF_64 NF_128 */


/* IDE/ATA Definition */
/*  With LATCH
    A0        A0  P0_0
    A1        A1  P0_1
    A2        A2  P0_2
    CS0#      A3  P0_3
    CS1#      A4  P0_4

    For compact flash
    A0        A8  P2_0
    A1        A9  P2_1
    A2        A10 P2_2
    CS0#      A11 P2_3
    CS1#      A12 P2_4
                        CS1#  CS0#    A2    A1    A0
    Data                1     0       0     0     0       0x10
    Error/features      1     0       0     0     1       0x11
    Sector count        1     0       0     1     0       0x12
    Sector No           1     0       0     1     1       0x13
    Cyl. Low            1     0       1     0     0       0x14
    Cyl. High           1     0       1     0     1       0x15
    Select Card/Head    1     0       1     1     0       0x16
    Status/Command      1     0       1     1     1       0x17
    Alt stat/ dev ctrl  0     1       1     1     0       0x0E
    Drive address       0     1       1     1     1       0x0F
*/

#define DATA_REGISTER             0x10
#define ERROR_REGISTER            0x11
#define SECTOR_COUNT_REGISTER     0x12
#define SECTOR_NUMBER_REGISTER    0x13
#define CYL_LOW_REGISTER          0x14
#define CYL_HIGH_REGISTER         0x15
#define DRIVE_HEAD_REGISTER       0x16
#define STATUS_COMMAND_REGISTER   0x17
#define ALT_STAT_REGISTER         0x0E
#define DRIVE_REGISTER            0x0F

#define H_DATA_REGISTER             0x1000
#define H_ERROR_REGISTER            0x1100
#define H_SECTOR_COUNT_REGISTER     0x1200
#define H_SECTOR_NUMBER_REGISTER    0x1300
#define H_CYL_LOW_REGISTER          0x1400
#define H_CYL_HIGH_REGISTER         0x1500
#define H_DRIVE_HEAD_REGISTER       0x1600
#define H_STATUS_COMMAND_REGISTER   0x1700
#define H_ALT_STAT_REGISTER         0x0E00
#define H_DRIVE_REGISTER            0x0F00


#define IDE_PORT_IDLE             0x18

#define IDE_BASE_ADDRESS          0x8000

#define IDE_DATA_ADDRESS          (IDE_BASE_ADDRESS + DATA_REGISTER + H_DATA_REGISTER)
#define IDE_ERROR_ADDRESS         (IDE_BASE_ADDRESS + ERROR_REGISTER + H_ERROR_REGISTER)
#define IDE_SECTOR_COUNT_ADDRESS  (IDE_BASE_ADDRESS + SECTOR_COUNT_REGISTER + H_SECTOR_COUNT_REGISTER)
#define IDE_SECTOR_NUMBER_ADDRESS (IDE_BASE_ADDRESS + SECTOR_NUMBER_REGISTER + H_SECTOR_NUMBER_REGISTER)
#define IDE_CYLINDER_LOW_ADDRESS  (IDE_BASE_ADDRESS + CYL_LOW_REGISTER + H_CYL_LOW_REGISTER)
#define IDE_CYLINDER_HIGH_ADDRESS (IDE_BASE_ADDRESS + CYL_HIGH_REGISTER + H_CYL_HIGH_REGISTER)
#define IDE_DRIVE_HEAD_ADDRESS    (IDE_BASE_ADDRESS + DRIVE_HEAD_REGISTER + H_DRIVE_HEAD_REGISTER)
#define IDE_STATUS_ADDRESS        (IDE_BASE_ADDRESS + STATUS_COMMAND_REGISTER + H_STATUS_COMMAND_REGISTER)
#define IDE_COMMAND_ADDRESS       (IDE_BASE_ADDRESS + STATUS_COMMAND_REGISTER + H_STATUS_COMMAND_REGISTER)
#define IDE_ALT_STATUS_ADDRESS    (IDE_BASE_ADDRESS + ALT_STAT_REGISTER + H_ALT_STAT_REGISTER)
#define IDE_DRIVE_ADDRESS         (IDE_BASE_ADDRESS + DRIVE_REGISTER + H_DRIVE_REGISTER)


/* Compact Flash Definition   */
#define PIO_CDET1                  P3_3
#define PIO_CDET0                  P3_2
#define PIO_PWR                    P5_0
#define PIO_INTRQ                  P5_1
#define PIO_RESET                  P5_2
#define PIO_IORDY                  P5_3


/* Key Definition */
#define KEY_SOUND       KEY_0_0             //* sound select 
#define KEY_MEM         KEY_0_1             //* memory select 
#define KEY_REPEAT_A_B  KEY_0_2             //*  
#define KEY_REPEAT      KEY_0_3             //* repeat set / unset 
#define KEY_PARENT      KEY_0_3             //* parent directory 

#define KEY_INC         KEY_1_0             //* + 
#define KEY_PREV        KEY_1_1             //* Previous 
#define KEY_NEXT        KEY_1_2             //* Next 
#define KEY_PLAY        KEY_1_3             //* Play or enter directory 
#define KEY_PAUSE       KEY_1_3             //* Pause 
#define KEY_ENTER       KEY_1_3             //* Enter in menu 

#define KEY_DEC         KEY_2_0             //* - 
#define KEY_REC         KEY_2_1             //* Record 
#define KEY_STOP        KEY_2_2             //* Stop 
#define KEY_MODE        KEY_2_2            //* mode change 


#define KEY_INC_REP     (KEY_INC | KBD_REP_MASK)
#define KEY_DEC_REP     (KEY_DEC | KBD_REP_MASK)
#define KEY_INC_LREP    (KEY_INC | KBD_LREP_MASK)
#define KEY_DEC_LREP    (KEY_DEC | KBD_LREP_MASK)
#define KEY_FR          (KEY_1_1 | KBD_REP_MASK)
#define KEY_FF          (KEY_1_2 | KBD_REP_MASK)
#define KEY_LFR         (KEY_1_1 | KBD_LREP_MASK)
#define KEY_LFF         (KEY_1_2 | KBD_LREP_MASK)
#define KEY_DEL         (KEY_2_1 | KBD_LREP_MASK)


/*_____ D E F I N I T I O N ________________________________________________*/


/*_____ D E C L A R A T I O N ______________________________________________*/


#endif    /* _BOARD_H_ */

