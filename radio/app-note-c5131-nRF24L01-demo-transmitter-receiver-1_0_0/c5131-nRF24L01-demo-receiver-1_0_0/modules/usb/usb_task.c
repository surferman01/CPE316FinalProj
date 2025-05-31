//!
//! @file usb_task.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file manages the USB controller.
//!
//! The USB task checks the income of new requests from the USB Host.
//! When a Setup request occurs, this task will launch the processing
//! of this setup contained in the usb_enum.c file.
//! Other class specific requests are also processed in this file.
//! This file manages all the USB events:
//! Suspend / Resume / Reset / Start Of Frame / Wake Up / Vbus events
//!
//! @version 1.11 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_task.c,v 1.11 2006/05/04 09:17:39 cgasnier Exp $
//!
//! @todo
//! @bug
//!/

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf\conf_usb.h"
#include "usb_task.h"
#include "lib_mcu\usb\usb_drv.h"
#include "modules\usb\usb_enum.h"
#include "modules\scsi_decoder\scsi_decoder.h"
#include "modules\control_access\ctrl_access.h"
#include "intrins.h"
#include "lib_board\nRF24L01\nRF_API.h"
#include "lib_board\nRF24L01\nRF24L01_lib.h"
#include "lib_board\nRF24L01\Protocol_API.h"



//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________

//!
//! Public : (bit) usb_connected
//! usb_connected is set to TRUE when VBUS has been detected
//! usb_connected is set to FALSE otherwise
//!/
volatile bit   usb_connected;
U8 xdata sof_counter;
U8 xdata hid_idle_duration;
volatile U8 data  toggle_next_time;
volatile U8 data  usb_command_running;

static _MEM_TYPE_SLOW_ U8  dCBWTag[4];
static _MEM_TYPE_SLOW_ U8  dCBWDataTransferLength[4];
static bit ms_data_direction;
static U8 usb_mouse_state;
static bit usb_mouse_event;
static U8 usb_mouse_event_stored;
static U8 report_ready;

//static U8  gl_memory;
_MEM_TYPE_SLOW_ U8 usb_LUN;
_MEM_TYPE_SLOW_ U8 usb_mouse_report[4];

extern _MEM_TYPE_SLOW_ U8  usb_configuration_nb;

extern _MEM_TYPE_SLOW_ U8  g_scsi_command[16];
extern _MEM_TYPE_SLOW_ U8  g_scsi_status;
extern _MEM_TYPE_FAST_ U32 g_scsi_data_remaining;
extern bit ms_multiple_drive;

Uchar uc_rf_data;
BYTE IRQ_Source = 0x00; 

void usb_mass_storage_cbw (void);
void usb_mass_storage_csw (void);
void usb_mouse_management (void);
bit  Is_usb_mouse_event   (void);

#define Usb_set_ms_data_direction_in()  (ms_data_direction = 1)
#define Usb_set_ms_data_direction_out() (ms_data_direction = 0)
#define Is_usb_ms_data_direction_in()   (ms_data_direction == 1)

#define Usb_mouse_up()                  (usb_mouse_report[2]--)
#define Usb_mouse_down()                (usb_mouse_report[2]++)
#define Usb_mouse_left()                (usb_mouse_report[1]--)
#define Usb_mouse_right()               (usb_mouse_report[1]++)
#define Usb_mouse_click_left()          (usb_mouse_report[0] |= 0x01)
#define Usb_mouse_click_right()         (usb_mouse_report[0] |= 0x02)
#define Usb_mouse_click_middle()        (usb_mouse_report[0] |= 0x04)
#define Usb_mouse_release_left()        (usb_mouse_report[0] &= ~0x01)
#define Usb_mouse_release_right()       (usb_mouse_report[0] &= ~0x02)
#define Usb_mouse_release_middle()      (usb_mouse_report[0] &= ~0x04)
#define Usb_mouse_scroll_up()           (usb_mouse_report[3]++)
#define Usb_mouse_scroll_down()         (usb_mouse_report[3]--)
#define Usb_mouse_report_reset()        (usb_mouse_report[0]=0,usb_mouse_report[1]=0,usb_mouse_report[2]=0,usb_mouse_report[3]=0)

#define Is_usb_report_ready()           (report_ready)
#define Usb_set_report_not_ready()      (report_ready = 0)
#define Usb_set_report_ready()          (report_ready = 1)

//_____ D E C L A R A T I O N S ____________________________________________
//!
//! @brief This function initializes the USB the associated variables.
//!
//! This function enables the USB controller and init the USB interrupts.
//! The aim is to allow the USB connection detection in order to send
//! the appropriate USB event to the operating mode manager.
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
//!/
void usb_task_init(void)
{
   ms_multiple_drive = 0;
   Set_page_usb();
   Usb_enable();                      // enable the USB controller
   usb_start_device();
   sof_counter = 0;
   usb_connected = TRUE;
   usb_mouse_state = 0;
   Usb_mouse_report_reset();
   usb_mouse_event = FALSE;
   usb_mouse_event_stored = 0;
   report_ready = 1;
   toggle_next_time = 0;
   usb_command_running = 0;

}



//!
//! @brief This function initializes the USB device controller
//!
//! This function enables the USB controller and init the USB interrupts.
//! The aim is to allow the USB connection detection in order to send
//! the appropriate USB event to the operating mode manager.
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
//!
//!/
void usb_start_device (void)
{
U16 c;
   usb_generate_clock();
   Set_page_usb(); 
   Usb_unfreeze_clock();
   Usb_set_speed();             // for speed analysis
   Usb_ack_suspend();
   Usb_detach();
   for (c=0; c<1000; c++);
   Usb_attach();
   Usb_enable_suspend_interrupt();
   Usb_enable_resume_interrupt();
   Usb_enable_reset_interrupt();
   Usb_enable_wake_up_interrupt();
   Usb_enable_sof_interrupt();
   Enable_usb_interrupt();   
   Enable_interrupt();
   usb_init_device();         // configure the USB controller EP0

   Usb_attach();
}

//! @brief Entry point of the USB mamnagement
//!
//! This function is the entry point of the USB management. Each USB
//! event is checked here in order to launch the appropriate action.
//! If a Setup request occurs on the Default Control Endpoint,
//! the usb_process_request() function is call in the usb_enum.c file
//! If a new USB mass storage Command Block Wrapper (CBW) occurs,
//! this one will be decoded and the SCSI command will be taken in charge
//! by the scsi decoder.
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
void usb_task(void)
{
   if (!usb_connected)         /* power down mode when USB not connected */
   { 
      Pll_stop();           /* disable PLL before going in power down mode */
      Enable_interrupt();

      // Shutdown board ressources
      Led_0_off();
      Disable_ale();
      if (!usb_connected)
      {
         Usb_suspend_action();    // see in conf_usb.h
         _nop_();
		}
   }
   else //USB Connected
   {
    // USB MANAGEMENT

    Set_page_usb ();

    Usb_select_endpoint(EP_CONTROL);
    if (Is_usb_receive_setup())
    {
      usb_process_request();
    }  
 //!
 //! this part of the code can be modify following the application
 //!
 /*   Usb_select_endpoint(EP_MS_OUT);
    if (Is_usb_receive_out())
    { 
      usb_command_running = 1;
      usb_mass_storage_cbw();
      usb_mass_storage_csw();
      usb_command_running = 0;
    }No MS*/

 //!
 //! end of modification
 //!
   }
}


//! @brief USB Command Block Wrapper (CBW) management
//!
//! This function decodes the CBW command and stores the SCSI command
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
void usb_mass_storage_cbw (void)
{
bit cbw_error;
U8  c;

   cbw_error = FALSE;
   Usb_select_endpoint(EP_MS_OUT);           //! check if dCBWSignature is correct
   if (0x55 != Usb_read_byte()) { cbw_error = TRUE; } //! 'U'
   if (0x53 != Usb_read_byte()) { cbw_error = TRUE; } //! 'S'
   if (0x42 != Usb_read_byte()) { cbw_error = TRUE; } //! 'B'
   if (0x43 != Usb_read_byte()) { cbw_error = TRUE; } //! 'C'
             
   dCBWTag[0] = Usb_read_byte();             //! Store CBW Tag to be repeated in CSW
   dCBWTag[1] = Usb_read_byte();
   dCBWTag[2] = Usb_read_byte();
   dCBWTag[3] = Usb_read_byte();
 
   ((U8*)&g_scsi_data_remaining)[3] = Usb_read_byte();
   ((U8*)&g_scsi_data_remaining)[2] = Usb_read_byte();
   ((U8*)&g_scsi_data_remaining)[1] = Usb_read_byte();
   ((U8*)&g_scsi_data_remaining)[0] = Usb_read_byte();
 
   if (Usb_read_byte() != 0x00)              //! if (bmCBWFlags.bit7 == 1) {direction = IN}
   {
      Usb_set_ms_data_direction_in();
      if (cbw_error)
      {
         Usb_ack_receive_out_ms();
         Usb_select_endpoint(EP_MS_IN);
         Usb_enable_stall_handshake();
         return;
      }
   }
   else
   {
      Usb_set_ms_data_direction_out();
      if (cbw_error)
      {
         Usb_enable_stall_handshake();
         Usb_ack_receive_out_ms();
         return;
      }
   }
 
   usb_LUN = Usb_read_byte();

   if (!ms_multiple_drive)
   {
// SG
      usb_LUN = get_cur_lun();
   }
 
   ACC      = Usb_read_byte();                  //! dummy CBWCBLength read
 
 
   for (c=0; c<16; c++)                         // store scsi_command
   {
      g_scsi_command[c] = Usb_read_byte();
   }
 
   Usb_ack_receive_out_ms();
 
   if (Is_usb_ms_data_direction_in())
   {
      Usb_select_endpoint(EP_MS_IN);
   }
 
   if (TRUE != scsi_decode_command())
   {
      if (g_scsi_data_remaining != 0)
      {
         Usb_enable_stall_handshake();
      }
   }
}

//! @brief USB Command Status Wrapper (CSW) management
//!
//! This function sends the status in relation with the last CBW
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
void usb_mass_storage_csw (void)
{
   Usb_select_endpoint(EP_MS_IN);
   while (Is_usb_endpoint_stall_requested())
   {
      Usb_select_endpoint(EP_CONTROL);
      if (Is_usb_receive_setup())       { usb_process_request(); }
      Usb_select_endpoint(EP_MS_IN);
   }
 
   Usb_select_endpoint(EP_MS_OUT);
   while (Is_usb_endpoint_stall_requested())
   {
      Usb_select_endpoint(EP_CONTROL);
      if (Is_usb_receive_setup())       { usb_process_request(); }
      Usb_select_endpoint(EP_MS_OUT);
   }
   Usb_select_endpoint(EP_MS_IN);
   while(!Is_usb_write_enabled());
                                                         //! write CSW Signature
   Usb_write_byte(0x55);                                 //! 'U'
   Usb_write_byte(0x53);                                 //! 'S'
   Usb_write_byte(0x42);                                 //! 'B'
   Usb_write_byte(0x53);                                 //! 'S'
                                                         //! write stored CBW Tag
   Usb_write_byte(dCBWTag[0]);
   Usb_write_byte(dCBWTag[1]);
   Usb_write_byte(dCBWTag[2]);
   Usb_write_byte(dCBWTag[3]);
                                                         //! write data residue value
   Usb_write_byte( ((Byte*)&g_scsi_data_remaining)[3] );
   Usb_write_byte( ((Byte*)&g_scsi_data_remaining)[2] );
   Usb_write_byte( ((Byte*)&g_scsi_data_remaining)[1] );
   Usb_write_byte( ((Byte*)&g_scsi_data_remaining)[0] );
   
   //! write command status
   Usb_write_byte(g_scsi_status);                        //! 0 -> PASS, 1 -> FAIL
 
   Usb_send_in();
}

//! @brief Is_usb_mouse_event
//!
//! This function returns TRUE if a event occurs on the mouse
//! (FALSE if not). The mouse report is updated by this function
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
bit Is_usb_mouse_event(void)
{
  Uchar dummy;
  Uchar RX_pload[1];

  Usb_mouse_report_reset();

  //a mouse message arrived
  if (IRQ_Source == RX_DR) 
  {
    //Read FIFO with mouse message
    SPI_Read_Buf(RD_RX_PLOAD, RX_pload, 1);

	//clear IT
    L01_Clear_IRQ(MASK_IRQ_FLAGS);

    uc_rf_data = RX_pload[0];

    dummy = uc_rf_data;
    if ((dummy & MOUSE_MSK_SET_LEFTC) == 0x00)  // c_left   
      Usb_mouse_click_left();
  
    if ((dummy & MOUSE_MSK_SET_RIGHTC) == 0x00)  // c_right
      Usb_mouse_click_right();
  
    if ((dummy & MOUSE_MSK_SET_LEFT) == 0x00)  // left
	  usb_mouse_report[1] = MOUSE_MOVE_NEG;
    else if ((dummy & MOUSE_MSK_SET_RIGHT) == 0x00)  // right
	  usb_mouse_report[1] = MOUSE_MOVE_POS;
    else 
	  usb_mouse_report[1] = MOUSE_MOVE_STOP;
  
    if ((dummy & MOUSE_MSK_SET_DOWN) == 0x00)  // down
      usb_mouse_report[2] = MOUSE_MOVE_POS;
    else if ((dummy & MOUSE_MSK_SET_UP) == 0x00)  // up
      usb_mouse_report[2] = MOUSE_MOVE_NEG;

    else
      usb_mouse_report[2] = MOUSE_MOVE_STOP;

    return TRUE;
  }
  
  return FALSE;
}

//! @brief USB interrupt process
//!
//! This function is called each time a USB interrupt occurs.
//! The following USB events are taken in charge:
//! - Suspend
//! - Wake-Up
//! - Reset
//! For each event, the user can launch an action by completing
//! the associate define
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
void usb_interrupt(void) interrupt IRQ_USB
{
U8 data endpoint_stored;

   if (Is_usb_sof())
   {
      Usb_ack_sof();
      sof_counter++;
      if (usb_command_running == 1) 
      {
         if (sof_counter == 0x40)
         {
            Led_0_toggle();
            sof_counter = 0;
         }
      }
      else
      {
         if (sof_counter == 0xFF)
         {
            Led_0_toggle();
         }
      }
      if (usb_configuration_nb != 0)
      {
         if (Is_usb_report_ready())
         {
            if (Is_usb_mouse_event())
            {
               endpoint_stored = UEPNUM;
               Usb_set_report_not_ready();
               Usb_select_endpoint(EP_MOUSE_IN);
               Usb_write_byte(usb_mouse_report[0]);
               Usb_write_byte(usb_mouse_report[1]);
               Usb_write_byte(usb_mouse_report[2]);
               Usb_write_byte(usb_mouse_report[3]);
               Usb_send_control_in();
               Usb_select_endpoint(endpoint_stored);
            }
         }
         else
         {
            endpoint_stored = UEPNUM;
            Usb_select_endpoint(EP_MOUSE_IN);
            if (Is_usb_in_ready())
            {
               Usb_ack_in_ready();
               Usb_set_report_ready();
            }
            Usb_select_endpoint(endpoint_stored);
         }
         return;
      }
   }

  if (Is_usb_suspend())
  {
    Usb_ack_suspend();
    Usb_freeze_clock();
    usb_connected = FALSE;
  }
  
  if (Is_usb_wake_up())
  {
    if (!usb_connected)      /* reconfigure PLL when exiting power down mode */
    {
       Pll_enable();
    }
    Usb_unfreeze_clock();
    Enable_ale();
    Usb_ack_suspend();
	 Usb_ack_wake_up();   	    
	 usb_connected = TRUE;
  }  
  
  if (Is_usb_reset()) 
  {  
     Usb_ack_reset();
     usb_enum_var_init();
     Usb_enable_suspend_interrupt();
     Usb_enable_wake_up_interrupt();
     Usb_enable_sof_interrupt();
     usb_connected = TRUE;
     ms_multiple_drive = FALSE;            /* multiple disk support init */
     Led_0_on();
  }
}




