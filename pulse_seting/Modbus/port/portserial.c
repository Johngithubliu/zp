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
#include "mydefine.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

//static u8 TxEnable;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
	//for port1
	
    if(xRxEnable && xTxEnable)
    {
		TxEnable = 1;
		REN = 1;		//接收使能
		ES = 1;		//中断使能
    } else if(!xRxEnable && xTxEnable) {
		TxEnable = 1;
		REN = 0;		//接收禁止
		ES = 1;		//中断使能
    } else if(xRxEnable && !xTxEnable) {
		TxEnable = 0;
		REN = 1;		//接收使能
		ES = 1;		//中断使能
	} else {
		TxEnable = 0;
		REN = 0;		//接收禁止
		ES = 0;		//中断禁止
    }
		
		
		//for port2
	/*
    if(xRxEnable && xTxEnable)
    {
		TxEnable = 1;
		S2CON|=0x10;		//接收使能
		IE2|=0x01;		//中断使能
    } else if(!xRxEnable && xTxEnable) {
		TxEnable = 1;
		S2CON&=0xef;		//接收禁止
		IE2|=0x01;		//中断使能
    } else if(xRxEnable && !xTxEnable) {
		TxEnable = 0;
		S2CON|=0x10;		//接收使能
		IE2|=0x01;		//中断使能
	} else {
		TxEnable = 0;
		S2CON&=0xef;		//接收禁止
		IE2&=0xfe; 		//中断禁止
    }
		*/
		
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{

	ucDataBits = ucDataBits;
	eParity = eParity;
	
	if((ulBaudRate>115200)||(ulBaudRate<9600))
	{	
		ulBaudRate=19200;
	}
	
	BRT = 256-SYSCLK/ulBaudRate/32;		

	if(ucPORT==1)
	{//com1
		PCON &= 0x7F;	//??????
		SCON = 0x50;		//9???,?????
		
		AUXR |= 0x04;		//???????????Fosc,?1T
		AUXR |= 0x01;		//??1?????????????????
		AUXR |= 0x10;		//??????????
		
		REN = 0;							// 接收禁止，使能串口时打开
		ES = 0;                            // Disable UART0 interrupts
	}
	else
	{//com2
		AUXR &= 0xF7;		//??????
		S2CON = 0x50;		//9???,?????

		AUXR |= 0x04;		//???????????Fosc,?1T
		AUXR |= 0x10;		//??????????
		
		S2CON&=0xef;							// 接收禁止，使能串口时打开
		IE2&=0xfe;                            // Disable UART1 interrupts
	}
	
		TxEnable = 0; 

    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
		//S2BUF = ucByte;
		SBUF=ucByte;
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */

		//*pucByte =S2BUF;
		*pucByte=SBUF;
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

//for port1
void UartISR(void) interrupt 4
{
	if(RI){
		RI = 0; 
		prvvUARTRxISR(); 	
		//for test
	//	TEST=0;
			
	}
   
	if(TI){
		TI = 0;
		if(TxEnable){
			prvvUARTTxReadyISR();

		}
	}
}  

//for port2
/*
void UartISR(void) interrupt 8
{
	if(S2CON&0x01){
		S2CON&=0xfe; 
		prvvUARTRxISR(); 	
			
	}
   
	if(S2CON&0x02){
		S2CON&=0xfd;
		if(TxEnable){
			prvvUARTTxReadyISR();

		}
	}
}  
*/
void            
vMBPortClose( void )
{


}