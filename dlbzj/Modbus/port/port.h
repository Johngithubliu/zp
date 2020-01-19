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
 * File: $Id: port.h,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

#include "stc12cxx.h"

#define NDEBUG
#include <assert.h>

#include "common.h"

#define F_MCU (11059200UL)			// 此常量代表晶振的频率
#define PRESCALE_FACTOR (12UL)		// 此常量代表T35定时器的预分频比

sbit LED1=P2^7;
sbit LED2=P2^6;

sbit KEY1=P2^4;
sbit KEY2=P2^5;


#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION( ) EA = 0;  
#define EXIT_CRITICAL_SECTION( ) EA = 1;    

typedef uint8_t BOOL;

typedef unsigned char UCHAR;
typedef signed char CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#endif
