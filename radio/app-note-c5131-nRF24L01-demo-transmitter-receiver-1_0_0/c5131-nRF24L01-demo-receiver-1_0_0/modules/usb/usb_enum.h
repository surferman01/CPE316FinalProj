//! @file usb_enum.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is a template for writing C software programs.
//!
//! This file contains the USB endpoint 0 management routines corresponding to
//! the standard enumeration process (refer to chapter 9 of the USB
//! specification.
//! This file calls routines of the usb_user_enum.c file for non-standard
//! request management.
//! The enumeration parameters (descriptor tables) are contained in the
//! usb_user_configuration.c file.
//!
//! @version 1.5 c5131-nRF24L01-demo-receiver-1_0_0 $Id: usb_enum.h,v 1.5 2006/05/04 13:48:09 cgasnier Exp $
//!
//! @todo
//! @bug

#ifndef _USB_ENUM_H_
#define _USB_ENUM_H_

//_____ I N C L U D E S ____________________________________________________
//#include "config.h"
#include "usb_user_configuration.h"

//_____ M A C R O S ________________________________________________________
#define Usb_str_28(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28) \
  { (a1) <<8, (a2) <<8, (a3) <<8, (a4) <<8, (a5) <<8, (a6) <<8 \
  , (a7) <<8, (a8) <<8, (a9) <<8, (a10)<<8, (a11)<<8, (a12)<<8 \
  , (a13)<<8, (a14)<<8, (a15)<<8, (a16)<<8, (a17)<<8, (a18)<<8 \
  , (a19)<<8, (a20)<<8, (a21)<<8, (a22)<<8, (a23)<<8, (a24)<<8 \
  , (a25)<<8, (a26)<<8, (a27)<<8, (a28)<<8 }

//_____ S T A N D A R D    D E F I N I T I O N S ___________________________
                    //! Standard Requests

#define GET_STATUS                     0x00
#define GET_DEVICE                     0x01
#define CLEAR_FEATURE                  0x01           //!< see FEATURES below
#define GET_STRING                     0x03
#define SET_FEATURE                    0x03           //!< see FEATURES below
#define SET_ADDRESS                    0x05
#define GET_DESCRIPTOR                 0x06
#define SET_DESCRIPTOR                 0x07
#define GET_CONFIGURATION              0x08
#define SET_CONFIGURATION              0x09
#define GET_INTERFACE                  0x0A
#define SET_INTERFACE                  0x0B
#define SYNCH_FRAME                    0x0C

#define GET_DEVICE_DESCRIPTOR             1
#define GET_CONFIGURATION_DESCRIPTOR      4

#define REQUEST_DEVICE_STATUS          0x80
#define REQUEST_INTERFACE_STATUS       0x81
#define REQUEST_ENDPOINT_STATUS        0x82
#define ZERO_TYPE                      0x00
#define INTERFACE_TYPE                 0x01
#define ENDPOINT_TYPE                  0x02

                     //! Descriptor Types

#define DEVICE_DESCRIPTOR                     0x01
#define CONFIGURATION_DESCRIPTOR              0x02
#define STRING_DESCRIPTOR                     0x03
#define INTERFACE_DESCRIPTOR                  0x04
#define ENDPOINT_DESCRIPTOR                   0x05
#define DEVICE_QUALIFIER_DESCRIPTOR           0x06
#define OTHER_SPEED_CONFIGURATION_DESCRIPTOR  0x07

                    //! Standard Features

#define FEATURE_DEVICE_REMOTE_WAKEUP   0x01
#define FEATURE_ENDPOINT_HALT          0x00

#define TEST_J                         0x01
#define TEST_K                         0x02
#define TEST_SEO_NAK                   0x03
#define TEST_PACKET                    0x04
#define TEST_FORCE_ENABLE              0x05


                        //! Device Status
#define BUS_POWERED                       0
#define SELF_POWERED                      1

                         //! Device State

#define ATTACHED                          0
#define POWERED                           1
#define DEFAULT                           2
#define ADDRESSED                         3
#define CONFIGURED                        4
#define SUSPENDED                         5

#define USB_CONFIG_ATTRIBUTES_RESERVED    0x80
#define USB_CONFIG_BUSPOWERED            (USB_CONFIG_ATTRIBUTES_RESERVED | 0x00)
#define USB_CONFIG_SELFPOWERED           (USB_CONFIG_ATTRIBUTES_RESERVED | 0x40)
#define USB_CONFIG_REMOTEWAKEUP          (USB_CONFIG_ATTRIBUTES_RESERVED | 0x20)

//_____ U S B   S T R U C T U R E S _________________________________________

                          //! Usb Request
typedef struct
{
   U8      bmRequestType;        //!< Characteristics of the request
   U8      bRequest;             //!< Specific request
   U16     wValue;               //!< field that varies according to request
   U16     wIndex;               //!< field that varies according to request
   U16     wLength;              //!< Number of bytes to transfer if Data
}  S_UsbRequest;

                //! Usb Device Descriptor
typedef struct {
   U8      bLength;              //!< Size of this descriptor in bytes
   U8      bDescriptorType;      //!< DEVICE descriptor type
   U16     bscUSB;               //!< Binay Coded Decimal Spec. release
   U8      bDeviceClass;         //!< Class code assigned by the USB
   U8      bDeviceSubClass;      //!< Sub-class code assigned by the USB
   U8      bDeviceProtocol;      //!< Protocol code assigned by the USB
   U8      bMaxPacketSize0;      //!< Max packet size for EP0
   U16     idVendor;             //!< Vendor ID. ATMEL = 0x03EB
   U16     idProduct;            //!< Product ID assigned by the manufacturer
   U16     bcdDevice;            //!< Device release number
   U8      iManufacturer;        //!< Index of manu. string descriptor
   U8      iProduct;             //!< Index of prod. string descriptor
   U8      iSerialNumber;        //!< Index of S.N.  string descriptor
   U8      bNumConfigurations;   //!< Number of possible configurations
}  S_usb_device_descriptor;


          //! Usb Configuration Descriptor
typedef struct {
   U8      bLength;              //!< size of this descriptor in bytes
   U8      bDescriptorType;      //!< CONFIGURATION descriptor type
   U16     wTotalLength;         //!< total length of data returned
   U8      bNumInterfaces;       //!< number of interfaces for this conf.
   U8      bConfigurationValue;  //!< value for SetConfiguration resquest
   U8      iConfiguration;       //!< index of string descriptor
   U8      bmAttibutes;          //!< Configuration characteristics
   U8      MaxPower;             //!< maximum power consumption
}  S_usb_configuration_descriptor;


              //! Usb Interface Descriptor
typedef struct {
   U8      bLength;               //!< size of this descriptor in bytes
   U8      bDescriptorType;       //!< INTERFACE descriptor type
   U8      bInterfaceNumber;      //!< Number of interface
   U8      bAlternateSetting;     //!< value to select alternate setting
   U8      bNumEndpoints;         //!< Number of EP except EP 0
   U8      bInterfaceClass;       //!< Class code assigned by the USB
   U8      bInterfaceSubClass;    //!< Sub-class code assigned by the USB
   U8      bInterfaceProtocol;    //!< Protocol code assigned by the USB
   U8      iInterface;            //!< Index of string descriptor
}  S_usb_interface_descriptor;


               //! Usb Endpoint Descriptor
typedef struct {
   U8      bLength;               //!< Size of this descriptor in bytes
   U8      bDescriptorType;       //!< ENDPOINT descriptor type
   U8      bEndpointAddress;      //!< Address of the endpoint
   U8      bmAttributes;          //!< Endpoint's attributes
   U16     wMaxPacketSize;        //!< Maximum packet size for this EP
   U8      bInterval;             //!< Interval for polling EP in ms
} S_usb_endpoint_descriptor;


               //! Usb Device Qualifier Descriptor
typedef struct {
   U8      bLength;               //!< Size of this descriptor in bytes
   U8      bDescriptorType;       //!< Device Qualifier descriptor type
   U16     bscUSB;                //!< Binay Coded Decimal Spec. release
   U8      bDeviceClass;          //!< Class code assigned by the USB
   U8      bDeviceSubClass;       //!< Sub-class code assigned by the USB
   U8      bDeviceProtocol;       //!< Protocol code assigned by the USB
   U8      bMaxPacketSize0;       //!< Max packet size for EP0
   U8      bNumConfigurations;    //!< Number of possible configurations
   U8      bReserved;             //!< Reserved for future use, must be zero
}  S_usb_device_qualifier_descriptor;


               //! Usb Language Descriptor
typedef struct {
   U8      bLength;               //!< size of this descriptor in bytes
   U8      bDescriptorType;       //!< STRING descriptor type
   U16     wlangid;               //!< language id
}  S_usb_language_id;


               //! Usb HID Descriptor
typedef struct {
   U8      bLength;               //!< size of this descriptor in bytes
   U8      bDescriptorType;       //!< STRING descriptor type
   U16     bcdHID;                //!< HID compliant specification
   U8      bCountryCode;          //!< Country code
   U8      bNumDescriptors;       //!< Number of class descriptor
   U8      bDescriptorClass;       //!< class descriptor
   U16     wDescriptorLength;     //!< hid descriptor length
}  S_usb_hid_descriptor;

               //! Usb HID Report Descriptor
typedef struct {
   U8      report[0x34];
}  S_usb_hid_report_descriptor;





//_____ D E C L A R A T I O N ______________________________________________

void    usb_enum_var_init(   void);
void    usb_process_request( void);

#endif  // _USB_ENUM_H_

