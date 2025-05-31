//! @file nRF24L01_lib.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level macros and definition for the
//! ATR2406 RF module.
//!
//! @version 1.1 c5131-nRF24L01-demo-transmitter-1_0_0
//!
//! @todo
//! @bug


#ifndef _nRF24L01_LIB_H_
#define _nRF24L01_LIB_H_

//_____ I N C L U D E S ____________________________________________________

//_____ M A C R O S ________________________________________________________

/*_____ D E F I N I T I O N ________________________________________________*/

//_____ D E F I N I T I O N S ______________________________________________


void  nRF24L01_init(void);
void nRF24L01_send_byte (Uchar uc_command);
Uchar nRF24L01_receive_byte (void);

#endif
