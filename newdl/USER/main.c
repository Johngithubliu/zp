
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


/*²¦ÅÌÊäÈë**/
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

extern int Cal_point;//±ê¶¨Ğ¡Êıµã
int err_setting_Slow_feed_timer;

int temp__division;
extern int Initial_zero_range;//²ÎÊı¿ª»úÖÃÁã·¶Î§
extern int Munal_zero_range;
extern int Cal_full_Mig;//±ê¶¨ÂúÁ¿³ÌÖµ
extern int Trace_zero_range;//²ÎÊıÁã¸ú×Ù·¶Î§
extern char Feed_signal_buffer[2];//·´À¡ĞÅºÅ£¬Èç È¥Æ¤ ÖÃÁã

extern char cal_finished;
extern float zero_zone;
extern char Cal_been_opened;
extern char Cal_zero_confirm_key_pressed,Cal_weight_confirm_key_pressed;
extern void KLTT_Cal_operation(void);
extern int select_division(void);	//weight_division
extern int Range_Filter_reg,Strong_Filter_reg;
extern int Strong_Filter0;//²ÎÊıÂË²¨Ç¿¶È0
extern int Range_Filter0;//²ÎÊıÂË²¨·¶Î§0
extern int state_operation,reservestate_operation;
extern char ADC_STAB;
extern unsigned long int Weighting_T,Weighting_package,Weighting_even;//³ÆÖØ_¶ÖÎ»£¬´üÊı£¬Æ½¾ùÖµ
extern float Cal_user_weight;//±ê¶¨ÓÃ»§íÀÂëÖØÁ¿

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
int seq_num;//Ğ¡ÆÁÄ»Éè¶¨/±ê¶¨µÄ²½ÖèĞòºÅ
void dis_para00(int seq_num,int para,char _image_ponter);
void dis_para02(int seq_num,int para,char _image_ponter);
void write_para01(int para,char _image_ponter);
void dis_para01(unsigned char seq_num,int para,char _image_ponter);

extern char Modbus_0x10_Value[30];
/*
 * º¯ÊıÃû£ºmain
 * ÃèÊö  £ºÖ÷º¯Êı
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ
 */

//1.Í¨Ñ¶Êı¾İ×é ÔÚÀ¥ÂØÍ¨Ì©ÖĞµØÖ·(Ò»¸öµØÖ·16Î»)
#define comm_bud_rate 1508-1// 1008-1  //²ÎÊı²¨ÌØÂÊ¹ØÁªºÅ
#define comm_address comm_bud_rate+1 //²ÎÊıÍ¨Ñ¶µØÖ·¹ØÁªºÅ
//2.ÖÃÁãÊı¾İ×é
#define Initial_zero_range_address comm_address+1  //²ÎÊı¿ª»úÖÃÁã·¶Î§
#define Munal_zero_range_address Initial_zero_range_address+1  //²ÎÊıÊÖ¶¯ÖÃÁã·¶Î§
#define Trace_zero_range_address Munal_zero_range_address+1  //²ÎÊıÁã¸ú×Ù·¶Î§
//3.ÑÓÊ±Êı¾İ×é
#define  Delay_tare_address  Trace_zero_range_address+1//²ÎÊıÈ¥Æ¤ÑÓÊ±Êä³ö
#define  Delay_Mid_feed_address  Delay_tare_address+1//²ÎÊıÖĞ¼ÓÑÓÊ±
#define  Delay_Pre_process_address  Delay_Mid_feed_address+1//²ÎÊıÌáÇ°ÑÓÊ±
#define  Delay_Next_loop_address  Delay_Pre_process_address+1//²ÎÊıÏÂÒ»Ñ­»·ÑÓÊ±
#define  Time_supvisor_Slow_feed_up_limt_address  Delay_Next_loop_address+1//²ÎÊıÂı¼Ó¼àÊÓÉÏÏŞÊ±¼ä
#define  Time_supvisor_Slow_feed_low_limt_address  Time_supvisor_Slow_feed_up_limt_address+1//²ÎÊıÂı¼Ó¼àÊÓÏÂÏŞÊ±¼ä
#define  Pulse_howmany_package_address  Time_supvisor_Slow_feed_low_limt_address+1//²ÎÊı¶àÉÙ´üÊä³öÂö³å
#define  Tare_howmany_package_address  Pulse_howmany_package_address+1//²ÎÊı¶àÉÙ´üÈ¥Æ¤Ò»´Î
#define  Recive_x_signals_address  Tare_howmany_package_address+1//²ÎÊıÊÕµ½x´ÎĞÅºÅÉè¶¨
#define  no_recive_x_signals_address  Recive_x_signals_address+1//²ÎÊıÊÕ²»µ½x´ÎĞÅºÅÉè¶¨ÏŞ
#define  Even_howmany_package_Pre_process_address  no_recive_x_signals_address+1//²ÎÊı¶àÉÙ´üÆ½¾ùĞŞÕıÒ»´ÎÌáÇ°Á¿
#define  Delay_slow_feed_address  Even_howmany_package_Pre_process_address+1//²ÎÊıÂı¼ÓÑÓÊ±
#define  Delay_initial_feed_address  Delay_slow_feed_address+1//²ÎÊı³õ¼ÓÑÓÊ±

//4.¿ØÖÆÊı¾İ×é   
#define   Slow_feed_modify_uper_limt_address  Delay_initial_feed_address +1//  ²ÎÊıÂı¼ÓĞŞÕıÉÏÏŞ
#define   Slow_feed_modify_down_limt_address   Slow_feed_modify_uper_limt_address +1//  ²ÎÊıÂı¼ÓĞŞÕıÏÂÏŞ
#define   Pre_feed_modify_uper_limt_address   Slow_feed_modify_down_limt_address +1//  ²ÎÊıÌáÇ°Á¿ĞŞÕıÉÏÏŞ
#define   Pre_feed_modify_down_limt_address   Pre_feed_modify_uper_limt_address +1//  ²ÎÊıÌáÇ°Á¿ĞŞÕıÏÂÏŞ
#define   Slow_feed_address   Pre_feed_modify_down_limt_address +1//  ²ÎÊıÂı¼ÓÁ¿
#define   Pre_feed_address   Slow_feed_address +1//  ²ÎÊıÌáÇ°Á¿
#define   Mid_feed_address   Pre_feed_address +1//  ²ÎÊıÖĞ¼ÓÖµ
#define   Dlet_error_address   Mid_feed_address +1//  ²ÎÊıÔÊ²î
 //*2017-10-28am*
#define   Target_uper_limt_address   Dlet_error_address +1  //  ²ÎÊı¶¨Á¿ÉÏÏŞ
#define   Target_down_limt_address   Target_uper_limt_address +1//  ²ÎÊı¶¨Á¿ÏÂÏŞ
#define   Target_address             Target_down_limt_address+1//  ²ÎÊı¶¨Öµ
#define   zero_range_address         Target_address +1//  ²ÎÊıÁãÇø
 //*2017-10-28am*
#define   Slow_feed_modify_address   zero_range_address +1//  ²ÎÊıÂı¼ÓĞŞÕıÁ¿

#define   Initial_power_value_address   Slow_feed_modify_address +1//  ²ÎÊı³õÉÏµçÖµ
#define   Initial_feed_value_address   Initial_power_value_address +1//  ²ÎÊı³õ¼ÓÁ¿
#define   Random_setting_value_address   Initial_feed_value_address +1//  ²ÎÊıËæ»úÉè¶¨Öµ



//5.ĞŞÕıÊı¾İ×é
#define   ID_user_cal_address   Random_setting_value_address +1//  ²ÎÊıÓÃ»§±ê¶¨¹ØÁªºÅ
#define   ID_slow_feed_modify_address   ID_user_cal_address +1//  ²ÎÊıÂı¼ÓĞŞÕı±ê¼Ç¹ØÁªºÅ
#define   ID_pre_feed_modify_address   ID_slow_feed_modify_address +1//  ²ÎÊıÌáÇ°ĞŞÕı±ê¼Ç¹ØÁªºÅ
#define   ID_run_mode_address   ID_pre_feed_modify_address +1//  ²ÎÊıÔËĞĞÄ£Ê½¹ØÁªºÅ

//7.±ê¶¨Êı¾İ×é
#define Cal_ID_division_address ID_run_mode_address+1//±ê¶¨·Ö¶ÈÊı¹ØÁªºÅ
#define Cal_user_weight_address Cal_ID_division_address+1//±ê¶¨ÓÃ»§íÀÂëÖØÁ¿
#define Cal_weight_address Cal_user_weight_address+2//±ê¶¨íÀÂëÖØÁ¿

//8.ÂË²¨Êı¾İ×é£º
#define Range_Filter0_address Cal_weight_address+2    //²ÎÊıÂË²¨·¶Î§0
#define Range_Filter1_address Range_Filter0_address+1//²ÎÊıÂË²¨·¶Î§1
#define Range_Filter2_address Range_Filter1_address+1//²ÎÊıÂË²¨·¶Î§2
#define Range_Filter3_address Range_Filter2_address+1//²ÎÊıÂË²¨·¶Î§3
#define Range_Filter4_address Range_Filter3_address+1//²ÎÊıÂË²¨·¶Î§4

#define Strong_Filter0_address Range_Filter4_address+1//²ÎÊıÂË²¨Ç¿¶È0
#define Strong_Filter1_address Strong_Filter0_address+1//²ÎÊıÂË²¨Ç¿¶È1
#define Strong_Filter2_address Strong_Filter1_address+1//²ÎÊıÂË²¨Ç¿¶È2
#define Strong_Filter3_address Strong_Filter2_address+1//²ÎÊıÂË²¨Ç¿¶È3
#define Strong_Filter4_address Strong_Filter3_address+1//²ÎÊıÂË²¨Ç¿¶È4

#define Cal_full_Mig_address  Strong_Filter4_address+1// 1561-1//ÂúÁ¿³ÌÖµ
#define Cal_pint_address  Cal_full_Mig_address+1// 1562//±ê¶¨Ğ¡Êıµã
#define Analog_mode_SEL_address    Cal_pint_address+1//Ä£ÄâÁ¿Êä³öÑ¡Ôñ



/*************************************************/
//1.Í¨Ñ¶Êı¾İ×é
extern int ID_budrate;//²ÎÊı²¨ÌØÂÊ¹ØÁªºÅ
extern int Comm_Addr;//²ÎÊıÍ¨Ñ¶µØÖ·¹ØÁªºÅ
//2.ÖÃÁãÊı¾İ×é
extern int Initial_zero_range;//²ÎÊı¿ª»úÖÃÁã·¶Î§
extern int Munal_zero_range;//²ÎÊıÊÖ¶¯ÖÃÁã·¶Î§
extern int Trace_zero_range;//²ÎÊıÁã¸ú×Ù·¶Î§
//3.ÑÓÊ±Êı¾İ×é
extern int Delay_tare;//²ÎÊıÈ¥Æ¤ÑÓÊ±Êä³ö
extern int Delay_Mid_feed;//²ÎÊıÖĞ¼ÓÑÓÊ±
extern int Delay_Pre_process;//²ÎÊıÌáÇ°ÑÓÊ±
extern int Delay_Next_loop;//²ÎÊıÏÂÒ»Ñ­»·ÑÓÊ±
extern int Time_supvisor_Slow_feed_up_limt;//²ÎÊıÂı¼Ó¼àÊÓÉÏÏŞÊ±¼ä
extern int Time_supvisor_Slow_feed_low_limt;//²ÎÊıÂı¼Ó¼àÊÓÏÂÏŞÊ±¼ä
extern int Pulse_howmany_package;//²ÎÊı¶àÉÙ´üÊä³öÂö³å
extern int Tare_howmany_package;//²ÎÊı¶àÉÙ´üÈ¥Æ¤Ò»´Î
extern int Recive_x_signals;//²ÎÊıÊÕµ½x´ÎĞÅºÅÉè¶¨
extern int no_recive_x_signals;//²ÎÊıÊÕ²»µ½x´ÎĞÅºÅÉè¶¨ÏŞ
extern int Even_howmany_package_Pre_process;//²ÎÊı¶àÉÙ´üÆ½¾ùĞŞÕıÒ»´ÎÌáÇ°Á¿
extern int Delay_slow_feed;//²ÎÊıÂı¼ÓÑÓÊ±
extern int Delay_initial_feed;//²ÎÊı³õ¼ÓÑÓÊ±

//4.¿ØÖÆÊı¾İ×é   
extern int Slow_feed_modify_uper_limt;//²ÎÊıÂı¼ÓĞŞÕıÉÏÏŞ
extern int Slow_feed_modify_down_limt;//²ÎÊıÂı¼ÓĞŞÕıÏÂÏŞ
extern int Pre_feed_modify_uper_limt;//²ÎÊıÌáÇ°Á¿ĞŞÕıÉÏÏŞ
extern int Pre_feed_modify_down_limt;//²ÎÊıÌáÇ°Á¿ĞŞÕıÏÂÏŞ
extern int Slow_feed;//²ÎÊıÂı¼ÓÁ¿
extern int Pre_feed;//²ÎÊıÌáÇ°Á¿
extern int Mid_feed;//²ÎÊıÖĞ¼ÓÖµ
extern int Dlet_error;//²ÎÊıÔÊ²î
extern int Target;//²ÎÊı¶¨Öµ
extern int Target_uper_limt;//²ÎÊı¶¨Á¿ÉÏÏŞ
extern int Target_down_limt;//²ÎÊı¶¨Á¿ÏÂÏŞ
extern int zero_range;//²ÎÊıÁãÇø
extern int Slow_feed_modify;//²ÎÊıÂı¼ÓĞŞÕıÁ¿
extern int Initial_power_value;//²ÎÊı³õÉÏµçÖµ
extern int Initial_feed_value;//²ÎÊı³õ¼ÓÁ¿
extern int Random_setting_value;//²ÎÊıËæ»úÉè¶¨Öµ

//5.ĞŞÕıÊı¾İ×é
extern int ID_user_cal ;//²ÎÊıÓÃ»§±ê¶¨¹ØÁªºÅ
extern int ID_slow_feed_modify ;//²ÎÊıÂı¼ÓĞŞÕı±ê¼Ç¹ØÁªºÅ
extern int ID_pre_feed_modify;//²ÎÊıÌáÇ°ĞŞÕı±ê¼Ç¹ØÁªºÅ
extern int ID_run_mode;//²ÎÊıÔËĞĞÄ£Ê½¹ØÁªºÅ



//6.ÔÓÏîÊı¾İ×é
extern int Clear_merory_code;//ÄÚ´æÇåÀíÃÜÂë
extern int Target_pulse;//²ÎÊı¶¨Á¿µÄ×ÜÂö³åÊı
extern int ID_weight_unit;//²ÎÊıÖØÁ¿µ¥Î»Éè¶¨¹ØÁªºÅ
extern int Num_shift;//²ÎÊı°à´Î
//7.extern int ;//±ê¶¨Êı¾İ×é
extern int Cal_code;//±ê¶¨ÃÜÂë
extern int Cal_point ;//±ê¶¨Ğ¡Êıµã
extern int Cal_ID_division;//±ê¶¨·Ö¶ÈÊı¹ØÁªºÅ
extern int Cal_full_Mig;//±ê¶¨ÂúÁ¿³ÌÖµ
extern float Cal_user_weight;//±ê¶¨ÓÃ»§íÀÂëÖØÁ¿
extern int Cal_zero;//±ê¶¨ÁãÎ»
extern int Cal_ratio;//±ê¶¨±êÂÊ
extern float Cal_weight;	//±ê¶¨íÀÂëÖØÁ¿

//8.ÂË²¨Êı¾İ×é£º
extern int Range_Filter0;//²ÎÊıÂË²¨·¶Î§0
extern int Range_Filter1;//²ÎÊıÂË²¨·¶Î§1
extern int Range_Filter2;//²ÎÊıÂË²¨·¶Î§2
extern int Range_Filter3;//²ÎÊıÂË²¨·¶Î§3
extern int Range_Filter4;//²ÎÊıÂË²¨·¶Î§4
extern int Strong_Filter0;//²ÎÊıÂË²¨Ç¿¶È0
extern int Strong_Filter1;//²ÎÊıÂË²¨Ç¿¶È1
extern int Strong_Filter2;//²ÎÊıÂË²¨Ç¿¶È2
extern int Strong_Filter3;//²ÎÊıÂË²¨Ç¿¶È3
extern int Strong_Filter4;//²ÎÊıÂË²¨Ç¿¶È4

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
char revers;//Ç°½øĞ´²ÎÊıÓĞĞ§
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
			EXTI_NVIC_Configuration(); //Æô¶¯EXTI ¶ÁAD
//			EXTI_Configuration();
	initi_usart2_control_pin();




			//¶ÁAD ²ÎÊı
			STMFLASH_Read (AD_ZEROX,(u16*)AD_ZERO_STORE.zero_pointx,4);
			zero_point=AD_ZERO_STORE.zero_point;	
			STMFLASH_Read (AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);
			ad_coefficent=AD_COFF_STORE.ad_coefficent;
			//¶Á¶ÖÎ»£¬´üÊı 
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
 
 if( !RCC_GetFlagStatus(RCC_FLAG_SFTRST))     //ÉÏµç¸´Î»flag=0, Èí¼ş¸´Î»flag=1
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

/**************¿ØÖÆ´°¿Ú4-20ma³õÊ¼»¯************************************/
test_4_20ma_port();
#define ANALOG_4_20mA               ( 4020 )
#define ANALOG_0_5V 5
AD5422Init ();		  
//init_TM1638(); //³õÊ¼»¯TM1638
flash_ponter=flash_ponter_image=3;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
//combinate_letter_digtal(OUQ,digtal_x);
flash_ponter=3;flashing=1;	 original=LED_display_buffer;
/*************************************************************************/

   flashing=0; 

set_shut_initial_mid_slow_prefeed_timer=ID_user_cal;  //*2017-10-28am*	set_shut_initial_mid_slow_prefeed_timer  ½èÓÃ	   ID_user_cal
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
if(state_operation<3)//Õû»ú ÂË²¨Ç¿¶È ÂË²¨·¶Î§
	   {
				 Strong_Filter_reg=Strong_Filter0; //²ÎÊıÂË²¨Ç¿¶È0
				 Range_Filter_reg=Range_Filter0; //²ÎÊıÂË²¨·¶Î§0
       }

	 if(!Initial_zero_flag&&ADC_STAB)//Initial_zero_range;//²ÎÊı¿ª»úÖÃÁã·¶Î§
	   {
		   if(((float)AFTER_AD_Result0/pow(10,Cal_point))<(((float)Initial_zero_range/100)*(float)Cal_full_Mig))
		   		{
					  zero_point=AD_Result0; 
					  Feed_signal_buffer[1]|=0x40;	//·´À¡ĞÅºÅ£¬Èç ÖÃÁãµÆ
					  Initial_zero_flag=1;
					//	printf("z_p=%ld,Cal_full_Mig=%f\r\n",zero_point,Cal_full_Mig);
	   			}
				//	else printf("no zero\r\n");
					
	   }

if(!clearing&&!calibing &&!programm_setting)	   //Éè¶¨£¬±ê¶¨£¬Çå³ı ²»¸ú×Ù
   {
	 if(Trace_zero_range&&AFTER_AD_Result0&&!buff_tare&&ADC_STAB)//²ÎÊıÁã¸ú×Ù·¶Î§Trace_zero_range;	ÎŞÆ¤£¬ÎÈ¶¨
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
							  	Feed_signal_buffer[1]|=0x40;	//·´À¡ĞÅºÅ£¬Èç ÖÃÁãµÆ
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
			
		//ÎªmodbusµÄÖØÁ¿Öµ(LONG) ½øĞĞmodbusÍ¨Ñ¶
			Modbus_weight.weight_long=(float)AFTER_AD_Result0-buff_tare;//ÖÃÁã,È¥Æ¤
			//·Ö¶ÈÊı
			temp__division=select_division();
			Modbus_weight.weight_long/=temp__division;
			Modbus_weight.weight_long=(int)Modbus_weight.weight_long*temp__division;
		
			_ltoa(Dis_ad,Modbus_weight.weight_long);	 //¸ºÊıÏÔÊ¾£¬¼Ó¸ºÅ
	}
  else
  	{
			_ltoa(Dis_ad,(float)AD_Result0);	 //ÏÔÊ¾AD_CODE
	
	}


	  
	if (AFTER_AD_Result0 < 0)
		AFTER_AD_Result0 = -AFTER_AD_Result0; //<0,È¡¾ø¶ÔÖµ
	Weight_value=(int)AFTER_AD_Result0; //ÖØÁ¿Öµ

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
				STMFLASH_Write(FL_Weighting_T,(u16*)&Weighting_T,4);//±£´æ¶ÖÎ»¡¤
				STMFLASH_Read (FL_Weighting_T,(u16*)&Weighting_T,4); //
				
				//printf("save Weighting_T=%ld\r\n",Weighting_T);
	
				STMFLASH_Write(FL_Weighting_package,(u16*)&Weighting_package,4);//±£´æ´üÊı¡¤
				STMFLASH_Read (FL_Weighting_package,(u16*)&Weighting_package,4);
}
void   ANALOG_0_5V_00(void)
{
//ANALOG_0_5V
	if(Modbus_weight.weight_long/pow(10,Cal_point)<=Cal_full_Mig&&Modbus_weight.weight_long>=0)
	{
		WriteControlRegisterToAD5422 (ANALOG_0_5V);
		WriteDataRegisterToAD5422 ((Modbus_weight.weight_long/pow(10,Cal_point)/Cal_full_Mig)*65534);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
	}
	else if(Modbus_weight.weight_long<0)
	{
		WriteControlRegisterToAD5422 (ANALOG_0_5V);
		WriteDataRegisterToAD5422 (0x0001);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
	}
	else if(Modbus_weight.weight_long/pow(10,Cal_point)>Cal_full_Mig)
	{
		WriteControlRegisterToAD5422 (ANALOG_0_5V);
		WriteDataRegisterToAD5422 (65534);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
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
		WriteDataRegisterToAD5422 (real_weight*65534);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
	}
	else if(Modbus_weight.weight_long<0)
	{
		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
		WriteDataRegisterToAD5422 (0x0001);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
	}
	else if(Modbus_weight.weight_long/pow(10,Cal_point)>Cal_full_Mig)
	{
		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
		WriteDataRegisterToAD5422 (65534);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
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
//		WriteDataRegisterToAD5422 ((Modbus_weight.weight_long/Cal_full_Mig)*65534);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
//	}
//	else if(Modbus_weight.weight_long<0)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_0_5V);
//		WriteDataRegisterToAD5422 (0x0001);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
//	}
//	else if(Modbus_weight.weight_long>Cal_full_Mig)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_0_5V);
//		WriteDataRegisterToAD5422 (65534);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
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
//		WriteDataRegisterToAD5422 (real_weight*65534);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
//	}
//	else if(Modbus_weight.weight_long<0)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
//		WriteDataRegisterToAD5422 (0x0001);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
//	}
//	else if(Modbus_weight.weight_long>Cal_full_Mig)
//	{
//		WriteControlRegisterToAD5422 (ANALOG_4_20mA);
//		WriteDataRegisterToAD5422 (65534);	 //ÂúÁ¿³ÌÖµ  Cal_full_Mig
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
				 if(flash_ponter_image>flash_ponter)	   //ÒÆ¶¯
				 flash_ponter_image=1; shift_key_Status=1;
			}

		}
	if((Read_key()==data_incr)&&(programm_setting||calibing))
			{
	 			if(!data_incr_Status) 
				{ 
					temp=++LED_display_buffer[8-flash_ponter_image];  // Êı¾İÔö¼Ó
					 if(temp>'9')//Êı×Ö±ØĞëÎªASCII,ÕâÑù0Ò²ÄÜÏÔÊ¾£¬ÒÔÃâÓëstrlenº¯Êı´ò¼Ü
					 LED_display_buffer[8-flash_ponter_image]='0';
					 if(temp>'9'||temp<'0')//Êı×Ö±ØĞëÎªASCII,ÕâÑù0Ò²ÄÜÏÔÊ¾£¬ÒÔÃâÓëstrlenº¯Êı´ò¼Ü
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
					Cal_Setting_step++;				 //stepping	  Éè¶¨È·ÈÏ
 next_steeep:
					interface_for_setting();
					entering=1;	data_confir_Status=1; return;
				}
		
		
		}


 	if((Read_key()==data_confir)&&calibing)
		{
	 			if(!data_confir_Status&&!entering) 
				{	
					Cal_Setting_step++;				 //stepping	±ê¶¨ È·ÈÏ
					interface_for_calbing();
					entering=1;	 data_confir_Status=1; return;
				}
		}

 	if((Read_key()==data_confir)&&clearing)
		{
	 			if(!data_confir_Status&&!entering) 
				{
					Cal_Setting_step++;				 //stepping	Çå³ı È·ÈÏ
					interface_for_clearing();
					entering=1;	data_confir_Status=1;  return;
				}
		}

/*************************************************************************/

	if((Read_key()==zero_ing))	{ if(flag_borrow_interface_for_setting)return;
	 if(!zero_ing_Status)

									  if(!clearing&&!calibing &&!programm_setting)	   //Éè¶¨ ±ê¶¨ Çå³ı Ê±ÖÃÁãÎŞĞ§
										   {Zero_operation();zero_ing_Status=1;}//¼üÅÌÖÃÁã
									  else
										{ 
										 if(clearing||calibing||programm_setting)
										 {	  
											  if(Cal_Setting_step>1)
											    {  revers=1;//µ¹ÍËĞ´²ÎÊıÎŞĞ§
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
								   Tare_operation();tare_ing_Status=1;//¼üÅÌÈ¥Æ¤
								}

	if((Read_key()==setting)&&!calibing&&!clearing)
			{	
//			Cal_Setting_step=0;		 //Éè¶¨½øÈë
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
					flash_ponter_image=0;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª	//Éè¶¨ÍË³ö
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
						write_LED_Block(Cal,0,LED_LIGHTENX);  //±ê¶¨½øÈë
						Cal_Setting_step=0;
						calibing=1;	calib_Status=1; return;
				}

	 			if(!calib_Status&&calibing) 
				{
					flash_ponter=flash_ponter_image=3;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª	  //±ê¶¨ÍË³ö
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
						write_LED_Block(CLE,0,LED_LIGHTENX);  //Çå³ı½øÈë
						Cal_Setting_step=0;
						clearing=1;	 deletex_Status=1;return;
				}

	 			if(!deletex_Status&&clearing) 
				{
					flash_ponter=flash_ponter_image=3;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª	  //Çå³ıÍË³ö
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
//1.Í¨Ñ¶Êı¾İ×é
//dis_para00(seq_num, ID_budrate,1 );//int ID_budrate;//²ÎÊı²¨ÌØÂÊ¹ØÁªºÅ RAM 
			   			case 1:
								KLTT_AADR=comm_bud_rate-1;
							    dis_para00(Cal_Setting_step, ID_budrate,4 );
						break;
//dis_para00(seq_num, Comm_Addr,1 );//RAM  int Comm_Addr;//²ÎÊıÍ¨Ñ¶µØÖ·¹ØÁªºÅ
			   			case 2:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Comm_Addr,2 );
						break;
//2.ÖÃÁãÊı¾İ×é
//dis_para00(seq_num, Initial_zero_range,1 );//RAM  int Initial_zero_range;//²ÎÊı¿ª»úÖÃÁã·¶Î§
			   			case 3:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Initial_zero_range,2 );
						break;

//dis_para00(seq_num, Munal_zero_range,1 );//RAM  int Munal_zero_range;//²ÎÊıÊÖ¶¯ÖÃÁã·¶Î§
			   			case 4:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Munal_zero_range,4 );
						break;

//dis_para00(seq_num, Trace_zero_range,1 );//RAM  int Trace_zero_range;//²ÎÊıÁã¸ú×Ù·¶Î§
			   			case 5:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Trace_zero_range,3 );
						break;
//3.ÑÓÊ±Êı¾İ×é
//dis_para00(seq_num, Delay_tare,1 );//RAM  int Delay_tare;//²ÎÊıÈ¥Æ¤ÑÓÊ±Êä³ö
			   			case 6:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_tare,2 );
						break;

//dis_para00(seq_num, Delay_Mid_feed,1 );//RAM  int Delay_Mid_feed;//²ÎÊıÖĞ¼ÓÑÓÊ±
			   			case 7:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_Mid_feed,2 );
						break;

//dis_para00(seq_num, Delay_Pre_process,1 );//RAM  int Delay_Pre_process;//²ÎÊıÌáÇ°ÑÓÊ±
			   			case 8:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_Pre_process,2 );
						break;

//dis_para00(seq_num, Delay_Next_loop,1 );//RAM  int Delay_Next_loop;//²ÎÊıÏÂÒ»Ñ­»·ÑÓÊ±
			   			case 9:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_Next_loop,2 );
						break;

//dis_para00(seq_num, Time_supvisor_Slow_feed_up_limt,1 );//RAM  int Time_supvisor_Slow_feed_up_limt;//²ÎÊıÂı¼Ó¼àÊÓÉÏÏŞÊ±¼ä
			   			case 10:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Time_supvisor_Slow_feed_up_limt,2 );
						break;

//dis_para00(seq_num, Time_supvisor_Slow_feed_low_limt,1 );//RAM  int Time_supvisor_Slow_feed_low_limt;//²ÎÊıÂı¼Ó¼àÊÓÏÂÏŞÊ±¼ä
			   			case 11:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Time_supvisor_Slow_feed_low_limt,2 );
						break;

//dis_para00(seq_num, Pulse_howmany_package,1 );//RAM  int Pulse_howmany_package;//²ÎÊı¶àÉÙ´üÊä³öÂö³å
			   			case 12:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Pulse_howmany_package,4 );
						break;

//dis_para00(seq_num, Tare_howmany_package,1 );//RAM  int Tare_howmany_package;//²ÎÊı¶àÉÙ´üÈ¥Æ¤Ò»´Î
			   			case 13:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Tare_howmany_package,4 );
						break;

//dis_para00(seq_num, Recive_x_signals,1 );//RAM  int Recive_x_signals;//²ÎÊıÊÕµ½x´ÎĞÅºÅÉè¶¨
			   			case 14:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Recive_x_signals,2 );	  //½èÓÃ³ÆÖØ¿ØÖÆÍê±Ï£¬ÏÔÊ¾±£³ÖÑÓÊ±
						break;

//dis_para00(seq_num, no_recive_x_signals,1 );//RAM  int no_recive_x_signals;//²ÎÊıÊÕ²»µ½x´ÎĞÅºÅÉè¶¨ÏŞ
			   			case 15:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, no_recive_x_signals,4 );
						break;

//dis_para00(seq_num, Even_howmany_package_Pre_process,1 );//RAM  int Even_howmany_package_Pre_process;//²ÎÊı¶àÉÙ´üÆ½¾ùĞŞÕıÒ»´ÎÌáÇ°Á¿
			   			case 16:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Even_howmany_package_Pre_process,4 );
						break;

//dis_para00(seq_num, Delay_slow_feed,1 );//RAM  int Delay_slow_feed;//²ÎÊıÂı¼ÓÑÓÊ±
			   			case 17:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Delay_slow_feed,2 );
						break;

//dis_para00(seq_num, Delay_initial_feed,1 );//RAM  int Delay_initial_feed;//²ÎÊı³õ¼ÓÑÓÊ±
			   			case 18:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Delay_initial_feed,2 );
						break;
//4.¿ØÖÆÊı¾İ×é   
//dis_para00(seq_num, Slow_feed_modify_uper_limt,1 );//RAM  int Slow_feed_modify_uper_limt;//²ÎÊıÂı¼ÓĞŞÕıÉÏÏŞ
			   			case 19:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Slow_feed_modify_uper_limt,5 );
						break;

//dis_para00(seq_num, Slow_feed_modify_down_limt,1 );//RAM  int Slow_feed_modify_down_limt;//²ÎÊıÂı¼ÓĞŞÕıÏÂÏŞ
			   			case 20:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Slow_feed_modify_down_limt,5 );
						break;

//dis_para00(seq_num, Pre_feed_modify_uper_limt,1 );//RAM  int Pre_feed_modify_uper_limt;//²ÎÊıÌáÇ°Á¿ĞŞÕıÉÏÏŞ
			   			case 21:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Pre_feed_modify_uper_limt,5 );
						break;

//dis_para00(seq_num, Pre_feed_modify_down_limt,1 );//RAM  int Pre_feed_modify_down_limt;//²ÎÊıÌáÇ°Á¿ĞŞÕıÏÂÏŞ
			   			case 22:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Pre_feed_modify_down_limt,5 );
						break;

//dis_para00(seq_num, Slow_feed,1 );//RAM  int Slow_feed;//²ÎÊıÂı¼ÓÁ¿
			   			case 23:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Slow_feed,5 );
						break;

//dis_para00(seq_num, Pre_feed,1 );//RAM  int Pre_feed;//²ÎÊıÌáÇ°Á¿
			   			case 24:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
//err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;	
								if(err_setting_Slow_feed)
								    {write_LED_Block(Err,0,LED_LIGHTENX);err_setting_Slow_feed_timer=30;}  //Cal_Setting_step=0;	flashing=0;	
								else
								    {dis_para00(Cal_Setting_step, Pre_feed,5 );}
						break;

//dis_para00(seq_num, Mid_feed,1 );//RAM  int Mid_feed;//²ÎÊıÖĞ¼ÓÖµ
			   			case 25:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
//err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;	
								if(err_setting_Pre_feed)
								    {write_LED_Block(Err,0,LED_LIGHTENX);err_setting_Slow_feed_timer=30;}  //Cal_Setting_step=0;	flashing=0;	
								else
								    {dis_para00(Cal_Setting_step, Mid_feed,5 );}
						break;

//dis_para00(seq_num, Dlet_error,1 );//RAM  int Dlet_error;//²ÎÊıÔÊ²î
			   			case 26:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Dlet_error,5 );
						break;


//dis_para00(seq_num, Target_uper_limt,1 );//RAM  int Target_uper_limt;//²ÎÊı¶¨Á¿ÉÏÏŞ		 //*2017-10-28am*
			   			case 27: //28:
						        if(!flag_borrow_interface_for_setting)
								{
									KLTT_AADR+=1;
									write_para01(KLTT_AADR,5);
								}
							     dis_para00(Cal_Setting_step, Target_uper_limt,5 );
								 
						break;

//dis_para00(seq_num, Target_down_limt,1 );//RAM  int Target_down_limt;//²ÎÊı¶¨Á¿ÏÂÏŞ	    //*2017-10-28am*
			   			case 28: //29:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Target_down_limt,5 );
						break;
//dis_para00(seq_num, Target,1 );//RAM  int Target;//²ÎÊı¶¨Öµ
			   			case 29: //27:													 //*2017-10-28am*
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Target,5 );
						break;

//dis_para00(seq_num, zero_range,1 );//RAM  int zero_range;//²ÎÊıÁãÇø
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
														flash_ponter_image=0;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª	//Éè¶¨ÍË³ö
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

//dis_para00(seq_num, Slow_feed_modify,1 );//RAM  int Slow_feed_modify;//²ÎÊıÂı¼ÓĞŞÕıÁ¿
			   			case 31:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Slow_feed_modify,5 );
						break;

//dis_para00(seq_num, Initial_power_value,1 );//RAM  int Initial_power_value;//²ÎÊı³õÉÏµçÖµ
			   			case 32:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Initial_power_value,1 );
						break;

//dis_para00(seq_num, Initial_feed_value,1 );//RAM  int Initial_feed_value;//²ÎÊı³õ¼ÓÁ¿
			   			case 33:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Initial_feed_value,5 );
						break;

//dis_para00(seq_num, Random_setting_value,1 );//RAM  int Random_setting_value;//²ÎÊıËæ»úÉè¶¨Öµ
			   			case 34:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, Random_setting_value,5 );
						break;
//5.ĞŞÕıÊı¾İ×é
//dis_para00(seq_num, ID_user_cal,1 );//RAM  int ID_user_cal ;//²ÎÊıÓÃ»§±ê¶¨¹ØÁªºÅ
			   			case 35:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,5);
							    dis_para00(Cal_Setting_step, ID_user_cal,2 );
						break;
//dis_para00(seq_num, ID_slow_feed_modify,1 );//RAM  int ID_slow_feed_modify ;//²ÎÊıÂı¼ÓĞŞÕı±ê¼Ç¹ØÁªºÅ
			   			case 36:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, ID_slow_feed_modify,1 );
						break;
//dis_para00(seq_num, ID_pre_feed_modify,1 );//RAM  int ID_pre_feed_modify;//²ÎÊıÌáÇ°ĞŞÕı±ê¼Ç¹ØÁªºÅ
			   			case 37:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, ID_pre_feed_modify,1 );
						break;
//dis_para00(seq_num, ID_run_mode,1 );//RAM  int ID_run_mode;//²ÎÊıÔËĞĞÄ£Ê½¹ØÁªºÅ
			   			case 38:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, ID_run_mode,1 );
								if(ID_run_mode)
									Signal_lamp[3]=0x02;
								else
									Signal_lamp[3]=0x0;
						break;
//8.ÂË²¨Êı¾İ×é£º
//dis_para00(seq_num, Range_Filter0,4 );//RAM  int Range_Filter0;//²ÎÊıÂË²¨·¶Î§0
			   			case 39:
								KLTT_AADR=ID_run_mode_address;//0x608;
								write_para01(KLTT_AADR,4);
							    dis_para00(Cal_Setting_step, Range_Filter0,2 );
						break;

//dis_para00(seq_num, Range_Filte1r,4 );//RAM  int Range_Filter1;//²ÎÊıÂË²¨·¶Î§1
			   			case 40:
								KLTT_AADR=Range_Filter0_address;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Range_Filter1,2 );
						break;

//dis_para00(seq_num, Range_Filter2,4 );//RAM  int Range_Filter2;//²ÎÊıÂË²¨·¶Î§2
			   			case 41:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Range_Filter2,2 );
						break;

//dis_para00(seq_num, Range_Filter3,4 );//RAM  int Range_Filter3;//²ÎÊıÂË²¨·¶Î§3
			   			case 42:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Range_Filter3,2 );
						break;

//dis_para00(seq_num, Range_Filter4,4 );//RAM  int Range_Filter4;//²ÎÊıÂË²¨·¶Î§4
			   			case 43:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,2);
							    dis_para00(Cal_Setting_step, Range_Filter4,2 );
						break;

//dis_para00(seq_num, Strong_Filter0,2 );//RAM  int Strong_Filter0;//²ÎÊıÂË²¨Ç¿¶È0
			   			case 44:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter0,1 );
						break;

//dis_para00(seq_num, Strong_Filter1,2 );//RAM  int Strong_Filter1;//²ÎÊıÂË²¨Ç¿¶È1
			   			case 45:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter1,1);
						break;

//dis_para00(seq_num, Strong_Filter2,2 );//RAM  int Strong_Filter2;//²ÎÊıÂË²¨Ç¿¶È2
			   			case 46:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter2,1 );
						break;

//dis_para00(seq_num, Strong_Filter3,2 );//RAM  int Strong_Filter3;//²ÎÊıÂË²¨Ç¿¶È3
			   			case 47:
								KLTT_AADR+=1;
								write_para01(KLTT_AADR,1);
							    dis_para00(Cal_Setting_step, Strong_Filter3,1 );
						break;

//dis_para00(seq_num, Strong_Filter4,2 );//RAM  int Strong_Filter4;//²ÎÊıÂË²¨Ç¿¶È4
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
//								seq_num=Cal_Setting_step;//Ğ¡ÆÁÄ»Éè¶¨/±ê¶¨µÄ²½ÖèĞòºÅ
//							    dis_para00(seq_num, ID_budrate,4 );//ppp²ÎÊıµÄRAMµØÖ·  ²ÎÊı²¨ÌØÂÊ¹ØÁªºÅ
//						break;
//					case 2:
//								seq_num=Cal_Setting_step;
//								write_para01(comm_bud_rate,4);//xxx:ÎªÊ¹ÓÃUSART1ÖĞµÄ³ÌĞò¶ÔÓ¦µÄÀ¥ÂØÍ¨Ì©µÄµØÖ·
//							    dis_para00(seq_num, Comm_Addr,1 );//ppp²ÎÊıµÄRAMµØÖ·	²ÎÊıÍ¨Ñ¶µØÖ·¹ØÁªºÅ
//						break;
// 					case 3:
//								seq_num=Cal_Setting_step;
//								write_para01(comm_address,4); //xxx:ÎªÊ¹ÓÃUSART1ÖĞµÄ³ÌĞò¶ÔÓ¦µÄÀ¥ÂØÍ¨Ì©µÄµØÖ·
//							    dis_para00(seq_num, ppp,1 ); //ppp²ÎÊıµÄRAMµØÖ· ²ÎÊı¿ª»úÖÃÁã·¶Î§
//						break;
// 					case 4:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:ÎªÊ¹ÓÃUSART1ÖĞµÄ³ÌĞò¶ÔÓ¦µÄÀ¥ÂØÍ¨Ì©µÄµØÖ·
//							    dis_para00(seq_num, ppp,1 );//ppp²ÎÊıµÄRAMµØÖ·
//						break;
// 					case 5:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:ÎªÊ¹ÓÃUSART1ÖĞµÄ³ÌĞò¶ÔÓ¦µÄÀ¥ÂØÍ¨Ì©µÄµØÖ·
//							    dis_para00(seq_num, ppp,1 );//ppp²ÎÊıµÄRAMµØÖ·
//						break;
// 					case 6:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:ÎªÊ¹ÓÃUSART1ÖĞµÄ³ÌĞò¶ÔÓ¦µÄÀ¥ÂØÍ¨Ì©µÄµØÖ·
//							    dis_para00(seq_num, ppp,1 );//ppp²ÎÊıµÄRAMµØÖ·
//						break;
// 					case 7:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:ÎªÊ¹ÓÃUSART1ÖĞµÄ³ÌĞò¶ÔÓ¦µÄÀ¥ÂØÍ¨Ì©µÄµØÖ·
//							    dis_para00(seq_num, ppp,1 );//ppp²ÎÊıµÄRAMµØÖ·
//						break;
// 					case 8:
//								seq_num=Cal_Setting_step;
//								write_para01(xxx,4);//xxx:ÎªÊ¹ÓÃUSART1ÖĞµÄ³ÌĞò¶ÔÓ¦µÄÀ¥ÂØÍ¨Ì©µÄµØÖ·
//							    dis_para00(seq_num, ppp,1 );//ppp²ÎÊıµÄRAMµØÖ·
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
 			   /******************±ê¶¨************************/
			   			case 1:
							    dis_para01(P, Cal_point,1 );	//ÏÔÊ¾Ğ¡Êıµã
						break;

			   			case 2:
								KLTT_AADR=Cal_pint_address;  //±£´æĞ¡Êıµã
								write_para01(KLTT_AADR,1);
							    dis_para01(0x0d, Cal_ID_division,2 );//ÏÔÊ¾·Ö¶ÈÊı
						break;

			   			case 3:
								KLTT_AADR=Cal_ID_division_address;  //Cal_pint_address  = 1562//±£´æ·Ö¶ÈÊı
								write_para01(KLTT_AADR,2);
							    dis_para01(0x0f, Cal_full_Mig,5 );//ÏÔÊ¾ÂúÁ¿³ÌÖµ
						break;

			   			case 4:
								KLTT_AADR=Cal_full_Mig_address;  //Cal_pint_address  = 1562//±£´æÂúÁ¿³ÌÖµ
								write_para01(KLTT_AADR,5);
							    dis_para01(0x0a, Analog_mode_SEL,1 );//ÏÔÊ¾Ä£ÄâÁ¿Ñ¡Ôñ
						break;

			   			case 5:
								KLTT_AADR=Analog_mode_SEL_address;  //±£´æÄ£ÄâÁ¿Ñ¡Ôñ
								write_para01(KLTT_AADR,1);
							    dis_para01(0x0c, (int)Modbus_pulse.target_totall_pulse,5 );//ÏÔÊ¾²ÎÊı¶¨Á¿µÄ×ÜÂö³åÊı

//					Modbus_Counter++;//²ÎÊı¶¨Á¿µÄ×ÜÂö³åÊı
//					if(Modbus_Address==1503) //1003:ModbusµØÖ·	 1003  FF_Target_pulse
//					{
//						Modbus_pulse.pulse[3]=0;
//						Modbus_pulse.pulse[2]=0;
//						Modbus_pulse.pulse[1]=Modbus_0x10_Value[0];
//						Modbus_pulse.pulse[0]=Modbus_0x10_Value[1];
//						STMFLASH_Write(FF_Target_pulse,(u16*)&Modbus_pulse.target_totall_pulse,4);	 Cal_user_weight
//					}

						break;

			   			case 6:
								KLTT_AADR=1503;  //±£´æ²ÎÊı¶¨Á¿µÄ×ÜÂö³åÊı
								write_para01(KLTT_AADR,5);
							    dis_para01(U, (int)Cal_user_weight,5 );//ÏÔÊ¾±ê¶¨ÓÃ»§íÀÂëÖØÁ¿
						break;

			   			case 7:
								KLTT_AADR=Cal_user_weight_address;  //±£´æ±ê¶¨ÓÃ»§íÀÂëÖØÁ¿
								write_para01(KLTT_AADR,5);
						//ÁãÎ»
							  flashing=0;	 
							  write_LED_Block(noLOAd,0,LED_LIGHTENX);
						break;

	 					case 8:	  // ¼Ó·¨Âë
						      //±£´æÁãÎ»
							  zero_point=AD_Result0; 
							  flashing=0;Cal_output;//*2017-10-28am*	 
							  write_LED_Block(ADLOAd,0,LED_LIGHTENX);
						break;
	 					case 9:	  //ÊäÈëÖØÁ¿ Ê×ÏÈÏÔÊ¾ÓÃ»§íÀÂëÖØÁ¿

								dis_para01(U, (int)Cal_user_weight,5 );//ÏÔÊ¾±ê¶¨ÓÃ»§íÀÂëÖØÁ¿
								ad_coefficent=((float)(AD_Result0-zero_point))/(Cal_weight/temp__division);	//±ê¶¨íÀÂëÖØÁ¿//
//								Un_Cal_output;//*2017-10-28am*	 								flash_ponter=flash_ponter_image=5;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
//								combinate_letter_digtal(Ht,digtal_w);
//								flashing=1;	 original=LED_display_buffer;	//²»ÄÜÁ¬Ğø¿ìËÙĞ´	 write_LED_Block £¬·ñÔò»áÒ»ÆğLEDÂÒÌø

						break;

	  					case 10:	  //Ğ´±ê¶¨íÀÂëÖØÁ¿£¬¼ÆËã±êÂÊ
//ÎªÁË¿ØÖÆ12v¼¤Àø£¬ÁéÃô¶È=2mv/V,·Ö¶ÈÊı=1£¬È«³Ì20000·Ö¶È£¬ÏµÊı²»ÄÜ<47.855.
//Ñ¡ÏµÊı×îĞ¡=45
								temp__division=select_division();
								KLTT_AADR=Cal_weight_address;  //±£´æ±ê¶¨ÓÃ»§íÀÂëÖØÁ¿
								write_para01(KLTT_AADR,5);
								ad_coefficent=((float)(AD_Result0-zero_point))/(Cal_weight/temp__division);	//±ê¶¨íÀÂëÖØÁ¿//								flash_ponter=flash_ponter_image=5;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
								Un_Cal_output;//*2017-10-28am*	 								flash_ponter=flash_ponter_image=5;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
//								if(ad_coefficent<45)
//						    	    {ad_coefficent=1;//cal_finished=0;
//									 flashing=0;write_LED_Block(Err,0,LED_LIGHTENX);//	·´À¡±ê¶¨³ö´í£¬µ¯³öĞÅÏ¢¿ò
//									}//·µ»Øºó²»±£´æÏµÊı£¬ÁãÎ»
//							    else
//									{
									  ad_coefficent/=temp__division;
										AD_ZERO_STORE.zero_point=zero_point;
										STMFLASH_Write(AD_ZEROX,(u16*)AD_ZERO_STORE.zero_pointx,4);//±£´æÁãÎ»
										STMFLASH_Read (AD_ZEROX,(u16*)AD_ZERO_STORE.zero_pointx,4);
										
										AD_COFF_STORE.ad_coefficent=ad_coefficent;
										STMFLASH_Write(AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);//±£´æÏµÊı
										STMFLASH_Read (AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);

//										 _ltoa(Dis_ad,ad_coefficent*10000);	
										 
//										 display_main_weight(); 
//									 flashing=0;write_LED_Block(good,0,LED_LIGHTENX);

//									 flashing=0;write_LED_Block(Dis_ad,0,LED_LIGHTENX);
							         dis_para00((int)P1, ad_coefficent*10000,6 );
//									}//·µ»Øºó±£´æÏµÊı£¬ÁãÎ»	   _ltoa(Dis_ad,ad_coefficent);	display_main_weight(); //ÏÔÊ¾AD_CODE

						break;
						case 11:
									borrow_write_para01_to_get_coeff=1;
									write_para01(KLTT_AADR,1);	//get coeffcirnt on the interface
								    ad_coefficent/=10000;
									AD_COFF_STORE.ad_coefficent=ad_coefficent;
									STMFLASH_Write(AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);//±£´æÏµÊı
									STMFLASH_Read (AD_COFFX,(u16*)AD_COFF_STORE.ad_coeffx,4);

									flashing=0;write_LED_Block(Dis_ad,0,LED_LIGHTENX);

						break;





//					case 1://Ğ¡Êıµã
//								flash_ponter=flash_ponter_image=1;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
//								combinate_letter_digtal(P1,digtal_p);
//								flashing=1;	 original=LED_display_buffer;	//²»ÄÜÁ¬Ğø¿ìËÙĞ´	 write_LED_Block £¬·ñÔò»áÒ»ÆğLEDÂÒÌø
//						break;
//
//					case 2:	//·Ö¶ÈÊı
//								flash_ponter=flash_ponter_image=2;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
//								combinate_letter_digtal(D,digtal_d);
//								flashing=1;	 original=LED_display_buffer;	//²»ÄÜÁ¬Ğø¿ìËÙĞ´	 write_LED_Block £¬·ñÔò»áÒ»ÆğLEDÂÒÌø
//						break;
//
//					case 3:	//ÂúÁ¿³Ì
//								flash_ponter=flash_ponter_image=5;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
//								combinate_letter_digtal(F,digtal_f);
//								flashing=1;	 original=LED_display_buffer;	//²»ÄÜÁ¬Ğø¿ìËÙĞ´	 write_LED_Block £¬·ñÔò»áÒ»ÆğLEDÂÒÌø
//						break;
//					case 4:	//ÁãÎ»
//							  flashing=0;	 
//							  write_LED_Block(noLOAd,0,LED_LIGHTENX);
//
//
//						break;
//
//
// 					case 5:	  // ¼Ó·¨Âë
//							  flashing=0;	 
//							  write_LED_Block(ADLOAd,0,LED_LIGHTENX);
//						break;
// 					case 6:	  //ÊäÈëÖØÁ¿
//								flash_ponter=flash_ponter_image=5;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
//								combinate_letter_digtal(Ht,digtal_w);
//								flashing=1;	 original=LED_display_buffer;	//²»ÄÜÁ¬Ğø¿ìËÙĞ´	 write_LED_Block £¬·ñÔò»áÒ»ÆğLEDÂÒÌø
//
//						break;
//
//  					case 7:	  //¼ÆËã±êÂÊ
//						break;


					default:
						break;
				}
//		}
 }

 /******************Çå³ı************************/

  void interface_for_clearing(void)
  {
  			switch (Cal_Setting_step)
				{
 			   /******************Çå³ı************************/
			   			case 1:
							    dis_para02(Cal_Setting_step, Weighting_T,5 );	//ÏÔÊ¾Ğ¡¶ÖÎ»
						break;
  			   			case 2:
							    dis_para02(Cal_Setting_step, Weighting_package,5 );	//ÏÔÊ¾´üÊı
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
////// 									  if(clearing||calibing||programm_setting)return;	   //Éè¶¨ ±ê¶¨ Çå³ı Ê±ÎŞĞ§
////
////
////;
//
//  			switch (Cal_Setting_step)
//				{
// 			   /******************Çå³ı************************/
//
//
//
////dis_para00(seq_num, Target_down_limt,1 );//RAM  int Target_down_limt;//²ÎÊı¶¨Á¿ÉÏÏŞ	    //*2017-10-28am*  28
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
// //dis_para00(seq_num, Target_uper_limt,1 );//RAM  int Target_uper_limt;//²ÎÊı¶¨Á¿ÏÂÏŞ		 //*2017-10-28am* 27
//			   			case 3: //28:
//								KLTT_AADR+=1;
//								write_para01(KLTT_AADR,5);
//							    dis_para00(Cal_Setting_step, Target_down_limt,5 );
//						break;
////dis_para00(seq_num, Target,1 );//RAM  int Target;//²ÎÊı¶¨Öµ
//			   			case 4: //27:													 //*2017-10-28am*	  29
//								KLTT_AADR+=1;
//								write_para01(KLTT_AADR,5);
//							    dis_para00(Cal_Setting_step, Target,5 );
//						break;
//
//			   			case 5:
//								KLTT_AADR+=1;
//								write_para01(KLTT_AADR,5);
//							    dis_para02(Cal_Setting_step, Weighting_T,5 );	//ÏÔÊ¾Ğ¡¶ÖÎ»
//						break;
//  			   			case 6:
//							    dis_para02(Cal_Setting_step, Weighting_package,5 );	//ÏÔÊ¾´üÊı
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

								flash_ponter=flash_ponter_image=_image_ponter;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
								flashing=1;	 original=LED_display_buffer;	//²»ÄÜÁ¬Ğø¿ìËÙĞ´	 write_LED_Block £¬·ñÔò»áÒ»ÆğLEDÂÒÌø
 
 }
 	  
 void dis_para00(int seq_num,int para,char _image_ponter)
 {
    unsigned char seq_x[10];
	seq_x[0]=P;

								_ltoa(Dis_ad,(long)seq_num);
								seq_x[1]=Dis_ad[0];	seq_x[2]=Dis_ad[1];seq_x[3]=0;
								_ltoa(Dis_ad,(long)para);
								combinate_letter_digtal(seq_x,Dis_ad);

								flash_ponter=flash_ponter_image=_image_ponter;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
								flashing=1;	 original=LED_display_buffer;	//²»ÄÜÁ¬Ğø¿ìËÙĞ´	 write_LED_Block £¬·ñÔò»áÒ»ÆğLEDÂÒÌø
 
 }
void dis_para01(unsigned char seq_num,int para,char _image_ponter)
 {
    unsigned char seq_x[10];
	seq_x[1]=0;seq_x[0]=seq_num;	
								_ltoa(Dis_ad,(long)para);
								combinate_letter_digtal(seq_x,Dis_ad);

								flash_ponter=flash_ponter_image=_image_ponter;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª
								flashing=1;	 original=LED_display_buffer;	//²»ÄÜÁ¬Ğø¿ìËÙĞ´	 write_LED_Block £¬·ñÔò»áÒ»ÆğLEDÂÒÌø
 }
 void write_para01(int para,char _image_ponter)
 {
 char i;
 long trans_buffer;
 unsigned char *p;  char data_buffer[10],*q;
 long para0;
// 									  if(clearing||calibing||programm_setting)return;	   //Éè¶¨ ±ê¶¨ Çå³ı Ê±ÎŞĞ§


 if(revers){revers=0;return;}//Ç°½øĞ´²ÎÊıÓĞĞ§}	   //Éè¶¨ ±ê¶¨ Çå³ı Ê±µ¹ÍËÎŞĞ§
 p=LED_display_buffer+8-flash_ponter;
 q=data_buffer;

 	for(i=0;i<flash_ponter;i++)
	{
	  if(!(*p<0x30||*p>0x39))
	  *q++=*p++;
	  else
	   *p++;//ÔÚ²»×ã¹æ¶¨Êı¾İ³¤¶ÈÊ±£¬Ìø¿ª Ç°Ãæ·¢°µÎ»
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
			

	Modbus_0x10_Value[0]=para0>>8;//²ÎÊı²¨ÌØÂÊ¹ØÁªºÅ
	Modbus_0x10_Value[1]=para0&0x00ff;

	Modbus_Address=para;	 //store data//²ÎÊı²¨ÌØÂÊ¹ØÁªºÅ
	Modbus_Counter=9;
	wang_USART1_Process();


 }
//.\usart1.c(226): extern int image_Pre_feed,system_indicator;
 void signal_convert_to_LED (void)
{
//  				if(Feed_signal_buffer[1]&0x08) //ÎÈ¶¨	
//				{Signal_lamp[2]=0x02;}
//				else
//				{Signal_lamp[2]=0x00;}



	if(state_operation<0) //	ÍË³ö¿ØÖÆ³ÌĞò£¬LEDµÆÎªºìµÆ	 
	{
	
//				Signal_lamp[4]=0x00;   Signal_lamp[5]=0x00; 
//				Signal_lamp[6]=0x00;   Signal_lamp[7]=0x00;
//err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;	

//Signal_lamp[4]=0x02; Signal_lamp[5]=0x02;	Signal_lamp[6]=0x02;
//  Signal_lamp[4]=0x02;

  				if(Feed_signal_buffer[1]&0x08) //ÎÈ¶¨	
				{Signal_lamp[2]=0x02;}
				else
				{Signal_lamp[2]=0x00;}

				if(ID_run_mode) //×Ô¶¯	
				{Signal_lamp[3]=0x02;}
				else
				{Signal_lamp[3]=0x00;}



//				if(Feed_signal_buffer[1]&0x80) //È¥Æ¤	
//				{Signal_lamp[0]=0x02;}	// ºìµÆ
//				else
//				{Signal_lamp[0]=0x00;}
				
				if(Feed_signal_buffer[1]&0x40) //ÁãÎ»	
				{Signal_lamp[1]=0x02;}
				else
				{Signal_lamp[1]=0x00;}
				
				Signal_lamp[6]=0x00;	
//				if(Feed_signal_buffer[1]&0x08) //ÎÈ¶¨	
//				{Signal_lamp[2]=0x02;}
//				else
//				{Signal_lamp[2]=0x00;}
				
//				if(ID_run_mode) //×Ô¶¯	
//				{Signal_lamp[3]=0x02;}
//				else
//				{Signal_lamp[3]=0x00;}

//				if(err_setting_Slow_feed==1) //	
//				{Signal_lamp[4]=0x02;}	// ºìµÆ
//				else
//				{Signal_lamp[4]=0x00;}
//
//				if(err_setting_Pre_feed==1) //	
//				{Signal_lamp[5]=0x02;}	// ºìµÆ
//				else
//				{Signal_lamp[5]=0x00;}
//
//				if(err_setting_Target==1) //	
//				{Signal_lamp[6]=0x02;}	// ºìµÆ
//				else
//				{Signal_lamp[6]=0x00;}
	}
	else //½øÈë¿ØÖÆ³ÌĞò£¬LEDµÆÎªÂÌµÆ
	{
	
					
	
//					if(system_indicator==2) //È¥Æ¤Êä³ö	
//					{Signal_lamp[0]=0x01;}	// ÂÌµÆ
//					else
//					{Signal_lamp[0]=0x00;}


//					if(system_indicator==3) //È¥Æ¤Íê³É	
//					{Signal_lamp[1]=0x01;}	// ÂÌµÆ
	
//					if(system_indicator==4) //³õ¼Ó	
//					{Signal_lamp[2]=0x01;}	// ÂÌµÆ
//	
//					if(system_indicator==5) //ÖĞ¼Ó	
//					{Signal_lamp[3]=0x01;}	// ÂÌµÆ
//	
//					if(system_indicator==6) //Âı¼Ó	
//					{Signal_lamp[4]=0x01;}	// ÂÌµÆ
//	
//					if(system_indicator==7) //ÌáÇ°	
//					{Signal_lamp[5]=0x01;}	// ÂÌµÆ
	
//					if(system_indicator==8) //ÁãÎ»	
					if (Modbus_weight.weight_long<=zero_range) 
					   Signal_lamp[6]=0x01;	// ÂÌµÆ
					else
					   Signal_lamp[6]=0x00;	
	
//					if(system_indicator==10) //ºÏ¸ñ	
//					{Signal_lamp[7]=0x01;}	// ÂÌµÆ
//	
////					if(system_indicator==9) //³¬²î	
////					{Signal_lamp[4]=0x02;}	// ºìµÆ
//
//					if(system_indicator==9) //³¬²î	
//					{Signal_lamp[1]=0x01;}	// ºìµÆ
//	
////					if(system_indicator==1) //Ç·²î	
////					{Signal_lamp[5]=0x02;}	// ºìµÆ
//
//					if(system_indicator==1) //Ç·²î	
//					{Signal_lamp[0]=0x01;}	// ºìµÆ

	
	}
}
void display_main_weight(void)
{
if(!clearing&&!calibing &&!programm_setting)	   //ÖØÁ¿ÏÔÊ¾Ìõ¼ş
   {flashing=0; 
//      write_LED_Block(scalb,Cal_point,Signal_lamp);
//       Delay(500);  
		Modify_Dis_ad();
	   write_LED_Block(Dis_ad,Cal_point,Signal_lamp);
   }
else
			original=LED_display_buffer; //Õâ¾ä±ØĞë
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
						//ÁãÎ»
							  flashing=0;	 
							  write_LED_Block(noLOAd,0,Signal_lamp);
						break;

	 					case 8:	  // ¼Ó·¨Âë
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
//1.Í¨Ñ¶Êı¾İ×é
				//dis_para00(seq_num, ID_budrate,1 );//int ID_budrate;//²ÎÊı²¨ÌØÂÊ¹ØÁªºÅ RAM 
//				case 01:
				 ID_budrate=9600;
				//dis_para00(seq_num, Comm_Addr,1 );//RAM  int Comm_Addr;//²ÎÊıÍ¨Ñ¶µØÖ·¹ØÁªºÅ
//				case 02:
				 Comm_Addr=2;
				//2.ÖÃÁãÊı¾İ×é
				//dis_para00(seq_num, Initial_zero_range,1 );//RAM  int Initial_zero_range;//²ÎÊı¿ª»úÖÃÁã·¶Î§
//				cas 03:
				 Initial_zero_range=30;
				//dis_para00(seq_num, Munal_zero_range,1 );//RAM  int Munal_zero_range;//²ÎÊıÊÖ¶¯ÖÃÁã·¶Î§
//				cas 04:
				 Munal_zero_range=1000;
				//dis_para00(seq_num, Trace_zero_range,1 );//RAM  int Trace_zero_range;//²ÎÊıÁã¸ú×Ù·¶Î§
//				cas 05:
				 Trace_zero_range=90;
				//3.ÑÓÊ±Êı¾İ×é
				//dis_para00(seq_num, Delay_tare,1 );//RAM  int Delay_tare;//²ÎÊıÈ¥Æ¤ÑÓÊ±Êä³ö
//				cas 06:
				 Delay_tare=40;
				//dis_para00(seq_num, Delay_Mid_feed,1 );//RAM  int Delay_Mid_feed;//²ÎÊıÖĞ¼ÓÑÓÊ±
//				cas 07:
				 Delay_Mid_feed=0;
				//dis_para00(seq_num, Delay_Pre_process,1 );//RAM  int Delay_Pre_process;//²ÎÊıÌáÇ°ÑÓÊ±
//				cas 08:
				 Delay_Pre_process=5;
				//dis_para00(seq_num, Delay_Next_loop,1 );//RAM  int Delay_Next_loop;//²ÎÊıÏÂÒ»Ñ­»·ÑÓÊ±
//				cas 09:
				 Delay_Next_loop=10;
				//dis_para00(seq_num, Time_supvisor_Slow_feed_up_limt,1 );//RAM  int Time_supvisor_Slow_feed_up_limt;//²ÎÊıÂı¼Ó¼àÊÓÉÏÏŞÊ±¼ä
//				cas 10:
				 Time_supvisor_Slow_feed_up_limt=40;
				//dis_para00(seq_num, Time_supvisor_Slow_feed_low_limt,1 );//RAM  int Time_supvisor_Slow_feed_low_limt;//²ÎÊıÂı¼Ó¼àÊÓÏÂÏŞÊ±¼ä
//				cas 11:
				 Time_supvisor_Slow_feed_low_limt=20;
				//dis_para00(seq_num, Pulse_howmany_package,1 );//RAM  int Pulse_howmany_package;//²ÎÊı¶àÉÙ´üÊä³öÂö³å
//				cas 12:
				 Pulse_howmany_package=100;
				//dis_para00(seq_num, Tare_howmany_package,1 );//RAM  int Tare_howmany_package;//²ÎÊı¶àÉÙ´üÈ¥Æ¤Ò»´Î
//				cas 13:
				 Tare_howmany_package=100;
				//dis_para00(seq_num, Recive_x_signals,1 );//RAM  int Recive_x_signals;//²ÎÊıÊÕµ½x´ÎĞÅºÅÉè¶¨
//				cas 14:
				 Recive_x_signals=0;	  //½èÓÃ³ÆÖØ¿ØÖÆÍê±Ï£¬ÏÔÊ¾±£³ÖÑÓÊ±
				//dis_para00(seq_num, no_recive_x_signals,1 );//RAM  int no_recive_x_signals;//²ÎÊıÊÕ²»µ½x´ÎĞÅºÅÉè¶¨ÏŞ
//				cas 15:
				 no_recive_x_signals=10;
				//dis_para00(seq_num, Even_howmany_package_Pre_process,1 );//RAM  int Even_howmany_package_Pre_process;//²ÎÊı¶àÉÙ´üÆ½¾ùĞŞÕıÒ»´ÎÌáÇ°Á¿
//				cas 16:
				 Even_howmany_package_Pre_process=100;
				//dis_para00(seq_num, Delay_slow_feed,1 );//RAM  int Delay_slow_feed;//²ÎÊıÂı¼ÓÑÓÊ±
//				cas 17:
				 Delay_slow_feed=15;
				//dis_para00(seq_num, Delay_initial_feed,1 );//RAM  int Delay_initial_feed;//²ÎÊı³õ¼ÓÑÓÊ±
//				cas 18:
				 Delay_initial_feed=0;
				//4.¿ØÖÆÊı¾İ×é   
				//dis_para00(seq_num, Slow_feed_modify_uper_limt,1 );//RAM  int Slow_feed_modify_uper_limt;//²ÎÊıÂı¼ÓĞŞÕıÉÏÏŞ
//				cas 19:
				 Slow_feed_modify_uper_limt=600;
				//dis_para00(seq_num, Slow_feed_modify_down_limt,1 );//RAM  int Slow_feed_modify_down_limt;//²ÎÊıÂı¼ÓĞŞÕıÏÂÏŞ
//				cas 20:
				 Slow_feed_modify_down_limt=400;
				//dis_para00(seq_num, Pre_feed_modify_uper_limt,1 );//RAM  int Pre_feed_modify_uper_limt;//²ÎÊıÌáÇ°Á¿ĞŞÕıÉÏÏŞ
//				cas 21:
				 Pre_feed_modify_uper_limt=10;
				//dis_para00(seq_num, Pre_feed_modify_down_limt,1 );//RAM  int Pre_feed_modify_down_limt;//²ÎÊıÌáÇ°Á¿ĞŞÕıÏÂÏŞ
//				cas 22:
				 Pre_feed_modify_down_limt=3;
				//dis_para00(seq_num, Slow_feed,1 );//RAM  int Slow_feed;//²ÎÊıÂı¼ÓÁ¿
//				cas 23:
				 Slow_feed=500;
				//dis_para00(seq_num, Pre_feed,1 );//RAM  int Pre_feed;//²ÎÊıÌáÇ°Á¿
//				cas 24:
				 Pre_feed=7;
				//dis_para00(seq_num, Mid_feed,1 );//RAM  int Mid_feed;//²ÎÊıÖĞ¼ÓÖµ
//				cas 25:
				 Mid_feed=2500;
				//dis_para00(seq_num, Dlet_error,1 );//RAM  int Dlet_error;//²ÎÊıÔÊ²î
//				cas 26:
				 Dlet_error=10;
				//dis_para00(seq_num, Target_uper_limt,1 );//RAM  int Target_uper_limt;//²ÎÊı¶¨Á¿ÉÏÏŞ		 //*2017-10-28am*
//				cas 27: 
				 Target_uper_limt=5050;
				//dis_para00(seq_num, Target_down_limt,1 );//RAM  int Target_down_limt;//²ÎÊı¶¨Á¿ÏÂÏŞ	    //*2017-10-28am*
//				cas 28: 
				 Target_down_limt=4950;
				//dis_para00(seq_num, Target,1 );//RAM  int Target;//²ÎÊı¶¨Öµ
//				cas 29: :													 //*2017-10-28am*
				 Target=5000;
				//dis_para00(seq_num, zero_range,1 );//RAM  int zero_range;//²ÎÊıÁãÇø
//				cas 30:
				 zero_range=200;
				//dis_para00(seq_num, Slow_feed_modify,1 );//RAM  int Slow_feed_modify;//²ÎÊıÂı¼ÓĞŞÕıÁ¿
//				cas 31:
				 Slow_feed_modify=20;
				//dis_para00(seq_num, Initial_power_value,1 );//RAM  int Initial_power_value;//²ÎÊı³õÉÏµçÖµ
//				cas 32:
				 Initial_power_value=0;
				//dis_para00(seq_num, Initial_feed_value,1 );//RAM  int Initial_feed_value;//²ÎÊı³õ¼ÓÁ¿
//				cas 33:
				 Initial_feed_value=3000;
				//dis_para00(seq_num, Random_setting_value,1 );//RAM  int Random_setting_value;//²ÎÊıËæ»úÉè¶¨Öµ
//				cas 34:
				 Random_setting_value=0;
				//5.ĞŞÕıÊı¾İ×é
				//dis_para00(seq_num, ID_user_cal,1 );//RAM  int ID_user_cal ;//²ÎÊıÓÃ»§±ê¶¨¹ØÁªºÅ
//				cas 35:
				 ID_user_cal=0;
				//dis_para00(seq_num, ID_slow_feed_modify,1 );//RAM  int ID_slow_feed_modify ;//²ÎÊıÂı¼ÓĞŞÕı±ê¼Ç¹ØÁªºÅ
//				cas 36:
				 ID_slow_feed_modify=0;
				//dis_para00(seq_num, ID_pre_feed_modify,1 );//RAM  int ID_pre_feed_modify;//²ÎÊıÌáÇ°ĞŞÕı±ê¼Ç¹ØÁªºÅ
//				cas 37:
				 ID_pre_feed_modify=0;
				//dis_para00(seq_num, ID_run_mode,1 );//RAM  int ID_run_mode;//²ÎÊıÔËĞĞÄ£Ê½¹ØÁªºÅ
//				cas 38:
				 ID_run_mode=0;
				//8.ÂË²¨Êı¾İ×é£º
				//dis_para00(seq_num, Range_Filter0,4 );//RAM  int Range_Filter0;//²ÎÊıÂË²¨·¶Î§0
//				cas 39:
				 Range_Filter0=22;
				//dis_para00(seq_num, Range_Filte1r,4 );//RAM  int Range_Filter1;//²ÎÊıÂË²¨·¶Î§1
//				cas 40:
				 Range_Filter1=22;
				//dis_para00(seq_num, Range_Filter2,4 );//RAM  int Range_Filter2;//²ÎÊıÂË²¨·¶Î§2
//				cas 41:
				 Range_Filter2=22;
				//dis_para00(seq_num, Range_Filter3,4 );//RAM  int Range_Filter3;//²ÎÊıÂË²¨·¶Î§3
//				cas 42:
				 Range_Filter3=22;
				//dis_para00(seq_num, Range_Filter4,4 );//RAM  int Range_Filter4;//²ÎÊıÂË²¨·¶Î§4
//				cas 43:
				 Range_Filter4=22;
				//dis_para00(seq_num, Strong_Filter0,2 );//RAM  int Strong_Filter0;//²ÎÊıÂË²¨Ç¿¶È0
//				cas 44:
				 Strong_Filter0=2;
				//dis_para00(seq_num, Strong_Filter1,2 );//RAM  int Strong_Filter1;//²ÎÊıÂË²¨Ç¿¶È1
//				cas 45:
				 Strong_Filter1=2;
				//dis_para00(seq_num, Strong_Filter2,2=;//RAM  int Strong_Filter2;//²ÎÊıÂË²¨Ç¿¶È2
//				cas 46:
				 Strong_Filter2=2;
				//dis_para00(seq_num, Strong_Filter3,2 );//RAM  int Strong_Filter3;//²ÎÊıÂË²¨Ç¿¶È3
//				cas 47:
				 Strong_Filter3=2;
				//dis_para00(seq_num, Strong_Filter4,2 );//RAM  int Strong_Filter4;//²ÎÊıÂË²¨Ç¿¶È4
//				cas 48:
				 Strong_Filter4=2;
				//return start
				Cal_Setting_step=0;	  	programm_setting=0;


//														  				flash_ponter=0;
//														flash_ponter_image=0;//³õÊ¼ÉÁË¸Î»ÖÃºÜÖØÒª	//Éè¶¨ÍË³ö
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
	 Cal_point=2;//±ê¶¨Ğ¡Êıµã
	 Cal_ID_division=0;//±ê¶¨·Ö¶ÈÊı¹ØÁªºÅ
	 Cal_full_Mig=50;//±ê¶¨ÂúÁ¿³ÌÖµ
	 Cal_user_weight=5000;//±ê¶¨ÓÃ»§íÀÂëÖØÁ¿
	 //Cal_zero;//±ê¶¨ÁãÎ»
	 //Cal_ratio;//±ê¶¨±êÂÊ
	 Cal_weight=5000;	//±ê¶¨íÀÂëÖØÁ¿
	 Modbus_pulse.target_totall_pulse=18200;
	 //add by John------
}



