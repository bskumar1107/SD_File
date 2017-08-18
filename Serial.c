/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SERIAL.C
 *      Purpose: Serial Input Output for Philips LPC214x
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <LPC214x.H>                    /* LPC214x definitions               */

/*----------------------------------------------------------------------------
 *       init_serial:  Initialize Serial Interface
 *---------------------------------------------------------------------------*/
void init_serial (void) {
  /* Initialize the serial interface */

  /* Enable RxD1 and TxD1 pins. */
  PINSEL0 &= ~0x0000000F;
  PINSEL0 |=  0x00000005;
  /* 8-bits, no parity, 1 stop bit */
  U0LCR = 0x83;
  /* 115200 Baud Rate @ 15MHz VPB Clock */
  U0DLL = 6;
  U0DLM = 0;
  U0FDR = 15<<4 | 5;
  U0LCR = 0x03;
  /* Enable FIFO with 8-byte trigger level. */
  U0FCR = 0x87;
}

/*----------------------------------------------------------------------------
 *       sendchar:  Write a character to Serial Port
 *---------------------------------------------------------------------------*/
int sendchar (int ch) {
  if (ch == '\n') {
    while (!(U0LSR & 0x20));
    U0THR = '\r';
  }
  while (!(U0LSR & 0x20));
  return (U0THR = ch);
}

/*----------------------------------------------------------------------------
 *       getkey:  Read a character from Serial Port
 *---------------------------------------------------------------------------*/
int getkey (void) {
  while (!(U0LSR & 0x01));
  return (U0RBR);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
