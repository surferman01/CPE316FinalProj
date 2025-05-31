//! @file config.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the system configuration definition
//!
//! @version 1.1 c5131-nRF24L01-demo-transmitter-1_0_0 $Id: config.h,v 1.1 2006/05/02 13:56:53 cgasnier Exp $
//!
//! @todo
//! @bug

#ifndef _CONFIG_H_
#define _CONFIG_H_


//_____ I N C L U D E S ____________________________________________________

#include "lib_mcu\compiler.h"                // Compiler definitions
#include "lib_mcu\mcu.h"                     // Register declaration
#include "lib_mcu\mcu_drv.h"                 // Component declaration


//_____ D E F I N I T I O N ________________________________________________
#define  FOSC       16000

#include "lib_board\nRF24L01\defines.h"  
#include "lib_board\nRF24L01\nRF24L01_lib.h"  
#include "lib_board\nRF24L01\LL_API_C51.h"  

#define nRF24L01

//#define nRF24L01_RECEIVER
#define nRF24L01_TRANSMITTER

#define CE P2_2
#define CSN P2_1

//_____ M A C R O S ________________________________________________________


#endif  //! _CONFIG_H_

