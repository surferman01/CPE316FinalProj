/*H**************************************************************************
* NAME:         nf.h         
*----------------------------------------------------------------------------
* Copyright (c) 2004 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      snd1c-demo-nf-mmc-4_6_0      
* REVISION:     1.12     
*----------------------------------------------------------------------------
* PURPOSE:        This file contains the high level NF definitions
*
*****************************************************************************/

#ifndef _NF_H_
#define _NF_H_

/*_____ I N C L U D E S ____________________________________________________*/

#include "nf_drv.h"              /* NF driver definition */


/*_____ M A C R O S ________________________________________________________*/


#define Usb_tx_complete()      Is_usb_in_ready()             //compatibility
#define Usb_rx_complete()      Is_usb_receive_out()         //compatibility
#define Usb_clear_TXCMPL()     Usb_ack_in_ready()           //compatibility
#define Usb_set_TXRDY()        Usb_send_control_in()        //compatibility
#define Usb_clear_RXOUT_PP()   Usb_ack_receive_out_ms()     //compatibility

/*_____ D E F I N I T I O N ________________________________________________*/
/* Number of page */
#define Nf_nb_page()              ((Uint32)(nf_zone_max * 1024) * (Uint16)(NF_PAGE_PER_BLOCK))
/* Disk size : number of block - 1 */
#define Nf_disk_size()            ((Uint32)(NF_DISK_SIZE))
#define Nf_disk_size_0()          ((Byte)(NF_DISK_SIZE >> 24))
#define Nf_disk_size_1()          ((Byte)(NF_DISK_SIZE >> 16))
#define Nf_disk_size_2()          ((Byte)(NF_DISK_SIZE >> 8))
#define Nf_disk_size_3()          ((Byte)(NF_DISK_SIZE))

/* Block size : size of a block in byte */
#define Nf_block_size()           ((Uint32)(NF_BLOCK_SIZE))
#define Nf_block_size_0()         ((Byte)((NF_BLOCK_SIZE) >> 24))
#define Nf_block_size_1()         ((Byte)((NF_BLOCK_SIZE) >> 16))
#define Nf_block_size_2()         ((Byte)((NF_BLOCK_SIZE) >> 8))
#define Nf_block_size_3()         ((Byte)(NF_BLOCK_SIZE))


/*_____ D E C L A R A T I O N ______________________________________________*/



/* System */
bit     nf_init (void);                  /* Init NF card                   */
bit     nf_read_open (Uint32);           /* Set read mode                  */
void    nf_read_close (void);            /* Close read mode                */
Byte    nf_read_byte (void);             /* Read 1 byte                    */
bit     nf_write_open (Uint32);          /* Set write mode                 */
bit     nf_write_close (void);           /* Close write mode               */
bit     nf_write_byte (Byte);            /* Write 1 byte                   */
bit     nf_write_n (Byte);               /* Write n bytes  from USB        */
bit     nf_block_erase(Uint32);          /* Erase one block (32 pages)     */
bit     hard_copy_mmc_to_nf(void);       /* Copy all bytes from MMC        */
bit     nf_erase_all_block(void);        /* Erase all blocks of the card   */
bit     nf_check_status(void);           /* Check the status after program */
                                         /* or erase block command         */
bit     nf_write_sector(Uint16);          /* Write a sector of 512b from USB*/
bit     nf_read_sector(Uint16);           /* Read a sector of 512b to USB   */

void    nf_calculate_ecc(void);          /* Calculate ECC                  */
s_format  * nf_format (void);       /* Prepare format of SmartMedia   */
#endif /* _NF_H_ */
