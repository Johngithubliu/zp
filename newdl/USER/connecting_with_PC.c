//unsigned char *conneting_ScorBoard(void);
#include "string.h"
extern void _ltoa( char *p, long wt);

extern union Modbus_packed    
{    
	long weight_long; 
	unsigned char long_weight[4];   
}Modbus_weight;
extern char FIN;


extern char Dis_ad[20];	
extern 	char ADresult[20];
extern 	int Cal_point;//标定小数点
char NDS1;
unsigned char status[6];
unsigned char status_ScorBoard[10];
//unsigned char *status_and_datum(void);

//发送数据
//数据的状态传送

 //   NDS1	 
unsigned char *status_and_datum(void)

{
unsigned char i,lenx;
char *p;
	status[0]=(char)Cal_point&0x3;//标定小数点
	if(FIN)
		status[0]|=0x08; //FIN=1 是发5位称重结果  FIN=0 是发实时称重显示数据
	
	if(NDS1)   //若处于设定,标定界面,则连续发"******"	no_scale
		{
			for(i=0;i<6;i++)
			{status[i]='*';}
		}
	else
		{
			if(FIN)	//FIN=1 是发5位称重结果
				{p=ADresult;
				 if(*p==0x16)status[0]|=0x04; //符号位
				 status[0]|=0x30; //变ASCII
				}
			else   //FIN=0 是发实时称重显示数据
				{					
//					_ltoa(Dis_ad,Modbus_weight.weight_long);				
					p=Dis_ad;
					if(*p==0x16)status[0]|=0x04; //符号位
					status[0]|=0x30; //变ASCII
					 if(*p==0x16)
					 	{
							 p++;
							 lenx=strlen(Dis_ad)-1;//jump point byte
						}
					 else
							 lenx=strlen(Dis_ad);
				}
			if(lenx>=5)
			  {
			  	for(i=1;i<6;i++)
					{status[i]=*p++;}
			  }
			 else
			  {
				for(i=1;i<=5-lenx;i++)
					{status[i]='X';}//<5,头部分，发‘X’	
			  	for(;i<6;i++)
					{status[i]=*p++;}
			  }
			FIN=0;
		}
  return 	status;	//returning strings  to be sent			  Dis_ad:"-" =0x16,后面ascii码 以零00 结尾
}


//unsigned char *conneting_ScorBoard(void)
//{
//unsigned char i,lenx,xor_buffer;
//char *p;
//	 		p=Dis_ad;
//			xor_buffer=0xfa;
//			status_ScorBoard[0]=0xfa;
//
//
//			if(*p==0x16)
//				{lenx=strlen(Dis_ad)-1;status_ScorBoard[6]=22;p++;}//如果为负数，向大屏幕发显示码=22即："-")
//		    else
//				{lenx=strlen(Dis_ad);status_ScorBoard[6]=0x10;}
//			if(lenx>=6)
//			  {	
//			  	for(i=1;i<6;i++)
//					{
//						status_ScorBoard[i]=*p++;
//						status_ScorBoard[i]&=0x0f;
//					}
//			  }
//		    else
//			  {	
//				for(i=1;i<=5-lenx;i++)
//					{status_ScorBoard[i]=0x10;}//头部分，发0x10黑码(大屏幕接收的显示码)	
//			  	for(;i<6;i++)
//					{status_ScorBoard[i]=*p++;status_ScorBoard[i]&=0x0f;}
//			  }
//			  status_ScorBoard[7]=(char)Cal_point&0x3;//标定小数点
//
//			  for(i=1;i<8;i++)
//				 {xor_buffer^=status_ScorBoard[i];}
//			   status_ScorBoard[8]=xor_buffer;
//			   status_ScorBoard[9]=0xbf;
//			   return 	status_ScorBoard;	
//
//
//
//
//		 
//
//}

unsigned char *conneting_ScorBoard(void)
{
unsigned char i,lenx,xor_buffer;
char *p;
	 		p=Dis_ad;
			xor_buffer=0xfa;
			status_ScorBoard[0]=0xfa;


			if(*p==0x16)
				{lenx=strlen(Dis_ad)-1;status_ScorBoard[6]=22;p++;}//如果为负数，向大屏幕发显示码=22即："-")
		    else
				{lenx=strlen(Dis_ad);status_ScorBoard[6]=0x10;}
			if(lenx>=6)
			  {	
			  	for(i=1;i<6;i++)
					{
						status_ScorBoard[i]=*p++;
						status_ScorBoard[i]&=0x0f;
					}
			  }
		    else
			  {	
				for(i=1;i<=5-lenx;i++)
					{status_ScorBoard[i]=0x10;}//头部分，发0x10黑码(大屏幕接收的显示码)	
			  	for(;i<6;i++)
					{status_ScorBoard[i]=*p++;status_ScorBoard[i]&=0x0f;}
			  }
			  status_ScorBoard[7]=(char)Cal_point&0x3;//标定小数点

			  if(status_ScorBoard[7])	//在小数点位数前赋以零0.000
			  {
			  	 for(i=5-Cal_point;i<=5-lenx;i++)
				 {
				 	status_ScorBoard[i]=0;
				 }
			  }
			  for(i=1;i<8;i++)
				 {xor_buffer^=status_ScorBoard[i];}
			   status_ScorBoard[8]=xor_buffer;
			   status_ScorBoard[9]=0xbf;
			   return 	status_ScorBoard;	







		 

}









