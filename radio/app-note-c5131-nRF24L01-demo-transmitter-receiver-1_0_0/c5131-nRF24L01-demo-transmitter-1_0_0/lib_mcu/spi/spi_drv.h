//! @file spi_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the SPI driver definition
//!
//! @version 1.4 c5131-nRF24L01-demo-transmitter-1_0_0 $Id: spi_drv.h,v 1.4 2006/05/02 14:34:50 cgasnier Exp $
//!
//! @todo
//! @bug

#ifndef _SPI_DRV_H_
#define _SPI_DRV_H_

//_____ I N C L U D E S ____________________________________________________


//_____ M A C R O S ________________________________________________________

//----- Modes -----
#define SPI_MASTER_MODE_0 (Byte)(MSK_MSTR)
#define SPI_MASTER_MODE_1 (Byte)(MSK_MSTR|0x04)
#define SPI_MASTER_MODE_2 (Byte)(MSK_MSTR|0x08)
#define SPI_MASTER_MODE_3 (Byte)(MSK_MSTR|0x0C)

#define SPI_SLAVE_MODE_0  (Byte)0x00
#define SPI_SLAVE_MODE_1  (Byte)0x04
#define SPI_SLAVE_MODE_2  (Byte)0x08
#define SPI_SLAVE_MODE_3  (Byte)0x0C

//----- Bit rates -----
#define SPI_RATE_0        (Byte)0x00    // Fper / 2
#define SPI_RATE_1        (Byte)0x01    // Fper / 4
#define SPI_RATE_2        (Byte)0x02    // Fper / 8
#define SPI_RATE_3        (Byte)0x03    // Fper / 16
#define SPI_RATE_4        (Byte)0x80    // Fper / 32
#define SPI_RATE_5        (Byte)0x81    // Fper / 64
#define SPI_RATE_6        (Byte)0x82    // Fper / 128


//_____ D E F I N I T I O N ________________________________________________

#ifdef nRF24L01
  #define open() nRF24L01_init()              
  #define putc(Uchar) Spi_write_data(Uchar) //nRF24L01_send_data(Uchar)  
  #define getc(Uchar) Spi_write_data(Uchar) //nRF24L01_receive_data(Uchar)  
#endif

//_____ D E C L A R A T I O N ______________________________________________

void    spi_set_prio (Byte);

#define Spi_enable()            (SPCON |= MSK_SPEN)
#define Spi_disable()           (SPCON &= ~MSK_SPEN)
#define Spi_enable_int()        (IEN1 |= MSK_ESPI)
#define Spi_disable_int()       (IEN1 &= ~MSK_ESPI)
#define Spi_enable_ss()         (SPCON &= ~MSK_SSDIS)
#define Spi_disable_ss()        (SPCON |= MSK_SSDIS)
#define Spi_set_mode(mode)      (SPCON &= ~(MSK_MSTR|MSK_MODE)); (SPCON |= mode)
#define Spi_set_rate(rate)      (SPCON &= ~MSK_SPR); (SPCON |= rate)
#define Spi_read_data()         (SPDAT)
#define Spi_write_data(b)       (SPDAT = b)
#define Spi_write_dummy()       (SPDAT = ACC);(ACC = SPSTA)
#define Spi_read_dummy()        (ACC = SPDAT)
#define Spi_wait_spif()         while ((SPSTA & MSK_SPIF) == 0) // for any SPI_RATE_x
#define Spi_wait_eor()          while ((SPSTA & MSK_SPIF) == 0) // wait end of reception
#define Spi_wait_eot()          while ((SPSTA & MSK_SPIF) == 0) // wait end of transmission
#define Spi_eor()               ((SPSTA & MSK_SPIF) == MSK_SPIF)// check end of reception
#define Spi_eot()               ((SPSTA & MSK_SPIF) == MSK_SPIF)// check end of transmission
#define Spi_ack_read()          (SPSTA = SPSTA);(SPSTA = SPSTA)  // for SPI_RATE_0 only
#define Spi_ack_cmd()           (SPSTA = SPSTA);(SPSTA = SPSTA)  // for SPI_RATE_0 only
#define Spi_ack_write()         (SPSTA = SPSTA);(SPSTA = SPSTA)                 // for SPI_RATE_0 only


#endif  // _SPI_DRV_H_

