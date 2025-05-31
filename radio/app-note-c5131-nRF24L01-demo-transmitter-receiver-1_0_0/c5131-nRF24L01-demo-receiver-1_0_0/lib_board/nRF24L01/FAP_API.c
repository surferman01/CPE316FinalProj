/** @file FAP.c
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
#include "defines.h"
#include "nRF_API.h"
#include "Protocol_API.h"
//#include "LL_API.h"

extern BYTE IRQ_Source;                                               // Global:IRQ_Source variable
extern BYTE xdata Freq_Table[FREQ_TABLE_SIZE];                        // FAP Channel Table

extern BYTE xdata TX_pload[TX_PLOAD_MAX_WIDTH];                       // TX payload buffer
extern BYTE xdata TX_pload_width;                                     // TX payload width variable

extern BYTE xdata RX_pload[RX_PLOAD_MAX_WIDTH];                       // RX payload buffer

extern BYTE LinkStat;                                                 // Global:Link Status variable
extern BYTE TX_Update;                                                // TX packet to send variable
extern BYTE RX_Timeout;                                               // Freq Agil Protocol timeout flag

#define TRY_LIMIT 0x03                                                // Try_Ctr max value

void Freq_Agil_Protocol()
{
BYTE Try_Ctr,Table_Ptr;
//BYTE PipeNum,PipeWidth;

  while(TRUE)                                                         // Endless loop, Frequency Agility Protocol
  {
    if(IRQ_Source)                                                    // Handle nRF24L01 IRQ
    {
      switch(IRQ_Source)                                              // Select IRQ_Source
      {
        case MAX_RT:                                                  // CASE: Max retries interrupt

          CE_Pin(CE_LOW);                                             // CE pin low
          
          if(!(Try_Ctr++ == TRY_LIMIT))                               // Increment & check if "Try_Ctr NOT > TRY_LIMIT"
          {
            if(Table_Ptr++ == 16)                                     // Freq_Agil_Table wrap?
            {

              Table_Ptr = 0x00;                                       // Wrap Table_Ptr to first channel
            }
            L01_Set_Channel(Freq_Table[Table_Ptr]);                   // Change channel
            L01_Clear_IRQ(MAX_RT);                                    // Clear MAX_RT IRQ flag
            
            LinkStat = MAX_RT;                                        // Current LinkStat = MAX RETRIES
          }
          else                                                        // Try_Ctr == TRY_LIMIT
          {
            Try_Ctr = 0;                                              // Clear Try_Ctr
            LinkStat = TRY_LIMIT;                                     // Current LinkStat = TRY_LIMIT
          }
          
        break;
        
        case TX_DS:                                                   // CASE: TX data sent interrupt

          CE_Pin(CE_LOW);                                             // CE pin low, to be sure..

          LinkStat = TX_DS;                                           // Current Link Status = TX DATA SENT

        break;

        case RX_DR:                                                   // CASE: RX data received interrupt

          L01_Read_RX_Pload(RX_pload);
          //Reset_Freq_Agil_Tmr();                                      // Reset freq_agil_timer for current pipe
          LinkStat = RX_DR;                                           // Current Link Status = RX DATA READY

        break;

        default:                                                      // Case:default
        break;
      }
      
      IRQ_Source = IDLE;                                              // Clear IRQ_Source
    }                                                                 // if(!(IRQ_Source == 0))

    if(TX_Update)                                                     // TX_MODE: New packet to send?
    {
      CE_Pin(CE_LOW);                                                 // CE pin low, just to be sure
      L01_Write_TX_Pload(TX_pload, TX_pload_width);                   // Writes new payload to L01
      CE_Pin(CE_PULSE);                                               // Pulse CE to send packet
      
      TX_Update = 0;                                                  // Clear TX_Update flag
    }

    if(RX_Timeout)                                                    // RX_MODE: Freq_Agil timeout
    {
      
      RX_Timeout = 0;
    }
    
  }

}
