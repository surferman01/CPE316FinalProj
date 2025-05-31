//! @file mcu.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief SFR Description file for AT8xC51SND3 product.
//!
//! @version 1.2 c5131-nRF24L01-demo-transmitter-1_0_0 $Id: mcu.h,v 1.2 2005/03/24 14:54:06 lguilhau Exp $
//!
//! @todo
//! @bug

#ifndef _MCU_H_
#define _MCU_H_

#ifndef ASM_INCLUDE  // define ASM_INCLUDE in your a51 source code before include of .h file
#  if (defined FPGA1)
#  elif (defined FPGA2)
#  elif (defined SND3)
#  elif (defined SND3A)
#  else
#     error Please specify the target in the compiler options one of (FPGA1, FPGA2, SND3, SND3A).
#  endif
#endif

// C51 CORE
// All Pages
#ifndef ASM_INCLUDE  // define ASM_INCLUDE in your a51 source code before include of .h file
Sfr   (A,         0xE0);
#endif

Sfr ( ACC   , 0xE0 ) ;        // Sfr ( ACC, bit addressable)
Sfr ( B     , 0xF0 ) ;        // Sfr ( B,   bit addressable)
Sfr ( PSW   , 0xD0 ) ;        // Sfr ( PSW, bit addressable)
Sfr ( SP    , 0x81 ) ;
Sfr ( DPL   , 0x82 ) ;
Sfr ( DPH   , 0x83 ) ;

Sbit  (P,         PSW,     0);
Sfr16 (DPTR, 0x82);

#define PM_PD   0x02    /* Power Down Mode */
#define PM_IDL  0x01    /* Idle Mode */

//  I / O    P O R T
Sfr ( P0    , 0x80 ) ;        /* Sfr ( P0, bit addressable)*/

Sbit ( P0_7 , P0 , 7 ) ;
Sbit ( P0_6 , P0 , 6 ) ;
Sbit ( P0_5 , P0 , 5 ) ;
Sbit ( P0_4 , P0 , 4 ) ;
Sbit ( P0_3 , P0 , 3 ) ;
Sbit ( P0_2 , P0 , 2 ) ;
Sbit ( P0_1 , P0 , 1 ) ;
Sbit ( P0_0 , P0 , 0 ) ;

Sfr ( P1    , 0x90 ) ;        /* Sfr ( P1, bit addressable)*/

Sbit ( P1_7 , P1 , 7 ) ;
Sbit ( P1_6 , P1 , 6 ) ;
Sbit ( P1_5 , P1 , 5 ) ;
Sbit ( P1_4 , P1 , 4 ) ;
Sbit ( P1_3 , P1 , 3 ) ;
Sbit ( P1_2 , P1 , 2 ) ;
Sbit ( P1_1 , P1 , 1 ) ;
Sbit ( P1_0 , P1 , 0 ) ;

Sfr ( P2    , 0xA0 ) ;        /* Sfr ( P2, bit addressable)*/

Sbit ( P2_7 , P2 , 7 ) ;
Sbit ( P2_6 , P2 , 6 ) ;
Sbit ( P2_5 , P2 , 5 ) ;
Sbit ( P2_4 , P2 , 4 ) ;
Sbit ( P2_3 , P2 , 3 ) ;
Sbit ( P2_2 , P2 , 2 ) ;
Sbit ( P2_1 , P2 , 1 ) ;
Sbit ( P2_0 , P2 , 0 ) ;

Sfr ( P3    , 0xB0 ) ;        /* Sfr ( P3, bit addressable)*/

Sbit ( P3_7 , P3 , 7 ) ;
Sbit ( P3_6 , P3 , 6 ) ;
Sbit ( P3_5 , P3 , 5 ) ;
Sbit ( P3_4 , P3 , 4 ) ;
Sbit ( P3_3 , P3 , 3 ) ;
Sbit ( P3_2 , P3 , 2 ) ;
Sbit ( P3_1 , P3 , 1 ) ;
Sbit ( P3_0 , P3 , 0 ) ;

Sfr ( P4    , 0xC0 ) ;        /* Sfr ( P4, bit addressable)*/

Sbit ( P4_1 , P4 , 1 ) ;
Sbit ( P4_0 , P4 , 0 ) ;


/* _____ T I M E R S __________________________________________________________
*/
Sfr ( TH0   , 0x8C ) ; 
Sfr ( TL0   , 0x8A ) ;
Sfr ( TH1   , 0x8D ) ;
Sfr ( TL1   , 0x8B ) ;
Sfr ( TH2   , 0xCD ) ;
Sfr ( TL2   , 0xCC ) ;
Sfr ( TCON  , 0x88 ) ;        /* Sfr ( TCON,  bit addressable)*/
Sfr ( TMOD  , 0x89 ) ;
Sfr ( T2CON , 0xC8 ) ;        /* Sfr ( T2CON, bit addressable)*/
Sfr ( T2MOD , 0xC9 ) ;
Sfr ( RCAP2H, 0xCB ) ;
Sfr ( RCAP2L, 0xCA ) ;
Sfr ( WDTRST, 0xA6 ) ;
Sfr ( WDTPRG, 0xA7 ) ;

                            /* TCON bits */
Sbit ( TF1   , TCON , 7 ) ;
Sbit ( TR1   , TCON , 6 ) ;
Sbit ( TF0   , TCON , 5 ) ;
Sbit ( TR0   , TCON , 4 ) ;
Sbit ( IE1_  , TCON , 3 ) ;
Sbit ( IT1   , TCON , 2 ) ;
Sbit ( IE0_  , TCON , 1 ) ;
Sbit ( IT0   , TCON , 0 ) ;
                            /* T2CON bits */
Sbit ( TF2   , T2CON , 7 ) ;
Sbit ( EXF2  , T2CON , 6 ) ;
Sbit ( RCLK  , T2CON , 5 ) ;
Sbit ( TCLK  , T2CON , 4 ) ;
Sbit ( EXEN2 , T2CON , 3 ) ;
Sbit ( TR2   , T2CON , 2 ) ;
Sbit ( C_T2  , T2CON , 1 ) ;
Sbit ( CP_RL2, T2CON , 0 ) ;


/* _____ S E R I A L    I / O _________________________________________________
*/
Sfr ( SCON  , 0x98 ) ;
Sfr ( SBUF  , 0x99 ) ;
Sfr ( SADEN , 0xB9 ) ;
Sfr ( SADDR , 0xA9 ) ;

Sbit ( FE_SM0, SCON , 7 ) ;
Sbit ( SM1   , SCON , 6 ) ;
Sbit ( SM2   , SCON , 5 ) ;
Sbit ( REN   , SCON , 4 ) ;
Sbit ( TB8   , SCON , 3 ) ;
Sbit ( RB8   , SCON , 2 ) ;
Sbit ( TI    , SCON , 1 ) ;
Sbit ( RI    , SCON , 0 ) ;

/* _____ B A U D    R A T E    G E N E R A T O R ______________________________
*/
Sfr ( BRL   , 0x9A ) ;
Sfr ( BDRCON, 0x9B ) ;

/* _____ P C A ________________________________________________________________
*/
Sfr ( CCON   , 0xD8 ) ;      /* Sfr ( CCON, bit addressable)*/
Sfr ( CMOD   , 0xD9 ) ;
Sfr ( CL     , 0xE9 ) ;
Sfr ( CH     , 0xF9 ) ;
Sfr ( CCAPM0 , 0xDA ) ;
Sfr ( CCAPM1 , 0xDB ) ;
Sfr ( CCAPM2 , 0xDC ) ;
Sfr ( CCAPM3 , 0xDD ) ;
Sfr ( CCAPM4 , 0xDE ) ;
Sfr ( CCAP0H , 0xFA ) ;
Sfr ( CCAP1H , 0xFB ) ;
Sfr ( CCAP2H , 0xFC ) ;
Sfr ( CCAP3H , 0xFD ) ;
Sfr ( CCAP4H , 0xFE ) ;
Sfr ( CCAP0L , 0xEA ) ;
Sfr ( CCAP1L , 0xEB ) ;
Sfr ( CCAP2L , 0xEC ) ;
Sfr ( CCAP3L , 0xED ) ;
Sfr ( CCAP4L , 0xEE ) ;

                             /* CCON bits */
Sbit ( CF    , CCON , 7 ) ;
Sbit ( CR    , CCON , 6 ) ;
Sbit ( CCF4  , CCON , 4 ) ;
Sbit ( CCF3  , CCON , 3 ) ;
Sbit ( CCF2  , CCON , 2 ) ;
Sbit ( CCF1  , CCON , 1 ) ;
Sbit ( CCF0  , CCON , 0 ) ;


/* _____ I N T E R R U P T ____________________________________________________
*/
Sfr ( IE0   , 0xA8 ) ;       /* Sfr ( IE0,  bit addressable			*/
Sfr ( IE1   , 0xB1 ) ;
Sfr ( IPL0  , 0xB8 ) ;       /* Sfr ( IPL0, bit addressable		*/
Sfr ( IPH0  , 0xB7 ) ;
Sfr ( IPL1  , 0xB2 ) ;
Sfr ( IPH1  , 0xB3 ) ;

                             /* IE0 bits */
Sbit ( EA   , IE0 , 7 ) ;
Sbit ( EC   , IE0 , 6 ) ;
Sbit ( ET2  , IE0 , 5 ) ;
Sbit ( ES   , IE0 , 4 ) ;
Sbit ( ET1  , IE0 , 3 ) ;
Sbit ( EX1  , IE0 , 2 ) ;
Sbit ( ET0  , IE0 , 1 ) ;
Sbit ( EX0  , IE0 , 0 ) ;
                             /* IPL0 bits */
Sbit ( PPCL , IPL0 , 6 ) ;
Sbit ( PT2L , IPL0 , 5 ) ;
Sbit ( PSL  , IPL0 , 4 ) ;
Sbit ( PTIL , IPL0 , 3 ) ;
Sbit ( PXIL , IPL0 , 2 ) ;
Sbit ( PT0L , IPL0 , 1 ) ;
Sbit ( PX0L , IPL0 , 0 ) ;

/* _____ P L L ________________________________________________________________
*/
Sfr ( PLLCON, 0xA3 ) ;
Sfr ( PLLDIV, 0xA4 ) ;

/* _____ K E Y B O A R D ______________________________________________________
*/
Sfr ( KBF   , 0x9E ) ;
Sfr ( KBE   , 0x9D ) ;
Sfr ( KBLS  , 0x9C ) ;

/* _____ T W I ________________________________________________________________
*/
Sfr ( SSCON , 0x93 ) ;
Sfr ( SSCS  , 0x94 ) ;
Sfr ( SSDAT , 0x95 ) ;
Sfr ( SSADR , 0x96 ) ;

/* _____ S P I ________________________________________________________________
*/
Sfr ( SPCON , 0xC3 ) ;
Sfr ( SPSTA , 0xC4 ) ;
Sfr ( SPDAT , 0xC5 ) ;

/* _____ U S B ________________________________________________________________
*/
Sfr ( USBCON , 0xBC ) ;
Sfr ( USBADDR, 0xC6 ) ;
Sfr ( USBINT , 0xBD ) ;
Sfr ( USBIEN , 0xBE ) ;
Sfr ( UEPNUM , 0xC7 ) ;
Sfr ( UEPCONX, 0xD4 ) ;
Sfr ( UEPSTAX, 0xCE ) ;
Sfr ( UEPRST , 0xD5 ) ;
Sfr ( UEPINT , 0xF8 ) ;         /* Sfr ( UEPINT, bit addressable) */
Sfr ( UEPIEN , 0xC2 ) ;
Sfr ( UEPDATX, 0xCF ) ;
Sfr ( UBYCTLX, 0xE2 ) ;
Sfr ( UBYCTHX, 0xE3 ) ;
Sfr ( UDPADDL, 0xD6 ) ;
Sfr ( UDPADDH, 0xD7 ) ;
Sfr ( UFNUML , 0xBA ) ;
Sfr ( UFNUMH , 0xBB ) ;
                                /* UEPINT bits */
Sbit ( EP6INT , UEPINT , 6 ) ;
Sbit ( EP5INT , UEPINT , 5 ) ;
Sbit ( EP4INT , UEPINT , 4 ) ;
Sbit ( EP3INT , UEPINT , 3 ) ;
Sbit ( EP2INT , UEPINT , 2 ) ;
Sbit ( EP1INT , UEPINT , 1 ) ;
Sbit ( EP0INT , UEPINT , 0 ) ;

/* _____ M I S C . ____________________________________________________________
*/
Sfr ( PCON   , 0x87 ) ;
Sfr ( AUXR   , 0x8E ) ;
Sfr ( AUXR1  , 0xA2 ) ;
Sfr ( CKCON0 , 0x8F ) ;
Sfr ( CKCON1 , 0xAF ) ;
Sfr ( CKSEL  , 0x85 ) ;
Sfr ( LEDCON , 0xF1 ) ;
Sfr ( FCON   , 0xD1 ) ;
Sfr ( EECON  , 0xD2 ) ;




/*
Sfr   (ACC,       0xE0);   // Bit addressable
Sfr   (B,         0xF0);   // Bit addressable
Sfr   (PSW,       0xD0);   // Bit addressable
Sfr   (SP,        0x81);
Sfr   (DPL,       0x82);
Sfr   (DPH,       0x83);
Sfr16 (DPTR,      0x82);
Sfr   (PPCON,     0xC0);   // Bit addressable

Sbit  (CY,        PSW,     7);
Sbit  (AC,        PSW,     6);
Sbit  (F0,        PSW,     5);
Sbit  (RS1,       PSW,     4);
Sbit  (RS0,       PSW,     3);
Sbit  (OV,        PSW,     2);
Sbit  (F1,        PSW,     1);
Sbit  (P,         PSW,     0);

Sbit  (PPS3,      PPCON,   3);
Sbit  (PPS2,      PPCON,   2);
Sbit  (PPS1,      PPCON,   1);
Sbit  (PPS0,      PPCON,   0);

// IO PORTS
// All Pages
Sfr   (P0,        0x80);   // Bit addressable
Sfr   (P1,        0x90);   // Bit addressable
Sfr   (P2,        0xA0);   // Bit addressable
Sfr   (P3,        0xB0);   // Bit addressable
// Page 0
Sfr   (P4,        0x98);   // Bit addressable
Sfr   (P5,        0xC8);   // Bit addressable
Sfr   (P1CON,     0x9E);

Sbit  (P0_7,      P0,      7);
Sbit  (P0_6,      P0,      6);
Sbit  (P0_5,      P0,      5);
Sbit  (P0_4,      P0,      4);
Sbit  (P0_3,      P0,      3);
Sbit  (P0_2,      P0,      2);
Sbit  (P0_1,      P0,      1);
Sbit  (P0_0,      P0,      0);

Sbit  (P1_7,      P1,      7);
Sbit  (P1_6,      P1,      6);
Sbit  (P1_5,      P1,      5);
Sbit  (P1_4,      P1,      4);
Sbit  (P1_3,      P1,      3);
Sbit  (P1_2,      P1,      2);
Sbit  (P1_1,      P1,      1);
Sbit  (P1_0,      P1,      0);

Sbit  (P2_7,      P2,      7);
Sbit  (P2_6,      P2,      6);
Sbit  (P2_5,      P2,      5);
Sbit  (P2_4,      P2,      4);
Sbit  (P2_3,      P2,      3);
Sbit  (P2_2,      P2,      2);
Sbit  (P2_1,      P2,      1);
Sbit  (P2_0,      P2,      0);

Sbit  (P3_7,      P3,      7);
Sbit  (P3_6,      P3,      6);
Sbit  (P3_5,      P3,      5);
Sbit  (P3_4,      P3,      4);
Sbit  (P3_3,      P3,      3);
Sbit  (P3_2,      P3,      2);
Sbit  (P3_1,      P3,      1);
Sbit  (P3_0,      P3,      0);

Sbit  (RD,        P3,      7);
Sbit  (WR,        P3,      6);
Sbit  (T1,        P3,      5);
Sbit  (T0,        P3,      4);
Sbit  (INT1,      P3,      3);
Sbit  (INT0,      P3,      2);
Sbit  (TXD,       P3,      1);
Sbit  (RXD,       P3,      0);

Sbit  (P4_7,      P4,      7);
Sbit  (P4_6,      P4,      6);
Sbit  (P4_5,      P4,      5);
Sbit  (P4_4,      P4,      4);
Sbit  (P4_3,      P4,      3);
Sbit  (P4_2,      P4,      2);
Sbit  (P4_1,      P4,      1);
Sbit  (P4_0,      P4,      0);

Sbit  (P5_3,      P5,      3);
Sbit  (P5_2,      P5,      2);
Sbit  (P5_1,      P5,      1);
Sbit  (P5_0,      P5,      0);


// POWER & SYSTEM MANAGEMENT
// Page 0
Sfr   (PSTA,      0x86);
Sfr   (PCON,      0x87);
Sfr   (AUXR1,     0xA2);
Sfr   (VBAT,      0x85);


// CLOCK MANAGEMENT
// Page 0
Sfr   (CKCON,     0x8F);
Sfr   (CKEN,      0xB9);
Sfr   (DFCCLK,    0xBA);
Sfr   (NFCCLK,    0xBC);
Sfr   (MMCCLK,    0xBD);


// INTERRUPT
// Page 0
Sfr   (IEN0,      0xA8);   // Bit addressable
Sfr   (IEN1,      0xB1);
Sfr   (IPH0,      0xB7);
Sfr   (IPH1,      0xB3);
Sfr   (IPL0,      0xB8);
Sfr   (IPL1,      0xB2);

Sbit  (EA,        IEN0,    7);
Sbit  (EAUP,      IEN0,    6);
Sbit  (EDFC,      IEN0,    5);
Sbit  (ES,        IEN0,    4);
Sbit  (ET1,       IEN0,    3);
Sbit  (EX1,       IEN0,    2);
Sbit  (ET0,       IEN0,    1);
Sbit  (EX0,       IEN0,    0);


// TIMERS
// Page 0
Sfr   (TCON,      0x88);   // Bit addressable
Sfr   (TMOD,      0x89);
Sfr   (TL0,       0x8A);
Sfr   (TL1,       0x8B);
Sfr   (TH0,       0x8C);
Sfr   (TH1,       0x8D);

Sbit  (TF1,       TCON,    7);
Sbit  (TR1,       TCON,    6);
Sbit  (TF0,       TCON,    5);
Sbit  (TR0,       TCON,    4);
Sbit  (IE1,       TCON,    3);
Sbit  (IT1,       TCON,    2);
Sbit  (IE0,       TCON,    1);
Sbit  (IT0,       TCON,    0);


// WATCHDOG
// Page 0
Sfr   (WDTRST,    0xA6);
Sfr   (WDTPRG,    0xA7);


// RAM INTERFACE
// Page 1
Sfr   (RDFCAL,    0xFD);
Sfr   (RDFCAM,    0xFE);
Sfr   (RDFCAH,    0xFF);


// MEMORY MANAGEMENT
// Page 0
Sfr   (MEMCON,    0xF1);
Sfr   (MEMCBAX,   0xF2);
Sfr   (MEMDBAX,   0xF3);
Sfr   (MEMXBAX,   0xF4);
Sfr   (MEMCSX,    0xF5);
Sfr   (MEMXSX,    0xF6);


// CONTEXT MANAGEMENT
// Page 0
Sfr   (PPCONX,    0xF7);
Sfr   (HCACCX,    0xE8);
Sfr   (HCSPX,     0xE9);
Sfr   (HCDPHX,    0xEA);
Sfr   (HCDPLX,    0xEB);
Sfr   (HCBX,      0xEC);
Sfr   (HCPSWX,    0xED);
Sfr   (HCPCHX,    0xEE);
Sfr   (HCPCLX,    0xEF);


// SCHEDULER MANAGEMENT
// Page 0
Sfr   (SCHCON,    0xFD);
Sfr   (SCHCLK,    0xFE);
Sfr   (SCHCFG,    0xFF);
// All Pages
Sfr   (SCHGPR3,   0xF9);
Sfr   (SCHGPR2,   0xFA);
Sfr   (SCHGPR1,   0xFB);
Sfr   (SCHGPR0,   0xFC);


// DATA FLOW CONTROLLER
// Page 1
Sfr   (DFCON,     0x89);
Sfr   (DFCCS,     0x88);   // Bit addressable
Sfr   (DFD0,      0x8A);
Sfr   (DFD1,      0x8B);
Sfr   (DFCRC,     0x8C);

Sbit  (DFABT1,    DFCCS,   7);
Sbit  (DFEOFE1,   DFCCS,   6);
Sbit  (DFEOFI1,   DFCCS,   5);
Sbit  (DFBSY1,    DFCCS,   4);
Sbit  (DFABT0,    DFCCS,   3);
Sbit  (DFEOFE0,   DFCCS,   2);
Sbit  (DFEOFI0,   DFCCS,   1);
Sbit  (DFBSY0,    DFCCS,   0);


// USB CONTROLLER
// USB General
// Page 1
Sfr   (USBCON,    0xE1);
Sfr   (USBSTA,    0xE2);
Sfr   (USBINT,    0xE3);
Sfr   (UDPADDH,   0xE4);
Sfr   (UDPADDL,   0xE5);
Sfr   (OTGCON,    0xE6);
Sfr   (OTGIEN,    0xE7);
Sfr   (OTGINT,    0xD1);

// USB Device
// Page 1
Sfr   (UDCON,     0xD9);
Sfr   (UDINT,     0xD8);   // bit addressable
Sfr   (UDIEN,     0xDA);
Sfr   (UDADDR,    0xDB);
Sfr   (UDFNUMH,   0xDC);
Sfr   (UDFNUML,   0xDD);
Sfr   (UDMFN,     0xDE);
Sfr   (UDTST,     0xDF);

Sbit  (UPRSMI,    UDINT,   6);
Sbit  (EORSMI,    UDINT,   5);
Sbit  (WAKEUPI,   UDINT,   4);
Sbit  (EORSTI,    UDINT,   3);
Sbit  (SOFI,      UDINT,   2);
Sbit  (MSOFI,     UDINT,   1);
Sbit  (SUSPI,     UDINT,   0);

// USB Endpoint
// Page 1
Sfr   (UENUM,     0xC9);
Sfr   (UERST,     0xCA);
Sfr   (UECONX,    0xCB);
Sfr   (UECFG0X,   0xCC);
Sfr   (UECFG1X,   0xCD);
Sfr   (UESTA0X,   0xCE);
Sfr   (UESTA1X,   0xCF);
Sfr   (UEINTX,    0xC8);   // bit addressable
Sfr   (UEIENX,    0xD2);
Sfr   (UEDATX,    0xD3);
Sfr   (UEBCHX,    0xD4);
Sfr   (UEBCLX,    0xD5);
Sfr   (UEINT,     0xD6);

Sbit  (FIFOCON,   UEINTX,  7);
Sbit  (NAKINI,    UEINTX,  6);
Sbit  (RWAL,      UEINTX,  5);
Sbit  (NAKOUTI,   UEINTX,  4);
Sbit  (RXSTPI,    UEINTX,  3);
Sbit  (RXOUTI,    UEINTX,  2);
Sbit  (STALLEDI,  UEINTX,  1);
Sbit  (TXINI,     UEINTX,  0);

// USB Host
// Page 1
Sfr   (UHCON,     0xD9);
Sfr   (UHINT,     0xD8);   // bit addressable
Sfr   (UHIEN,     0xDA);
Sfr   (UHADDR,    0xDB);
Sfr   (UHFNUMH,   0xDC);
Sfr   (UHFNUML,   0xDD);
Sfr   (UHFLEN,    0xDE);

Sbit  (HWUPI,     UHINT,   6);
Sbit  (HSOFI,     UHINT,   5);
Sbit  (RXRSMI,    UHINT,   4);
Sbit  (RSMEDI,    UHINT,   3);
Sbit  (RSTI,      UHINT,   2);
Sbit  (DDISCI,    UHINT,   1);
Sbit  (DCONNI,    UHINT,   0);

// USB Pipe
// Page 1
Sfr   (UPNUM,     0xC9);
Sfr   (UPRST,     0xCA);
Sfr   (UPCONX,    0xCB);
Sfr   (UPCFG0X,   0xCC);
Sfr   (UPCFG1X,   0xCD);
Sfr   (UPCFG2X,   0xCF);
Sfr   (UPSTAX,    0xCE);
Sfr   (UPINRQX,   0xDF);
Sfr   (UPERRX,    0xD7);
Sfr   (UPINTX,    0xC8);   // bit addressable
Sfr   (UPIENX,    0xD2);
Sfr   (UPDATX,    0xD3);
Sfr   (UPBCHX,    0xD4);
Sfr   (UPBCLX,    0xD5);
Sfr   (UPINT,     0xD6);

// Sbit  (FIFOCON,   UPINTX,  7);  // Already defined for the device
Sbit  (NAKEDI,    UPINTX,  6);
// Sbit  (RWAL,      UPINTX,  5);  // Already defined for the device
Sbit  (PERRE,     UPINTX,  4);
Sbit  (TXSTPI,    UPINTX,  3);
Sbit  (TXOUTI,    UPINTX,  2);
Sbit  (RXSTALLI,  UPINTX, 1);


// NAND FLASH CONTROLLER
// Page 1
Sfr   (NFCFG,     0x99);
Sfr   (NFLOG,     0x9A);
Sfr   (NFCON,     0x9B);
Sfr   (NFERR,     0x9C);
Sfr   (NFADR,     0x9D);
Sfr   (NFADC,     0x9E);
Sfr   (NFCMD,     0x9F);
Sfr   (NFACT,     0xA1);
Sfr   (NFDAT,     0xA2);
Sfr   (NFDATF,    0xA3);
Sfr   (NFSTA,     0x98);   // Bit addressable
Sfr   (NFECC,     0xA4);
Sfr   (NFINT,     0xA5);
Sfr   (NFIEN,     0xA6);
Sfr   (NFUDAT,    0xA7);

Sbit  (NFC_SMCD,  NFSTA,   7);
Sbit  (NFC_SMLCK, NFSTA,   6);
Sbit  (NFC_SF,    NFSTA,   5);
Sbit  (NFC_RUN,   NFSTA,   0);


// MMC CONTROLLER
// Page 1
Sfr   (MMCON0,    0xB1);
Sfr   (MMCON1,    0xB2);
Sfr   (MMCON2,    0xB3);
Sfr   (MMSTA,     0xB5);
Sfr   (MMDAT,     0xB6);
Sfr   (MMCMD,     0xB7);
Sfr   (MMINT,     0xBE);
Sfr   (MMMSK,     0xBF);


// AUDIO CONTROLLER
// Page 1
Sfr   (AUCON,     0xF1);
Sfr   (APCON0,    0xF2);
Sfr   (APCON1,    0xF3);
Sfr   (APSTA,     0xEA);
Sfr   (APDAT,     0xEB);
Sfr   (APINT,     0xF4);
Sfr   (APIEN,     0xE9);
// Page 2
Sfr   (APTIM0,    0xC6);
Sfr   (APTIM1,    0xC7);
Sfr   (APTIM2,    0xC9);
Sfr   (APRDVOL,   0xF1);
Sfr   (APLDVOL,   0xF2);
Sfr   (APBDVOL,   0xF3);
Sfr   (APMDVOL,   0xF4);
Sfr   (APTDVOL,   0xF5);
Sfr   (ACCON,     0xEA);
Sfr   (ACORG,     0xEB);
Sfr   (ACOLG,     0xEC);
Sfr   (ACIPG,     0xED);
Sfr   (ADICON0,   0xEE);
Sfr   (ADICON1,   0xEF);
Sfr   (ASCON,     0xE1);
Sfr   (ASSTA0,    0xE2);
Sfr   (ASSTA1,    0xE3);
Sfr   (ASSTA2,    0xE9);


// SPI CONTROLLER
// Page 1
Sfr   (SPCON,     0xC3);
Sfr   (SPSTA,     0xC4);
Sfr   (SPDAT,     0xC5);


// UART
// Page 1
Sfr   (SCON,      0xA8);   // Bit addressable
Sfr   (SBUF,      0xA9);

// Page 0
Sfr   (BRL,       0x91);
Sfr   (BDRCON,    0x92);

Sbit  (SM0,       SCON,    7);
Sbit  (FE,        SCON,    7);
Sbit  (SM1,       SCON,    6);
Sbit  (SM2,       SCON,    5);
Sbit  (REN,       SCON,    4);
Sbit  (TB8,       SCON,    3);
Sbit  (RB8,       SCON,    2);
Sbit  (TI,        SCON,    1);
Sbit  (RI,        SCON,    0);

// LCD INTERFACE
// Page 1
Sfr   (LCDCON0,   0x96);
Sfr   (LCDCON1,   0x8E);
Sfr   (LCDSTA,    0x8F);
Sfr   (LCDDAT,    0x97);
Sfr   (LCDBUM,    0x8D);

// KEYBOARD INTERFACE
// Page 0
Sfr   (KBCON,     0xA3);
Sfr   (KBSTA,     0xA4);

// ON CHIP DEBUG
// Page 3
Sfr   (OCDPIN,    0x89);
Sfr   (OACTIV,    0x99);
Sfr   (OBRL,      0x91);
// All Pages
Sfr   (OSCON,     0xF8);
Sfr   (OSBUF,     0x84);

*/
#endif // _MCU_H_
