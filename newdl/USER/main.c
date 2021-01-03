
#include "stm32f10x.h"
#include "usart1.h"
#include "LED.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "Pubic_Data.h"
#include "Key.h" 
#include "stdlib.h"
#include "math.h"

#include "STM32FLASH_1.h"
#include "STM32FLASH_2.h"
#include "timer.h"
//#include "modbus_para.h"
#include "usart1.h"
#include "delay.h"

//#define debug_usart1
unsigned char AUTH=1;

void defult_parameters(void);
#define Input_setting_para     X_input_state[4]


/*��������**/
#define Command_01 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define Command_02 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define Command_03 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define Command_04 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
extern char err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;
//extern void simulator_package(char typex,char addr0,char addr1,char reference[]);
  
#define stab_led_on   LED_LIGHTENX[1]=0x01;
#define stab_led_off   LED_LIGHTENX[1]=0x02;
extern unsigned long AD_Result0;
 long AFTER_AD_Result0;
long buff_AD_Result0;
float buff_tare;
union Modbus_packed    
{    
float weight_long; 
unsigned char long_weight[4];   
}Modbus_weight;

extern int Cal_point;//�궨С����
int err_setting_Slow_feed_timer;

int temp__division;
extern int Initial_zero_range;//�����������㷶Χ
extern int Munal_zero_range;
extern int Cal_full_Mig;//�궨������ֵ
extern int Trace_zero_range;//��������ٷ�Χ
extern char Feed_signal_buffer[2];//�����źţ��� ȥƤ ����

extern char cal_finished;
extern float zero_zone;
extern char Cal_been_opened;
extern char Cal_zero_confirm_key_pressed,Cal_weight_confirm_key_pressed;
extern void KLTT_Cal_operation(void);
extern int select_division(void);	//weight_division
extern int Range_Filter_reg,Strong_Filter_reg;
extern int Strong_Filter0;//�����˲�ǿ��0
extern int Range_Filter0;//�����˲���Χ0
extern int state_operation,reservestate_operation;
extern char ADC_STAB;
extern unsigned long int Weighting_T,Weighting_package,Weighting_even;//����_��λ��������ƽ��ֵ
extern float Cal_user_weight;//�궨�û���������

extern unsigned char F302;
extern unsigned char F302_Fram[];
extern unsigned char mod_rec_buf[];

char Initial_zero_flag;
int Eliminate_zero_point,Eliminate_zero_delay;

void store_ton_package(void);


#define pi 3.141592653589
double real[100],image[100];
char Dis_ad[20]={1,2,3,4,5,6,7};
char Dis_ad1[20]={y,2,dim,n,5,6,7,8};

extern unsigned char status_ScorBoard[10];

extern void store_test_result_to_stack(void);
extern char  *get_test_result_form_stack(void);
extern unsigned char *status_and_datum(void);
extern unsigned char *conneting_ScorBoard(void);
extern void verify_filter_ADCODE(void);

extern void Recover_para(void);
extern void Reply_pc_inquier(void);
extern char executive_contorl;
extern void process_feed_material(void);

extern  void AD5422Init (void);
extern  void WriteControlRegisterToAD5422 (int uiControlData);
extern void WriteDataRegisterToAD5422 (int uiAnalogData);
extern void test_4_20ma_port(void);

extern int Analog_mode_SEL;
void   ANALOG_4_20mA_00(void);
void   ANALOG_0_5V_00(void);

#define shift_key  0x00
#define data_incr  0x01
#define data_confir 0x02
#define zero_ing 0x03
#define tare_ing 0x04
#define setting 0x05
#define calib   0x06  
#define deletex   0x07  

char shift_key_Status;
char data_incr_Status;
char data_confir_Status;
char zero_ing_Status;
char tare_ing_Status;
char setting_Status;
char calib_Status;
char deletex_Status;

char borrow_write_para01_to_get_coeff;
char programm_setting,calibing,clearing,entering,operated;
//  const unsigned char P01[]={P,0x30,'1',dim,dim,dim,dim,dim,0};
//  const unsigned char P02[]={P,0x30,'2',dim,dim,dim,dim,dim,0};
// const unsigned char P03[]={P,'0','3',dim,dim,dim,dim,dim,0};
// unsigned char PXX[9];
char sel_AD_code,flag_borrow_interface_for_setting;
unsigned char Cal[]={0x0c,0x0a,L,dim,dim,dim,dim,dim,0};
unsigned char CLE[]={0x0c,L,0x0E,dim,dim,dim,dim,dim,0};

int Cal_Setting_step;
void interface_for_setting(void);
void  interface_for_calbing(void);
extern void wang_USART1_Process(void);
void interface_for_clearing(void);

extern int  Modbus_Address,Modbus_value;//,Modbus_CRC_CODE;
extern int Modbus_Counter;
int seq_num;//С��Ļ�趨/�궨�Ĳ������
void dis_para00(int seq_num,int para,char _image_ponter);
void dis_para02(int seq_num,int para,char _image_ponter);
void write_para01(int para,char _image_ponter);
void dis_para01(unsigned char seq_num,int para,char _image_ponter);

extern char Modbus_0x10_Value[30];
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */

//1.ͨѶ������ ������̩ͨ�е�ַ(һ����ַ16λ)
#define comm_bud_rate 1508-1// 1008-1  //���������ʹ�����
#define comm_address comm_bud_rate+1 //����ͨѶ��ַ������
//2.����������
#define Initial_zero_range_address comm_address+1  //�����������㷶Χ
#define Munal_zero_range_address Initial_zero_range_address+1  //�����ֶ����㷶Χ
#define Trace_zero_range_address Munal_zero_range_address+1  //��������ٷ�Χ
//3.��ʱ������
#define  Delay_tare_address  Trace_zero_range_address+1//����ȥƤ��ʱ���
#define  Delay_Mid_feed_address  Delay_tare_address+1//�����м���ʱ
#define  Delay_Pre_process_address  Delay_Mid_feed_address+1//������ǰ��ʱ
#define  Delay_Next_loop_address  Delay_Pre_process_address+1//������һѭ����ʱ
#define  Time_supvisor_Slow_feed_up_limt_address  Delay_Next_loop_address+1//�������Ӽ�������ʱ��
#define  Time_supvisor_Slow_feed_low_limt_address  Time_supvisor_Slow_feed_up_limt_address+1//�������Ӽ�������ʱ��
#define  Pulse_howmany_package_address  Time_supvisor_Slow_feed_low_limt_address+1//�������ٴ��������
#define  Tare_howmany_package_address  Pulse_howmany_package_address+1//�������ٴ�ȥƤһ��
#define  Recive_x_signals_address  Tare_howmany_package_address+1//�����յ�x���ź��趨
#define  no_recive_x_signals_address  Recive_x_signals_address+1//�����ղ���x���ź��趨��
#define  Even_howmany_package_Pre_process_address  no_recive_x_signals_address+1//�������ٴ�ƽ������һ����ǰ��
#define  Delay_slow_feed_address  Even_howmany_package_Pre_process_address+1//����������ʱ
#define  Delay_initial_feed_address  Delay_slow_feed_address+1//����������ʱ

//4.����������   
#define   Slow_feed_modify_uper_limt_address  Delay_initial_feed_address +1//  ����������������
#define   Slow_feed_modify_down_limt_address   Slow_feed_modify_uper_limt_address +1//  ����������������
#define   Pre_feed_modify_uper_limt_address   Slow_feed_modify_down_limt_address +1//  ������ǰ����������
#define   Pre_feed_modify_down_limt_address   Pre_feed_modify_uper_limt_address +1//  ������ǰ����������
#define   Slow_feed_address   Pre_feed_modify_down_limt_address +1//  ����������
#define   Pre_feed_address   Slow_feed_address +1//  ������ǰ��
#define   Mid_feed_address   Pre_feed_address +1//  �����м�ֵ
#define   Dlet_error_address   Mid_feed_address +1//  �����ʲ�
 //*2017-10-28am*
#define   Target_uper_limt_address   Dlet_error_address +1  //  ������������
#define   Target_down_limt_address   Target_uper_limt_address +1//  ������������
#define   Target_address             Target_down_limt_address+1//  ������ֵ
#define   zero_range_address         Target_address +1//  ��������
 //*2017-10-28am*
#define   Slow_feed_modify_address   zero_range_address +1//  ��������������

#define   Initial_power_value_address   Slow_feed_modify_address +1//  �������ϵ�ֵ
#define   Initial_feed_value_address   Initial_power_value_address +1//  ����������
#define   Random_setting_value_address   Initial_feed_value_address +1//  ��������趨ֵ



//5.����������
#define   ID_user_cal_address   Random_setting_value_address +1//  �����û��궨������
#define   ID_slow_feed_modify_address   ID_user_cal_address +1//  ��������������ǹ�����
#define   ID_pre_feed_modify_address   ID_slow_feed_modify_address +1//  ������ǰ������ǹ�����
#define   ID_run_mode_address   ID_pre_feed_modify_address +1//  ��������ģʽ������

//7.�궨������
#define Cal_ID_division_address ID_run_mode_address+1//�궨�ֶ���������
#define Cal_user_weight_address Cal_ID_division_address+1//�궨�û���������
#define Cal_weight_address Cal_user_weight_address+2//�궨��������

//8.�˲������飺
#define Range_Filter0_address Cal_weight_address+2    //�����˲���Χ0
#define Range_Filter1_address Range_Filter0_address+1//�����˲���Χ1
#define Range_Filter2_address Range_Filter1_address+1//�����˲���Χ2
#define Range_Filter3_address Range_Filter2_address+1//�����˲���Χ3
#define Range_Filter4_address Range_Filter3_address+1//�����˲���Χ4

#define Strong_Filter0_address Range_Filter4_address+1//�����˲�ǿ��0
#define Strong_Filter1_address Strong_Filter0_address+1//�����˲�ǿ��1
#define Strong_Filter2_address Strong_Filter1_address+1//�����˲�ǿ��2
#define Strong_Filter3_address Strong_Filter2_address+1//�����˲�ǿ��3
#define Strong_Filter4_address Strong_Filter3_address+1//�����˲�ǿ��4

#define Cal_full_Mig_address  Strong_Filter4_address+1// 1561-1//������ֵ
#define Cal_pint_address  Cal_full_Mig_address+1// 1562//�궨С����
#define Analog_mode_SEL_address    Cal_pint_address+1//ģ�������ѡ��



/*************************************************/
//1.ͨѶ������
extern int ID_budrate;//���������ʹ�����
extern int Comm_Addr;//����ͨѶ��ַ������
//2.����������
extern int Initial_zero_range;//�����������㷶Χ
extern int Munal_zero_range;//�����ֶ����㷶Χ
extern int Trace_zero_range;//��������ٷ�Χ
//3.��ʱ������
extern int Delay_tare;//����ȥƤ��ʱ���
extern int Delay_Mid_feed;//�����м���ʱ
extern int Delay_Pre_process;//������ǰ��ʱ
extern int Delay_Next_loop;//������һѭ����ʱ
extern int Time_supvisor_Slow_feed_up_limt;//�������Ӽ�������ʱ��
extern int Time_supvisor_Slow_feed_low_limt;//�������Ӽ�������ʱ��
extern int Pulse_howmany_package;//�������ٴ��������
extern int Tare_howmany_package;//�������ٴ�ȥƤһ��
extern int Recive_x_signals;//�����յ�x���ź��趨
extern int no_recive_x_signals;//�����ղ���x���ź��趨��
extern int Even_howmany_package_Pre_process;//�������ٴ�ƽ������һ����ǰ��
extern int Delay_slow_feed;//����������ʱ
extern int Delay_initial_feed;//����������ʱ

//4.����������   
extern int Slow_feed_modify_uper_limt;//����������������
extern int Slow_feed_modify_down_limt;//����������������
extern int Pre_feed_modify_uper_limt;//������ǰ����������
extern int Pre_feed_modify_down_limt;//������ǰ����������
extern int Slow_feed;//����������
extern int Pre_feed;//������ǰ��
extern int Mid_feed;//�����м�ֵ
extern int Dlet_error;//�����ʲ�
extern int Target;//������ֵ
extern int Target_uper_limt;//������������
extern int Target_down_limt;//������������
extern int zero_range;//��������
extern int Slow_feed_modify;//��������������
extern int Initial_power_value;//�������ϵ�ֵ
extern int Initial_feed_value;//����������
extern int Random_setting_value;//��������趨ֵ

//5.����������
extern int ID_user_cal ;//�����û��궨������
extern int ID_slow_feed_modify ;//��������������ǹ�����
extern int ID_pre_feed_modify;//������ǰ������ǹ�����
extern int ID_run_mode;//��������ģʽ������



//6.����������
extern int Clear_merory_code;//�ڴ���������
extern int Target_pulse;//������������������
extern int ID_weight_unit;//����������λ�趨������
extern int Num_shift;//�������
//7.extern int ;//�궨������
extern int Cal_code;//�궨����
extern int Cal_point ;//�궨С����
extern int Cal_ID_division;//�궨�ֶ���������
extern int Cal_full_Mig;//�궨������ֵ
extern float Cal_user_weight;//�궨�û���������
extern int Cal_zero;//�궨��λ
extern int Cal_ratio;//�궨����
extern float Cal_weight;	//�궨��������

//8.�˲������飺
extern int Range_Filter0;//�����˲���Χ0
extern int Range_Filter1;//�����˲���Χ1
extern int Range_Filter2;//�����˲���Χ2
extern int Range_Filter3;//�����˲���Χ3
extern int Range_Filter4;//�����˲���Χ4
extern int Strong_Filter0;//�����˲�ǿ��0
extern int Strong_Filter1;//�����˲�ǿ��1
extern int Strong_Filter2;//�����˲�ǿ��2
extern int Strong_Filter3;//�����˲�ǿ��3
extern int Strong_Filter4;//�����˲�ǿ��4

int KLTT_AADR;
extern union Modbus_packed_1    
{    
long target_totall_pulse; 
unsigned char pulse[4];   
}Modbus_pulse;

extern void Zero_operation(void);
extern void Tare_operation(void);

 extern int system_indicator;
 void signal_convert_to_LED (void);
u8 Signal_lamp[9];
u8 scale[9]={0x35,0x0c,0x0a,L,0x0e,dim,dim,dim,0x0};
u8 scalb[9]={0x35,0x0c,0x0a,L,0x0b,dim,dim,dim,0x0};
u8 dim_x[9]={dim,dim,dim,dim,dim,dim,dim,dim,0x0};
 void Setting_configur(void);
 void display_main_weight(void);
extern void Key_input_edge(void);
void Modify_Dis_ad(void);
char revers;//ǰ��д������Ч
void cal_display_for_LED(void);

extern int shut_initial_mid_slow_prefeed_timer[4],set_shut_initial_mid_slow_prefeed_timer;  //*2017-10-28am*
#define Cal_output           Y_output_state[12]=1
#define Un_Cal_output           Y_output_state[12]=0
extern char FIN;
extern unsigned char 	delt_pulse_string_modbus[15];//={0x0b,0x10,0x00,0x02,0x00,0x01,0x02};
extern unsigned char sig_pulse_string_modbus[15];//={0x0b,0x10,0x00,0x0f,0x00,0x01,0x02,0x00,0x00};

int stop_dis_AD_timer;
char AD_code_display;

extern unsigned char u3_send_count;
extern unsigned char modbus_Fram[32];
extern void clear_arr();
extern unsigned char arr_buf[];
int main(void)
{
	
//  char str0[10];
//  int i;
 	char SendBuff[25];
	char SendBuff_len[5];  //*p,
//	u8  overx[]={"\r\n "};
	long char_size;
	int px;	//char_location[5],
	u8 seq,i;//,j;
	char comm_over,comm_calibration;

	u8 data_strx3[]={'e','f','f','a','b','c','d'};
	u8 LED_LIGHTENX[]={0x01,0x00,0x00,0x00,0x0,0x0,0x0,0x0,0x0};
	
//	AD_Result0
//
//New_port: 
 Key_GPIO_Config();	  // Y_output_high_00;
 Out_Input_GPIO_Config();

//    state_operation=-1;
    state_operation=1;

 	RCC_Configuration();
#ifdef debug_usart1
	USART1_Config();
#else
	USART1_REMAP_Config();
#endif
	USART2_Config();
	USART3_Config();
	
	UART4_Config();
	UART5_Config();
	TIME_NVIC_Configuration();
	TIME_Configuration();
	NVIC_Configuration();
	LED_GPIO_Config();	
	seq=0;comm_over=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_6);
			CS5532_GPIO_Config();
			EXTI_NVIC_Configuration(); //����EXTI ��AD
//			EXTI_Configuration();
	initi_usart2_control_pin();




			//��AD ����
			STMFLASH_Read (AD_ZEROX,(u16*)AD_ZERO_STORE.zero_pointx,4);
			zero_point=AD_ZERO_STORE.zero_point;	
			STMFLASH_Read (AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);
			ad_coefficent=AD_COFF_STORE.ad_coefficent;
			//����λ������ 
			STMFLASH_Read (FL_Weighting_T,(u16*)&Weighting_T,4);
			//printf("r1=%ld\n",Weighting_T);
			STMFLASH_Read (FL_Weighting_package,(u16*)&Weighting_package,4);
			//printf("r2=%ld\n",Weighting_T);
			arr_buf[0x12]=(Weighting_T>>8)&0x0ff;
			arr_buf[0x13]=Weighting_T&0x0ff;
			arr_buf[0x14]=(Weighting_package>>8)&0x0ff;
			arr_buf[0x15]=Weighting_package&0x0ff;

			Recover_para();
			//printf("r3=%ld\n",Weighting_T);
			//printf("FF_Target_pulse =%ld\r\n",Modbus_pulse.target_totall_pulse);
			
	//		printf("Weighting_package =%ld\r\n",Weighting_package);

			init_cs5532();
 
 if( !RCC_GetFlagStatus(RCC_FLAG_SFTRST))     //�ϵ縴λflag=0, �����λflag=1
{

	
	real[0]=cos(pi/3);
	image[0]=sin(pi/2);
	real[2]=tan(pi/6);
	real[3]=tan(pi/4);
	
	real[1]=cos(pi/4);
	image[1]=sin(pi/6);
}
USART2_RE_lowx;
USART2_DE_lowx;

/**************���ƴ���4-20ma��ʼ��************************************/
test_4_20ma_port();
#define ANALOG_4_20mA               ( 4020 )
#define ANALOG_0_5V 5
AD5422Init ();		  
//init_TM1638(); //��ʼ��TM1638
flash_ponter=flash_ponter_image=3;//��ʼ��˸λ�ú���Ҫ
//combinate_letter_digtal(OUQ,digtal_x);
flash_ponter=3;flashing=1;	 original=LED_display_buffer;
/*************************************************************************/

   flashing=0; 

set_shut_initial_mid_slow_prefeed_timer=ID_user_cal;  //*2017-10-28am*	set_shut_initial_mid_slow_prefeed_timer  ����	   ID_user_cal
Delay(5000000);
Delay(2000000);


while(1)
{

USART3_RE_lowx;
USART3_DE_lowx;

	
 if (FIN)
 	{		
		USART2_DE_high;
		for(i=0;i<8;i++)
		{
			USART_SendData(USART2,delt_pulse_string_modbus[i]);
			
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		}
		
		Delay(4000);
		
		USART2_DE_lowx;
	   FIN=0;
	}

else
   {
	 for(i=0;i<10;i++)
		{	USART_SendData(UART4,status_ScorBoard[i]);
			while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
		}
	}
	 
	if (F302)
 	{		
	
		for(i=0;i<F302;i++)
		{	
			USART_SendData(USART1,F302_Fram[i]);
				
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			
		}
		Delay(4000);
		if(AUTH)
		{
			if((F302==6)&&(F302_Fram[0]=='*')&&(F302_Fram[1]=='*'))			while(1);//Lock
		}
		F302=0;
	
	}
	
	if(u3_send_count)
	{
		USART3_RE_high;
		USART3_DE_high;
		for(i=0;i<u3_send_count;i++)
		{			
			USART_SendData(USART3,modbus_Fram[i]);
			
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
			
		}
		clear_arr();
		Delay(6000);
		u3_send_count=0;
		USART3_RE_lowx;
		USART3_DE_lowx;
		
	}
	

/*******AD filter intinaled***************************/
if(state_operation<3)//���� �˲�ǿ�� �˲���Χ
	   {
				 Strong_Filter_reg=Strong_Filter0; //�����˲�ǿ��0
				 Range_Filter_reg=Range_Filter0; //�����˲���Χ0
       }

	 if(!Initial_zero_flag&&ADC_STAB)//Initial_zero_range;//�����������㷶Χ
	   {
		   if(((float)AFTER_AD_Result0/pow(10,Cal_point))<(((float)Initial_zero_range/100)*(float)Cal_full_Mig))
		   		{
					  zero_point=AD_Result0; 
					  Feed_signal_buffer[1]|=0x40;	//�����źţ��� �����
					  Initial_zero_flag=1;
					//	printf("z_p=%ld,Cal_full_Mig=%f\r\n",zero_point,Cal_full_Mig);
	   			}
				//	else printf("no zero\r\n");
					
	   }

if(!clearing&&!calibing &&!programm_setting)	   //�趨���궨����� ������
   {
	 if(Trace_zero_range&&AFTER_AD_Result0&&!buff_tare&&ADC_STAB)//��������ٷ�ΧTrace_zero_range;	��Ƥ���ȶ�
	   {
//		   if((float)abs(AFTER_AD_Result0)/pow(10,Cal_point)<((float)Trace_zero_range/100)*(float)Cal_full_Mig)
		   if((float)abs(AFTER_AD_Result0)<(float)Trace_zero_range)
		   		{
					switch (Eliminate_zero_point)  
					{
						case 0:
							  Eliminate_zero_delay=100; Eliminate_zero_point=1;
						break;
						case 1:
						      if(!Eliminate_zero_delay)
							  {zero_point=AD_Result0; 
							  	Feed_signal_buffer[1]|=0x40;	//�����źţ��� �����
							   Eliminate_zero_point=0;}
						break;
						default:
						break;
					}

	   			}
		   else
		   		{
					Eliminate_zero_point=0;	Eliminate_zero_delay=200;
				}
	   }
	else
		{
			Eliminate_zero_point=0;	Eliminate_zero_delay=200;
		}

  }

  if(!AD_code_display)
  	{
			 buff_AD_Result0 =(long)AD_Result0;
			 AFTER_AD_Result0=(((float)((float)AD_Result0-(float)zero_point))/ad_coefficent+0.5);
			
		//Ϊmodbus������ֵ(LONG) ����modbusͨѶ
			Modbus_weight.weight_long=(float)AFTER_AD_Result0-buff_tare;//����,ȥƤ
			//�ֶ���
			temp__division=select_division();
			Modbus_weight.weight_long/=temp__division;
			Modbus_weight.weight_long=(int)Modbus_weight.weight_long*temp__division;
		
			_ltoa(Dis_ad,Modbus_weight.weight_long);	 //������ʾ���Ӹ��
	}
  else
  	{
			_ltoa(Dis_ad,(float)AD_Result0);	 //��ʾAD_CODE
	
	}


	  
	if (AFTER_AD_Result0 < 0)
		AFTER_AD_Result0 = -AFTER_AD_Result0; //<0,ȡ����ֵ
	Weight_value=(int)AFTER_AD_Result0; //����ֵ

if(Analog_mode_SEL)
	{ANALOG_0_5V_00();}
else
	{ANALOG_4_20mA_00();}

	conneting_ScorBoard();

if(!err_setting_Slow_feed_timer)
	{do_flashing(flash_ponter_image);
	 err_setting_Slow_feed=0;
	 err_setting_Pre_feed=0; err_setting_Target=0;
//err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;	
	}


	Key_input_edge();
	Setting_configur();
	signal_convert_to_LED ();

		 if(!stop_dis_AD_timer)
			{display_main_weight();}

	cal_display_for_LED();
//defult parameters
	defult_parameters();


}    


}

void store_ton_package(void)
{
				STMFLASH_Write(FL_Weighting_T,(u16*)&Weighting_T,4);//�����λ��
				STMFLASH_Read (FL_Weighting_T,(u16*)&Weighting_T,4); //
				
				//printf("save Weighting_T=%ld\r\n",Weighting_T);
	
				STMFLASH_Write(FL_Weighting_package,(u16*)&Weighting_package,4);//���������
				STMFLASH_Read (FL_Weighting_package,(u16*)&Weighting_package,4);
}
void   ANALOG_0_5V_00(void)
{
//ANALOG_0_5V
	if(Modbus_weight.weight_long/pow(10,Cal_point)<=Cal_full_Mig&&Modbus_weight.weight_long>=0)
	{
		WriteControlRegisterToAD5422 (ANALOG_0_5V);
		WriteDataRegisterToAD5422 ((Modbus_weight.weight_long/pow(10,Cal_point)/Cal_full_Mig)*65534);	 //������ֵ  Cal_full_Mig
	}
	else if(Modbus_weight.weight_long<0)
	{
		WriteControlRegisterToAD5422 (ANALOG_0_5V);
		WriteDataRegisterToAD5422 (0x0001);	 //������ֵ  Cal_full_Mig
	}
	else if(Modbus_weight.weight_long/pow(10,Cal_point)>Cal_full_Mig)
	{
		WriteControlRegisterToAD5422 (ANALOG_0_5V);
		WriteDataRegisterToAD5422 (65534);	 //������ֵ  Cal_full_Mig
	}
}
void   ANALOG_4_20mA_00(void)
{
float real_weight;
real_weight=Modbus_weight.weight_long/pow(10,Cal_point)/Cal_full_Mig;
//ANALOG_4_20mA
	if(Modbus_weight.weight_long/pow(10,Cal_point)<=Cal_full_Mig&&Modbus_weight.weight_long>=0)
	{
		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
		WriteDataRegisterToAD5422 (real_weight*65534);	 //������ֵ  Cal_full_Mig
	}
	else if(Modbus_weight.weight_long<0)
	{
		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
		WriteDataRegisterToAD5422 (0x0001);	 //������ֵ  Cal_full_Mig
	}
	else if(Modbus_weight.weight_long/pow(10,Cal_point)>Cal_full_Mig)
	{
		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
		WriteDataRegisterToAD5422 (65534);	 //������ֵ  Cal_full_Mig
	}
}

//void   ANALOG_0_5V_00(void)
//{
////ANALOG_0_5V
////	if(Modbus_weight.weight_long/pow(10,Cal_point)<=Cal_full_Mig&&Modbus_weight.weight_long>=0)
////	{
//	if(Modbus_weight.weight_long<=Cal_full_Mig&&Modbus_weight.weight_long>=0)
//	{
//
//		WriteControlRegisterToAD5422 (ANALOG_0_5V);
//		WriteDataRegisterToAD5422 ((Modbus_weight.weight_long/Cal_full_Mig)*65534);	 //������ֵ  Cal_full_Mig
//	}
//	else if(Modbus_weight.weight_long<0)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_0_5V);
//		WriteDataRegisterToAD5422 (0x0001);	 //������ֵ  Cal_full_Mig
//	}
//	else if(Modbus_weight.weight_long>Cal_full_Mig)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_0_5V);
//		WriteDataRegisterToAD5422 (65534);	 //������ֵ  Cal_full_Mig
//	}
//}
//void   ANALOG_4_20mA_00(void)
//{
//float real_weight;
//real_weight=Modbus_weight.weight_long/Cal_full_Mig;
////ANALOG_4_20mA
//	if(Modbus_weight.weight_long<=Cal_full_Mig&&Modbus_weight.weight_long>=0)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
//		WriteDataRegisterToAD5422 (real_weight*65534);	 //������ֵ  Cal_full_Mig
//	}
//	else if(Modbus_weight.weight_long<0)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
//		WriteDataRegisterToAD5422 (0x0001);	 //������ֵ  Cal_full_Mig
//	}
//	else if(Modbus_weight.weight_long>Cal_full_Mig)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
//		WriteDataRegisterToAD5422 (65534);	 //������ֵ  Cal_full_Mig
//	}
//}


void Setting_configur(void)
{
//#define shift_key  0x00
//#define data_incr  0x01
//#define data_confir 0x02
//#define zero_ing 0x03
//#define tare_ing 0x04
//#define setting 0x05
//#define calib   0x06  
const unsigned char Sett[]={'5',0x0e,t,t,dim,dim,dim,dim,0};
unsigned char PXX[9];
unsigned char temp;
	if((Read_key()==shift_key)&&(programm_setting||calibing))
		{
 			if(!shift_key_Status) 
			{ 
				 flash_ponter_image++;
				 if(flash_ponter_image>flash_ponter)	   //�ƶ�
				 flash_ponter_image=1; shift_key_Status=1;
			}

		}
	if((Read_key()==data_incr)&&(programm_setting||calibing))
			{
	 			if(!data_incr_Status) 
				{ 
					temp=++LED_display_buffer[8-flash_ponter_image];  // ��������
					 if(temp>'9')//���ֱ���ΪASCII,����0Ҳ����ʾ��������strlen�������
					 LED_display_buffer[8-flash_ponter_image]='0';
					 if(temp>'9'||temp<'0')//���ֱ���ΪASCII,����0Ҳ����ʾ��������strlen�������
					 LED_display_buffer[8-flash_ponter_image]='0';
					 data_incr_Status=1;
			    }
			
			}
/*************************************************************************/
	if((Read_key()==data_confir)&&programm_setting)
		{
	 			if(!data_confir_Status&&!entering) 
				{
//					if(flag_borrow_interface_for_setting&&Cal_Setting_step==30)
//					{flag_borrow_interface_for_setting=0;interface_for_setting();
//					goto next_steeep0;}


				   if(flag_borrow_interface_for_setting)
					 {
					 	if(Cal_Setting_step<31)
					 	   Cal_Setting_step++;
						   goto next_steeep;
					 }
					Cal_Setting_step++;				 //stepping	  �趨ȷ��
 next_steeep:
					interface_for_setting();
					entering=1;	data_confir_Status=1; return;
				}
		
		
		}


 	if((Read_key()==data_confir)&&calibing)
		{
	 			if(!data_confir_Status&&!entering) 
				{	
					Cal_Setting_step++;				 //stepping	�궨 ȷ��
					interface_for_calbing();
					entering=1;	 data_confir_Status=1; return;
				}
		}

 	if((Read_key()==data_confir)&&clearing)
		{
	 			if(!data_confir_Status&&!entering) 
				{
					Cal_Setting_step++;				 //stepping	��� ȷ��
					interface_for_clearing();
					entering=1;	data_confir_Status=1;  return;
				}
		}

/*************************************************************************/

	if((Read_key()==zero_ing))	{ if(flag_borrow_interface_for_setting)return;
	 if(!zero_ing_Status)

									  if(!clearing&&!calibing &&!programm_setting)	   //�趨 �궨 ��� ʱ������Ч
										   {Zero_operation();zero_ing_Status=1;}//��������
									  else
										{ 
										 if(clearing||calibing||programm_setting)
										 {	  
											  if(Cal_Setting_step>1)
											    {  revers=1;//����д������Ч
												   Cal_Setting_step--;
												   KLTT_AADR-=2;
												   if(programm_setting)
													   interface_for_setting();
												   if(calibing)
													   interface_for_calbing();
												   if(clearing)
													   interface_for_clearing();
												}
										  }
										  zero_ing_Status=1;
										  return;
									    } 	
	                            }
	if((Read_key()==tare_ing))	{  if(!tare_ing_Status)
								   Tare_operation();tare_ing_Status=1;//����ȥƤ
								}

	if((Read_key()==setting)&&!calibing&&!clearing)
			{	
//			Cal_Setting_step=0;		 //�趨����
	 			if(!setting_Status&&!programm_setting) 
				{ 
					flashing=0;Cal_Setting_step=0;
					strncpy(PXX,Sett,9 );Delay(1000);
					write_LED_Block(PXX,0,LED_LIGHTENX);  //title
					programm_setting=1;	setting_Status=1; return;
				}

	 			if(!setting_Status&&programm_setting) 
				{ 
next_steeep0:       flash_ponter=0;
					flash_ponter_image=0;//��ʼ��˸λ�ú���Ҫ	//�趨�˳�
//					combinate_letter_digtal(OUQ,digtal_x);
					flash_ponter=0;flashing=0;	
					 original=LED_display_buffer;
				   write_LED_Block(dim_x,0,Signal_lamp);
					Cal_Setting_step=0;
				   programm_setting=0;
//				   STB_high0;//STB=1
				   Delay(100000); setting_Status=1;
				   flag_borrow_interface_for_setting=0;
				   	return;
				}
		    }

	if((Read_key()==calib)&&!programm_setting&&!clearing)	
			{	Cal_Setting_step=0;
	 			if(!calib_Status&&!calibing) 
				{ 
// interface_for_calbing();
						flashing=0;	 
						write_LED_Block(Cal,0,LED_LIGHTENX);  //�궨����
						Cal_Setting_step=0;
						calibing=1;	calib_Status=1; return;
				}

	 			if(!calib_Status&&calibing) 
				{
					flash_ponter=flash_ponter_image=3;//��ʼ��˸λ�ú���Ҫ	  //�궨�˳�
					combinate_letter_digtal(OUQ,digtal_x);
					flash_ponter=3;flashing=1;	 original=LED_display_buffer;
					Cal_Setting_step=0;
				 
				   calibing=0;calib_Status=1;	return;
				}
		    }
				

	if((Read_key()==deletex)&&!programm_setting&&!calibing)	
			{	Cal_Setting_step=0;
	 			if(!deletex_Status&&!clearing) 
				{ 
						flashing=0;	 	 //state_operation=1;
						write_LED_Block(CLE,0,LED_LIGHTENX);  //�������
						Cal_Setting_step=0;
						clearing=1;	 deletex_Status=1;return;
				}

	 			if(!deletex_Status&&clearing) 
				{
					flash_ponter=flash_ponter_image=3;//��ʼ��˸λ�ú���Ҫ	  //����˳�
					combinate_letter_digtal(OUQ,digtal_x);
					flash_ponter=3;flashing=1;	 original=LED_display_buffer;
					Cal_Setting_step=0;	//state_operation=-1;
				 
				   clearing=0;	deletex_Status=1;return;
				}
		    }	
}

void Key_input_edge(void)
  {

//  	if((Read_key()!=shift_key))  shift_key_Status=0;
//	if((Read_key()!=data_incr))  data_incr_Status=0;
	  if((Read_key()==shift_key)) 
			 AD_code_display=1;
	  if((Read_key()==data_incr))
			  AD_code_display=0;


//	      {if(AD_code_display)
//			  AD_code_display=0;
//		  else
//		      AD_code_display=1;
//		  }


	if((Read_key()!=shift_key)) 
	 shift_key_Status=0;
	if((Read_key()!=data_incr)) 
	 data_incr_Status=0;


	if((Read_key()==data_confir))	{//data_confir_Status=1;
	                                 revers=0; }
	    	else
		   {data_confir_Status=0;entering=0;Delay(10);}
	if((Read_key()!=zero_ing))      zero_ing_Status=0;

	if((Read_key()!=tare_ing))tare_ing_Status=0;
	if((Read_key()!=setting))  { setting_Status=0;Delay(10);}
	if((Read_key()!=calib))	  calib_Status=0;
	if((Read_key()!=deletex)) deletex_Status=0;


  }


void interface_for_setting(void)
{
//   unsigned char P01[]={P,'0','1',0};
   unsigned char P02[]={P,'0','2',0};
   unsigned char P03[]={P,'0','3',0};
   unsigned char P04[]={P,'0','4',0};
   unsigned char End[]={0x0e,n,0x0d,0};
//   unsigned char Err[]={0x0e,0x50,0x50,0};
   unsigned char Err[]={0x0e,r,r,0};

			switch (Cal_Setting_step)
				{
//1.ͨѶ������
//dis_para00(seq_num, ID_budrate,1 );//int ID_budrate;//���������ʹ����� RAM 
			   			case 1:
								KLTT_AADR=comm_bud_rate-1;
							    dis_para00(Cal_Setting_step, ID_budrate,4 );
						break;
//dis_para00(seq_num, Comm_Addr,1 );//RAM  int Comm_Addr;//����ͨѶ��ַ������
			   			case 2:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Comm_Addr,2 );
						break;
//2.����������
//dis_para00(seq_num, Initial_zero_range,1 );//RAM  int Initial_zero_range;//�����������㷶Χ
			   			case 3:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Initial_zero_range,2 );
						break;

//dis_para00(seq_num, Munal_zero_range,1 );//RAM  int Munal_zero_range;//�����ֶ����㷶Χ
			   			case 4:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Munal_zero_range,4 );
						break;

//dis_para00(seq_num, Trace_zero_range,1 );//RAM  int Trace_zero_range;//��������ٷ�Χ
			   			case 5:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Trace_zero_range,3 );
						break;
//3.��ʱ������
//dis_para00(seq_num, Delay_tare,1 );//RAM  int Delay_tare;//����ȥƤ��ʱ���
			   			case 6:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_tare,2 );
						break;

//dis_para00(seq_num, Delay_Mid_feed,1 );//RAM  int Delay_Mid_feed;//�����м���ʱ
			   			case 7:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_Mid_feed,2 );
						break;

//dis_para00(seq_num, Delay_Pre_process,1 );//RAM  int Delay_Pre_process;//������ǰ��ʱ
			   			case 8:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_Pre_process,2 );
						break;

//dis_para00(seq_num, Delay_Next_loop,1 );//RAM  int Delay_Next_loop;//������һѭ����ʱ
			   			case 9:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_Next_loop,2 );
						break;

//dis_para00(seq_num, Time_supvisor_Slow_feed_up_limt,1 );//RAM  int Time_supvisor_Slow_feed_up_limt;//�������Ӽ�������ʱ��
			   			case 10:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Time_supvisor_Slow_feed_up_limt,2 );
						break;

//dis_para00(seq_num, Time_supvisor_Slow_feed_low_limt,1 );//RAM  int Time_supvisor_Slow_feed_low_limt;//�������Ӽ�������ʱ��
			   			case 11:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Time_supvisor_Slow_feed_low_limt,2 );
						break;

//dis_para00(seq_num, Pulse_howmany_package,1 );//RAM  int Pulse_howmany_package;//�������ٴ��������
			   			case 12:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Pulse_howmany_package,4 );
						break;

//dis_para00(seq_num, Tare_howmany_package,1 );//RAM  int Tare_howmany_package;//�������ٴ�ȥƤһ��
			   			case 13:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Tare_howmany_package,4 );
						break;

//dis_para00(seq_num, Recive_x_signals,1 );//RAM  int Recive_x_signals;//�����յ�x���ź��趨
			   			case 14:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Recive_x_signals,2 );	  //���ó��ؿ�����ϣ���ʾ������ʱ
						break;

//dis_para00(seq_num, no_recive_x_signals,1 );//RAM  int no_recive_x_signals;//�����ղ���x���ź��趨��
			   			case 15:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, no_recive_x_signals,4 );
						break;

//dis_para00(seq_num, Even_howmany_package_Pre_process,1 );//RAM  int Even_howmany_package_Pre_process;//�������ٴ�ƽ������һ����ǰ��
			   			case 16:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Even_howmany_package_Pre_process,4 );
						break;

//dis_para00(seq_num, Delay_slow_feed,1 );//RAM  int Delay_slow_feed;//����������ʱ
			   			case 17:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Delay_slow_feed,2 );
						break;

//dis_para00(seq_num, Delay_initial_feed,1 );//RAM  int Delay_initial_feed;//����������ʱ
			   			case 18:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_initial_feed,2 );
						break;
//4.����������   
//dis_para00(seq_num, Slow_feed_modify_uper_limt,1 );//RAM  int Slow_feed_modify_uper_limt;//����������������
			   			case 19:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Slow_feed_modify_uper_limt,5 );
						break;

//dis_para00(seq_num, Slow_feed_modify_down_limt,1 );//RAM  int Slow_feed_modify_down_limt;//����������������
			   			case 20:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Slow_feed_modify_down_limt,5 );
						break;

//dis_para00(seq_num, Pre_feed_modify_uper_limt,1 );//RAM  int Pre_feed_modify_uper_limt;//������ǰ����������
			   			case 21:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Pre_feed_modify_uper_limt,5 );
						break;

//dis_para00(seq_num, Pre_feed_modify_down_limt,1 );//RAM  int Pre_feed_modify_down_limt;//������ǰ����������
			   			case 22:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Pre_feed_modify_down_limt,5 );
						break;

//dis_para00(seq_num, Slow_feed,1 );//RAM  int Slow_feed;//����������
			   			case 23:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Slow_feed,5 );
						break;

//dis_para00(seq_num, Pre_feed,1 );//RAM  int Pre_feed;//������ǰ��
			   			case 24:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
//err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;	
								if(err_setting_Slow_feed)
								    {write_LED_Block(Err,0,LED_LIGHTENX);err_setting_Slow_feed_timer=30;}  //Cal_Setting_step=0;	flashing=0;	
								else
								    {dis_para00(Cal_Setting_step, Pre_feed,5 );}
						break;

//dis_para00(seq_num, Mid_feed,1 );//RAM  int Mid_feed;//�����м�ֵ
			   			case 25:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
//err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;	
								if(err_setting_Pre_feed)
								    {write_LED_Block(Err,0,LED_LIGHTENX);err_setting_Slow_feed_timer=30;}  //Cal_Setting_step=0;	flashing=0;	
								else
								    {dis_para00(Cal_Setting_step, Mid_feed,5 );}
						break;

//dis_para00(seq_num, Dlet_error,1 );//RAM  int Dlet_error;//�����ʲ�
			   			case 26:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Dlet_error,5 );
						break;


//dis_para00(seq_num, Target_uper_limt,1 );//RAM  int Target_uper_limt;//������������		 //*2017-10-28am*
			   			case 27: //28:
						        if(!flag_borrow_interface_for_setting)
								{
									KLTT_AADR+=1;
									write_para01(KLTT_AADR,5);
								}
							     dis_para00(Cal_Setting_step, Target_uper_limt,5 );
								 
						break;

//dis_para00(seq_num, Target_down_limt,1 );//RAM  int Target_down_limt;//������������	    //*2017-10-28am*
			   			case 28: //29:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Target_down_limt,5 );
						break;
//dis_para00(seq_num, Target,1 );//RAM  int Target;//������ֵ
			   			case 29: //27:													 //*2017-10-28am*
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Target,5 );
						break;

//dis_para00(seq_num, zero_range,1 );//RAM  int zero_range;//��������
			   			case 30:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);

//err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;	
								if(err_setting_Target)
								    {write_LED_Block(Err,0,LED_LIGHTENX);err_setting_Slow_feed_timer=30;}  //Cal_Setting_step=0;	flashing=0;	
								else
								  {
											if(flag_borrow_interface_for_setting&&Cal_Setting_step==30)
											{flag_borrow_interface_for_setting=0;
		
										  				flash_ponter=0;
														flash_ponter_image=0;//��ʼ��˸λ�ú���Ҫ	//�趨�˳�
										//					combinate_letter_digtal(OUQ,digtal_x);
														flash_ponter=0;flashing=0;	
														 original=LED_display_buffer;
													   write_LED_Block(dim_x,0,Signal_lamp);
														Cal_Setting_step=0;
													   programm_setting=0;
										//				   STB_high0;//STB=1
													   Delay(100000); setting_Status=1;
													   flag_borrow_interface_for_setting=0;
													   return;
												 }
		
									    dis_para00(Cal_Setting_step, zero_range,5 );
							   }

						break;

//dis_para00(seq_num, Slow_feed_modify,1 );//RAM  int Slow_feed_modify;//��������������
			   			case 31:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Slow_feed_modify,5 );
						break;

//dis_para00(seq_num, Initial_power_value,1 );//RAM  int Initial_power_value;//�������ϵ�ֵ
			   			case 32:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Initial_power_value,1 );
						break;

//dis_para00(seq_num, Initial_feed_value,1 );//RAM  int Initial_feed_value;//����������
			   			case 33:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Initial_feed_value,5 );
						break;

//dis_para00(seq_num, Random_setting_value,1 );//RAM  int Random_setting_value;//��������趨ֵ
			   			case 34:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Random_setting_value,5 );
						break;
//5.����������
//dis_para00(seq_num, ID_user_cal,1 );//RAM  int ID_user_cal ;//�����û��궨������
			   			case 35:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, ID_user_cal,2 );
						break;
//dis_para00(seq_num, ID_slow_feed_modify,1 );//RAM  int ID_slow_feed_modify ;//��������������ǹ�����
			   			case 36:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, ID_slow_feed_modify,1 );
						break;
//dis_para00(seq_num, ID_pre_feed_modify,1 );//RAM  int ID_pre_feed_modify;//������ǰ������ǹ�����
			   			case 37:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, ID_pre_feed_modify,1 );
						break;
//dis_para00(seq_num, ID_run_mode,1 );//RAM  int ID_run_mode;//��������ģʽ������
			   			case 38:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, ID_run_mode,1 );
								if(ID_run_mode)
									Signal_lamp[3]=0x02;
								else
									Signal_lamp[3]=0x0;
						break;
//8.�˲������飺
//dis_para00(seq_num, Range_Filter0,4 );//RAM  int Range_Filter0;//�����˲���Χ0
			   			case 39:
								KLTT_AADR=ID_run_mode_address;//0x608;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Range_Filter0,2 );
						break;

//dis_para00(seq_num, Range_Filte1r,4 );//RAM  int Range_Filter1;//�����˲���Χ1
			   			case 40:
								KLTT_AADR=Range_Filter0_address;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Range_Filter1,2 );
						break;

//dis_para00(seq_num, Range_Filter2,4 );//RAM  int Range_Filter2;//�����˲���Χ2
			   			case 41:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Range_Filter2,2 );
						break;

//dis_para00(seq_num, Range_Filter3,4 );//RAM  int Range_Filter3;//�����˲���Χ3
			   			case 42:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Range_Filter3,2 );
						break;

//dis_para00(seq_num, Range_Filter4,4 );//RAM  int Range_Filter4;//�����˲���Χ4
			   			case 43:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Range_Filter4,2 );
						break;

//dis_para00(seq_num, Strong_Filter0,2 );//RAM  int Strong_Filter0;//�����˲�ǿ��0
			   			case 44:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter0,1 );
						break;

//dis_para00(seq_num, Strong_Filter1,2 );//RAM  int Strong_Filter1;//�����˲�ǿ��1
			   			case 45:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter1,1);
						break;

//dis_para00(seq_num, Strong_Filter2,2 );//RAM  int Strong_Filter2;//�����˲�ǿ��2
			   			case 46:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter2,1 );
						break;

//dis_para00(seq_num, Strong_Filter3,2 );//RAM  int Strong_Filter3;//�����˲�ǿ��3
			   			case 47:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter3,1 );
						break;

//dis_para00(seq_num, Strong_Filter4,2 );//RAM  int Strong_Filter4;//�����˲�ǿ��4
			   			case 48:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter4,1 );
						break;
//return start
			   			case 49:
								KLTT_AADR+=1;
//								Cal_Setting_step=0;
								write_para01(KLTT_AADR,2);
								flashing=0;	 write_LED_Block(End,0,LED_LIGHTENX);  Cal_Setting_step=0;
						break;

//  set_shut_initial_mid_slow_prefeed_timer=ID_user_cal;  //*2017-10-28am*
			   			case 50:
//								flashing=0;	 write_LED_Block(End,0,LED_LIGHTENX);  Cal_Setting_step=0;
								
						break;



//			   			case 51:
//									Cal_Setting_step=0;
//						break;

//
//					case 1:
//								seq_num=Cal_Setting_step;//С��Ļ�趨/�궨�Ĳ������
//							    dis_para00(seq_num, ID_budrate,4 );//ppp������RAM��ַ  ���������ʹ�����
//						break;
//					case 2:
//								seq_num=Cal_Setting_step;
//								write_para01(comm_bud_rate,4);//xxx:Ϊʹ��USART1�еĳ����Ӧ������̩ͨ�ĵ�ַ
//							    dis_para00(seq_num, Comm_Addr,1 );//ppp������RAM��ַ	����ͨѶ��ַ������
//						break;
// 					case 3:
//								seq_num=Cal_Setting_step;
//								write_para01(comm_address,4); //xxx:Ϊʹ��USART1�еĳ����Ӧ������̩ͨ�ĵ�ַ
//							    dis_para00(seq_num, ppp,1 ); //ppp������RAM��ַ �����������㷶Χ
//						break;
// 					case 4:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:Ϊʹ��USART1�еĳ����Ӧ������̩ͨ�ĵ�ַ
//							    dis_para00(seq_num, ppp,1 );//ppp������RAM��ַ
//						break;
// 					case 5:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:Ϊʹ��USART1�еĳ����Ӧ������̩ͨ�ĵ�ַ
//							    dis_para00(seq_num, ppp,1 );//ppp������RAM��ַ
//						break;
// 					case 6:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:Ϊʹ��USART1�еĳ����Ӧ������̩ͨ�ĵ�ַ
//							    dis_para00(seq_num, ppp,1 );//ppp������RAM��ַ
//						break;
// 					case 7:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:Ϊʹ��USART1�еĳ����Ӧ������̩ͨ�ĵ�ַ
//							    dis_para00(seq_num, ppp,1 );//ppp������RAM��ַ
//						break;
// 					case 8:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:Ϊʹ��USART1�еĳ����Ӧ������̩ͨ�ĵ�ַ
//							    dis_para00(seq_num, ppp,1 );//ppp������RAM��ַ
//						break;



//					case 3:
//
//
//
//						break;
//					case 4:
//						break;


 					case 200:
						break;

					default:
						break;
				}		  	 

}


void  interface_for_calbing(void)
{
   unsigned char P1[]={P,0};
   unsigned char digtal_p[]={'0',0x0};

   unsigned char D[]={0x0d,0};
   unsigned char digtal_d[]={'0','0',0x0};

   unsigned char F[]={0x0f,0};
   unsigned char digtal_f[]={'0','1','2','3','4',0x0};

   unsigned char Ht[]={H,t,0};
   unsigned char digtal_w[]={'5','1','8','3','4',0x0};

   unsigned char Err[]={0x0e,r,r,0x0};
   unsigned char good[]={'9',o,o,0x0d,0x0};		   

			switch (Cal_Setting_step)
				{
 			   /******************�궨************************/
			   			case 1:
							    dis_para01(P, Cal_point,1 );	//��ʾС����
						break;

			   			case 2:
								KLTT_AADR=Cal_pint_address;  //����С����
								write_para01(KLTT_AADR,1);
							    dis_para01(0x0d, Cal_ID_division,2 );//��ʾ�ֶ���
						break;

			   			case 3:
								KLTT_AADR=Cal_ID_division_address;  //Cal_pint_address  = 1562//����ֶ���
								write_para01(KLTT_AADR,2);
							    dis_para01(0x0f, Cal_full_Mig,5 );//��ʾ������ֵ
						break;

			   			case 4:
								KLTT_AADR=Cal_full_Mig_address;  //Cal_pint_address  = 1562//����������ֵ
								write_para01(KLTT_AADR,5);
							    dis_para01(0x0a, Analog_mode_SEL,1 );//��ʾģ����ѡ��
						break;

			   			case 5:
								KLTT_AADR=Analog_mode_SEL_address;  //����ģ����ѡ��
								write_para01(KLTT_AADR,1);
							    dis_para01(0x0c, (int)Modbus_pulse.target_totall_pulse,5 );//��ʾ������������������

//					Modbus_Counter++;//������������������
//					if(Modbus_Address==1503) //1003:Modbus��ַ	 1003  FF_Target_pulse
//					{
//						Modbus_pulse.pulse[3]=0;
//						Modbus_pulse.pulse[2]=0;
//						Modbus_pulse.pulse[1]=Modbus_0x10_Value[0];
//						Modbus_pulse.pulse[0]=Modbus_0x10_Value[1];
//						STMFLASH_Write(FF_Target_pulse,(u16*)&Modbus_pulse.target_totall_pulse,4);	 Cal_user_weight
//					}

						break;

			   			case 6:
								KLTT_AADR=1503;  //���������������������
								write_para01(KLTT_AADR,5);
							    dis_para01(U, (int)Cal_user_weight,5 );//��ʾ�궨�û���������
						break;

			   			case 7:
								KLTT_AADR=Cal_user_weight_address;  //����궨�û���������
								write_para01(KLTT_AADR,5);
						//��λ
							  flashing=0;	 
							  write_LED_Block(noLOAd,0,LED_LIGHTENX);
						break;

	 					case 8:	  // �ӷ���
						      //������λ
							  zero_point=AD_Result0; 
							  flashing=0;Cal_output;//*2017-10-28am*	 
							  write_LED_Block(ADLOAd,0,LED_LIGHTENX);
						break;
	 					case 9:	  //�������� ������ʾ�û���������

								dis_para01(U, (int)Cal_user_weight,5 );//��ʾ�궨�û���������
								ad_coefficent=((float)(AD_Result0-zero_point))/(Cal_weight/temp__division);	//�궨��������//
//								Un_Cal_output;//*2017-10-28am*	 								flash_ponter=flash_ponter_image=5;//��ʼ��˸λ�ú���Ҫ
//								combinate_letter_digtal(Ht,digtal_w);
//								flashing=1;	 original=LED_display_buffer;	//������������д	 write_LED_Block �������һ��LED����

						break;

	  					case 10:	  //д�궨�����������������
//Ϊ�˿���12v������������=2mv/V,�ֶ���=1��ȫ��20000�ֶȣ�ϵ������<47.855.
//ѡϵ����С=45
								temp__division=select_division();
								KLTT_AADR=Cal_weight_address;  //����궨�û���������
								write_para01(KLTT_AADR,5);
								ad_coefficent=((float)(AD_Result0-zero_point))/(Cal_weight/temp__division);	//�궨��������//								flash_ponter=flash_ponter_image=5;//��ʼ��˸λ�ú���Ҫ
								Un_Cal_output;//*2017-10-28am*	 								flash_ponter=flash_ponter_image=5;//��ʼ��˸λ�ú���Ҫ
//								if(ad_coefficent<45)
//						    	    {ad_coefficent=1;//cal_finished=0;
//									 flashing=0;write_LED_Block(Err,0,LED_LIGHTENX);//	�����궨����������Ϣ��
//									}//���غ󲻱���ϵ������λ
//							    else
//									{
									  ad_coefficent/=temp__division;
										AD_ZERO_STORE.zero_point=zero_point;
										STMFLASH_Write(AD_ZEROX,(u16*)AD_ZERO_STORE.zero_pointx,4);//������λ
										STMFLASH_Read (AD_ZEROX,(u16*)AD_ZERO_STORE.zero_pointx,4);
										
										AD_COFF_STORE.ad_coefficent=ad_coefficent;
										STMFLASH_Write(AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);//����ϵ��
										STMFLASH_Read (AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);

//										 _ltoa(Dis_ad,ad_coefficent*10000);	
										 
//										 display_main_weight(); 
//									 flashing=0;write_LED_Block(good,0,LED_LIGHTENX);

//									 flashing=0;write_LED_Block(Dis_ad,0,LED_LIGHTENX);
							         dis_para00((int)P1, ad_coefficent*10000,6 );
//									}//���غ󱣴�ϵ������λ	   _ltoa(Dis_ad,ad_coefficent);	display_main_weight(); //��ʾAD_CODE

						break;
						case 11:
									borrow_write_para01_to_get_coeff=1;
									write_para01(KLTT_AADR,1);	//get coeffcirnt on the interface
								    ad_coefficent/=10000;
									AD_COFF_STORE.ad_coefficent=ad_coefficent;
									STMFLASH_Write(AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);//����ϵ��
									STMFLASH_Read (AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);

									flashing=0;write_LED_Block(Dis_ad,0,LED_LIGHTENX);

						break;





//					case 1://С����
//								flash_ponter=flash_ponter_image=1;//��ʼ��˸λ�ú���Ҫ
//								combinate_letter_digtal(P1,digtal_p);
//								flashing=1;	 original=LED_display_buffer;	//������������д	 write_LED_Block �������һ��LED����
//						break;
//
//					case 2:	//�ֶ���
//								flash_ponter=flash_ponter_image=2;//��ʼ��˸λ�ú���Ҫ
//								combinate_letter_digtal(D,digtal_d);
//								flashing=1;	 original=LED_display_buffer;	//������������д	 write_LED_Block �������һ��LED����
//						break;
//
//					case 3:	//������
//								flash_ponter=flash_ponter_image=5;//��ʼ��˸λ�ú���Ҫ
//								combinate_letter_digtal(F,digtal_f);
//								flashing=1;	 original=LED_display_buffer;	//������������д	 write_LED_Block �������һ��LED����
//						break;
//					case 4:	//��λ
//							  flashing=0;	 
//							  write_LED_Block(noLOAd,0,LED_LIGHTENX);
//
//
//						break;
//
//
// 					case 5:	  // �ӷ���
//							  flashing=0;	 
//							  write_LED_Block(ADLOAd,0,LED_LIGHTENX);
//						break;
// 					case 6:	  //��������
//								flash_ponter=flash_ponter_image=5;//��ʼ��˸λ�ú���Ҫ
//								combinate_letter_digtal(Ht,digtal_w);
//								flashing=1;	 original=LED_display_buffer;	//������������д	 write_LED_Block �������һ��LED����
//
//						break;
//
//  					case 7:	  //�������
//						break;


					default:
						break;
				}
//		}
 }

 /******************���************************/

  void interface_for_clearing(void)
  {
  			switch (Cal_Setting_step)
				{
 			   /******************���************************/
			   			case 1:
							    dis_para02(Cal_Setting_step, Weighting_T,5 );	//��ʾС��λ
						break;
  			   			case 2:
							    dis_para02(Cal_Setting_step, Weighting_package,5 );	//��ʾ����
						break;
						case 3:
			 					programm_setting=1;	setting_Status=1; 
								KLTT_AADR=comm_bud_rate-1+26;
								Cal_Setting_step=27;
								flag_borrow_interface_for_setting=1;//for seting  target Target_down_limt Target_uper_limt
								clearing=0;
								interface_for_setting();		    //
					   	break;
					default:
						break;
				}
  
  }


//  void interface_for_clearing(void)
//  {
//// char i;
//// long trans_buffer;
//// unsigned char *p;  char data_buffer[10],*q;
////// long para0;
////// 									  if(clearing||calibing||programm_setting)return;	   //�趨 �궨 ��� ʱ��Ч
////
////
////;
//
//  			switch (Cal_Setting_step)
//				{
// 			   /******************���************************/
//
//
//
////dis_para00(seq_num, Target_down_limt,1 );//RAM  int Target_down_limt;//������������	    //*2017-10-28am*  28
//
//						case 1:
//								   dis_para00(Cal_Setting_step, sel_AD_code,1 );//select AD CODE or weight
//						break;
//			   			case 2:
//									
//
//									sel_AD_code=LED_display_buffer[0];
//
// //**********************************************************************************
//
//								KLTT_AADR=comm_bud_rate-1+26;
//								dis_para00(Cal_Setting_step, Target_uper_limt,5 );
//						break;
// //dis_para00(seq_num, Target_uper_limt,1 );//RAM  int Target_uper_limt;//������������		 //*2017-10-28am* 27
//			   			case 3: //28:
//								KLTT_AADR+=1;
//								write_para01(KLTT_AADR,5);
//							    dis_para00(Cal_Setting_step, Target_down_limt,5 );
//						break;
////dis_para00(seq_num, Target,1 );//RAM  int Target;//������ֵ
//			   			case 4: //27:													 //*2017-10-28am*	  29
//								KLTT_AADR+=1;
//								write_para01(KLTT_AADR,5);
//							    dis_para00(Cal_Setting_step, Target,5 );
//						break;
//
//			   			case 5:
//								KLTT_AADR+=1;
//								write_para01(KLTT_AADR,5);
//							    dis_para02(Cal_Setting_step, Weighting_T,5 );	//��ʾС��λ
//						break;
//  			   			case 6:
//							    dis_para02(Cal_Setting_step, Weighting_package,5 );	//��ʾ����
//						break;
//
//
//  				
//					default:
//						break;
//				}
//  
//  }

 
  void dis_para02(int seq_num,int para,char _image_ponter)
 {
    unsigned char seq_x[10];
	seq_x[0]=0x0c;

								_ltoa(Dis_ad,(long)seq_num);
								seq_x[1]=Dis_ad[0];	seq_x[2]=Dis_ad[1];seq_x[3]=0;
								_ltoa(Dis_ad,(long)para);
								combinate_letter_digtal(seq_x,Dis_ad);

								flash_ponter=flash_ponter_image=_image_ponter;//��ʼ��˸λ�ú���Ҫ
								flashing=1;	 original=LED_display_buffer;	//������������д	 write_LED_Block �������һ��LED����
 
 }
 	  
 void dis_para00(int seq_num,int para,char _image_ponter)
 {
    unsigned char seq_x[10];
	seq_x[0]=P;

								_ltoa(Dis_ad,(long)seq_num);
								seq_x[1]=Dis_ad[0];	seq_x[2]=Dis_ad[1];seq_x[3]=0;
								_ltoa(Dis_ad,(long)para);
								combinate_letter_digtal(seq_x,Dis_ad);

								flash_ponter=flash_ponter_image=_image_ponter;//��ʼ��˸λ�ú���Ҫ
								flashing=1;	 original=LED_display_buffer;	//������������д	 write_LED_Block �������һ��LED����
 
 }
void dis_para01(unsigned char seq_num,int para,char _image_ponter)
 {
    unsigned char seq_x[10];
	seq_x[1]=0;seq_x[0]=seq_num;	
								_ltoa(Dis_ad,(long)para);
								combinate_letter_digtal(seq_x,Dis_ad);

								flash_ponter=flash_ponter_image=_image_ponter;//��ʼ��˸λ�ú���Ҫ
								flashing=1;	 original=LED_display_buffer;	//������������д	 write_LED_Block �������һ��LED����
 }
 void write_para01(int para,char _image_ponter)
 {
 char i;
 long trans_buffer;
 unsigned char *p;  char data_buffer[10],*q;
 long para0;
// 									  if(clearing||calibing||programm_setting)return;	   //�趨 �궨 ��� ʱ��Ч


 if(revers){revers=0;return;}//ǰ��д������Ч}	   //�趨 �궨 ��� ʱ������Ч
 p=LED_display_buffer+8-flash_ponter;
 q=data_buffer;

 	for(i=0;i<flash_ponter;i++)
	{
	  if(!(*p<0x30||*p>0x39))
	  *q++=*p++;
	  else
	   *p++;//�ڲ���涨���ݳ���ʱ������ ǰ�淢��λ
	}
	*q=0;


// 	for(i=8-flash_ponter;i<8;i++)
//	{
//	  data_buffer[i-(8-flash_ponter)]=LED_display_buffer[i];
//	}
//	data_buffer[flash_ponter]=0;
    trans_buffer=atol(data_buffer);
	para0=trans_buffer;
				if(borrow_write_para01_to_get_coeff)
				{	ad_coefficent=(float)para0;
					borrow_write_para01_to_get_coeff=0;
					return;
				}
			

	Modbus_0x10_Value[0]=para0>>8;//���������ʹ�����
	Modbus_0x10_Value[1]=para0&0x00ff;

	Modbus_Address=para;	 //store data//���������ʹ�����
	Modbus_Counter=9;
	wang_USART1_Process();


 }
//.\usart1.c(226): extern int image_Pre_feed,system_indicator;
 void signal_convert_to_LED (void)
{
//  				if(Feed_signal_buffer[1]&0x08) //�ȶ�	
//				{Signal_lamp[2]=0x02;}
//				else
//				{Signal_lamp[2]=0x00;}



	if(state_operation<0) //	�˳����Ƴ���LED��Ϊ���	 
	{
	
//				Signal_lamp[4]=0x00;   Signal_lamp[5]=0x00; 
//				Signal_lamp[6]=0x00;   Signal_lamp[7]=0x00;
//err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;	

//Signal_lamp[4]=0x02; Signal_lamp[5]=0x02;	Signal_lamp[6]=0x02;
//  Signal_lamp[4]=0x02;

  				if(Feed_signal_buffer[1]&0x08) //�ȶ�	
				{Signal_lamp[2]=0x02;}
				else
				{Signal_lamp[2]=0x00;}

				if(ID_run_mode) //�Զ�	
				{Signal_lamp[3]=0x02;}
				else
				{Signal_lamp[3]=0x00;}



//				if(Feed_signal_buffer[1]&0x80) //ȥƤ	
//				{Signal_lamp[0]=0x02;}	// ���
//				else
//				{Signal_lamp[0]=0x00;}
				
				if(Feed_signal_buffer[1]&0x40) //��λ	
				{Signal_lamp[1]=0x02;}
				else
				{Signal_lamp[1]=0x00;}
				
				Signal_lamp[6]=0x00;	
//				if(Feed_signal_buffer[1]&0x08) //�ȶ�	
//				{Signal_lamp[2]=0x02;}
//				else
//				{Signal_lamp[2]=0x00;}
				
//				if(ID_run_mode) //�Զ�	
//				{Signal_lamp[3]=0x02;}
//				else
//				{Signal_lamp[3]=0x00;}

//				if(err_setting_Slow_feed==1) //	
//				{Signal_lamp[4]=0x02;}	// ���
//				else
//				{Signal_lamp[4]=0x00;}
//
//				if(err_setting_Pre_feed==1) //	
//				{Signal_lamp[5]=0x02;}	// ���
//				else
//				{Signal_lamp[5]=0x00;}
//
//				if(err_setting_Target==1) //	
//				{Signal_lamp[6]=0x02;}	// ���
//				else
//				{Signal_lamp[6]=0x00;}
	}
	else //������Ƴ���LED��Ϊ�̵�
	{
	
					
	
//					if(system_indicator==2) //ȥƤ���	
//					{Signal_lamp[0]=0x01;}	// �̵�
//					else
//					{Signal_lamp[0]=0x00;}


//					if(system_indicator==3) //ȥƤ���	
//					{Signal_lamp[1]=0x01;}	// �̵�
	
//					if(system_indicator==4) //����	
//					{Signal_lamp[2]=0x01;}	// �̵�
//	
//					if(system_indicator==5) //�м�	
//					{Signal_lamp[3]=0x01;}	// �̵�
//	
//					if(system_indicator==6) //����	
//					{Signal_lamp[4]=0x01;}	// �̵�
//	
//					if(system_indicator==7) //��ǰ	
//					{Signal_lamp[5]=0x01;}	// �̵�
	
//					if(system_indicator==8) //��λ	
					if (Modbus_weight.weight_long<=zero_range) 
					   Signal_lamp[6]=0x01;	// �̵�
					else
					   Signal_lamp[6]=0x00;	
	
//					if(system_indicator==10) //�ϸ�	
//					{Signal_lamp[7]=0x01;}	// �̵�
//	
////					if(system_indicator==9) //����	
////					{Signal_lamp[4]=0x02;}	// ���
//
//					if(system_indicator==9) //����	
//					{Signal_lamp[1]=0x01;}	// ���
//	
////					if(system_indicator==1) //Ƿ��	
////					{Signal_lamp[5]=0x02;}	// ���
//
//					if(system_indicator==1) //Ƿ��	
//					{Signal_lamp[0]=0x01;}	// ���

	
	}
}
void display_main_weight(void)
{
if(!clearing&&!calibing &&!programm_setting)	   //������ʾ����
   {flashing=0; 
//      write_LED_Block(scalb,Cal_point,Signal_lamp);
//       Delay(500);  
		Modify_Dis_ad();
	   write_LED_Block(Dis_ad,Cal_point,Signal_lamp);
   }
else
			original=LED_display_buffer; //������
}


void Modify_Dis_ad(void)
{
char lenx,i,*p,*q,buffre[9];
int temp;
	if(!Cal_point)return;

	if(Dis_ad[0]==0x16)  
	lenx=strlen(Dis_ad+1);//minus
    else
	lenx=strlen(Dis_ad);//positive


	temp=Cal_point-lenx;
	if(temp<0)
	return;


	temp++;
	if(Dis_ad[0]==0x16)  //minus
	{
		p=Dis_ad+1;q=buffre;
		do
		  {*q++=*p;}
		while(*++p);
		p=Dis_ad+1;
		for(i=0;i<temp;i++)
			*p++=0x30;
		q=buffre;
		for(i=0;i<lenx;i++)
			{*p++=*q++;}
		*p=0;
	}
   else
	{
		p=Dis_ad;q=buffre;
		do
		  {*q++=*p;}
		while(*++p);
		p=Dis_ad;
		for(i=0;i<temp;i++)
			*p++=0x30;
		q=buffre;
		for(i=0;i<lenx;i++)
			{*p++=*q++;}
		*p=0;
	}
}


void cal_display_for_LED(void)
{
										 if(!calibing)return;

			switch (Cal_Setting_step)
				{

			   			case 7:
						//��λ
							  flashing=0;	 
							  write_LED_Block(noLOAd,0,Signal_lamp);
						break;

	 					case 8:	  // �ӷ���
							  flashing=0;	 
							  write_LED_Block(ADLOAd,0,Signal_lamp);
						break;

					default:
						break;
				}
}

void defult_parameters(void)
{
long i;
if(!Input_setting_para)return;
for(i=0;i<1000000;i++);
if(!Input_setting_para)return;
//1.ͨѶ������
				//dis_para00(seq_num, ID_budrate,1 );//int ID_budrate;//���������ʹ����� RAM 
//				case 01:
				 ID_budrate=9600;
				//dis_para00(seq_num, Comm_Addr,1 );//RAM  int Comm_Addr;//����ͨѶ��ַ������
//				case 02:
				 Comm_Addr=2;
				//2.����������
				//dis_para00(seq_num, Initial_zero_range,1 );//RAM  int Initial_zero_range;//�����������㷶Χ
//				cas 03:
				 Initial_zero_range=30;
				//dis_para00(seq_num, Munal_zero_range,1 );//RAM  int Munal_zero_range;//�����ֶ����㷶Χ
//				cas 04:
				 Munal_zero_range=1000;
				//dis_para00(seq_num, Trace_zero_range,1 );//RAM  int Trace_zero_range;//��������ٷ�Χ
//				cas 05:
				 Trace_zero_range=90;
				//3.��ʱ������
				//dis_para00(seq_num, Delay_tare,1 );//RAM  int Delay_tare;//����ȥƤ��ʱ���
//				cas 06:
				 Delay_tare=40;
				//dis_para00(seq_num, Delay_Mid_feed,1 );//RAM  int Delay_Mid_feed;//�����м���ʱ
//				cas 07:
				 Delay_Mid_feed=0;
				//dis_para00(seq_num, Delay_Pre_process,1 );//RAM  int Delay_Pre_process;//������ǰ��ʱ
//				cas 08:
				 Delay_Pre_process=5;
				//dis_para00(seq_num, Delay_Next_loop,1 );//RAM  int Delay_Next_loop;//������һѭ����ʱ
//				cas 09:
				 Delay_Next_loop=10;
				//dis_para00(seq_num, Time_supvisor_Slow_feed_up_limt,1 );//RAM  int Time_supvisor_Slow_feed_up_limt;//�������Ӽ�������ʱ��
//				cas 10:
				 Time_supvisor_Slow_feed_up_limt=40;
				//dis_para00(seq_num, Time_supvisor_Slow_feed_low_limt,1 );//RAM  int Time_supvisor_Slow_feed_low_limt;//�������Ӽ�������ʱ��
//				cas 11:
				 Time_supvisor_Slow_feed_low_limt=20;
				//dis_para00(seq_num, Pulse_howmany_package,1 );//RAM  int Pulse_howmany_package;//�������ٴ��������
//				cas 12:
				 Pulse_howmany_package=100;
				//dis_para00(seq_num, Tare_howmany_package,1 );//RAM  int Tare_howmany_package;//�������ٴ�ȥƤһ��
//				cas 13:
				 Tare_howmany_package=100;
				//dis_para00(seq_num, Recive_x_signals,1 );//RAM  int Recive_x_signals;//�����յ�x���ź��趨
//				cas 14:
				 Recive_x_signals=0;	  //���ó��ؿ�����ϣ���ʾ������ʱ
				//dis_para00(seq_num, no_recive_x_signals,1 );//RAM  int no_recive_x_signals;//�����ղ���x���ź��趨��
//				cas 15:
				 no_recive_x_signals=10;
				//dis_para00(seq_num, Even_howmany_package_Pre_process,1 );//RAM  int Even_howmany_package_Pre_process;//�������ٴ�ƽ������һ����ǰ��
//				cas 16:
				 Even_howmany_package_Pre_process=100;
				//dis_para00(seq_num, Delay_slow_feed,1 );//RAM  int Delay_slow_feed;//����������ʱ
//				cas 17:
				 Delay_slow_feed=15;
				//dis_para00(seq_num, Delay_initial_feed,1 );//RAM  int Delay_initial_feed;//����������ʱ
//				cas 18:
				 Delay_initial_feed=0;
				//4.����������   
				//dis_para00(seq_num, Slow_feed_modify_uper_limt,1 );//RAM  int Slow_feed_modify_uper_limt;//����������������
//				cas 19:
				 Slow_feed_modify_uper_limt=600;
				//dis_para00(seq_num, Slow_feed_modify_down_limt,1 );//RAM  int Slow_feed_modify_down_limt;//����������������
//				cas 20:
				 Slow_feed_modify_down_limt=400;
				//dis_para00(seq_num, Pre_feed_modify_uper_limt,1 );//RAM  int Pre_feed_modify_uper_limt;//������ǰ����������
//				cas 21:
				 Pre_feed_modify_uper_limt=10;
				//dis_para00(seq_num, Pre_feed_modify_down_limt,1 );//RAM  int Pre_feed_modify_down_limt;//������ǰ����������
//				cas 22:
				 Pre_feed_modify_down_limt=3;
				//dis_para00(seq_num, Slow_feed,1 );//RAM  int Slow_feed;//����������
//				cas 23:
				 Slow_feed=500;
				//dis_para00(seq_num, Pre_feed,1 );//RAM  int Pre_feed;//������ǰ��
//				cas 24:
				 Pre_feed=7;
				//dis_para00(seq_num, Mid_feed,1 );//RAM  int Mid_feed;//�����м�ֵ
//				cas 25:
				 Mid_feed=2500;
				//dis_para00(seq_num, Dlet_error,1 );//RAM  int Dlet_error;//�����ʲ�
//				cas 26:
				 Dlet_error=10;
				//dis_para00(seq_num, Target_uper_limt,1 );//RAM  int Target_uper_limt;//������������		 //*2017-10-28am*
//				cas 27: 
				 Target_uper_limt=5050;
				//dis_para00(seq_num, Target_down_limt,1 );//RAM  int Target_down_limt;//������������	    //*2017-10-28am*
//				cas 28: 
				 Target_down_limt=4950;
				//dis_para00(seq_num, Target,1 );//RAM  int Target;//������ֵ
//				cas 29: :													 //*2017-10-28am*
				 Target=5000;
				//dis_para00(seq_num, zero_range,1 );//RAM  int zero_range;//��������
//				cas 30:
				 zero_range=200;
				//dis_para00(seq_num, Slow_feed_modify,1 );//RAM  int Slow_feed_modify;//��������������
//				cas 31:
				 Slow_feed_modify=20;
				//dis_para00(seq_num, Initial_power_value,1 );//RAM  int Initial_power_value;//�������ϵ�ֵ
//				cas 32:
				 Initial_power_value=0;
				//dis_para00(seq_num, Initial_feed_value,1 );//RAM  int Initial_feed_value;//����������
//				cas 33:
				 Initial_feed_value=3000;
				//dis_para00(seq_num, Random_setting_value,1 );//RAM  int Random_setting_value;//��������趨ֵ
//				cas 34:
				 Random_setting_value=0;
				//5.����������
				//dis_para00(seq_num, ID_user_cal,1 );//RAM  int ID_user_cal ;//�����û��궨������
//				cas 35:
				 ID_user_cal=0;
				//dis_para00(seq_num, ID_slow_feed_modify,1 );//RAM  int ID_slow_feed_modify ;//��������������ǹ�����
//				cas 36:
				 ID_slow_feed_modify=0;
				//dis_para00(seq_num, ID_pre_feed_modify,1 );//RAM  int ID_pre_feed_modify;//������ǰ������ǹ�����
//				cas 37:
				 ID_pre_feed_modify=0;
				//dis_para00(seq_num, ID_run_mode,1 );//RAM  int ID_run_mode;//��������ģʽ������
//				cas 38:
				 ID_run_mode=0;
				//8.�˲������飺
				//dis_para00(seq_num, Range_Filter0,4 );//RAM  int Range_Filter0;//�����˲���Χ0
//				cas 39:
				 Range_Filter0=22;
				//dis_para00(seq_num, Range_Filte1r,4 );//RAM  int Range_Filter1;//�����˲���Χ1
//				cas 40:
				 Range_Filter1=22;
				//dis_para00(seq_num, Range_Filter2,4 );//RAM  int Range_Filter2;//�����˲���Χ2
//				cas 41:
				 Range_Filter2=22;
				//dis_para00(seq_num, Range_Filter3,4 );//RAM  int Range_Filter3;//�����˲���Χ3
//				cas 42:
				 Range_Filter3=22;
				//dis_para00(seq_num, Range_Filter4,4 );//RAM  int Range_Filter4;//�����˲���Χ4
//				cas 43:
				 Range_Filter4=22;
				//dis_para00(seq_num, Strong_Filter0,2 );//RAM  int Strong_Filter0;//�����˲�ǿ��0
//				cas 44:
				 Strong_Filter0=2;
				//dis_para00(seq_num, Strong_Filter1,2 );//RAM  int Strong_Filter1;//�����˲�ǿ��1
//				cas 45:
				 Strong_Filter1=2;
				//dis_para00(seq_num, Strong_Filter2,2=;//RAM  int Strong_Filter2;//�����˲�ǿ��2
//				cas 46:
				 Strong_Filter2=2;
				//dis_para00(seq_num, Strong_Filter3,2 );//RAM  int Strong_Filter3;//�����˲�ǿ��3
//				cas 47:
				 Strong_Filter3=2;
				//dis_para00(seq_num, Strong_Filter4,2 );//RAM  int Strong_Filter4;//�����˲�ǿ��4
//				cas 48:
				 Strong_Filter4=2;
				//return start
				Cal_Setting_step=0;	  	programm_setting=0;


//														  				flash_ponter=0;
//														flash_ponter_image=0;//��ʼ��˸λ�ú���Ҫ	//�趨�˳�
//										//					combinate_letter_digtal(OUQ,digtal_x);
//														flash_ponter=0;flashing=0;	
//														 original=LED_display_buffer;
//													   write_LED_Block(dim_x,0,Signal_lamp);
//														Cal_Setting_step=0;
//													   programm_setting=0;
//										//				   STB_high0;//STB=1
//													   Delay(100000); setting_Status=1;
//													   flag_borrow_interface_for_setting=0;

	//add by John
	 Cal_point=2;//�궨С����
	 Cal_ID_division=0;//�궨�ֶ���������
	 Cal_full_Mig=50;//�궨������ֵ
	 Cal_user_weight=5000;//�궨�û���������
	 //Cal_zero;//�궨��λ
	 //Cal_ratio;//�궨����
	 Cal_weight=5000;	//�궨��������
	 Modbus_pulse.target_totall_pulse=18200;
	 //add by John------
}



