#include <intrins.h>
#include "stc12cxx.h"
#include "iap.h"
#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */
#include "mydefine.h"
#include <stdio.h>
#include "24lc16.h"
 
void read(unsigned char padd,unsigned char address,unsigned char *Pdata,unsigned char len); 
void write(unsigned char padd,unsigned char address,unsigned char *pdd,unsigned char len); 

//------------------------------------------------------------------------------ 


void s24(void)   

{ 
	SDA_OUT();
	SCL_CLR; 
	_nop_();_nop_(); _nop_();_nop_();
	_nop_();_nop_(); _nop_();_nop_();
   	SDA_SET; 
	_nop_();_nop_(); _nop_();_nop_();
	SCL_SET; 
    	_nop_();_nop_(); _nop_();_nop_(); 
	SDA_CLR; 
	_nop_();_nop_(); _nop_();_nop_();
        SCL_CLR; 
	_nop_();_nop_(); _nop_();_nop_();
} 
//------------------------------------------------------------------------------ 


void p24(void)                 
{ 
	SDA_OUT();
	SCL_CLR;
	_nop_();_nop_(); _nop_();_nop_();
	SDA_CLR; 
 	
	_nop_();_nop_(); _nop_();_nop_();
   	SCL_SET; 
	_nop_();_nop_(); _nop_();_nop_();
        SDA_SET; 
	_nop_();_nop_(); _nop_();_nop_();
	//SCL_CLR; 
	_nop_();_nop_(); _nop_();_nop_();
} 
void  ack(void)
{
	
	SDA_OUT();
	_nop_();_nop_(); _nop_();_nop_();
	SCL_CLR;
	_nop_();_nop_(); _nop_();_nop_();
	SDA_CLR;
	_nop_();_nop_(); _nop_();_nop_();
	SCL_SET;
	_nop_();_nop_(); _nop_();_nop_();
	SCL_CLR;
	
}
 
unsigned char  nack(void)
{
	unsigned char ackdat;
	SDA_IN();
	
	SCL_CLR;
	_nop_();_nop_(); _nop_();_nop_();
	
	_nop_();_nop_(); _nop_();_nop_();
	SCL_SET;
	_nop_();_nop_(); _nop_();_nop_();
	ackdat=SDA_DATA;
	_nop_(); 
    	_nop_(); 
	SCL_CLR;
	return ackdat;
}
 


//------------------------------------------------------------------------------ 

unsigned char rd24(void) /////////////////
{       
   	unsigned char E_dat,i;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		E_dat<<=1;
		SCL_SET;
		_nop_();_nop_();_nop_();_nop_();
		E_dat|=SDA_DATA;
		_nop_();_nop_();_nop_();_nop_();
		SCL_CLR;
	}
	SDA_OUT();
	return(E_dat); 

} 

//------------------------------------------------------------------------------ 


void wt24(unsigned char dd) ////////////////

{     
 	unsigned char i;
 	SDA_OUT();
	for(i=0;i<8;i++)
	{
		if(dd&0x80)SDA_SET;
		else SDA_CLR;
		_nop_();_nop_();_nop_();_nop_();
		SCL_SET;
		_nop_();_nop_();_nop_();_nop_();
		SCL_CLR;
		_nop_();_nop_();_nop_();_nop_();
		dd<<=1;
	}
	

} 

//--------------------------------------------------------------------------- 
void read(unsigned char padd,unsigned char address,unsigned char *Pdata,unsigned char len) 

{ 

	E_CS_CLR;
	padd<<=1;
	s24();                     
    	wt24(0xa0|padd);                
    	nack();                       
    	wt24(address);              
	nack(); 
	s24();                          
	wt24(0xa1|padd);                
	nack();
	while(len!=1)
	{
    		*Pdata=rd24();  
		ack();
		Pdata++;
		len--;
	}
	*Pdata=rd24(); 
	nack();
	p24();
	E_CS_SET;
	

} 

//------------------------------------------------------------------------------ 
void write(unsigned char padd,unsigned char address,unsigned char *pdd,unsigned char len) 

{ 
	int i;
	E_CS_CLR;
	padd<<=1;
	s24();                     
	wt24(0xa0|padd);           
	nack(); 
    	wt24(address);             
    	nack(); 
	for(;len!=0;len--)
	{
		wt24(*pdd);                  
		nack();
		pdd++;
	}
	p24(); 
	
	for(i=0;i<24000;i++){_nop_();_nop_();}
	
	E_CS_SET;
	
	
}  

unsigned char Iap_Read(unsigned int address)
{
	unsigned char dat;
	read((address>>8)&0x0ff,address&0x0ff,&dat,1);
	
	return dat;
}
void Iap_Write(unsigned int address,unsigned char dat)
{
	write((address>>8)&0x0ff,address&0x0ff,&dat,1);
}

