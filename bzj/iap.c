#include "stc12cxx.h"


void Iap_Write(unsigned int address,unsigned char dat)
{
   	IAP_CONTR=0x8b;
	IAP_DATA=dat;
	IAP_ADDRH=(address>>8)&0xff;
	IAP_ADDRL=address&0x0ff;
	IAP_CMD=0x02;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5;
	IAP_CONTR=0x00;

}

unsigned char Iap_Read(unsigned int address)
{
	IAP_CONTR=0x8b;

	IAP_ADDRH=(address>>8)&0xff;
	IAP_ADDRL=address&0x0ff;
	IAP_CMD=0x01;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5;
	IAP_CONTR=0x00;

	return IAP_DATA;

}


void Iap_Erase(unsigned int address)
{	
	IAP_CONTR=0x8b;
	
	IAP_ADDRH=(address>>8)&0xff;
	IAP_ADDRL=address&0x0ff;
	IAP_CMD=0x03;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5;
	IAP_CONTR=0x00;

}