/**
 * @file usb_task.h,v
 *
 * Copyright (c) 2004 Atmel.
 *
 * Please read file license.txt for copyright notice.
 *
 * @brief This file contains the function declarations
 *
 * @version 1.2 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_task.h,v 1.2 2006/05/04 09:17:33 cgasnier Exp $
 *
 * @todo
 * @bug
 */

#ifndef _USB_TASK_H_
#define _USB_TASK_H_

//_____ I N C L U D E S ____________________________________________________


//_____ M A C R O S ________________________________________________________

#define Set_page_usb()       //Mcu_set_sfr_page_usb()

#ifdef nRF24L01 
//_____ D E F I N I T I O N ________________________________________________
#define MOUSE_MSK_SET_LEFTC    0x20
#define MOUSE_MSK_SET_RIGHTC   0x10
#define MOUSE_MSK_SET_LEFT     0x08
#define MOUSE_MSK_SET_RIGHT    0x04
#define MOUSE_MSK_SET_DOWN     0x02
#define MOUSE_MSK_SET_UP       0x01

#define MOUSE_MOVE_NEG         0xFD
#define MOUSE_MOVE_POS         0x03
#define MOUSE_MOVE_STOP        0x00
#endif  // nRF24L01

//_____ D E C L A R A T I O N S ____________________________________________

void usb_task_init     (void);
void usb_start_device  (void);
void usb_task          (void);


#endif /* _USB_TASK_H_ */

