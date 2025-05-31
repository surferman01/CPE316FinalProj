//! @file usb_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the USB low level driver definition 
//!
//! @version 1.21 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_drv.h,v 1.21 2006/04/06 12:57:57 cgasnier Exp $
//!
//! @todo
//! @bug

#ifndef _USB_DRV_H_
#define _USB_DRV_H_

//_____ I N C L U D E S ____________________________________________________
#include "conf\conf_usb.h"

typedef enum endpoint_parameter{ep_num, ep_type, ep_direction, ep_size, ep_bank, nyet_status} t_endpoint_parameter;

#ifndef NO_SUPPORT_USB_PING_PONG
   extern  bdata bit   rx_bank;
#endif

//_____ M A C R O S ________________________________________________________

#define EP_CONTROL            0
#define EP_1                  1
#define EP_2                  2
#define EP_3                  3
#define EP_4                  4
#define EP_5                  5
#define EP_6                  6
#define EP_7                  7

#define MSK_EP_DIR            0x7F

//! Parameters for endpoint configuration
//! These define are the values used to enable and configure an endpoint.
#define TYPE_CONTROL             0
#define TYPE_ISOCHRONOUS         1
#define TYPE_BULK                2
#define TYPE_INTERRUPT           3

#define DIRECTION_OUT            0
#define DIRECTION_IN             1

#define SIZE_8                   0
#define SIZE_16                  1
#define SIZE_32                  2
#define SIZE_64                  3
#define SIZE_128                 4
#define SIZE_256                 5
#define SIZE_512                 6
#define SIZE_1024                7

#define ONE_BANK                 0
#define TWO_BANKS                1

#define NYET_ENABLED             0
#define NYET_DISABLED            1

#define Usb_build_ep_config0(type, dir, nyet)  ((type<<6) | (nyet<<1) | (dir))
#define Usb_build_ep_config1(size, bank     )  ((size<<4) | (bank<<2)        )
#define usb_configure_endpoint(num, type, dir, size, bank, nyet)             \
                                    ( Usb_select_endpoint(num),              \
                                      usb_config_ep(Usb_build_ep_config0(type, dir, nyet),\
                                                    Usb_build_ep_config1(size, bank)    ))


//! General USB management 
//! These macros manage the USB controller.

#define Usb_enable()                  (USBCON |= MSK_USBE)
#define Usb_disable()                 (USBCON &= ~MSK_USBE)
#define Usb_select_device()           // compatibility
#define Usb_select_host()             // compatibility
#define Usb_freeze_clock()            (USBCON  |=  MSK_SUSPCLK)
#define Usb_unfreeze_clock()          (USBCON  &= ~MSK_SUSPCLK)
#define Usb_enable_id_interrupt()     // compatibility
#define Usb_disable_id_interrupt()    // compatibility
#define Usb_enable_vbus_interrupt()   // compatibility
#define Usb_disable_vbus_interrupt()  // compatibility
#define Is_usb_enabled()              (USBCON & MSK_USBE == MSK_USBE)
#define Is_usb_host_enabled()         (FALSE)// compatibility
#define Is_usb_clock_freezed()        (USBCON & MSK_SUSPCLK == MSK_SUSPCLK)

#define Is_usb_vbus_high()            (TRUE)// compatibility
#define Is_usb_high_speed()           (FALSE)// compatibility
#define Is_usb_id_device()            (TRUE)// compatibility

#define Usb_ack_id_transition()       // compatibility
#define Usb_ack_vbus_transition()     // compatibility
#define Is_usb_id_transition()        (FALSE) // compatibility
#define Is_usb_vbus_transition()      (FALSE) // compatibility

#define Usb_enter_dpram_mode()        // compatibility
#define Usb_exit_dpram_mode()         // compatibility
#define Usb_set_dpram_address(addr)   // compatibility

#define Usb_enable_vbus()             // compatibility
#define Usb_disable_vbus()            // compatibility
#define Usb_device_initiate_hnp()     // compatibility
#define Usb_host_accept_hnp()         // compatibility
#define Usb_host_reject_hnp()         // compatibility
#define Usb_device_initiate_srp()     // compatibility
#define Usb_select_vbus_srp_method()  // compatibility
#define Usb_select_data_srp_method()  // compatibility
#define Usb_enable_vbus_hw_control()  // compatibility
#define Usb_disable_vbus_hw_control() // compatibility
#define Is_usb_vbus_enabled()         (TRUE)  // compatibility
#define Is_usb_hnp()                  (FALSE) // compatibility
#define Is_usb_device_srp()           (FALSE) // compatibility

#define Usb_enable_suspend_time_out_interrupt()   // compatibility
#define Usb_enable_hnp_error_interrupt()          // compatibility
#define Usb_enable_role_exchange_interrupt()      // compatibility
#define Usb_enable_bconnection_error_interrupt()  // compatibility
#define Usb_enable_vbus_error_interrupt()         // compatibility
#define Usb_enable_srp_interrupt()                // compatibility
#define Usb_disable_suspend_time_out_interrupt()  // compatibility
#define Usb_disable_hnp_error_interrupt()         // compatibility
#define Usb_disable_role_exchange_interrupt()     // compatibility
#define Usb_disable_bconnection_error_interrupt() // compatibility
#define Usb_disable_vbus_error_interrupt()        // compatibility
#define Usb_disable_srp_interrupt()               // compatibility

#define Usb_ack_suspend_time_out_interrupt()      // compatibility
#define Usb_ack_hnp_error_interrupt()             // compatibility
#define Usb_ack_role_exchange_interrupt()         // compatibility
#define Usb_ack_bconnection_error_interrupt()     // compatibility
#define Usb_ack_vbus_error_interrupt()            // compatibility
#define Usb_ack_srp_interrupt()                   // compatibility
#define Is_usb_suspend_time_out_interrupt()       (FALSE) // compatibility
#define Is_usb_hnp_error_interrupt()              (FALSE) // compatibility
#define Is_usb_role_exchange_interrupt()          (FALSE) // compatibility
#define Is_usb_bconnection_error_interrupt()      (FALSE) // compatibility
#define Is_usb_vbus_error_interrupt()             (FALSE) // compatibility
#define Is_usb_srp_interrupt()                    (FALSE) // compatibility


//! USB Device management
//! These macros manage the USB Device controller.
//!

#define Usb_initiate_remote_wake_up()             (USBCON   |=  MSK_SDRMWKUP)
#define Usb_detach()                              (USBCON   |=  MSK_DETACH)
#define Usb_attach()                              (USBCON   &= ~MSK_DETACH)
#define Is_usb_pending_remote_wake_up()           (USBCON & MSK_UPRSM == MSK_UPRSM)
#define Is_usb_detached()                         (USBCON & MSK_DETACH == MSK_DETACH)

#define Usb_ack_remote_wake_up_start()            (USBCON &= ~MSK_SDRMWUP)
#define Usb_ack_resume()                          (USBINT &= ~MSK_WUPCPU)
#define Usb_ack_wake_up()                         (USBINT &= ~MSK_WUPCPU)
#define Usb_ack_reset()                           (USBINT &= ~MSK_EORINT)
#define Usb_ack_sof()                             (USBINT &= ~MSK_SOFINT)
#define Usb_ack_micro_sof()                       // compatibility
#define Usb_ack_suspend()                         (USBINT &= ~MSK_SPINT)
#define Is_usb_remote_wake_up_start()             (USBCON & MSK_SDRMWUP)
#define Is_usb_resume()                           (USBINT & MSK_WUPCPU)
#define Is_usb_wake_up()                          (USBINT & MSK_WUPCPU)
#define Is_usb_reset()                            (USBINT & MSK_EORINT)
#define Is_usb_sof()                              (USBINT & MSK_SOFINT)
#define Is_usb_micro_sof()                        (FALSE) // compatibiliy
#define Is_usb_suspend()                          (USBINT & MSK_SPINT == MSK_SPINT)

#define Usb_enable_remote_wake_up_interrupt()     // compatibility
#define Usb_enable_resume_interrupt()             (USBIEN  |=  MSK_EWUPCPU)
#define Usb_enable_wake_up_interrupt()            (USBIEN  |=  MSK_EWUPCPU)
#define Usb_enable_reset_interrupt()              (USBIEN  |=  MSK_EEORINT)
#define Usb_enable_sof_interrupt()                (USBIEN  |=  MSK_ESOFINT)
#define Usb_enable_micro_sof_interrupt()          // compatibility
#define Usb_enable_suspend_interrupt()            (USBIEN  |=  MSK_ESPINT)
#define Usb_disable_remote_wake_up_interrupt()    // compatibility
#define Usb_disable_resume_interrupt()            (USBIEN  &= ~MSK_EWUPCPU)
#define Usb_disable_wake_up_interrupt()           (USBIEN  &= ~MSK_EWUPCPU)
#define Usb_disable_reset_interrupt()             (USBIEN  &= ~MSK_EEORINT)
#define Usb_disable_sof_interrupt()               (USBIEN  &= ~MSK_ESOFINT)
#define Usb_disable_micro_sof_interrupt()         // compatibility
#define Usb_disable_suspend_interrupt()           (USBIEN  &= ~MSK_ESPINT)

#define Usb_enable_address()                      (USBADDR  |=  MSK_FEN)
#define Usb_disable_address()                     (USBADDR  &= ~MSK_FEN)
#define Usb_configure_address(addr)               (Usb_disable_address(), USBADDR  = (U8)addr)

#define Usb_frame_number()                        ((U16)((((U16)UFNUMH & 0x07) << 8) | ((U16)UDNUML)))
#define Usb_micro_frame_number()                  (0) // compatibility
#define Is_usb_frame_number_crc_error()           (UFNUMH & MSK_CRCERR == MSK_CRCERR)

#define Usb_enter_test_packet_mode()              // compatibility
#define Usb_enter_test_k_mode()                   // compatibility
#define Usb_enter_test_j_mode()                   // compatibility
#define Usb_force_normal_speed()                  // compatibility
#define Usb_force_full_speed()                    // compatibility
#define Usb_force_high_speed()                    // compatibility
#define Usb_force_full_speed_utmi()               // compatibility
#define Usb_exit_test_packet_mode()               // compatibility
#define Usb_exit_test_k_mode()                    // compatibility
#define Usb_exit_test_j_mode()                    // compatibility

#define Usb_set_loopback_mode()                   // compatibility

//! General endpoint management 
//! These macros manage the common features of the endpoints
//!

#define Usb_select_endpoint(ep)                   (UEPNUM = (U8)ep)
#define Usb_select_endpoint_dfc(ep)               (UEPNUM = (U8)ep) // compatibility

#define Usb_reset_endpoint(ep)                    (UEPRST   =   1 << (U8)ep, UEPRST  =  0)

#define Usb_enable_endpoint()                     (UEPCONX  |=  MSK_EPEN)
#define Usb_enable_stall_handshake()              (UEPSTAX  |=  MSK_STALLRQ)
#define Usb_reset_data_toggle()                   // compatibility
#define Usb_select_epnum_for_dfc()                // compatibility
#define Usb_enable_dfc()                          // compatibility
#define Usb_disable_endpoint()                    (UEPCONX  &= ~MSK_EPEN)
#define Usb_disable_stall_handshake()             (UEPSTAX  &= ~MSK_STALLRQ)
#define Usb_select_epnum_for_cpu()                // compatibility
#define Usb_disable_dfc()                         // compatibility
#define Is_usb_endpoint_enabled()                 (UEPCONX & MSK_EPEN)
#define Is_usb_endpoint_stall_requested()         (UEPSTAX & MSK_STALLRQ)

#define Usb_configure_endpoint_type(type)         (UEPCONX =   (UEPCONX & ~MSK_EPTYPE) | ((U8)type))
#define Usb_enable_iso_autoswitch()               // compatibility
#define Usb_enable_autoswitch()                   // compatibility
#define Usb_enable_nyet()                         // compatibility
#define Usb_disable_iso_autoswitch()              // compatibility
#define Usb_disable_autoswitch()                  // compatibility
#define Usb_disable_nyet()                        // compatibility
#define Usb_configure_endpoint_direction(dir)     (UEPCONX =   (UEPCONX & ~MSK_EPDIR)  | ((U8)dir << 2))

#define Usb_configure_endpoint_size(size)         // compatibility
#define Usb_configure_endpoint_bank(bank)         // compatibility
#define Usb_allocate_memory()                     // compatibility
#define Usb_unallocate_memory()                   // compatibility

#define Usb_ack_overflow_interrupt()              // compatibility
#define Usb_ack_underflow_interrupt()             // compatibility
#define Usb_ack_zlp()                             // compatibility
#define Usb_data_toggle()                         // compatibility
#define Usb_nb_busy_bank()                        (0xFF)
#define Is_usb_one_bank_busy()                    (FALSE)
#define Is_endpoint_configured()                  (UEPCONX & MSK_EPEN == MSK_EPEN)
#define Is_usb_overflow()                         (FALSE)
#define Is_usb_underflow()                        (FALSE)
#define Is_usb_zlp()                              (FALSE)

#define Usb_control_direction()                   (UEPCONX & MSK_EPDIR)
#define Usb_current_bank()                        (0xFF)

#define Usb_ack_fifocon()                         // compatibility
#define Usb_ack_nak_in()                          // compatibility
#define Usb_ack_nak_out()                         // compatibility
#define Usb_ack_receive_setup()                   (UEPSTAX &= ~MSK_RXSETUP)
#define Usb_ack_receive_out()                     (UEPSTAX &= ~MSK_RXOUT)
#define Usb_ack_stalled()                         (UEPSTAX &= ~MSK_STALLED)
#define Usb_ack_in_ready()                        (UEPSTAX &= ~MSK_TXCMPL)
#define Usb_kill_last_in_bank()                   // compatibility
#define Is_usb_read_enabled()                     (TRUE)// compatibility
#define Is_usb_write_enabled()                    (TRUE)// compatibility
#define Is_usb_read_control_enabled()             (UEPSTAX & MSK_TXCMPL) // compatibility
#define Is_usb_receive_setup()                    (UEPSTAX & MSK_RXSETUP)
#define Is_usb_receive_out()                      (UEPSTAX & MSK_RXOUTB0B1)
#ifdef NO_SUPPORT_USB_PING_PONG
  #define Usb_ack_receive_out_ms()                 Usb_ack_receive_out()
  #define Is_usb_receive_out_ms()                  Is_usb_receive_out()
#else
  #define Usb_ack_receive_out_ms()                {if(rx_bank){UEPSTAX&=~MSK_RXOUTB1;}else{UEPSTAX&=(~MSK_RXOUTB0);}rx_bank=~rx_bank;}
  #define Is_usb_receive_out_ms()                 Is_usb_receive_out()
#endif
#ifdef NO_SUPPORT_USB_PING_PONG
  #define Usb_ack_receive_out_hid()                 Usb_ack_receive_out()
  #define Is_usb_receive_out_hid()                  Is_usb_receive_out()
#else
  #define Usb_ack_receive_out_hid()                {if(rx_bank){UEPSTAX&=~MSK_RXOUTB1;}else{UEPSTAX&=(~MSK_RXOUTB0);}rx_bank=~rx_bank;}
  #define Is_usb_receive_out_hid()                 Is_usb_receive_out()
#endif
#define Is_usb_in_ready()                         (UEPSTAX & MSK_TXCMPL)
#define Usb_send_in()                              UEPSTAX |= MSK_TXRDY; while(!Is_usb_in_ready());Usb_ack_in_ready()
#define Usb_send_control_in()                     (UEPSTAX |= MSK_TXRDY)
#define Usb_free_out_bank()                       (UEPSTAX |= MSK_RXOUT)
#define Usb_ack_control_out()                     (UEPSTAX |= MSK_RXOUT)

#define Usb_set_DIR()                 (UEPSTAX |= MSK_DIR)


#define Usb_enable_flow_error_interrupt()         // compatibility
#define Usb_enable_nak_in_interrupt()             // compatibility
#define Usb_enable_nak_out_interrupt()            // compatibility
#define Usb_enable_receive_setup_interrupt()      // compatibility
#define Usb_enable_receive_out_interrupt()        // compatibility
#define Usb_enable_stalled_interrupt()            // compatibility
#define Usb_enable_in_ready_interrupt()           // compatibility
#define Usb_disable_flow_error_interrupt()        // compatibility
#define Usb_disable_nak_in_interrupt()            // compatibility
#define Usb_disable_nak_out_interrupt()           // compatibility
#define Usb_disable_receive_setup_interrupt()     // compatibility
#define Usb_disable_receive_out_interrupt()       // compatibility
#define Usb_disable_stalled_interrupt()           // compatibility
#define Usb_disable_in_ready_interrupt()          // compatibility

#define Usb_read_byte()                           (UEPDATX)
#define Usb_write_byte(byte)                      (UEPDATX  =   (U8)byte)

#define Usb_byte_counter()                        ((((U16)UBYCTHX) << 8) | (UBYCTLX))
#define Usb_byte_counter_8()                      ((U8)UBYCTLX)

#define Usb_interrupt_flags()                     (UEPINT != 0x00)
#define Is_usb_endpoint_event()                   (Usb_interrupt_flags())

// ADVANCED MACROS

#define Usb_select_ep_for_cpu(ep)                 // compatibility
#define Usb_select_ep_for_dfc(ep)                 // compatibility

#define Usb_set_cfg0(cfg0)                        (cfg0 = cfg0)//(UECFG0X = cfg0)
#define Usb_set_cfg1(cfg1)                        (cfg1 = cfg1)//(UECFG1X = (UECFG1X & MSK_ALLOC) | cfg1)


//! wSWAP
//! This macro swaps the U8 order in words.
//!
//! @param x        (U16) the 16 bit word to swap
//!
//! @return         (U16) the 16 bit word x with the 2 bytes swaped

#define wSWAP(x)        \
   (   (((x)>>8)&0x00FF) \
   |   (((x)<<8)&0xFF00) \
   ) 


//! dwSWAP
//! This macro swaps the U8 order in double words.
//!
//! @param x        (U32) the 32 bit double word to swap
//!
//! @return         (U32) the 32 bit double word x with the 4 bytes swaped
#if 0
#define dwSWAP(x)              \
   (   (((x)>>24)&0x000000FF) \
   |   (((x)>> 8)&0x0000FF00) \
   |   (((x)<<24)&0xFF000000) \
   |   (((x)<< 8)&0x00FF0000) \
   ) 
#endif

#define PLL_3MHz             0xF0
#define PLL_4MHz             0xC0
#define PLL_6MHz             0x70
#define PLL_8MHz             0x50
#define PLL_12MHz            0x30
#define PLL_13714MHz         0x61
#define PLL_16MHz            0x20
#define PLL_18MHz            0x72
#define PLL_20MHz            0xB4
#define PLL_24MHz            0x10
#define PLL_32MHz            0x21
#define PLL_40MHz            0xB9

#define Pll_stop()                    (PLLCON &= ~MSK_PLLEN)
#define Pll_set_div(n)                (PLLDIV = n)
#define Pll_enable()                  (PLLCON |= MSK_PLLEN) 




//_____ D E C L A R A T I O N ______________________________________________

U8      usb_config_ep                (U8, U8);
U8      usb_select_enpoint_interrupt (void);
U16     usb_get_nb_byte_epw          (void);
U8      usb_send_packet              (U8 , U8*, U8);
U8      usb_read_packet              (U8 , U8*, U8);
void    usb_halt_endpoint            (U8);
void    usb_reset_endpoint           (U8);
U8      usb_init_device              (void);

void    usb_generate_clock           (void);

#endif  // _USB_DRV_H_

