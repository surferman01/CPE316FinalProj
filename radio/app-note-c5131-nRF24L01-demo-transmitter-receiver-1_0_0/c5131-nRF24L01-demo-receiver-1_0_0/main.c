//! @file main.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//! @brief Main program for SND3
//!
//! @version 1.2 c5131-nRF24L01-demo-receiver-1_0_0 $Id: main.c,v 1.2 2006/05/04 13:27:15 cgasnier Exp $
//!
//! @todo
//! @bug

//_____ I N C L U D E S ____________________________________________________
#include "config.h"                             // System definition

#ifdef nRF24L01_RECEIVER
#include "conf\conf_usb.h"
#endif

#ifdef nRF24L01
#include "lib_mcu\spi\spi_drv.h"
#endif


//_____ M A C R O S ________________________________________________________
//_____ D E F I N I T I O N ________________________________________________
//_____ D E C L A R A T I O N ______________________________________________
extern  void    scheduler        (void);
        void    main             (void);


#ifdef nRF24L01_TRANSMITTER

#define MOUSE_LEFTC    (P3_3 == 0)
#define MOUSE_RIGHTC   (P1_3 == 0)
#define MOUSE_LEFT     (P3_7 == 0)
#define MOUSE_RIGHT    (P4_0 == 0)
#define MOUSE_DOWN     (P3_6 == 0)
#define MOUSE_UP       (P3_5 == 0)

#define MOUSE_MSK_SET_LEFTC    0x20
#define MOUSE_MSK_SET_RIGHTC   0x10
#define MOUSE_MSK_SET_LEFT     0x08
#define MOUSE_MSK_SET_RIGHT    0x04
#define MOUSE_MSK_SET_DOWN     0x02
#define MOUSE_MSK_SET_UP       0x01

#define MOUSE_MSK_CLR_LEFTC    0x1F
#define MOUSE_MSK_CLR_RIGHTC   0x2F
#define MOUSE_MSK_CLR_LEFT     0x37
#define MOUSE_MSK_CLR_RIGHT    0x3B
#define MOUSE_MSK_CLR_DOWN     0x3D
#define MOUSE_MSK_CLR_UP       0x3E

#define MOUSE_CMD_STOP         0x3F

/*F**************************************************************************
* NAME: mouse_process
*----------------------------------------------------------------------------
* PARAMS:   
* return: none
*----------------------------------------------------------------------------
* PURPOSE: Wait for a mouse event to send info via RF
* 
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void mouse_process (void)
{
  Uchar uc_count;
  Uchar uc_command;
  Uchar uc_command_old;

  uc_command = MOUSE_CMD_STOP;

  do{
    //wait for an event on mouse
    do{
      if (MOUSE_LEFTC)
        uc_command &= MOUSE_MSK_CLR_LEFTC;
      else
        uc_command |= MOUSE_MSK_SET_LEFTC;

      if (MOUSE_RIGHTC)
        uc_command &= MOUSE_MSK_CLR_RIGHTC;
      else
        uc_command |= MOUSE_MSK_SET_RIGHTC;
	     
      if (MOUSE_LEFT)
        uc_command &= MOUSE_MSK_CLR_LEFT;
      else
        uc_command |= MOUSE_MSK_SET_LEFT;

      if (MOUSE_RIGHT)
        uc_command &= MOUSE_MSK_CLR_RIGHT;
      else
        uc_command |= MOUSE_MSK_SET_RIGHT;

      if (MOUSE_DOWN)
        uc_command &= MOUSE_MSK_CLR_DOWN;
      else
        uc_command |= MOUSE_MSK_SET_DOWN;

      if (MOUSE_UP)
        uc_command &= MOUSE_MSK_CLR_UP;
      else
        uc_command |= MOUSE_MSK_SET_UP;

    }while (uc_command == uc_command_old);

    // wait
    uc_count = 0xFF;
	do{
      uc_count = uc_count - 1;
    }while(uc_count > 1);   

    nRF24L01_send_byte (uc_command);
    
    uc_command_old = uc_command;
  }while(1); 
}

#endif

/*F**************************************************************************
* NAME: main
*----------------------------------------------------------------------------
* PARAMS:   
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* In transmitter mode, wait for a mouse event
* In receiver mode, use of scheduler module
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void main (void)
{
  Set_x2_mode();

  //open COM
  open();

#ifdef nRF24L01_TRANSMITTER
  mouse_process();
#endif

#ifdef nRF24L01_RECEIVER
  Configure_led(0,LED_PORT_10MA);
  Led_0_off();

  // Endless scheduler execution
  scheduler();
#endif
}








