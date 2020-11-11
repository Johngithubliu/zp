/*******************************************************************************
// Explain: Header file declaration
*******************************************************************************/
#ifndef __INCLUDES_H__
#define __INCLUDES_H__

// Explain: Global variable declaration
#ifdef INCLUDES_GLOBALS
#define INC_EXT
#else
#define INC_EXT		extern
#endif

// Explain: Header file contains
//#include <p18f67k22.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <delays.h>
#include <math.h>

/*******************************************************************************
// Explain: 
*******************************************************************************/
typedef unsigned char       BOOLEAN;
typedef unsigned char       INT8U;
typedef signed char         INT8S;
typedef unsigned int        INT16U;
typedef signed int          INT16S;
typedef unsigned long       INT32U;
typedef signed long         INT32S;
typedef float               FP32;
typedef double              FP64;
// 
#define NOP()               Nop()               // PICC  asm("nop")
#define BIT0                ( 0x01 )            // 第一位
#define BIT1                ( 0x02 )            // 第二位
#define BIT2                ( 0x04 )            // 第三位
#define BIT3                ( 0x08 )            // 第四位
#define BIT4                ( 0x10 )            // 第五位
#define BIT5                ( 0x20 )            // 第六位
#define BIT6                ( 0x40 )            // 第七位
#define BIT7                ( 0x80 )            // 第八位
// 
#define STATE_OFF           ( 0x00 )            // 状态-关
#define STATE_ON            ( 0x01 )            // 状态-开
#define STATE_FALSE         ( 0x00 )
#define STATE_TURE          ( 0x01 )
#define STATE_NO            ( 0x00 )
#define STATE_YES           ( 0x01 )
#define Null                ( 0x00 )
#define STATE_GROSS         ( 0x00 )
#define STATE_NET           ( 0x01 )
// 
//#define WDT_CLR()           ClrWdt()            // 喂狗
//#define __code              rom
//#define __generic           rom
//#define OS_ENTER_CRITICAL() INTCONbits.GIE = STATE_OFF;     // 禁止中断
//#define OS_EXIT_CRITICAL()  INTCONbits.GIE = STATE_ON;      // 开启中断

/*******************************************************************************
// Explain: 
*******************************************************************************/
// Explain: signed long Union
typedef union UNION_sLong {
    INT8U dat[4];
    INT32S slon;
}UnionsLong;
// Explain: unsigned long Union
typedef union UNION_uLong {
    INT8U dat[4];
    INT32U ulon;
}UnionuLong;

// Explain: signed Int Union
typedef union UNION_sInt {
    INT8U dat[2];
    INT16S sInt;
}UnionsInt;
// Explain: unsigned Int Union
typedef union UNION_uInt {
    INT8U dat[2];
    INT16U uInt;
}UnionuInt;

// Explain: float Union
typedef union UNION_Float {
    INT8U dat[4];
    float flo;
}UnionFloat;



/*******************************************************************************
// Explain: Header file contains
*******************************************************************************/
#include "AD5422_H.H"
//#include "CS553X_H.H"
//#include "DISPLAY_H.H"
//#include "EXTCOMM_H.H"
//#include "FEATURES_H.H"
//#include "KEYPROCESS_H.H"
//#include "PIC_EEPROM_H.H"
//#include "RELAY_OUTPUT_H.H"
//#include "XK3101H_H.H"
//#include "main.h"


#endif

