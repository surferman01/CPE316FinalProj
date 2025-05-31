/** @file ISR.c
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
//  Global defines & includes
#include "config.h"
#include "defines.h"
#include "nRF_API.h"
//#include "LL_API.h"
#include "Protocol_API.h"

extern BYTE xdata RX_buffer[USB_RX_SIZE];
extern BYTE xdata RX_pload[RX_PLOAD_MAX_WIDTH];
extern BYTE xdata TX_pload[TX_PLOAD_MAX_WIDTH];
extern BYTE xdata TX_pload_width;
extern BYTE USB_Rx_Comp;
extern BYTE IRQ_Source;
extern BYTE Timer_Mode;
extern BYTE Trans_Tmr,Trans_Tmr_Ctr;
extern BYTE Sweep_Mode;
extern BYTE Sweep_Time_Ctr;
extern BYTE sw_low,sw_high,next_ch;
extern BYTE Sweep_Progress;
//extern BYTE RX_pipe_num;
extern BYTE Test_Mode;
extern BYTE Com_Mode;
extern BYTE xdata Table_Ptr;                                      // Pointer for Freq_table lookup
extern BYTE xdata Freq_table[FREQ_TABLE_SIZE];
extern BYTE Agil_Timeout;
extern BYTE LinkStatus[2];
extern BYTE LinkStat, LastStat;
extern BYTE TX_Update;
extern BYTE Led_Blink1,Led_Blink2,Led_Blink3,Led_Blink4;
extern UINT Link_Loss_Delay;
//****************************************
//
// Extern Interrupt0
//
// Interrupt from nRF24L01
// data ready in RX-payload
//
// In _TESTMODE_:
// Read RX-payload, clear RX_DR....
//
//****************************************
void nRF24L01_IRQ(void) interrupt IRQ_INT0
{
BYTE temp;

  temp = L01_Get_Status();                                        // Read status, get IRQ source & pipe#
  switch(temp & MASK_IRQ_FLAGS)                                   // Get IRQ source
  {
    case RX_DR:                                                   // Data Ready RX FIFO ?
      IRQ_Source = RX_DR;                                         // Flag for RX_DR
    break;

    case TX_DS:                                                   // Data Sent TX FIFO ?
      IRQ_Source = TX_DS;                                         // Flag for TX_DS
    break;

    case MAX_RT:                                                  // Max #of retries ?
      IRQ_Source = MAX_RT;                                        // Flag for MAX_RT
    break;

    default:                                                      // Default, clear IRQ flags
      L01_Clear_IRQ(MASK_IRQ_FLAGS);
    break;
  }

}


