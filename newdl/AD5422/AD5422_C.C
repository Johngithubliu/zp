/*******************************************************************************
* Copyright 2009 Keli Electric Manufacturing(Ningbo)Co.,Ltd. All Rights Reserved.
* Module Name: AD5422_C.C
* Author: WangLiPing
* Version: V1.0
* Create Date: 2010.10.5 ----- 2011.9.8
* Description: 
* Remark: OK
* Revision History: 
*******************************************************************************/


/*******************************************************************************
// Explain: Header file contains
*******************************************************************************/
#define AD5422_GLOBALS
#include "INCLUDES.H"


/*******************************************************************************
// Prototype: WriteByteToAd5422
// Description: 
// Remark: 
// Input: INT8U ucWriteData
// Output: None
*******************************************************************************/
void DA_NOPS (unsigned int temp)  
{  
  unsigned int i,j;
  
  for(i=0;i<temp;i++)
  {
    for(j=0;j<10;j++);      
  }
}

/*******************************************************************************
// Prototype: WriteByteToAd5422
// Description: 
// Remark: 
// Input: INT8U ucWriteData
// Output: None
*******************************************************************************/
void WriteByteToAd5422 (INT8U ucAd5422Cmd, INT16U uiWriteData)
{
    INT32U ulTemplet;
    UnionuLong uulAD5422Temp;
    
    uulAD5422Temp.ulon = uiWriteData;
    uulAD5422Temp.dat[3] = 0;
    uulAD5422Temp.dat[2] = ucAd5422Cmd;
    
    AD5422_CLK_L(); AD5422_CLK_L();
    DA_NOPS(100);
    
    AD5422_DIN_L(); AD5422_DIN_L();
    DA_NOPS(100);
    
    AD5422_LAT_L(); AD5422_LAT_L();
    DA_NOPS(100);
    for (ulTemplet = 0x800000; ulTemplet > 0; ulTemplet >>= 1)
    {
        AD5422_CLK_L(); AD5422_CLK_L();
        DA_NOPS(100);
        
        if (uulAD5422Temp.ulon & ulTemplet)
        {
            AD5422_DIN_H();
        }
        else
        {
            AD5422_DIN_L();
        }
        DA_NOPS(100);
        
        AD5422_CLK_H(); AD5422_CLK_H();
        DA_NOPS(100);
    }
    AD5422_LAT_H(); AD5422_LAT_H();
    DA_NOPS(100);
}

/*******************************************************************************
// Prototype: AD5422Init
// Description: 
// Remark: 
// Input: None
// Output: None
*******************************************************************************/
void AD5422Init (void)
{
    WriteByteToAd5422 (AD5422_CMD_RSTREG, AD5422_RCMD_REST);
}

/*******************************************************************************
// Prototype: WriteControlRegisterToAD5422
// Description: 
// Remark: 
// Input: INT16U uiControlData
// Output: None
*******************************************************************************/
void WriteControlRegisterToAD5422 (INT16U uiControlData)
//void WriteControlRegisterToAD5422 (int uiControlData)
{
    AD5422uIntUnion AD5422Temp;
    
    AD5422Temp.uiInt = 0;
    switch (uiControlData)
    {
        case ANALOG_0_5V: { AD5422Temp.uiInt = AD5422_CCMD_OVRRNG | AD5422_CCMD_OUTEN | AD5422_CCMD_0_5V; break; }
        case ANALOG_0_10V: { AD5422Temp.uiInt = AD5422_CCMD_OVRRNG | AD5422_CCMD_OUTEN | AD5422_CCMD_0_10V; break; }
        case ANALOG_0_24mA: { AD5422Temp.uiInt = AD5422_CCMD_OUTEN | AD5422_CCMD_0_24mA; break; }
		case ANALOG_4_20mA: { AD5422Temp.uiInt = AD5422_CCMD_OUTEN | AD5422_CCMD_4_20mA; break; }

        default: { AD5422Temp.uiInt = AD5422_CCMD_OUTEN | AD5422_CCMD_0_24mA; break; }
    }
    WriteByteToAd5422 (AD5422_CMD_CTRLREG, AD5422Temp.uiInt);
}

/*******************************************************************************
// Prototype: WriteDataRegisterToAD5422
// Description: 
// Remark: 
// Input: INT16U uiAnalogData
// Output: None
*******************************************************************************/
void WriteDataRegisterToAD5422 (INT16U uiAnalogData)
//void WriteDataRegisterToAD5422 (int uiAnalogData)
{
    WriteByteToAd5422 (AD5422_CMD_DREG, uiAnalogData);
}
void test_4_20ma_port(void)
{

// AD5422_LAT_H();//              GPIO_SetBits(GPIOG, GPIO_Pin_5)  		//GP4DAT = (GP4DAT | 0x00080000)	//PC10->/LAT
// AD5422_LAT_H();//              GPIO_SetBits(GPIOG, GPIO_Pin_5)  		//GP4DAT = (GP4DAT | 0x00080000)	//PC10->/LAT
// AD5422_LAT_L();//              GPIO_ResetBits(GPIOG, GPIO_Pin_5)		//GP4DAT = (GP4DAT & 0xFFF7FFFF)
// 
// AD5422_CLK_H();//              GPIO_SetBits(GPIOG, GPIO_Pin_7)			//GP4DAT = (GP4DAT | 0x00100000)	//PC11->SCLK
// AD5422_CLK_L();//              GPIO_ResetBits(GPIOG, GPIO_Pin_7)		//GP4DAT = (GP4DAT & 0xffefffff)
// AD5422_CLK_H();//              GPIO_SetBits(GPIOG, GPIO_Pin_7)			//GP4DAT = (GP4DAT | 0x00100000)	//PC11->SCLK
//
// AD5422_DIN_H();//              GPIO_SetBits(GPIOG, GPIO_Pin_6)			//GP4DAT = (GP4DAT | 0x00100000)	//PB10->DIN
// AD5422_DIN_L();//              GPIO_ResetBits(GPIOG, GPIO_Pin_6)		//GP4DAT = (GP4DAT & 0xffefffff)
// AD5422_DIN_H();//              GPIO_SetBits(GPIOG, GPIO_Pin_6)			//GP4DAT = (GP4DAT | 0x00100000)	//PB10->DIN

// AD5422_CLEAN_H();//              GPIO_SetBits(GPIOG, GPIO_Pin_3)			//GP4DAT = (GP4DAT | 0x00100000)	//PB10->DIN
 AD5422_CLEAN_L();//              GPIO_ResetBits(GPIOG, GPIO_Pin_3)		//GP4DAT = (GP4DAT & 0xffefffff)
// AD5422_CLEAN_H();//              GPIO_SetBits(GPIOG, GPIO_Pin_3)			//GP4DAT = (GP4DAT | 0x00100000)	//PB10->DIN

}

