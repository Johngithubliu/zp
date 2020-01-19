/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

static u8 TxEnable;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */

    if(xRxEnable && xTxEnable)
    {
		TxEnable = 1;
		S2CON |= S2REN;//接收使能
		IE2 |= ES2;//中断使能
    } else if(!xRxEnable && xTxEnable) {
		TxEnable = 1;
		S2CON &= ~S2REN;//接收使能
		IE2 |= ES2;//中断使能
    } else if(xRxEnable && !xTxEnable) {
		TxEnable = 0;
		S2CON |= S2REN;//接收使能
		IE2 |= ES2;//中断使能
	} else {
		TxEnable = 0;
		S2CON &= ~S2REN;//接收使能
		IE2 &= ~ES2;//中断使能
    }
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	u16 bps;



	ucPORT = ucPORT;
	ucDataBits = ucDataBits;
	eParity = eParity;
	bps = ulBaudRate;

	AUXR |= BRTx12;
	AUXR |= S2SMOD;

	BRT = 0-(F_MCU/16/bps);

	AUXR |= BRTR;

	S2CON &= ~S2SM0;
	S2CON |= S2SM1;
	S2CON &= ~S2REN;//接收不使能
	S2CON &= ~S2RI;	  
	S2CON &= ~S2TI;

	IE2 &= ~ES2;//中断不使能

	TxEnable = 0; 

    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
	S2BUF = ucByte;
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
	*pucByte = S2BUF;
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
//    pxMBFrameCBTransmitterEmpty(  );
//	if(eMBCurrentMode==MB_RTU){
		xMBRTUTransmitFSM();
//	}else if(eMBCurrentMode==MB_ASCII){
//		xMBASCIITransmitFSM();
//	}
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{		
//    pxMBFrameCBByteReceived(  ); 	  
//	if(eMBCurrentMode==MB_RTU){	 
		xMBRTUReceiveFSM();	  
//	}else if(eMBCurrentMode==MB_ASCII){
//		xMBASCIIReceiveFSM();
//	}
}


void UartISR(void) interrupt 8
{
	if(S2CON&S2RI){
		S2CON &= ~S2RI;
		prvvUARTRxISR(); 	
	}
   
	if(S2CON&S2TI){
		S2CON &= ~S2TI;
		if(TxEnable){
			prvvUARTTxReadyISR();
		}
	}
}  

void            
vMBPortClose( void )
{


}