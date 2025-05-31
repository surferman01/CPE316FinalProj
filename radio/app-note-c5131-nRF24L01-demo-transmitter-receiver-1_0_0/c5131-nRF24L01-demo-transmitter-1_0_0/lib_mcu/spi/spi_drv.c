//! @file spi_drv.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the SPI driver routines
//!
//! This file contains the SPI driver routines
//!
//! @version 1.2 c5131-nRF24L01-demo-transmitter-1_0_0 $Id: spi_drv.c,v 1.2 2005/07/08 07:45:23 lguilhau Exp $
//!
//! @todo
//! @bug

//_____ I N C L U D E S ____________________________________________________


#include "config.h"                         /* lib configuration header */
#include "spi_drv.h"                        /* spi driver definition */


//_____ M A C R O S ________________________________________________________

//_____ D E C L A R A T I O N ______________________________________________

//! spi_set_prio.
//!
//!  Set the SPI controller priority interrupt
//!
//! @warning Code:xx bytes (function code length)
//!
//! EXAMPLE:
//!
//! NOTE:
//!

void spi_set_prio (Byte priority)
{
  if ((priority == 1) || (priority == 3))     /* set LSB priority bit */
  {
    IPL1 |=  MSK_ESPI;
  }
  if ((priority == 2) || (priority == 3))     /* set MSB priority bit */
  {
    IPH1 |= MSK_ESPI;
  }
}
