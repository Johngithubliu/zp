#include "stm32f10x.h"
/*******************************************************************************
// Explain: Header file contains
*******************************************************************************/
#ifndef __AD5422_H_H__
#define __AD5422_H_H__

// Explain: Global variable declaration
#ifdef AD5422_GLOBALS
#define AD5422_EXT
#else
#define AD5422_EXT			extern
#endif

/*******************************************************************************
// Explain: AD5422 Pin Definition
*******************************************************************************/
#define AD5422_LAT_H()              GPIO_SetBits(GPIOG, GPIO_Pin_5)  		//GP4DAT = (GP4DAT | 0x00080000)	//PC10->/LAT
#define AD5422_LAT_L()              GPIO_ResetBits(GPIOG, GPIO_Pin_5)		//GP4DAT = (GP4DAT & 0xFFF7FFFF)
#define AD5422_CLK_H()              GPIO_SetBits(GPIOG, GPIO_Pin_7)			//GP4DAT = (GP4DAT | 0x00100000)	//PC11->SCLK
#define AD5422_CLK_L()              GPIO_ResetBits(GPIOG, GPIO_Pin_7)		//GP4DAT = (GP4DAT & 0xffefffff)
#define AD5422_DIN_H()              GPIO_SetBits(GPIOG, GPIO_Pin_6)			//GP4DAT = (GP4DAT | 0x00100000)	//PB10->DIN
#define AD5422_DIN_L()              GPIO_ResetBits(GPIOG, GPIO_Pin_6)		//GP4DAT = (GP4DAT & 0xffefffff)

#define AD5422_CLEAN_H()              GPIO_SetBits(GPIOG, GPIO_Pin_3)			//GP4DAT = (GP4DAT | 0x00100000)	//PB10->DIN
#define AD5422_CLEAN_L()              GPIO_ResetBits(GPIOG, GPIO_Pin_3)		//GP4DAT = (GP4DAT & 0xffefffff)


/*******************************************************************************
// Explain: 
*******************************************************************************/
#define AD5422_CMD_DREG             ( 0x01 )
#define AD5422_CMD_CTRLREG          ( 0x55 )
#define AD5422_CMD_RSTREG           ( 0x56 )
// output mode R2,R1,R0
#define AD5422_CCMD_0_5V            ( 0x00 )
#define AD5422_CCMD_0_10V           ( 0x01 )
#define AD5422_CCMD_5_5V            ( 0x02 )
#define AD5422_CCMD_10_10V          ( 0x03 )
#define AD5422_CCMD_4_20mA          ( 0x05 )
#define AD5422_CCMD_0_20mA          ( 0x06 )
#define AD5422_CCMD_0_24mA          ( 0x07 )
// 
#define AD5422_CCMD_OVRRNG          (0)//( 0x4000 )
#define AD5422_CCMD_OUTEN           ( 0x1000 )
// 
#define ANALOG_0_5V                 ( 5 )
#define ANALOG_0_10V                ( 10 )
#define ANALOG_5_5V                 ( 505 )
#define ANALOG_10_10V               ( 10010 )
#define ANALOG_4_20mA               ( 4020 )
#define ANALOG_0_20mA               ( 20 )
#define ANALOG_0_24mA               ( 24 )
// 
#define AD5422_RCMD_REST            ( 0x0001 )

/*******************************************************************************
// Explain: 
*******************************************************************************/
#define ANALOG_VZERO_CORSFED()      ( 1638 )
#define ANALOG_VZERO_FINEFED()      ( 5 )
#define ANALOG_IZERO_CORSFED()      ( 1092 )
#define ANALOG_IZERO_FINEFED()      ( 5 )
// 
#define ANALOG_VFULL_CORSFED()      ( 1638 )
#define ANALOG_VFULL_FINEFED()      ( 5 )
#define ANALOG_IFULL_CORSFED()      ( 1092 )
#define ANALOG_IFULL_FINEFED()      ( 5 )
// 
#define ANALOG_VZERO_LOWTHSSHLD()   ( 60000 )
#define ANALOG_VZERO_UPTHSSHLD()    ( 32768 )
#define ANALOG_IZERO_LOWTHSSHLD()   ( 60000 )
#define ANALOG_IZERO_UPTHSSHLD()    ( 32768 )
#define ANALOG_VFULL_LOWTHSSHLD()   ( 32768 )
#define ANALOG_VFULL_UPTHSSHLD()    ( 65535 )
#define ANALOG_IFULL_LOWTHSSHLD()   ( 32768 )
#define ANALOG_IFULL_UPTHSSHLD()    ( 65535 )

/*******************************************************************************
// Explain: 
*******************************************************************************/
AD5422_EXT FP32 GfAnagVolSlope;
AD5422_EXT FP32 GfAnagCurSlope;

/*******************************************************************************
// Explain: 
*******************************************************************************/
typedef union {
    INT8U ucData[2];
    INT16U uiInt;
}AD5422uIntUnion;

/*******************************************************************************
// Explain: Function declaration
*******************************************************************************/
AD5422_EXT void DA_NOPS (unsigned int temp);
AD5422_EXT void WriteByteToAd5422 (INT8U ucAd5422Cmd, INT16U uiWriteData);
AD5422_EXT void AD5422Init (void);
AD5422_EXT void WriteControlRegisterToAD5422 (INT16U uiControlData);
AD5422_EXT void WriteDataRegisterToAD5422 (INT16U uiAnalogData);


#endif

