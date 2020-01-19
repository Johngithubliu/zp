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
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "includes.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mydefine.h"


/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

static u16 usPcaDelayValue;

/* ----------------------- Start implementation -----------------------------*/
//-----------------------------------------------------------------------------
// xMBPortTimersInit
// 在此函数中对T35超时定时器进行设置，当定时器超时后，中断函数
// 调用xMBRTUTimerT35Expired()函数。
// 
//-----------------------------------------------------------------------------
BOOL
xMBPortTimersInit( USHORT ulBaudRate )
{
    
	if( ulBaudRate !=0 )
    {
      usPcaDelayValue = ( 7UL * 11UL* SYSCLK) / ( 2* ulBaudRate *12UL);
    } 
		else usPcaDelayValue = ( 7UL * 11UL* SYSCLK) / ( 2* 19200 *12UL);
		//=1838=0x738;
		CCON=0;
		CMOD=0;
		CCAPM0=0x48;
		CCAP0L=U16_LO(usPcaDelayValue); 
		CCAP0H=U16_HI(usPcaDelayValue); 

    return TRUE;
}


void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
		CCON=0;
		CCAPM0=0x49;
	CL=0;
	CH=0;
	CR=1;


}

void
vMBPortTimersDisable(  )
{
		CR=0;
		CCAPM0=0x48;

		CCON=0;
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )  interrupt 7  //PCA0 interrupt handler
{ 
//    ( void )pxMBPortCBTimerExpired(  );		
//	if(eMBCurrentMode==MB_RTU){
		xMBRTUTimerT35Expired();
//	}else if(eMBCurrentMode==MB_ASCII){
//		xMBASCIITimerT1SExpired();
//	}
}

void            
vMBPortTimersDelay( USHORT usTimeOutMS )
{
	usTimeOutMS = usTimeOutMS;
}
