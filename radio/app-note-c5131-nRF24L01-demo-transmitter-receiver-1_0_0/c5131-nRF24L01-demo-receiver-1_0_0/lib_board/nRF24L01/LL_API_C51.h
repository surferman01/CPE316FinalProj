//! @file LL_API_C51.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level macros and definition for the
//! ATR2406 RF module.
//!
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0
//!
//! @todo
//! @bug


#ifndef _LL_API_C51_H_
#define _LL_API_C51_H_

//_____ I N C L U D E S ____________________________________________________

//_____ M A C R O S ________________________________________________________

/*_____ D E F I N I T I O N ________________________________________________*/

// CE_Action definitions
#define CE_LOW    0x00
#define CE_HIGH   0x01


//_____ D E F I N I T I O N S ______________________________________________

void nRF24L01_reset(void);
void nRF24L01_tx_config(void);
void nRF24L01_rx_config(void);

void  C5131_enable_it(void);

BYTE SPI_HW(BYTE byte);
void SPI_Init(BYTE Mode);
void CSN_Pin(BYTE state);
void CE_Pin(BYTE action);    

#endif
