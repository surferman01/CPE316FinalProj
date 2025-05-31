/** @file API.c
 *
 * @author Runar Kjellhaug
 *
 * @compiler This program has been tested with Keil C51 V7.50.
 *
 *
 * 2006/05/02 14:26:36
 * 1.1
 *
 */
#include "config.h"
#include "nRF_API.h"

//**********************************************************//

//**********************************************************//


// Declare HW/SW SPI Mode variable
extern BYTE SPI_Mode;


// Variable that indicates nRF24L01 interrupt source
extern BYTE IRQ_Source;
//********************************************************************************************************************//



//**********************************************************//
//
//  Function: SPI_RW
//
//  Description:
//  Writes one byte to nRF24L01, and return the byte read
//  from nRF24L01 during write, according to SPI protocol
//
//  In/Out parameters:
//  In: 'byte', current byte to be written
//  Out: 'SPI0DAT', HW SPI mode, 'byte' SW SPI mode,
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_RW(BYTE byte)
{
#ifndef nRF24L01
  BYTE bit_ctr;
#endif

  if (SPI_Mode == HW_MODE)                        // Select HW or SW SPI..
  {
    return(SPI_HW(byte));                         // Perform HW SPI operation
  }
#ifndef nRF24L01
  else                                            // software SPI....
  {
    for(bit_ctr=0;bit_ctr<8;bit_ctr++)
    {
      MOSI_Pin(byte & 0x80);                      // output 'byte', MSB to MOSI
      byte = (byte << 1);                         // shift next bit into MSB..
      SCK_Pin(1);                                 // Set SCK high..
      byte |= MISO_Pin();                         // capture current MISO bit
      SCK_Pin(0);                                 // ..then set SCK low again
    }
    MOSI_Pin(0);                                  // MOSI pin low before return

    return(byte);                                 // return 'read' byte
  }
#endif
}


//**********************************************************//
//
//  Function: SPI_Read
//
//  Description:
//  Read one byte from nRF24L01 register, 'reg'
//
//
//  In/Out parameters:
//  In: reg, register to read
//  Out: return reg_val, register value.
//
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_Read(BYTE reg)
{
  BYTE reg_val;

  CSN = 0;                                        // CSN low, initialize SPI communication...
  CSN_Pin(0);
  SPI_RW(reg);                                    // Select register to read from..
  reg_val = SPI_RW(0);                            // ..then read registervalue
  CSN_Pin(1);                                     // CSN high, terminate SPI communication

  return(reg_val);                                // return register value
}


//**********************************************************//
//
//  Function: SPI_RW_Reg
//
//  Description:
//  Writes value 'value' to register 'reg'
//
//
//  In/Out parameters:
//  In: 'reg' register to write value 'value' to.
//  Return status byte.
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_RW_Reg(BYTE reg, BYTE value)
{
  BYTE status;

  CSN_Pin(0);                                     // CSN low, init SPI transaction
  status = SPI_RW(reg);                           // select register
  SPI_RW(value);                                  // ..and write value to it..
  CSN_Pin(1);                                     // CSN high again

  return(status);                                 // return nRF24L01 status byte
}


//**********************************************************//
//
//  Function: SPI_Write_Buf
//
//  Description:
//  Writes contents of buffer '*pBuf' to nRF24L01
//  Typically used to write TX payload, Rx/Tx address
//
//
//  In/Out parameters:
//  In: register 'reg' to write, buffer '*pBuf*' contains
//  data to be written and buffer size 'buf_size' is #of
//  bytes to be written
//  Out: return nRF24L01 status byte.
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
  BYTE status,byte_ctr;

  CSN_Pin(0);                                     // Set CSN low, init SPI tranaction
  status = SPI_RW(reg);                           // Select register to write to and read status byte

  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     // then write all byte in buffer(*pBuf)
    SPI_RW(*pBuf++);

  CSN_Pin(1);                                     // Set CSN high again

  return(status);                                 // return nRF24L01 status byte
}


//**********************************************************//
//
//  Function: SPI_Read_Buf
//
//  Description:
//  Reads 'bytes' #of bytes from register 'reg'
//  Typically used to read RX payload, Rx/Tx address
//
//
//  In/Out parameters:
//  In: 'reg', register to read from, '*pBuf' are buffer
//  the read bytes are stored to and 'bytes' are #of bytes
//  to read.
//  Out: return nRF24L01 status byte.
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
  BYTE status,byte_ctr;

  CSN_Pin(0);                                     // Set CSN low, init SPI tranaction
  status = SPI_RW(reg);                           // Select register to write to and read status byte

  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = SPI_RW(0);                   // Perform SPI_RW to read byte from nRF24L01

  CSN_Pin(1);                                     // Set CSN high again

  return(status);                                 // return nRF24L01 status byte
}




