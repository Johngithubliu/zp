//#include <modbus_para.h>
#include "math.h"
#include "stm32f10x.h"
#include "usart1.h"
#include "delay.h"

#define XX_image_Pulse_howmany_package 1
#define XX_image_Tare_howmany_package  2
#define XX_image_Even_howmany_package_Pre_process  3
#define XX_image_Pre_feed  4 

void send_302(void);

int frezen_AD_DISPLAY_DELAY_TIMER;
int frezen_AD_DISPLAY_DELAY_TIMER_flag;

int delay_tare_timer; 
char delay_tare_timer_flag;
char set_shut_initial_mid_slow_prefeed_timer_35;

extern void Write_image_para(char i);

extern int stop_dis_AD_timer;
int image_Tare_howmany_package,Delt_temp;
int state_operation,reservestate_operation;
char c_t100,auto_t100,t100;
char taring_signal;
float zero_zone;
char ADC_STAB;
char Tare_Late_Status,Tare_state;
char Zero_Late_Status,Zero_state;
char Start_Late_Status,Input_material_door_Status;
char Stop_Late_Status;
char Clear_Late_Status,Input_machine_Pasue_Status;
char Input_calibration_Confirm_Status,Input_calibration_Status,Input_setting_para_Status;
char package_over,package_over_no_first;
//char main_powered;
int image_Pulse_howmany_package,image_Even_howmany_package_Pre_process;
int image_Pre_feed;
int system_indicator;
int sum;
char KLTT_Tare_Late_Status,KLTT_Zero_Late_Status ;
char count_enable_ok;
char cal_finished;



int Range_Filter_reg,Strong_Filter_reg;
extern unsigned long AD_QUEEN[40];
extern char QUEEN;
long AD_Result_buffer;
int  Ad_enter_stable_COUNTER;
unsigned long sum_ad;

void AD_F_process(void);


/****************************************************/
extern union Modbus_packed    
{    
float weight_long; 
unsigned char long_weight[4];   
}Modbus_weight;

extern float buff_tare;
extern long AFTER_AD_Result0;
extern char Feed_signal_buffer[2];//�����źţ��� ȥƤ ����
extern char KLTT_tare,KLTT_zero;
extern int Munal_zero_range;//�����ֶ����㷶Χ
extern int Cal_full_Mig;//�궨������ֵ
extern int Weighting_T,Weighting_package,Weighting_even;//����_��λ��������ƽ��ֵ

extern char Cal_zero_confirm_key_pressed,Cal_weight_confirm_key_pressed;
char Cal_zero_confirm_key_pressed_Late_Status,Cal_weight_confirm_key_pressed_Late_Status;
extern long zero_point;
extern float ad_coefficent;
extern unsigned long AD_Result0;


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

//5.����������
extern int ID_user_cal ;//�����û��궨������
extern int ID_slow_feed_modify ;//��������������ǹ�����
extern int ID_pre_feed_modify;//������ǰ������ǹ�����
extern int ID_run_mode;//��������ģʽ������

//7.int ;//�궨������
extern int Cal_code;//�궨����
extern int Cal_point;//�궨С����
extern int Cal_ID_division;//�궨�ֶ���������
extern int Cal_full_Mig;//�궨������ֵ
extern float Cal_user_weight;//�궨�û���������
extern int Cal_zero;//�궨��λ
extern int Cal_ratio;//�궨����
extern float Cal_weight;	//�궨��������

extern int Comm_Addr;//����ͨѶ��ַ������

/****************************************************/
extern char  X_input_state[50];
extern char  Y_output_state[50];	   //bool
/***output rename**/
#define Tare                 Y_output_state[0]=1
#define Pass                 Y_output_state[1]=1
#define Over                 Y_output_state[2]=1
#define Mid_fed              Y_output_state[3]=1
#define Slow_fed             Y_output_state[4]=1
#define Pre_fed              Y_output_state[5]=1
#define Start                Y_output_state[6]=1
#define Stop                 Y_output_state[7]=1
#define N_package_output     Y_output_state[8]=1
//#define Zero_zone_output     Y_output_state[9]=1
//#define Initial_power_output Y_output_state[10]=1
#define Material_door_output Y_output_state[9]=1
#define LOW_ERR              Y_output_state[10]=1
#define Random_output        Y_output_state[11]=1
#define Cal_output           Y_output_state[12]=1
#define Initial_output       Y_output_state[13]=1

#define Insert_board_output  Y_output_state[14]=1
#define Clamp_output         Y_output_state[15]=1


#define Un_Tare                 Y_output_state[0]=0
#define Un_Pass                 Y_output_state[1]=0
#define Un_Over                 Y_output_state[2]=0
#define Un_Mid_fed              Y_output_state[3]=0
#define Un_Slow_fed             Y_output_state[4]=0
#define Un_Pre_fed              Y_output_state[5]=0
#define Un_Start                Y_output_state[6]=0
#define Un_Stop                 Y_output_state[7]=0
#define Un_N_package_output     Y_output_state[8]=0
//#define Un_Zero_zone_output     Y_output_state[9]=0
#define Un_Material_door_output     Y_output_state[9]=0
//#define Un_Initial_power_output Y_output_state[10]=0
#define Un_LOW_ERR              Y_output_state[10]=0
#define Un_Random_output        Y_output_state[11]=0
#define Un_Cal_output           Y_output_state[12]=0
#define Un_Initial_output       Y_output_state[13]=0
#define Un_Insert_board_output  Y_output_state[14]=0
#define Un_Clamp_output         Y_output_state[15]=0

//���������
#define Input_start            X_input_state[0]
#define Input_stop             X_input_state[1]
#define Input_zero             X_input_state[2]
#define Input_tare             X_input_state[3]
#define Input_setting_para     X_input_state[4]
#define Input_calibration      X_input_state[5]
#define Input_clear_number     X_input_state[6]
#define Input_count_enable     X_input_state[7]
#define Input_clamp_bag        X_input_state[8]
#define Input_comm_calibration X_input_state[9]
#define Input_material_door_open X_input_state[10]
#define Input_machine_Pasue      X_input_state[11]
#define Input_calibration_Confirm X_input_state[12]
#define Input_material_door     X_input_state[7]


#define filter_len				 15
/****************************************************/
void accumulate(void);
void Test_Pulse_howmany_package(void);
void process_feed_material(void);
/****************************************************/
void Scanning_input_KLTT(void);
void Teating_weight_Random_output(void);
void Teating_weight_zero(void);
void process_feed_material(void);
void Test_input_edge(void);
void KLTT_Test_input_edge(void);
void Zero_operation(void);
void Tare_operation(void);
void KLTT_Cal_operation(void);
void Clear_output(void);
int select_division(void);
char FIN;
/****************************************************/
extern void store_ton_package(void);   

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

//extern unsigned char Signal_lamp[9];
extern int q_num;
extern void _ltoa( char *p, long wt);
char ADresult[20];
#define  stack_top 2 //30
float AD_result_stack[stack_top+2];
unsigned char stack_head;
//float weight_long;
void store_test_result_to_stack(void);

char  *get_test_result_form_stack(void);
extern unsigned char *status_and_datum(void);
//1.ѹ����head����Ȼ��head*4 2.ȡ����[3][2][1][0],���������ƶ�4��byte head-4
///****************************************************/
//   Scanning_input_KLTT();
//   Teating_weight_Random_output();
//   Teating_weight_zero();
//   process_feed_material();
//   Test_input_edge();
//   KLTT_Test_input_edge();
///****************************************************/
extern  unsigned char LED_LIGHTENX[9];//={0x01,0x02,0x01,0x03,0x0,0x0,0x0,0x0,0x0};
extern unsigned char Signal_lamp[9];
int shut_initial_mid_slow_prefeed_timer[4],set_shut_initial_mid_slow_prefeed_timer;  //*2017-10-28am*
int set__initial_mid_slow_prefeed_timer[4];
#define Machine_Pause 100
extern void  interface_for_calbing(void);
extern char calibing,programm_setting;
extern int Cal_Setting_step;
char initi_state;
extern void return_data(unsigned char *comm3_reply_point);
extern void return_data_xx(unsigned char *comm3_reply_point);


extern union Modbus_packed_1    
{    
long target_totall_pulse; 
unsigned char pulse[4];   
}Modbus_pulse;
int delt_pulse_count;
void delt_pulse(void);
extern unsigned int mtb_crc_calc( unsigned char *crc_str, char count);
 unsigned char 	delt_pulse_string_modbus[15]={0x0b,0x10,0x00,0x02,0x00,0x01,0x02};
 unsigned char sig_pulse_string_modbus[15]={0x0b,0x10,0x00,0x0f,0x00,0x01,0x02,0x00,0x00};


float Y0,Y1,Y2;

unsigned char comm3_buffer[10],*comm3_buffer_point,Data,comm3_Modbus_Counter;
 char Dis_adxx[10];
void PC_comm3_command(unsigned char Data);
int Data_Addr;
 
unsigned char F302=0;
unsigned char F302_Fram[9];
 
void Scanning_input_KLTT(void)
{
long i;
//		if (~Start_Late_Status&Input_start)
		if (Input_start)
		{
		//	for(i=0;i<10000000;i++);
		//	if(!Input_start)return;


		    if(state_operation==Machine_Pause)
			{state_operation=reservestate_operation;return;}   //*2017-10-28am*
// 					if (Modbus_weight.weight_long<=zero_range)  //�������� zero_range	 //*2017-10-28am*
					state_operation=1;reservestate_operation=0;image_Tare_howmany_package=Tare_howmany_package;//main_powered;	
			return;
		}//������	 //port.0
		
//		if (~Stop_Late_Status&Input_stop)	  //port.1
		

		if (Input_stop)	  //port.1
		 {
			//for(i=0;i<10000000;i++);
			//if(!Input_stop)return;

				state_operation=-1;
				Un_Tare                 ;//[0]=0
				Un_Pass                 ;//[1]=0
				Un_Over                 ;//[2]=0
				Un_Mid_fed              ;//[3]=0
				Un_Slow_fed             ;//[4]=0
				Un_Pre_fed              ;//[5]=0
				Un_Start                ;//[6]=0
				Un_Stop                 ;//[7]=0
				Un_N_package_output     ;//[8]=0
				Un_Material_door_output ;//[9]=0     
//				Un_Zero_zone_output     ;//[9]=0
				Un_LOW_ERR              ;//[10]=0
				Un_Random_output        ;//[11]=0
				Un_Cal_output           ;//[12]=0
				Un_Initial_output       ;//[13]=0
				Un_Insert_board_output  ;//[14]=0
				Un_Clamp_output         ;//[15]=0

				system_indicator=0;
				taring_signal=1;Tare_operation();
				return;
		 }


/****************************************************/	
			if(~Zero_Late_Status&Input_zero)  //rise edge
				{
					Zero_operation(); return;
				}
					
			if(~KLTT_Zero_Late_Status&KLTT_zero)  //rise edge  //����̩ͨ����	 //port.3
				{
					Zero_operation(); return;
				}

			if(~KLTT_Tare_Late_Status&KLTT_tare)  //rise edge  //����̩ͨȥƤ  //port.5
				{
				   Tare_operation(); return;
				}
			if(~Tare_Late_Status&Input_tare)  //rise edge
				{
				   Tare_operation(); return;
				}
			if(~Input_machine_Pasue_Status&Input_machine_Pasue)  //rise edge  //*2017-10-28am*
				{ reservestate_operation=state_operation;
				  state_operation=Machine_Pause; return; //pause 
				}
			if(~Input_calibration_Status&Input_calibration)  //rise edge  //*2017-10-28am*
				{

					Cal_Setting_step=8;calibing=1;				 //�궨��ʼ
					interface_for_calbing();


				}
			if(~Input_calibration_Confirm_Status&Input_calibration_Confirm)  //rise edge  //*2017-10-28am*
				{	

					Cal_Setting_step=9;				 //stepping	�궨 ȷ��
					interface_for_calbing();
					 c_t100=100;
					while(--c_t100);
					Cal_Setting_step=10;				 //stepping	�궨 ȷ��
					interface_for_calbing();
					 c_t100=50;
					while(--c_t100);
					Cal_Setting_step=0;				 //stepping	�궨 ȷ��
					calibing=0;
				}

			if(~Input_setting_para_Status&Input_setting_para)  //rise edge  //*2017-10-28am*
				{

					Cal_Setting_step=27;   //flashing=0;//�趨���������޶�����ʼ
					programm_setting=1;


				}

if(state_operation==-1)
 {
	 if(~Input_material_door_Status&&Input_material_door)
	{
			initi_state=1;
	}

	 switch (initi_state)
	 {
	 		case 1:
					Delt_temp=abs(Target-Initial_feed_value); // ��������ֵ Initial_feed_value ������ֵ Target
					if (Modbus_weight.weight_long>Delt_temp)
						{ c_t100=Delay_initial_feed;
							 Initial_output;
							 initi_state=2;
						}
			break;
			case 2:
					if (!c_t100)
						{	Un_Initial_output;	//1.�������=0 
						    initi_state=100;
						}
			break;
		default:
			break;
	}
 }
//X8���������С�Y15����ʱT34��Y15��(T34��0����ʱ����            ����ʱY15�ޣ�
if(state_operation>=1)
 {
	 if(~Input_material_door_Status&&Input_material_door)
	{
			initi_state=1;
	}

	 switch (initi_state)
	 {
	 		case 1:
						shut_initial_mid_slow_prefeed_timer[0]=Random_setting_value; //t34
						Insert_board_output;
						initi_state=2;
			break;
			case 2:
					if (!shut_initial_mid_slow_prefeed_timer[0])
						{	Un_Insert_board_output;	//1.������=0 
						    initi_state=100;
						}
			break;
		default:
			break;
	}
 }


//
////							Delt_temp=abs(Target-Pre_feed); // ������ǰ�� Pre_feed ������ֵ Target
////							if (Modbus_weight.weight_long>Delt_temp)
////								{ 	  }
//
//
//
//
//
//
//



/****************************************************/
	if (~Clear_Late_Status&Input_clear_number)
		{Weighting_T=0;Weighting_package=0;
		 store_ton_package();
			//��ʼ������
			image_Tare_howmany_package=Tare_howmany_package;
			image_Even_howmany_package_Pre_process=Even_howmany_package_Pre_process;
			image_Pulse_howmany_package=Pulse_howmany_package;
			image_Pre_feed=Pre_feed;

			Write_image_para(XX_image_Pulse_howmany_package);
			Write_image_para(XX_image_Tare_howmany_package);
			Write_image_para(XX_image_Even_howmany_package_Pre_process);
			Write_image_para(XX_image_Pre_feed);
		}	 //port.6

	if(Input_count_enable)
	   {count_enable_ok=1;}
}

void Zero_operation(void)
	{  if(taring_signal||!ADC_STAB)return;//��Ƥ,���ȶ� ������
//	   if((float)AFTER_AD_Result0/pow(10,Cal_point)<((float)Munal_zero_range/100)*(float)Cal_full_Mig)
		   if((float)abs(AFTER_AD_Result0)<(float)Munal_zero_range)
	   		{zero_point=AD_Result0;
	 	     Feed_signal_buffer[1]|=0x40;	//�����źţ��� ����λ��
		    }
	   else
	        {//	�����������������Ϣ��
			  Feed_signal_buffer[1]&=0xbf; 	//�����źţ��� ����λ��
			  Feed_signal_buffer[0]|=0x01; //	�����������������Ϣ��
			}
	}


void Tare_operation(void)
	{	
//		if(ADC_STAB)
//		{
			if(!taring_signal)
			{
				taring_signal=1; buff_tare=(float)AFTER_AD_Result0;
				Feed_signal_buffer[1]|=0x80;	//�����źţ��� ȥƤ 
			}
			else 
			{
				taring_signal=0;buff_tare=0;
				Feed_signal_buffer[1]&=0x7f;
			}
//		}
//		else
//		{
//		 //tare err report   
//		}
	}
void Test_input_edge(void)
  {
	Tare_Late_Status=Input_tare;
	Zero_Late_Status=Input_zero;
 	Start_Late_Status=Input_start;
	Stop_Late_Status=Input_stop;
	Clear_Late_Status=Input_clear_number;
	Input_machine_Pasue_Status=Input_machine_Pasue;
	Input_calibration_Confirm_Status=Input_calibration_Confirm;	 //*2017-10-28am*
	Input_calibration_Status=Input_calibration;
	Input_material_door_Status=Input_material_door;	//*2017-10-28am*
	Input_setting_para_Status=Input_setting_para;
  }


void KLTT_Test_input_edge(void)
  {
		KLTT_Tare_Late_Status=KLTT_tare;
		KLTT_Zero_Late_Status=KLTT_zero;
		Cal_zero_confirm_key_pressed_Late_Status=Cal_zero_confirm_key_pressed;
		Cal_weight_confirm_key_pressed_Late_Status=Cal_weight_confirm_key_pressed;
  }

void KLTT_Cal_operation(void)
{
  int temp__division;

		 //Ϊ�˿���12v������������=2mv/V,�ֶ���=1��ȫ��20000�ֶȣ�ϵ������<47.855.
		 //ѡϵ����С=45
		temp__division=select_division();

	if(~Cal_zero_confirm_key_pressed_Late_Status&Cal_zero_confirm_key_pressed)  //rise edge  //����̩ͨ��λȷ��
		{
			zero_point=AD_Result0; return;
		}
	if(~Cal_weight_confirm_key_pressed_Late_Status&Cal_weight_confirm_key_pressed)  //rise edge  //����̩ͨ����ȷ��	 //port.3
		{

 
            if(!ID_user_cal)//�����û��궨������
			{	//�û�����
				ad_coefficent=((float)(AD_Result0-zero_point))/(Cal_user_weight/temp__division);//�궨�û���������
			}
			else
			{	//��������
				ad_coefficent=((float)(AD_Result0-zero_point))/(Cal_weight/temp__division);	//�궨��������

			}
		
		if(ad_coefficent<45)
    	    {ad_coefficent=1;cal_finished=0;
			 Feed_signal_buffer[0]|=0x02; //	�����궨����������Ϣ��
			 return;}//���غ󲻱���ϵ������λ
	    else
			{ad_coefficent/=temp__division;cal_finished=1;return;}//���غ󱣴�ϵ������λ
		}

}


int select_division(void)	//weight_division
{
	switch (Cal_ID_division)  //�궨�ֶ���������
	{
		case 0:
			    return 1;
		case 1:
			    return 2;
		case 2:
			   return  5;
		case 3:
			    return 10;
		case 4:
			    return 20;
		case 5:
			    return 50;
		default:
			return 1;
	}
}
/****************************************************/

void Teating_weight_zero(void)
{
	if (Modbus_weight.weight_long<zero_range)  //�������� zero_range
		{ //Zero_zone_output;
//		  Un_Pre_fed;		  *2017-10-28am*
			Feed_signal_buffer[1]|=0x40;	//�����źţ��� ����λ��
	    }
	else
	    {   //Un_Zero_zone_output;
			Feed_signal_buffer[1]&=0xbf; 	//�����źţ��� ����λ��		
		}


//����������
/*
12������趨ֵ
  ��ǰ����>Ŀ�궨ֵ-��������ֵʱ����趨ֵ����У�
  ��ǰ����<Ŀ�궨ֵ-�����м�ֵʱ����趨ֵ����ޣ����Ƿ�ѭ��������û��ϵ) 
*/
//if(Modbus_weight.weight_long>Target-Slow_feed)
//{Random_output;}
//else
//{Un_Random_output;}

/*
8�������ź����
�������������ź������, ֹͣ�ź������    
9��ֹͣ�ź����
��ֹͣ��ֹͣ�ź�����У������ź������    
*/

if(state_operation==-1)
{Stop;Un_Start;}
else
{Start;Un_Stop;}

}

void Teating_weight_Random_output(void)
{
		if(state_operation<2)return; //no start
		Delt_temp=Target-Mid_feed; // �����м��� Mid_feed ������ֵ Target	
		if (Modbus_weight.weight_long<Delt_temp)
			{Un_Random_output;}
		Delt_temp=Target-Slow_feed; // ���������� Slow_feed ������ֵ Target
		if (Modbus_weight.weight_long>=Delt_temp)
		    {Random_output;}
}
unsigned int pulse_delay;
void process_feed_material(void)
{ unsigned char *px,i;
//int frezen_AD_DISPLAY_DELAY_TIMER;
//if(q_num<1500)	return;

// 					if (Modbus_weight.weight_long<=zero_range&&state_operation>=1&&!set_shut_initial_mid_slow_prefeed_timer)  //�������� zero_range	 //*2017-10-28am*
//++++++++++++++++++++ 				
//					if (Modbus_weight.weight_long<=zero_range&&!set_shut_initial_mid_slow_prefeed_timer&&state_operation!=-1)  //�������� zero_range	 //*2017-10-28am*
//				
//					{ Clear_output();state_operation=3;}//state_operation=3;}
				
				
				
					 //Un_Initial_output;	}	//*2017-10-28am* //1.�������=0
// 					if (Modbus_weight.weight_long<=zero_range&&state_operation>=1)  //�������� zero_range	 //*2017-10-28am*
//					{Un_Mid_fed;Un_Slow_fed;Un_Pre_fed;Un_Insert_board_output;	}	//*2017-10-28am* //1.������ǰ���� ������=0
					
    if(state_operation==-1){system_indicator=0;return;}
			   if(ADC_STAB)
			   	{
 					if (Modbus_weight.weight_long<=zero_range&&ADC_STAB)  //�������� zero_range	 //*2017-10-28am*
					{Signal_lamp[6]=0x01;}	//*2017-10-28am*
					else	
					{Signal_lamp[6]=0x00;}	   //*2017-10-28am*
				}

  //*********************					


//		   if(set_shut_initial_mid_slow_prefeed_timer)
//		   		{
//					if(!shut_initial_mid_slow_prefeed_timer[0]&&set__initial_mid_slow_prefeed_timer[0])  //*2017-10-28am*	
//					{Un_Initial_output;set__initial_mid_slow_prefeed_timer[0]=0;}//[13]=0
//					if(!shut_initial_mid_slow_prefeed_timer[1]&&set__initial_mid_slow_prefeed_timer[1])  //*2017-10-28am*	
//					{Un_Mid_fed;set__initial_mid_slow_prefeed_timer[1]=0;}//[3]=0
//					if(!shut_initial_mid_slow_prefeed_timer[2]&&set__initial_mid_slow_prefeed_timer[2])  //*2017-10-28am*	
//					{Un_Slow_fed;set__initial_mid_slow_prefeed_timer[2]=0;}//[4]=0
//					if(!shut_initial_mid_slow_prefeed_timer[3]&&set__initial_mid_slow_prefeed_timer[3])  //*2017-10-28am*	
//					{Un_Pre_fed;set__initial_mid_slow_prefeed_timer[3]=0;}//[5]=0
//				}



//	Tare;
//   system_indicator=2;

	switch(state_operation)
	{
	  
//		case 0:
//		        if(main_powered)break;
//
//
//				if (Modbus_weight.weight_long>Initial_power_value) // �������ϵ�ֵ Initial_power_value 
//					{
//						state_operation=1; main_powered=1;
//						Initial_power_output;system_indicator=1; //���ϵ����
//						Delt_temp=Target-Pre_feed; // ������ǰ�� Pre_feed ������ֵ Target
//						if (Modbus_weight.weight_long<Delt_temp) break;
//					    Pre_fed;//��ǰ���
//						state_operation=9; main_powered=1; break;
//					}
//				else
//				    {state_operation=1; main_powered=1;}
//		break;
//							Delt_temp=abs(Target-Pre_feed); // ������ǰ�� Pre_feed ������ֵ Target
//							if (Modbus_weight.weight_long>Delt_temp&&state_operation<=9)  state_operation=10;//*2017-10-28am*
		case 1:
//		   for(i=0;i<9;i++)
//			  {if(i!=6)Signal_lamp[i]=0x00;}//�źŵ�ȫ��	//*2017-10-28am*
			if(ID_run_mode)  //��������ģʽ������ ID_run_mode=1 �Զ� ID_run_mode=0 �ֶ�
			{ if(image_Tare_howmany_package) //�������ٴ�ȥƤһ��Tare_howmany_package
				 {image_Tare_howmany_package--;	state_operation=3;
//				 Write_image_para(XX_image_Tare_howmany_package);
				 }//no tare operation
//					 package_unover=1;

			   if(!image_Tare_howmany_package)//else//�������ٴ�ȥƤһ��=0,ȥƤһ��
			       { if(set_shut_initial_mid_slow_prefeed_timer)
					   	{image_Tare_howmany_package=Tare_howmany_package+1;} //recover number of tare
					  else
					  	{image_Tare_howmany_package=Tare_howmany_package+1;}	
//					Write_image_para(XX_image_Tare_howmany_package);
					 Tare;
					 Tare;
					 system_indicator=2;
					 c_t100=Delay_tare; 
					 state_operation=2;     //��ȥƤ���������ȥƤ��ʱ���	Delay_tare
					 delay_tare_timer=Delay_tare;
					 delay_tare_timer_flag=1;
					  taring_signal=0;//Tare_operation();
//					 if(package_over_no_first)
//					  package_over=1;
//					 else
//					 	package_over_no_first=1;
				   }
			}
			else //manual
			{ state_operation=3;
				 Strong_Filter_reg=Strong_Filter1; //�����˲�ǿ��1	 ����
				 Range_Filter_reg=Range_Filter1; //�����˲���Χ1			
			}//no tare operation
		break;
	
		case 2:  //waitting delay
			if (!c_t100)
			{ Feed_signal_buffer[1]|=0x80;	//�����źţ��� ȥƤ�Ƶ��� ȥƤ����
			   Tare_operation(); //+++++++++
//			   Un_Tare;system_indicator=3;//ȥƤ���OFF
			   state_operation=3;//*2017-10-28am*
//				 Strong_Filter_reg=Strong_Filter1; //�����˲�ǿ��1	 ����
//				 Range_Filter_reg=Range_Filter1; //�����˲���Χ1
						 Strong_Filter_reg=Strong_Filter2; //�����˲�ǿ��2	 �м�  //*2017-10-28am*
						 Range_Filter_reg=Range_Filter2; //�����˲���Χ2
			}
		break;

		case 3:
				Delt_temp=abs(Target-Initial_feed_value); // ���������� Initial_feed_value ������ֵ Target  ����������ʱ Delay_initial_feed
				if (Modbus_weight.weight_long>Delt_temp)
					{  c_t100=Delay_initial_feed;
					   Initial_output;Signal_lamp[2]=0x01;//system_indicator=4;	//1.������� //+++++++++
						state_operation=4;

//  					if(system_indicator==4) //����	
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
//



					}
		break;
		case 4:
				Delt_temp=abs(Target-Initial_feed_value); // ��������ֵ Initial_feed_value ������ֵ Target
//				if (Modbus_weight.weight_long>Delt_temp||!c_t100)
//					    Initial_output;	//1.������� //+++++++++
					    if(set_shut_initial_mid_slow_prefeed_timer&&!set__initial_mid_slow_prefeed_timer[0])
						{shut_initial_mid_slow_prefeed_timer[0]=set_shut_initial_mid_slow_prefeed_timer;
						 set__initial_mid_slow_prefeed_timer[0]=1;}
				if (!c_t100)
					{	
						//*2017-10-28am*
						state_operation=5;//system_indicator=4;
						 Strong_Filter_reg=Strong_Filter2; //�����˲�ǿ��2	 �м�
						 Range_Filter_reg=Range_Filter2; //�����˲���Χ2
					}
		break;
	
		case 5:
				Delt_temp=abs(Target-Mid_feed); // �����м��� Mid_feed ������ֵ Target	 �����м���ʱ Delay_initial_feed
				if (Modbus_weight.weight_long>Delt_temp)
					{ c_t100=Delay_Mid_feed;//�����м���ʱ
					  Mid_fed;Signal_lamp[3]=0x01;//system_indicator=5;//+++++++++
						state_operation=6;
					}
		break;
	
		case 6:
//						 Mid_fed;//+++++++++
					    if(set_shut_initial_mid_slow_prefeed_timer&&!set__initial_mid_slow_prefeed_timer[1])
						{shut_initial_mid_slow_prefeed_timer[1]=set_shut_initial_mid_slow_prefeed_timer;
						set__initial_mid_slow_prefeed_timer[1]=1;}
						//*2017-10-28am*
			if (!c_t100)
				{  state_operation=7; //system_indicator=5;//1.�м����

						 Strong_Filter_reg=Strong_Filter3; //�����˲�ǿ��3	 ����
						 Range_Filter_reg=Range_Filter3; //�����˲���Χ3
				   if (ID_slow_feed_modify) //��������������ǹ�����	  ID_slow_feed_modify
					  auto_t100	=2; //auto_t100Ϊ���̼�ʾ������
				}
		break;
	
		case 7:
				Delt_temp=abs(Target-Slow_feed); // ���������� Slow_feed ������ֵ Target
				if (Modbus_weight.weight_long>Delt_temp)
					{ c_t100=Delay_slow_feed;Signal_lamp[4]=0x01; //1.�������//����������ʱ
					Slow_fed;//+++++++++
						state_operation=8;
					}
		break;
		case 8:
						if (system_indicator!=6) 
						{
	
//						Slow_fed;//+++++++++
						    if(set_shut_initial_mid_slow_prefeed_timer)
							{shut_initial_mid_slow_prefeed_timer[2]=set_shut_initial_mid_slow_prefeed_timer;
							set__initial_mid_slow_prefeed_timer[2]=1;}
					    }
						system_indicator=6; //1.�������
//						Signal_lamp[4]=0x01;
						//*2017-10-28am*

 			if (!c_t100) 
				{  
					 Strong_Filter_reg=Strong_Filter4; //�����˲�ǿ��4	 ��ǰ
					 Range_Filter_reg=Range_Filter4; //�����˲���Χ4
					if (Modbus_weight.weight_long<zero_range)  //�������� zero_range
						{ 
							if(ID_run_mode)  //��������ģʽ������ ID_run_mode=0 �Զ� ID_run_mode=1 �ֶ�
								{ //�Զ�
								 	Clear_output();state_operation=1;	break;
								}
						    else
							   {
							   	  Clear_output(); state_operation=3;	break;
							   }
						}
	
				   else
				        {
							Delt_temp=abs(Target-Pre_feed); // ������ǰ�� Pre_feed ������ֵ Target
							if (Modbus_weight.weight_long>Delt_temp)
								{ 
								   if(ID_slow_feed_modify)//��������������ǹ����� ID_slow_feed_modify
									{	//����̫������
										if (auto_t100-Time_supvisor_Slow_feed_up_limt>=0&&Slow_feed-Slow_feed_modify_down_limt >0) //�������Ӽ�������ʱ��Time_supvisor_Slow_feed_up_limt
											{ //2 ���������������� Slow_feed_modify_down_limt //1 ����������������  Slow_feed_modify_uper_limt	//d �������������� Slow_feed_modify
													  Slow_feed-=Slow_feed_modify; //5 ����������	 Slow_feed
											}
									   else	//����̫������_  //�������Ӽ�������ʱ�� Time_supvisor_Slow_feed_low_limt
									        {
												if (auto_t100-Time_supvisor_Slow_feed_low_limt<0&&Slow_feed_modify_uper_limt-Slow_feed>0) //�������Ӽ�������ʱ��Time_supvisor_Slow_feed_up_limt
													{ //2 ���������������� Slow_feed_modify_down_limt //1 ����������������  Slow_feed_modify_uper_limt
															  Slow_feed+=Slow_feed_modify; //5 ����������	 Slow_feed
													}
											}
									}
									  auto_t100	=0;state_operation=9;
									  c_t100=Delay_Pre_process; //������ǰ��ʱ Delay_Pre_process
								}					
						}
					   
				}
		break;
		case 9:	 //����?  �����ʲ� Dlet_error  mm
//		state_operation=10;	 Pre_fed;system_indicator=7;//��ǰ���
//	 			if (!c_t100&&system_indicator!=7) //������ǰ��ʱ 
	 			if (system_indicator!=7) //������ǰ��ʱ 
					{			//Pre_fed; //*2017-10-28am*
					    if(set_shut_initial_mid_slow_prefeed_timer)
						{shut_initial_mid_slow_prefeed_timer[3]=set_shut_initial_mid_slow_prefeed_timer;
						set__initial_mid_slow_prefeed_timer[3]=1;}
						Pre_fed;  Signal_lamp[5]=0x01;
					 }
						system_indicator=7; //*2017-10-28am*
						//*2017-10-28am*
	 			if (!c_t100) //������ǰ��ʱ 
					{			//Pre_fed; //*2017-10-28am*
								system_indicator=7;
								//state_operation=10;//��ǰ���
								Delt_temp=abs(Target+Dlet_error); // �����ʲ� Dlet_error ������ֵ Target
								if (Modbus_weight.weight_long>=Delt_temp)
									{ //���ϲ�?
									  Feed_signal_buffer[1]|=0x10;	//�����źţ��� ���ϲ����
									  auto_t100	=0;state_operation=10;Over;
									  system_indicator=9; //����
									  Signal_lamp[1]=0x01;	// �̵�
//									  store_test_result_to_stack();
//									  accumulate();
//										get_test_result_form_stack();
//										FIN=1;
										
										//delt_pulse(1,Modbus_weight.weight_long-Delt_temp);
//									  store_test_result_to_stack();
//									  get_test_result_form_stack();
//									  px=status_and_datum();
							
//									  break;
										send_302();
									}
								Delt_temp=abs(Target-Dlet_error); // �����ʲ� Dlet_error ������ֵ Target
								if (Modbus_weight.weight_long<=Delt_temp)
									{ //Ƿ�²�? 
										
									  Feed_signal_buffer[1]|=0x10;	//�����źţ��� Ƿ�����
									  //auto_t100	=0;state_operation=10;LOW_ERR;
									  
										
										system_indicator=1; //Ƿ��
									  Signal_lamp[0]=0x01;	// �̵�
//									  store_test_result_to_stack();
//									  accumulate();
//										store_test_result_to_stack();
//										get_test_result_form_stack();
//										FIN=1; 
										delt_pulse_count=(float)(Delt_temp-Modbus_weight.weight_long)*((float)Modbus_pulse.target_totall_pulse/(float)Target);
										delt_pulse();
										if(no_recive_x_signals>100)no_recive_x_signals=100;
										if(no_recive_x_signals<5)no_recive_x_signals=5;
										pulse_delay=Delay_Pre_process+delt_pulse_count/no_recive_x_signals;
										if(pulse_delay>120)pulse_delay=120;
										c_t100=pulse_delay;
										//c_t100=Delay_Pre_process;
										auto_t100=0;state_operation=9;
//									  break;
									}
								 //�ϸ�
								if (abs(Target+Dlet_error)>Modbus_weight.weight_long && Modbus_weight.weight_long>abs(Target-Dlet_error))
									{ Feed_signal_buffer[1]|=0x20;	//�����źţ��� �ϸ����
									  auto_t100	=0;state_operation=10;Pass;
									  system_indicator=10; //�ϸ�
									  Signal_lamp[7]=0x01;// �̵�

//									  store_test_result_to_stack();
//									  accumulate();
//										store_test_result_to_stack();
//										get_test_result_form_stack();
//										FIN=1;
//									  break;
										
										send_302();
							       }

									  store_test_result_to_stack();
									  accumulate();
									  frezen_AD_DISPLAY_DELAY_TIMER=3;
									  frezen_AD_DISPLAY_DELAY_TIMER_flag=1;
										
										

//					if(system_indicator==10) //�ϸ�	
//					{Signal_lamp[7]=0x01;}	// �̵�
//	
////					if(system_indicator==9) //����	
////					{Signal_lamp[4]=0x02;}	// ���
//
//					if(system_indicator==9) //����	
//					{Signal_lamp[1]=0x01;}	// �̵�
//	
////					if(system_indicator==1) //Ƿ��	
////					{Signal_lamp[5]=0x02;}	// ���
//
//					if(system_indicator==1) //Ƿ��	
//					{Signal_lamp[0]=0x01;}	// �̵�


//									  c_t100=Delay_Next_loop/2;
//									  store_test_result_to_stack();
//									  get_test_result_form_stack();
//									  px=status_and_datum();
					}
		break;
		case 10:	 
//				if (!c_t100) 	  //	if(ADC_STAB)
//				{ 
//						state_operation=11;//system_indicator=8;Signal_lamp[6]=0x01;
//						c_t100=Delay_Next_loop/2;
//				}
                    if(!frezen_AD_DISPLAY_DELAY_TIMER&&frezen_AD_DISPLAY_DELAY_TIMER_flag)    
					{ stop_dis_AD_timer=Recive_x_signals; frezen_AD_DISPLAY_DELAY_TIMER_flag=0;}

					if (Modbus_weight.weight_long<=zero_range)  //�������� zero_range
//					{Clear_output();
					{
						state_operation=11;//system_indicator=8;Signal_lamp[6]=0x01;
						c_t100=Delay_Next_loop; //������һѭ����ʱDelay_Next_loop
					}
		break;
		case 11:
	 			if (!c_t100) 	  //	if(ADC_STAB)
				{ 
					if (Modbus_weight.weight_long<=zero_range)  //�������� zero_range
				    { state_operation=1;  Clear_output();
					 Strong_Filter_reg=Strong_Filter0; //�����˲�ǿ��1	 ����
					 Range_Filter_reg=Range_Filter0; //�����˲���Χ1

		   for(i=0;i<8;i++)
			  {if(i!=6)Signal_lamp[i]=0x00;}//�źŵ�ȫ��	//*2017-10-28am*
//					 Signal_lamp[7]=0x00; Signal_lamp[1]=0x00;Signal_lamp[0]=0x00;
					 }

				}
		break;				
				
					
		default:
			break;
	}

 					if (Modbus_weight.weight_long<=zero_range&&!set_shut_initial_mid_slow_prefeed_timer&&state_operation!=-1&&state_operation<10)  //�������� zero_range	 //*2017-10-28am*
				
					{	 

							Un_Mid_fed              ;//[3]=0
							Un_Slow_fed             ;//[4]=0
							Un_Pre_fed              ;//[5]=0
							Un_Initial_output       ;//[13]=0	   
							Un_Pass                 ;//[1]=0
							Un_Over                 ;//[2]=0
							Un_LOW_ERR              ;//[10]=0

		   for(i=0;i<8;i++)
			  {if(i!=6)Signal_lamp[i]=0x00;}//�źŵ�ȫ��	//*2017-10-28am*
//												 Signal_lamp[7]=0x00; Signal_lamp[1]=0x00;Signal_lamp[0]=0x00;
					 state_operation=3;

					}

 		   if(set_shut_initial_mid_slow_prefeed_timer&&state_operation<11)
		   		{
					if(!shut_initial_mid_slow_prefeed_timer[0]&&set__initial_mid_slow_prefeed_timer[0])  //*2017-10-28am*	
					{Un_Initial_output;
					 Signal_lamp[2]=0x00; set__initial_mid_slow_prefeed_timer[0]=0;}//[13]=0

					if(!shut_initial_mid_slow_prefeed_timer[1]&&set__initial_mid_slow_prefeed_timer[1])  //*2017-10-28am*	
					{Un_Mid_fed; 
					 Signal_lamp[3]=0x00; set__initial_mid_slow_prefeed_timer[1]=0;}//[3]=0

					if(!shut_initial_mid_slow_prefeed_timer[2]&&set__initial_mid_slow_prefeed_timer[2])  //*2017-10-28am*	
					{Un_Slow_fed;
					 Signal_lamp[4]=0x00; set__initial_mid_slow_prefeed_timer[2]=0;}//[4]=0

					if(!shut_initial_mid_slow_prefeed_timer[3]&&set__initial_mid_slow_prefeed_timer[3])  //*2017-10-28am*	
					{Un_Pre_fed;
					 Signal_lamp[5]=0x00;  set__initial_mid_slow_prefeed_timer[3]=0;}//[5]=0
				}
		   if(!delay_tare_timer&&delay_tare_timer_flag)
			  { Un_Tare;system_indicator=3;
				  delay_tare_timer_flag=0;
				  state_operation=1;
			  }//ȥƤ���OFF


}



//�������ٴ��������  Pulse_howmany_package
void Test_Pulse_howmany_package(void)
{
	  if(image_Pulse_howmany_package)
	     {
		        image_Pulse_howmany_package--;
				Write_image_para(XX_image_Pulse_howmany_package);
				
			if(!image_Pulse_howmany_package)
			{
				image_Pulse_howmany_package=Pulse_howmany_package;
				Write_image_para(XX_image_Pulse_howmany_package);
				N_package_output; t100=400;
				//������壬��ʱ�ر�
			}
		} 		
}
void accumulate(void)
{
long weight_temp;

 //������ǰ������ǹ�����	ID_pre_feed_modify
	Test_Pulse_howmany_package();
	if(ID_pre_feed_modify)
	 {
	 	sum+=Modbus_weight.weight_long;
		//store sum
	   //�������ٴ�ƽ������һ����ǰ�� Even_howmany_package_Pre_process
	  if(image_Even_howmany_package_Pre_process)
	     {image_Even_howmany_package_Pre_process--;
		 					Write_image_para(XX_image_Even_howmany_package_Pre_process);
			  if(!image_Even_howmany_package_Pre_process)
			     {
				   image_Even_howmany_package_Pre_process=Even_howmany_package_Pre_process;
				   Weighting_even=sum/Even_howmany_package_Pre_process;
					Write_image_para(XX_image_Even_howmany_package_Pre_process);

//	Target		  Dlet_error
					sum=0;
		          if(Weighting_even-Target>0&&Pre_feed<Pre_feed_modify_uper_limt)//a ������������ Target_down_limt //a ������������ Target_uper_limt                               
					  {Pre_feed++;	sum=0;
					   	Write_image_para(XX_image_Pre_feed);
					  } //����ۼ���	
		          if(Weighting_even-Target<0&&Pre_feed>Pre_feed_modify_down_limt)//a ������������ Target_down_limt //a ������������ Target_uper_limt                               
						{
								Pre_feed--;	sum=0; //����ۼ���
								Write_image_para(XX_image_Pre_feed);
						}




		//        ��ֵ����<ƽ��ֵ<��ֵ����
//		          if(Weighting_even>=Target_down_limt&&Weighting_even<=Target_uper_limt)//a ������������ Target_down_limt //a ������������ Target_uper_limt                               
//		                {sum=0;} //����ۼ���
//					//����������,��ǰ����һ	
////		          if(Weighting_even>Target_uper_limt&&image_Pre_feed<Pre_feed+4)
//		          if(Weighting_even>Target_uper_limt&&Pre_feed<Pre_feed_modify_uper_limt) //*2017-10-28am*
//				  	{// ������ǰ�� Pre_feed
//						Pre_feed++;	sum=0; //����ۼ���	
//						Write_image_para(XX_image_Pre_feed);
//					   //RAM  int Pre_feed_modify_uper_limt;//������ǰ����������
//					} 
//		          //����������,��ǰ����һ		                                   
//		          if(Weighting_even<Target_down_limt&&Pre_feed>Pre_feed_modify_down_limt) //*2017-10-28am*
//				       {	   //RAM  int Pre_feed_modify_down_limt;//������ǰ����������
//								Pre_feed--;	sum=0; //����ۼ���
//								Write_image_para(XX_image_Pre_feed);
//					   }   
					   
					   
					   
					                                  
				 }
		 }
	 }

				if(Cal_point)
					 {	weight_temp=Modbus_weight.weight_long;
						weight_temp+=0.5*pow(10,Cal_point);
						weight_temp/=pow(10,Cal_point);

//						Modbus_weight.weight_long+=0.5*pow(10,Cal_point);
//						 Modbus_weight.weight_long/=pow(10,Cal_point);
					 }
				Weighting_T+=weight_temp;
//				Weighting_T+=Modbus_weight.weight_long;
				Weighting_package++;
				store_ton_package();

}  




void AD_F_process(void)
{
int delt,temp_ad;
unsigned char i;
float  view_AD_Result0,tempx0,tempx1,tempx2;
		  view_AD_Result0=AD_Result0;
		  delt=abs(AD_Result0-AD_Result_buffer);

		  if(delt>Range_Filter_reg*1000) //200
			{
					for(i=0;i<filter_len;i++)
					{
					  AD_QUEEN[i]=AD_Result0;//+zero_point[0];
				    }					      
					AD_Result_buffer=AD_Result0;ADC_STAB=0;Ad_enter_stable_COUNTER=0;
					Feed_signal_buffer[1]&=0xF7; //�ȶ�ָʾ
			}
		  else
		   {		  
					   if (Ad_enter_stable_COUNTER<=1)
							{
								 Ad_enter_stable_COUNTER++;
//		  						 if(Ad_enter_stable_COUNTER>filter_len/2)
//								  {
								   ADC_STAB=1;Feed_signal_buffer[1]|=0x08; //�ȶ�ָʾ
//								  }
//								  else
//							      {
//								       return;
//								  }
			   				}
		  }

		   			 //slide filter
						for(i=0;i<filter_len;i++)
						{AD_QUEEN[filter_len-i]=AD_QUEEN[filter_len-1-i];}
						AD_QUEEN[0]=AD_Result0;

						sum_ad=0;
						for(i=0;i<filter_len;i++)
						{sum_ad+=AD_QUEEN[i];}
//						 tempx2=(float)sum_ad;
						 sum_ad/=filter_len;

//						  tempx0=(float)Strong_Filter_reg/100;
//						  tempx1=(1-tempx0)*(float)AD_Result_buffer+tempx0*(float)sum_ad;

//						  tempx0=(float)(1-exp(-(double)delt))*(float)Strong_Filter_reg/100;
//						  tempx1=(1-tempx0)*(float)AD_Result_buffer+tempx0*(float)sum_ad;


						  tempx0=pow(2,-Strong_Filter_reg);
						  tempx1=(1-tempx0)*AD_Result_buffer+tempx0*sum_ad;

						  AD_Result_buffer=(long)tempx1;
						  AD_Result0=AD_Result_buffer;

//		  }

					 return;


		  if(delt>Range_Filter_reg) //200
			{
					for(i=0;i<Strong_Filter_reg;i++)
					{
					  AD_QUEEN[i]=AD_Result0;//+zero_point[0];
				    }					      
					AD_Result_buffer=AD_Result0;ADC_STAB=0;Ad_enter_stable_COUNTER=0;
					Feed_signal_buffer[1]&=0xF7; //�ȶ�ָʾ
			}
		  else
		   {
						if(QUEEN>=Strong_Filter_reg)QUEEN=0;
						AD_QUEEN[QUEEN++]=AD_Result0;
						sum_ad=0;
						for(i=0;i<Strong_Filter_reg;i++)
						{sum_ad+=AD_QUEEN[i];}
						 sum_ad/=Strong_Filter_reg;
					     AD_Result_buffer=sum_ad;
						 AD_Result0=sum_ad;
						 Ad_enter_stable_COUNTER++;
						 if(Ad_enter_stable_COUNTER>Strong_Filter_reg/2)
						 {
						   ADC_STAB=1;Feed_signal_buffer[1]|=0x08; //�ȶ�ָʾ
						 }
						 
		  }
}
 
void Clear_output(void)
{
	if(!set_shut_initial_mid_slow_prefeed_timer)   //*2017-10-28am*
	   {
				Un_Mid_fed              ;//[3]=0
				Un_Slow_fed             ;//[4]=0
				Un_Pre_fed              ;//[5]=0
				Un_Initial_output       ;//[13]=0	   
	   
	   }
//				Un_Tare                 ;//[0]=0
				Un_Pass                 ;//[1]=0
				Un_Over                 ;//[2]=0
//				Un_Mid_fed              ;//[3]=0
//				Un_Slow_fed             ;//[4]=0
//				Un_Pre_fed              ;//[5]=0
//				Un_Start                ;//[6]=0
				Un_Stop                 ;//[7]=0
				Un_N_package_output     ;//[8]=0
//				Un_Zero_zone_output     ;//[9]=0   *2017-10-28am*
				Un_LOW_ERR              ;//[10]=0
				Un_Random_output        ;//[11]=0
				Un_Cal_output           ;//[12]=0
//				Un_Initial_output       ;//[13]=0
//��ָʾ��
				Feed_signal_buffer[1]&=~(0x80);//ȥƤ
				Feed_signal_buffer[1]&=~(0x20);//�ϸ�
				Feed_signal_buffer[1]&=~(0x10);//����
				system_indicator=0;//�ȴ�
}
void store_test_result_to_stack(void)
{
	if(stack_head>=stack_top)stack_head=0;
	AD_result_stack[stack_head]=Modbus_weight.weight_long;
	stack_head++;

//1.ѹ����head����
}

char  *get_test_result_form_stack(void)
{
unsigned char i;
//long temp_ad;
//	 temp_ad= AD_result_stack[0];
     memset(ADresult,10);
	 if(!FIN)
	 {
		for(i=0;i<8;i++)
		{
			ADresult[i]='F';	 
	    }
		return ADresult;
	 }
	 
	//1.�ײ������
	_ltoa( ADresult,AD_result_stack[0]);
	if(!stack_head)	
		{return ADresult;}
	//shift down
	for(i=0;i<stack_head;i++)
	{
		AD_result_stack[i]=AD_result_stack[i+1];
	}
	stack_head--;
	if(stack_head>=stack_top)stack_head=0;
	return ADresult;
}

void PC_comm3_command(unsigned char Data)
{
  unsigned char  xStart[]={"YStartXR"};
   unsigned char  xStop[]={"YStopXR"};
//  unsigned char  xTarget[]={"YTargetX"};
//  unsigned char  xPackage[]={"YPackageX"};
//  unsigned char  xTotal[]={"YTotalX"};
  unsigned char  xClean[]={"YCleanXR"};

   
 		switch (comm3_Modbus_Counter)
		{
			case 0:
				if(Data=='X')//slave
				{
					comm3_buffer_point=comm3_buffer;
					memset(comm3_buffer,0,10);
					comm3_Modbus_Counter++;
				}
			break;

// 			case 0:
//			break;
 			case 1:
			        Data_Addr=Data;
					comm3_buffer_point=comm3_buffer;
					comm3_Modbus_Counter++;

			break;

 			case 2:
			    Data_Addr&=0x0f;
			    Data_Addr*= 10;
				Data_Addr+=(Data&0x0f); 
				if(Data_Addr==Comm_Addr)//slave
				{
					comm3_buffer_point=comm3_buffer;
					memset(comm3_buffer,0,10);
					comm3_Modbus_Counter++;
				}
				else
				{
				 	memset(comm3_buffer,0,10);
					comm3_Modbus_Counter=0;
					return;
				}

			break;


			default:
//						   Modbus_Data[0]=0;
//					       Modbus_function=0;//if CRC ok 
//						   comm3_Modbus_Counter=0;

				if(Data!='Y')//slave
				 {
				  *comm3_buffer_point=Data;
				   comm3_buffer_point++;
   				  comm3_Modbus_Counter++;
				  if(comm3_Modbus_Counter>10)
				  {
				   	comm3_Modbus_Counter=0;
				  }
				 }
				 else if(Data=='Y')//slave
				 { 
				   if(strcmp(comm3_buffer,"Start")==0)
				   {
					    if(state_operation==Machine_Pause)
						{state_operation=reservestate_operation;return;}   //*2017-10-28am*
	 					if (Modbus_weight.weight_long<=zero_range)  //�������� zero_range	 //*2017-10-28am*
						{state_operation=1;reservestate_operation=0;image_Tare_howmany_package=Tare_howmany_package;}//main_powered;
						 return_data(xStart);
						return;	
				   }
				   if(strcmp(comm3_buffer,"Stop")==0)
				   {
//						state_operation=-1;
						Un_Tare                 ;//[0]=0
						Un_Pass                 ;//[1]=0
						Un_Over                 ;//[2]=0
						Un_Mid_fed              ;//[3]=0
						Un_Slow_fed             ;//[4]=0
						Un_Pre_fed              ;//[5]=0
						Un_Start                ;//[6]=0
						Un_Stop                 ;//[7]=0
						Un_N_package_output     ;//[8]=0
						Un_Material_door_output ;//[9]=0     
		//				Un_Zero_zone_output     ;//[9]=0
						Un_LOW_ERR              ;//[10]=0
						Un_Random_output        ;//[11]=0
						Un_Cal_output           ;//[12]=0
						Un_Initial_output       ;//[13]=0
						Un_Insert_board_output  ;//[14]=0
						Un_Clamp_output         ;//[15]=0
		
						system_indicator=0;
						taring_signal=1;Tare_operation();
						return_data(xStop);	
				   		return;
				   }
				   if(strcmp(comm3_buffer,"Target")==0)
				   {
						_ltoa(Dis_adxx,(long)Target);
						return_data_xx(Dis_adxx);
						return;	
				   }
				   if(strcmp(comm3_buffer,"Package")==0)
				   {
						_ltoa(Dis_adxx,(long)Weighting_package);
						return_data_xx(Dis_adxx);
						return;	
				   }
				   if(strcmp(comm3_buffer,"Total")==0)
				   {
				   	  _ltoa(Dis_adxx,(long)Weighting_T);
					  	return_data_xx(Dis_adxx);
						return;	
				   }
				   if(strcmp(comm3_buffer,"Clean")==0)
				   {
						Weighting_T=0;Weighting_package=0;
						 store_ton_package();
							//��ʼ������
							image_Tare_howmany_package=Tare_howmany_package;
							image_Even_howmany_package_Pre_process=Even_howmany_package_Pre_process;
							image_Pulse_howmany_package=Pulse_howmany_package;
							image_Pre_feed=Pre_feed;
				
							Write_image_para(XX_image_Pulse_howmany_package);
							Write_image_para(XX_image_Tare_howmany_package);
							Write_image_para(XX_image_Even_howmany_package_Pre_process);
							Write_image_para(XX_image_Pre_feed);
						    return_data(xClean);
							return;				   }
				   if(strcmp(comm3_buffer,"Weight")==0)	 
				   {	get_test_result_form_stack();
				        return_data_xx(ADresult);
						return;	
				   }

				   if(strcmp(comm3_buffer,"Point")==0)
				   {
				   	  _ltoa(Dis_adxx,(long)Cal_point);
					  	return_data_xx(Dis_adxx);
						return;	
				   }

				 }

	   break;
  }
				

}




void delt_pulse()
{
		int px;
		int i;;

	
		delt_pulse_string_modbus[0]=0x0b;
		delt_pulse_string_modbus[1]=0x06;
		delt_pulse_string_modbus[2]=0x00;
		delt_pulse_string_modbus[3]=0x17;

		delt_pulse_string_modbus[4]=delt_pulse_count>>8&0x0ff;
		delt_pulse_string_modbus[5]=delt_pulse_count&0x0ff;
		px=mtb_crc_calc((unsigned char *)delt_pulse_string_modbus,6);
		delt_pulse_string_modbus[6]=px&0x0ff;					             
		delt_pulse_string_modbus[7]=(px>>8)&0x0ff;	
	

	//	Delay((u32)100*delt_pulse_count);
		FIN=1;
}
void send_302()
{
		unsigned int px;
		unsigned long int weigth;
	
		F302_Fram[0]=Comm_Addr;
		F302_Fram[1]=0x03;
		F302_Fram[2]=0x04;
		
		//F302_Fram[3]=Modbus_weight.long_weight[0];
		//F302_Fram[4]=Modbus_weight.long_weight[1];;
		//F302_Fram[5]=Modbus_weight.long_weight[2];
		//F302_Fram[6]=Modbus_weight.long_weight[3];
	
	
		weigth=(unsigned long int)Modbus_weight.weight_long;
		weigth*=pow(10,4-Cal_point);
	
		F302_Fram[3]=(weigth>>24)&0x0ff;
		F302_Fram[4]=(weigth>>16)&0x0ff;
		F302_Fram[5]=(weigth>>8)&0x0ff;
		F302_Fram[6]=weigth&0x0ff;
	
		px=mtb_crc_calc((unsigned char *)delt_pulse_string_modbus,7);
		F302_Fram[7]=px&0x0ff;					             
		F302_Fram[8]=(px>>8)&0x0ff;	
	

	//	Delay((u32)100*delt_pulse_count);
		F302=1;
}

// if (FIN)
// 	{
//	 for(i=0;i<12;i++)
//		{	USART_SendData(UART4,sig_pulse_string_modbus[i]);
//			while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
//		}
//
//	 for(i=0;i<12;i++)
//		{	USART_SendData(UART4,delt_pulse_string_modbus[i]);
//			while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
//		}
//	   FIN=0;
//	}

 
//_

//		if(KEYN<=30&&KEYN>=15){KEYN--;return;}//KEYN=30 after key processing
//		else
//		     {
//			   if(!KEYH){ KEYN=0;OLDK=0;KEYH=0;return;}//release
//			   else	return;//����
//			 } 
//
//        if(!KEYH)return;//key empty
//		if(KEYH&&!KEYN)
//		  {OLDK=KEYH;KEYN++;return;}//test first pressed
//		if(KEYH&&KEYN)
//		 {
//	        if(KEYH!=OLDK)
//				{ KEYN=0;OLDK=0;KEYH=0;return;}//��������������
//			else
//			  { 
//				   if(KEYN<9)
//				   	   { KEYN++; return;}
//				    else//when KEYN=9,watting for key processing
//					   {return;} //KEYN=30 after key processing
//			  }
//	    }




