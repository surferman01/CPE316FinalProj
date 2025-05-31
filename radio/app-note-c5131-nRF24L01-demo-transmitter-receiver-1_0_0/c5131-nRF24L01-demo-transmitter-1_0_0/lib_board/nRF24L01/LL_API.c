/** @file low_level.c
 *
 * @author Runar Kjellhaug
 *
 * @compiler This program has been tested with Keil C51 V7.50.
 *
 * @copyright
 * Copyright (c) 2005 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 * @endcopyright
 *
 * 2006/05/02 14:26:37
 * 1.1
 *
 */
#include "config.h"
#include "LL_API.h"
#include "nRF_API.h"
#include "Protocol_API.h"
#include "defines.h"

extern BYTE xdata TX_pload[TX_PLOAD_MAX_WIDTH];         // TX payload buffer
extern BYTE xdata Freq_table[FREQ_TABLE_SIZE];          // Freq Agility table
extern BYTE xdata Freq_agil[NUM_OF_PIPES];              // Freq Agility On/Off for pipe.n
extern BYTE xdata Trans_Ctrl[TRANS_PARAMS];             // Transmission Control bytes
extern BYTE xdata RX_pload_length[NUM_OF_PIPES];        // holds #of bytes for pipe 0..5
extern BYTE xdata TX_pload_width;                       // TX payload width variable
extern BYTE xdata TX_buffer[USB_TX_SIZE];               // Next packet to sent to host
extern BYTE LinkStatus[2];
extern BYTE LinkStat, LastStat;                         // Link Status bytes

extern BYTE Com_Mode;
extern BYTE SPI_Mode;

extern UINT Link_Loss_Delay;
extern BYTE Try_Ctr;

code const BYTE P0_ADDR[4] =  {0xE7,0xE7,0xE7,0xE7};    // Default P0 address
code const BYTE P1_ADDR[4] =  {0xC2,0xC2,0xC2,0xC2};    // Default P1 address


//C5131 registers
#define SPI0DAT SPDAT
#define SPI0CN SPDAT
#define SPI0CFG SPDAT
#define SPI0CKR SPDAT
#define XBR0 SPDAT



void CPU_Init(void)
{

}

void CE_Pin(BYTE action)                                // CE pin high, low or pulse..
{
  switch(action)
  {
    case CE_LOW:                                        // action == 0, CE low
      CE = 0;
    break;

    case CE_HIGH:                                       // action == 1, CE high
      CE = 1;
    break;

    case CE_PULSE:                                      // action == 2, CE pulse (10µs)
      CE = 1;                                           // Set CE pin high
      TR0 = 1;                                          // Start Timer0, CE pulse timer
    break;
  }
}

void CSN_Pin(BYTE state)                                // Set/reset CSN pin
{
  if(state)
    CSN = 1;
  else
    CSN = 0;
}

void SCK_Pin(BYTE state)                                // Set/reset SCK pin
{
  if(state)
    SCK = 1;
  else
    SCK = 0;
}

void MOSI_Pin(BYTE state)                               // Set/reset MOSI pin
{
  if(state)
    MOSI = 1;
  else
    MOSI = 0;
}

BYTE MISO_Pin(void)                                     // Read MISO pin
{
  return MISO;
}

BYTE SPI_HW(BYTE byte)                                  // Write one byte using F320's hardware SPI
{
  SPI0DAT = byte;                                       // Writes 'byte' to nRF24L01
  while(WAIT_SPIF);                                     // ..and wait until SPIF goes high, i.e. transaction finish
  SPI0CN &= 0x7f;                                       // clear SPIF

  return(SPI0DAT);                                      // return received byte
}

void SPI_Init(BYTE Mode)
{
  if(Mode)                                             // If HW_SW = 1 =>> hardware SPI, else software SPI.
  {
    SPI0CFG = SPI_CFG;                                  // SPI Configuration Register
    SPI0CN  = SPI_CTR;                                  // SPI Control Register
    SPI0CKR = SPI_CLK;                                  // SPI Clock Rate Register
    XBR0 |= SPI0E;                                      // SPI SW Mode, enable SPI Crossbar

    SPI_Mode = HW_MODE;
  }
  else                                                  // Software SPI mode
  {
    SPI0CN  = 0x00;                                     // SPI Control Register, SPI Disabled
    
    XBR0 &= ~SPI0E;                                     // SPI SW Mode, disable SPI Crossbar
    SPI_Mode = SW_MODE;
  }
}