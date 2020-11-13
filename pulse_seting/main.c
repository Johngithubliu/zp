
//pulse_setting
//v39
//copyright by John
//2015.07.12


#include "stc12cxx.h"
#include "intrins.h"
//--------------------- 
#include "stdio.h"
#include "math.h"
 

#include "mb.h"
#include "iap.h"

#include "mydefine.h"

#define STOP	0
#define START	1

#define max_speed 	var[0]
#define length1 		var[1]
#define step 				var[2]
#define coeffcient 	var[3]
#define line 				var[4]
#define fenzi 			var[5]
#define fenmu 			var[6]
#define interleave 	var[7]
#define mode				var[8]
#define time_used		var[9]
#define task				var[10]
#define up					var[11]
#define down				var[12]
#define top					var[13]
#define debug				var[14]
#define bitswitch		var[15]
										  
#define direct1 		var[15]&0x01

#define interleave1 	var[16]
#define interleave2 	var[17]
#define up1	var[18]
#define up2	var[19]

#define speed3	var[20]

#define length3 var[21]
#define speed4	var[22]
#define pulse_active	var[23]
//--------------------------------
//define LED
#define LED1_ON	P16=0
#define LED1_OFF	P16=1
#define LED1_FLASH	P16=!P16

#define MODE_UNIDIRECT	0
//单向模式=0
#define MODE_RECIPRO	1
//往复=1
#define MODE_2	2
#define MODE_3	3
//the other
//

#define TEST	P20
#define PULSE1	P13
//2A
#define DIRECT1	P12
//3A
#define PULSE 	P10
//1A
#define DIRECT	P11
//4A
#define READP0		{P0=0xff;IN_OE=0; input=P0;	IN_OE=1;}
#define WRITEP0	{P0=output,OUT_LE=1;OUT_LE=0;OUT_OE=0;}


//deifne input-------------
 
#define IN_OE		P17
#define OUT_LE	P16
#define OUT_OE	P14

#define KEY_FORWARD		(input&0x02)
//35
#define KEY_BACKWARD	(input&0x01)
//37
//#define KEY_RECIPRO		(input&0x04)
//#define MODE1			(input&0x04)

//39
#define KEY_JOG_FORWARD	(input&0x08)
#define INCDEC			(input&0x08)
//11

#define KEY_JOG_BACKWARD	(input&0x10)
//40
//#define MODE				(input&0x20)

//12
#define TOC1				(input&0x80)
//toc1
#define TOC2				(input&0x40)
//toc2
//--------------------------------
 //define const

//#define TRUE	1
//#define FALSE	0

//#define CW	0
//#define CCW 1
 
//#define SYSCLK		11059200L
//#define SYSCLK			22118400L
#define T1MS			(65536-SYSCLK/12/1000)


#define T10MS			(65536-SYSCLK/12/100)
//#define T20MS			(65536-SYSCLK/12/50)

#define T20MS			(65536-SYSCLK/12/5)

#define T01MS			(65536-SYSCLK/12/10000)
#define NMS				10
// 10ms



// in mm
//#define PPMM	8.0
//pulse per mm
//#define	RPM2F	2500/60
// pulse per sec,RPM to frequency

#define START_SPEED	100
//6% of full scal
#define MAX_SPEED	2000.0
// in RPM


//-----------------------------------------------------------------------------
// Modbus Defines
//-----------------------------------------------------------------------------
#define REG_HOLDING_START   	0x1	//Hold寄存器的起始编号
#define REG_HOLDING_NREGS     	32+32	   	//Hold寄存器的数量，每个寄存器为2个字节

/* ----------------------- Static variables ---------------------------------*/

static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];
unsigned char status;
idata unsigned int curver_1[16],curver_2[16];
unsigned int curver_1u[16],curver_2u[16],curver_1d[16],curver_2d[16];
unsigned int curver_1u2[16];
float curver_temp[16];
unsigned int	var[32];
unsigned int real_up;
bit f_save;
bit f_forward,f_backward;
bit f_first;
bit CW,CCW;

void init_sys();
void init_port();
void init_timer0();
void init_timer1();
void init_curve();
void delay1ms();
void delay_nms(int);

void delay01ms();
void forward();
void backward();
void forwardm1();
void backwardm1();
void find_z();

unsigned char n1,n2;
unsigned char cych,cycl;
unsigned int cycle;
unsigned int thl0;
unsigned char th0,tl0;
unsigned char input;
unsigned char output;

idata unsigned char shadow_th0,shadow_tl0;
float ppmm;

int count_01ms;
unsigned int count_ms;
unsigned int count_10ms;
unsigned int t_chsp,t_chsp1,t_chsp2,t_chsp3;

bit f_sec; 
bit f_nms;
bit f_forward,f_backward;
bit f_change_speed;
unsigned int pulse;
unsigned long pulse_l_o,pulse_l,pulse_l_u,pulse_l_d;
unsigned long pulse_save;
const unsigned int CURVER1[16]=
{
//65362,
65300,
65380,

65442,
65466,
65484,
65496,
65504,
65509,
65512,
65516,
65517,
65519,
65520,
65520,
65522,
65522


};
const unsigned int CURVER2[16]=
{	
65522,
65522,
65520,
65520,
65519,
65517,
65516,
65512,	
65509,	
65504,	
65496,	
65484,	
65466,	
65442,	
65380,	
65300

}	 ;
const unsigned int CURVER3[16]=
{
1,
3,
6,
10,
15,
21,
28,
36,
43,
49,
54,
58,
61,
63,
64,
64

};



void main(void)
{
   eMBErrorCode    eStatus;
	unsigned char i;
//---------------------******-----------------	
	init_sys();
	init_port();
	init_curve();

	output=0;
	PULSE=0;
	f_forward=1;
	f_backward=1;
	f_first=1;
	

	init_timer0();
	init_timer1();
	EA=1;

	//led flash--------
	output=0x55;
	WRITEP0;
	delay_nms(1000);
	output=0x00;
	WRITEP0;
	delay_nms(1000);
	output=0xaa;
	WRITEP0;
	delay_nms(1000);
	output=0x00;
	WRITEP0;
	delay_nms(1000);
	//led flash--------
	

	
//---------------------******-----------------	
	
	EA=1;				//允许所有中断
//--------------- Modbus Config -------------------------------------------------

    eStatus = eMBInit( MB_RTU, 0x0B, 1, 19200, MB_PAR_NONE ); 
	    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );
	
		status=STOP;
		f_save=0;
	
//--------------- Main Loop -------------------------------------------------
    delay_nms(100);
		find_z();
		for( ;; )
    {
        ( void )eMBPoll(  );
				if(pulse_active)
				{
					f_save=0;
					pulse_save=pulse_l;
					pulse_l=pulse_active;
					forward();
					pulse_l=pulse_save;
					pulse_active=0;
				
				}
				if(f_save)
				{
					
					Iap_Erase(0);
					for(i=0;i<16;i++)
					{
						Iap_Write((unsigned int)i,curver_1[i]);
						Iap_Write((unsigned int)i+16,curver_2[i]);
					}
					for(i=0;i<32;i++)
					{
 						Iap_Write((unsigned int)i+32,(unsigned char )(var[i]>>8)&0x0ff);
						Iap_Write((unsigned int)i+64,(unsigned char )var[i]&0x0ff);
					
					}	
					init_curve();
					f_save=0;
				}
				
//-------------------------				
				READP0;
				switch(mode)
				{
					case MODE_UNIDIRECT:
						
						if(KEY_BACKWARD)
						{
								
								task=1;
								output|=0x01;
								WRITEP0;
							/*
								if(INCDEC)
									pulse_l+=step;
								else pulse_l-=step;
							*/	
							if(pulse_l>pulse_l_u)pulse_l=pulse_l_u;
								else if(pulse_l<pulse_l_d)pulse_l=pulse_l_d;
								//debug=pulse_l;                                                                                                                                                                                                                   
								
								forward();
							//add after ver 30	
								READP0;
								if(INCDEC)
							//	if(TOC2)//for test only
									pulse_l-=step;
								else pulse_l+=step;
								//end add after ver 30
						}
			//wait for key release...
						while(1)
						{
							READP0;
							if(!KEY_BACKWARD)  
							{
								delay_nms(200);
								break;
							}
							
						}
			// released.
													
						output&=~0x01;
						WRITEP0;
						task=0;
					
						break;
					case MODE_RECIPRO:
						if(KEY_FORWARD)
						{
							if(!f_forward)	
							{
								task=1;
								output|=0x01;
								WRITEP0;
								
								forwardm1();
								
							}
			//wait for key release...
							while(1)
							{
								READP0;
								if(!KEY_FORWARD)  
								{
									delay_nms(200);
									break;
								}
								
							}
// released.
										
							f_forward=1;
							f_backward=0;
						
							output&=~0x01;
								WRITEP0;
							task=0;
						
						
						}
			//*************2********************
						else if(KEY_BACKWARD)
						{
							if(!f_backward)
							{
								task=2;
								output|=0x02;
								WRITEP0;
								
								backwardm1();
							
							}

			//wait for key release...
							while(1)
							{
								READP0;
								if(!KEY_FORWARD)  
								{
									delay_nms(200);
									break;
								}
								
							}
			//key released
							
							f_forward=0;
							f_backward=1;

							output&=~0x02;
							WRITEP0;
						}
						
						break;
					case MODE_2:
						break;
					case MODE_3:
						break;
					default:
						break;
					
					
				}		

		}//end for
	}    

//-----------------------------------------------------------------------------
// eMBRegHoldingCB
//-----------------------------------------------------------------------------
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart);
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
							if(iRegIndex<32)
							{
								*pucRegBuffer++ = ( unsigned char )( var[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( var[iRegIndex] & 0xFF );
							}
							else if(iRegIndex<32+16)
							{
								*pucRegBuffer++ = ( unsigned char )( curver_1[iRegIndex-32] >>8 );;
                *pucRegBuffer++ = ( unsigned char )( curver_1[iRegIndex-32] & 0xFF );
							}
							else if(iRegIndex<48+16)
							{
								*pucRegBuffer++ = ( unsigned char )( curver_2[iRegIndex-48]>>8);
                *pucRegBuffer++ = ( unsigned char )( curver_2[iRegIndex-48] & 0xFF );
							}
							
							
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
          	if(status==STOP)
						{
							while( usNRegs > 0 )
							{
															
								if(iRegIndex<32)
								{
										var[iRegIndex]=*pucRegBuffer++;
										var[iRegIndex]<<=8;
										var[iRegIndex]+=*pucRegBuffer++;				
										f_save=1;
								}	
								else if(iRegIndex<32+16)
								{
										curver_1[iRegIndex-32]=*pucRegBuffer++;	
										curver_1[iRegIndex-32]<<=8;
										curver_1[iRegIndex-32]+=*pucRegBuffer++;
										//f_save=1;
								}
									else if(iRegIndex<48+16)
									{
										curver_2[iRegIndex-48]=*pucRegBuffer++;	
										curver_2[iRegIndex-48]<<=8;		
										curver_2[iRegIndex-48]+=*pucRegBuffer++;
									//	f_save=1;
									}
																				
									iRegIndex++;
									usNRegs--;
							}
						}
						else eStatus =MB_EINVAL;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

void init_sys()
{
	CLK_DIV=0;
}
void init_port()
{
	
	P1M1=0x00;
	P1M0=0x00;
	P2M1=0x00;
	P2M0=0x00;
	P3M1=0x00;
	P3M0=0x00;

	P1=0xff;
	P2=0xff;
	P3=0xff;  
	
	IN_OE=1;
	P0=0;
 	OUT_LE=1;
	OUT_OE=0;
	OUT_LE=0;

}
void	init_curve()

{
	unsigned char i;
	int sp_delta;
	signed long int tc;
		
	
		
		for(i=0;i<32;i++)
		{
			 			
			var[i]=Iap_Read((unsigned int)i+32);
			var[i]<<=8;
			var[i]+=Iap_Read((unsigned int)i+64);
		}		

		if(speed3>2500)speed3=2500;
		else if(speed3<1)speed3=1;
		
		for(i=0;i<16;i++)
		{
			//curver_1[i]=Iap_Read((unsigned int)i);
			//curver_2[i]=Iap_Read((unsigned int)i+16);
	
	//----------------------------
		//	curver_1[i]=CURVER1[i];
		//	curver_2[i]=CURVER2[i];
			
			//if(mode==MODE_RECIPRO)
			{
					//------------speed3 up and down--------------
				tc=((long)(65536-CURVER1[i]))*2500/speed3;
				if(tc>65535)tc=65535;
				
				curver_1[i]=65536-tc;
				//if(curver_1[i]<147)curver_1[i]=147;
				
					//--------------------------
				tc=((long)(65536-CURVER2[i]))*2500/speed3;
				if(tc>65535)tc=65535;
				
				curver_2[i]=65536-tc;
				//if(curver_2[i]<147)curver_2[i]=147;
				
				
				
				//---------max speed is speed1-----------------
				tc=((long)(65536-CURVER1[i]))*2500/max_speed;
				if(tc>65535)tc=65535;
				
				curver_1u[i]=65536-tc;
				//if(curver_1u[i]<147)curver_1u[i]=147;
				
		//----------------------------
				
				tc=((long)(65536-CURVER1[i]))*2500/step;
				if(tc>65535)tc=65535;
				
				curver_2u[i]=65536-tc;
				//if(curver_2u[i]<147)curver_2u[i]=147;
				//-------------------------------------
					
				
				//--------------------------
				tc=((long)(65536-CURVER2[i]))*2500/max_speed;
				if(tc>65535)tc=65535;
				
				curver_1d[i]=65536-tc;
				//if(curver_1d[i]<147)curver_1d[i]=147;
				
		//----------------------------
				
				tc=((long)(65536-CURVER2[i]))*2500/step;
				if(tc>65535)tc=65535;
				
				curver_2d[i]=65536-tc;
				//if(curver_2d[i]<147)curver_2d[i]=147;
				//-------------------------------------
			}
	//----------------------------
		}
			
		//--****************get 1u2********************
		sp_delta=step-max_speed;
		for(i=0;i<16;i++)//get speed point --RPM
		{
			curver_temp[i]=1.0*sp_delta*CURVER3[i]/CURVER3[15]+max_speed;
		}
		for(i=0;i<16;i++) //change to freques
		{
			curver_temp[i]=curver_temp[i]*2500/60;
		}
		for(i=0;i<16;i++) //change to counter
		{
			curver_temp[i]=SYSCLK/curver_temp[i]/12;
		}
		for(i=0;i<16;i++) //change to counter
		{
			curver_1u2[i]=65536-curver_temp[i];
		}
		//--******************get 1u2**********************
		
		
		ppmm=line/coeffcient*fenzi/fenmu;
		if(!ppmm)ppmm=8;
		//debug=ppmm;
		pulse_l_o=length1*ppmm;
		pulse_l_u=pulse_l_o*1.1;
		pulse_l_d=pulse_l_o*0.9;
		pulse_l=pulse_l_o;
//		if(mode==MODE_UNIDIRECT)debug=pulse_l;
		
		real_up=up;
//		stop_point=up;
		
		if((coeffcient==0xffff)||(coeffcient==0))
		{//first new chip;
			coeffcient=325;
			max_speed=2500;
			line=2500;
			mode=0;
			length1=2350;
			step=0;
			fenzi=1000;
			fenmu=1000;
			real_up=4500;
			interleave=10;
		}
		if((interleave==0)||(interleave>20))interleave=10  ;
		t_chsp1=(65536-SYSCLK/12/1000*interleave);
		if((interleave1==0)||(interleave1>20))interleave1=10  ;
		t_chsp2=(65536-SYSCLK/12/1000*interleave1);
		if((interleave2==0)||(interleave2>20))interleave2=10  ;
		t_chsp3=(65536-SYSCLK/12/1000*interleave2);
	
	

}
void init_timer1()
{
 	TMOD &= 0x0F;	
	TMOD |= 0x10;	

	ET1=1;
}



void init_timer0()
{

   	TMOD &= 0xF0;	
	TMOD |= 0x01;
	ET0=1;

}


void timer0 (void) interrupt 1  using 1
{  
//reload
//	TEST=0;
	TR0=0;
	TH0=shadow_th0;
	TL0=shadow_tl0;
	
	TR0=1;

	if(pulse)
	{
	 	if(PULSE)
		{
			PULSE=0;
			pulse--;
		}
		else 
		{
			PULSE=1;
		}
 	}
	else 
	{
		if(PULSE)
		{
			PULSE=0;
		}
		else 
		{
			PULSE=1;
		}
		TR0=0;

	}
//	TEST=1;
	
}
void timer1 (void) interrupt 3  using 2
{
	TL1=t_chsp;
	TH1=t_chsp>>8;
		f_change_speed=1;
	 	TEST=!TEST;
	
}

void find_z()
{
	long int i;
	output|=0x0f;
	WRITEP0;
	delay_nms(1000);//add v38 ,wait for sevice to ready.
	delay_nms(1000);//add v38
	switch(mode)
	{
		case MODE_UNIDIRECT:
	
			if(direct1){CW=0;CCW=1;}
			else {CW=1;CCW=0;}
			break;
		case MODE_RECIPRO:
		
			if(direct1){CW=0;CCW=1;}
			else {CW=1;CCW=0;}
		
			READP0;
			if(TOC1)
			{
				DIRECT=CW;
				for(i=0;i<1100;i++)
				{
					PULSE=1;
					delay_nms(2);
					PULSE=0;
					delay_nms(2);
					READP0;
					if(!TOC1)break;
				}
			
			} 	

			READP0;
			delay_nms(500);
			DIRECT=CCW;
			if(TOC1)//while(1);//modify to back to orig postion
				//err!!!!!!!!!!!!!!!!
			{
				for(i=0;i<1100;i++)
				{
					PULSE=1;
					delay_nms(2);
					PULSE=0;
					delay_nms(2);
					READP0;
					if(!TOC1)break;
				}
			}
			else
			{
				for(i=0;i<length1*ppmm;i++)
				{
					READP0;
					if(TOC1) break;	
					
					PULSE=1;
					delay_nms(2);
					PULSE=0;
					delay_nms(2);
				
				}
			
				output=0;
				WRITEP0;

				for(i=0;i<500;i++)
				{
					
					PULSE=1;
					delay_nms(2);
					PULSE=0;
					delay_nms(2);
				
				}
			}
			if(direct1)
				{
				//v14
				f_forward=1;
				f_backward=0;
					
			}
			else
			{
				//v16
				f_forward=0;
				f_backward=1;
				CW=0;CCW=1;
			}
			break;
		case MODE_2:
			break;
		case MODE_3:
			break;
		default:
			break;
	}
	output&=0xf0;
	WRITEP0;

}
void forward()
{
	unsigned  int n1,n2,n3;
//	unsigned long slow,pulse_slow;
	unsigned int pulse_h;
//	unsigned int pulse_h22;
	unsigned char mult_pulse;
	unsigned int pulse_remainder;
	int i;

	if(max_speed>2500)max_speed=2500;
	if(max_speed<1)max_speed=1;
	

	n1=65535-2500*8/max_speed;
	
	if(debug>2000)debug=2000;
	if(debug<0)debug=0;

	if(debug)n2=65535-2500*8/debug;//debug used as last slow speed
	else n2=0;
	
	if(speed4>1000)speed4=1000;
	if(speed4<0)speed4=0;

	if(speed4)n3=65535-2500*8/speed4;//debug used as last slow speed
	else n3=0;
	
	real_up=up;//add @20150531
	if(pulse_l<real_up*2)
	{
		mult_pulse=0;
		pulse_remainder=0;
		pulse_h=pulse_l/2;
		top=0;
	//	n1=pulse_l*n1/real_up/2;
		if(n1<147){n1=147;}
		
	}
	else
	{
		//top=pulse_l-real_up-real_up;
		mult_pulse=(pulse_l-real_up-real_up)/(unsigned int)0xffff;
		pulse_remainder=(pulse_l-real_up-real_up)%(unsigned int)0xffff;
		pulse_h=real_up;
	}

	DIRECT=CW;

	//-------------up------------------------		
		pulse=pulse_h;
		shadow_tl0=curver_1u[0]&0x0ff;
		shadow_th0=curver_1u[0]>>8;
		TH0=shadow_th0;
		TL0=shadow_tl0;
		TR0=1;
	
	//
	
		f_change_speed=0;
		t_chsp=t_chsp1;
		TL1=t_chsp;
		TH1=t_chsp>>8;

		TR1=1;
		i=1;
		while(i<=15)
		{  	
			while(!f_change_speed)
			{
				if(!pulse){i=15;TR1=0;break;}
			};
			if(curver_1u[i]<n1)
			{//TH0=curver_1u[i];
				shadow_tl0=curver_1u[i]&0x0ff;
				shadow_th0=curver_1u[i]>>8;	
			
			}
			else 
			{//TH0=n1;
				shadow_tl0=n1&0x0ff;
				shadow_th0=n1>>8;
			
			
			}
			i++;
			f_change_speed=0;		
		};
		TR1=0; 
		while(pulse)//wait up finishi
		{
			READP0;
		//	if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} 
	
	//top1

	if(pulse_remainder)pulse=pulse_remainder;
	TR0=1;
	while(pulse)
	{
		READP0;
		//if(TOC2)
		//{TR0=0;pulse=0;return;};	
	} //wait...;
	
		
	for(i=0;i<mult_pulse;i++)
	{
		pulse=(unsigned int)0xffff;
		TR0=1;
		while(pulse)
		{
			READP0;
		//	if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} //wait...;
		
	}
//top1 finishi
	
	//-------------down------------------------		
		pulse=pulse_h;
		TR0=1;
	
		f_change_speed=0;
		t_chsp=t_chsp1;
		TL1=t_chsp;
		TH1=t_chsp>>8;

		TR1=1;
		i=1;
		while(i<=15)
		{  	
			while(!f_change_speed)
			{
				if(!pulse){i=15;TR1=0;break;}
			};
			//if(curver_1d[i]<TH0)TH0=curver_1d[i];
			if(curver_1d[i]<(shadow_tl0+(unsigned int)(shadow_th0<<8)))
			{
				shadow_tl0=curver_1d[i]&0x0ff;
				shadow_th0=curver_1d[i]>>8;
				if((shadow_tl0+(unsigned int)(shadow_th0<<8))<n2)
				{
					shadow_tl0=n2&0x0ff;
					shadow_th0=n2>>8;
				}
				
			}
			
			i++;
			f_change_speed=0;		
		};
		TR1=0; 
		while(pulse)
		{
			READP0;
		//	if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} //wait...;
		
		
//add midle speed if debug!=0
		if(n2)
		{
			TR0=0;
			shadow_tl0=n2&0x0ff;
			shadow_th0=n2>>8;
			TH0=shadow_th0;
			TL0=shadow_tl0;
			TR0=1;
			for(i=0;i<50;i++)
			{
				pulse=0x0ffff;
				while(pulse)//wait down finishi
				{
					READP0;
					if(TOC2)
					{TR0=0;pulse=0;break;};	
				}
			}		
		}
//add slowest speed if speed4!=0		
		if(n3)
		{
			TR0=0;
			shadow_tl0=n3&0x0ff;
			shadow_th0=n3>>8;
			TH0=shadow_th0;
			TL0=shadow_tl0;
			TR0=1;
			for(i=0;i<10;i++)
			{
				pulse=0x0ffff;
				while(pulse)//wait down finishi
				{
					READP0;
					if(TOC1)
					{TR0=0;pulse=0;return;};	
				}
			}		
		}

}


void delay1ms()
{
	int i;
 	for(i=0;i<420;i++);
}
void delay01ms()
{
	int i;
 	for(i=0;i<10;i++);
}
void delay_nms(int ms)
{
	int i;
 	for(i=0;i<ms;i++)delay1ms();
}

void forwardm1()
{
	unsigned char i;
	unsigned int pulse_least;
	unsigned int pulse_h;
	unsigned char mult_pulse;
	unsigned int pulse_remainder;
	
	DIRECT=CCW;
	
	real_up=up;//add @20150531
	
	if(pulse_l<real_up*2)
	{
		mult_pulse=0;
		pulse_remainder=0;
		pulse_h=pulse_l/2;
	}
	else
	{
		mult_pulse=(pulse_l-real_up-real_up)/(unsigned int)0xffff;
		pulse_remainder=(pulse_l-real_up-real_up)%(unsigned int)0xffff;
		pulse_h=real_up;
	}
	
	
	pulse=pulse_h;
	//debug=pulse_remainder;


//-------------up------------------------		
		i=0;
	//	TH0=curver_1[0];
	//	TL0=curver_1[0];
	
		shadow_tl0=curver_1[0]&0x0ff;
		shadow_th0=curver_1[0]>>8;
		TH0=shadow_th0;
		TL0=shadow_tl0;
		TR0=1;

		TR0=1;
	
	//	count_280us=0;
		f_change_speed=0;
		t_chsp=t_chsp1;
		TL1=t_chsp;
		TH1=t_chsp>>8;

		TR1=1;
		while(i<15)
		{  	
			while(!f_change_speed);
			i++;
			f_change_speed=0;
			//TH0=curver_1[i];
			shadow_tl0=curver_1[i]&0x0ff;
			shadow_th0=curver_1[i]>>8;
					
		};
		TR1=0; 
	
		pulse_least=pulse;
	//	real_up=pulse_h-pulse;
	//	debug=real_up;
		while(pulse)
		{
			READP0;
		//	if(TOC1)
		//	{TR0=0;pulse=0;return;};	
		} //wait up ok....;
//-----------top segment----------------			
//	  TEST=0; 		
//-------------high speed-------------------------
		
		if(pulse_remainder)pulse=pulse_remainder;
		TR0=1;
//--- remainder ok---		
		
		for(i=0;i<mult_pulse;i++)
		{
			pulse=(unsigned int)0xffff;
			while(pulse)
			{
				READP0;
			//	if(TOC1)
			//	{TR0=0;pulse=0;return;};	
			} //wait...;
			
			TR0=1;
		}

//----------above is top----------------	

//-------------high speed end------------------
	
//------------down1--------------------------------
		i=0;

		f_change_speed=0;
		t_chsp=t_chsp1;
		TL1=t_chsp;
		TH1=t_chsp>>8;
		
		while(pulse)
		{
			READP0;
		//	if(TOC1)
		//	{TR0=0;pulse=0;return;};	
		} //wait...;
		
		
		pulse=pulse_h;
		TR0=1;
		//TH0=curver_2[0];
		//TL0=curver_2[0];
		
		shadow_tl0=curver_2[0]&0x0ff;
		shadow_th0=curver_2[0]>>8;
		TH0=shadow_th0;
		TL0=shadow_tl0;
		TR1=1;
		//TEST=1;
		while(i<15)
		{  	
			while(!f_change_speed);
			i++;
			f_change_speed=0;
			//TH0=curver_2[i];
			
			shadow_tl0=curver_2[i]&0x0ff;
			shadow_th0=curver_2[i]>>8;
			
		}
		TR1=0; 


//------------down1 ok--------------------------------
		 

		while(pulse)
		{	
			READP0;
		//	if(TOC1)
		//	{TR0=0;break; };
		}

		//real_up=pulse_h-pulse_least;
		up=real_up;
		down=up;
		top=pulse_remainder;
		if(f_first){f_first=0;f_save=1;}
		//debug=mult_pulse;
		time_used=(pulse_l-real_up-real_up)*0.021+320;
}

void backwardm1()
{
	unsigned int n1,n2;
	unsigned long slow,pulse_slow;
	unsigned int pulse_h;
	unsigned int pulse_h22;
	unsigned char mult_pulse;
	unsigned int pulse_remainder;
	int i;
	bit f_n1n2;

	

	if(max_speed>2500)max_speed=2500;
	if(step>2500)step=2500;
	if(max_speed<1)max_speed=1;
	if(step<1)step=1;
	if(debug>100)debug=100;
	if(debug<0)debug=0;

	n1=65535-2500*8/max_speed;
	n2=65535-2500*8/step;
	
	pulse_l=length1*ppmm;
	slow=pulse_l/100*debug;
	pulse_slow=pulse_l-slow;
	
	real_up=up1;//add @20150531
	if(pulse_slow<real_up*2)
	{
		mult_pulse=0;
		pulse_remainder=0;
		pulse_h=pulse_slow/2;
		top=0;
		n1=pulse_slow*n1/real_up/2;
		if(n1<147){n1=147;}
		
	}
	else
	{
		//top=pulse_l-real_up-real_up;
		mult_pulse=(pulse_slow-real_up-real_up)/(unsigned int)0xffff;
		pulse_remainder=(pulse_slow-real_up-real_up)%(unsigned int)0xffff;
		pulse_h=real_up;
	}

	DIRECT=CW;

	//-------------up------------------------		
		pulse=pulse_h;
		//TH0=curver_1u[0];
		//TL0=curver_1u[0];
	
		shadow_tl0=curver_1u[0]&0x0ff;
		shadow_th0=curver_1u[0]>>8;
		TH0=shadow_th0;
		TL0=shadow_tl0;
		TR0=1;
	
	//
	
		f_change_speed=0;
		t_chsp=t_chsp2;
		TL1=t_chsp;
		TH1=t_chsp>>8;

		TR1=1;
		i=1;
		while(i<=15)
		{  	
			while(!f_change_speed)
			{
				if(!pulse){i=15;TR1=0;break;}
			};
			if(curver_1u[i]<n1)//TH0=curver_1u[i];
			{
				shadow_tl0=curver_1u[i]&0x0ff;
				shadow_th0=curver_1u[i]>>8;
			}
			else //TH0=n1;
			{
				shadow_tl0=n1&0x0ff;
				shadow_th0=n1>>8;
			}
			i++;
			f_change_speed=0;		
		};
		TR1=0; 
		while(pulse)//wait up finishi
		{
			READP0;
		//	if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} 
	
	//top1

	if(pulse_remainder)pulse=pulse_remainder;
	TR0=1;
	while(pulse)
	{
		READP0;
		//if(TOC2)
		//{TR0=0;pulse=0;return;};	
	} //wait...;
	
		
	for(i=0;i<mult_pulse;i++)
	{
		pulse=(unsigned int)0xffff;
		TR0=1;
		while(pulse)
		{
			READP0;
		//	if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} //wait...;
		
	}
//top1 finishi

//---------------------------------------
	// check KEY_BACKWARD 
	pulse=pulse_h;
	TR0=1;

	READP0;
	if((KEY_BACKWARD)&&(slow!=0))  //if key down..,edit by 2015.7.12
	{			
		while(pulse)
		{
			READP0;
			//if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} //wait...;
		f_n1n2=1;
	}

	else						//no key down,stop,then wait for re_active 
	{
	//-------------down------------------------		
	//	pulse=pulse_h;
	//	TR0=1;
	
		f_change_speed=0;
		t_chsp=t_chsp2;
		TL1=t_chsp;
		TH1=t_chsp>>8;

		TR1=1;
		i=1;
		while(i<=15)
		{  	
			while(!f_change_speed)
			{
				if(!pulse){i=15;TR1=0;break;}
			};
			//if(curver_1d[i]<TH0)TH0=curver_1d[i];
			if(curver_1d[i]<(shadow_tl0+(unsigned int )(shadow_th0<<8)))
			{
				shadow_tl0=curver_1d[i]&0x0ff;
				shadow_th0=curver_1d[i]>>8;
			}
			i++;
			f_change_speed=0;		
		};
		TR1=0; 
		
		while(pulse)//wait down finishi
		{
			READP0;
		//	if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} 
		
		while(1)//wait key down.
		{
			READP0;
			if(KEY_BACKWARD)  
			{			
				break;
			}
			delay_nms(5);
		}
		f_n1n2=0;
	}
	if(slow!=0)
	{
//2s section  start	-------------------------------
	
		if(f_n1n2)real_up=(up1>=up2?up1:up2);//add @20150531
		else real_up=up2;
		if(real_up==up2)
		{//use up1,up2,up2
			if(slow<real_up*2)
			{
				mult_pulse=0;
				pulse_remainder=0;
				pulse_h=slow/2;
				top=0;
				n2=pulse_slow*n2/real_up/2;
				if(n2<147)n2=147;
				
			}
			else
			{
				mult_pulse=(slow-real_up-real_up)/(unsigned int)0xffff;
				pulse_remainder=(slow-real_up-real_up)%(unsigned int)0xffff;
				pulse_h=real_up;
				
			}
			pulse_h22=up2;
		}
		else
			
		{//use up1,up1,up2
			if(slow<up1+up2)
			{
				mult_pulse=0;
				pulse_remainder=0;
				pulse_h=slow/2;
				top=0;
				n2=pulse_slow*n2/real_up/2;
				if(n2<147)n2=147;
				
			}
			else
			{
				mult_pulse=(slow-up1-up2)/(unsigned int)0xffff;
				pulse_remainder=(slow-up1-up2)%(unsigned int)0xffff;
				pulse_h=real_up;
				
			}
			pulse_h22=up2;
			
			
		}
			//-------------up------------------------		
		pulse=pulse_h;
		f_change_speed=0;
		if(f_n1n2)t_chsp=(up1>=up2?t_chsp2:t_chsp3);//add @20150531
		else 	t_chsp=t_chsp3;
		TL1=t_chsp;
		TH1=t_chsp>>8;
		TR0=1;
		if(!f_n1n2)//is n1->0->n2
		{
			//TH0=curver_1[0];
			//TL0=curver_1[0];
			shadow_th0=curver_1[0]>>8;
			shadow_tl0=curver_1[0]&0x0ff;
			
			TL0=shadow_tl0;
			TH0=shadow_th0;
		//
		
			TR1=1;
			i=1;
			while(i<=15)
			{  	
				while(!f_change_speed)
				{
					if(!pulse){i=15;TR1=0;break;}
				};
				if(curver_2u[i]<n2)//TH0=curver_2u[i];
				{
					shadow_th0=curver_2u[i]>>8;
					shadow_tl0=curver_2u[i]&0x0ff;
				}
				else //TH0=n2;
				{
					shadow_th0=n2>>8;
					shadow_tl0=n2&0x0ff;
				}
				i++;
				f_change_speed=0;		
			};
			TR1=0; 
		}
		else//is n1->n2
		{
				TR1=1;
				i=1;
				while(i<=15)
				{  	
					while(!f_change_speed)
					{
						if(!pulse){i=15;TR1=0;break;}
					};
					if(n1<curver_1u2[i]<n2)//TH0=curver_1u2[i];
					{
						shadow_th0=curver_1u2[i]>>8;
						shadow_tl0=curver_1u2[i]&0x0ff;
					}
					else //{TH0=n2;TR1=0;break;}
					{
						shadow_th0=n2>>8;
						shadow_tl0=n2&0x0ff;
						TR1=0;break;
					}
					i++;
					f_change_speed=0;		
				};
				TR1=0; 
	
			
			f_n1n2=0;
		}
		while(pulse)//wait pulse_h finishi
		{
			READP0;
		//	if(TOC2)
			//{TR0=0;pulse=0;return;};	
		} 
	
		
	
//--------high_top------------
		if(pulse_remainder)pulse=pulse_remainder;
		TR0=1;
		while(pulse)
		{
			READP0;
		//	if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} //wait...;
		
			
		for(i=0;i<mult_pulse;i++)
		{
			pulse=(unsigned int)0xffff;
			TR0=1;
			while(pulse)
			{
				READP0;
		//		if(TOC2)
			//	{TR0=0;pulse=0;return;};	
			} //wait...;
			
		}

//--------high_top------------		

		pulse=pulse_h22;
		TR0=1;
		f_change_speed=0;
		t_chsp=t_chsp3;
		TL1=t_chsp;
		TH1=t_chsp>>8;
		TR1=1;
		i=1;
		while(i<=15)
		{  	
			while(!f_change_speed)
			{
				if(!pulse){i=15;TR1=0;break;}
			};
			//if(curver_2d[i]<TH0)TH0=curver_2d[i];
			if(curver_2d[i]<(shadow_tl0+(unsigned int)(shadow_th0<<8)))
			{
				shadow_th0=curver_2d[i]>>8;
				shadow_tl0=curver_2d[i]&0x0ff;
			}
			i++;
			f_change_speed=0;		
		};
		TR1=0; 
		
		while(pulse)//wait pulse_h finishi
		{
			READP0;
		//	if(TOC2)
		//	{TR0=0;pulse=0;return;};	
		} 
	
		
	}
	//add to mode1 if length3!=0;
	if(length3)
	{
		output|=0x04;
		WRITEP0;
		
		if(speed4>2000)speed4=2000;
		if(speed4<1)speed4=1;
		n2=65535-2500*8/speed4;
		shadow_th0=n2>>8;
		shadow_tl0=n2&0x0ff;
		TR0=0;
		TH0=shadow_th0;
		TL0=shadow_tl0;
		while(1)
		{
			READP0;
			if(KEY_JOG_BACKWARD)  
			{
				delay_nms(200);
				break;
			}
			
		}
		for(i=0;i<5;i++)
		{
			pulse=length3;
			TR0=1;
			while(pulse)//wait pulse_h finishi
			{
				READP0;
			//	if(TOC2)
			//	{TR0=0;pulse=0;return;};	
			}
		}
		delay_nms(2000);
		DIRECT=CCW;
		for(i=0;i<5;i++)
		{		
		pulse=length3;
			
			TR0=1;
			while(pulse)//wait pulse_h finishi
			{
				READP0;
			//	if(TOC2)
			//	{TR0=0;pulse=0;return;};	
			} 		
		}	
		while(1)
		{
			READP0;
			if(!KEY_JOG_BACKWARD)  
			{
				delay_nms(200);
				break;
			}
			
		}
		output&=~0x04;
		WRITEP0;
		
		
	}
	
//2s section  end -------------------------------------

}