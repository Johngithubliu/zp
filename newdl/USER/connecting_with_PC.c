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
extern 	int Cal_point;//�궨С����
char NDS1;
unsigned char status[6];
unsigned char status_ScorBoard[10];
//unsigned char *status_and_datum(void);

//��������
//���ݵ�״̬����

 //   NDS1	 
unsigned char *status_and_datum(void)

{
unsigned char i,lenx;
char *p;
	status[0]=(char)Cal_point&0x3;//�궨С����
	if(FIN)
		status[0]|=0x08; //FIN=1 �Ƿ�5λ���ؽ��  FIN=0 �Ƿ�ʵʱ������ʾ����
	
	if(NDS1)   //�������趨,�궨����,��������"******"	no_scale
		{
			for(i=0;i<6;i++)
			{status[i]='*';}
		}
	else
		{
			if(FIN)	//FIN=1 �Ƿ�5λ���ؽ��
				{p=ADresult;
				 if(*p==0x16)status[0]|=0x04; //����λ
				 status[0]|=0x30; //��ASCII
				}
			else   //FIN=0 �Ƿ�ʵʱ������ʾ����
				{					
//					_ltoa(Dis_ad,Modbus_weight.weight_long);				
					p=Dis_ad;
					if(*p==0x16)status[0]|=0x04; //����λ
					status[0]|=0x30; //��ASCII
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
					{status[i]='X';}//<5,ͷ���֣�����X��	
			  	for(;i<6;i++)
					{status[i]=*p++;}
			  }
			FIN=0;
		}
  return 	status;	//returning strings  to be sent			  Dis_ad:"-" =0x16,����ascii�� ����00 ��β
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
//				{lenx=strlen(Dis_ad)-1;status_ScorBoard[6]=22;p++;}//���Ϊ�����������Ļ����ʾ��=22����"-")
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
//					{status_ScorBoard[i]=0x10;}//ͷ���֣���0x10����(����Ļ���յ���ʾ��)	
//			  	for(;i<6;i++)
//					{status_ScorBoard[i]=*p++;status_ScorBoard[i]&=0x0f;}
//			  }
//			  status_ScorBoard[7]=(char)Cal_point&0x3;//�궨С����
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
				{lenx=strlen(Dis_ad)-1;status_ScorBoard[6]=22;p++;}//���Ϊ�����������Ļ����ʾ��=22����"-")
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
					{status_ScorBoard[i]=0x10;}//ͷ���֣���0x10����(����Ļ���յ���ʾ��)	
			  	for(;i<6;i++)
					{status_ScorBoard[i]=*p++;status_ScorBoard[i]&=0x0f;}
			  }
			  status_ScorBoard[7]=(char)Cal_point&0x3;//�궨С����

			  if(status_ScorBoard[7])	//��С����λ��ǰ������0.000
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









