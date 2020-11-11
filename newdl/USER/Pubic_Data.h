//char block_express[30][400]; 
//char *block_express[30];
#include "stdbool.h"
#include "stdlib.h"
#include "orginal_signal.h"
#include "CS5532_PRO.h"

#define FALSE 0
#define TRUE  1
bool X3;
/*���ʽ�ķ���*/
//char Output_express_group[100];
int Flash_Test_ADDR_Area[60]; 
int Flash_Test_Expression_Len[60];
char Flash_Test_Area[1024+512];
char Expression_Output[2048];//a over write occur in the Len_expression when 1024  (for contents exced 1024)
char Weight_parameter[1300];
char Step_parameter[1300];
char Flash_Test_Weight_para[1024*2];
char Flash_Test_Step_para[1024];

char tare_ing,untare;
char *px_expression;
int  Len_expression[60]; 
int  Addr_expression[60]; 
u32  Sum_Len_expression;
u8 typex,addr0,addr1,reference[10];

//struct output_of_block
//{
//    char type1[1];
//    char addr[2];
////    char parameter[5];
//    char reference[10];
////    char Output[350];//Output[5];
////    int count_of_output;
//};
//struct output_of_block  container_of_output_message;

u8 count_of_output;//[5];
u8 Flash_count_of_output;//;[5];
/*
#Send Express len  112!
#D00%{X��00}&({P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01}+{P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-02})?S=1!
#Send Express len  109!
#R01%{X��00}&({P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-02}+{P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01})?!
#Send Express len   55!
#D00%R��01&P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01?Tare!
*/
//struct exp_strcture
//{
//            int start[6][6];
//            int ending[6][6];//ÿ�������������
//            int body[6];//ÿ�������
////			char cal_buffer[6][6][5];//���� �����������ʽ
//
//};
//struct exp_strcture Level_message[6];//ÿ����Ϣ
//u8 group, LX0, LX1;
//u8 Level[6];//ÿ��Ĳ���


/*************************************
 analysis_logic_express �����

�� group ��level(group) �� body(group,level)
���ŵ���ʼ��ֹ���� start(group,level,body),end(group,level,body)
C,T �Ĳ�������ַ /C �ĵ�ַ D�ĵ�ַ
D��ִ�нű�
************************************/
//void  analysis_logic_express(char str[]) //block ��������
//{
//u8 j,seq;
//char *p,stk[10];
//int i,body;
//group=0;LX0=0;LX1=0;
//	for(i=0;i<6;i++)
//	    {Level[i]=0;}
//	for(i=0;i<6;i++)
//		{
//			for(j=0;j<6;j++)
//		    	{Level_message[i].body[j]=0;}
//		}
//
////		 strcat(str,")");
////		 memcpy(str,"(",1); 
//
//for(i=0;i<strlen(str);i++)  
// { 
// 			if(str[i]=='!')
//   			 return;//break;
// 
//   switch (str[i])
//   {
//			case '(': //Level[i]:0-- Level[i]  ��ʵ�ʲ���ΪLevel[i]+1  i:��������� ����i+1
//                 LX0 = LX1;
//                 if(LX0 > Level[group])//����������
//                        {Level[group]=LX0;} //�м���
//                 body=Level_message[group].body[LX0]; //ÿ���еģ�����
//                 Level_message[group].start[LX0][body]=i;
//                 LX1++;
//			break;
//		   	case ')':
//                LX1--; LX0=LX1;
//                body=Level_message[group].body[LX0];//ÿ���еģ�����
//                Level_message[group].ending[LX0][body]=i;
//                Level_message[group].body[LX0]++;
//                if(LX0==0)
//                       {
//					     group++;
//						 LX0=0;LX1=0; //����+1
//         			   }
//
//			break;		
//			case '%':
//		//#{X>00}&({P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01}+{P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-01&P-02})%D00?S=1!
////		typex=str[i+1];addr0=str[i+2];addr1=str[i+3];
// 		if(str[i+1]!='/')
//					   {
//							typex=str[i+1];addr0=str[i+2];addr1=str[i+3];
//							 p=	str+i+5; //  %C00?!
//							            //    12345
//							 if(*p=='!') {return;}
//				       }
//		else
//		               {
//					        typex=str[i+1];addr0=str[i+3];addr1=str[i+4];
//							 p=	str+i+6;//  %/C00?!
//							            //   123456
//							 if(*p=='!') {return;}
//					   }
////						   	 container_of_output_message.type1[0]=typex;
////							 container_of_output_message.addr[0]=addr0;
////							 container_of_output_message.addr[1]=addr1;
// 
//
//						 
//					 p=	str+i+5;
////					 if(*p=='!') {return;}
////					 //T_timer_value[Timer_num] C_counter_value[Counter_num];
////					 	return;				   		
//					 switch (str[i+1])
//						 {
//						 	case 'T':
//							        seq=(str[i+2]-0x30)*10+(str[i+3]-0x30);
//									memset(stk,0,10);
//									j=0;
//									while(*p!='!')
//									 {
//									   if(*p!='.') 
//									   stk[j++]=*(p++);
//									   else p++;
//									 }
//									T_timer_value[seq]=atol(stk);//��ʱֵ
//							return;						     
//							case 'C':
//							        seq=(str[i+2]-0x30)*10+(str[i+3]-0x30);
//									memset(stk,0,10);
//									j=0;
//									while(*p!='!')
//									 {
//									   if(*p!='.') 
//									   stk[j++]=*(p++);
//									   else p++;
//									 }
//								   C_counter_value[seq]=atol(stk);//����ֵ
//							return;						     
//							case 'D':
//									j=0;
//									while(*p!='!')
//									 {
//								   		reference[j++]=*(p++);//container_of_output_message.reference[j++]=*(p++);
//								 	 }
//								 		reference[j]='!';//container_of_output_message.reference[j]='!';//��Ϊ������
//							return;
//							default:
//								break;
//						}
//	 
////			break;
////			default:
////			break;
//	}
//
// }
//}
//bool true_false(bool X1,bool X2)
//{
//  	return X1&X2;
//
//}
///****************************************************************/
//bool Detect_edge(u8 Item_1,char Item_0,u8 k)
// {return 0;}
///****************************************************************/
/****************************************************************
 �ɼ�����״̬�����ݱ���״̬���С�&������
*************************************************************/
bool Basic_item_with_switch_state(char xk[])
{
bool X[100];
bool Y;
int Lenp;
u8 i,j,k;
char *p;
char xp[200],Item[6];
p=xk;
if(strstr(p,"P-02"))
  return FALSE;

p=xk;j=0;  //++++++++++++++++
Lenp=strlen(p);
for(i=0;i<Lenp;i+=4)
	{
			strncpy(Item,p,4);
			Item[4]=0;//�ָ�����
			p+=4;
			k=(u8)atol(Item+2);
			switch (Item[0])
					{
						case 'X':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=X_input_state[k];}
							else //�����غ��½��� or ������ or �½���
							    {X[j]=Detect_edge(Item[1],Item[0],k);}
							j++;
						break;
						case 'x':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=!X_input_state[k];}
							j++;
						break;
						case 'R':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=R_relay_state[k];}
							else //�����غ��½��� or ������ or �½���
							    {X[j]=Detect_edge(Item[1],Item[0],k);}
							j++;	break;
						case 'r':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=!R_relay_state[k];}
							j++;
						break;
						case 'T':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=T_timer_state[k];}
							else //�����غ��½��� or ������ or �½���
							    {X[j]=Detect_edge(Item[1],Item[0],k);}
							j++;	break;
						case 't':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=!T_timer_state[k];}
							j++;
						break;
						case 'C':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=C_counter_state[k];}
							else //�����غ��½��� or ������ or �½���
							    {X[j]=Detect_edge(Item[1],Item[0],k);}
							j++;	break;
						case 'c':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=!C_counter_state[k];}
							j++;
						break;
						case 'W':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=Weight_control_state[k];}
							else //�����غ��½��� or ������ or �½���
							    {X[j]=Detect_edge(Item[1],Item[0],k);}
							j++;
						break;
						case 'S':
							if(Item[1]=='-')	//'��ƽ
								{X[j]=Step_control_state[k];}
							j++;
						break;
						case 'P':
							if(k==1)X[j]=TRUE;//'true��--->P-01
							if(k==2)X[j]=FALSE;//'false��--->P-02
							j++;
						break;
						default:
							break;
					}
					if(j>0)
					{	//�����㣬������������=0
						if(!X[j-1])
						return FALSE;
					}
	}

//                    '�����߼�ʽ
                    Y=TRUE;
                    for(i=0;i<j;i++) 
                         Y=Y&&X[i];
                    return Y;
}

//bool And_Or_item_with_switch_state(char strk[],int len)
//{
// bool Y;
// char xk[200];
// char *p;
// unsigned char k;
// int i;
// int Lenx;
//
//
//Lenx=len;//strlen(strk);
//p=strk;
//	  	  if(*p=='(')
//		    { 	k=0;memset(xk,0,Lenx+1);
//				p++; 
//				for(i=1;i<Lenx;i++)
//					{	
//						if(*p!=')'&&*p!='+'&&*p!='%')
//							{
//							 if(*p!='{'&&*p!='}'&&*p!='&'&&*p!='*')
//							    xk[k++]=*p++;
//							  else
//							   	p++;							 
//							 }
//							else if(*p=='+')
//	                              { 
//								    if(Basic_item_with_switch_state(xk))
//										{memset(xk,0,k+1);Y=TRUE;goto over_op;}
//									memset(xk,0,k+1);k=0;
//									p++;
//								   }
//						    	else if(*p==')'||*p=='%')//*(p)=='%' last one
//									{
//									    if(Basic_item_with_switch_state(xk))
//											{memset(xk,0,i+1);Y=TRUE;goto over_op;}
//									    else
//									        {memset(xk,0,i+1);Y=FALSE;goto over_op;}									
//									}
//					}
//			}
//
//
//over_op:
//	  return Y;
//}

/*****************************************
 �������ŵ����꣬�� �� ����С��� �� ����
������������ַ��������ԡ�*��

************************************************/
//bool calculate_ecxpress(char str[],int len) 
//{
//int j,i,k;
//int Lenx,Lenp;
//u32 str_start,str_end;
//char strk[200],strp[200];
//char *p;
//const char star[]="******************************************************************************************************************************************************";
//const char true_x[]=("P-01");
//const char false_x[]=("P-02");
//
//bool result_cal;
//Lenp=len;
//	 strncpy(strp,str,Lenp);
//     for(i=group-1;i>=0;i--) //�����ţ�����group>0,  ������ ����group=0���˶β����� group=1,2..,level(group)=0,1,2..,body(group,level)=1,2,3...      
//           { 
//			   for(j=Level[i];j>=0;j--)
//	                {
//						for(k=Level_message[i].body[j]-1;k>=0;k--)
//		                   //��ȡ��ʼ��ֹ������ָ�� ��
//						 {
//		                   str_start=Level_message[i].start[j][k];
//		                   str_end=Level_message[i].ending[j][k];
//						   p=str_start+str;//memset(strk,0,30);
//						   Lenx=str_end-str_start+1;
//						   strncpy(strk,p,Lenx);//pָ������洢��ĳ�����ʽ��"("����ʼ���� ����=Lenx���ݴ���strk
//						   strncpy(p,star,Lenx);//pָ������洢��ĳ�����ʽ��"("����ʼ���� ����=Lenx���ݸ���"*"
////						   strncpy(p,true_x,4);
////						   strncpy(p,false_x,4);
//		                   //���������߼�ֵ
//		                    if(And_Or_item_with_switch_state(strk,Lenx))//���� ��  �������ʽ
//								{
//								   strncpy(p,true_x,4);//true��--->P-01
//								}
//		                    else
//							    {
//								   strncpy(p,false_x,4); //false--.P-02
//								}
//		                    //ȥ��������� ��  ���߼����[P-01 ��P-02)�͡���*������Բ��ı�ԭ�߼����ʽ���� �Ա���һ�� �������
//		                 }
//	              }
//         }
////		 strcat(str,")");
//		 memcpy(str,"(",1); 
//		 if(And_Or_item_with_switch_state(str,Lenp))
//		 	  {result_cal=TRUE;}
//		 else {result_cal=FALSE;}	 
//		 strncpy(str,strp,Lenp);
//		 return result_cal;
//}


//��ʼ��"#"
//'weight parameters  W_**&&&&&,%?  **:��� &&&&&:����ֵ %:����������(= < >)
//��� "W_Totall" + CStr(kx) + "@!"  kx:���ݸ���
//����"#"  W_**&&&&&,%? W_**&&&&&,%?.......W_**&&&&&,%?	 "W_Totall" + CStr(kx) + "@!"
/*�ֽ���������*/
void Desolve_weight_para(char str[])
{
char *p,stk[10],*kx,W_req;
u8 seq,j;
int value;
static u8 i;
//char *strstr(const char *string, const char *strSearch); 
		memset(stk,0,10);i=0; 
		p=strstr(str,"W_Totall");
		p+=strlen("W_Totall");
		while(*p!='@')
		{
			  stk[i]=*p;
			  i++;
			  p++;
		}
		W_Totall=(u8)atol(stk);
		
		kx=str;
	for(j=0;j<W_Totall;j++)	
	{
		    memset(stk,0,10); 
			p=kx;
			p=strstr(p,"W_");
			p+=2;
			stk[0]=*(p++);
			stk[1]=*(p++);
			seq=(u8)atol(stk);//���
			memset(stk,0,10);i=0; 
			while(*p!=',')
			{
				stk[i]=*(p++);
				i++;
			}
			value=atol(stk);//��������ֵ
			W_req=*(++p);
			Weight_control_value[seq]=value;
			Weight_control_requirement[seq]=W_req;
			kx=p;
	}
}

 //��ʼ��"#"
//'step parameters  W_**&&&&&,%?  **:��� &&&&&:����ֵ %:����������(= < >)
//��� "S_Totall" + CStr(kx) + "@!"  kx:���ݸ���
//����"#"  S_**&&&&&,%? S_**&&&&&,%?.......S_**&&&&&,%?	 "S_Totall" + CStr(kx) + "@!"
/*�ֽ���������*/
void Desolve_step_para(char str[])
{
char *p,stk[10],*kx,S_req;
u8 seq,j;
int value;
static u8 i;
//char *strstr(const char *string, const char *strSearch); 
		memset(stk,0,10);i=0; 
		p=strstr(str,"S_Totall");
		p+=strlen("S_Totall");
		while(*p!='@')
		{
			  stk[i]=*p;
			  i++;
			  p++;
		}
		S_Totall=(u8)atol(stk);
		
		kx=str;
	for(j=0;j<S_Totall;j++)	
	{
		    memset(stk,0,10); 
			p=kx;
			p=strstr(p,"S_");
			p+=2;
			stk[0]=*(p++);
			stk[1]=*(p++);
			seq=(u8)atol(stk);//���
			memset(stk,0,10);i=0; 
			while(*p!=',')
			{
				stk[i]=*(p++);
				i++;
			}
			value=atol(stk);//��������ֵ
			S_req=*(++p);
			Step_control_value[seq]=value;
			Step_control_requirement[seq]=S_req;
			kx=p;
	}
}


//void sample_express(char str[])
//{}
//void complicate_express(char str[])
//{}


bool Make_AND_OR(char str[])
{
 bool Y;
 char xk[200];
 char *p;
 unsigned char k;
 int i,j;
 char *addreess_start;
 char *addreess_end; 


							  addreess_start=str; j=0;
							  while(strchr(addreess_start,'+'))
							    {
							      addreess_end=strchr(addreess_start,'+');
								  k=0;p=addreess_start;
								  for(i=(int)addreess_start;i<(int)addreess_end;i++)
								  {	
							          if(*p!='{'&&*p!='}'&&*p!='&'&&*p!='*'&&*p!='(')
							 			 xk[k++]=*p++;
									  else
										 p++;									 
								  }	
								  xk[k++]=0;//��������
                                   //����AND����
								  Y= Basic_item_with_switch_state(xk);
								  if(Y)//������㣬����Ϊ1�����������=1
								  		goto over_op;
								  addreess_start=addreess_end;
								  addreess_start++;
								} 
							  //���һ������
							      addreess_end=strchr(addreess_start,')');
								  k=0;p=addreess_start;
								  for(i=(int)addreess_start;i<(int)addreess_end;i++)
								  {	
							          if(*p!='{'&&*p!='}'&&*p!='&'&&*p!='*'&&*p!='('&&*p!=')'&&*p!='#')
							 			 xk[k++]=*p++;
									  else
										 p++;									 
								  }	
								  xk[k++]=0;//��������
                                   //����AND����
								  Y=Basic_item_with_switch_state(xk);
// if(Basic_item_with_switch_state(xk))


over_op:
	  return Y;


}
void output_analysis(char str[])
{

u8 seq,*p,j;
char stk[11];
					 p=str;//ָ��'%'��ĵ�һ���ַ�	
					 switch (*(p+1))
						 {
						    case 'Y':
									typex=*(p+1);addr0=*(p+2);addr1=*(p+3);
							return;	
												     
						    case 'R':
									typex=*(p+1);addr0=*(p+2);addr1=*(p+3);
							return;	
												     
						    case '/':
									typex=*(p+1);addr0=*(p+3);addr1=*(p+4);
							return;						     

						 	case 'T':
									typex=*(p+1);addr0=*(p+2);addr1=*(p+3);
							        seq=(addr0-0x30)*10+(addr1-0x30);
									p=strchr(str,'?');
									stk[0]=*(p+1);
									stk[1]=*(p+3);//jump '.'
									stk[2]=0;//������
									T_timer_value[seq]=atol(stk);//��ʱֵ
							return;						     
							case 'C':
									typex=*(p+1);addr0=*(p+2);addr1=*(p+3);
							        seq=(addr0-0x30)*10+(addr1-0x30);
									p=strchr(str,'?');
									stk[0]=*(p+1);
									stk[1]=*(p+2);
									stk[2]=0;//������
								   C_counter_value[seq]=atol(stk);//����ֵ
							return;						     
							case 'D':
									typex=*(p+1);addr0=*(p+2);addr1=*(p+3);
							        seq=(addr0-0x30)*10+(addr1-0x30);
									p=strchr(str,'?')+1;
									j=0;
									while(*p!='!')
									 {
								   		reference[j++]=*(p++);//container_of_output_message.reference[j++]=*(p++);
								 	 }
								 		reference[j++]='!';//container_of_output_message.reference[j]='!';//��Ϊ������
										reference[j++]=0;//�ָ���
							return;

						    case 't': //ȥƤԪ��,%tare00 ������Է���
									typex=*(p+1);
							return;	

						    case 'x': //��ƤԪ��,%x_tare ������Է���
									typex=*(p+1);
							return;	

							default:
								break;
						}

}


void simulator_package(void)
{
bool state;
u8 i,reg_addr,pc,j;

char *px;
char *point_0;
char *addreess,*addreess_r;
char *Expression_Addr_strat[120];
char *Expression_Addr_end[120];
char *Expression_Addr_precent[120];

char  Expression_everyone[120];
char  Bracket_Expression[80];
int   Expression_lengthx[120],Len_R_L; 

char *W_start;//,*W_end;
char Wpx[5],*Wpx1;
u8 Wi; 
const char star[]="******************************************************************************************************************************************************";
const char true_x[]=("P-01");
const char false_x[]=("P-02");

  point_0=Expression_Output;//looking for '#'
    for(i=0;i<=count_of_output;i++)
	  {
	      addreess=strchr(point_0,'#');
		  point_0=addreess;
		  point_0++;
		  Expression_Addr_strat[i]=addreess;
	  }

  point_0=Expression_Output; //looking for '!'
    for(i=0;i<=count_of_output;i++)
	  {
	      addreess=strchr(point_0,'!');
		  point_0=addreess;
		  point_0++;
		  Expression_Addr_end[i]=addreess;
	  }

  point_0=Expression_Output;  //looking for '%'
    for(i=0;i<=count_of_output;i++)
	  {
	      addreess=strchr(point_0,'%');
		  point_0=addreess;
		  point_0++;
		  Expression_Addr_precent[i]=addreess;
	  }

    for(i=0;i<=count_of_output;i++)	//caculate every expression length
	  {
		  Expression_lengthx[i]=(int)(Expression_Addr_end[i]-Expression_Addr_strat[i]+1);
	  }

    for(i=0;i<=count_of_output;i++)	//fetch every expression
	  {
	       //���ܱ��ʽ��ȡ��һ�����ʽ
		  strncpy(Expression_everyone,Expression_Addr_strat[i],Expression_lengthx[i]);
		  *(Expression_everyone+Expression_lengthx[i])=0;	 //���һ���ַ�Ϊ��
		  //����ȡ��һ�����ʽ�������ʽ('%'����Ĳ���)
		  output_analysis(Expression_Addr_precent[i]);
			  if(strchr(Expression_everyone,'('))
			     {
					 while(strchr(Expression_everyone,'('))
					 {	  //���ʽ�������ţ��������Ų���
					      addreess=strrchr(Expression_everyone,'(');//ȡ����ǰ���ʽ�����ұߵ����ű��ʽ
						  addreess_r=strchr(addreess,')');
						  Len_R_L=addreess_r-addreess+1;
						  strncpy(Bracket_Expression,addreess,Len_R_L );
						  *(Bracket_Expression+Len_R_L)=0;//���һ���ַ�Ϊ��
						  strncpy(addreess,star,Len_R_L );//��Expression_everyone��ȡ�������Ų�����'*'����
						  //�������ڽ����������
							 	if(Make_AND_OR(Bracket_Expression))
								 {//����������true/false��ֵP-01 
								   memcpy(addreess,true_x,4); //���Ų��ֵ�������true
								 }
								else
								 {//����������true/false��ֵP-02
								   memcpy(addreess,false_x,4);//���Ų��ֵ�������false
								 }	
					 }
					 //���ʽ���������Ų���������ϣ���ϣ�˵������������
								 px=strrchr(Expression_everyone,'%');
								 *px=')';//'%'��')'�Ա����Make_AND_OR
								 //���������
							     state=Make_AND_OR(Expression_everyone);	
				  }
			  else	  	 	  
			  //Expression_everyone���ʽ�г�ʼ����������
			    {
								 px=strrchr(Expression_everyone,'%');
								 *px=')';//'%'��')'�Ա����Make_AND_OR
								 //���������
								state=Make_AND_OR(Expression_everyone);	
			   }
//������������ִ�����			   
						 reg_addr=(addr0-0x30)*10+(addr1-0x30);
						 if(state)
						 {
							  switch (typex)//
							  {
							  	case 'Y':
											Y_output_state[reg_addr]=True;
									break;
								case 'C':
											Counter_trigger(reg_addr); //'����������+1
									break;
							  	case '/':  //'/'='/C'
											Counter_reset(reg_addr); //'��λ������
									break;
								case 'R':
											R_relay_state[reg_addr]=True;
									break;
							  	case 'T':
											Timer_trigger(reg_addr); //'������ʱ��
									break;
							  	case 't': //ȥƤԪ����������Է���	%tare00
												tare_ing=1;  //ȥƤ	
												untare=0;
									break;
							  	case 'x': //��ƤԪ����������Է��� %x_tare
												tare_ing=0;  //��Ƥ	
												untare=1;
									break;

								case 'D':
									   switch (reference[0])
									   {
									   	case 'S'://�޸ĵ�ǰ����ֵ

												if(strstr(reference,"S=plus"))
													  {
													    if(Step_value<90)
													       Step_value++;
													  	break;
													  }
		 										if(strstr(reference,"S=dec"))
													  {
													    if(Step_value>0)
													       Step_value--;
													  	break;
													  }
												        Step_value=(reference[2]-0x30)*10+reference[3]-0x30;
					//							Step_control_requirement[reg_addr]=reference[1];
											break;
										case 'T'://ȥƤ
												tare_ing=1;  //ȥƤ	
												untare=0;
											break;
										case 'U'://��Ƥ
												tare_ing=0;  //��Ƥ	
												untare=1;
											break;
									   	case 'Z'://����
											break;
										case 'W'://�޸���������ֵ ,����ֵ��ʽ��W00=1000,W01=1234!
										    //*W_start,*W_end

												W_start=reference;
										   while(strchr(W_start,'W'))
										     {
											    W_start=strchr(W_start,'W');
												Wpx[0]=*(W_start+1);
												Wpx[1]=*(W_start+2);
												Wpx[2]=0;
												Wi=atol(Wpx);//ȡW**
												//ȡ����ֵW**=$$$$
												Wpx[0]=*(W_start+4);
												Wpx[1]=*(W_start+5);
												Wpx[2]=*(W_start+6);
												Wpx[3]=*(W_start+7);
												Wpx[4]=0;
											    Weight_control_value[Wi]=atol(Wpx);
												W_start++;
											 }
											break;
										default:
											break;
									}
									break;
								default:
									break;
						      }
						 }
					 else
					 {
						  switch (typex)
						  {
						  	case 'Y':
										Y_output_state[reg_addr]=False;
								break;
							case 'C':
										Counter_running_reset(reg_addr); //'��λ�������Ĵ�����־����һ���ټ�1
								break;
							case 'R':
										R_relay_state[reg_addr]=False;
								break;
						  	case 'T':
										Timer_reset(reg_addr);// '��λ��ʱ��
								break;
				
							default:
								break;
					    }	 
					 
					 
					 }
			   					 			 	   
		 }
	  /********************************/







  
//  px_expression=Expression_Output;
//  for(i=0;i<=count_of_output;i++)
//  {
//	 analysis_logic_express(px_expression+Addr_expression[i]);
//	 state=calculate_ecxpress(px_expression+Addr_expression[i],Len_expression[i]);
//	 reg_addr=(addr0-0x30)*10+(addr1-0x30);
//	 if(state)
////	 if(i)
//
//	 {
////					 container_of_output_message[block].type1[0]=str[i+1];
////					 container_of_output_message[block].addr[0]=str[i+2];
////					 container_of_output_message[block].addr[1]=str[i+3];
////		  reg_addr=(container_of_output_message.addr[0]-0x30)*10+(container_of_output_message.addr[1]-0x30);
//
////		  typex,addr0,addr1,
//		  switch (typex)//
//		  {
//		  	case 'Y':
//						Y_output_state[reg_addr]=True;
//				break;
//			case 'C':
//						Counter_trigger(reg_addr); //'����������+1
//				break;
//		  	case '/':  //'/'='/C'
//						Counter_reset(reg_addr); //'��λ������
//				break;
//			case 'R':
//						R_relay_state[reg_addr]=True;
//				break;
//		  	case 'T':
//						Timer_trigger(reg_addr); //'������ʱ��
//				break;
//			case 'D':
//				   switch (reference[0])
//				   {
//				   	case 'S'://�޸ĵ�ǰ����ֵ
//					        Step_value=(reference[2]-0x30)*10+reference[3]-0x30;
////							Step_control_requirement[reg_addr]=reference[1];
//						break;
//					case 'T'://ȥƤ
//							tare_ing=1;  //ȥƤ	
//							untare=0;
//						break;
//					case 'U'://��Ƥ
//							tare_ing=0;  //��Ƥ	
//							untare=1;
//						break;
//				   	case 'Z'://����
//						break;
//					case 'W'://�޸���������ֵ
//						break;
//
//					default:
//						break;
//				}
//
//
//			
//				break;
//
//
//			default:
//				break;
//	}
//	 
//	 }
//	 else
//	 {
//		  switch (typex)
//		  {
//		  	case 'Y':
////			            if(reg_addr==2)
////					{	pc=1;
////
////						if(C_counter_state[0])
////							{Y_output_high_07;}
////						else
////							{Y_output_lowx_07;}
////
////						Y_output_state[reg_addr]=False;	 }
////						else
//						Y_output_state[reg_addr]=False;
//				break;
//			case 'C':
//						Counter_running_reset(reg_addr); //'��λ�������Ĵ�����־����һ���ټ�1
//				break;
//			case 'R':
//						R_relay_state[reg_addr]=False;
//				break;
//		  	case 'T':
//						Timer_reset(reg_addr);// '��λ��ʱ��
//				break;
//
//			default:
//				break;
//	}	 
//	 
//	 
//	 }
//
//
//  }
}
 
/***********************************************************/
//Public Sub simulator_package()
//Dim i, j, k, seq, px As Integer
//Dim str As String
//Dim output_state As Boolean
//'j = 0 'tempolorily
// For i = 1 To Total_blocks
//  For j = 0 To container_of_output_message(i).count_of_output - 1 'һ�����ڶ�·���
//  
//        str = container_of_output_message(i).Output(j)
//        Call analysis_logic_express(str) '�������ʽ�Ľṹ
//        output_state = calculate_ecxpress(str) '����ģ�����뿪��״̬��ȷ��ÿ��������
//        '�������״̬������
//        seq = CInt(container_of_output_message(i).addr(j))
//        If output_state Then
//             Select Case container_of_output_message(i).type1(j)
//                Case Is = "Y"
//                        graphic_address = CurDir() + "\simulator_component\Y_on.bmp"
//                        Set �����.Y_output(seq).Picture = LoadPicture(graphic_address)
//                Case Is = "C"
//                        Call Counter_trigger(seq) '����������+1
//                        If C_counter_state(seq) Then
//                            graphic_address = CurDir() + "\simulator_component\Y_on.bmp"
//                            Set �����_����.C_output(seq).Picture = LoadPicture(graphic_address)
//                        End If
//                Case Is = "/C"
//                        Call Counter_reset(seq) '��λ������
//                        graphic_address = CurDir() + "\simulator_component\Y_off.bmp"
//                        Set �����_����.C_output(seq).Picture = LoadPicture(graphic_address)
//                        
//                Case Is = "R"
//                        R_relay_state(seq) = True '��̵�����λ
//                        graphic_address = CurDir() + "\simulator_component\Y_on.bmp"
//                        Set �����_��̵���.R_output(seq).Picture = LoadPicture(graphic_address)
//                Case Is = "T"
//                        Call Timer_trigger(seq) '������ʱ��
//                        If T_timer_state(seq) Then
//                            graphic_address = CurDir() + "\simulator_component\Y_on.bmp"
//                            Set �����_��ʱ.T_output(seq).Picture = LoadPicture(graphic_address)
//                        End If
//                        
//                Case Is = "D" '��ű�
//                       Call desolve_sript(container_of_output_message(i).reference(j))
//                       
//                Case Is = "tare"
//                            tare_value = Weight_value
//                Case Is = "x_tare"
//                            tare_value = 0
//                            
//                Case Is = "zero"
//                        If Weight_value > 100 Then
//                                MsgBox "�������㷶Χ", 1
//                        Else
//                               tare_value = Weight_value
//                
//                        End If
//                        
//                        
//            End Select
//        
//      Else
//             Select Case container_of_output_message(i).type1(j)
//                Case Is = "Y"
//                        graphic_address = CurDir() + "\simulator_component\Y_off.bmp"
//                        Set �����.Y_output(seq).Picture = LoadPicture(graphic_address)
//                Case Is = "C"
//                        Call Counter_running_reset(seq) '��λ�������Ĵ�����־����һ���ټ�1
//'                        graphic_address = CurDir() + "\simulator_component\Y_off.bmp"
//'                        Set �����_����.C_output(seq).Picture = LoadPicture(graphic_address)
//                Case Is = "R"
//                        R_relay_state(seq) = False '��̵�����λ
//                        graphic_address = CurDir() + "\simulator_component\Y_off.bmp"
//                        Set �����_��̵���.R_output(seq).Picture = LoadPicture(graphic_address)
//                Case Is = "T"
//                        Call Timer_reset(seq) '��λ��ʱ��
//                        graphic_address = CurDir() + "\simulator_component\Y_off.bmp"
//                        Set �����_��ʱ.T_output(seq).Picture = LoadPicture(graphic_address)
//            End Select
//        
//      End If
//   Next
// Next
//End Sub


//                         Call simulator_package '�����
//                        Call Store_XRCT_Status '����X R C T��״̬Ϊ�����������

