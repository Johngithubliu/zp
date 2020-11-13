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

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

static u16 n;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
//	AUXR |= T0x12;//12T

	TMOD &= ~GATE0;	  
	TMOD &= ~C_T0;
	TMOD |= T0_M1;

	n = usTim1Timerout50us;

    return TRUE;
}


void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	TH0 = U16_HI(0-(F_MCU/12/20000)*n);		//其实此两句应该放到xMBPortTimersInit函数中
	TL0 = U16_LO(0-(F_MCU/12/20000)*n);

	TF0 = 0;
	ET0 = 1;
	TR0 = 1;
}

void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
	ET0 = 0;
	TR0 = 0;
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )  interrupt 1
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
